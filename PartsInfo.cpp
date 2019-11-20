#include "pch.h"
#include "PartsInfo.h"

//======================================================
// File Name : PartsInfo.cpp
// Summary : アイテム情報クラス
// Date : 2019/11/18
// Author : Takafumi Ban
//======================================================

#include "pch.h"
#include "PartsInfo.h"
#include "GameContext.h"

// パワーアップパーツ最大使用可能数
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
	// 同時に使用できるパーツ数を設定
	m_max = PARTS_MAX_USE[kind];
	// 点滅時間のリセット
	m_blinkTimer.resize(m_max + 1);
	ResetBlink();
	// 取得数の文字列を初期化
	m_cntStr.Initialize(texture, 0, 0, "");
	// 点滅管理生成
	m_pBlink = std::make_unique<Blink>();
	// 点滅間隔の設定
	m_pBlink->Initialize(0.16f);
	// 点滅開始
	m_pBlink->Start();
}

// 更新関数
void PartsInfo::Update(float elapsedTime)
{
	// 点滅管理更新
	m_pBlink->Update(elapsedTime);
}

// 描画
void PartsInfo::Render(const DirectX::SimpleMath::Matrix& viewMatrix, const DirectX::SimpleMath::Matrix&
	projectionMatrix)
{
	if (!m_texture) return;
	// 使用中のパーツ描画（パネルの下地）
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
	// NORMAL アイコンの描画
	srcRect.left += 48;
	srcRect.right += 48;
	GameContext::Get<DirectX::SpriteBatch>() -> Draw(m_texture, DirectX::SimpleMath::Vector2((float)m_x, (float)m_y), &srcRect);
	// 取得パーツアイコンの描画
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
	// 取得数が多い場合は文字列で表示する
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
	m_blinkTimer[pos] = 60 * 3; // 3 秒
	m_get++;
}

bool PartsInfo::SetUseCnt(int cnt)
{
	// 取得したパーツ数以上使用できない場合は false を返す
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
	// 点滅間隔の設定
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