//////////////////////////////////////////////////////////////////////////////////
///  @file   SceneDownSample.cpp
///  @brief  �`��V�[�����c�������ɏk������V�F�[�_. �ڂ��������邱�Ƃŕi�������サ�Ă��܂� @n
///          �g�p�}�N�� @n
///          - USE_COMPUTE_SHADER (Switch) : �R���s���[�g�V�F�[�_�[���g�p���܂�. �g�p����ꍇ��THREADGROUP_SIZE_X, Y���`���Ă�������@n
///          - USE_LOW_QUALITY    (Switch) : �_�E���T���v�����O�̕i���������܂�. ������ꍇ, �ڂ��������܂���.
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
/*! @brief �f�t�H���g�R���X�g���N�^*/
SceneDownSample::SceneDownSample()
{

}

/*! @brief �f�X�g���N�^*/
SceneDownSample::~SceneDownSample()
{

}

/*! @brief �G���W�����g����������*/
SceneDownSample::SceneDownSample(const LowLevelGraphicsEnginePtr& engine, const tstring& useMacro, const tstring& addName)
	: IFullScreenEffector(engine)
{
	Check(engine);

	const auto name = CombineDebugName(addName, SP("SceneDownSample"));

	/*-------------------------------------------------------------------
	-            �}�N���̏���
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
*  @brief     ��ʂ̃T�C�Y��ύX���܂�.
*  @param[in] const gu::uint32 �V������
*  @param[in] const gu::uint32 �V��������
*  @return    void
*************************************************************************/
void SceneDownSample::OnResize(const gu::uint32 newWidth, const gu::uint32 newHeight)
{
	if (!EnableResize(newWidth, newHeight)) { return; }

	_engine->OnResize(newWidth, newHeight);
}

/*!**********************************************************************
*  @brief     �R�}���h���X�g�ɕ`����e��o�^���܂�
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
		// UAV�̐ݒ肪�܂�
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
*  @brief     �p�C�v���C���X�e�[�g���������܂�.
*  @param[in] const gu::tstring& �f�o�b�O�\����
*  @return    void
*************************************************************************/
void SceneDownSample::PreparePipelineState(const gu::tstring& addName)
{
	const auto device  = _engine->GetDevice();
	const auto factory = device->CreatePipelineFactory();

	/*-------------------------------------------------------------------
	-            �`����ݒ肷��
	---------------------------------------------------------------------*/


	/*-------------------------------------------------------------------
	-            ���\�[�X���C�A�E�g�̏���
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
	-            �V�F�[�_�[�쐬
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
*  @brief     Texture���̃o�b�t�@���������܂�.
*  @param[in] const gu::tstring& �f�o�b�O�\����
*  @return    void
*************************************************************************/
void SceneDownSample::PrepareTextureInfoBuffer(const gu::tstring& addName)
{
	const auto device   = _engine->GetDevice();
	const auto metaData = GPUBufferMetaData::ConstantBuffer(sizeof(SceneTextureInfo), 1);

	// �o�b�t�@�̏���
	const auto buffer = device->CreateBuffer(metaData);
	buffer->SetName(addName + SP("SceneTextureInfo"));
	_resourceViews.Push(device->CreateResourceView(ResourceViewType::ConstantBuffer, buffer));

	// ���̐ݒ�
	_sceneTextureInfo.SceneTextureInvertSize = { 1.0f / _width, 1.0f / _height };
	_sceneTextureInfo.ViewportMax[0] = _width;
	_sceneTextureInfo.ViewportMax[1] = _height;
	_sceneTextureInfo.ViewportMin[0] = 0;
	_sceneTextureInfo.ViewportMin[1] = 0;

	buffer->UploadByte(&_sceneTextureInfo, metaData.GetTotalByte(), 0, nullptr);
	_resourceViews.Push(device->CreateResourceView(ResourceViewType::ConstantBuffer, buffer));
}
#pragma endregion Protected Function