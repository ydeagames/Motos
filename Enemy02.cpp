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

#define	MAX_DATA		3
#define MAX_PARAMS		SCREEN_WIDTH

Enemy02::Enemy02(const std::string& tag)
	: Enemy01(tag)
{
}

void Enemy02::Initialize(int x, int y)
{
	Enemy01::Initialize(x, y);

	// ニューラルネットワークを初期化する(入力層、隠れ層、出力層)
	m_neuralNetwork.Initialize(8, 40, 8);

	// 学習率を設定する
	// Setup learning rate
	m_neuralNetwork.SetLearningRate(0.2);

	// モメンタムを設定する
	// Setup momentum
	m_neuralNetwork.SetMomentum(true, 0.8);
}

void Enemy02::State_Normal(float elapsedTime)
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
	DirectX::SimpleMath::Vector3 tmp = m_position;
	m_position += m_vel;
	if (CheckFloor() == false)
	{
		DirectX::SimpleMath::Matrix rotY = DirectX::SimpleMath::Matrix::CreateRotationY(DirectX::XMConvertToRadians(180.f));
		m_vel = DirectX::SimpleMath::Vector3::Transform(m_vel, rotY);
		m_dir = (m_dir + 4) % 8;
	}
	m_position = tmp;

	double error = 1.0;
	int		count = 0;

	//// 機械学習する
	//while (error > 0.001 && count < 10000)
	//{
	//	error = 0.0;
	//	count++;
	//	for (int i = 0; i < MAX_PARAMS; i++)
	//	{
	//		// 入力データをニューラルネットワークにセットする
	//		// 角度
	//		m_neuralNetwork.SetInput(0, double(i) / double(SCREEN_WIDTH));
	//		// sinデータ
	//		m_neuralNetwork.SetDesiredOutput(0, double(graph[i]) / double(SCREEN_HEIGHT));

	//		// 前方伝播する
	//		m_neuralNetwork.FeedForward();
	//		// 誤差を計算する
	//		error += m_neuralNetwork.CalculateError();
	//		// 誤差逆伝播する
	//		m_neuralNetwork.BackPropagate();
	//	}
	//	// 誤差を計算する
	//	error = error / MAX_PARAMS;
	//}
}
