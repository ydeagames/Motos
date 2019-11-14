//======================================================
// File Name	: Score.cpp
// Summary		: �X�R�A�N���X
// Date			: 2019/11/13
// Author		: Takafumi Ban
//======================================================
#include "pch.h"
#include "Score.h"
#include "GameContext.h"

Score::Score()
{
}

Score::~Score()
{
}

void Score::Initialize(ID3D11ShaderResourceView* texture, int x, int y, const char * title, DirectX::FXMVECTOR titleColor, DirectX::FXMVECTOR scoreColor, float blinkTime)
{
	m_titleMoji.Initialize(texture, x, y, title, titleColor);
	m_scoreMoji.Initialize(texture, x + Moji::SIZE, y + Moji::SIZE, "      00", scoreColor);

	// �_�ŊǗ�����
	m_pBlink = std::make_unique<Blink>();
	// �_�ŊԊu�̐ݒ�
	m_pBlink->Initialize(blinkTime);
}

// �X�V�֐�
void Score::Update(float elapsedTime)
{
	// �_�ŊǗ��X�V
	m_pBlink->Update(elapsedTime);
}

// �`��
void Score::Render(const DirectX::SimpleMath::Matrix& viewMatrix, const DirectX::SimpleMath::Matrix& projectionMatrix)
{
	if (m_pBlink->GetState())
	{
		m_titleMoji.Draw();
	}
	m_scoreMoji.Draw();
}

void Score::SetScore(int score)
{
	char str[] = "      00";
	int length = std::strlen(str);
	if (score > 99999999) score = 99999999;
	std::snprintf(str, sizeof(str)/sizeof(char), "%*d", length, score);
	//int pos = 7;
	//while (score && pos >= 0)
	//{
	//	str[pos] = '0' + score % 10;
	//	score /= 10;
	//	pos--;
	//}
	m_scoreMoji.SetStr(str);
}

void Score::BlinkTitle(bool flag)
{
	if (flag)
	{
		m_pBlink->Start();
	}
	else
	{
		m_pBlink->Stop();
		m_pBlink->Reset(true);
	}
}
