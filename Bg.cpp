//======================================================
// File Name	: Bg.cpp
// Summary		: �w�i�N���X
// Date			: 2019/11/28
// Author		: Takafumi Ban
//======================================================
#include "pch.h"
#include "Bg.h"
#include "GameWindow.h"
#include <Effects.h>
#include <CommonStates.h>
#include "GameContext.h"
#include "DeviceResources.h"
#include "Camera.h"


Bg::Bg()
	: m_angle(0.0f)
{
	SetDrawPrio(GameWindow::DRAW_BG);
}

void Bg::Initialize()
{
	m_gameWindow = GameContext::Get<GameWindow>();

	// ���f���f�[�^�̓ǂݍ���
	DirectX::EffectFactory fx(GameContext::Get<DX::DeviceResources>()->GetD3DDevice());
	fx.SetDirectory(L"Resources\\Models");
	m_model = DirectX::Model::CreateFromCMO(GameContext::Get<DX::DeviceResources>()->GetD3DDevice(), L"Resources\\Models\\bg.cmo", fx);

	m_model->UpdateEffects([&](DirectX::IEffect* effect)
		{
			DirectX::IEffectLights* lights = dynamic_cast<DirectX::IEffectLights*>(effect);
			if (lights)
			{
				// ���C�g�̉e�����Ȃ���
				lights->SetAmbientLightColor(DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f));
				lights->SetLightEnabled(0, false);
				lights->SetLightEnabled(1, false);
				lights->SetLightEnabled(2, false);
			}
			DirectX::BasicEffect* basicEffect = dynamic_cast<DirectX::BasicEffect*>(effect);
			if (basicEffect)
			{
				// �G�~�b�V�����F�𔒂ɐݒ肷��
				basicEffect->SetEmissiveColor(DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f));
			}
		});


}

void Bg::Update(float elapsedTime)
{
	m_angle += 0.0002f;
}

void Bg::Render()
{
	DirectX::SimpleMath::Matrix world = DirectX::SimpleMath::Matrix::CreateRotationY(m_angle);
	world *= DirectX::SimpleMath::Matrix::CreateTranslation(4.5f, -10.0f, 5.5f);
	DirectX::SimpleMath::Matrix view = DirectX::SimpleMath::Matrix::CreateLookAt(DirectX::SimpleMath::Vector3(0, 0, 0), DirectX::SimpleMath::Vector3(0, 0, -1), DirectX::SimpleMath::Vector3::Up);

	// ���f���̕`��
	m_model->Draw(
		GameContext::Get<DX::DeviceResources>()->GetD3DDeviceContext(),
		*GameContext::Get<DirectX::CommonStates>(),
		world, view, m_gameWindow->GetCamera()->getProjectionMatrix());
}
