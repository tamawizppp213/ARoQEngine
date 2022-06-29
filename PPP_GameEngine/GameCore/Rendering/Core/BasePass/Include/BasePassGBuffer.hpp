//////////////////////////////////////////////////////////////////////////////////
///             @file   BasePassGBuffer.hpp
///             @brief  BasePassGBuffer (ue4 like render pipeline) reference HLSL シェーダの魔導書
///             @author Toide Yutaro
///             @date   2022_05_04 
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef BASE_PASS_GBUFFER_HPP
#define BASE_PASS_GBUFFER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Buffer.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace basepass
{
	enum class GBufferType
	{
		Albedo,
		CountOfGBufferType,
	};
	/****************************************************************************
	*				  			GBuffer
	*************************************************************************//**
	*  @class     GBuffer
	*  @brief     GBuffer
	*****************************************************************************/
	class GBuffer
	{
		using RenderTarget = ColorBuffer;
		using SceneGPUAddress = UINT64;
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		void Initialize(int screenWidth, int screenHeight);
		void OnResize(int newWidth, int newHeight);
		void Draw(SceneGPUAddress scene);
		void Finalize();
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		const ColorBuffer& GetGBuffer(GBufferType type) const { return _renderTargets[(int)type]; }
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		GBuffer();
		~GBuffer();
		GBuffer(const GBuffer&)            = delete;
		GBuffer& operator=(const GBuffer&) = delete;
		GBuffer(GBuffer&&)                 = default;
		GBuffer& operator=(GBuffer&&)      = default;
	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/

		/****************************************************************************
		**                Private Member Variables
		*****************************************************************************/
		RenderTarget _renderTargets[(int)GBufferType::CountOfGBufferType];
	};
}
#endif