#pragma once

//======================================================
// File Name : Life.h
// Summary : �c�@�N���X
// Date : 2019/11/18
// Author : Takafumi Ban
//======================================================
#pragma once
#include "GameObject.h"

class Life : public GameObject
{
public:
	// �G�̃T�C�Y
	static const int SIZE = 24;
public:
	// �R���X�g���N�^
	Life();
	// �������֐�
	void Initialize(ID3D11ShaderResourceView* texture, int x, int y);
	// �X�V�֐�
	virtual void Update(float elapsedTime);
	// �`��
	virtual void Render();
	// �c�@���̎擾
	int GetLife() { return m_life; }
	// �c�@���̐ݒ�
	void SetLife(int life) { m_life = life; }
private:
	// �e�N�X�`���n���h��
	ID3D11ShaderResourceView* m_texture;
	// �\���ʒu
	int m_x, m_y;
	// �c�@��
	int m_life;
};