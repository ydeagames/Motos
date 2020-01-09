//======================================================
// File Name	: GameWindow.cpp
// Summary		: �Q�[���E�B���h�E
// Date			: 2019/11/26
// Author		: Takafumi Ban
//======================================================
#include "pch.h"
#include "GameWindow.h"
#include "GameContext.h"
#include "DeviceResources.h"
#include "Camera.h"
#include "Floor.h"
#include "GameObjectManager.h"
#include "ObjectManager.h"
#include "Floor.h"
#include "Stage.h"
#include "Bg.h"
#include "InfoWindow.h"
#include "PartsInfo.h"
#include "Life.h"
#include "Round.h"
#include "Score.h"


const float GameWindow::DIR_ANGLE[] =
{
	DirectX::XMConvertToRadians(45.0f * 0),
	DirectX::XMConvertToRadians(45.0f * 1),
	DirectX::XMConvertToRadians(45.0f * 2),
	DirectX::XMConvertToRadians(45.0f * 3),
	DirectX::XMConvertToRadians(45.0f * 4),
	DirectX::XMConvertToRadians(45.0f * 5),
	DirectX::XMConvertToRadians(45.0f * 6),
	DirectX::XMConvertToRadians(45.0f * 7),
};

// �d�́i�n�����9.8m/s^2�j
const float GameWindow::GRAVITY = 9.8f;

// �������x�i�P�b�Ԏ��ړ�����h�b�g���j
const float GameWindow::FALL_SPEED = 8.f;

// ���������I�u�W�F�N�g����ʂ�������鋗��
const float GameWindow::FALL_ROTATE_SPEED = 10.f;

// �������̃I�u�W�F�N�g�̉�]���x�i�P�b�ԂɂP�W�O�x��]�j
const float GameWindow::FALL_DISTANCE = 180.f;

// ���E���h��
const int GameWindow::ROUND_MAX = 3;
// �n�C�X�R�A�̏����l�i1000 �_�j
const int GameWindow::HIGH_SCORE = 1000;


GameWindow::GameWindow()
	: m_gameState(STATE_PARTS_SELECT)
	, m_round(0)
	, m_life(0)
	, m_getPowerupParts(0)
	, m_powerupParts(0)
	, m_usePowerupParts(0)
	, m_getJumpParts(0)
	, m_jumpParts(0)
	, m_useJumpParts(0)
	, m_selectParts(POWERUP)
	, m_selectPartsDisplayFlag(false)
	, m_highScore(HIGH_SCORE)
	, m_score(0)
	, m_highScoreUpdate(false)
{
	SetDrawPrio(GameWindow::DRAW_TOP);
}

GameWindow::~GameWindow()
{
	if (m_pStage)
	{
		m_pStage->ResetStageData();
		m_pStage = nullptr;
	}
	if (m_pBg)
	{
		GameObject::Destroy(m_pBg);
		m_pBg = nullptr;
	}
}

void GameWindow::Initialize()
{
	m_camera = std::make_unique<Camera>();
	m_camera->Initialize();

	// Box����
	//m_pShape = DirectX::GeometricPrimitive::CreateBox(GameContext::Get<DX::DeviceResources>()->GetD3DDeviceContext(), DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f));

	// Bg�o�^
	(m_pBg = AddObject<Bg>())
		->Initialize();

	// �X�e�[�W
	m_pStage = std::make_unique<Stage>();
	m_pStage->Initialize();
	m_pStage->LoadStageData(L"StageData/Stage01.csv");
	m_pStage->SetStageData();

	//{
	//	DirectX::EffectFactory fx(GameContext::Get<DX::DeviceResources>()->GetD3DDevice());
	//	fx.SetDirectory(L"Resources\\Models");
	//	m_pModels.emplace_back(DirectX::Model::CreateFromCMO(GameContext::Get<DX::DeviceResources>()->GetD3DDevice(), L"Resources\\Models\\floorPanel_00.cmo", fx));
	//	m_pFloor->SetModel(Floor::NONE, (m_pModels.end() - 1)->get());
	//	m_pModels.emplace_back(DirectX::Model::CreateFromCMO(GameContext::Get<DX::DeviceResources>()->GetD3DDevice(), L"Resources\\Models\\floorPanel_01.cmo", fx));
	//	m_pFloor->SetModel(Floor::ENEMY1, (m_pModels.end() - 1)->get());
	//	m_pFloor->SetModel(Floor::DAMAGED, (m_pModels.end() - 1)->get());
	//	m_pFloor->SetModel(Floor::DEAD, (m_pModels.end() - 1)->get());
	//	m_pFloor->SetModel(Floor::FALL, (m_pModels.end() - 1)->get());
	//}
}

void GameWindow::Update(float elapsedTime)
{
	auto kb = DirectX::Keyboard::Get().GetState();
	m_tracker.Update(kb);
	m_pStage->GetPlayer()->Move(elapsedTime, m_tracker);
}

void GameWindow::Render()
{
	// �J�����擾
	//DirectX::SimpleMath::Matrix world;
	//world = DirectX::SimpleMath::Matrix::CreateTranslation(4.5f, 0.0f, 5.5f);
	//m_pShape->Draw(world, m_camera->getViewMatrix(), m_camera->getProjectionMatrix());
}

Camera* GameWindow::GetCamera()
{
	return m_camera.get();
}

Stage* GameWindow::GetStage()
{
	return m_pStage.get();
}

void GameWindow::GetPowerupParts()
{
	InfoWindow* infoWindow = GameContext::Get<InfoWindow>();
	m_getPowerupParts++;
	infoWindow->GetPowerParts()->GetParts();
}
void GameWindow::GetJumpParts()
{
	InfoWindow* infoWindow = GameContext::Get<InfoWindow>();
	m_getJumpParts++;
	infoWindow->GetJumpParts()->GetParts();
}
void GameWindow::ResetPowerupParts()
{
	InfoWindow* infoWindow = GameContext::Get<InfoWindow>();
	m_powerupParts = m_getPowerupParts = 0;
	infoWindow->GetPowerParts()->Reset();
}
void GameWindow::ResetJumpParts()
{
	InfoWindow* infoWindow = GameContext::Get<InfoWindow>();
	m_jumpParts = m_getJumpParts = 0;
	infoWindow->GetJumpParts()->Reset();
}
GameWindow::GAME_STATE GameWindow::InitializeGame(float elapsedTime)
{
	InfoWindow* infoWindow = GameContext::Get<InfoWindow>();
	// ���_��������
	SetScore(0);
	// �n�C�X�R�A�X�V�t���O�𗎂Ƃ�
	m_highScoreUpdate = false;
	// �n�C�X�R�A�X�V�ʒm���R�b�Ԃɐݒ�
	m_highScoreBlinkTime = 3.0f;
	// ���E���h����ݒ�
	m_round = 1;
	infoWindow->GetRound()->SetRound(m_round);
	// �c�@����ݒ�
	m_life = 3;
	infoWindow->GetLife()->SetLife(m_life);
	// �p���[�A�b�v�p�[�c�̃��Z�b�g
	ResetPowerupParts();
	// �W�����v�p�[�c�̃��Z�b�g
	ResetJumpParts();
	// �X�e�[�W�f�[�^�̓ǂݍ���
	m_pStage->LoadStageData(GetStageFilename(m_round));
	// �X�e�[�W�f�[�^�̐ݒ�
	m_pStage->SetStageData();
	// �n�C�X�R�A��ݒ�
	infoWindow->GetHighScore()->SetScore(m_highScore);
	// 1P ��_��
	infoWindow->GetScore()->BlinkTitle(true);
	// �p�[�c�I����ʂ�
	return STATE_PARTS_SELECT;
}
GameWindow::GAME_STATE GameWindow::SelectParts(float elapsedTime)
{
	InfoWindow* infoWindow = GameContext::Get<InfoWindow>();
	// �p�[�c������������Ă��Ȃ���΃p�[�c�I����ʂ̓X���[
	if (m_powerupParts == 0 && m_jumpParts == 0) return STATE_START;
	switch (m_selectParts)
	{
	case POWERUP: // �p���[�A�b�v�p�[�c�I��
		infoWindow->GetPowerParts()->SelectPartsMode(true);
		// �E�L�[�������ꂽ
		if (m_tracker.IsKeyPressed(DirectX::Keyboard::Right))
		{
			if (m_usePowerupParts < PartsInfo::PARTS_MAX_USE[PartsInfo::POWER_PARTS]
				&& m_usePowerupParts < m_powerupParts)
			{
				m_usePowerupParts++;
			}
		}
		// ���L�[�������ꂽ
		if (m_tracker.IsKeyPressed(DirectX::Keyboard::Left))
		{
			if (m_usePowerupParts > 0)
			{
				m_usePowerupParts--;
			}
		}
		infoWindow->GetPowerParts()->SetUseCnt(m_usePowerupParts);
		// �X�y�[�X�L�[�Ŏ���
		if (m_powerupParts == 0 || m_tracker.pressed.Space)
		{
			// �v���C���[�Ƀp���[�A�b�v�p�[�c�𑕒�����
			m_pStage->GetPlayer()->SetPowerupParts(m_usePowerupParts);
			m_selectParts = JUMP;
			infoWindow->GetPowerParts()->SelectPartsMode(false);
		}
		break;
	case JUMP: // �W�����v�p�[�c�I��
		infoWindow->GetJumpParts()->SelectPartsMode(true);
		// �E�L�[�������ꂽ
		if (m_tracker.IsKeyPressed(DirectX::Keyboard::Right))
		{
			if (m_useJumpParts < PartsInfo::PARTS_MAX_USE[PartsInfo::JUMP_PARTS]
				&& m_useJumpParts < m_jumpParts)
			{
				m_useJumpParts++;
			}
		}
		// ���L�[�������ꂽ
		if (m_tracker.IsKeyPressed(DirectX::Keyboard::Left))
		{
			if (m_useJumpParts > 0)
			{
				m_useJumpParts--;
			}
		}
		infoWindow->GetJumpParts()->SetUseCnt(m_useJumpParts);
		// �X�y�[�X�L�[�Ŏ���
		if (m_jumpParts == 0 || m_tracker.pressed.Space)
		{
			infoWindow->GetJumpParts()->SelectPartsMode(false);
			m_selectParts = POWERUP;
			if (m_useJumpParts)
			{
				// �W�����v�p�[�c�𑕒�
				m_pStage->GetPlayer()->SetJumpParts(true);
			}
			else
			{
				// �W�����v�p�[�c���O��
				m_pStage->GetPlayer()->SetJumpParts(false);
			}
			return STATE_START;
		}
		break;
	}
	return STATE_PARTS_SELECT;
}
GameWindow::GAME_STATE GameWindow::StartGame(float elapsedTime)
{
	// �G��S�ăA�N�e�B�u�ɂ���
	auto enemyList = m_pStage->GetEnemyList();
	for (Object* p : enemyList)
	{
		p->SetActive(true);
	}
	// �Q�[������
	return STATE_GAME;
}
GameWindow::GAME_STATE GameWindow::PlayGame(float elapsedTime)
{
	InfoWindow* infoWindow = GameContext::Get<InfoWindow>();
	GAME_STATE state = STATE_GAME;
	// �v���C���[�̈ړ�����
	m_pStage->GetPlayer()->Move(elapsedTime, m_tracker);
	// �v���C���[�����񂾂�
	if (m_pStage->GetPlayer()->GetState() == Player::STATE_DEAD)
	{
		// �c�@�������炷
		m_life--;
		// ���E�C���h�E�̎c�@�������炷
		infoWindow->GetLife()->SetLife(m_life);
		if (m_life == 0)
		{
			// �Q�[���I�[�o�[
			state = STATE_GAMEOVER;
		}
		else
		{
			// �������`�������W
			state = STATE_AGAIN;
		}
	}
	else
	{
		// �G���S�łŃN���A
		if (CheckEnemyAllDead() == true)
		{
			// ���̃X�e�[�W��
			state = STATE_NEXT;
		}
	}
	// �n�C�X�R�A�̃^�C�g���_�ł��~�߂�
	if (m_highScoreUpdate && state != STATE_GAME)
	{
		m_highScoreBlinkTime = 0.0f;
	}
	// �n�C�X�R�A�̃^�C�g���_�ł��~�߂鏈��
	if (m_highScoreUpdate && m_highScoreBlinkTime > 0.0f)
	{
		m_highScoreBlinkTime -= elapsedTime;
	}
	else
	{
		infoWindow->GetHighScore()->BlinkTitle(false);
	}
	return state;
}
GameWindow::GAME_STATE GameWindow::NextGame(float elapsedTime)
{
	InfoWindow* infoWindow = GameContext::Get<InfoWindow>();
	// ���E���h�������Z
	m_round++;
	if (m_round > ROUND_MAX) m_round = 1;
	infoWindow->GetRound()->SetRound(m_round);
	// �X�e�[�W�����Z�b�g����
	m_pStage->ResetStageData();
	// �X�e�[�W�f�[�^�̓ǂݍ���
	m_pStage->LoadStageData(GetStageFilename(m_round));
	// �X�e�[�W�f�[�^�̐ݒ�
	m_pStage->SetStageData();
	// �p�[�c�I����
	return STATE_PARTS_SELECT;
}
void GameWindow::SetScore(int score)
{
	InfoWindow* infoWindow = GameContext::Get<InfoWindow>();
	m_score = score;
	infoWindow->GetScore()->SetScore(m_score);
	// �n�C�X�R�A�X�V���`�F�b�N
	if (m_score > m_highScore)
	{
		m_highScore = m_score;
		infoWindow->GetHighScore()->SetScore(m_highScore);
		// �n�C�X�R�A�X�V�ʒm�̓_�ł͈�x����
		if (m_highScoreUpdate == false)
		{
			// �n�C�X�R�A�X�V�t���O�𗧂Ă�
			m_highScoreUpdate = true;
			// �n�C�X�R�A�̃^�C�g����_�ł�����
			infoWindow->GetHighScore()->BlinkTitle(true);
		}
	}
}
int GameWindow::GetScore()
{
	return m_score;
}
wchar_t* GameWindow::GetStageFilename(int round)
{
	static wchar_t fname[32];
	wsprintfW(fname, L"StageData\\Stage%02d.csv", round);
	return fname;
}
bool GameWindow::CheckEnemyAllDead()
{
	auto enemyList = m_pStage->GetEnemyList();
	for (Object* p : enemyList)
	{
		// �\�� ON �̃I�u�W�F�N�g������ΑS������ł��Ȃ�
		if (p->GetDisplayFlag() == true) return false;
	}
	return true;
}
void GameWindow::UpdateParts(bool stageClearFlag)
{
	InfoWindow* infoWindow = GameContext::Get<InfoWindow>();
	// �X�e�[�W�N���A�����̂Ŏg�p�����p�[�c�������A�擾�����p�[�c�𑫂�
	if (stageClearFlag == true)
	{
		m_powerupParts -= m_usePowerupParts;
		m_powerupParts += m_getPowerupParts;
		m_jumpParts -= m_useJumpParts;
		m_jumpParts += m_getJumpParts;
	}
	// �v���C���[�ɑ��������p�[�c������
	m_pStage->GetPlayer()->SetPowerupParts(0);
	m_pStage->GetPlayer()->SetJumpParts(false);
	// ����g�p�����p�[�c�A�擾�����p�[�c����������
	m_getPowerupParts = m_usePowerupParts = 0;
	m_getJumpParts = m_useJumpParts = 0;
	// ���E�C���h�E�̃p�[�c��������������
	infoWindow->GetPowerParts()->SetUseCnt(0);
	infoWindow->GetJumpParts()->SetUseCnt(0);
	infoWindow->GetPowerParts()->SetGetCnt(m_powerupParts);
	infoWindow->GetJumpParts()->SetGetCnt(m_jumpParts);
	// �p�[�c�_�ł����Z�b�g����
	infoWindow->GetPowerParts()->ResetBlink();
	infoWindow->GetJumpParts()->ResetBlink();
}