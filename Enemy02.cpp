//======================================================
// File Name	: Enemy02.cpp
// Summary		: エネミー
// Date			: 2019/12/19
// Author		: YdeaGames
//======================================================
#include "pch.h"
#include "Enemy02.h"
#include "GameContext.h"
#include "DeviceResources.h"
#include <CommonStates.h>
#include "GameWindow.h"
#include "CollisionManager.h"
#include "Camera.h"
#include "Stage.h"
#include "MyNeuralNetwork.h"
#include "GameAI.h"

Enemy02::Enemy02(const std::string & tag)
	: Enemy01(tag)
{
}

void Enemy02::Initialize(int x, int y)
{
	Enemy01::Initialize(x, y);
}

int Enemy02::GetKeyToDir(int key)
{
	// ビットフィールドのキー情報を８方向(0～7）に変換するテーブル
	int table[16] = { -1, 0, 2, 1, 4, -1, 3, -1, 6, 7, -1, -1, 5, -1, -1, -1 };
	return table[key];
}

void Enemy02::State_Normal(float elapsedTime)
{
	auto stage = GameContext::Get<GameWindow>()->GetStage();
	auto& enemy = GetPosition();
	auto& player = stage->GetPlayer()->GetPosition();
	const auto myPosition = DirectX::SimpleMath::Vector2(player.x, player.z);
	const auto playerPosition = DirectX::SimpleMath::Vector2(enemy.x, enemy.z);
	auto output = GameContext::Get<GameAI>()->GetPlayingData(GameAI::Input{ myPosition , playerPosition });

	// 加える力
	float force = 0.0f;
	
	// 方向キーが押されたら
	if (output.upArrow || output.downArrow || output.leftArrow || output.rightArrow)
	{
		int key = 0;

		// 押された方向キーのビットを立てる
		if (output.upArrow) key |= 1 << GameWindow::UP;
		if (output.downArrow) key |= 1 << GameWindow::DOWN;
		if (output.leftArrow) key |= 1 << GameWindow::LEFT;
		if (output.rightArrow) key |= 1 << GameWindow::RIGHT;
		int dir = GetKeyToDir(key);
		if (dir != -1) m_dir = dir;

		// 力を加えて自機を加速させる
		force = 0.03f;
	}

	// 力を加える
	AddForce(GameWindow::DIR_ANGLE[m_dir], force);
}
