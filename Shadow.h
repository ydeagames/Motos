//======================================================
// File Name	: Shadow.h
// Summary		: 影クラス（Objectに付ける影）
// Date			: 2020/01/15
// Author		: Takafumi Ban
//======================================================
#pragma once

#include "GameObject.h"
#include "Model.h"

class GameWindow;
class Object;

class Shadow : public GameObject
{
private:
	// 親のポインタ
	Object* m_pParent;

	// モデルハンドル
	DirectX::Model* m_model;

	// 表示フラグ
	bool m_active;

	// ブレンドステート
	Microsoft::WRL::ComPtr<ID3D11BlendState> m_blendState;

public:
	// コンストラクタ
	Shadow();

	// 初期化関数
	void Initialize(Object* pParent, DirectX::Model* model);

	// 更新関数
	void Update(float elapsedTime) override;

	// 描画関数
	void Render() override;

	// 表示のON/OFFする関数
	void Active(bool flag) { m_active = flag; }
};
