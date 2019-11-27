//======================================================
// File Name	: GameWindow.h
// Summary		: ゲームウィンドウ
// Date			: 2019/11/26
// Author		: Takafumi Ban
//======================================================
#pragma once

#include "GameObject.h"
#include "GeometricPrimitive.h"

class GameWindow : public GameObject
{
public:
	// 画面サイズ
	static const int SCREEN_W = 960;
	static const int SCREEN_H = 720;

public:
	// コンストラクタ
	GameWindow();

	// 初期化関数
	void Initialize();

	// 更新
	virtual void Update(float elapsedTime);
	// 描画関数
	virtual void Render(const DirectX::SimpleMath::Matrix& viewMatrix, const DirectX::SimpleMath::Matrix& projectionMatrix);

private:
	// カメラテスト用モデル
	std::unique_ptr <DirectX::GeometricPrimitive> m_pShape;
};
