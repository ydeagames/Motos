//======================================================
// File Name	: GameWindow.cpp
// Summary		: ゲームウィンドウ
// Date			: 2019/11/26
// Author		: Takafumi Ban
//======================================================
#include "pch.h"
#include "GameWindow.h"
#include "GameContext.h"
#include "DeviceResources.h"
#include "Camera.h"


GameWindow::GameWindow()
{
}

void GameWindow::Initialize()
{
	// Box生成
	m_pShape = DirectX::GeometricPrimitive::CreateBox(GameContext::Get<DX::DeviceResources>()->GetD3DDeviceContext(), DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f));
}

void GameWindow::Update(float elapsedTime)
{
}

void GameWindow::Render(const DirectX::SimpleMath::Matrix& viewMatrix, const DirectX::SimpleMath::Matrix& projectionMatrix)
{
	// カメラ取得
	DirectX::SimpleMath::Matrix world;
	world = DirectX::SimpleMath::Matrix::CreateTranslation(4.5f, 0.0f, 5.5f);
	auto camera = GameContext::Get<Camera>();
	m_pShape->Draw(world, camera->GetView(), camera->GetProjection());
}


