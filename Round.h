#pragma once

//======================================================
// File Name : Round.h
// Summary : ���E���h���N���X
// Date : 2019/11/18
// Author : Takafumi Ban
//======================================================
#pragma once
#include "GameObject.h"
#include "Moji.h"
class Round : public GameObject
{
public:
	// �R���X�g���N�^
	Round();
	// �������֐�
	void Initialize(ID3D11ShaderResourceView* texture, int x, int y);
	// �X�V�֐�
	virtual void Update(float elapsedTime);
	// �`��
	virtual void Render();
	// ���E���h���̐ݒ�֐�
	void SetRound(int round);
private:
	// ������
	Moji m_moji;
};