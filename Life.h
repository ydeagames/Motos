#pragma once

//======================================================
// File Name : Life.h
// Summary : 残機クラス
// Date : 2019/11/18
// Author : Takafumi Ban
//======================================================
#pragma once
#include "GameObject.h"

class Life : public GameObject
{
public:
	// 絵のサイズ
	static const int SIZE = 24;
public:
	// コンストラクタ
	Life();
	// 初期化関数
	void Initialize(ID3D11ShaderResourceView* texture, int x, int y);
	// 更新関数
	virtual void Update(float elapsedTime);
	// 描画
	virtual void Render(const DirectX::SimpleMath::Matrix& viewMatrix, const DirectX::SimpleMath::Matrix& projectionMatrix);
	// 残機数の取得
	int GetLife() { return m_life; }
	// 残機数の設定
	void SetLife(int life) { m_life = life; }
private:
	// テクスチャハンドル
	ID3D11ShaderResourceView* m_texture;
	// 表示位置
	int m_x, m_y;
	// 残機数
	int m_life;
};