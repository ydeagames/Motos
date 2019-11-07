//======================================================
// File Name : DebugFont.h
// Summary : �f�o�b�O�t�H���g
// Date : 2019/4/18
// Author : Takafumi Ban
//======================================================
#pragma once
#include "SimpleMath.h"
#include "SpriteBatch.h"
#include "SpriteFont.h"
// �V���O���g���̃^�C�v
// 0:���g�̐��|�C���^�ŊǗ��i���F�����I�ȉ�������Ȃ��ƃ��������[�N����j
// 1:���g�̃��j�[�N�|�C���^�ŊǗ��i���F���j�[�N�|�C���^�̗��𐄏��j
// 2:�ÓI�Ȏ��ԂŊǗ��i���F�v���O�����I���̃^�C�~���O�ł�������ł��Ȃ��j
#define DEBUG_FONT_SINGLETON_TYPE 1
class DebugFont
{
public:
	// ������ő吔�T�C�Y
	static const size_t STRING_SIZE_MAX = 256;
private:
	DebugFont();
public:
	~DebugFont();
public:
	// ����
	void create(ID3D11Device1* device, ID3D11DeviceContext1* context);
	// �`��
	void draw();
	// ���Z�b�g
	void reset();
	// �\�����o�^
	void print(float posX, float posY, wchar_t const* const format, ...);
	void print(DirectX::SimpleMath::Vector2 pos, wchar_t const* const format, ...);
#if DEBUG_FONT_SINGLETON_TYPE == 0
public:
	// �I�u�W�F�N�g�ւ̃|�C���^��Ԃ�
	static DebugFont* GetInstance()
	{
		if (m_pInstance == nullptr)
		{
			// �I�u�W�F�N�g�𐶐����Am_instance�ϐ��Ɋi�[����
			m_pInstance = new DebugFont();
		}
		// �I�u�W�F�N�g�ւ̃|�C���^��Ԃ�
		return m_pInstance;
	}
	// �I�u�W�F�N�g�ւ̎Q�Ƃ�Ԃ�
	static DebugFont& getRefInstance()
	{
		if (m_pInstance == nullptr)
		{
			// �I�u�W�F�N�g�𐶐����Am_instance�ϐ��Ɋi�[����
			m_pInstance = new DebugFont();
		}
		// �I�u�W�F�N�g�ւ̃|�C���^��Ԃ�
		return *m_pInstance;
	}
	// �I�u�W�F�N�g��j������
	static void Dispose()
	{
		if (m_pInstance != nullptr)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}
private:
	static DebugFont* m_pInstance;
#elif DEBUG_FONT_SINGLETON_TYPE == 1
public:
	// �I�u�W�F�N�g�ւ̃|�C���^��Ԃ�
	static DebugFont* GetInstance()
	{
		if (m_pInstance.get() == nullptr)
		{
			// �I�u�W�F�N�g�𐶐����A�|�C���^��m_pInstance�ϐ��Ɋi�[����
			m_pInstance.reset(new DebugFont());
		}
		// �I�u�W�F�N�g�ւ̃|�C���^��Ԃ�
		return m_pInstance.get();
	}
	// �I�u�W�F�N�g��j������
	static void Dispose()
	{
		if (m_pInstance)
			m_pInstance.reset();
	}
private:
	// �I�u�W�F�N�g�ւ̃|�C���^
	static std::unique_ptr<DebugFont> m_pInstance;
#else
public:
	// �I�u�W�F�N�g�ւ̃|�C���^��Ԃ�
	static DebugFont* GetInstance()
	{
		// �I�u�W�F�N�g�ւ̃|�C���^��Ԃ�
		return &m_instance;
	}
	// �I�u�W�F�N�g�̎Q�Ƃ�Ԃ�
	static DebugFont& getRefInstance()
	{
		// �I�u�W�F�N�g�ւ̃|�C���^��Ԃ�
		return m_instance;
	}
private:
	// �I�u�W�F�N�g
	static DebugFont m_instance;
#endif
private:
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	std::unique_ptr<DirectX::SpriteFont> m_spriteFont;
	wchar_t* m_string;
	DirectX::SimpleMath::Vector2 m_pos;
};
