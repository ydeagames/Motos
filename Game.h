//
// Game.h
//

#pragma once

#include "DeviceResources.h"
#include "StepTimer.h"

class ObjectManager;
class GameStateManager;
class CollisionManager;
class GameAI;
class EffectMask;


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

	// �E�B���h�E
	HWND                                    m_window;

    // Device resources.
    std::unique_ptr<DX::DeviceResources>    m_deviceResources;

    // Rendering loop timer.
    DX::StepTimer                           m_timer;

	// �r���[�s��
	DirectX::SimpleMath::Matrix				m_view;

	// �ˉe�s��
	DirectX::SimpleMath::Matrix				m_projection;

	// �}�E�X
	std::unique_ptr<DirectX::Mouse>			m_mouse;

	std::unique_ptr<DirectX::Keyboard>      m_keyboard;

	// �R�����X�e�[�g
	std::unique_ptr<DirectX::CommonStates>	m_state;
	// �X�v���C�g�o�b�`
	std::unique_ptr<DirectX::SpriteBatch>	m_sprites;
	// �X�v���C�g�t�H���g
	std::unique_ptr<DirectX::SpriteFont>	m_font;
	
	// �}�X�N
	std::unique_ptr<EffectMask>				m_effectMask;

	std::unique_ptr<CollisionManager>       m_collisionManager;

	std::unique_ptr<GameStateManager>       m_gameStateManager;

	std::unique_ptr<GameAI>					m_gameAI;
};
