#include "pch.h"

#include "PlayState.h"

#include <Keyboard.h>

#include "DebugFont.h"
#include "DeviceResources.h"
#include "GameContext.h"
#include "GameStateManager.h"
#include "InfoWindow.h"
#include "GameObjectManager.h"
#include "ObjectManager.h"
#include "GameWindow.h"
#include "GameAI.h"


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

	{
		// ゲームウィンドウ生成
		m_pGameWindow = std::make_unique<GameWindow>();
		// 生ポインタを登録
		GameContext::Register<GameWindow>(m_pGameWindow.get());
		// ゲームウィンドウを登録
		GameContext::Get<ObjectManager>()->GetGameOM()->Add(std::move(m_pGameWindow));
		// ゲームウィンドウ初期化
		GameContext::Get<GameWindow>()->Initialize();
	}
	{
		// 情報ウィンドウ生成
		m_pInfoWindow = std::make_unique<InfoWindow>();
		// 生ポインタを登録
		GameContext::Register<InfoWindow>(m_pInfoWindow.get());
		// 情報ウィンドウを登録
		GameContext::Get<ObjectManager>()->GetInfoOM()->Add(std::move(m_pInfoWindow));
		// 情報ウィンドウ初期化
		GameContext::Get<InfoWindow>()->Initialize();
	}
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

	debugFont->print(10, 100, L"Calc: %d", GameContext::Get<GameAI>()->calcNum);
	debugFont->draw();
	debugFont->print(10, 130, L"Data: %d", GameContext::Get<GameAI>()->m_data.size());
	debugFont->draw();
}



void PlayState::Finalize()
{
	{
		// 消えるフラグを設定
		GameContext::Get<GameWindow>()->Invalidate();
		// 消えるフラグを設定
		GameContext::Get<InfoWindow>()->Invalidate();
	}
	{
		// GameContextからはずす
		GameContext::Reset<GameWindow>();
		// GameContextからはずす
		GameContext::Reset<InfoWindow>();
	}
}
