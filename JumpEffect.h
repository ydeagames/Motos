//======================================================
// File Name	: JumpEffect.h
// Summary		: ジャンプ時のエフェクト表示クラス
// Date			: 2020/01/15
// Author		: Takafumi Ban
//======================================================
#pragma once

#include "GameObject.h"

class GameWindow;

class JumpEffect : public GameObject
{
	// エフェクト表示時間
	static const float DISPLAY_TIME;

	// 表示位置
	DirectX::SimpleMath::Vector3 m_pos;

	// 表示時間カウンタ
	float m_timer;

public:
	// コンストラクタ
	JumpEffect();

	// 初期化関数
	void Initialize(DirectX::SimpleMath::Vector3 pos);

	// 更新関数
	void Update(float elapsedTime) override;

	// 描画関数
	void Render() override;
};
