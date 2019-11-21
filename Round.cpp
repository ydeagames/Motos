#include "pch.h"
#include "Round.h"

//======================================================
// File Name : Round.cpp
// Summary : ラウンド数クラス
// Date : 2019/11/18
// Author : Takafumi Ban
//======================================================
#include "pch.h"
#include "Round.h"
#include "GameContext.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

Round::Round()
{
}
void Round::Initialize(ID3D11ShaderResourceView* texture, int x, int y)
{
	m_moji.Initialize(texture, x, y, "ROUND 0");
}
// 更新関数
void Round::Update(float elapsedTime)
{
}
// 描画
void Round::Render(const DirectX::SimpleMath::Matrix& viewMatrix, const DirectX::SimpleMath::Matrix& projectionMatrix)
{
	m_moji.Draw();
}
void Round::SetRound(int round)
{
	char str[] = "ROUND ";
	if (round > 99) round = 99;
	str[6] = '0' + round % 10;
	if (round / 10 != 0) str[5] = '0' + round / 10;
	m_moji.SetStr(str);
}