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
#include "../../Interface/Include/GBuffer.hpp"


//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace gc::basepass
{
	/****************************************************************************
	*				  			GBuffer
	****************************************************************************/
	/* @class     GBuffer
	*  @brief     GBuffer (普段はDefaultTypeを使用するが, カスタマイズも可能にしている.)
	*****************************************************************************/
	class GBuffer : public gc::rendering::GBuffer
	{
	public:
		enum BufferType
		{
			Albedo,
			Normal,
			CountOf
		};
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		void OnResize(const std::uint32_t width, const std::uint32_t height) override;

		void Draw(const GPUResourceViewPtr& scene) override;

		/****************************************************************************
		**                Public Property
		*****************************************************************************/

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		GBuffer() = default;
		
		GBuffer(const LowLevelGraphicsEnginePtr& engine,
			const gc::rendering::GBufferDesc& desc = gc::rendering::GBufferDesc((std::uint64_t)BufferType::CountOf),
			const gu::tstring& addName = SP(""));

		~GBuffer();
		
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		void PreparePipelineState(const gu::tstring& name) override;

		void PrepareFrameBuffers(const gu::tstring& name) override;

		/****************************************************************************
		**                Protected Property
		*****************************************************************************/
		
	};
}
#endif