#include "pch.h"
#include "InfoWindow.h"
#include "GameContext.h"
#include "DeviceResources.h"

InfoWindow::InfoWindow()
{
}

void InfoWindow::Initialize()
{
	// テクスチャの読み込み
	DirectX::CreateWICTextureFromFile(GameContext::Get<DX::DeviceResources>()->GetD3DDevice(), L"Resources/Textures/texture01.png", nullptr, m_texture01.GetAddressOf());
	DirectX::CreateWICTextureFromFile(GameContext::Get<DX::DeviceResources>()->GetD3DDevice(), L"Resources/Textures/right.png", nullptr, m_bg01Texture.GetAddressOf());
}

void InfoWindow::Update(float elapsedTime)
{
}

void InfoWindow::Render(const DirectX::SimpleMath::Matrix& viewMatrix, const DirectX::SimpleMath::Matrix& projectionMatrix)
{
	// 情報ウィンドウの背景の表示
	GameContext::Get<DirectX::SpriteBatch>()->Draw(m_bg01Texture.Get(), DirectX::SimpleMath::Vector2(0, 0));
}
