//======================================================
// File Name	: Blink.h
// Summary		: 点滅クラス
// Date			: 2019/11/13
// Author		: Takafumi Ban
//======================================================
#pragma once

#include <SimpleMath.h>

class Blink
{
public:
	// コンストラクタ
	Blink();

	// 初期化関数
	void Initialize(float interval);

	// 更新関数
	virtual void Update(float elapsedTime);

	// 点滅状態の取得関数
	bool GetState();

	// リセット関数
	// flagには初期状態を渡してください
	void Reset(bool flag);

	// 点滅をスタートする関数
	void Start();

	// 点滅を停止する関数
	void Stop();

private:
	// 点滅フラグ
	bool m_blinkFlag;

	// 点滅時間
	float m_blinkTime;

	// 間隔（単位：秒）
	float m_interval;

	// 停止フラグ
	bool m_stopFlag;
};
