#include "pch.h"
#include "Camera.h"
#include "GameWindow.h"

void Camera::Initialize()
{
	// �����l�ݒ�
	m_target = DirectX::SimpleMath::Vector3(4.5f, 0, 5.5f);
	m_eye = DirectX::SimpleMath::Vector3(4.5f, 10.5f, 11.0f);
	// ��L 2 �� UP ���g���r���[�s��쐬
	m_view = DirectX::SimpleMath::Matrix::CreateLookAt(m_eye, m_target, DirectX::SimpleMath::Vector3::Up);
	// �ˉe�s����쐬���邪�A�K�v�Ȉ����͈ȉ��̒ʂ�
	m_projection = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		DirectX::XMConvertToRadians(45.0f),
		GameWindow::SCREEN_W / static_cast<float>(GameWindow::SCREEN_H),
		0.1f,
		200.0f);
}

DirectX::SimpleMath::Matrix Camera::getViewMatrix() const
{
	return m_view;
}

DirectX::SimpleMath::Matrix Camera::getProjectionMatrix() const
{
	return m_projection;
}
