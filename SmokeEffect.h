//======================================================
// File Name	: SmokeEffect.h
// Summary		: 煙のエフェクト表示クラス
// Date			: 2020/01/15
// Author		: Takafumi Ban
//======================================================
#pragma once
#include "GameObject.h"

class GameWindow;

class SmokeEffect : public GameObject
{
private:
	// エフェクト表示時間
	static const float DISPLAY_TIME;

	// 表示位置
	DirectX::SimpleMath::Vector3 m_pos;

	// 表示時間カウンタ
	float m_timer;

	// アルファ値
	float m_alpha;

	// 移動速度
	DirectX::SimpleMath::Vector3 m_v;

public:
	// コンストラクタ
	SmokeEffect();
	
	// 初期化関数
	void Initialize(DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 v);

	// 更新関数
	void Update(float elapsedTime) override;

	// 描画関数
	void Render() override;
};