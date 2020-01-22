#pragma once

#include <memory>

#include "GameState.h"
#include "ObjectManager.h"


class DebugCamera;
class GridFloor;
class InfoWindow;
class GameWindow;


class PlayState: public GameState
{
	public:
		PlayState();

	public:
		virtual ~PlayState();


	public:
		void Initialize() override;
		void Update(float elapsedTime) override;
		void Render() override;
		void Finalize() override;

	private:
		// ゲームウィンドウ
		std::unique_ptr<GameWindow> m_pGameWindow;
		// 情報ウィンドウ
		std::unique_ptr<InfoWindow> m_pInfoWindow;

		// ゲーム画面のビューポート
		D3D11_VIEWPORT							m_viewportGame;
		// 情報画面のビューポート
		D3D11_VIEWPORT							m_viewportInfo;

		// ゲームオブジェクトマネージャー
		std::unique_ptr<ObjectManager>			m_objectManager;
};
