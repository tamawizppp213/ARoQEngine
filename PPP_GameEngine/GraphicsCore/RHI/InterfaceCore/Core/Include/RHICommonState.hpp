//////////////////////////////////////////////////////////////////////////////////
///             @file   RHICommonState.hpp
///             @brief  CommonState
///             @author Toide Yutaro
///             @date   2022_06_12
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef RHI_COMMON_STATE_HPP
#define RHI_COMMON_STATE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include <cstdint>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::core
{
	enum class APIVersion : std::uint8_t
	{
		Unknown    = 0,
		DirectX12  = 1,
		Vulkan     = 2
	};

	enum class TextureAddressingMode : std::uint8_t
	{
		Wrap     = 0,
		Mirror   = 1,
		Clamp    = 2,
		Boarder  = 3,
	};

	enum class PrimitiveTopology : std::uint8_t
	{
		Undefined     = 0,
		PointList     = 1,
		LineList      = 2,
		LineStrip     = 3,
		TriangleList  = 4,
		TriangleStrip = 5,
		CountOfPrimitiveTopology
	};

	enum class CullingMode : std::uint8_t
	{
		None,
		Front,
		Back
	};

	enum class PixelFormat
	{

	};

	/****************************************************************************
	*				  			ClearValue
	*************************************************************************//**
	*  @class     ClearValue
	*  @brief     Clear value
	*****************************************************************************/
	struct ClearValue
	{
		struct DepthClearValue
		{
			float Depth = 1.0f;
			std::uint8_t Stencil = 0;
		};
		union
		{
			float Color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
			DepthClearValue DepthValue;
		};

		ClearValue() = default;
		ClearValue(const float* color) { for (int i = 0; i < 4; ++i) { Color[i] = color[i]; } }
		ClearValue(float depth, std::uint8_t stencil)
		{
			DepthValue.Depth = depth; DepthValue.Stencil = stencil;
		}
	};
	/****************************************************************************
	*				  			Viewport 
	*************************************************************************//**
	*  @class     Viewport 
	*  @brief     Vireport
	*****************************************************************************/
	struct Viewport
	{
		float TopLeftX  = 0.0f;
		float TopLeftY  = 0.0f;
		float Width     = 0.0f;
		float Height    = 0.0f;
		float MinDepth  = 0.0f;
		float MaxDepth  = 1.0f;
		Viewport(float topLeftX, float topLeftY, float width, float height, float minDepth = 0.0f, float maxDepth = 1.0f)
		{
			this->TopLeftX = topLeftX; this->TopLeftY = topLeftY; this->Width = width; this->Height = height; this->MinDepth = minDepth; this->MaxDepth = maxDepth;
		}
	};
	/****************************************************************************
	*				  			ScissorRect
	*************************************************************************//**
	*  @class     ScissorRect
	*  @brief     Scissor Rectangle
	*****************************************************************************/
	struct ScissorRect
	{
		long Left   = 0;
		long Top    = 0;
		long Right  = 0;
		long Bottom = 0;
		ScissorRect(long left, long top, long right, long bottom)
		{
			this->Left = left; this->Top = top; this->Right = right; this->Bottom = bottom;
		}
	};

}
#endif