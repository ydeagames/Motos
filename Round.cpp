#include "pch.h"
#include "Round.h"

//======================================================
// File Name : Round.cpp
// Summary : ���E���h���N���X
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
// �X�V�֐�
void Round::Update(float elapsedTime)
{
}
// �`��
void Round::Render()
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