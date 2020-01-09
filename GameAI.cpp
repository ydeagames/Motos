#include "pch.h"
#include "GameAI.h"
#include "GameWindow.h"
#include "Stage.h"

#define	MAX_DATA		3
#define MAX_PARAMS		SCREEN_WIDTH

using namespace std::chrono_literals;

void GameAI::Input::Configure(NeuralNetwork& neural) const
{
	neural.SetInput(0, double(myPosition.x / Stage::STAGE_W));
	neural.SetInput(1, double(myPosition.y / Stage::STAGE_H));
	//neural.SetInput(2, double(playerPosition.x / Stage::STAGE_W));
	//neural.SetInput(3, double(playerPosition.y / Stage::STAGE_H));
	//neural.SetInput(4, double(myVelocity.x / Player::MAX_SPEED));
	//neural.SetInput(5, double(myVelocity.y / Player::MAX_SPEED));
	//neural.SetInput(6, double(playerVelocity.x / Player::MAX_SPEED));
	//neural.SetInput(7, double(playerVelocity.y / Player::MAX_SPEED));
	neural.SetInput(2, floorCheck != false);
}

void GameAI::Output::Configure(NeuralNetwork& neural) const
{
	neural.SetDesiredOutput(0, upArrow != false);
	neural.SetDesiredOutput(1, downArrow != false);
	neural.SetDesiredOutput(2, leftArrow != false);
	neural.SetDesiredOutput(3, rightArrow != false);
}

GameAI::Output GameAI::Output::Create(NeuralNetwork& neural)
{
	Output out{};
	out.upArrow = neural.GetOutput(0) > .5;
	out.downArrow = neural.GetOutput(1) > .5;
	out.leftArrow = neural.GetOutput(2) > .5;
	out.rightArrow = neural.GetOutput(3) > .5;
	return out;
}

void GameAI::Initialize()
{
	// ニューラルネットワークを初期化する(入力層、隠れ層、出力層)
	m_aiA.Initialize(3, 100, 4);
	m_aiB.Initialize(3, 100, 4);

	// 学習率を設定する
	// Setup learning rate
	m_aiA.SetLearningRate(0.2);
	m_aiB.SetLearningRate(0.2);

	// モメンタムを設定する
	// Setup momentum
	m_aiA.SetMomentum(true, 0.8);
	m_aiB.SetMomentum(true, 0.8);

	if (!m_thread.joinable())
		m_thread = std::thread([&]() { Calculate(); });
}

GameAI::~GameAI()
{
	if (m_thread.joinable())
	{
		m_thread_stop = true;
		m_thread.join();
	}
}

NeuralNetwork& GameAI::GetPlayingAI()
{
	return m_swap ? m_aiA : m_aiB;
}

NeuralNetwork& GameAI::GetLerningAI()
{
	return m_swap ? m_aiB : m_aiA;
}

void GameAI::SwapAI()
{
	m_swap = !m_swap;
}

GameAI::Output GameAI::GetPlayingData(const Input& input)
{
	auto& neural = GetPlayingAI();

	input.Configure(neural);

	neural.FeedForward();

	return Output::Create(neural);
}

void GameAI::AddLerningData(const Input& input, const Output& output)
{
	m_dataMutex.lock();
	m_data.push_back(Data{ input, output });
	m_dataMutex.unlock();
}

void GameAI::ForgetRecently()
{
	m_dataMutex.lock();
	if (m_data.size() > 5)
	{
		m_data.erase(m_data.end() - 5, m_data.end());
	}
	m_dataMutex.unlock();
}

void GameAI::ForgetPast()
{
	m_dataMutex.lock();
	if (m_data.size() > 100)
	{
		m_data.erase(m_data.begin(), m_data.end() - 20);
	}
	m_dataMutex.unlock();
}

void GameAI::Calculate()
{
	auto data = m_data;

	while (!m_thread_stop)
	{
		if (m_data.size() > 5)
		{
			auto& neural = GetLerningAI();

			m_dataMutex.lock();
			data = m_data;
			m_dataMutex.unlock();

			//neural.CleanUp();
			//neural.Initialize(4, 40, 4);
			//neural.SetLearningRate(0.2);
			//neural.SetMomentum(true, 0.8);

			double error = 1.0;
			int		count = 0;

			// 機械学習する
			while (error > 0.001 && count < 10000)
			{
				error = 0.0;
				count++;

				for (auto& element : data)
				{
					// 入力データをニューラルネットワークにセットする
					// 入力
					element.input.Configure(neural);
					// 期待される出力
					element.output.Configure(neural);

					// 前方伝播する
					neural.FeedForward();
					// 誤差を計算する
					error += neural.CalculateError();
					// 誤差逆伝播する
					neural.BackPropagate();
				}

				// 誤差を計算する
				error = error / data.size();


				if (m_thread_stop)
					return;
			}

			ForgetPast();
			SwapAI();
			calcNum++;
		}

		std::this_thread::sleep_for(1s);
	}
}
