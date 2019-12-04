//======================================================
// File Name	: Stage.
// Summary		: ステージクラス
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

Stage::Stage()
	: m_floors{nullptr}, m_stageData{0}
{
}

Stage::~Stage()
{
}

void Stage::Initialize()
{
	// モデルデータの読み込み
	DirectX::EffectFactory fx(GameContext::Get<DX::DeviceResources>()->GetD3DDevice());
	fx.SetDirectory(L"Resources\\Models");
	m_floorModels[Floor::NORMAL] = DirectX::Model::CreateFromCMO(GameContext::Get<DX::DeviceResources>()->GetD3DDevice(), L"Resources\\Models\\floorPanel_00.cmo", fx);
	m_floorModels[Floor::DAMAGED] = DirectX::Model::CreateFromCMO(GameContext::Get<DX::DeviceResources>()->GetD3DDevice(), L"Resources\\Models\\floorPanel_01.cmo", fx);

	// 床のタスク生成
	for (int j = 0; j < STAGE_H; j++)
	{
		for (int i = 0; i < STAGE_W; i++)
		{
			std::unique_ptr<Floor> pFloor = std::make_unique<Floor>();
			m_floors[j][i] = pFloor.get();
			m_floors[j][i]->Initialize(this, i, j);
			GameContext::Get<ObjectManager>()->GetGameOM()->Add(std::move(pFloor));
			// 各状態のモデルを設定
			m_floors[j][i]->SetModel(Floor::NORMAL, m_floorModels[Floor::NORMAL].get());
			m_floors[j][i]->SetModel(Floor::DAMAGED, m_floorModels[Floor::DAMAGED].get());
			m_floors[j][i]->SetModel(Floor::FALL, m_floorModels[Floor::DAMAGED].get());
		}
	}
}

Floor * Stage::GetFloor(int x, int y)
{
	if (x >= 0 && x < STAGE_W && y >= 0 && y < STAGE_H)
	{
		return m_floors[y][x];
	}
	return nullptr;
}

bool Stage::LoadStageData(wchar_t * fname)
{
	std::wstring str;
	// ファイルのオープン
	std::wifstream ifs(fname);
	//----- ステージデータ -----//
	// ヘッダ文字列
	std::wstring stageHeadName = L"STAGE";
	// ヘッダ部分までシーク
	while (!ifs.eof())
	{
		getline(ifs, str);
		if (str.compare(0, stageHeadName.size(), stageHeadName) == 0)
		{
			break;
		}
	}
	// ステージデータの読み込み
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
	//----- ステージ上のオブジェクトデータ -----//
	// ヘッダ文字列
	std::wstring objectHeadName = L"OBJECT";
	// ファイルポインタを先頭へシーク
	ifs.clear();
	ifs.seekg(0);
	// ヘッダ部分までシーク
	while (!ifs.eof())
	{
		getline(ifs, str);
		if (str.compare(0, objectHeadName.size(), objectHeadName) == 0)
		{
			break;
		}
	}
	// オブジェクトデータの読み込み
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
	// ファイルのクローズ
	ifs.close();
	return true;
}

void Stage::SetStageData()
{
	// 床を元の状態に戻す
	for (int j = 0; j < STAGE_H; j++)
	{
		for (int i = 0; i < STAGE_W; i++)
		{
			m_floors[j][i]->SetState(static_cast<Floor::State>(m_stageData.stage[j][i]));
		}
	}

	// 全てのオブジェクトタスクの削除
	DeleteAllObject();

	//----- 各ゲームオブジェクトの生成 -----//

	// ステージ上のオブジェクトを配置する
	for (int j = 0; j < STAGE_H; j++)
	{
		for (int i = 0; i < STAGE_W; i++)
		{
			// オブジェクトの種類によりタスク生成する
			switch (m_stageData.object[j][i])
			{
			case OBJECT_ID::PLAYER:	// プレイヤー
				break;

			case OBJECT_ID::POWERUP_PARTS:	// パワーアップパーツ
				break;

			case OBJECT_ID::JUMP_PARTS:		// ジャンプパーツ
				break;

			case OBJECT_ID::ENEMY_1:	// 敵１
				break;

			case OBJECT_ID::ENEMY_2:	// 敵２
				break;
			}
		}
	}
}

void Stage::ResetStageData()
{
	// 床をリセット
	for (int j = 0; j < STAGE_H; j++)
	{
		for (int i = 0; i < STAGE_W; i++)
		{
			m_floors[j][i]->Reset();
		}
	}
}

bool Stage::CheckFloor(DirectX::SimpleMath::Vector3 pos, float w, float h)
{
	return true;
}

void Stage::DamageFloor(DirectX::SimpleMath::Vector3 pos, float w, float h)
{
}

void Stage::DeleteAllObject()
{
}

void Stage::ConvertPosToMapChip(float x, float z, int* floor_x, int* floor_y)
{
	*floor_x = (int)floorf(x + 0.5f);
	*floor_y = (int)floorf(z + 0.5f);
}

