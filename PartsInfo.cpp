#include "pch.h"
#include "PartsInfo.h"

//======================================================
// File Name : PartsInfo.cpp
// Summary : �A�C�e�����N���X
// Date : 2019/11/18
// Author : Takafumi Ban
//======================================================

#include "pch.h"
#include "PartsInfo.h"
#include "GameContext.h"

// �p���[�A�b�v�p�[�c�ő�g�p�\��
const int PartsInfo::PARTS_MAX_USE[] = { 7, 1 };
PartsInfo::PartsInfo()
	: m_texture(nullptr), m_x(0), m_y(0), m_max(0), m_get(0), m_use(0), m_blinkFlag(false)
{
}

void PartsInfo::Initialize(ID3D11ShaderResourceView* texture, int x, int y, PARTS_KIND kind)
{
	m_texture = texture;
	m_x = x;
	m_y = y;
	m_kind = kind;
	// �����Ɏg�p�ł���p�[�c����ݒ�
	m_max = PARTS_MAX_USE[kind];
	// �_�Ŏ��Ԃ̃��Z�b�g
	m_blinkTimer.resize(m_max + 1);
	ResetBlink();
	// �擾���̕������������
	m_cntStr.Initialize(texture, 0, 0, "");
	// �_�ŊǗ�����
	m_pBlink = std::make_unique<Blink>();
	// �_�ŊԊu�̐ݒ�
	m_pBlink->Initialize(0.16f);
	// �_�ŊJ�n
	m_pBlink->Start();
}

// �X�V�֐�
void PartsInfo::Update(float elapsedTime)
{
	// �_�ŊǗ��X�V
	m_pBlink->Update(elapsedTime);
}

// �`��
void PartsInfo::Render()
{
	if (!m_texture) return;
	// �g�p���̃p�[�c�`��i�p�l���̉��n�j
	RECT srcRect = { 0 * 24, 3 * 24, 2 * 24, 5 * 24 };
	if (m_use == 0)
	{
		if (m_blinkFlag == false || m_pBlink->GetState())
		{
			GameContext::Get<DirectX::SpriteBatch>() -> Draw(m_texture, DirectX::SimpleMath::Vector2((float)m_x, (float)m_y), &srcRect);
		}
	}
	else
	{
		for (int i = 0; i < m_use; i++)
		{
			if (m_blinkFlag == false || i != (m_use - 1) || m_pBlink->GetState())
			{
				GameContext::Get<DirectX::SpriteBatch>() -> Draw(m_texture, DirectX::SimpleMath::Vector2((float)m_x + (i % 5 + 1) * 48, (float)m_y + (i / 5) * 48), &srcRect);
			}
		}
	}
	// ENEMY1 �A�C�R���̕`��
	srcRect.left += 48;
	srcRect.right += 48;
	GameContext::Get<DirectX::SpriteBatch>() -> Draw(m_texture, DirectX::SimpleMath::Vector2((float)m_x, (float)m_y), &srcRect);
	// �擾�p�[�c�A�C�R���̕`��
	int cnt = 0;
	if (m_get > 0)
	{
		srcRect.left += 48 * (m_kind + 1);
		srcRect.right += 48 * (m_kind + 1);
		for (int i = 0; i < m_get && i < m_max; i++)
		{
			if (m_blinkTimer[i] != 0 && m_pBlink->GetState() || m_blinkTimer[i] == 0)
			{
				GameContext::Get<DirectX::SpriteBatch>() -> Draw(m_texture, DirectX::SimpleMath::Vector2((float)m_x + (cnt % 5 + 1) * 48, (float)m_y + (cnt / 5) * 48), &srcRect);
			}
			if (m_blinkTimer[i] > 0) m_blinkTimer[i]--;
			cnt++;
		}
	}
	// �擾���������ꍇ�͕�����ŕ\������
	if ((m_get > m_max) && (m_blinkTimer[cnt] != 0 && m_pBlink->GetState() || m_blinkTimer[cnt] == 0))
	{
		int get = m_get;
		if (get > 99) get = 99;
		char str[] = " ";
		str[1] = '0' + get % 10;
		if (get / 10 != 0)
			str[0] = '0' + char(get / 10);
		m_cntStr.SetStr(str);
		m_cntStr.SetPos(m_x + (cnt % 5 + 1) * 48, m_y + (cnt / 5) * 48 + 24);
		m_cntStr.Draw();
	}
	if (m_blinkTimer[cnt] > 0) m_blinkTimer[cnt]--;
}

void PartsInfo::Reset()
{
	m_get = m_use = 0;
	m_pBlink->Reset(true);
}

void PartsInfo::GetParts()
{
	int pos = m_get;
	if (pos >= (int)m_blinkTimer.size())
	{
		pos = (int)m_blinkTimer.size() - 1;
	}
	m_blinkTimer[pos] = 60 * 3; // 3 �b
	m_get++;
}

bool PartsInfo::SetUseCnt(int cnt)
{
	// �擾�����p�[�c���ȏ�g�p�ł��Ȃ��ꍇ�� false ��Ԃ�
	if (cnt > m_get) return false;
	m_use = cnt;
	return true;
}

int PartsInfo::GetUseCnt()
{
	return m_use;
}

void PartsInfo::SelectPartsMode(bool flag)
{
	m_blinkFlag = flag;
	// �_�ŊԊu�̐ݒ�
	if (m_blinkFlag == true)
	{
		m_pBlink->Initialize(0.08f);
	}
	else
	{
		m_pBlink->Initialize(0.16f);
	}
}

void PartsInfo::SetGetCnt(int cnt)
{
	m_get = cnt;
}

void PartsInfo::ResetBlink()
{
	for (size_t i = 0; i < m_blinkTimer.size(); i++)
	{
		m_blinkTimer[i] = 0;
	}
}