//======================================================
// File Name	: Shadow.h
// Summary		: �e�N���X�iObject�ɕt����e�j
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
	// �e�̃|�C���^
	Object* m_pParent;

	// ���f���n���h��
	DirectX::Model* m_model;

	// �\���t���O
	bool m_active;

	// �u�����h�X�e�[�g
	Microsoft::WRL::ComPtr<ID3D11BlendState> m_blendState;

public:
	// �R���X�g���N�^
	Shadow();

	// �������֐�
	void Initialize(Object* pParent, DirectX::Model* model);

	// �X�V�֐�
	void Update(float elapsedTime) override;

	// �`��֐�
	void Render() override;

	// �\����ON/OFF����֐�
	void Active(bool flag) { m_active = flag; }
};
