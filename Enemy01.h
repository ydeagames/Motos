//======================================================
// File Name	: Enemy01.h
// Summary		: プレイヤークラス
// Date			: 2019/12/05
// Author		: Takafumi Ban
//======================================================
#pragma once

#include "Object.h"
#include "Keyboard.h"

class Enemy01 : public Object
{
public:
	// プレイヤーのタイプ別モデル
	enum ModelType
	{
		NORMAL,	// 通常

		MODEL_TYPE_NUM
	};

	// 敵の状態
	enum STATE
	{
		STATE_NORMAL, // 通常
		STATE_HIT, // 吹き飛ばされ状態
		STATE_FALL, // 落下中
		STATE_DEAD, // 死亡
	};

	// 床との判定用の幅と高さ
	static const float WIDTH;
	static const float HEIGHT;

	static const float RADIUS;
	
	// プレイヤーの最大移動速度
	static const float MAX_SPEED;

	// プレイヤーの重さ
	static const float WEIGHT;

	// 床に対する摩擦係数
	static const float FRICTION;

	// 思考間隔（単位：秒）
	static const float THINK_INTERVAL;


protected:
	// モデルデータへのポインタ
	DirectX::Model* m_models[MODEL_TYPE_NUM];

	// プレイヤーの状態
	STATE m_state;

	// 思考タイマー
	float m_thinkTimer;


public:
	// コンストラクタ
	Enemy01();

	// 初期化関数
	void Initialize(int x, int y);

	// モデル設定関数
	void SetModel(ModelType modelType, DirectX::Model* model);

	// オブジェクトＩＤを取得する関数
	Object::OBJECT_ID GetID() { return Object::PLAYER; }

	// 更新関数
	void Update(float elapsedTime) override;

	// 描画関数
	void Render() override;

	// 更新処理（通常）
	virtual void State_Normal(float elapsedTime);
	// 更新処理（衝突して制御不能中）
	virtual void State_Hit(float elapsedTime);

	// 更新処理（落下中）
	virtual void State_Fall(float elapsedTime);

	// 衝突したら呼ばれる関数
	void OnCollision(GameObject* object);

	// プレイヤーの状態を取得する関数
	Enemy01::STATE GetState();

	// リセット関数
	void Reset();
};
