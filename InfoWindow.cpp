#include "pch.h"
#include "InfoWindow.h"
#include "GameContext.h"
#include "DeviceResources.h"
#include "GameObjectManager.h"
#include "ObjectManager.h"
#include "Score.h"

InfoWindow::InfoWindow()
{
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
