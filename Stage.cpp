//======================================================
// File Name	: Stage.
// Summary		: �X�e�[�W�N���X
// Date			: 2019/11/28
// Author		: Takafumi Ban
//======================================================
#include "pch.h"
#include "Stage.h"
#include "GameWindow.h"
#include <Effects.h>
#include <CommonStates.h>
#include "GameContext.h"
#include "DeviceResources.h"
#include "GameObjectManager.h"
#include "ObjectManager.h"
#include "Enemy03.h"

Stage::Stage()
	: m_floors{ nullptr }, m_stageData{ 0 }
{
}

Stage::~Stage()
{
	// ��������
	for (int j = 0; j < STAGE_H; j++)
	{
		for (int i = 0; i < STAGE_W; i++)
		{
			m_floors[j][i]->Invalidate();
			m_floors[j][i] = nullptr;
		}
	}

	// ���ׂẴI�u�W�F�N�g������
	DeleteAllObject();
}

void Stage::Initialize()
{
	// ���f���f�[�^�̓ǂݍ���
	DirectX::EffectFactory fx(GameContext::Get<DX::DeviceResources>()->GetD3DDevice());
	fx.SetDirectory(L"Resources\\Models");

	// ���̃��f��
	m_floorModels[Floor::NORMAL] = DirectX::Model::CreateFromCMO(GameContext::Get<DX::DeviceResources>()->GetD3DDevice(), L"Resources\\Models\\floorPanel_00.cmo", fx);
	m_floorModels[Floor::DAMAGED] = DirectX::Model::CreateFromCMO(GameContext::Get<DX::DeviceResources>()->GetD3DDevice(), L"Resources\\Models\\floorPanel_01.cmo", fx);

	// ���̃^�X�N����
	for (int j = 0; j < STAGE_H; j++)
	{
		for (int i = 0; i < STAGE_W; i++)
		{
			std::unique_ptr<Floor> pFloor = std::make_unique<Floor>();
			m_floors[j][i] = pFloor.get();
			m_floors[j][i]->Initialize(this, i, j);
			GameContext::Get<ObjectManager>()->GetGameOM()->Add(std::move(pFloor));
			// �e��Ԃ̃��f����ݒ�
			m_floors[j][i]->SetModel(Floor::NORMAL, m_floorModels[Floor::NORMAL].get());
			m_floors[j][i]->SetModel(Floor::DAMAGED, m_floorModels[Floor::DAMAGED].get());
			m_floors[j][i]->SetModel(Floor::FALL, m_floorModels[Floor::DAMAGED].get());
		}
	}

	// �v���C���[�̃��f��
	m_playerModels[Player::NORMAL] = DirectX::Model::CreateFromCMO(GameContext::Get<DX::DeviceResources>()->GetD3DDevice(), L"Resources\\Models\\player_01.cmo", fx);
	m_playerModels[Player::WING] = DirectX::Model::CreateFromCMO(GameContext::Get<DX::DeviceResources>()->GetD3DDevice(), L"Resources\\Models\\player_02.cmo", fx);

	// �p�[�c�̃��f��
	m_partsModels[Parts::POWERUP] = DirectX::Model::CreateFromCMO(GameContext::Get<DX::DeviceResources>()->GetD3DDevice(), L"Resources\\Models\\powerupParts.cmo", fx);
	m_partsModels[Parts::JUMP] = DirectX::Model::CreateFromCMO(GameContext::Get<DX::DeviceResources>()->GetD3DDevice(), L"Resources\\Models\\jumpParts.cmo", fx);

	// �G�̃��f��
	m_enemyModels[Enemy01::ENEMY1] = DirectX::Model::CreateFromCMO(GameContext::Get<DX::DeviceResources>()->GetD3DDevice(), L"Resources\\Models\\enemy_01.cmo", fx);
	m_enemyModels[Enemy01::ENEMY2] = DirectX::Model::CreateFromCMO(GameContext::Get<DX::DeviceResources>()->GetD3DDevice(), L"Resources\\Models\\igaguriEnemy.cmo", fx);
	m_enemyModels[Enemy01::ENEMY3] = DirectX::Model::CreateFromCMO(GameContext::Get<DX::DeviceResources>()->GetD3DDevice(), L"Resources\\Models\\targetEnemy.cmo", fx);
}

Floor* Stage::GetFloor(int x, int y)
{
	if (x >= 0 && x < STAGE_W && y >= 0 && y < STAGE_H)
	{
		return m_floors[y][x];
	}
	return nullptr;
}

bool Stage::LoadStageData(wchar_t* fname)
{
	std::wstring str;
	// �t�@�C���̃I�[�v��
	std::wifstream ifs(fname);
	//----- �X�e�[�W�f�[�^ -----//
	// �w�b�_������
	std::wstring stageHeadName = L"STAGE";
	// �w�b�_�����܂ŃV�[�N
	while (!ifs.eof())
	{
		getline(ifs, str);
		if (str.compare(0, stageHeadName.size(), stageHeadName) == 0)
		{
			break;
		}
	}
	// �X�e�[�W�f�[�^�̓ǂݍ���
	for (int i = 0; i < STAGE_H; i++)
	{
		getline(ifs, str);
		std::wstring tmp;
		std::wistringstream stream(str);
		for (int j = 0; j < STAGE_W; j++)
		{
			std::getline(stream, tmp, L',');
			m_stageData.stage[i][j] = std::stoi(tmp);
		}
	}
	//----- �X�e�[�W��̃I�u�W�F�N�g�f�[�^ -----//
	// �w�b�_������
	std::wstring objectHeadName = L"OBJECT";
	// �t�@�C���|�C���^��擪�փV�[�N
	ifs.clear();
	ifs.seekg(0);
	// �w�b�_�����܂ŃV�[�N
	while (!ifs.eof())
	{
		getline(ifs, str);
		if (str.compare(0, objectHeadName.size(), objectHeadName) == 0)
		{
			break;
		}
	}
	// �I�u�W�F�N�g�f�[�^�̓ǂݍ���
	for (int i = 0; i < STAGE_H; i++)
	{
		getline(ifs, str);
		std::wstring tmp;
		std::wistringstream stream(str);
		for (int j = 0; j < STAGE_W; j++)
		{
			std::getline(stream, tmp, L',');
			m_stageData.object[i][j] = std::stoi(tmp);
		}
	}
	// �t�@�C���̃N���[�Y
	ifs.close();
	return true;
}

void Stage::SetStageData()
{
	// �������̏�Ԃɖ߂�
	for (int j = 0; j < STAGE_H; j++)
	{
		for (int i = 0; i < STAGE_W; i++)
		{
			m_floors[j][i]->SetState(static_cast<Floor::State>(m_stageData.stage[j][i]));
		}
	}

	// �S�ẴI�u�W�F�N�g�^�X�N�̍폜
	DeleteAllObject();

	//----- �e�Q�[���I�u�W�F�N�g�̐��� -----//

	// �X�e�[�W��̃I�u�W�F�N�g��z�u����
	for (int j = 0; j < STAGE_H; j++)
	{
		for (int i = 0; i < STAGE_W; i++)
		{
			// �I�u�W�F�N�g�̎�ނɂ��^�X�N��������
			switch (m_stageData.object[j][i])
			{

			case OBJECT_ID::PLAYER:	// �v���C���[
			{
				std::unique_ptr<Player> pPlayer = std::make_unique<Player>("Player");
				m_player = pPlayer.get();
				m_player->Initialize(i, j);
				// �e��Ԃ̃��f����ݒ�
				m_player->SetModel(Player::NORMAL, m_playerModels[Player::NORMAL].get());
				m_player->SetModel(Player::WING, m_playerModels[Player::WING].get());
				m_player->SetJumpParts(true);
				GameContext::Get<ObjectManager>()->GetGameOM()->Add(std::move(pPlayer));

				// ���Ƃ̔���֐���o�^
				m_player->SetCheckFloorFunction([&](Object* object)
					{
						return CheckFloor(object->GetPosition(), object->GetWidth(), object->GetHeight());
					});

				// �W�����v���n���̊֐���o�^
				m_player->SetJumpEndFunction([&](Object* object)
					{
						DamageFloor(object->GetPosition(), object->GetWidth(), object->GetHeight());
					});
			}
			break;

			case OBJECT_ID::POWERUP_PARTS:	// �p���[�A�b�v�p�[�c
			{
				std::unique_ptr<Parts> pParts = std::make_unique<Parts>("PowerupParts");
				pParts->Initialize(Parts::POWERUP, i, j, m_partsModels[Parts::POWERUP].get());
				pParts->SetCheckFloorFunction([&](Object* object)
					{
						return CheckFloor(object->GetPosition(), object->GetWidth(), object->GetHeight());
					});
				m_parts.push_back(pParts.get());
				GameContext::Get<ObjectManager>()->GetGameOM()->Add(std::move(pParts));
			}
			break;

			case OBJECT_ID::JUMP_PARTS:		// �W�����v�p�[�c
			{
				std::unique_ptr<Parts> pParts = std::make_unique<Parts>("JumpParts");
				pParts->Initialize(Parts::JUMP, i, j, m_partsModels[Parts::JUMP].get());
				pParts->SetCheckFloorFunction([&](Object* object)
					{
						return CheckFloor(object->GetPosition(), object->GetWidth(), object->GetHeight());
					});
				m_parts.push_back(pParts.get());
				GameContext::Get<ObjectManager>()->GetGameOM()->Add(std::move(pParts));
			}
			break;

			case OBJECT_ID::ENEMY_1:	// �G�P
			{
				std::unique_ptr<Enemy01> pEnemy01 = std::make_unique<Enemy01>("Enemy01");
				pEnemy01->Initialize(i, j);
				// �e��Ԃ̃��f����ݒ�
				pEnemy01->SetModel(Enemy01::ENEMY1, m_enemyModels[Enemy01::ENEMY1].get());

				// ���Ƃ̔���֐���o�^
				pEnemy01->SetCheckFloorFunction([&](Object* object)
					{
						return CheckFloor(object->GetPosition(), object->GetWidth(), object->GetHeight());
					});

				// ���_��ݒ肷��֐���o�^
				pEnemy01->SetAddScoreFunction([&](int score)
					{
						GameContext::Get<GameWindow>()->SetScore(GameContext::Get<GameWindow>()->GetScore() + score);
					});

				m_enemy01 = pEnemy01.get();
				m_enemies.push_back(pEnemy01.get());
				GameContext::Get<ObjectManager>()->GetGameOM()->Add(std::move(pEnemy01));
			}
			break;

			case OBJECT_ID::ENEMY_2:	// �G�Q
			{
				std::unique_ptr<Enemy02> pEnemy02 = std::make_unique<Enemy02>("Enemy02");
				pEnemy02->Initialize(i, j);
				// �e��Ԃ̃��f����ݒ�
				pEnemy02->SetModel(Enemy02::ENEMY1, m_enemyModels[Enemy02::ENEMY2].get());

				// ���Ƃ̔���֐���o�^
				pEnemy02->SetCheckFloorFunction([&](Object* object)
					{
						return CheckFloor(object->GetPosition(), object->GetWidth(), object->GetHeight());
					});

				// ���_��ݒ肷��֐���o�^
				pEnemy02->SetAddScoreFunction([&](int score)
					{
						GameContext::Get<GameWindow>()->SetScore(GameContext::Get<GameWindow>()->GetScore() + score);
					});

				m_enemy02 = pEnemy02.get();
				m_enemies.push_back(pEnemy02.get());
				GameContext::Get<ObjectManager>()->GetGameOM()->Add(std::move(pEnemy02));
			}
			break;

			case OBJECT_ID::ENEMY_3:	// �G�Q
			{
				std::unique_ptr<Enemy03> pEnemy03 = std::make_unique<Enemy03>("Enemy02");
				pEnemy03->Initialize(i, j);
				// �e��Ԃ̃��f����ݒ�
				pEnemy03->SetModel(Enemy02::ENEMY1, m_enemyModels[Enemy02::ENEMY3].get());

				// ���Ƃ̔���֐���o�^
				pEnemy03->SetCheckFloorFunction([&](Object* object)
					{
						return CheckFloor(object->GetPosition(), object->GetWidth(), object->GetHeight());
					});

				// ���_��ݒ肷��֐���o�^
				pEnemy03->SetAddScoreFunction([&](int score)
					{
						GameContext::Get<GameWindow>()->SetScore(GameContext::Get<GameWindow>()->GetScore() + score);
					});

				m_enemy03 = pEnemy03.get();
				m_enemies.push_back(pEnemy03.get());
				GameContext::Get<ObjectManager>()->GetGameOM()->Add(std::move(pEnemy03));
			}
			break;

			}
		}
	}
}

void Stage::ResetStageData()
{
	// �������Z�b�g
	for (int j = 0; j < STAGE_H; j++)
	{
		for (int i = 0; i < STAGE_W; i++)
		{
			m_floors[j][i]->Reset();
		}
	}
}

Player* Stage::GetPlayer()
{
	return m_player;
}

const std::vector<Object*>& Stage::GetEnemyList()
{
	return m_enemies;
}

Enemy01* Stage::GetEnemy01()
{
	return m_enemy01;
}

Enemy02* Stage::GetEnemy02()
{
	return m_enemy02;
}

Enemy03* Stage::GetEnemy03()
{
	return m_enemy03;
}

bool Stage::CheckFloor(DirectX::SimpleMath::Vector3 pos, float w, float h)
{
	int x, y;
	const DirectX::SimpleMath::Vector2 corner[4] = {
	DirectX::SimpleMath::Vector2(-1.0f, -1.0f),
	DirectX::SimpleMath::Vector2(1.0f, -1.0f),
	DirectX::SimpleMath::Vector2(1.0f, 1.0f),
	DirectX::SimpleMath::Vector2(-1.0f, 1.0f)
	};
	// ���S����̋����ɂ��邽�ߕ��ƍ������Q�Ŋ���
	w /= 2.0f;
	h /= 2.0f;
	// �S�_�𒲂ׂ�
	int i = 0;
	for (; i < 4; i++)
	{
		// 3D ��Ԃ̍��W���}�b�v�`�b�v�̈ʒu�ɕϊ�����
		ConvertPosToMapChip(pos.x + w * corner[i].x, pos.z + h * corner[i].y, &x, &y);
		Floor* floor = this->GetFloor(x, y);
		if ((floor != nullptr) && (floor->GetState() == Floor::State::NORMAL || floor->GetState() == Floor::State::DAMAGED))
		{
			break;
		}
	}
	if (i == 4) return false;
	return true;
}

void Stage::DamageFloor(DirectX::SimpleMath::Vector3 pos, float w, float h)
{
	int x, y;
	const DirectX::SimpleMath::Vector2 corner[4] = {
	DirectX::SimpleMath::Vector2(-1.0f, -1.0f),
	DirectX::SimpleMath::Vector2(1.0f, -1.0f),
	DirectX::SimpleMath::Vector2(1.0f, 1.0f),
	DirectX::SimpleMath::Vector2(-1.0f, 1.0f)
	};
	// ���S����̋����ɂ��邽�ߕ��ƍ������Q�Ŋ���
	w /= 2.0f;
	h /= 2.0f;
	int i = 0;
	for (; i < 4; i++)
	{
		// 3D ��Ԃ̍��W���}�b�v�`�b�v�̈ʒu�ɕϊ�����
		ConvertPosToMapChip(pos.x + w * corner[i].x, pos.z + h * corner[i].y, &x, &y);
		Floor* floor = this->GetFloor(x, y);
		if ((floor != nullptr)
			&& ((floor->GetState() == Floor::State::NORMAL) || (floor->GetState() == Floor::State::DAMAGED))
			)
		{
			floor->Damage();
		}
	}
}

void Stage::DeleteAllObject()
{
	if (m_player != nullptr)
	{
		m_player->Invalidate();
		m_player = nullptr;
	}

	for (auto parts : m_parts)
	{
		if (parts != nullptr)
		{
			parts->Invalidate();
			parts = nullptr;
		}
	}
	m_parts.clear();

	for (auto enemy : m_enemies)
	{
		if (enemy != nullptr)
		{
			enemy->Invalidate();
			enemy = nullptr;
		}
	}
	m_enemies.clear();
}

void Stage::ConvertPosToMapChip(float x, float z, int* floor_x, int* floor_y)
{
	*floor_x = (int)floorf(x + 0.5f);
	*floor_y = (int)floorf(z + 0.5f);
}

