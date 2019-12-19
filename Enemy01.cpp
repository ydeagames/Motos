//======================================================
// File Name	: Enemy01.cpp
// Summary		: エネミー
// Date			: 2019/12/18
// Author		: Takafumi Ban
//======================================================
#include "pch.h"
#include "Enemy01.h"
#include "GameContext.h"
#include "DeviceResources.h"
#include <CommonStates.h>
#include "GameWindow.h"
#include "CollisionManager.h"
#include "Camera.h"
#include "Stage.h"
#include "SphereCollider.h"

// 重さ
const float Enemy01::WEIGHT = 0.8f;
// 床との判定用の幅と高さ
const float Enemy01::WIDTH = 0.2f;
const float Enemy01::HEIGHT = 0.2f;
// オブジェクト同士の判定用の半径
const float Enemy01::RADIUS = 0.4f;
// 最大移動速度
const float Enemy01::MAX_SPEED = 0.06f;
// 床に対する摩擦係数
const float Enemy01::FRICTION = 0.1f;
// 思考間隔（単位：秒）
const float Enemy01::THINK_INTERVAL = 0.4f;
Enemy01::Enemy01()
	: m_models{ nullptr }
	, m_state(STATE_NORMAL)
	, m_thinkTimer(0)
{
}

void Enemy01::Initialize(int x, int y)
{
	m_x = x;
	m_y = y;
	m_pos = DirectX::SimpleMath::Vector3((float)x, 0.0f, (float)y);

	// 質量
	m_weight = WEIGHT;

	// 摩擦係数
	m_coefficientOfFriction = FRICTION;

	// 半径
	m_radius = RADIUS;

	// 幅と高さ
	m_width = Enemy01::WIDTH;
	m_height = Enemy01::HEIGHT;

	// 位置と向きをリセット
	Reset();

	// 当たり判定 WIP
	m_collider = std::make_unique<SphereCollider>(this, m_radius);
}

void Enemy01::SetModel(ModelType modelType, DirectX::Model* model)
{
	m_models[modelType] = model;
}

void Enemy01::Update(float elapsedTime)
{
	// 削除リクエストがあればタスクを削除
	if (IsInvalid()) return;

	// アクティブフラグがfalseの場合は何もしない
	if (!m_activeFlag) return;

	// 状態によって処理を分岐させる
	switch (m_state)
	{
	case STATE_NORMAL:	// 通常
		State_Normal(elapsedTime);
		break;
	case STATE_HIT:		// ヒット中
		State_Hit(elapsedTime);
		break;
	case STATE_FALL:	// 落下中
		State_Fall(elapsedTime);
		break;
	default:
		break;
	}

	// 摩擦により速度を落とす
	Friction(elapsedTime);

	// 最大速度以上にならないよう調整
	if (m_vel.LengthSquared() > MAX_SPEED * MAX_SPEED)
	{
		m_vel.Normalize();
		m_vel *= MAX_SPEED;
	}

	// 位置に速度を足す
	m_pos += m_vel;

	// プレイヤーと床のチェック
	if (m_state == Enemy01::STATE_NORMAL && CheckFloor() == false)
	{
		// 落ちたので停止させる
		m_vel = DirectX::SimpleMath::Vector3::Zero;
		// 状態を落下中へ
		m_state = STATE_FALL;
		// 描画順をFALLへ
		SetDrawPrio(GameWindow::DRAW_FALL);
	}
}

void Enemy01::Render()
{
	GameWindow* gameWindow = GameContext::Get<GameWindow>();
	DX::DeviceResources* deviceResources = GameContext::Get<DX::DeviceResources>();
	Camera* camera = gameWindow->GetCamera();

	if (!gameWindow || !m_models[NORMAL] || !m_displayFlag) return;

	// 向いている角度を角度テーブルから取得
	float angle = GameWindow::DIR_ANGLE[m_dir];

	// ワールド行列を作成
	DirectX::SimpleMath::Matrix world = DirectX::SimpleMath::Matrix::CreateRotationY(angle) * DirectX::SimpleMath::Matrix::CreateTranslation(m_pos);

	// モデルの描画（ジャンプパーツを付けているかどうかでモデルが違う）
	m_models[NORMAL]->Draw(deviceResources->GetD3DDeviceContext(),
		*GameContext::Get<DirectX::CommonStates>(),
		world, camera->getViewMatrix(), camera->getProjectionMatrix());
}

void Enemy01::State_Normal(float elapsedTime)
{
	GameWindow* gameWindow = GameContext::Get<GameWindow>();
	// プレイヤーを取得
	Object* target = gameWindow->GetStage()->GetPlayer();

	// 定期的にプレイヤーの向きを変える
	m_thinkTimer += elapsedTime;
	if (m_thinkTimer >= THINK_INTERVAL)
	{
		m_thinkTimer = 0.f;

		// ランダムで移動方向を決定
		m_dir = rand() % 8;

		// ターゲット方向に向ける
		if (target)
		{
			m_dir = GetDir(target);
		}
	}

	// 力を加える
	AddForce(GameWindow::DIR_ANGLE[m_dir], .03);

	// 落ちそうなら向きと速度を反転する
	DirectX::SimpleMath::Vector3 tmp = m_pos;
	m_pos += m_vel;
	if (CheckFloor() == false)
	{
		DirectX::SimpleMath::Matrix rotY = DirectX::SimpleMath::Matrix::CreateRotationY(DirectX::XMConvertToRadians(180.f));
		m_vel = DirectX::SimpleMath::Vector3::Transform(m_vel, rotY);
		m_dir = (m_dir + 4) % 8;
	}
	m_pos = tmp;

	// 当たり判定 WIP
	GameContext::Get<CollisionManager>()->Add("Object", m_collider.get());
	m_position = m_pos;
}

void Enemy01::State_Hit(float elapsedTime)
{
	// 摩擦により停止したら
	if (m_vel == DirectX::SimpleMath::Vector3::Zero)
	{
		m_state = STATE_NORMAL;
	}
}

void Enemy01::State_Fall(float elapsedTime)
{
	m_pos.y -= GameWindow::FALL_SPEED * elapsedTime;
	// ある程度落下したら
	if (m_pos.y < -GameWindow::FALL_DISTANCE)
	{
		// 死亡
		m_state = STATE_DEAD;
		// 表示（OFF）
		SetDisplayFlag(false);
	}
}

void Enemy01::OnCollision(GameObject* object)
{
	if (Object * obj = dynamic_cast<Object*>(object))
	{
		auto diff = GetPosition() - obj->GetPosition();
		auto angle = std::atan2(diff.z, diff.x);
		AddForce(angle, obj->GetHitForce());
	}
}

Enemy01::STATE Enemy01::GetState()
{
	return m_state;
}

void Enemy01::Reset()
{
	// プレイヤーを元の状態に戻す
	m_dir = 0;
	m_pos = DirectX::SimpleMath::Vector3((float)m_x, 0.0f, (float)m_y);
	m_state = STATE_NORMAL;
	SetDisplayFlag(true);
}
