#include "pch.h"
#include "InfoWindow.h"
#include "GameContext.h"
#include "DeviceResources.h"
#include "GameObjectManager.h"
#include "ObjectManager.h"
#include "Score.h"
#include "PartsInfo.h"
#include "Round.h"
#include "Life.h"

InfoWindow::InfoWindow()
{
}

InfoWindow::~InfoWindow()
{
	if (m_pHighScore)
		m_pHighScore->Invalidate();
	if (m_pScore)
		m_pScore->Invalidate();
	if (m_pPowerup)
		m_pPowerup->Invalidate();
	if (m_pJump)
		m_pJump->Invalidate();
	if (m_pRound)
		m_pRound->Invalidate();
	if (m_pLife)
		m_pLife->Invalidate();
}

void InfoWindow::Initialize()
{
	// テクスチャの読み込み
	DirectX::CreateWICTextureFromFile(GameContext::Get<DX::DeviceResources>()->GetD3DDevice(), TEXTURE_ATLAS, nullptr, m_texture01.GetAddressOf());
	DirectX::CreateWICTextureFromFile(GameContext::Get<DX::DeviceResources>()->GetD3DDevice(), TEXTURE_BACKGROUND, nullptr, m_bg01Texture.GetAddressOf());
	//DirectX::CreateWICTextureFromFile(GameContext::Get<DX::DeviceResources>()->GetD3DDevice(), L"Resources/Textures/right.png", nullptr, m_bg01Texture.GetAddressOf());

	// ハイスコアタスク登録
	(m_pHighScore = AddObject<Score>())
		->Initialize(m_texture01.Get(), TILE_W * 1, TILE_H * 2, "HIGH SCORE", DirectX::SimpleMath::Color(191 / 255.0f, 194 / 255.0f, 245 / 255.0f), DirectX::Colors::Yellow, 0.2f);
	// スコアタスク登録
	(m_pScore = AddObject<Score>())
		->Initialize(m_texture01.Get(), TILE_W * 1, TILE_H * 4, "1P", DirectX::Colors::Red, DirectX::Colors::White, 0.5f);
	// パワーアップパーツ取得タスク登録
	(m_pPowerup = AddObject<PartsInfo>())
		->Initialize(m_texture01.Get(), TILE_W * 1, TILE_H * 21, PartsInfo::POWER_PARTS);
	// ジャンプパーツタスク登録
	(m_pJump = AddObject<PartsInfo>())
		->Initialize(m_texture01.Get(), TILE_W * 1, TILE_H * 25, PartsInfo::JUMP_PARTS);
	// ラウンドパーツ取得タスク登録
	(m_pRound = AddObject<Round>())
		->Initialize(m_texture01.Get(), TILE_W * 6, TILE_H * 28);
	// ライフタスク登録
	(m_pLife = AddObject<Life>())
		->Initialize(m_texture01.Get(), TILE_W * 1, TILE_H * 28);

	// 設定
	m_pRound->SetRound(1);
	m_pLife->SetLife(3);
	m_pScore->BlinkTitle(true);
}

void InfoWindow::Update(float elapsedTime)
{
	//static int score = 0;
	//m_pScore->SetScore(score++);
}

void InfoWindow::Render()
{
	// 情報ウィンドウの背景の表示
	GameContext::Get<DirectX::SpriteBatch>()->Draw(m_bg01Texture.Get(), DirectX::SimpleMath::Vector2(0, 0));
}
