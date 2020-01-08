#pragma once

#include "GameObject.h"

class Score;
class PartsInfo;
class Round;
class Life;

class InfoWindow : public GameObject
{
public:
	// 画面サイズ
	static const int SCREEN_W = 1280 - 960;
	static const int SCREEN_H = 720;
	static const int TILE_W = 24;
	static const int TILE_H = 24;
	constexpr static const wchar_t* TEXTURE_ATLAS = L"Resources/Textures/texture01.png";
	constexpr static const wchar_t* TEXTURE_BACKGROUND = L"Resources/Textures/bg01.png";

public:
	InfoWindow();
	~InfoWindow();

	void Initialize();
	virtual void Update(float elapsedTime);
	virtual void Render();

	// ハイスコアにアクセスする関数
	Score* GetHighScore() { return m_pHighScore; }
	// スコアにアクセスする関数
	Score* GetScore() { return m_pScore; }
	// パワーアップパーツにアクセスする関数
	PartsInfo* GetPowerParts() { return m_pPowerup; }
	// ジャンプパーツにアクセスする関数
	PartsInfo* GetJumpParts() { return m_pJump; }
	// ラウンドにアクセスする関数
	Round* GetRound() { return m_pRound; }
	// ライフにアクセスする関数
	Life* GetLife() { return m_pLife; }

	// 追加
	template<typename T>
	T* AddObject()
	{
		std::unique_ptr<T> pObject = std::make_unique<T>();
		auto raw = pObject.get();
		GameContext::Get<ObjectManager>()->GetInfoOM()->Add(std::move(pObject));
		return raw;
	}

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
	// ラウンド
	Round* m_pRound;
	// ライフ
	Life* m_pLife;
};

