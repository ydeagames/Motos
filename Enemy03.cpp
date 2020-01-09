//======================================================
// File Name	: Enemy03.cpp
// Summary		: エネミー
// Date			: 2019/01/09
// Author		: YdeaGames
//======================================================
#include "pch.h"
#include "Enemy03.h"
#include "GameContext.h"
#include "DeviceResources.h"
#include <CommonStates.h>
#include "GameWindow.h"
#include "CollisionManager.h"
#include "Camera.h"
#include "Stage.h"
#include "MyNeuralNetwork.h"
#include "GameAI.h"

Enemy03::Enemy03(const std::string& tag)
	: Enemy01(tag)
{
}

void Enemy03::Initialize(int x, int y)
{
	Enemy01::Initialize(x, y);
}

int Enemy03::GetKeyToDir(int key)
{
	// ビットフィールドのキー情報を８方向(0～7）に変換するテーブル
	int table[16] = { -1, 0, 2, 1, 4, -1, 3, -1, 6, 7, -1, -1, 5, -1, -1, -1 };
	return table[key];
}

void Enemy03::State_Normal(float elapsedTime)
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
			if (m_avoid)
			{
				m_dir = (GetDir(target) + 4) % 8;
			}
			else
			{
				m_dir = GetDir(target);
				if (DirectX::SimpleMath::Vector3::DistanceSquared(GetPosition(), target->GetPosition()) < 1.5f)
					m_avoid = true;
			}
		}
	}

	// 力を加える
	AddForce(GameWindow::DIR_ANGLE[m_dir], .03);

	// 落ちそうなら向きと速度を反転する
	DirectX::SimpleMath::Vector3 tmp = m_position;
	m_position += m_vel;
	if (CheckFloor() == false)
	{
		DirectX::SimpleMath::Matrix rotY = DirectX::SimpleMath::Matrix::CreateRotationY(DirectX::XMConvertToRadians(180.f));
		m_vel = DirectX::SimpleMath::Vector3::Transform(m_vel, rotY);
		m_dir = (m_dir + 4) % 8;
		if (rand() % 10 < 2)
			m_avoid = false;
	}
	m_position = tmp;
}
