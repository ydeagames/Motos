//======================================================
// File Name	: Score.h
// Summary		: �X�R�A�N���X
// Date			: 2019/11/13
// Author		: Takafumi Ban
//======================================================
#pragma once

#include "GameObject.h"
#include "Moji.h"
#include "Blink.h"

class Game;

class Score : public GameObject
{
public:
	// �R���X�g���N�^
	Score();

	// �f�X�g���N�^
	~Score();

	// �������֐�
	void Initialize(ID3D11ShaderResourceView* texture, int x, int y, const char* title, DirectX::FXMVECTOR titleColor = DirectX::Colors::White, DirectX::FXMVECTOR scoreColor = DirectX::Colors::White, float blinkTime = 0.0f);

	// �X�V�֐�
	virtual void Update(float elapsedTime);
	// �`��
	virtual void Render();

	// �X�R�A�̐ݒ�֐�
	void SetScore(int score);

	// �^�C�g�������̓_�ł�ON/OFF�֐�
	void BlinkTitle(bool flag);

private:
	// �^�C�g��������
	Moji m_titleMoji;

	// �_��������
	Moji m_scoreMoji;

	// �_�ŊǗ��I�u�W�F�N�g
	std::unique_ptr<Blink> m_pBlink;
};
