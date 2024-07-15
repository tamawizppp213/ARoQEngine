//////////////////////////////////////////////////////////////////////////////////
//              @file   UIRenderer.cpp
///             @brief  UI Renderer
///             @author Toide Yutaro
///             @date   2022_06_01
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/UIRenderer.hpp"
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
using namespace engine;
using namespace gm;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor
UIRenderer::UIRenderer(const LowLevelGraphicsEnginePtr& engine, const gu::tstring& addName, const std::uint32_t maxUICount)
	: _engine(engine), _maxWritableUICount(maxUICount)
{
	/*-------------------------------------------------------------------
	-            Set name
	---------------------------------------------------------------------*/
	gu::tstring name = SP(""); if (addName != SP("")) { name += addName; name += SP("::"); }
	name += SP("UIRenderer::");

	/*-------------------------------------------------------------------
	-            Prepare Resources
	---------------------------------------------------------------------*/
	PrepareMaxImageBuffer(name);
	PreparePipelineState(name);
}


UIRenderer::~UIRenderer()
{
	if (!_vertexBuffers.IsEmpty()) { _vertexBuffers.Clear(); _vertexBuffers.ShrinkToFit(); }
	if (!_indexBuffers .IsEmpty()) { _indexBuffers .Clear(), _indexBuffers .ShrinkToFit(); }
} 

#pragma endregion Constructor and Destructor 
void UIRenderer::Clear()
{
	if (_prevTotalImageCount == 0) { return; }

	const auto prevFrameIndex = (_engine->GetCurrentFrameIndex() + LowLevelGraphicsEngine::FRAME_BUFFER_COUNT - 1) % LowLevelGraphicsEngine::FRAME_BUFFER_COUNT;
	ClearVertexBuffer(prevFrameIndex, _prevTotalImageCount);
}

/****************************************************************************
*					AddFrameObject
****************************************************************************/
/* @fn        void UIRenderer::AddFrameObject(const gu::DynamicArray<ui::Image>& images, const Texture& texture)
* 
*  @brief     Add Frame Object
* 
*  @param[in] const gu::DynamicArray<ImagePtr>& image
* 
*  @param[in] const ResourceViewPtr& resourceView
* 
*  @return 　　void
*****************************************************************************/
//void UIRenderer::AddFrameObjects(const gu::DynamicArray<ImagePtr>& images, const ResourceViewPtr& view)
//{
//	/*-------------------------------------------------------------------
//	-               sprite count check
//	---------------------------------------------------------------------*/
//	if (_totalImageCount + images.size() > _maxWritableUICount)
//	{
//		throw std::runtime_error("The maximum number of sprites exceeded. \n If the maximum number is not exceeded, please check whether DrawEnd is being called. \n");
//	}
//
//	/*-------------------------------------------------------------------
//	-               Check whether spriteList is empty
//	---------------------------------------------------------------------*/
//	if (images.empty()) { return ; }
//
//	/*-------------------------------------------------------------------
//	-               Add vertex data
//	---------------------------------------------------------------------*/
//	const auto  currentFrame = _engine->GetCurrentFrameIndex();
//	const auto& vertexBuffer = _vertexBuffers[currentFrame];
//	const auto  oneRectVertexCount = 4;
//
//	vertexBuffer->CopyStart();
//	// _maxWritableUICount - _totalImageCount is 残りの登録できる数
//	for (std::uint32_t i = 0; i < std::min<std::uint32_t>((std::uint32_t)images.size(), _maxWritableUICount - _totalImageCount); ++i)
//	{
//		vertexBuffer->CopyTotalData(images[i]->GetVertices(), 4, (i + _totalImageCount) * oneRectVertexCount);
//	}
//	vertexBuffer->CopyEnd();
//
//	/*-------------------------------------------------------------------
//	-               Count sprite num
//	---------------------------------------------------------------------*/
//	CountUpDrawImageAndView(images.size(), view);
//	
//}

void UIRenderer::AddFrameObjects(const gu::DynamicArray<Image>& images, const ResourceViewPtr& view)
{
	/*-------------------------------------------------------------------
	-               sprite count check
	---------------------------------------------------------------------*/
	if (_totalImageCount + images.Size() > _maxWritableUICount)
	{
		throw std::runtime_error("The maximum number of sprites exceeded. \n If the maximum number is not exceeded, please check whether DrawEnd is being called. \n");
	}

	/*-------------------------------------------------------------------
	-               Check whether spriteList is empty
	---------------------------------------------------------------------*/
	if (images.IsEmpty()) { return ; }

	/*-------------------------------------------------------------------
	-               Add vertex data
	---------------------------------------------------------------------*/
	const auto  currentFrame = _engine->GetCurrentFrameIndex();
	const auto& vertexBuffer = _vertexBuffers[currentFrame];
	const auto  oneRectVertexCount = 4;

	vertexBuffer->Map();
	// _maxWritableUICount - _totalImageCount is 残りの登録できる数
	for (std::uint32_t i = 0; i < std::min<std::uint32_t>((std::uint32_t)images.Size(), _maxWritableUICount - _totalImageCount); ++i)
	{
		vertexBuffer->UploadIndex(images[i].GetVertices(), 4, ((size_t)i + (size_t)_totalImageCount) * (size_t)oneRectVertexCount, nullptr, true);
	}
	vertexBuffer->Unmap();

	/*-------------------------------------------------------------------
	-               Count sprite num
	---------------------------------------------------------------------*/
	CountUpDrawImageAndView(images.Size(), view);
}
/****************************************************************************
*					Draw
****************************************************************************/
/* @fn        void UIRenderer::Draw()
* 
*  @brief     Render all registered frame ui objects
* 
*  @param[in] void
* 
*  @return 　　void
*****************************************************************************/
void UIRenderer::Draw()
{
	_prevTotalImageCount = _totalImageCount;
	if (_totalImageCount == 0) { return; }

	const std::uint32_t currentFrame = _engine->GetCurrentFrameIndex();
	const auto commandList  = _engine->GetCommandList(core::CommandListType::Graphics);

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
		_resourceViews[i]->Bind(commandList, 0, _resourceLayout);

		// Draw Rects in one texture at a time
		const std::uint32_t oneImageTotalIndexCount = 6 * _imageCountList[i];
		commandList->DrawIndexedInstanced(oneImageTotalIndexCount, 1, indexOffset, 0, 0);
		indexOffset += oneImageTotalIndexCount;
	}

	/*-------------------------------------------------------------------
	-               Reset Stack Count
	---------------------------------------------------------------------*/
	_totalImageCount = 0;
	_needCallDrawIndexCount = 0;
	_imageCountList.Clear(); _imageCountList.ShrinkToFit();
	_resourceViews.Clear(); _resourceViews.ShrinkToFit();
}

#pragma region Protected Function
/****************************************************************************
*						ClearVertexBuffer
****************************************************************************/
/* @fn        void UIRenderer::ClearVertexBuffer(const std::uint32_t frameIndex, const size_t vertexCount)
*
*  @brief     Clear gpu vertex buffer
*
*  @param[in] const std::uint32_t frameIndex
*  @param[in] const size_t vertexCount
*
*  @return 　　void
*****************************************************************************/
void UIRenderer::ClearVertexBuffer(const std::uint32_t frameIndex, const size_t vertexCount)
{
	if (vertexCount == 0) { return; }

	gu::DynamicArray<gm::Vertex> v
	(
		vertexCount,
		Vertex(Float3(0, 0, 0), Float3(0, 0, 0), Float4(1, 1, 1, 1), Float2(0, 0))
	);
	
	_vertexBuffers[frameIndex]->UploadByte(v.Data(), sizeof(Vertex) * vertexCount);
}

/****************************************************************************
*						PrepareMaxImageBuffer
****************************************************************************/
/* @fn        void UIRenderer::PrepareMaxImageBuffer(const gu::tstring& name)
*
*  @brief     Prepare max writable ui count size buffer
*
*  @param[in] gu::tstring& name
*
*  @return 　　void
*****************************************************************************/
void UIRenderer::PrepareMaxImageBuffer(const gu::tstring& name)
{
	const auto device = _engine->GetDevice();

	/*-------------------------------------------------------------------
	-            Create Index List
	---------------------------------------------------------------------*/
	const std::uint32_t rectIndex[]     = { 0,1,3,1,2,3 };
	const std::uint32_t rectIndexCount  = static_cast<std::uint32_t>(_countof(rectIndex));
	const std::uint32_t rectVertexCount = 4;

	gu::DynamicArray<std::uint32_t> indices(_maxWritableUICount * rectIndexCount);
	for (std::uint32_t i = 0; i < _maxWritableUICount; ++i)
	{
		for (std::uint32_t j = 0; j < rectIndexCount; ++j)
		{
			// 次登録するRectには頂点数分ずらす (+4)
			indices[(std::uint64_t)rectIndexCount * i + j] = i * rectVertexCount + rectIndex[j];
		}
	}

	/*-------------------------------------------------------------------
	-            Create Vertex and Index Buffer
	---------------------------------------------------------------------*/
	const auto frameCount = LowLevelGraphicsEngine::FRAME_BUFFER_COUNT;
	
	// prepare frame count buffer
	_vertexBuffers.Resize(frameCount);
	_indexBuffers .Resize(frameCount);

	for (std::uint32_t i = 0; i < frameCount; ++i)
	{
		{
			const auto totalCount = _maxWritableUICount * rectVertexCount;
			const auto vbMetaData = GPUBufferMetaData::VertexBuffer(sizeof(gm::Vertex), totalCount, core::MemoryHeap::Upload);
			
			_vertexBuffers[i] = device->CreateBuffer(vbMetaData);
			_vertexBuffers[i]->SetName(name + SP("VB"));
			ClearVertexBuffer(i, totalCount);
		}

		{
			const auto ibMetaData = GPUBufferMetaData::IndexBuffer(sizeof(std::uint32_t), indices.Size(), MemoryHeap::Default, ResourceState::Common);
			
			_indexBuffers[i] = device->CreateBuffer(ibMetaData);
			_indexBuffers[i]->SetName(name + SP("IB"));
			_indexBuffers[i]->UploadByte(indices.Data(), ibMetaData.GetTotalByte(), 0, _engine->GetCommandList(CommandListType::Copy));
		}
	}
}

/****************************************************************************
*							PreparePipelineState
****************************************************************************/
/* @fn        void Skybox::PreparePipelineState(const gu::tstring& name)
* 
*  @brief     Prepare pipelineState
* 
*  @param[in] gu::tstring& name
* 
*  @return 　　void
*****************************************************************************/
void UIRenderer::PreparePipelineState(const gu::tstring& name)
{
	const auto device  = _engine->GetDevice();
	const auto factory = device->CreatePipelineFactory();
	
	/*-------------------------------------------------------------------
	-             Setup resource layout elements
	---------------------------------------------------------------------*/
	_resourceLayout = device->CreateResourceLayout
	(
		{ ResourceLayoutElement(DescriptorHeapType::SRV, 0) },
		{ SamplerLayoutElement(device->CreateSampler(SamplerInfo::GetDefaultSampler(LinearWrap)),1) }
	);

	/*-------------------------------------------------------------------
	-             Set up graphic pipeline state
	---------------------------------------------------------------------*/
	const auto vs = factory->CreateShaderState();
	const auto ps = factory->CreateShaderState();
	vs->Compile({ ShaderType::Vertex, SP("Shader\\Sprite\\ShaderDefault2D.hlsl"), SP("VSMain"), { SP("Shader\\Core")} });
	ps->Compile({ ShaderType::Pixel , SP("Shader\\Sprite\\ShaderDefault2D.hlsl"), SP("PSMain"), { SP("Shader\\Core") } });

	/*-------------------------------------------------------------------
	-             Set up graphic pipeline state
	---------------------------------------------------------------------*/
	_pipeline = device->CreateGraphicPipelineState(_engine->GetRenderPass(), _resourceLayout);
	_pipeline->SetBlendState(factory->CreateSingleBlendState(core::BlendProperty::AlphaBlend()));
	_pipeline->SetRasterizerState(factory->CreateRasterizerState(RasterizerProperty::Solid()));
	_pipeline->SetInputAssemblyState(factory->CreateInputAssemblyState(GPUInputAssemblyState::GetDefaultVertexElement()));
	_pipeline->SetDepthStencilState(factory->CreateDepthStencilState());
	_pipeline->SetVertexShader(vs);
	_pipeline->SetPixelShader(ps);
	_pipeline->CompleteSetting();
	_pipeline->SetName(name + SP("PSO"));
}
#pragma endregion Protected Function

#pragma region Private Function
void UIRenderer::CountUpDrawImageAndView(const std::uint64_t arrayLength, const ResourceViewPtr& view)
{
	_needCallDrawIndexCount++;
	_totalImageCount += static_cast<std::uint32_t>(arrayLength);
	_imageCountList.Push(static_cast<std::uint32_t>(arrayLength));
	_resourceViews.Push(view);
}
#pragma endregion Private Function