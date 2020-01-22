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

	m_objectManager = std::make_unique<ObjectManager>();
	GameContext::Register<ObjectManager>(m_objectManager);

	// ビューポートの矩形領域の設定（ゲーム画面）
	m_viewportGame = CD3D11_VIEWPORT(0.0f, 0.0f, static_cast<float>(960), static_cast<float>(720));
	// ビューポートの矩形領域の設定（情報画面）
	m_viewportInfo = CD3D11_VIEWPORT(static_cast<float>(960), 0.0f, static_cast<float>(InfoWindow::SCREEN_W), static_cast<float>(InfoWindow::SCREEN_H));

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
}



void PlayState::Update(float elapsedTime)
{
	DirectX::Keyboard::State keyState = DirectX::Keyboard::Get().GetState();
	if (keyState.IsKeyDown(DirectX::Keyboard::P))
	{
		GameStateManager* gameStateManager = GameContext::Get<GameStateManager>();
		gameStateManager->PushState("Pause");
	}

	m_objectManager->GetGameOM()->Update(elapsedTime);
	m_objectManager->GetInfoOM()->Update(elapsedTime);
}



void PlayState::Render()
{
	DX::DeviceResources* deviceResources = GameContext::Get<DX::DeviceResources>();

	{
		auto context = deviceResources->GetD3DDeviceContext();
		auto sprites = GameContext::Get<DirectX::SpriteBatch>();
		auto states = GameContext::Get<DirectX::CommonStates>();
		
		//----------------------//
		// ゲーム画面の描画 //
		//----------------------//
		// ビューポートを変更する（左側へ描画エリアを変更する）
		context->RSSetViewports(1, &m_viewportGame);
		sprites->Begin(DirectX::SpriteSortMode_Deferred, states->NonPremultiplied());
		// TODO: ビュー行列とプロジェクション行列を設定
		//SimpleMath::Matrix viewMat, projMat;
		// ゲーム画面のオブジェクト描画
		m_objectManager->GetGameOM()->Render();
		sprites->End(); // <---スプライトの描画はここでまとめて行われている
		//------------------------------//
		// ゲーム画面の描画（ここまで） //
		//------------------------------//
		//------------------//
		// 情報画面の描画 //
		//------------------//
		// ビューポートを変更する（右側へ描画エリアを変更する）
		context->RSSetViewports(1, &m_viewportInfo);
		sprites->Begin(DirectX::SpriteSortMode_Deferred, states->NonPremultiplied());
		// 情報画面のオブジェクト描画
		m_objectManager->GetInfoOM()->Render();
		sprites->End(); // <---スプライトの描画はここでまとめて行われている
		//------------------------------//
		// 情報画面の描画（ここまで） //
		//------------------------------//
		// ビューポートを変更する（画面全体）
		auto viewport = deviceResources->GetScreenViewport();
		context->RSSetViewports(1, &viewport);
	}

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
	
	GameContext::Reset<ObjectManager>();
	m_objectManager.reset();
}
