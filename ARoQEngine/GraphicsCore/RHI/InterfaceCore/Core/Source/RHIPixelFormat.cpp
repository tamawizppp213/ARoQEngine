//////////////////////////////////////////////////////////////////////////////////
///  @file   RHIPixelFormat.cpp
///  @brief  �摜�f�[�^���������߂̃N���X�ł�. 
///  @author toide
///  @date   2024/04/27 17:03:20
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIPixelFormat.hpp"
#include "GameUtility/Base/Include/GUAssert.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi::core;
using namespace gu;

namespace
{
	PixelFormatInfo g_PixelFormats[(gu::uint8)PixelFormat::CountOfPixelFormat] =
	{
		//              PixelFormat                      BlockSizeX, BlockSizeY, BlockSizeZ, BlockBytes, ChannelCount, Supported 
		PixelFormatInfo(PixelFormat::Unknown             ,          0,          0,          0,          0,            0, false),
		PixelFormatInfo(PixelFormat::R32G32B32A32_FLOAT  ,          1,          1,          1,          16,           4, true ),
		PixelFormatInfo(PixelFormat::R32G32B32A32_UINT   ,          1,          1,          1,          16,           4, true),
		PixelFormatInfo(PixelFormat::R32G32B32A32_SINT   ,          1,          1,          1,          16,           4, true),
		PixelFormatInfo(PixelFormat::R16G16B16A16_FLOAT  ,          1,          1,          1,           8,           4, true),
		PixelFormatInfo(PixelFormat::R16G16B16A16_UINT   ,          1,          1,          1,           8,           4, true),
		PixelFormatInfo(PixelFormat::R16G16B16A16_SINT   ,          1,          1,          1,           8,           4, true),
		PixelFormatInfo(PixelFormat::R16G16B16A16_UNORM  ,          1,          1,          1,           8,           4, true),
		PixelFormatInfo(PixelFormat::R16G16B16A16_SNORM  ,          1,          1,          1,           8,           4, true),
		PixelFormatInfo(PixelFormat::R8G8B8A8_UINT       ,          1,          1,          1,           4,           4, true),
		PixelFormatInfo(PixelFormat::R8G8B8A8_SINT       ,          1,          1,          1,           4,           4, true),
		PixelFormatInfo(PixelFormat::R8G8B8A8_UNORM      ,          1,          1,          1,           4,           4, true),
		PixelFormatInfo(PixelFormat::R8G8B8A8_SNORM      ,          1,          1,          1,           4,           4, true),
		PixelFormatInfo(PixelFormat::R8G8B8A8_UNORM_SRGB ,          1,          1,          1,           4,           4, true),
		PixelFormatInfo(PixelFormat::B8G8R8A8_UNORM      ,          1,          1,          1,           4,           4, true),
		PixelFormatInfo(PixelFormat::B8G8R8A8_UNORM_SRGB ,          1,          1,          1,           4,           4, true),
		PixelFormatInfo(PixelFormat::R32G32B32_FLOAT     ,          1,          1,          1,           12,          3, true),
		PixelFormatInfo(PixelFormat::R32G32B32_UINT      ,          1,          1,          1,           12,          3, true),
		PixelFormatInfo(PixelFormat::R32G32B32_SINT      ,          1,          1,          1,           12,          3, true),
		PixelFormatInfo(PixelFormat::R32G32_FLOAT        ,          1,          1,          1,            8,          2, true),
		PixelFormatInfo(PixelFormat::R32G32_UINT         ,          1,          1,          1,            8,          2, true),
		PixelFormatInfo(PixelFormat::R32G32_SINT         ,          1,          1,          1,            8,          2, true),
		PixelFormatInfo(PixelFormat::R16G16_FLOAT        ,          1,          1,          1,            4,          2, true),
		PixelFormatInfo(PixelFormat::R16G16_UINT         ,          1,          1,          1,            4,          2, true),
		PixelFormatInfo(PixelFormat::R16G16_SINT         ,          1,          1,          1,            4,          2, true),
		PixelFormatInfo(PixelFormat::R8G8_UINT           ,          1,          1,          1,            2,          2, true),
		PixelFormatInfo(PixelFormat::R8G8_SINT           ,          1,          1,          1,            2,          2, true),
		PixelFormatInfo(PixelFormat::R8G8_UNORM          ,          1,          1,          1,            2,          2, true),
		PixelFormatInfo(PixelFormat::R8G8_SNORM          ,          1,          1,          1,            2,          2, true),
		PixelFormatInfo(PixelFormat::D32_FLOAT           ,          1,          1,          1,            4,          1, true),
		PixelFormatInfo(PixelFormat::R32_FLOAT           ,          1,          1,          1,            4,          1, true),
		PixelFormatInfo(PixelFormat::R32_UINT            ,          1,          1,          1,            4,          1, true),
		PixelFormatInfo(PixelFormat::R32_SINT            ,          1,          1,          1,            4,          1, true),
		PixelFormatInfo(PixelFormat::D16_UNORM           ,          1,          1,          1,            2,          1, true),
		PixelFormatInfo(PixelFormat::R16_FLOAT           ,          1,          1,          1,            2,          1, true),
		PixelFormatInfo(PixelFormat::R16_UINT            ,          1,          1,          1,            2,          1, true),
		PixelFormatInfo(PixelFormat::R16_SINT            ,          1,          1,          1,            2,          1, true),
		PixelFormatInfo(PixelFormat::R16_UNORM           ,          1,          1,          1,            2,          1, true),
		PixelFormatInfo(PixelFormat::R16_SNORM           ,          1,          1,          1,            2,          1, true),
		PixelFormatInfo(PixelFormat::R8_UINT             ,          1,          1,          1,            1,          1, true),
		PixelFormatInfo(PixelFormat::R8_SINT             ,          1,          1,          1,            1,          1, true),
		PixelFormatInfo(PixelFormat::R8_UNORM            ,          1,          1,          1,            1,          1, true),
		PixelFormatInfo(PixelFormat::R8_SNORM            ,          1,          1,          1,            1,          1, true),
		PixelFormatInfo(PixelFormat::A8_UNORM            ,          1,          1,          1,            1,          1, true),
		PixelFormatInfo(PixelFormat::R1_UNORM            ,          1,          1,          1,            1,          1, false),
		PixelFormatInfo(PixelFormat::D32_FLOAT_S8X24_UINT,          1,          1,          1,            8,          2, true),
		PixelFormatInfo(PixelFormat::D24_UNORM_S8_UINT   ,          1,          1,          1,            4,          2, true),
		PixelFormatInfo(PixelFormat::R10G10B10A2_UNORM   ,          1,          1,          1,            4,          4, true),
		PixelFormatInfo(PixelFormat::R10G10B10A2_UINT    ,          1,          1,          1,            4,          4, true),
		PixelFormatInfo(PixelFormat::BC1_UNORM           ,          4,          4,          1,            8,          3, true),
		PixelFormatInfo(PixelFormat::BC1_SRGB            ,          4,          4,          1,            8,          3, true),
		PixelFormatInfo(PixelFormat::BC2_UNORM           ,          4,          4,          1,            16,          4, true),
		PixelFormatInfo(PixelFormat::BC2_SRGB            ,          4,          4,          1,            16,          4, true),
		PixelFormatInfo(PixelFormat::BC3_UNORM           ,          4,          4,          1,            16,          4, true),
		PixelFormatInfo(PixelFormat::BC3_SRGB            ,          4,          4,          1,            16,          4, true),
		PixelFormatInfo(PixelFormat::BC4_UNORM           ,          4,          4,          1,             8,          1, true),
		PixelFormatInfo(PixelFormat::BC4_SNORM           ,          4,          4,          1,             8,          1, true),
		PixelFormatInfo(PixelFormat::BC5_UNORM           ,          4,          4,          1,            16,          2, true),
		PixelFormatInfo(PixelFormat::BC5_SNORM           ,          4,          4,          1,            16,          2, true),
		PixelFormatInfo(PixelFormat::BC7_UNORM           ,          4,          4,          1,            16,          4, true),
		PixelFormatInfo(PixelFormat::BC7_UNORM_SRGB      ,          4,          4,          1,            16,          4, true),
		PixelFormatInfo(PixelFormat::BC6H_UFLOAT16       ,          4,          4,          1,            16,          3, true),
		PixelFormatInfo(PixelFormat::BC6H_SFLOAT16       ,          4,          4,          1,            16,          3, true),
	};
}
//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor

#pragma endregion Constructor and Destructor

#pragma region Static Function
/*!**********************************************************************
*  @brief    �s�N�Z���t�H�[�}�b�g�����擾
*  @return   �s�N�Z���t�H�[�}�b�g : Global�ȃt�H�[�}�b�g����擾���邽��, �l��ς���ꍇ�͐T�d�ɍs���Ă�������. 
*************************************************************************/
PixelFormatInfo& PixelFormatInfo::Get(const PixelFormat format)
{
	const gu::uint32 index = static_cast<gu::uint32>(format);

	if (index >= (gu::uint32)PixelFormat::CountOfPixelFormat) 
	{ 
		Confirm(false);
		return g_PixelFormats[0]; 
	};

	return g_PixelFormats[index];
}

/*!**********************************************************************
*  @brief    �s�N�Z���t�H�[�}�b�g�����擾
*  @return   �s�N�Z���t�H�[�}�b�g : Global�ȃt�H�[�}�b�g����擾���邽��, �l��ς���ꍇ�͐T�d�ɍs���Ă�������.
*************************************************************************/
const PixelFormatInfo& PixelFormatInfo::GetConst(const PixelFormat format)
{
	const gu::uint32 index = static_cast<gu::uint32>(format);

	return index < (gu::uint32)PixelFormat::CountOfPixelFormat ? g_PixelFormats[index] : g_PixelFormats[0];
}

#pragma endregion Static Function
#pragma region Public Function

#pragma region Format Check
/****************************************************************************
*                    IsInteger
*************************************************************************//**
*  @brief     �����t�H�[�}�b�g��
*
*  @param[in] void
*
*  @return    �����t�H�[�}�b�gUnsigned int��Signed int�ł����true
*****************************************************************************/
bool PixelFormatInfo::IsInteger() const
{
	using enum PixelFormat;

	switch (Format)
	{
		case R32G32B32A32_UINT:
		case R32G32B32A32_SINT:
		case R16G16B16A16_UINT:
		case R16G16B16A16_SINT:
		case R8G8B8A8_UINT:
		case R8G8B8A8_SINT:
		case R32G32B32_UINT:
		case R32G32B32_SINT:
		case R32G32_UINT:
		case R32G32_SINT:
		case R16G16_UINT:
		case R16G16_SINT:
		case R8G8_UINT:
		case R8G8_SINT:
		case R32_UINT:
		case R32_SINT:
		case R16_UINT:
		case R16_SINT:
		case R8_UINT:
		case R8_SINT:
		{
			return true;
		}
	}

	return false;
}

/****************************************************************************
*                    IsFloat
*************************************************************************//**
*  @brief     Float�t�H�[�}�b�g��
*
*  @param[in] void
*
*  @return    Float�t�H�[�}�b�g�ł���ꍇ��true
*****************************************************************************/
bool PixelFormatInfo::IsFloat() const
{
	using enum PixelFormat;
	
	switch (Format)
	{
		case R32G32B32A32_FLOAT:
		case R16G16B16A16_FLOAT:
		case R32G32B32_FLOAT:
		case R32G32_FLOAT:
		case R16G16_FLOAT:
		case D32_FLOAT:
		case R32_FLOAT:
		case R16_FLOAT:
		{
			return true;
		}
	}

	return false;
}

/****************************************************************************
*                    IsHDR
*************************************************************************//**
*  @brief     HDR�Ŏg�p�����t�H�[�}�b�g��
*
*  @param[in] void
*
*  @return    HDR�Ŏg�p�ł���t�H�[�}�b�g (1.0�ȏ�̒l���i�[�o����t�H�[�}�b�g)�ł����true
*****************************************************************************/
bool PixelFormatInfo::IsHDR() const
{
	using enum PixelFormat;
	return Format == R32G32B32A32_FLOAT || Format == R16G16B16A16_FLOAT || Format == BC6H_SFLOAT16 || 
		   Format == BC6H_UFLOAT16      || Format == R16_FLOAT          || Format == R32_FLOAT;
}

/****************************************************************************
*                    IsDepthStencil
*************************************************************************//**
*  @brief     Depth��Stencil�Ŏg�p����Ă���t�H�[�}�b�g��
*
*  @param[in] void
*
*  @return    DepthStencil�Ŏg�p�ł���t�H�[�}�b�g�̏ꍇtrue
*****************************************************************************/
bool PixelFormatInfo::IsDepthStencil() const
{
	using enum PixelFormat;

	switch (Format)
	{
		case D32_FLOAT:
		case D32_FLOAT_S8X24_UINT:
		case D24_UNORM_S8_UINT:
		case D16_UNORM:
		{
			return true;
		}
	}
	return false;
}

/****************************************************************************
*                    IsStencil
*************************************************************************//**
*  @brief     Stencil�Ŏg�p����Ă���t�H�[�}�b�g��
*
*  @param[in] void
*
*  @return    Stencil�Ŏg�p�ł��� (uint8�̂悤�Ȃ���)�ꍇtrue
*****************************************************************************/
bool PixelFormatInfo::IsStencil() const
{
	using enum PixelFormat;

	switch (Format)
	{
		case D24_UNORM_S8_UINT:
		case D32_FLOAT_S8X24_UINT:
		{
			return true;
		}
	}
	
	return false;
}

/****************************************************************************
*                    IsBlockCompressed
*************************************************************************//**
*  @brief     �u���b�N���k�̃t�H�[�}�b�g�ł��邩�ǂ���
*
*  @param[in] void
*
*  @return    �e�N�X�`���̃s�N�Z�����u���b�N���Ƃɕ������Ĉ��k����t�H�[�}�b�g�̏ꍇtrue
*****************************************************************************/
bool PixelFormatInfo::IsBlockCompressed() const
{
	using enum PixelFormat;

	switch (Format)
	{
		case BC1_UNORM:
		case BC1_SRGB:
		case BC2_UNORM:
		case BC2_SRGB:
		case BC3_UNORM:
		case BC3_SRGB:
		case BC4_UNORM:
		case BC4_SNORM:
		case BC5_UNORM:
		case BC5_SNORM:
		case BC6H_UFLOAT16:
		case BC6H_SFLOAT16:
		case BC7_UNORM:
		case BC7_UNORM_SRGB: 
		{
			return true;
		}

		return false;
	}
}

#pragma endregion Format Check
#pragma endregion Public Function