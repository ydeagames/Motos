#pragma once
#include "MyNeuralNetwork.h"

class GameAI
{
private:
	bool m_swap = false;
	NeuralNetwork m_aiA;
	NeuralNetwork m_aiB;
	std::mutex m_dataMutex;
	std::thread m_thread;
	bool m_thread_stop = false;
	
public:
	struct Input
	{
		DirectX::SimpleMath::Vector2 myPosition;
		DirectX::SimpleMath::Vector2 playerPosition;
		DirectX::SimpleMath::Vector2 myVelocity;
		DirectX::SimpleMath::Vector2 playerVelocity;
		bool floorCheck;

		void Configure(NeuralNetwork& neural) const;
	};

	struct Output
	{
		bool upArrow;
		bool downArrow;
		bool leftArrow;
		bool rightArrow;

		void Configure(NeuralNetwork& neural) const;
		static Output Create(NeuralNetwork& neural);
	};

	struct Data
	{
		Input input;
		Output output;
	};

	int calcNum;

public:
	std::vector<Data> m_data;

public:
	void Initialize();
	~GameAI();
	NeuralNetwork& GetPlayingAI();
	NeuralNetwork& GetLerningAI();
	void SwapAI();
	Output GetPlayingData(const Input& input);
	void AddLerningData(const Input& input, const Output& output);
	void ForgetRecently();
	void ForgetPast();

private:
	void Calculate();
};
