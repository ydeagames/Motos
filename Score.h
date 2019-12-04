//======================================================
// File Name	: Score.h
// Summary		: スコアクラス
// Date			: 2019/11/13
// Author		: Takafumi Ban
//======================================================
#pragma once

#include "GameObject.h"
#include "Moji.h"
#include "Blink.h"

class Game;

class Score : public GameObject
{
public:
	// コンストラクタ
	Score();

	// デストラクタ
	~Score();

	// 初期化関数
	void Initialize(ID3D11ShaderResourceView* texture, int x, int y, const char* title, DirectX::FXMVECTOR titleColor = DirectX::Colors::White, DirectX::FXMVECTOR scoreColor = DirectX::Colors::White, float blinkTime = 0.0f);

	// 更新関数
	virtual void Update(float elapsedTime);
	// 描画
	virtual void Render();

	// スコアの設定関数
	void SetScore(int score);

	// タイトル文字の点滅のON/OFF関数
	void BlinkTitle(bool flag);

private:
	// タイトル文字列
	Moji m_titleMoji;

	// 点数文字列
	Moji m_scoreMoji;

	// 点滅管理オブジェクト
	std::unique_ptr<Blink> m_pBlink;
};
