#include "pch.h"

#include "PlayState.h"

#include <Keyboard.h>

#include "DebugFont.h"
#include "DeviceResources.h"
#include "GameContext.h"
#include "GameStateManager.h"



PlayState::PlayState()
	: GameState()
{
}



PlayState::~PlayState()
{
}



void PlayState::Initialize()
{
	DirectX::CommonStates* state           = GameContext::Get<DirectX::CommonStates>();
	DX::DeviceResources*   deviceResources = GameContext::Get<DX::DeviceResources>();
	ID3D11Device*          device          = deviceResources->GetD3DDevice();
	ID3D11DeviceContext*   deviceContext   = deviceResources->GetD3DDeviceContext();
}



void PlayState::Update()
{
	DirectX::Keyboard::State keyState = DirectX::Keyboard::Get().GetState();
	if (keyState.IsKeyDown(DirectX::Keyboard::P))
	{
		GameStateManager* gameStateManager = GameContext::Get<GameStateManager>();
		gameStateManager->PushState("Pause");
	}
}



void PlayState::Render()
{
	DX::DeviceResources* deviceResources = GameContext::Get<DX::DeviceResources>();

	DebugFont* debugFont = DebugFont::GetInstance();
	debugFont->print(10, 10, L"PlayState");
	debugFont->draw();
	debugFont->print(10, 40, L"[P] Pause");
	debugFont->draw();
}



void PlayState::Finalize()
{
}
