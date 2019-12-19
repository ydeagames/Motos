﻿//======================================================
// File Name	: GameWindow.h
// Summary		: ゲームウィンドウ
// Date			: 2019/11/26
// Author		: Takafumi Ban
//======================================================
#pragma once

#include "GameObject.h"
#include "GeometricPrimitive.h"

class Camera;
class Floor;
class Stage;
class Bg;

class GameWindow : public GameObject
{
public:
	// 画面サイズ
	static const int SCREEN_W = 960;
	static const int SCREEN_H = 720;

	enum DIR
	{
		UP,
		LEFT,
		DOWN,
		RIGHT
	};

	// 描画順
	enum DRAW_PRIORITY
	{
		DRAW_TOP, // 一番手前（２Ⅾ表示やエフェクトで使用）
		DRAW_OBJECT, // 床の上のオブジェクト
		DRAW_SHADOW, // オブジェクトの影
		DRAW_STAGE, // ステージ
		DRAW_FALL, // 落下オブジェクト
		DRAW_BG, // 背景
	};

	static const float DIR_ANGLE[];

	static const float GRAVITY;

	static const float FALL_SPEED;
	static const float FALL_ROTATE_SPEED;
	static const float FALL_DISTANCE;

public:
	// コンストラクタ
	GameWindow();
	~GameWindow();

	// 初期化関数
	void Initialize();

	// 更新
	virtual void Update(float elapsedTime);
	// 描画関数
	virtual void Render();

	// カメラ
	Camera* GetCamera();

	// ステージ
	Stage* GetStage();

	// 追加
	template<typename T>
	T* AddObject()
	{
		std::unique_ptr<T> pObject = std::make_unique<T>();
		auto raw = pObject.get();
		GameContext::Get<ObjectManager>()->GetGameOM()->Add(std::move(pObject));
		return raw;
	}

private:
	// カメラテスト用モデル
	//std::unique_ptr <DirectX::GeometricPrimitive> m_pShape;
	// モデルプール
	//std::vector<std::unique_ptr <DirectX::Model>> m_pModels;

	// カメラ
	std::unique_ptr<Camera> m_camera;

	// Bg
	Bg* m_pBg;
	// ステージ
	std::unique_ptr<Stage> m_pStage;
	// 床
	Floor* m_pFloor;

	// キーボードトラッカー
	DirectX::Keyboard::KeyboardStateTracker m_tracker;
};
