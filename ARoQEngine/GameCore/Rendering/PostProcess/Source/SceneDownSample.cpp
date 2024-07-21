//////////////////////////////////////////////////////////////////////////////////
///  @file   SceneDownSample.cpp
///  @brief  描画シーンを縦横半分に縮小するシェーダ. ぼかしを入れることで品質を向上しています @n
///          使用マクロ @n
///          - USE_COMPUTE_SHADER (Switch) : コンピュートシェーダーを使用します. 使用する場合はTHREADGROUP_SIZE_X, Yを定義してください@n
///          - USE_LOW_QUALITY    (Switch) : ダウンサンプリングの品質を下げます. 下げる場合, ぼかしを入れません.
///  @author toide
///  @date   2024/07/18 0:43:02
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Rendering/PostProcess/Include/SceneDownSample.hpp"
#include "GraphicsCore/Engine/Include/LowLevelGraphicsEngine.hpp"
#include "GraphicsCore/RHI/InterfaceCore/PipelineState/Include/GPUPipelineState.hpp"
#include "GraphicsCore/RHI/InterfaceCore/PipelineState/Include/GPUPipelineFactory.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIFrameBuffer.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUBuffer.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUResourceView.hpp"
#include "GameUtility/Base/Include/GUParse.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace engine;
using namespace rhi::core;
using namespace gu;

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor
/*! @brief デフォルトコンストラクタ*/
SceneDownSample::SceneDownSample()
{

}

/*! @brief デストラクタ*/
SceneDownSample::~SceneDownSample()
{

}

/*! @brief エンジンを使った初期化*/
SceneDownSample::SceneDownSample(const LowLevelGraphicsEnginePtr& engine, const tstring& useMacro, const tstring& addName)
	: IFullScreenEffector(engine)
{
	Check(engine);

	const auto name = CombineDebugName(addName, SP("SceneDownSample"));

	/*-------------------------------------------------------------------
	-            マクロの準備
	---------------------------------------------------------------------*/
	if (Parse::Contains(useMacro, USE_COMPUTE_SHADER_MACRO))
	{
		_useMacros.Push(USE_COMPUTE_SHADER_MACRO);
		_useComputeShader = true;
	}
	if (Parse::Contains(useMacro, USE_LOW_QUALITY_MACRO))
	{
		_useMacros.Push(USE_LOW_QUALITY_MACRO);
	}

	PreparePipelineState(name);
}
#pragma endregion Constructor and Destructor

#pragma region Public Function
/*!**********************************************************************
*  @brief     画面のサイズを変更します.
*  @param[in] const gu::uint32 新しい幅
*  @param[in] const gu::uint32 新しい高さ
*  @return    void
*************************************************************************/
void SceneDownSample::OnResize(const gu::uint32 newWidth, const gu::uint32 newHeight)
{
	if (!EnableResize(newWidth, newHeight)) { return; }

	_engine->OnResize(newWidth, newHeight);
}

/*!**********************************************************************
*  @brief     コマンドリストに描画内容を登録します
*  @param[in] void
*  @return    void
*************************************************************************/
void SceneDownSample::Draw()
{
	const auto frameIndex  = _engine->GetCurrentFrameIndex();
	const auto device      = _engine->GetDevice();
	const auto commandList = _engine->GetCommandList(CommandListType::Graphics);
	
	if (_useComputeShader)
	{
		commandList->SetResourceLayout(_resourceLayout);
		commandList->SetComputePipeline(_computePipeline);
		_engine->GetFrameBuffer(frameIndex)->GetRenderTargetSRV()->Bind(commandList, 0);
		// UAVの設定がまだ
		commandList->Dispatch(THREAD_GROUP_SIZE, THREAD_GROUP_SIZE, 1);
	}
	else
	{
		commandList->SetResourceLayout(_resourceLayout);
		commandList->SetGraphicsPipeline(_pipeline);
		commandList->SetVertexBuffer(_vertexBuffers[frameIndex]);
		commandList->SetIndexBuffer(_indexBuffer, rhi::core::PixelFormat::R16_UINT);
		_resourceViews[0]->Bind(commandList, 0);
		_engine->GetFrameBuffer(frameIndex)->GetRenderTargetSRV()->Bind(commandList, 0);
		commandList->DrawIndexedInstanced(6, 1, 0, 0, 0);
	}
}

#pragma endregion Public Function

#pragma region Protected Function
/*!**********************************************************************
*  @brief     パイプラインステートを準備します.
*  @param[in] const gu::tstring& デバッグ表示名
*  @return    void
*************************************************************************/
void SceneDownSample::PreparePipelineState(const gu::tstring& addName)
{
	const auto device  = _engine->GetDevice();
	const auto factory = device->CreatePipelineFactory();

	/*-------------------------------------------------------------------
	-            描画先を設定する
	---------------------------------------------------------------------*/


	/*-------------------------------------------------------------------
	-            リソースレイアウトの準備
	---------------------------------------------------------------------*/
	gu::DynamicArray<rhi::core::ResourceLayoutElement> elements =
	{
		rhi::core::ResourceLayoutElement(DescriptorHeapType::CBV, 0), // SceneTextureInfo
		rhi::core::ResourceLayoutElement(DescriptorHeapType::SRV, 0), // SceneTexture
	};

	if (UseComputeShader())
	{
		elements.Push(ResourceLayoutElement(DescriptorHeapType::UAV, 0)); 
	}

	_resourceLayout = device->CreateResourceLayout
	(
		elements,{ SamplerLayoutElement(device->CreateSampler(SamplerInfo::GetDefaultSampler(LinearClamp)), 0)}
	);

	/*-------------------------------------------------------------------
	-            シェーダー作成
	---------------------------------------------------------------------*/
	if (UseComputeShader())
	{
		_computePipeline = CreateDefaultFullScreenComputePipelineState(_resourceLayout, factory->CreateShaderState());
	}
	else
	{
		const auto vs = factory->CreateShaderState();
		const auto ps = factory->CreateShaderState();
		vs->Compile({ ShaderType::Vertex, SP("Shader\\PostProcess\\ShaderScreenPass.hlsl"     ), SP("MainVS"), { SP("Shader\\Core")} });
		ps->Compile({ ShaderType::Pixel , SP("Shader\\PostProcess\\ShaderSceneDownSample.hlsl"), SP("MainPS"), { SP("Shader\\Core")}, _useMacros });

		_pipeline = CreateDefaultFullScreenGraphicsPipelineState(_engine->GetDrawContinueRenderPass(), _resourceLayout, vs, ps);
		_pipeline->SetName(CombineDebugName(addName, SP("PSO")));
	}
}

/*!**********************************************************************
*  @brief     Texture情報のバッファを準備します.
*  @param[in] const gu::tstring& デバッグ表示名
*  @return    void
*************************************************************************/
void SceneDownSample::PrepareTextureInfoBuffer(const gu::tstring& addName)
{
	const auto device   = _engine->GetDevice();
	const auto metaData = GPUBufferMetaData::ConstantBuffer(sizeof(SceneTextureInfo), 1);

	// バッファの準備
	const auto buffer = device->CreateBuffer(metaData);
	buffer->SetName(addName + SP("SceneTextureInfo"));
	_resourceViews.Push(device->CreateResourceView(ResourceViewType::ConstantBuffer, buffer));

	// 情報の設定
	_sceneTextureInfo.SceneTextureInvertSize = { 1.0f / _width, 1.0f / _height };
	_sceneTextureInfo.ViewportMax[0] = _width;
	_sceneTextureInfo.ViewportMax[1] = _height;
	_sceneTextureInfo.ViewportMin[0] = 0;
	_sceneTextureInfo.ViewportMin[1] = 0;

	buffer->UploadByte(&_sceneTextureInfo, metaData.GetTotalByte(), 0, nullptr);
	_resourceViews.Push(device->CreateResourceView(ResourceViewType::ConstantBuffer, buffer));
}
#pragma endregion Protected Function