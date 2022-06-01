//////////////////////////////////////////////////////////////////////////////////
//              @file   UIRenderer.cpp
///             @brief  UI Renderer
///             @author Toide Yutaro
///             @date   2022_06_01
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Rendering/Sprite/Include/UIRenderer.hpp"
#include "GraphicsCore/Engine/Include/GraphicsCoreEngine.hpp"
#include "GraphicsCore/DirectX12/Core/Include/DirectX12Buffer.hpp"
#include "GraphicsCore/DirectX12/Core/Include/DirectX12PrimitiveGeometry.hpp"
#include "GraphicsCore/DirectX12/SimpleInclude/IncludeGraphicsPSO.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace ui;
using namespace gm;
namespace
{
	RootSignature         s_RootSignature;
	GraphicsPipelineState s_PipelineState;
}

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
UIRenderer::~UIRenderer()
{

}
/****************************************************************************
*					StartUp
*************************************************************************//**
*  @fn        void UIRenderer::StartUp(const std::wstring& addName)
*  @brief     Start up module
*  @param[in] const std::wstring& addName
*  @return 　　void
*****************************************************************************/
void UIRenderer::StartUp(const std::wstring& addName)
{
	/*-------------------------------------------------------------------
	-            Set name
	---------------------------------------------------------------------*/
	std::wstring name = L""; if (addName != L"") { name += addName; name += L"::"; }
	name += L"UIRenderer::";
	/*-------------------------------------------------------------------
	-            Prepare Resource
	---------------------------------------------------------------------*/
	PrepareRootSignature(name);
	PreparePipelineState(name);
	PrepareVertexAndIndexBuffer(name);

}

/****************************************************************************
*					AddFrameObject
*************************************************************************//**
*  @fn        void UIRenderer::AddFrameObject(const std::vector<ui::Image>& images, const Texture& texture)
*  @brief     Add Frame Object
*  @param[in] const std::vector<ui::Image>& image
*  @param[in] const Texture& texture
*  @return 　　void
*****************************************************************************/
void UIRenderer::AddFrameObject(const std::vector<ui::Image>& images, const Texture& texture)
{
	/*-------------------------------------------------------------------
	-               sprite count check
	---------------------------------------------------------------------*/
	if (_imageStackCount + images.size() > MAX_WRITABLE_UI_COUNT)
	{
		MessageBox(NULL, L"The maximum number of sprites exceeded. \n If the maximum number is not exceeded, please check whether DrawEnd is being called.", L"Warning", MB_ICONWARNING);
		return;
	}
	/*-------------------------------------------------------------------
	-               Check whether spriteList is empty
	---------------------------------------------------------------------*/
	if (images.empty()) { return ; }
	/*-------------------------------------------------------------------
	-               FrameStart Check
	---------------------------------------------------------------------*/
	if (!_isFrameStart)
	{
		auto& engine  = GraphicsCoreEngine::Instance();
		_context      = engine.GetCommandContext();
		_currentFrame = engine.GetGraphicsDevice()->GetCurrentFrameIndex();
		_isFrameStart = true;

	}
	/*-------------------------------------------------------------------
	-               Add vertex data
	---------------------------------------------------------------------*/
	_meshBuffer[_currentFrame].VertexBuffer->CopyStart();
	for (int i = 0; i < (int)gm::Min((INT64)images.size(), (INT64)MAX_WRITABLE_UI_COUNT - (INT64)_imageStackCount); ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			_meshBuffer[_currentFrame].VertexBuffer->CopyData((i + _imageStackCount) * 4 + j, (const void*)(&images[i].GetVertices()[j]));
		}
	}
	_meshBuffer[_currentFrame].VertexBuffer->CopyEnd();
	/*-------------------------------------------------------------------
	-               Count sprite num
	---------------------------------------------------------------------*/
	_drawCallNum++;
	_imageStackCount += static_cast<int>(images.size());
	_imageCountList.push_back(static_cast<int>(images.size()));
	_textures.push_back(texture);
}

/****************************************************************************
*					Draw
*************************************************************************//**
*  @fn        void UIRenderer::Draw()
*  @brief     Draw
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void UIRenderer::Draw()
{
	_context->SetRootSignature(s_RootSignature.GetSignature());
	_context->SetPipelineState(s_PipelineState.GetPipelineState());
	_context->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	_context->SetVertexBuffer(_meshBuffer[_currentFrame].VertexBufferView());
	_context->SetIndexBuffer(_meshBuffer[_currentFrame].IndexBufferView());

	/*-------------------------------------------------------------------
	-                 Draw
	---------------------------------------------------------------------*/
	int imageCounter = 0;
	for (int i = 0; i < _drawCallNum; ++i)
	{
		_context->SetGraphicsRootDescriptorTable(0, GraphicsCoreEngine::Instance().GetGraphicsDevice()->GetGPUResourceView(HeapFlag::SRV, _textures[i].TextureID));
		_context->DrawIndexedInstanced((UINT)(6 * _imageCountList[i]), 1, 6 * imageCounter, 0, 1);
		imageCounter += _imageCountList[i];
	}

	/*-------------------------------------------------------------------
	-               Add vertex data
	---------------------------------------------------------------------*/
	_meshBuffer[_currentFrame].VertexBuffer->CopyStart();
	for (int i = 0; i < (int)gm::Min((INT64)_imageStackCount, (INT64)MAX_WRITABLE_UI_COUNT); ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			auto vertex = VertexPositionNormalColorTexture(Float3(0, 0, 0), Float3(0, 0, 0), Float4(1, 1, 1, 1), Float2(0, 0));
			_meshBuffer[_currentFrame].VertexBuffer->CopyData((i + _imageStackCount) * 4 + j, (const void*)(&vertex));
		}
	}
	_meshBuffer[_currentFrame].VertexBuffer->CopyEnd();

	/*-------------------------------------------------------------------
	-               Reset Stack Count
	---------------------------------------------------------------------*/
	_imageStackCount = 0;
	_drawCallNum     = 0;
	_imageCountList.clear();
	_textures.clear();

	_isFrameStart = false;

}
/****************************************************************************
*					ShutDown
*************************************************************************//**
*  @fn        void UIRenderer::ShutDown
*  @brief     Draw
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void UIRenderer::ShutDown()
{
	_meshBuffer    .reset();
	_textures      .clear(); _textures.shrink_to_fit();
	_imageCountList.clear(); _imageCountList.shrink_to_fit();
}
#pragma region Protected Function
/****************************************************************************
*					PrepareVertexAndIndexBuffer
*************************************************************************//**
*  @fn        void UIRenderer::PrepareVertexAndIndexBuffer(const std::wstring& name)
*  @brief     Prepare Rect vertex and index buffer
*  @param[in] const std::wstring& name
*  @return 　　bool
*****************************************************************************/
void UIRenderer::PrepareVertexAndIndexBuffer(const std::wstring& name)
{
	/*-------------------------------------------------------------------
	-            Create Index List
	---------------------------------------------------------------------*/
	std::vector<UINT32> indices(MAX_WRITABLE_UI_COUNT * 6);
	const UINT32 rectIndex[] = {0,1,3,1,2,3};
	for (int i = 0; i < MAX_WRITABLE_UI_COUNT; ++i)
	{
		for (int j = 0; j < 6; ++j)
		{
			indices[(UINT64)6 * i + j] = i * 4 + rectIndex[j];
		}
	}

	/*-------------------------------------------------------------------
	-            Create Mesh Buffer
	---------------------------------------------------------------------*/
	auto& engine       = GraphicsCoreEngine::Instance();
	int totalFrameSize = engine.GetFrameBufferCount();
	_meshBuffer = std::make_unique<MeshBuffer[]>(totalFrameSize);
	for (int i = 0; i < totalFrameSize; ++i)
	{
		auto& meshBuffer = _meshBuffer[i];
		/*-------------------------------------------------------------------
		-            Calcurate Buffer Size
		---------------------------------------------------------------------*/
		auto vertexByteSize = sizeof(VertexPositionNormalColorTexture);       
		auto indexByteSize  = sizeof(UINT32);                                // 32 byte index
		auto vertexCount    = MAX_WRITABLE_UI_COUNT * 4;
		auto indexCount     = indices.size();

		/*-------------------------------------------------------------------
		-            Set Vertex Buffer and Index Buffer
		---------------------------------------------------------------------*/
		meshBuffer.VertexBuffer = std::make_unique<UploadBuffer>(engine.GetDevice(), static_cast<UINT>(vertexByteSize), static_cast<UINT>(vertexCount), false, name + L"VertexBuffer");
		meshBuffer.VertexBuffer->CopyStart();
		for (int j = 0; j < MAX_WRITABLE_UI_COUNT * 4; ++j)
		{
			auto vertex = VertexPositionNormalColorTexture(Float3(0, 0, 0), Float3(0, 0, 0), Float4(1, 1, 1, 1), Float2(0, 0));
			meshBuffer.VertexBuffer->CopyData(j, &vertex);
		}
		meshBuffer.VertexBuffer->CopyEnd();

		meshBuffer.IndexBuffer = std::make_unique<UploadBuffer>(engine.GetDevice(), static_cast<UINT>(indexByteSize), static_cast<UINT>(indexCount), false, name + L"IndexBuffer");
		meshBuffer.IndexBuffer->CopyStart();
		meshBuffer.IndexBuffer->CopyTotalData(indices.data(), static_cast<int>(indexCount));
		meshBuffer.IndexBuffer->CopyEnd();
	}
}
/****************************************************************************
*							PrepareRootSignature
*************************************************************************//**
*  @fn        void Skybox::PrepareRootSignature(const std::wstring& name)
*  @brief     Prepare RootSignature for UIRenderer
*  @param[in] std::wstring& name
*  @return 　　void
*****************************************************************************/
void UIRenderer::PrepareRootSignature(const std::wstring& name)
{
	s_RootSignature.Reset(1, 1);
	s_RootSignature.SetStaticSampler(SamplerType::SamplerLinearWrap);
	s_RootSignature[0].InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 0, 1); // Texture
	s_RootSignature.CompleteSetting(GraphicsCoreEngine::Instance().GetDevice(), name + L"RootSignature");
}
/****************************************************************************
*							PreparePipelineState
*************************************************************************//**
*  @fn        void Skybox::PreparePipelineState(const std::wstring& name)
*  @brief     Prepare pipelineState
*  @param[in] std::wstring& name
*  @return 　　void
*****************************************************************************/
void UIRenderer::PreparePipelineState(const std::wstring& name)
{
	auto device = GraphicsCoreEngine::Instance().GetGraphicsDevice();
	RASTERIZER_DESC rasterizerState = RASTERIZER_DESC(D3D12_DEFAULT);
	rasterizerState.CullMode        = D3D12_CULL_MODE_NONE;               // All culling 

	DEPTH_STENCIL_DESC depthStencil = DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	depthStencil.DepthFunc          = D3D12_COMPARISON_FUNC_LESS_EQUAL;

	BlobComPtr vs = CompileShader(L"Shader\\Sprite\\ShaderDefault2D.hlsl", L"VSMain", L"vs_6_4");
	BlobComPtr ps = CompileShader(L"Shader\\Sprite\\ShaderDefault2D.hlsl", L"PSMain", L"ps_6_4");

	s_PipelineState.SetGraphicsPipelineStateDescriptor(device->GetDefaultPSOConfig());
	s_PipelineState.SetRasterizerState(rasterizerState);
	s_PipelineState.SetInputLayouts(VertexPositionNormalColorTexture::InputLayout);
	s_PipelineState.SetRootSignature(s_RootSignature);
	s_PipelineState.SetDepthStencilState(depthStencil);
	s_PipelineState.SetBlendState(GetBlendState(BlendStateType::AlphaBlend));
	s_PipelineState.SetVertexShader(vs);
	s_PipelineState.SetPixelShader(ps);
	s_PipelineState.CompleteSetting(GraphicsCoreEngine::Instance().GetDevice(), name + L"PipelineState");
}
#pragma endregion Protected Function