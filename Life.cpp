#include "pch.h"
#include "Life.h"

//======================================================
// File Name : Life.cpp
// Summary : �c�@�N���X
// Date : 2019/11/18
// Author : Takafumi Ban
//======================================================
#include "pch.h"
#include "Life.h"
#include "GameContext.h"
#include <SpriteBatch.h>

Life::Life()
	: m_texture(nullptr), m_x(0), m_y(0), m_life(0)
{
}
void Life::Initialize(ID3D11ShaderResourceView* texture, int x, int y)
{
	m_texture = texture;
	m_x = x;
	m_y = y;
}
// �X�V�֐�
void Life::Update(float elapsedTime)
{
}
// �`��
void Life::Render(const DirectX::SimpleMath::Matrix& viewMatrix, const DirectX::SimpleMath::Matrix& projectionMatrix)
{
	if (!m_texture) return;
	RECT srcRect = { 0 * SIZE, 5 * SIZE, 1 * SIZE, 6 * SIZE };
	for (int i = 0; i < m_life; i++)
	{
		GameContext::Get<DirectX::SpriteBatch>() -> Draw(m_texture, DirectX::SimpleMath::Vector2((float)m_x + SIZE * i, (float)m_y), &srcRect);
	}
}