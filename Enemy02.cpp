//======================================================
// File Name	: Enemy02.cpp
// Summary		: エネミー
// Date			: 2019/12/19
// Author		: YdeaGames
//======================================================
#include "pch.h"
#include "Enemy02.h"
#include "GameContext.h"
#include "DeviceResources.h"
#include <CommonStates.h>
#include "GameWindow.h"
#include "CollisionManager.h"
#include "Camera.h"
#include "Stage.h"

Enemy02::Enemy02(const std::string& tag)
	: Enemy01(tag)
{
}

void Enemy02::Initialize(int x, int y)
{
	Enemy01::Initialize(x, y);
}

void Enemy02::State_Normal(float elapsedTime)
{

}
