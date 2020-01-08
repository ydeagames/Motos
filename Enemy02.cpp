//======================================================
// File Name	: Enemy02.cpp
// Summary		: �G�l�~�[
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

	// �j���[�����l�b�g���[�N������������(���͑w�A�B��w�A�o�͑w)
	m_neuralNetwork.Initialize(8, 40, 8);

	// �w�K����ݒ肷��
	// Setup learning rate
	m_neuralNetwork.SetLearningRate(0.2);

	// �������^����ݒ肷��
	// Setup momentum
	m_neuralNetwork.SetMomentum(true, 0.8);
}

void Enemy02::State_Normal(float elapsedTime)
{
	GameWindow* gameWindow = GameContext::Get<GameWindow>();
	// �v���C���[���擾
	Object* target = gameWindow->GetStage()->GetPlayer();

	// ����I�Ƀv���C���[�̌�����ς���
	m_thinkTimer += elapsedTime;
	if (m_thinkTimer >= THINK_INTERVAL)
	{
		m_thinkTimer = 0.f;

		// �����_���ňړ�����������
		m_dir = rand() % 8;

		// �^�[�Q�b�g�����Ɍ�����
		if (target)
		{
			m_dir = GetDir(target);
		}
	}

	// �͂�������
	AddForce(GameWindow::DIR_ANGLE[m_dir], .03);

	// ���������Ȃ�����Ƒ��x�𔽓]����
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

	//// �@�B�w�K����
	//while (error > 0.001 && count < 10000)
	//{
	//	error = 0.0;
	//	count++;
	//	for (int i = 0; i < MAX_PARAMS; i++)
	//	{
	//		// ���̓f�[�^���j���[�����l�b�g���[�N�ɃZ�b�g����
	//		// �p�x
	//		m_neuralNetwork.SetInput(0, double(i) / double(SCREEN_WIDTH));
	//		// sin�f�[�^
	//		m_neuralNetwork.SetDesiredOutput(0, double(graph[i]) / double(SCREEN_HEIGHT));

	//		// �O���`�d����
	//		m_neuralNetwork.FeedForward();
	//		// �덷���v�Z����
	//		error += m_neuralNetwork.CalculateError();
	//		// �덷�t�`�d����
	//		m_neuralNetwork.BackPropagate();
	//	}
	//	// �덷���v�Z����
	//	error = error / MAX_PARAMS;
	//}
}
