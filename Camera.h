#pragma once

class Camera
{
private:
	DirectX::SimpleMath::Vector3 m_target;
	DirectX::SimpleMath::Vector3 m_eye;
	DirectX::SimpleMath::Matrix m_view;
	DirectX::SimpleMath::Matrix m_projection;

public:
	void Initialize();
	DirectX::SimpleMath::Matrix getViewMatrix() const;
	DirectX::SimpleMath::Matrix getProjectionMatrix() const;
};

