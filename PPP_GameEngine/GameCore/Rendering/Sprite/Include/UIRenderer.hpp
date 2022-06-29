//////////////////////////////////////////////////////////////////////////////////
///             @file   UIRenderer.hpp
///             @brief  UIRenderer
///             @author Toide Yutaro
///             @date   2022_06_01
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef UI_RENDERER_HPP
#define UI_RENDERER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Texture.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Buffer.hpp"
#include "Image.hpp"
#include <vector>
#include <string>
#include <memory>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
//struct MeshBuffer;
//class  UploadBuffer;
namespace rhi::directX12
{
	class  CommandContext;
}
struct Texture;

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace ui
{
	/****************************************************************************
	*				  			Sprite
	*************************************************************************//**
	*  @class     Sprite
	*  @brief     2D Sprite
	*****************************************************************************/
	class UIRenderer
	{
		using SceneGPUAddress   = uint64_t;
		using MeshBufferPtr     = std::unique_ptr<MeshBuffer[]>;
		using CommandContextPtr = rhi::directX12::CommandContext*;
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		virtual void StartUp(const std::wstring& addName = L"");
		void AddFrameObject(const std::vector<ui::Image>& images, const Texture& texture);
		void Draw();
		virtual void ShutDown();
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		UIRenderer() = default;
		virtual ~UIRenderer();
		UIRenderer(const UIRenderer&)            = delete;
		UIRenderer& operator=(const UIRenderer&) = delete;
		UIRenderer(UIRenderer&&)                 = default;
		UIRenderer& operator=(UIRenderer&&)      = default;
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		void PrepareRootSignature(const std::wstring& name);
		void PreparePipelineState(const std::wstring& name);
		void PrepareVertexAndIndexBuffer(const std::wstring& name);
		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		MeshBufferPtr        _meshBuffer = nullptr;
		CommandContextPtr    _context    = nullptr;
		std::vector<Texture> _textures;
		int  _currentFrame     = 0;
		int  _imageStackCount  = 0;
		int  _drawCallNum      = 0;
		bool _isFrameStart     = false;
		bool _isFrameEnd       = true;
		std::vector<int> _imageCountList;
	private:
		static constexpr int MAX_WRITABLE_UI_COUNT = 1024;
	};
}
#endif