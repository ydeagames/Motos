//======================================================
// File Name	: Bg.h
// Summary		: 背景クラス
// Date			: 2019/11/28
// Author		: Takafumi Ban
//======================================================
#pragma once

#include "GameObject.h"
#include <Model.h>

class GameWindow;

class Bg : public GameObject
{
	// ゲームウインドウへのポインタ
	GameWindow* m_gameWindow;

	// モデルデータへのポインタ
	std::unique_ptr<DirectX::Model> m_model;

	// 回転角
	float m_angle;

public:
	// コンストラクタ
	Bg();

	// 初期化関数
	void Initialize(GameWindow* gameWindow);

	// 更新
	virtual void Update(float elapsedTime);
	// 描画関数
	virtual void Render(const DirectX::SimpleMath::Matrix& viewMatrix, const DirectX::SimpleMath::Matrix& projectionMatrix);
};