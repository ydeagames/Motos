//======================================================
// File Name	: Player.cpp
// Summary		: プレイヤークラス
// Date			: 2019/12/05
// Author		: Takafumi Ban
//======================================================
#include "pch.h"
#include "Player.h"
#include "GameContext.h"
#include "DeviceResources.h"
#include <CommonStates.h>
#include "GameWindow.h"
#include "CollisionManager.h"
#include "Camera.h"
#include "SphereCollider.h"
#include "GameAI.h"
#include "Stage.h"
#include "ObjectManager.h"
#include "JumpEffect.h"
#include "GameObjectManager.h"
#include "HitEffect.h"
#include "Resources/Sounds/WorkUnit_0/CueSheet_0.h"
#include "ADX2Le.h"

// プレイヤーの重さ
const float Player::WEIGHT = 1.0f;

// 床に対する摩擦係数
const float Player::FRICTION = 0.1f;

// 床との判定用の幅と高さ
const float Player::WIDTH = 0.2f;
const float Player::HEIGHT = 0.2f;

// プレイヤーの最大移動速度
const float Player::MAX_SPEED = 0.1f;

// ジャンプしているフレーム数
const int Player::JUMP_FRAME = 30;

// ジャンプの高さ
const float Player::JUMP_HEIGHT = 1.5f;

// オブジェクト同士の判定用の半径
const float Player::RADIUS = 0.4f;

float SLerp(float start, float end, float t)
{
	float rate = (sinf(3.f * DirectX::XM_PI / 2.f + DirectX::XM_PI * t) + 1.f) / 2.f;
	return start + (end - start) * rate;
}

Player::Player(const std::string& tag, DirectX::Model* shadowModel)
	: Object(tag, shadowModel)
	, m_models{ nullptr }
	, m_powerupParts(0)
	, m_jumpParts(false)
	, m_state(STATE_NORMAL)
	, m_jumpCounter(0)
	, m_fallRotateAngle(0.0f)
{
}

void Player::Initialize(int x, int y)
{
	m_x = x;
	m_y = y;
	m_position = DirectX::SimpleMath::Vector3((float)x, 0.0f, (float)y);
	m_lastPos = DirectX::SimpleMath::Vector2(m_position.x, m_position.z);

	// 質量を設定
	SetPowerupParts(0);

	// 摩擦係数
	m_coefficientOfFriction = FRICTION;

	// 半径
	m_radius = RADIUS;

	// 幅と高さ
	m_width = Player::WIDTH;
	m_height = Player::HEIGHT;

	// 当たり判定 WIP
	m_collider = std::make_unique<SphereCollider>(this, m_radius);
}

void Player::SetModel(ModelType modelType, DirectX::Model* model)
{
	m_models[modelType] = model;
}

void Player::Update(float elapsedTime)
{
	// 削除リクエストがあればタスクを削除
	if (IsInvalid()) return;

	// 影
	ShadowActive(m_state != STATE_FALL && m_state != STATE_DEAD && m_activeFlag);

	// アクティブフラグがfalseの場合は何もしない
	if (!m_activeFlag) return;

	// 摩擦により速度を落とす
	Friction(elapsedTime);

	// 最大速度以上にならないよう調整
	if (m_vel.LengthSquared() > MAX_SPEED* MAX_SPEED)
	{
		m_vel.Normalize();
		m_vel *= MAX_SPEED;
	}

	// 状態によって処理を分岐させる
	switch (m_state)
	{
	case STATE_NORMAL:	// 通常
		State_Normal(elapsedTime);
		break;
	case STATE_JUMP:	// ジャンプ中
		State_Jump(elapsedTime);
		break;
	case STATE_HIT:		// ヒット中
		State_Hit(elapsedTime);
		break;
	case STATE_FALL:	// 落下中
		State_Fall(elapsedTime);
		break;
	default:
		break;
	}

	// 位置に速度を足す
	m_position += m_vel;

	// プレイヤーと床のチェック
	if (m_state == Player::STATE_NORMAL && CheckFloor() == false)
	{
		// 落ちたので停止させる
		m_vel = DirectX::SimpleMath::Vector3::Zero;
		// 状態を落下中へ
		m_state = STATE_FALL;
		// 描画順をFALLへ
		SetDrawPrio(GameWindow::DRAW_FALL);

		// 落ちた記憶は抹消
		GameContext::Get<GameAI>()->ForgetRecently();
	}

	// ジャンプの処理
	if (m_jumpCounter != 0)
	{
		// ゲームっぽいジャンプにするため、サイン波でジャンプ時の高さを算出する
		m_jumpCounter--;
		int cnt = JUMP_FRAME - m_jumpCounter;
		float angle = 180.0f / (float)JUMP_FRAME * cnt;
		m_position.y = sinf(DirectX::XMConvertToRadians(angle)) * JUMP_HEIGHT;
		if (m_jumpCounter == 0)
		{
			// 床に着地した
			m_position.y = 0.0f;
			// 床にダメージを与える
			if (m_jumpEndFunction) m_jumpEndFunction(this);
		}
	}
}

void Player::Render()
{
	GameWindow* gameWindow = GameContext::Get<GameWindow>();
	DX::DeviceResources* deviceResources = GameContext::Get<DX::DeviceResources>();
	Camera* camera = gameWindow->GetCamera();

	if (!gameWindow || !m_models[NORMAL] || !m_displayFlag) return;

	// 向いている角度を角度テーブルから取得
	float angle = GameWindow::DIR_ANGLE[m_dir];

	// 落下中の回転を加える
	angle += m_fallRotateAngle;

	// ワールド行列を作成
	DirectX::SimpleMath::Matrix world = DirectX::SimpleMath::Matrix::CreateRotationY(angle) * DirectX::SimpleMath::Matrix::CreateTranslation(m_position);

	// モデルの描画（ジャンプパーツを付けているかどうかでモデルが違う）
	m_models[m_jumpParts ? WING : NORMAL]->Draw(deviceResources->GetD3DDeviceContext(),
		*GameContext::Get<DirectX::CommonStates>(),
		world, camera->getViewMatrix(), camera->getProjectionMatrix());

	if (m_state != STATE_DEAD && m_state != STATE_FALL)
		// 衝突判定マネージャーに登録
		GameContext::Get<CollisionManager>()->Add(GetTag(), m_collider.get());
}

void Player::State_Normal(float elapsedTime)
{
	m_position = m_position;
}

void Player::State_Jump(float elapsedTime)
{
	if (m_jumpCounter == 0)
	{
		// 通常の状態へ
		m_state = STATE_NORMAL;
	}
}

void Player::State_Hit(float elapsedTime)
{
	// 摩擦により停止したら
	if (m_vel == DirectX::SimpleMath::Vector3::Zero)
	{
		if (m_jumpCounter != 0)
		{
			m_state = STATE_JUMP;
		}
		else
		{
			m_state = STATE_NORMAL;
		}
	}
}

void Player::State_Fall(float elapsedTime)
{
	m_position.y -= GameWindow::FALL_SPEED * elapsedTime;
	// 回転しながら落下する
	m_fallRotateAngle -= DirectX::XMConvertToRadians(GameWindow::FALL_ROTATE_SPEED) * elapsedTime;
	// ある程度落下したら
	if (m_position.y < -GameWindow::FALL_DISTANCE)
	{
		// 死亡
		m_state = STATE_DEAD;
		// 表示（OFF）
		SetDisplayFlag(false);
		// 落下中の回転をリセット
		m_fallRotateAngle = 0.0f;
	}
}

void Player::OnCollision(GameObject* object)
{
	//if (Object * obj = dynamic_cast<Object*>(object))
	//{
	//	auto diff = GetPosition() - obj->GetPosition();
	//	auto angle = std::atan2(diff.z, diff.x);
	//	AddForce(angle, obj->GetHitForce());
	//}
	GameWindow* gameWindow = GameContext::Get<GameWindow>();
	// 衝突した相手によって処理を変える
	if (object->GetTag() == "Enemy01")
	{
		OnCollision_Enemy01(object);
	}
	else if (object->GetTag() == "Enemy02")
	{
		OnCollision_Enemy01(object);
	}
	else if (object->GetTag() == "PowerupParts")
	{
		gameWindow->GetPowerupParts();
	}
	else if (object->GetTag() == "JumpParts")
	{
		gameWindow->GetJumpParts();
	}
}

void Player::OnCollision_Enemy01(GameObject* object)
{
	DirectX::SimpleMath::Vector3 playerDir = DirectX::SimpleMath::Vector3(0.0f, 0.0f, -1.0f);
	DirectX::SimpleMath::Vector3 v;
	DirectX::SimpleMath::Matrix rotY;
	float playerAngle;
	Object* obj = static_cast<Object*>(object);
	// 停止させる
	m_vel = DirectX::SimpleMath::Vector3::Zero;
	// プレイヤーから敵方向へのベクトルを求める
	v = object->GetPosition() - this->GetPosition();
	// プレイヤーの向きベクトルを作成
	playerAngle = GameWindow::DIR_ANGLE[m_dir];
	rotY = DirectX::SimpleMath::Matrix::CreateRotationY(playerAngle);
	playerDir = DirectX::SimpleMath::Vector3::Transform(playerDir, rotY);
	// ジャンプ時の衝突などで敵の前に落下することがあるので吹っ飛ばす方向が前か後ろかを判断する
	if (v.Dot(playerDir) > 0)
	{
		// 進行方向の逆方向に力を加える
		playerAngle += DirectX::XMConvertToRadians(180.0f);
	}
	// 通常時とジャンプ時で跳ね返りの力を変える
	if (m_state == STATE_NORMAL)
	{
		AddForce(playerAngle, obj->GetHitForce()); // 通常
	}
	else
	{
		AddForce(playerAngle, obj->GetHitForce() / 2.0f); // ジャンプ中
	}

	{
		// ヒット時のエフェクトを出す（表示位置は衝突した相手との中間地点）
		v = object->GetPosition() - this->GetPosition();
		v.x /= 2.0f;
		v.z /= 2.0f;
		DirectX::SimpleMath::Vector3 pos = m_position + v;
		std::unique_ptr<HitEffect> hitEffect = std::make_unique<HitEffect>();
		hitEffect->Initialize(m_position);
		GameContext::Get<ObjectManager>()->GetGameOM()->Add(std::move(hitEffect));
	}

	GameContext::Get<Adx2Le>()->Play(CRI_CUESHEET_0_CUE_1);

	// 衝突状態へ
	m_state = STATE_HIT;
}

Player::STATE Player::GetState()
{
	return m_state;
}

void Player::Move(float elapsedTime, const DirectX::Keyboard::KeyboardStateTracker& tracker)
{
	// プレイヤーが通常、ジャンプ時のみ移動する
	if (m_state != STATE_NORMAL && m_state != STATE_JUMP) return;

	// キー入力を取得
	auto kb = DirectX::Keyboard::Get().GetState();

	// 加える力
	float force = 0.0f;

	int key = 0;

	// 方向キーが押されたら
	if (kb.Up || kb.Down || kb.Left || kb.Right)
	{
		// 押された方向キーのビットを立てる
		if (kb.Up) key |= 1 << GameWindow::UP;
		if (kb.Down) key |= 1 << GameWindow::DOWN;
		if (kb.Left) key |= 1 << GameWindow::LEFT;
		if (kb.Right) key |= 1 << GameWindow::RIGHT;
		int dir = GetKeyToDir(key);
		if (dir != -1) m_dir = dir;

		// 力を加えて自機を加速させる
		force = 0.03f;
	}

	if (force > 0)
	{
		const auto myPosition = GetXZ(GetPosition());
		if (DirectX::SimpleMath::Vector2::Distance(myPosition, m_lastPos) > .8f)
		{
			auto stage = GameContext::Get<GameWindow>()->GetStage();
			const auto enemyPosition = GetXZ(stage->GetEnemy01()->GetPosition());
			const auto myVelocity = GetXZ(GetVelocity());
			const auto enemyVelocity = GetXZ(stage->GetEnemy01()->GetVelocity());

			GameContext::Get<GameAI>()->AddLerningData(
				GameAI::Input{ myPosition , enemyPosition, myVelocity, enemyVelocity, CheckFloor() },
				GameAI::Output{ (key & GameWindow::UP) != 0, (key & GameWindow::DOWN) != 0, (key & GameWindow::LEFT) != 0, (key & GameWindow::RIGHT) != 0 }
			);

			m_lastPos = myPosition;
		}
	}

	// スペースキーでジャンプ
	if (m_state == STATE_NORMAL && m_jumpParts && tracker.pressed.Space)
	{
		m_state = STATE_JUMP;
		m_jumpCounter = JUMP_FRAME;

		{
			// ジャンプエフェクト生成・登録
			std::unique_ptr<JumpEffect> jumpEffect = std::make_unique<JumpEffect>();
			jumpEffect->Initialize(m_position);
			GameContext::Get<ObjectManager>()->GetGameOM()->Add(std::move(jumpEffect));
		}

		GameContext::Get<Adx2Le>()->Play(CRI_CUESHEET_0_CUE_0);
	}

	// 力を加える
	AddForce(GameWindow::DIR_ANGLE[m_dir], force);
}

void Player::Reset()
{
	// プレイヤーを元の状態に戻す
	m_dir = 0;
	m_position = DirectX::SimpleMath::Vector3((float)m_x, 0.0f, (float)m_y);
	m_state = STATE_NORMAL;
	SetDisplayFlag(true);
	SetDrawPrio(GameWindow::DRAW_OBJECT);
}

void Player::SetPowerupParts(int cnt)
{
	m_powerupParts = cnt;
	m_weight = WEIGHT + m_powerupParts * 0.1f;
}

void Player::SetJumpParts(bool flag)
{
	m_jumpParts = flag;
}

int Player::GetKeyToDir(int key)
{
	// ビットフィールドのキー情報を８方向(0〜7）に変換するテーブル
	int table[16] = { -1, 0, 2, 1, 4, -1, 3, -1, 6, 7, -1, -1, 5, -1, -1, -1 };
	return table[key];
}

void Player::SetJumpEndFunction(std::function<void(Object*)> func)
{
	// ジャンプの終了時に呼ばれる関数を登録する
	m_jumpEndFunction = func;
}
