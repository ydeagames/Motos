//======================================================
// File Name	: Moji.h
// Summary		: �����N���X
// Date			: 2019/11/13
// Author		: Takafumi Ban
//======================================================
#pragma once

#include <SimpleMath.h>
#include <SpriteBatch.h>

class Game;

// ������\���N���X
// ���A���t�@�x�b�g�啶���Ɛ����̂ݑΉ�
class Moji
{
public:
	// �����̃T�C�Y
	static const int SIZE = 24;

public:
	// �R���X�g���N�^
	Moji();

	// �������֐�
	void Initialize(ID3D11ShaderResourceView* texture, int x, int y, const char* str, DirectX::FXMVECTOR color = DirectX::Colors::White);

	// �`��֐�
	void Draw();

	// ������ݒ�֐�
	void SetStr(const char* str);

	// �\���ʒu�̐ݒ�֐�
	void SetPos(int x, int y);

private:
	// �e�N�X�`���n���h��
	ID3D11ShaderResourceView* m_texture;

	// ������
	std::string m_str;

	// �\���ʒu
	int m_x, m_y;

	// �����̐F
	DirectX::SimpleMath::Color m_color;
};
