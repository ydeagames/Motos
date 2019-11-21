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
}

void InfoWindow::Initialize()
{
	// テクスチャの読み込み
	DirectX::CreateWICTextureFromFile(GameContext::Get<DX::DeviceResources>()->GetD3DDevice(), L"Resources/Textures/texture01.png", nullptr, m_texture01.GetAddressOf());
	DirectX::CreateWICTextureFromFile(GameContext::Get<DX::DeviceResources>()->GetD3DDevice(), L"Resources/Textures/bg01.png", nullptr, m_bg01Texture.GetAddressOf());
	//DirectX::CreateWICTextureFromFile(GameContext::Get<DX::DeviceResources>()->GetD3DDevice(), L"Resources/Textures/right.png", nullptr, m_bg01Texture.GetAddressOf());

	// ハイスコアタスク登録
	std::unique_ptr<Score> pHighScore = std::make_unique<Score>();
	m_pHighScore = pHighScore.get();
	m_pHighScore->Initialize(m_texture01.Get(), 24, 24, "HIGH SCORE", DirectX::SimpleMath::Color(191 / 255.0f, 194 / 255.0f, 245 / 255.0f), DirectX::Colors::Yellow, 0.2f);
	GameContext::Get<ObjectManager>()->GetInfoOM()->Add(std::move(pHighScore));
	// スコアタスク登録
	std::unique_ptr<Score> pScore = std::make_unique<Score>();
	m_pScore = pScore.get();
	m_pScore->Initialize(m_texture01.Get(), 24, 24 * 4, "1P", DirectX::Colors::Red, DirectX::Colors::White, 0.5f);
	GameContext::Get<ObjectManager>()->GetInfoOM()->Add(std::move(pScore));
	// パワーアップパーツ取得タスク登録
	std::unique_ptr<PartsInfo> pPowerup = std::make_unique<PartsInfo>();
	m_pPowerup = pPowerup.get();
	m_pPowerup->Initialize(m_texture01.Get(), 24, 24 * 21, PartsInfo::POWER_PARTS);
	GameContext::Get<ObjectManager>()->GetInfoOM()->Add(std::move(pPowerup));
	// ジャンプパーツタスク登録
	std::unique_ptr<PartsInfo> pJump = std::make_unique<PartsInfo>();
	m_pJump = pJump.get();
	m_pJump->Initialize(m_texture01.Get(), 24, 24 * 25, PartsInfo::JUMP_PARTS);
	GameContext::Get<ObjectManager>()->GetInfoOM()->Add(std::move(pJump));
	// ラウンドパーツ取得タスク登録
	std::unique_ptr<Round> pRound = std::make_unique<Round>();
	m_pRound = pRound.get();
	m_pRound->Initialize(m_texture01.Get(), 24 * 6, 24 * 28);
	GameContext::Get<ObjectManager>()->GetInfoOM()->Add(std::move(pRound));
	// ライフタスク登録
	std::unique_ptr<Life> pLife = std::make_unique<Life>();
	m_pLife = pLife.get();
	m_pLife->Initialize(m_texture01.Get(), 24, 24 * 28);
	GameContext::Get<ObjectManager>()->GetInfoOM()->Add(std::move(pLife));
	m_pLife->SetLife(3);

	m_pScore->BlinkTitle(true);
	m_pScore->SetScore(114514);
}

void InfoWindow::Update(float elapsedTime)
{
}

void InfoWindow::Render(const DirectX::SimpleMath::Matrix& viewMatrix, const DirectX::SimpleMath::Matrix& projectionMatrix)
{
	// 情報ウィンドウの背景の表示
	GameContext::Get<DirectX::SpriteBatch>()->Draw(m_bg01Texture.Get(), DirectX::SimpleMath::Vector2(0, 0));
}
