//======================================================
// File Name	: Stage.h
// Summary		: ステージクラス
// Date			: 2019/11/28
// Author		: Takafumi Ban
//======================================================
#pragma once

#include <vector>
#include "Floor.h"
#include "Player.h"
#include "Parts.h"
#include "Enemy01.h"
#include "Enemy02.h"
#include "Enemy03.h"

class GameWindow;

class Stage
{
public:
	// オブジェクトＩＤ
	enum OBJECT_ID
	{
		NONE,			// なし
		PLAYER,			// プレイヤー
		POWERUP_PARTS,	// パワーアップパーツ
		JUMP_PARTS,		// ジャンプパーツ
		ENEMY_1,		// 敵１
		ENEMY_2,		// 敵２
		ENEMY_3,		// 敵３
	};
	
	// ステージの大きさ
	static const int STAGE_W = 10;
	static const int STAGE_H = 10;

	// ステージデータの構造体
	struct StageData
	{
		int stage[STAGE_H][STAGE_W];
		int object[STAGE_H][STAGE_W];
	};

public:
	// コンストラクタ
	Stage();
	// デストラクタ
	~Stage();

	// 初期化関数
	void Initialize();

	// 床タスク取得関数
	Floor* GetFloor(int x, int y);

	// ステージデータの読み込み関数
	bool LoadStageData(wchar_t* fname);

	// ステージデータ初期化関数
	void SetStageData();

	// ステージデータのリセット関数
	void ResetStageData();

	// プレイヤー取得関数
	Player* GetPlayer();

	// 敵
	const std::vector<Object*>& GetEnemyList();
	Enemy01* GetEnemy01();
	Enemy02* GetEnemy02();
	Enemy03* GetEnemy03();

private:
	// 落下チェック関数
	bool CheckFloor(DirectX::SimpleMath::Vector3 pos, float w, float h);

	// 床にダメージを与える関数
	void DamageFloor(DirectX::SimpleMath::Vector3 pos, float w, float h);

	// 登録されているタスク（プレイヤー、敵、パーツ）を全て削除する関数
	void DeleteAllObject();

	// 位置からステージのマップチップの位置に変換する関数
	void ConvertPosToMapChip(float x, float z, int* floor_x, int* floor_y);

private:
	//----- ゲームオブジェクトへのポインタ -----//

	// 床オブジェクトへのポインタ
	Floor* m_floors[STAGE_H][STAGE_W];

	//----- モデルへのポインタ -----//

	// 床のモデル
	std::unique_ptr<DirectX::Model> m_floorModels[Floor::STATE_NUM];

	// ステージデータ
	StageData m_stageData;

	// プレイヤーのモデル
	std::unique_ptr<DirectX::Model> m_playerModels[Player::MODEL_TYPE_NUM];

	// プレイヤー
	Player* m_player = nullptr;

	// パーツのモデル
	std::unique_ptr<DirectX::Model> m_partsModels[Parts::KIND_NUM];

	// パーツ
	std::vector<Parts*> m_parts;

	// 敵のモデル
	std::unique_ptr<DirectX::Model> m_enemyModels[Enemy01::MODEL_TYPE_NUM];
	
	// 影のモデル
	std::unique_ptr<DirectX::Model> m_shadowModel;

	// 敵
	std::vector<Object*> m_enemies;
	Enemy01* m_enemy01 = nullptr;
	Enemy02* m_enemy02 = nullptr;
	Enemy03* m_enemy03 = nullptr;
};
