//======================================================
// File Name	: Player.cpp
// Summary		: プレイヤークラス
// Date			: 2019/12/05
// Author		: Takafumi Ban
//======================================================
#include "pch.h"
#include "Player.h"
#include "GameContext.h"
#include "DeviceResources.h"
#include <CommonStates.h>
#include "GameWindow.h"
#include "CollisionManager.h"
#include "Camera.h"

// プレイヤーの重さ
const float Player::WEIGHT = 1.0f;

// 床に対する摩擦係数
const float Player::FRICTION = 0.1f;

// 床との判定用の幅と高さ
const float Player::WIDTH = 0.2f;
const float Player::HEIGHT = 0.2f;

// プレイヤーの最大移動速度
const float Player::MAX_SPEED =	0.1f;

Player::Player()
	: m_models{ nullptr }
	, m_powerupParts(0)
	, m_jumpParts(false)
	, m_state(STATE_NORMAL)
{
}

void Player::Initialize(int x, int y)
{
	m_x = x;
	m_y = y;
	m_pos = DirectX::SimpleMath::Vector3((float)x, 0.0f, (float)y);

	// 質量を設定
	SetPowerupParts(0);

	// 摩擦係数
	m_coefficientOfFriction = FRICTION;

	// 幅と高さ
	m_weight = Player::WIDTH;
	m_height = Player::HEIGHT;
}

void Player::SetModel(ModelType modelType, DirectX::Model * model)
{
	m_models[modelType] = model;
}

void Player::Update(float elapsedTime)
{
	// 削除リクエストがあればタスクを削除
	if (IsInvalid()) return;

	// アクティブフラグがfalseの場合は何もしない
	if (!m_activeFlag) return;

	// 摩擦により速度を落とす
	Friction(elapsedTime);

	// 最大速度以上にならないよう調整
	if (m_vel.LengthSquared() > MAX_SPEED * MAX_SPEED)
	{
		m_vel.Normalize();
		m_vel *= MAX_SPEED;
	}

	// 状態によって処理を分岐させる
	switch (m_state)
	{
	case STATE_NORMAL:	// 通常
		break;
	case STATE_JUMP:	// ジャンプ中
		State_Jump(elapsedTime);
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

	// 位置に速度を足す
	m_pos += m_vel;

	// プレイヤーと床のチェック
	if (m_state == Player::STATE_NORMAL && CheckFloor() == false)
	{
		// 落ちたので停止させる
		m_vel = DirectX::SimpleMath::Vector3::Zero;
		// 状態を落下中へ
		m_state = STATE_FALL;
	}
}

void Player::Render()
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
	m_models[m_jumpParts ? WING : NORMAL]->Draw(deviceResources->GetD3DDeviceContext(),
		*GameContext::Get<DirectX::CommonStates>(),
		world, camera->getViewMatrix(), camera->getProjectionMatrix());
}

void Player::State_Jump(float elapsedTime)
{
}

void Player::State_Hit(float elapsedTime)
{
}

void Player::State_Fall(float elapsedTime)
{
}

void Player::OnCollision(GameObject* object)
{
}

Player::STATE Player::GetState()
{
	return m_state;
}

void Player::Move(float elapsedTime, const DirectX::Keyboard::KeyboardStateTracker& tracker)
{
	// プレイヤーが通常、ジャンプ時のみ移動する
	if (m_state != STATE_NORMAL && m_state != STATE_JUMP) return;

	// キー入力を取得
	auto kb = DirectX::Keyboard::Get().GetState();

	// 加える力
	float force = 0.0f;

	// 方向キーが押されたら
	if (kb.Up || kb.Down || kb.Left || kb.Right)
	{
		// 押された方向キーのビットを立てる
		int key = 0;
		if (kb.Up) key |= 1 << GameWindow::UP;
		if (kb.Down) key |= 1 << GameWindow::DOWN;
		if (kb.Left) key |= 1 << GameWindow::LEFT;
		if (kb.Right) key |= 1 << GameWindow::RIGHT;
		int dir = GetKeyToDir(key);
		if (dir != -1) m_dir = dir;

		// 力を加えて自機を加速させる
		force = 0.03f;
	}

	// 力を加える
	AddForce(GameWindow::DIR_ANGLE[m_dir], force);
}

void Player::Reset()
{
	// プレイヤーを元の状態に戻す
	m_dir = 0;
	m_pos = DirectX::SimpleMath::Vector3((float)m_x, 0.0f, (float)m_y);
	m_state = STATE_NORMAL;
	SetDisplayFlag(true);
}

void Player::SetPowerupParts(int cnt)
{
	m_powerupParts = cnt;
	m_weight = WEIGHT + m_powerupParts * 0.1f;
}

void Player::SetJumpParts(bool flag)
{
	m_jumpParts = flag;
}

int Player::GetKeyToDir(int key)
{
	// ビットフィールドのキー情報を８方向(0～7）に変換するテーブル
	int table[16] = { -1, 0, 2, 1, 4, -1, 3, -1, 6, 7, -1, -1, 5, -1, -1, -1 };
	return table[key];
}


