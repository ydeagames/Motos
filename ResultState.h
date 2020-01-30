#pragma once


#include "GameState.h"


class ResultState : public GameState
{
private:
	float m_time = 0;

	// エフェクト
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;

	// スプライトバッチ
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

	// プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_plane;

	// インプットレイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;

	// テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_backTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_spaceTexture;

public:
	ResultState();

public:
	virtual ~ResultState();


public:
	void Initialize() override;
	void Update(float elapsedTime) override;
	void Render() override;
	void Finalize() override;
};
