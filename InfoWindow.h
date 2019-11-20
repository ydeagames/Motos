#pragma once

#include "GameObject.h"

class Score;
class PartsInfo;

class InfoWindow : public GameObject
{
public:
	// 画面サイズ
	static const int SCREEN_W = 1280 - 960;
	static const int SCREEN_H = 720;

public:
	InfoWindow();
	~InfoWindow();

	void Initialize();
	virtual void Update(float elapsedTime);
	virtual void Render(const DirectX::SimpleMath::Matrix& viewMatrix, const DirectX::SimpleMath::Matrix& projectionMatrix);

	// ハイスコアにアクセスする関数
	Score* GetHighScore() { return m_pHighScore; }
	// スコアにアクセスする関数
	Score* GetScore() { return m_pScore; }
	// パワーアップパーツにアクセスする関数
	PartsInfo* GetPowerParts() { return m_pPowerup; }
	// ジャンプパーツにアクセスする関数
	PartsInfo* GetJumpParts() { return m_pJump; }

private:
	// テクスチャハンドル（背景）
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_bg01Texture;
	// テクスチャハンドル（背景）
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture01;

	// ハイスコア
	Score* m_pHighScore;
	// スコア
	Score* m_pScore;
	// パーツ情報（パワーアップ）
	PartsInfo* m_pPowerup;
	// パーツ情報（ジャンプ）
	PartsInfo* m_pJump;
};

