//======================================================
// File Name	: EffectMask.h
// Summary		: ��ʐ؂�ւ��p�̃}�X�N�\���N���X
// Date			: 2020/01/21
// Author		: Takafumi Ban
//======================================================
#pragma once

class Game;

class EffectMask
{
private:

	// �s�N�Z���V�F�[�_�[�ɓn���萔�o�b�t�@�̍\����
	struct cbChangesEveryFrame
	{
		FLOAT radius;
		FLOAT aspectRatio;
		DirectX::XMFLOAT2 dummy;
	};

	// �萔�o�b�t�@
	cbChangesEveryFrame m_cbChangesEveryFrame;

	Microsoft::WRL::ComPtr<ID3D11Buffer> m_cbBuffer;

	// �s�N�Z���V�F�[�_�[
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;

	// 2D�e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_texture2D;

	// �����_�[�^�[�Q�b�g�r���[
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTargetView;

	// �V�F�[�_�[���\�[�X�r���[
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_shaderResourceView;

	// �I�[�v�����̓N���[�Y����܂ł̎���
	float m_interval;

	// ����(0�`1):0�̏ꍇ���Ă���
	float m_rate;

	// �I�[�v���t���O�itrue�̏ꍇ�I�[�v���j
	bool m_open;

	// �}�X�N�̐F
	DirectX::SimpleMath::Color m_color;

public:
	// �R���X�g���N�^
	EffectMask();

	// �������֐�
	void Initialize(float interval);

	// �X�V����
	void Update(float elapsedTime);

	// �`��
	void Draw();

	// �I�[�v������֐�
	void Open();

	// �N���[�Y����֐�
	void Close();

	// �I�[�v�������`�F�b�N����֐�
	bool IsOpen();

	// �N���[�Y�����`�F�b�N����֐�
	bool IsClose();

	// �}�X�N�̐F�̐ݒ�֐�
	void SetColor(DirectX::FXMVECTOR color) { m_color = color; }

	// �I�[�v�����Ԃ��֐�(0�`1)
	float GetOpenRate() { return m_rate; }
};
