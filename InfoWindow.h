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

