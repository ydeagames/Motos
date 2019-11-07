#include "pch.h"

#include "TitleState.h"

#include "DebugFont.h"
#include "GameContext.h"
#include "GameStateManager.h"



TitleState::TitleState()
	: GameState()
	, m_count(0)
{
}



TitleState::~TitleState()
{
}



void TitleState::Initialize()
{
	m_count = 0;
}



void TitleState::Update()
{
	m_count++;

	if (m_count > 120)
	{
		GameStateManager* gameStateManager = GameContext::Get<GameStateManager>();
		gameStateManager->RequestState("Play");
		m_count = 0;
	}
}



void TitleState::Render()
{
	DebugFont* debugFont = DebugFont::GetInstance();
	debugFont->print(10, 10, L"TitleState");
	debugFont->draw();
	debugFont->print(10, 40, L"%3d / 120", m_count);
	debugFont->draw();
}



void TitleState::Finalize()
{

}
