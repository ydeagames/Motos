//======================================================
// File Name	: Parts.cpp
// Summary		: プレイヤークラス
// Date			: 2019/12/10
// Author		: Takafumi Ban
//======================================================
#include "pch.h"
#include "Parts.h"
#include "GameWindow.h"

// 床との判定用の幅と高さ
const float Parts::WIDTH = 0.2f;
const float Parts::HEIGHT = 0.2f;

Parts::Parts()
	: m_kind(Kind::NONE), m_state(STATE_NORMAL)
{
}

void Parts::Initialize(Kind kind, int x, int y, DirectX::Model * model)
{
	m_model = model;
	m_x = x;
	m_y = y;
	m_pos = DirectX::SimpleMath::Vector3((float)x, 0.0f, (float)y);
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

	// 位置に速度を足す
	m_pos += m_vel;

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
}

Object::OBJECT_ID Parts::GetID()
{
	if (m_kind == POWERUP) return Object::POWERUP_PARTS;
	if (m_kind == JUMP) return Object::JUMP_PARTS;
	return Object::NONE;
}

void Parts::OnCollision(GameObject* object)
{
}

void Parts::Reset()
{
	// 再び取ってない状態に戻す
	SetActive(true);
	SetDisplayFlag(true);
	m_state = STATE_NORMAL;
	m_dir = 0;
	m_pos = DirectX::SimpleMath::Vector3((float)m_x, 0.0f, (float)m_y);
	SetDrawPrio(GameWindow::DRAW_OBJECT);
}

void Parts::State_Fall(float elapsedTime)
{
	// ↓↓↓↓↓↓↓↓↓ 記述してみよう ↓↓↓↓↓↓↓↓↓

	// 下に落ちる
	m_pos.y -= GameWindow::FALL_SPEED * elapsedTime;

	// ある程度落下したら
	if (m_pos.y < -GameWindow::FALL_DISTANCE)
	{
		// ステート変更
		m_state = STATE_DEAD;
		// 表示（OFF）
		SetDisplayFlag(false);
	}
	// ↑↑↑↑↑↑↑↑↑ 記述してみよう ↑↑↑↑↑↑↑↑↑
}
