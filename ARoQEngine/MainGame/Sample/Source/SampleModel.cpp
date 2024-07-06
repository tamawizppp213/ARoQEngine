//////////////////////////////////////////////////////////////////////////////////
//              @file   SampleSky.cpp
///             @brief  SampleSky
///             @author Toide Yutaro
///             @date   2022_04_23
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "MainGame/Sample/Include/SampleModel.hpp"
#include "GameCore/Rendering/EnvironmentMap/Include/SkyDome.hpp"
#include "GameCore/Core/Include/Camera.hpp"
#include "GameCore/Rendering/Model/Include/GameModel.hpp"
#include "GameUtility/Base/Include/Screen.hpp"
#include "GameCore/Rendering/PostProcess/Include/DepthOfField.hpp"
#include "GameCore/Rendering/Core/BasePass/Include/BasePassZPrepass.hpp"
#include "GameCore/Rendering/Core/BasePass/Include/BasePassGBuffer.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace sample;
using namespace rhi;
using namespace rhi::core;
using namespace gc;
using namespace gc::core;

gu::SharedPointer<basepass::ZPrepass> zprepass = nullptr;
gu::SharedPointer<basepass::GBuffer> gbuffer = nullptr;
//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
SampleModel::SampleModel()
{

}
SampleModel::~SampleModel()
{

}
#pragma region Public Function
/****************************************************************************
*                       Initialize
****************************************************************************/
/* @fn        void SampleModel::Initialize(const GameTimerPtr& gameTimer)
*  @brief     Initialize scene
*  @param[in] GameTimer* gameTimer
*  @return 　　void
*****************************************************************************/
void SampleModel::Initialize(const PPPEnginePtr& engine, const GameTimerPtr& gameTimer)
{
	Scene::Initialize(engine, gameTimer);
}
/****************************************************************************
*                       Update
****************************************************************************/
/* @fn        void SampleModel::Update()
*  @brief     Update Scene
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void SampleModel::Update()
{
	Scene::Update();
	_camera->Update(_gameTimer);
	_model->Update(_gameTimer->DeltaTime());
}
/****************************************************************************
*                       Draw
****************************************************************************/
/* @fn        void SampleModel::Draw()
*  @brief     Draw Scene
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void SampleModel::Draw()
{
	/*zprepass->Draw(_camera->GetResourceView());
	gbuffer->Draw(_camera->GetResourceView());*/
	_engine->BeginSwapchainRenderPass();
	_skybox->Draw(_camera->GetResourceView());
	//_dof->Draw(zprepass->GetRenderedTextureView());
}
/****************************************************************************
*                       Terminate
****************************************************************************/
/* @fn        void SampleModel::Terminate()
*  @brief     Terminate Scene
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void SampleModel::Terminate()
{
	
}
#pragma endregion Public Function

#pragma region Protected Function

/****************************************************************************
*                       LoadMaterials
****************************************************************************/
/* @fn        void SampleModel::LoadMaterials(GameTimer* gameTimer)
*  @brief     Load Materials
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void SampleModel::LoadMaterials()
{
	/*-------------------------------------------------------------------
	-             Open Copy CommandList
	---------------------------------------------------------------------*/
	const auto copyCommandList = _engine->GetCommandList(CommandListType::Copy);
	const auto graphicsCommandList = _engine->GetCommandList(CommandListType::Graphics);
	copyCommandList->BeginRecording();
	graphicsCommandList->BeginRecording();
	/*-------------------------------------------------------------------
	-           Camera
	---------------------------------------------------------------------*/
	_camera = gu::MakeShared<Camera>(_engine);
	_camera->SetPosition(0.0f, 10.0f, -20.0f);
	/*-------------------------------------------------------------------
	-           Skybox
	---------------------------------------------------------------------*/
	_skybox = gu::MakeShared<SkyDome>(_engine, SP("Resources/grasscube1024.dds"));
	/*-------------------------------------------------------------------
	-           Model
	---------------------------------------------------------------------*/
	_model = GameObject::Create<GameModel>(_engine);
	_model->Load(SP("Resources/YYB Hatsune Miku/YYB Hatsune Miku_10th_v1.02.pmx"));
	_model->SetDebugColor(gm::Float4(1, 0, 0, 1));

	zprepass = gu::MakeShared<basepass::ZPrepass>(_engine, Screen::GetScreenWidth(), Screen::GetScreenHeight());
	zprepass->Add(_model);
	gbuffer = gu::MakeShared<basepass::GBuffer>(_engine);
	gbuffer->Add(_model);

	_dof = gu::MakeShared<Dof>(_engine, (float)Screen::GetScreenWidth(), (float)Screen::GetScreenHeight());

	/*-------------------------------------------------------------------
	-             Close Copy CommandList and Flush CommandQueue
	---------------------------------------------------------------------*/
	graphicsCommandList->EndRecording();
	copyCommandList->EndRecording();

	_engine->FlushGPUCommands(CommandListType::Graphics);
	_engine->FlushGPUCommands(CommandListType::Copy);
}
/****************************************************************************
*                       OnKeyboardInput
****************************************************************************/
/* @fn        void SampleModel::OnKeyboardInput()
*  @brief     KeyboardInput
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void SampleModel::OnKeyboardInput()
{
	const float deltaTime = _gameTimer->DeltaTime();
	const float speed = 15.0f;
	/*-------------------------------------------------------------------
	-           Keyboad Input W (Move Camera )
	---------------------------------------------------------------------*/
	if (_gameInput.GetKeyboard()->IsPress(DIK_W))
	{
		_camera->Walk(speed * deltaTime);
	}
	/*-------------------------------------------------------------------
	-           Keyboad Input S (Move Camera)
	---------------------------------------------------------------------*/
	if (_gameInput.GetKeyboard()->IsPress(DIK_S))
	{
		_camera->Walk(-speed * deltaTime);
	}
	if(_gameInput.GetKeyboard()->IsPress(DIK_D))
	{
		_camera->Strafe(speed * deltaTime);
	}
	/*-------------------------------------------------------------------
	-           Keyboad Input S (Move Camera)
	---------------------------------------------------------------------*/
	if (_gameInput.GetKeyboard()->IsPress(DIK_A))
	{
		_camera->Strafe(-speed * deltaTime);
	}

}
/****************************************************************************
*                       OnMouseInput
****************************************************************************/
/* @fn        void SampleModel::OnMouseInput()
*  @brief     MouseInput
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void SampleModel::OnMouseInput()
{
	/*-------------------------------------------------------------------
	-           Mouse Input Left Button
	---------------------------------------------------------------------*/
	if (_gameInput.GetMouse()->IsPress(MouseButton::LEFT))
	{
		//float dx = DirectX::XMConvertToRadians(0.25f * static_cast<float>(_gameInput.GetMouse()->GetMouseVelocity().x)); // 0.25f: 感度設定したい
		//float dy = DirectX::XMConvertToRadians(0.25f * static_cast<float>(_gameInput.GetMouse()->GetMouseVelocity().y));

		// _camera->RotatePitch(dy);
		// _camera->RotateWorldY(dx);
	}
}
/****************************************************************************
*                       OnGamePadInput
****************************************************************************/
/* @fn        void SampleModel::OnGamePadInput()
*  @brief     GamePadInput
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void SampleModel::OnGamePadInput()
{

}

#pragma endregion Protected Function