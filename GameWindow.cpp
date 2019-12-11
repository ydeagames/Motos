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

const float GameWindow::GRAVITY = 9.8f;

const float GameWindow::FALL_SPEED = 9.8f;
const float GameWindow::FALL_ROTATE_SPEED = 9.8f;
const float GameWindow::FALL_DISTANCE = 9.8f;


GameWindow::GameWindow()
{
	SetDrawPrio(GameWindow::DRAW_TOP);
}

GameWindow::~GameWindow()
{
	if (m_pStage)
		m_pStage->ResetStageData();
	if (m_pBg)
		GameObject::Destroy(m_pBg);
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

