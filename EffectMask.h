//======================================================
// File Name	: EffectMask.h
// Summary		: 画面切り替え用のマスク表示クラス
// Date			: 2020/01/21
// Author		: Takafumi Ban
//======================================================
#pragma once

class Game;

class EffectMask
{
private:

	// ピクセルシェーダーに渡す定数バッファの構造体
	struct cbChangesEveryFrame
	{
		FLOAT radius;
		FLOAT aspectRatio;
		DirectX::XMFLOAT2 dummy;
	};

	// 定数バッファ
	cbChangesEveryFrame m_cbChangesEveryFrame;

	Microsoft::WRL::ComPtr<ID3D11Buffer> m_cbBuffer;

	// ピクセルシェーダー
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;

	// 2Dテクスチャ
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_texture2D;

	// レンダーターゲットビュー
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTargetView;

	// シェーダーリソースビュー
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_shaderResourceView;

	// オープン又はクローズするまでの時間
	float m_interval;

	// 割合(0～1):0の場合閉じている
	float m_rate;

	// オープンフラグ（trueの場合オープン）
	bool m_open;

	// マスクの色
	DirectX::SimpleMath::Color m_color;

public:
	// コンストラクタ
	EffectMask();

	// 初期化関数
	void Initialize(float interval);

	// 更新処理
	void Update(float elapsedTime);

	// 描画
	void Draw();

	// オープンする関数
	void Open();

	// クローズする関数
	void Close();

	// オープン中かチェックする関数
	bool IsOpen();

	// クローズ中かチェックする関数
	bool IsClose();

	// マスクの色の設定関数
	void SetColor(DirectX::FXMVECTOR color) { m_color = color; }

	// オープン具合を返す関数(0～1)
	float GetOpenRate() { return m_rate; }
};
