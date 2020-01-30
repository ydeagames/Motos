//
// Game.cpp
//

#include "pch.h"
#include "Game.h"

#include "ObjectManager.h"
#include "GameObjectManager.h"
#include "CollisionManager.h"
#include "GameContext.h"
#include "GameStateManager.h"
#include "DebugFont.h"
#include "InfoWindow.h"
#include "Camera.h"
#include "GameWindow.h"

#include "PlayState.h"
#include "TitleState.h"
#include "PauseState.h"
#include "ResultState.h"
#include "GameAI.h"
#include "EffectMask.h"
#include "ADX2Le.h"

extern void ExitGame();

using namespace DirectX;

using Microsoft::WRL::ComPtr;

Game::Game() noexcept(false)
{
	m_deviceResources = std::make_unique<DX::DeviceResources>();
	m_deviceResources->RegisterDeviceNotify(this);
}

Game::~Game()
{
	m_adx2Le->Finalize();
}

// Initialize the Direct3D resources required to run.
void Game::Initialize(HWND window, int width, int height)
{
	// ウィンドウ
	m_window = window;

	// マウスの作成
	m_mouse = std::make_unique<Mouse>();
	m_mouse->SetWindow(window);

	m_keyboard = std::make_unique<Keyboard>();


	m_deviceResources->SetWindow(window, width, height);

	m_deviceResources->CreateDeviceResources();
	CreateDeviceDependentResources();

	// コモンステート作成
	m_state = std::make_unique<CommonStates>(m_deviceResources->GetD3DDevice());
	GameContext::Register<DirectX::CommonStates>(m_state);
	m_deviceResources->CreateWindowSizeDependentResources();
	CreateWindowSizeDependentResources();

	// TODO: Change the timer settings if you want something other than the default variable timestep mode.
	// e.g. for 60 FPS fixed timestep update logic, call:
	/*
	m_timer.SetFixedTimeStep(true);
	m_timer.SetTargetElapsedSeconds(1.0 / 60);
	*/


	// ADX
	m_adx2Le = std::make_unique<Adx2Le>();
	m_adx2Le->Initialize(L"Resources/Sounds/MotosSound.acf");

	m_adx2Le->LoadAcbFile(L"Resources/Sounds/WorkUnit_0/CueSheet_0.acb");
	GameContext::Register<Adx2Le>(m_adx2Le);


	DebugFont* debugFont = DebugFont::GetInstance();
	debugFont->create(m_deviceResources->GetD3DDevice(), m_deviceResources->GetD3DDeviceContext());


	m_gameStateManager = std::make_unique<GameStateManager>();
	m_gameStateManager->RegisterState<TitleState>("Title");
	m_gameStateManager->RegisterState<PlayState>("Play");
	m_gameStateManager->RegisterState<PauseState>("Pause");
	m_gameStateManager->RegisterState<ResultState>("Result");
	m_gameStateManager->SetStartState("Title");


	m_collisionManager = std::make_unique<CollisionManager>();
	m_collisionManager->AllowCollision("Player", "Enemy01");
	m_collisionManager->AllowCollision("Player", "Enemy02");
	m_collisionManager->AllowCollision("Player", "PowerupParts");
	m_collisionManager->AllowCollision("Player", "JumpParts");
	m_collisionManager->AllowCollision("Enemy01", "Enemy01");
	m_collisionManager->AllowCollision("Enemy02", "Enemy02");
	m_collisionManager->AllowCollision("Enemy01", "Enemy02");


	m_gameAI = std::make_unique<GameAI>();
	m_gameAI->Initialize();


	GameContext::Register<GameStateManager>(m_gameStateManager);
	GameContext::Register<DX::DeviceResources>(m_deviceResources);
	GameContext::Register<CollisionManager>(m_collisionManager);
	GameContext::Register<GameAI>(m_gameAI);
	
	m_effectMask = std::make_unique<EffectMask>();
	m_effectMask->Initialize(1.f);
	GameContext::Register<EffectMask>(m_effectMask);

	//GameContext::Get<GameObjectManager>()->Add(std::make_unique<Ball>());
}

#pragma region Frame Update
// Executes the basic game loop.
void Game::Tick()
{
	m_timer.Tick([&]()
		{
			Update(m_timer);
		});

	Render();
}

// Updates the world.
void Game::Update(DX::StepTimer const& timer)
{
	float elapsedTime = float(timer.GetElapsedSeconds());

	// TODO: Add your game logic here.
	elapsedTime;
	m_gameStateManager->Update(elapsedTime);
	m_collisionManager->DetectCollision();
	m_effectMask->Update(elapsedTime);

	m_adx2Le->Update();
}
#pragma endregion

#pragma region Frame Render
// Draws the scene.
void Game::Render()
{
	// Don't try to render anything before the first Update.
	if (m_timer.GetFrameCount() == 0)
	{
		return;
	}

	Clear();

	m_deviceResources->PIXBeginEvent(L"Render");
	auto context = m_deviceResources->GetD3DDeviceContext();

	// TODO: Add your rendering code here.
	context;
	
	{
		// ゲームステートの描画
		m_gameStateManager->Render();
		// マスク
		m_effectMask->Draw();
	}

	m_deviceResources->PIXEndEvent();

	// Show the new frame.
	m_deviceResources->Present();
}

// Helper method to clear the back buffers.
void Game::Clear()
{
	m_deviceResources->PIXBeginEvent(L"Clear");

	// Clear the views.
	auto context = m_deviceResources->GetD3DDeviceContext();
	auto renderTarget = m_deviceResources->GetRenderTargetView();
	auto depthStencil = m_deviceResources->GetDepthStencilView();

	context->ClearRenderTargetView(renderTarget, Colors::CornflowerBlue);
	context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	context->OMSetRenderTargets(1, &renderTarget, depthStencil);

	// Set the viewport.
	auto viewport = m_deviceResources->GetScreenViewport();
	context->RSSetViewports(1, &viewport);

	m_deviceResources->PIXEndEvent();
}
#pragma endregion

#pragma region Message Handlers
// Message handlers
void Game::OnActivated()
{
	// TODO: Game is becoming active window.
}

void Game::OnDeactivated()
{
	// TODO: Game is becoming background window.
}

void Game::OnSuspending()
{
	// TODO: Game is being power-suspended (or minimized).
}

void Game::OnResuming()
{
	m_timer.ResetElapsedTime();

	// TODO: Game is being power-resumed (or returning from minimize).
}

void Game::OnWindowMoved()
{
	auto r = m_deviceResources->GetOutputSize();
	m_deviceResources->WindowSizeChanged(r.right, r.bottom);
}

void Game::OnWindowSizeChanged(int width, int height)
{
	m_deviceResources->CreateWindowSizeDependentResources();
	// if (!m_deviceResources->WindowSizeChanged(width, height))
	// 	return;
	//
	// CreateWindowSizeDependentResources();

	// TODO: Game window is being resized.
}

// Properties
void Game::GetDefaultSize(int& width, int& height) const
{
	// TODO: Change to desired default window size (note minimum size is 320x200).
	width = Game::SCREEN_W;
	height = Game::SCREEN_H;
}

void Game::ChangeFullscreen(bool flag)
{
#ifndef WINDOWED_FULLSCREEN
	m_deviceResources->GetSwapChain()->SetFullscreenState(flag, NULL);
#elif 
	if (!flag)
	{
		SetWindowLongPtr(m_window, GWL_STYLE, WS_OVERLAPPEDWINDOW);
		SetWindowLongPtr(m_window, GWL_EXSTYLE, 0);

		int width, height;
		GetDefaultSize(width, height);

		ShowWindow(m_window, SW_SHOWNORMAL);

		SetWindowPos(m_window, HWND_TOP, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
	}
	else
	{
		SetWindowLongPtr(m_window, GWL_STYLE, 0);
		SetWindowLongPtr(m_window, GWL_EXSTYLE, WS_EX_TOPMOST);

		SetWindowPos(m_window, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

		ShowWindow(m_window, SW_SHOWMAXIMIZED);
	}
#endif
}
#pragma endregion

#pragma region Direct3D Resources
// These are the resources that depend on the device.
void Game::CreateDeviceDependentResources()
{
	auto device = m_deviceResources->GetD3DDevice();
	auto context = m_deviceResources->GetD3DDeviceContext();

	// TODO: Initialize device dependent objects here (independent of window size).
	device;
	context;

	// コモンステート作成
	m_state = std::make_unique<CommonStates>(m_deviceResources->GetD3DDevice());
	GameContext::Register<DirectX::CommonStates>(m_state);
	// スプライトバッチの作成
	m_sprites = std::make_unique<SpriteBatch>(context);
	GameContext::Register<SpriteBatch>(m_sprites);
	// スプライトフォントの作成
	m_font = std::make_unique<SpriteFont>(device, L"SegoeUI_18.spritefont");
	GameContext::Register<SpriteFont>(m_font);
}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateWindowSizeDependentResources()
{
	// TODO: Initialize windows-size dependent objects here.

	// TODO: 行列の設定
	m_view = SimpleMath::Matrix::Identity;
	m_projection = SimpleMath::Matrix::Identity;
}

void Game::OnDeviceLost()
{
	// TODO: Add Direct3D resource cleanup here.
}

void Game::OnDeviceRestored()
{
	CreateDeviceDependentResources();

	CreateWindowSizeDependentResources();
}
#pragma endregion
