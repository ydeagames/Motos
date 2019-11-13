#pragma once

#include "GameObject.h"

class InfoWindow : public GameObject
{
public:
	// ��ʃT�C�Y
	static const int SCREEN_W = 1280 - 960;
	static const int SCREEN_H = 720;

public:
	InfoWindow();

	void Initialize();
	virtual void Update(float elapsedTime);
	virtual void Render(const DirectX::SimpleMath::Matrix& viewMatrix, const DirectX::SimpleMath::Matrix& projectionMatrix);

private:
	// �e�N�X�`���n���h���i�w�i�j
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_bg01Texture;
	// �e�N�X�`���n���h���i�w�i�j
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture01;
};

