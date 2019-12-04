#pragma once

//======================================================
// File Name : Round.h
// Summary : ラウンド数クラス
// Date : 2019/11/18
// Author : Takafumi Ban
//======================================================
#pragma once
#include "GameObject.h"
#include "Moji.h"
class Round : public GameObject
{
public:
	// コンストラクタ
	Round();
	// 初期化関数
	void Initialize(ID3D11ShaderResourceView* texture, int x, int y);
	// 更新関数
	virtual void Update(float elapsedTime);
	// 描画
	virtual void Render();
	// ラウンド数の設定関数
	void SetRound(int round);
private:
	// 文字列
	Moji m_moji;
};