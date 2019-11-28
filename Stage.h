//======================================================
// File Name	: Stage.h
// Summary		: ステージクラス
// Date			: 2019/11/28
// Author		: Takafumi Ban
//======================================================
#pragma once

#include <vector>
#include "Floor.h"
#include <Model.h>

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
	void Initialize(GameWindow* gameWindow);

	// ゲームウインドウのポインタを取得する関数
	GameWindow* GetGameWindow() { return m_gameWindow; }

	// 床タスク取得関数
	Floor* GetFloor(int x, int y);

	// ステージデータの読み込み関数
	bool LoadStageData(wchar_t* fname);

	// ステージデータ初期化関数
	void SetStageData();

	// ステージデータのリセット関数
	void ResetStageData();

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
	// ゲームウインドウへのポインタ
	GameWindow* m_gameWindow;

	//----- ゲームオブジェクトへのポインタ -----//

	// 床オブジェクトへのポインタ
	Floor* m_floors[STAGE_H][STAGE_W];

	//----- モデルへのポインタ -----//

	// 床のモデル
	std::unique_ptr<DirectX::Model> m_floorModels[Floor::STATE_NUM];

	// ステージデータ
	StageData m_stageData;
};
