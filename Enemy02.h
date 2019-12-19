//======================================================
// File Name	: Enemy02.h
// Summary		: プレイヤークラス
// Date			: 2019/12/19
// Author		: YdeaGames
//======================================================
#pragma once

#include "Object.h"
#include "Keyboard.h"
#include "Enemy01.h"

class Enemy02 : public Enemy01
{
public:
	// 初期化関数
	void Initialize(int x, int y);

	// 更新処理（通常）
	void State_Normal(float elapsedTime) override;
};
