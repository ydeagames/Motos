//======================================================
// File Name	: Enemy03.h
// Summary		: プレイヤークラス
// Date			: 2019/01/09
// Author		: YdeaGames
//======================================================
#pragma once

#include "Object.h"
#include "Keyboard.h"
#include "Enemy01.h"

class Enemy03 : public Enemy01
{
private:
	bool m_avoid;
	
public:
	Enemy03(const std::string& tag, DirectX::Model* shadowModel);
	
	// 初期化関数
	void Initialize(int x, int y);

	int GetKeyToDir(int key);
	// 更新処理（通常）
	void State_Normal(float elapsedTime) override;
};
