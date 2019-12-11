//======================================================
// File Name	: Floor.cpp
// Summary		: 床クラス
// Date			: 2019/11/28
// Author		: Takafumi Ban
//======================================================
#include "pch.h"
#include "Floor.h"
#include "GameWindow.h"
#include "Stage.h"
#include <CommonStates.h>
#include "GameContext.h"
#include "DeviceResources.h"
#include "Camera.h"

// ダメージの移行時間
const float Floor::DAMEGE_DELAY_TIME = 0.2f;

Floor::Floor()
	: m_stage(nullptr), m_models{nullptr}, m_state(NONE)
{
	SetDrawPrio(GameWindow::DRAW_STAGE);
}

void Floor::Initialize(Stage * stage, int x, int y)
{
	m_stage = stage;
	m_pos = DirectX::SimpleMath::Vector3((float)x, 0.0f, (float)y);
}

void Floor::Update(float elapsedTime)
{
	// ダメージフラグを落とす
	m_damageFlag = false;

	// ダメージ移行
	if (m_damageTimer > 0.0f)
	{
		m_damageTimer -= elapsedTime;
		if (m_damageTimer <= 0.0f)
		{
			m_damageTimer = 0.0f;
			switch (m_state)
			{
			case NORMAL:	// 通常
				m_state = Floor::DAMAGED;	// 破損した床へ
				break;
			case DAMAGED:	// 破損した床
				m_state = Floor::FALL;		// 落下へ
				break;
			default:
				break;
			}
		}
	}

	// 落下状態の場合は床を下に落とす
	if (m_state == Floor::FALL)
	{
		m_pos.y -= 0.1f;

		// ある程度落下したら消す
		if (m_pos.y < -3.0f)
		{
			m_state = Floor::DEAD;
		}
	}
}

void Floor::Render()
{
	GameWindow* gameWindow = GameContext::Get<GameWindow>();

	if (!m_stage || !m_models[m_state]) return;

	// ワールド行列の作成
	DirectX::SimpleMath::Matrix world = DirectX::SimpleMath::Matrix::CreateTranslation(m_pos);

	// ダメージが入った瞬間からモデル切り替えする
	State state = m_state;


	// モデルの描画
	m_models[state]->Draw(GameContext::Get<DX::DeviceResources>()->GetD3DDeviceContext()
		, *GameContext::Get<DirectX::CommonStates>()
		, world
		, gameWindow->GetCamera()->getViewMatrix()
		, gameWindow->GetCamera()->getProjectionMatrix(), false, [&]()
	{
		// 床は半透明の加算合成（半透明で暗くなってしまうので）
		GameContext::Get<DX::DeviceResources>()->GetD3DDeviceContext()->OMSetBlendState(GameContext::Get<DirectX::CommonStates>()->Additive(), nullptr, 0xffffffff);
	});

}

void Floor::SetModel(State state, DirectX::Model * model)
{
	m_models[state] = model;
}

void Floor::Damage()
{
	// １フレームにダメージは１度だけ
	if (m_damageFlag) return;
	m_damageFlag = true;
	// ダメージ移行タイマーセット
	m_damageTimer = DAMEGE_DELAY_TIME;
}

void Floor::Reset()
{
	m_pos.y = 0.0f;
	if (m_state != Floor::NONE)
	{
		m_state = Floor::NORMAL;
		m_damageFlag = false;
		m_damageTimer = 0.0f;
	}
}

