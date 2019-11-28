#pragma once

#include "GameObject.h"

class Score;
class PartsInfo;
class Round;
class Life;

class InfoWindow : public GameObject
{
public:
	// ��ʃT�C�Y
	static const int SCREEN_W = 1280 - 960;
	static const int SCREEN_H = 720;
	static const int TILE_W = 24;
	static const int TILE_H = 24;
	constexpr static const wchar_t* TEXTURE_ATLAS = L"Resources/Textures/texture01.png";
	constexpr static const wchar_t* TEXTURE_BACKGROUND = L"Resources/Textures/bg01.png";

public:
	InfoWindow();
	~InfoWindow();

	void Initialize();
	virtual void Update(float elapsedTime);
	virtual void Render(const DirectX::SimpleMath::Matrix& viewMatrix, const DirectX::SimpleMath::Matrix& projectionMatrix);

	// �n�C�X�R�A�ɃA�N�Z�X����֐�
	Score* GetHighScore() { return m_pHighScore; }
	// �X�R�A�ɃA�N�Z�X����֐�
	Score* GetScore() { return m_pScore; }
	// �p���[�A�b�v�p�[�c�ɃA�N�Z�X����֐�
	PartsInfo* GetPowerParts() { return m_pPowerup; }
	// �W�����v�p�[�c�ɃA�N�Z�X����֐�
	PartsInfo* GetJumpParts() { return m_pJump; }

	// �ǉ�
	template<typename T>
	T* AddObject()
	{
		std::unique_ptr<T> pObject = std::make_unique<T>();
		auto raw = pObject.get();
		GameContext::Get<ObjectManager>()->GetInfoOM()->Add(std::move(pObject));
		return raw;
	}

private:
	// �e�N�X�`���n���h���i�w�i�j
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_bg01Texture;
	// �e�N�X�`���n���h���i�w�i�j
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture01;

	// �n�C�X�R�A
	Score* m_pHighScore;
	// �X�R�A
	Score* m_pScore;
	// �p�[�c���i�p���[�A�b�v�j
	PartsInfo* m_pPowerup;
	// �p�[�c���i�W�����v�j
	PartsInfo* m_pJump;
	// ���E���h
	Round* m_pRound;
	// ���C�t
	Life* m_pLife;
};

