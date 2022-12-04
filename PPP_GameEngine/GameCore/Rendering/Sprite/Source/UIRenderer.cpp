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
#include "GraphicsCore/Engine/Include/LowLevelGraphicsEngine.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIResourceLayout.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUBuffer.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUResourceView.hpp"
#include "GraphicsCore/RHI/InterfaceCore/PipelineState/Include/GPUPipelineState.hpp"
#include "GraphicsCore/RHI/InterfaceCore/PipelineState/Include/GPUPipelineFactory.hpp"
#include <stdexcept>

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi;
using namespace rhi::core;
using namespace ui;
using namespace gm;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
UIRenderer::UIRenderer(const LowLevelGraphicsEnginePtr& engine, const std::wstring& addName, const std::uint32_t maxUICount)
	: _engine(engine), _maxWritableUICount(maxUICount)
{
	/*-------------------------------------------------------------------
	-            Set name
	---------------------------------------------------------------------*/
	std::wstring name = L""; if (addName != L"") { name += addName; name += L"::"; }
	name += L"UIRenderer::";
	/*-------------------------------------------------------------------
	-            Prepare Resources
	---------------------------------------------------------------------*/
	PrepareMaxImageBuffer(name);
	PreparePipelineState(name);
}
UIRenderer::~UIRenderer()
{
	if (!_vertexBuffers.empty()) { _vertexBuffers.clear(); _vertexBuffers.shrink_to_fit(); }
	if (!_indexBuffers.empty())  { _indexBuffers.clear(), _indexBuffers.shrink_to_fit(); }
} 

/****************************************************************************
*					AddFrameObject
*************************************************************************//**
*  @fn        void UIRenderer::AddFrameObject(const std::vector<ui::Image>& images, const Texture& texture)
*  @brief     Add Frame Object
*  @param[in] const std::vector<ui::Image>& image
*  @param[in] const Texture& texture
*  @return Å@Å@void
*****************************************************************************/
void UIRenderer::AddFrameObjects(const std::vector<ImagePtr>& images, const ResourceViewPtr& view)
{
	/*-------------------------------------------------------------------
	-               sprite count check
	---------------------------------------------------------------------*/
	if (_totalImageCount + images.size() > _maxWritableUICount)
	{
		throw std::runtime_error("The maximum number of sprites exceeded. \n If the maximum number is not exceeded, please check whether DrawEnd is being called. \n");
	}
	/*-------------------------------------------------------------------
	-               Check whether spriteList is empty
	---------------------------------------------------------------------*/
	if (images.empty()) { return ; }

	/*-------------------------------------------------------------------
	-               Add vertex data
	---------------------------------------------------------------------*/
	const auto currentFrame = _engine->GetCurrentFrameIndex();
	const auto vertexBuffer = _vertexBuffers[currentFrame];
	const auto oneRectVertexCount = 4;

	vertexBuffer->CopyStart();
	// _maxWritableUICount - _totalImageCount is écÇËÇÃìoò^Ç≈Ç´ÇÈêî
	for (std::uint32_t i = 0; i < std::min<std::uint32_t>(images.size(), _maxWritableUICount - _totalImageCount); ++i)
	{
		vertexBuffer->CopyTotalData(images[i]->GetVertices(), 4, (i + _totalImageCount) * oneRectVertexCount);
	}
	vertexBuffer->CopyEnd();
	/*-------------------------------------------------------------------
	-               Count sprite num
	---------------------------------------------------------------------*/
	_needCallDrawIndexCount++;
	_totalImageCount += static_cast<std::uint32_t>(images.size());
	_imageCountList.emplace_back(static_cast<std::uint32_t>(images.size()));
	_resourceViews.emplace_back(view);
}

/****************************************************************************
*					Draw
*************************************************************************//**
*  @fn        void UIRenderer::Draw()
*  @brief     Draw
*  @param[in] void
*  @return Å@Å@void
*****************************************************************************/
void UIRenderer::Draw()
{
	if (_totalImageCount == 0) { return; }

	const std::uint32_t currentFrame = _engine->GetCurrentFrameIndex();
	const auto commandList  = _engine->GetCommandList(core::CommandListType::Graphics, currentFrame);

	/*-------------------------------------------------------------------
	-                 Draw command list
	---------------------------------------------------------------------*/
	commandList->SetResourceLayout(_resourceLayout);
	commandList->SetGraphicsPipeline(_pipeline);
	commandList->SetVertexBuffer(_vertexBuffers[currentFrame]);
	commandList->SetIndexBuffer (_indexBuffers[currentFrame]);

	/*-------------------------------------------------------------------
	-                 Draw
	---------------------------------------------------------------------*/
	std::uint32_t indexOffset = 0;
	for (std::uint32_t i = 0; i < _needCallDrawIndexCount; ++i)
	{
		// Regist root descriptor table 
		commandList->SetDescriptorHeap(_resourceViews[i]->GetHeap());
		_resourceViews[i]->Bind(commandList, 0);

		// Draw Rects in one texture at a time
		const std::uint32_t oneImageTotalIndexCount = 6 * _imageCountList[i];
		commandList->DrawIndexedInstanced(oneImageTotalIndexCount, 1, indexOffset, 0, 0);
		indexOffset += oneImageTotalIndexCount;
	}

	/*-------------------------------------------------------------------
	-               Clear vertex data
	---------------------------------------------------------------------*/
	ClearVertexBuffer(currentFrame, 4 * _totalImageCount);

	/*-------------------------------------------------------------------
	-               Reset Stack Count
	---------------------------------------------------------------------*/
	_totalImageCount = 0;
	_needCallDrawIndexCount = 0;
	_imageCountList.clear(); _imageCountList.shrink_to_fit();
	_resourceViews.clear(); _resourceViews.shrink_to_fit();
}

#pragma region Protected Function
void UIRenderer::ClearVertexBuffer(const std::uint32_t frameIndex, const size_t vertexCount)
{
	if (vertexCount == 0) { return; }

	std::vector<gm::Vertex> v
	(
		vertexCount,
		Vertex(Float3(0, 0, 0), Float3(0, 0, 0), Float4(1, 1, 1, 1), Float2(0, 0))
	);
	
	_vertexBuffers[frameIndex]->Update(v.data(), vertexCount);
}
void UIRenderer::PrepareMaxImageBuffer(const std::wstring& name)
{
	const auto device = _engine->GetDevice();

	/*-------------------------------------------------------------------
	-            Create Index List
	---------------------------------------------------------------------*/
	const std::uint32_t rectIndex[]     = { 0,1,3,1,2,3 };
	const std::uint32_t rectIndexCount  = static_cast<std::uint32_t>(_countof(rectIndex));
	const std::uint32_t rectVertexCount = 4;

	std::vector<std::uint32_t> indices(_maxWritableUICount * rectIndexCount);
	for (std::uint32_t i = 0; i < _maxWritableUICount; ++i)
	{
		for (std::uint32_t j = 0; j < rectIndexCount; ++j)
		{
			// éüìoò^Ç∑ÇÈRectÇ…ÇÕí∏ì_êîï™Ç∏ÇÁÇ∑ (+4)
			indices[(std::uint64_t)rectIndexCount * i + j] = i * rectVertexCount + rectIndex[j];
		}
	}

	/*-------------------------------------------------------------------
	-            Create Vertex and Index Buffer
	---------------------------------------------------------------------*/
	const auto frameCount = LowLevelGraphicsEngine::FRAME_BUFFER_COUNT;
	
	// prepare frame count buffer
	_vertexBuffers.resize(frameCount);
	_indexBuffers .resize(frameCount);

	for (std::uint32_t i = 0; i < frameCount; ++i)
	{
		{
			const auto totalCount = _maxWritableUICount * rectVertexCount;
			const auto vbMetaData = GPUBufferMetaData::VertexBuffer(sizeof(gm::Vertex), totalCount, core::MemoryHeap::Upload);
			
			_vertexBuffers[i] = device->CreateBuffer(vbMetaData);
			_vertexBuffers[i]->SetName(name + L"VB");
			ClearVertexBuffer(i, totalCount);
		}

		{
			const auto ibMetaData = GPUBufferMetaData::IndexBuffer(sizeof(std::uint32_t), indices.size(), MemoryHeap::Default, ResourceState::Common);
			
			_indexBuffers[i] = device->CreateBuffer(ibMetaData);
			_indexBuffers[i]->SetName(name + L"IB");
			_indexBuffers[i]->Pack(indices.data(), _engine->GetCommandList(CommandListType::Copy, _engine->GetCurrentFrameIndex()));
		}
	}
}

/****************************************************************************
*							PreparePipelineState
*************************************************************************//**
*  @fn        void Skybox::PreparePipelineState(const std::wstring& name)
*  @brief     Prepare pipelineState
*  @param[in] std::wstring& name
*  @return Å@Å@void
*****************************************************************************/
void UIRenderer::PreparePipelineState(const std::wstring& name)
{
	const auto device  = _engine->GetDevice();
	const auto factory = device->CreatePipelineFactory();
	
	/*-------------------------------------------------------------------
	-             Setup resource layout elements
	---------------------------------------------------------------------*/
	_resourceLayout = device->CreateResourceLayout
	(
		{ ResourceLayoutElement(DescriptorHeapType::SRV, 0) },
		{ SamplerLayoutElement(device->CreateSampler(SamplerInfo::GetDefaultSampler(SamplerLinearWrap)),0) }
	);

	/*-------------------------------------------------------------------
	-             Set up graphic pipeline state
	---------------------------------------------------------------------*/
	const auto vs = factory->CreateShaderState();
	const auto ps = factory->CreateShaderState();
	vs->Compile(ShaderType::Vertex, L"Shader\\Sprite\\ShaderDefault2D.hlsl", L"VSMain", 6.4f, { L"Shader\\Core" });
	ps->Compile(ShaderType::Pixel , L"Shader\\Sprite\\ShaderDefault2D.hlsl", L"PSMain", 6.4f, { L"Shader\\Core" });

	/*-------------------------------------------------------------------
	-             Set up graphic pipeline state
	---------------------------------------------------------------------*/
	_pipeline = device->CreateGraphicPipelineState(_engine->GetRenderPass(), _resourceLayout);
	_pipeline->SetBlendState(factory->CreateSingleBlendState(core::BlendProperty::AlphaBlend()));
	_pipeline->SetRasterizerState(factory->CreateRasterizerState());
	_pipeline->SetInputAssemblyState(factory->CreateInputAssemblyState(GPUInputAssemblyState::GetDefaultVertexElement()));
	_pipeline->SetDepthStencilState(factory->CreateDepthStencilState());
	_pipeline->SetVertexShader(vs);
	_pipeline->SetPixelShader(ps);
	_pipeline->CompleteSetting();
	_pipeline->SetName(name + L"PSO");
}
#pragma endregion Protected Function