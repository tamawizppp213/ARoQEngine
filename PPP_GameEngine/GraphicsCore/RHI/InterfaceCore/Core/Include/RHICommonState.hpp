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

#pragma region Blend State
	/****************************************************************************
	*				  			BlendFactor
	*************************************************************************//**
	*  @class     BlendFactor
	*  @brief     Color blend factor (directX12 based)
	*****************************************************************************/
	enum class BlendFactor : std::uint32_t
	{
		Zero = 1,
		One,
		Source_Color,
		Inverse_Source_Color,
		Source_Alpha,
		Inverse_Source_Alpha,
		Dest_Alpha,
		Inverse_Dest_Alpha,
		Dest_Color,
		Inverse_Dest_Color,
		Source_Alpha_Saturate,
	};
	/****************************************************************************
	*				  			BlendOperator
	*************************************************************************//**
	*  @class     BlendOperator
	*  @brief     Color blend calculate opration (directX12 based)
	*****************************************************************************/
	enum class BlendOperator : std::uint32_t
	{
		Add = 1,
		Subtract = 2,
		Reverse_Subtract = 3,
		Min = 4,
		Max = 5
	};
	/****************************************************************************
	*				  			ColorMask
	*************************************************************************//**
	*  @class     ColorMask 
	*  @brief     Color mask bit flag
	*****************************************************************************/
	enum class ColorMask : std::uint8_t
	{
		None  = 0,
		Red   = 0x1,
		Green = 0x2,
		Blue  = 0x4,
		Alpha = 0x8,
		All   = Red | Green | Blue | Alpha
	};
	inline ColorMask operator | (const ColorMask& left, const ColorMask& right)
	{
		return static_cast<ColorMask>( static_cast<std::uint32_t>(left) | static_cast<std::uint32_t>(right));
	}
	/****************************************************************************
	*				  			BlendProperty
	*************************************************************************//**
	*  @class     BlendProperty
	*  @brief     Property
	*****************************************************************************/
	struct BlendProperty
	{
		BlendOperator ColorOperator    = BlendOperator::Add;
		BlendOperator AlphaOperator    = BlendOperator::Add;
		BlendFactor   DestinationAlpha = BlendFactor::Zero;
		BlendFactor   Destination      = BlendFactor::Zero;
		BlendFactor   SourceAlpha      = BlendFactor::One;
		BlendFactor   Source           = BlendFactor::One;
		ColorMask     ColorMask        = ColorMask::All;

		bool Enable = false;
		BlendProperty() = default;
	};
#pragma endregion Blend State
	enum class TextureAddressingMode : std::uint8_t
	{
		Wrap     = 1,
		Mirror   = 2,
		Clamp    = 3,
		Boarder  = 4,
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
		Unknown,
		R8G8B8A8_UNORM,
		B8G8R8A8_UNORM,
		R10G10B10A2_UNORM,
		R32G32B32A32_FLOAT,
		R16G16B16A16_FLOAT,
		D32_FLOAT,
		D24_UNORM_S8_UINT,
		R32G32B32_FLOAT,
		CountOfPixelFormat
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
		Viewport() = default;
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
		ScissorRect() = default;
		ScissorRect(long left, long top, long right, long bottom)
		{
			this->Left = left; this->Top = top; this->Right = right; this->Bottom = bottom;
		}
	};
	/****************************************************************************
	*				  			WindowInfo
	*************************************************************************//**
	*  @class     WindowInfo
	*  @brief     Window
	*****************************************************************************/
	struct WindowInfo
	{
		size_t Width  = 0;
		size_t Height = 0;
		void*  Handle    = nullptr;
		void*  HInstance = nullptr; // windows—p
		WindowInfo()  = default;
		WindowInfo(size_t width, size_t height, void* handle, void* hInstance = nullptr)
		{
			this->Width = width; this->Height = height; this->Handle = handle; this->HInstance = hInstance;
		}
	};
}
#endif