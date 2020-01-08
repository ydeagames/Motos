//======================================================
// File Name	: GameWindow.cpp
// Summary		: ゲームウィンドウ
// Date			: 2019/11/26
// Author		: Takafumi Ban
//======================================================
#include "pch.h"
#include "GameWindow.h"
#include "GameContext.h"
#include "DeviceResources.h"
#include "Camera.h"
#include "Floor.h"
#include "GameObjectManager.h"
#include "ObjectManager.h"
#include "Floor.h"
#include "Stage.h"
#include "Bg.h"
#include "InfoWindow.h"
#include "PartsInfo.h"
#include "Life.h"
#include "Round.h"
#include "Score.h"


const float GameWindow::DIR_ANGLE[] =
{
	DirectX::XMConvertToRadians(45.0f * 0),
	DirectX::XMConvertToRadians(45.0f * 1),
	DirectX::XMConvertToRadians(45.0f * 2),
	DirectX::XMConvertToRadians(45.0f * 3),
	DirectX::XMConvertToRadians(45.0f * 4),
	DirectX::XMConvertToRadians(45.0f * 5),
	DirectX::XMConvertToRadians(45.0f * 6),
	DirectX::XMConvertToRadians(45.0f * 7),
};

// 重力（地球上は9.8m/s^2）
const float GameWindow::GRAVITY = 9.8f;

// 落下速度（１秒間似移動するドット数）
const float GameWindow::FALL_SPEED = 8.f;

// 落下したオブジェクトが画面から消える距離
const float GameWindow::FALL_ROTATE_SPEED = 10.f;

// 落下中のオブジェクトの回転速度（１秒間に１８０度回転）
const float GameWindow::FALL_DISTANCE = 180.f;

// ラウンド数
const int GameWindow::ROUND_MAX = 3;
// ハイスコアの初期値（1000 点）
const int GameWindow::HIGH_SCORE = 1000;


GameWindow::GameWindow()
	: m_gameState(STATE_PARTS_SELECT)
	, m_round(0)
	, m_life(0)
	, m_getPowerupParts(0)
	, m_powerupParts(0)
	, m_usePowerupParts(0)
	, m_getJumpParts(0)
	, m_jumpParts(0)
	, m_useJumpParts(0)
	, m_selectParts(POWERUP)
	, m_selectPartsDisplayFlag(false)
	, m_highScore(HIGH_SCORE)
	, m_score(0)
	, m_highScoreUpdate(false)
{
	SetDrawPrio(GameWindow::DRAW_TOP);
}

GameWindow::~GameWindow()
{
	if (m_pStage)
	{
		m_pStage->ResetStageData();
		m_pStage = nullptr;
	}
	if (m_pBg)
	{
		GameObject::Destroy(m_pBg);
		m_pBg = nullptr;
	}
}

void GameWindow::Initialize()
{
	m_camera = std::make_unique<Camera>();
	m_camera->Initialize();

	// Box生成
	//m_pShape = DirectX::GeometricPrimitive::CreateBox(GameContext::Get<DX::DeviceResources>()->GetD3DDeviceContext(), DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f));

	// Bg登録
	(m_pBg = AddObject<Bg>())
		->Initialize();

	// ステージ
	m_pStage = std::make_unique<Stage>();
	m_pStage->Initialize();
	m_pStage->LoadStageData(L"StageData/Stage01.csv");
	m_pStage->SetStageData();

	//{
	//	DirectX::EffectFactory fx(GameContext::Get<DX::DeviceResources>()->GetD3DDevice());
	//	fx.SetDirectory(L"Resources\\Models");
	//	m_pModels.emplace_back(DirectX::Model::CreateFromCMO(GameContext::Get<DX::DeviceResources>()->GetD3DDevice(), L"Resources\\Models\\floorPanel_00.cmo", fx));
	//	m_pFloor->SetModel(Floor::NONE, (m_pModels.end() - 1)->get());
	//	m_pModels.emplace_back(DirectX::Model::CreateFromCMO(GameContext::Get<DX::DeviceResources>()->GetD3DDevice(), L"Resources\\Models\\floorPanel_01.cmo", fx));
	//	m_pFloor->SetModel(Floor::NORMAL, (m_pModels.end() - 1)->get());
	//	m_pFloor->SetModel(Floor::DAMAGED, (m_pModels.end() - 1)->get());
	//	m_pFloor->SetModel(Floor::DEAD, (m_pModels.end() - 1)->get());
	//	m_pFloor->SetModel(Floor::FALL, (m_pModels.end() - 1)->get());
	//}
}

void GameWindow::Update(float elapsedTime)
{
	auto kb = DirectX::Keyboard::Get().GetState();
	m_tracker.Update(kb);
	m_pStage->GetPlayer()->Move(elapsedTime, m_tracker);
}

void GameWindow::Render()
{
	// カメラ取得
	//DirectX::SimpleMath::Matrix world;
	//world = DirectX::SimpleMath::Matrix::CreateTranslation(4.5f, 0.0f, 5.5f);
	//m_pShape->Draw(world, m_camera->getViewMatrix(), m_camera->getProjectionMatrix());
}

Camera* GameWindow::GetCamera()
{
	return m_camera.get();
}

Stage* GameWindow::GetStage()
{
	return m_pStage.get();
}

void GameWindow::GetPowerupParts()
{
	InfoWindow* infoWindow = GameContext::Get<InfoWindow>();
	m_getPowerupParts++;
	infoWindow->GetPowerParts()->GetParts();
}
void GameWindow::GetJumpParts()
{
	InfoWindow* infoWindow = GameContext::Get<InfoWindow>();
	m_getJumpParts++;
	infoWindow->GetJumpParts()->GetParts();
}
void GameWindow::ResetPowerupParts()
{
	InfoWindow* infoWindow = GameContext::Get<InfoWindow>();
	m_powerupParts = m_getPowerupParts = 0;
	infoWindow->GetPowerParts()->Reset();
}
void GameWindow::ResetJumpParts()
{
	InfoWindow* infoWindow = GameContext::Get<InfoWindow>();
	m_jumpParts = m_getJumpParts = 0;
	infoWindow->GetJumpParts()->Reset();
}
GameWindow::GAME_STATE GameWindow::InitializeGame(float elapsedTime)
{
	InfoWindow* infoWindow = GameContext::Get<InfoWindow>();
	// 得点を初期化
	SetScore(0);
	// ハイスコア更新フラグを落とす
	m_highScoreUpdate = false;
	// ハイスコア更新通知を３秒間に設定
	m_highScoreBlinkTime = 3.0f;
	// ラウンド数を設定
	m_round = 1;
	infoWindow->GetRound()->SetRound(m_round);
	// 残機数を設定
	m_life = 3;
	infoWindow->GetLife()->SetLife(m_life);
	// パワーアップパーツのリセット
	ResetPowerupParts();
	// ジャンプパーツのリセット
	ResetJumpParts();
	// ステージデータの読み込み
	m_pStage->LoadStageData(GetStageFilename(m_round));
	// ステージデータの設定
	m_pStage->SetStageData();
	// ハイスコアを設定
	infoWindow->GetHighScore()->SetScore(m_highScore);
	// 1P を点滅
	infoWindow->GetScore()->BlinkTitle(true);
	// パーツ選択画面へ
	return STATE_PARTS_SELECT;
}
GameWindow::GAME_STATE GameWindow::SelectParts(float elapsedTime)
{
	InfoWindow* infoWindow = GameContext::Get<InfoWindow>();
	// パーツを一つも所持していなければパーツ選択画面はスルー
	if (m_powerupParts == 0 && m_jumpParts == 0) return STATE_START;
	switch (m_selectParts)
	{
	case POWERUP: // パワーアップパーツ選択中
		infoWindow->GetPowerParts()->SelectPartsMode(true);
		// 右キーが押された
		if (m_tracker.IsKeyPressed(DirectX::Keyboard::Right))
		{
			if (m_usePowerupParts < PartsInfo::PARTS_MAX_USE[PartsInfo::POWER_PARTS]
				&& m_usePowerupParts < m_powerupParts)
			{
				m_usePowerupParts++;
			}
		}
		// 左キーが押された
		if (m_tracker.IsKeyPressed(DirectX::Keyboard::Left))
		{
			if (m_usePowerupParts > 0)
			{
				m_usePowerupParts--;
			}
		}
		infoWindow->GetPowerParts()->SetUseCnt(m_usePowerupParts);
		// スペースキーで次へ
		if (m_powerupParts == 0 || m_tracker.pressed.Space)
		{
			// プレイヤーにパワーアップパーツを装着する
			m_pStage->GetPlayer()->SetPowerupParts(m_usePowerupParts);
			m_selectParts = JUMP;
			infoWindow->GetPowerParts()->SelectPartsMode(false);
		}
		break;
	case JUMP: // ジャンプパーツ選択中
		infoWindow->GetJumpParts()->SelectPartsMode(true);
		// 右キーが押された
		if (m_tracker.IsKeyPressed(DirectX::Keyboard::Right))
		{
			if (m_useJumpParts < PartsInfo::PARTS_MAX_USE[PartsInfo::JUMP_PARTS]
				&& m_useJumpParts < m_jumpParts)
			{
				m_useJumpParts++;
			}
		}
		// 左キーが押された
		if (m_tracker.IsKeyPressed(DirectX::Keyboard::Left))
		{
			if (m_useJumpParts > 0)
			{
				m_useJumpParts--;
			}
		}
		infoWindow->GetJumpParts()->SetUseCnt(m_useJumpParts);
		// スペースキーで次へ
		if (m_jumpParts == 0 || m_tracker.pressed.Space)
		{
			infoWindow->GetJumpParts()->SelectPartsMode(false);
			m_selectParts = POWERUP;
			if (m_useJumpParts)
			{
				// ジャンプパーツを装着
				m_pStage->GetPlayer()->SetJumpParts(true);
			}
			else
			{
				// ジャンプパーツを外す
				m_pStage->GetPlayer()->SetJumpParts(false);
			}
			return STATE_START;
		}
		break;
	}
	return STATE_PARTS_SELECT;
}
GameWindow::GAME_STATE GameWindow::StartGame(float elapsedTime)
{
	// 敵を全てアクティブにする
	auto enemyList = m_pStage->GetEnemyList();
	for (Object* p : enemyList)
	{
		p->SetActive(true);
	}
	// ゲーム中へ
	return STATE_GAME;
}
GameWindow::GAME_STATE GameWindow::PlayGame(float elapsedTime)
{
	InfoWindow* infoWindow = GameContext::Get<InfoWindow>();
	GAME_STATE state = STATE_GAME;
	// プレイヤーの移動処理
	m_pStage->GetPlayer()->Move(elapsedTime, m_tracker);
	// プレイヤーが死んだら
	if (m_pStage->GetPlayer()->GetState() == Player::STATE_DEAD)
	{
		// 残機数を減らす
		m_life--;
		// 情報ウインドウの残機数も減らす
		infoWindow->GetLife()->SetLife(m_life);
		if (m_life == 0)
		{
			// ゲームオーバー
			state = STATE_GAMEOVER;
		}
		else
		{
			// もう一回チャレンジ
			state = STATE_AGAIN;
		}
	}
	else
	{
		// 敵が全滅でクリア
		if (CheckEnemyAllDead() == true)
		{
			// 次のステージへ
			state = STATE_NEXT;
		}
	}
	// ハイスコアのタイトル点滅を止める
	if (m_highScoreUpdate && state != STATE_GAME)
	{
		m_highScoreBlinkTime = 0.0f;
	}
	// ハイスコアのタイトル点滅を止める処理
	if (m_highScoreUpdate && m_highScoreBlinkTime > 0.0f)
	{
		m_highScoreBlinkTime -= elapsedTime;
	}
	else
	{
		infoWindow->GetHighScore()->BlinkTitle(false);
	}
	return state;
}
GameWindow::GAME_STATE GameWindow::NextGame(float elapsedTime)
{
	InfoWindow* infoWindow = GameContext::Get<InfoWindow>();
	// ラウンド数を加算
	m_round++;
	if (m_round > ROUND_MAX) m_round = 1;
	infoWindow->GetRound()->SetRound(m_round);
	// ステージをリセットする
	m_pStage->ResetStageData();
	// ステージデータの読み込み
	m_pStage->LoadStageData(GetStageFilename(m_round));
	// ステージデータの設定
	m_pStage->SetStageData();
	// パーツ選択へ
	return STATE_PARTS_SELECT;
}
void GameWindow::SetScore(int score)
{
	InfoWindow* infoWindow = GameContext::Get<InfoWindow>();
	m_score = score;
	infoWindow->GetScore()->SetScore(m_score);
	// ハイスコア更新をチェック
	if (m_score > m_highScore)
	{
		m_highScore = m_score;
		infoWindow->GetHighScore()->SetScore(m_highScore);
		// ハイスコア更新通知の点滅は一度だけ
		if (m_highScoreUpdate == false)
		{
			// ハイスコア更新フラグを立てる
			m_highScoreUpdate = true;
			// ハイスコアのタイトルを点滅させる
			infoWindow->GetHighScore()->BlinkTitle(true);
		}
	}
}
int GameWindow::GetScore()
{
	return m_score;
}
wchar_t* GameWindow::GetStageFilename(int round)
{
	static wchar_t fname[32];
	wsprintfW(fname, L"StageData\\Stage%02d.csv", round);
	return fname;
}
bool GameWindow::CheckEnemyAllDead()
{
	auto enemyList = m_pStage->GetEnemyList();
	for (Object* p : enemyList)
	{
		// 表示 ON のオブジェクトがいれば全部死んでいない
		if (p->GetDisplayFlag() == true) return false;
	}
	return true;
}
void GameWindow::UpdateParts(bool stageClearFlag)
{
	InfoWindow* infoWindow = GameContext::Get<InfoWindow>();
	// ステージクリアしたので使用したパーツを引き、取得したパーツを足す
	if (stageClearFlag == true)
	{
		m_powerupParts -= m_usePowerupParts;
		m_powerupParts += m_getPowerupParts;
		m_jumpParts -= m_useJumpParts;
		m_jumpParts += m_getJumpParts;
	}
	// プレイヤーに装着したパーツを解除
	m_pStage->GetPlayer()->SetPowerupParts(0);
	m_pStage->GetPlayer()->SetJumpParts(false);
	// 今回使用したパーツ、取得したパーツ数も初期化
	m_getPowerupParts = m_usePowerupParts = 0;
	m_getJumpParts = m_useJumpParts = 0;
	// 情報ウインドウのパーツ装着情報も初期化
	infoWindow->GetPowerParts()->SetUseCnt(0);
	infoWindow->GetJumpParts()->SetUseCnt(0);
	infoWindow->GetPowerParts()->SetGetCnt(m_powerupParts);
	infoWindow->GetJumpParts()->SetGetCnt(m_jumpParts);
	// パーツ点滅をリセットする
	infoWindow->GetPowerParts()->ResetBlink();
	infoWindow->GetJumpParts()->ResetBlink();
}