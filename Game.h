//
// Game.h
//

#pragma once

#include "DeviceResources.h"
#include "StepTimer.h"

class GameObjectManager;


// A basic game implementation that creates a D3D11 device and
// provides a game loop.
class Game final : public DX::IDeviceNotify
{
public:
	static const int SCREEN_W = 1280;
	static const int SCREEN_H = 720;

public:

    Game() noexcept(false);
	~Game();

    // Initialization and management
    void Initialize(HWND window, int width, int height);

    // Basic game loop
    void Tick();

    // IDeviceNotify
    virtual void OnDeviceLost() override;
    virtual void OnDeviceRestored() override;

    // Messages
    void OnActivated();
    void OnDeactivated();
    void OnSuspending();
    void OnResuming();
    void OnWindowMoved();
    void OnWindowSizeChanged(int width, int height);

    // Properties
    void GetDefaultSize( int& width, int& height ) const;

	void ChangeFullscreen(bool flag);

private:

    void Update(DX::StepTimer const& timer);
    void Render();

    void Clear();

    void CreateDeviceDependentResources();
    void CreateWindowSizeDependentResources();

	// ウィンドウ
	HWND                                    m_window;

    // Device resources.
    std::unique_ptr<DX::DeviceResources>    m_deviceResources;

    // Rendering loop timer.
    DX::StepTimer                           m_timer;

	// ビュー行列
	DirectX::SimpleMath::Matrix				m_view;

	// 射影行列
	DirectX::SimpleMath::Matrix				m_projection;

	// マウス
	std::unique_ptr<DirectX::Mouse>			m_pMouse;

	std::unique_ptr<DirectX::Keyboard>      m_pKeyboard;

	// コモンステート
	std::unique_ptr<DirectX::CommonStates>	m_pState;

	std::unique_ptr<GameObjectManager>      m_pGameObjectManager;
};
