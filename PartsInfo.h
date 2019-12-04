#pragma once

//======================================================
// File Name : PartsInfo.h
// Summary : �A�C�e�����N���X
// Date : 2019/11/18
// Author : Takafumi Ban
//======================================================
#pragma once
#include "GameObject.h"
#include "Moji.h"
#include "Blink.h"
#include <vector>
class Game;
class PartsInfo : public GameObject
{
public:
	// �p�[�c�̎��
	enum PARTS_KIND
	{
		POWER_PARTS,
		JUMP_PARTS,
		PARTS_KIND_NUM,
	};
	// �e�p�[�c�̍ő�g�p�\��
	static const int PARTS_MAX_USE[PARTS_KIND_NUM];
public:
	// �R���X�g���N�^
	PartsInfo();
	// �������֐�
	void Initialize(ID3D11ShaderResourceView* texture, int x, int y, PARTS_KIND kind);
	// �X�V�֐�
	virtual void Update(float elapsedTime);
	// �`��
	virtual void Render();
	// �p�[�c�擾�������Z�b�g����֐�
	void Reset();
	// �p�[�c�擾�����擾����֐�
	void GetParts();
	// �g�p����p�[�c�̐���ݒ肷��֐�
	bool SetUseCnt(int cnt);
	// �g�p�����p�[�c�����擾����֐�
	int GetUseCnt();
	// �g�p����p�[�c��ݒ肷�鎞�̓_�Őݒ�
	void SelectPartsMode(bool flag);
	// �擾����ݒ肷��֐�
	void SetGetCnt(int cnt);
	// �_�ł̃��Z�b�g�֐�
	void ResetBlink();
private:
	// �e�N�X�`���n���h��
	ID3D11ShaderResourceView* m_texture;
	// �\���ʒu
	int m_x, m_y;
	// �p�[�c�̎��
	PARTS_KIND m_kind;
	// �����Ɏg�p�ł���p�[�c��
	int m_max;
	// �ێ��p�[�c��
	int m_get;
	// ���ݎg�p���̃p�[�c���i0 �ȉ��̏ꍇ�͕\���Ȃ��j
	int m_use;
	// �擾���𕶎���ŕ\�����邽�߂̃I�u�W�F�N�g
	Moji m_cntStr;
	// �_�ŊǗ��I�u�W�F�N�g
	std::unique_ptr<Blink> m_pBlink;
	// �_�Ń^�C�}�[
	std::vector<int> m_blinkTimer;
	// �g�p����p�[�c�̐���ݒ蒆�̓_�Ńt���O
	bool m_blinkFlag;
};