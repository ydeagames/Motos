//======================================================
// File Name	: Parts.cpp
// Summary		: プレイヤークラス
// Date			: 2019/12/10
// Author		: Takafumi Ban
//======================================================
#include "pch.h"
#include "Parts.h"
#include "GameWindow.h"
#include "GameContext.h"
#include "CollisionManager.h"

// 床との判定用の幅と高さ
const float Parts::WIDTH = 0.2f;
const float Parts::HEIGHT = 0.2f;

Parts::Parts(const std::string& tag, DirectX::Model* shadowModel)
	: Object(tag, shadowModel)
	, m_kind(Kind::NONE), m_state(STATE_NORMAL)
{
}

void Parts::Initialize(Kind kind, int x, int y, DirectX::Model * model)
{
	m_model = model;
	m_x = x;
	m_y = y;
	m_position = DirectX::SimpleMath::Vector3((float)x, 0.0f, (float)y);
	m_kind = kind;

	// 質量
	m_weight = 0.1f;

	// 摩擦係数
	m_coefficientOfFriction = 5.0f;

	// 半径
	m_radius = 0.4f;

	// 幅と高さ
	m_width = Parts::WIDTH;
	m_height = Parts::HEIGHT;

	m_collider = std::make_unique<SphereCollider>(this, m_radius);
}

void Parts::Update(float elapsedTime)
{
	// ↓↓↓↓↓↓↓↓↓ 記述してみよう ↓↓↓↓↓↓↓↓↓

	// 削除リクエストがあればタスクを削除
	if (IsInvalid()) return;

	// 摩擦により速度を落とす
	Friction(elapsedTime);

	// 状態によって処理を分岐させる
	switch (m_state)
	{
	case STATE_NORMAL:	// 通常
		break;
	case STATE_FALL:	// 落下中
		State_Fall(elapsedTime);
		break;
	default:
		break;
	}

	// 影
	ShadowActive(m_state != STATE_FALL && m_activeFlag);

	// 位置に速度を足す
	m_position += m_vel;

	// 通常状態で床の上でなければ落下する
	if (m_state == STATE_NORMAL && CheckFloor() == false)
	{
		// 落ちたので停止させる
		m_vel = DirectX::SimpleMath::Vector3::Zero;
		// 状態を落下中へ
		m_state = STATE_FALL;
		// 描画順を変える
		SetDrawPrio(GameWindow::DRAW_FALL);
	}

	// ↑↑↑↑↑↑↑↑↑ 記述してみよう ↑↑↑↑↑↑↑↑↑

	// 衝突判定マネージャーに登録
	GameContext::Get<CollisionManager>()->Add(GetTag(), m_collider.get());
}

Object::OBJECT_ID Parts::GetID()
{
	if (m_kind == POWERUP) return Object::POWERUP_PARTS;
	if (m_kind == JUMP) return Object::JUMP_PARTS;
	return Object::NONE;
}

void Parts::OnCollision(GameObject* object)
{
	if (object->GetTag() == "Player")
	{
		// アクティブフラグを落とす
		SetActive(false);
		// 表示（OFF）
		SetDisplayFlag(false);
	}
	else
	{
		// 衝突したのでいったん停止させる
		m_vel = DirectX::SimpleMath::Vector3::Zero;
		// 衝突した相手へのベクトルを求めて移動方向を算出
		DirectX::SimpleMath::Vector3 v = object->GetPosition() - this->GetPosition();
		float angle = atan2f(v.x, v.z);
		AddForce(angle, 0.1f);
	}
}

void Parts::Reset()
{
	// 再び取ってない状態に戻す
	SetActive(true);
	SetDisplayFlag(true);
	m_state = STATE_NORMAL;
	m_dir = 0;
	m_position = DirectX::SimpleMath::Vector3((float)m_x, 0.0f, (float)m_y);
	SetDrawPrio(GameWindow::DRAW_OBJECT);
}

void Parts::State_Fall(float elapsedTime)
{
	// ↓↓↓↓↓↓↓↓↓ 記述してみよう ↓↓↓↓↓↓↓↓↓

	// 下に落ちる
	m_position.y -= GameWindow::FALL_SPEED * elapsedTime;

	// ある程度落下したら
	if (m_position.y < -GameWindow::FALL_DISTANCE)
	{
		// ステート変更
		m_state = STATE_DEAD;
		// 表示（OFF）
		SetDisplayFlag(false);
	}
	// ↑↑↑↑↑↑↑↑↑ 記述してみよう ↑↑↑↑↑↑↑↑↑
}
