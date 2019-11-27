#include "pch.h"
#include "InfoWindow.h"
#include "GameContext.h"
#include "DeviceResources.h"
#include "GameObjectManager.h"
#include "ObjectManager.h"
#include "Score.h"
#include "PartsInfo.h"
#include "Round.h"
#include "Life.h"

InfoWindow::InfoWindow()
{
}

InfoWindow::~InfoWindow()
{
	for (auto& object : GameContext::Get<ObjectManager>()->GetInfoOM()->GetObjects())
		object->Invalidate();
}

void InfoWindow::Initialize()
{
	// �e�N�X�`���̓ǂݍ���
	DirectX::CreateWICTextureFromFile(GameContext::Get<DX::DeviceResources>()->GetD3DDevice(), TEXTURE_ATLAS, nullptr, m_texture01.GetAddressOf());
	DirectX::CreateWICTextureFromFile(GameContext::Get<DX::DeviceResources>()->GetD3DDevice(), TEXTURE_BACKGROUND, nullptr, m_bg01Texture.GetAddressOf());
	//DirectX::CreateWICTextureFromFile(GameContext::Get<DX::DeviceResources>()->GetD3DDevice(), L"Resources/Textures/right.png", nullptr, m_bg01Texture.GetAddressOf());

	// �n�C�X�R�A�^�X�N�o�^
	(m_pHighScore = AddObject<Score>())
		->Initialize(m_texture01.Get(), TILE_W * 1, TILE_H * 2, "HIGH SCORE", DirectX::SimpleMath::Color(191 / 255.0f, 194 / 255.0f, 245 / 255.0f), DirectX::Colors::Yellow, 0.2f);
	// �X�R�A�^�X�N�o�^
	(m_pScore = AddObject<Score>())
		->Initialize(m_texture01.Get(), TILE_W * 1, TILE_H * 4, "1P", DirectX::Colors::Red, DirectX::Colors::White, 0.5f);
	// �p���[�A�b�v�p�[�c�擾�^�X�N�o�^
	(m_pPowerup = AddObject<PartsInfo>())
		->Initialize(m_texture01.Get(), TILE_W * 1, TILE_H * 21, PartsInfo::POWER_PARTS);
	// �W�����v�p�[�c�^�X�N�o�^
	(m_pJump = AddObject<PartsInfo>())
		->Initialize(m_texture01.Get(), TILE_W * 1, TILE_H * 25, PartsInfo::JUMP_PARTS);
	// ���E���h�p�[�c�擾�^�X�N�o�^
	(m_pRound = AddObject<Round>())
		->Initialize(m_texture01.Get(), TILE_W * 6, TILE_H * 28);
	// ���C�t�^�X�N�o�^
	(m_pLife = AddObject<Life>())
		->Initialize(m_texture01.Get(), TILE_W * 1, TILE_H * 28);

	// �ݒ�
	m_pRound->SetRound(1);
	m_pLife->SetLife(3);
	m_pScore->BlinkTitle(true);
}

void InfoWindow::Update(float elapsedTime)
{
	static int score = 0;
	m_pScore->SetScore(score++);
}

void InfoWindow::Render(const DirectX::SimpleMath::Matrix& viewMatrix, const DirectX::SimpleMath::Matrix& projectionMatrix)
{
	// ���E�B���h�E�̔w�i�̕\��
	GameContext::Get<DirectX::SpriteBatch>()->Draw(m_bg01Texture.Get(), DirectX::SimpleMath::Vector2(0, 0));
}
