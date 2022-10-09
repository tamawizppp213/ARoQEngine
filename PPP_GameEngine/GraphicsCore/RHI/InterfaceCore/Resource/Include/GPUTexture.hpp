//////////////////////////////////////////////////////////////////////////////////
///             @file   RHIFrameBuffer.hpp
///             @brief  Frame Buffer (Render Target and )
///             @author Toide Yutaro
///             @date   2022_07_19
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GPU_TEXTURE_HPP
#define GPU_TEXTURE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GPUResource.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////

namespace rhi::core
{
	class RHIDevice;
	/****************************************************************************
	*				  			TemplateClass
	*************************************************************************//**
	*  @class     TemplateClass
	*  @brief     temp
	*****************************************************************************/
	class GPUTexture : public GPUResource
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		/* @ brief: texture width, mipLevel (mipmap half size)*/
		size_t GetWidth   (const size_t mipLevel = 0) const noexcept;
		/* @brief: texture height, mipLevel*/
		size_t GetHeight  (const size_t mipLevel = 0) const noexcept;
		/* @brief: texture depth*/
		size_t GetDepth   (const size_t mipLevel = 0) const noexcept;
		/* @brief : Get texture byte size*/
		size_t GetByteSize(const size_t mipLevel = 0) const noexcept;
		inline ResourceType GetResourceType() const noexcept { return _metaData.ResourceType; }
		/* @brief : Get texture pixel format */
		inline PixelFormat       GetPixelFormat() const noexcept { return _metaData.PixelFormat; }
		/* @brief : Get multi sample count (normally Sample 1)*/
		inline MultiSample       GetMultiSample() const noexcept { return _metaData.Sample; }
		/* @brief : Get texture dimension (1D Å` 3D)*/
		inline ResourceDimension GetDimension  () const noexcept { return _metaData.Dimension; }
		/* @brief : Get resource usage*/
		inline ResourceUsage GetUsage() const noexcept { return _metaData.ResourceUsage; }
		/* @brief : Get Clear Value (normally color : white)*/
		inline ClearValue        GetClearValue () const noexcept { return _metaData.ClearColor; }
		/* @brief : Get miplevels count*/
		inline size_t GetMipMapLevels() const noexcept { return _metaData.MipLevels; }
		/* @brief : Get Arrays*/
		inline size_t GetArrayLength() const noexcept { return _metaData.Dimension == ResourceDimension::Dimension3D ? _metaData.DepthOrArraySize : 1; }
		/* @brief : Is Array*/
		inline bool IsArray() const noexcept { return GetArrayLength() != 1; }
		/* @brief : texture pixel index*/
		inline bool GetIndex(const size_t mipSlices = 0, const size_t arraySlice = 0) const noexcept { return arraySlice * GetMipMapLevels() + mipSlices; }
		/* @brief : Get physical texture size*/
		inline size_t GetPhysicalSize() const noexcept { return _physicalSize; }
		/* @brief : Get alignment*/
		inline size_t GetAlignment() const noexcept { return _alignment; }

		GPUTextureMetaData&      GetMetaData()                { return _metaData; }
		const GPUTextureMetaData GetMetaData() const noexcept { return _metaData; }
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		
	protected:
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		GPUTexture() = default;
		~GPUTexture() = default;
		explicit GPUTexture(const std::shared_ptr<RHIDevice>& device, const GPUTextureMetaData& metaData): core::GPUResource(device), _metaData(metaData) {};
		
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		virtual void CreateTextureBuffer(){};
		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		GPUTextureMetaData _metaData;
		size_t _physicalSize = 0;
		size_t _alignment = 0;

	};
}
#endif