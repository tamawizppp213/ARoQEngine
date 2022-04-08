//////////////////////////////////////////////////////////////////////////////////
///             @file   GraphicsCoreResourceType.hpp
///             @brief  ResourceType
///             @author Toide Yutaro
///             @date   2022_03_23
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GRAPHICS_CORE_RESOURCE_TYPE_HPP
#define GRAPHICS_CORE_RESOURCE_TYPE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
enum class HeapTypeFlag
{
	RTV = 0x0001,
	DSV = 0x0002,
	CBV = 0x0004,
	SRV = 0x0008,
	UAV = 0x0010,
};

enum class TextureType : unsigned int
{
	Unknown          = 0,
	Buffer           = 1,
	Texture1D        = 2,
	Texture1DArray   = 3,
	Texture2D        = 4,
	Texture2DArray   = 5,
	Texture2DMS      = 6,
	Texture2DMSArray = 7,
	Texture3D        = 8,
	TextureCube      = 9,
	TextureCubeArray = 10,
	RayTracing_Acceleration_Structure = 11
};
#endif