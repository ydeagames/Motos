#include "pch.h"
#include "Camera.h"
#include "GameWindow.h"

void Camera::Initialize()
{
	// 初期値設定
	m_target = DirectX::SimpleMath::Vector3(4.5f, 0, 5.5f);
	m_eye = DirectX::SimpleMath::Vector3(4.5f, 10.5f, 11.0f);
	// 上記 2 つと UP を使いビュー行列作成
	m_view = DirectX::SimpleMath::Matrix::CreateLookAt(m_eye, m_target, DirectX::SimpleMath::Vector3::Up);
	// 射影行列も作成するが、必要な引数は以下の通り
	m_projection = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		DirectX::XMConvertToRadians(45.0f),
		GameWindow::SCREEN_W / static_cast<float>(GameWindow::SCREEN_H),
		0.1f,
		200.0f);
}

DirectX::SimpleMath::Matrix Camera::GetView() const
{
	return m_view;
}

DirectX::SimpleMath::Matrix Camera::GetProjection() const
{
	return m_projection;
}
