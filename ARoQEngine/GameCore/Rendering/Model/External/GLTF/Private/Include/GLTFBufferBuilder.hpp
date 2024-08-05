//////////////////////////////////////////////////////////////////////////////////
///             @file   GLTFBufferBuilder.hpp
///             @brief  Buffer Builder
///             @author Copyright (c) Microsoft Corporation. All rights reserved.Licensed under the MIT License.
///                     Partially edit by Toide
///             @date   2022_05_23  
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GLTF_BUFFER_BUILDER_HPP
#define GLTF_BUFFER_BUILDER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Rendering/Model/External/GLTF/Public/Include/GLTFDocument.hpp"
#include "GameCore/Rendering/Model/External/GLTF/Private/Include/GLTFAsset.hpp"
#include <functional>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace gltf
{
	namespace detail
	{
		class ResourceWriter;
		/****************************************************************************
		*				  			AccessorDesc
		****************************************************************************/
		/* @class     AccessorDesc
		*  @brief     Accessor Desc
		*****************************************************************************/
		class AccessorDesc
		{
		public:
			/****************************************************************************
			**                Public Function
			*****************************************************************************/
			bool IsValid() const { return AccessorDataType != asset::AccessorType::Type_Unknown && ComponentDataType != asset::ComponentType::Component_Unknown; }
			/****************************************************************************
			**                Public Property
			*****************************************************************************/
			asset::AccessorType AccessorDataType;
			asset::ComponentType ComponentDataType;
			bool   Normalized;
			size_t ByteOffset;
			std::vector<float> MinValues;
			std::vector<float> MaxValues;
			/****************************************************************************
			**                Constructor and Destructor
			*****************************************************************************/
			AccessorDesc() = default;
			AccessorDesc(asset::AccessorType accessorType, asset::ComponentType componentType, bool normalized = false, std::vector<float> minValues = {}, std::vector<float> maxValues = {}, size_t byteOffset = 0)
				: AccessorDataType(accessorType), ComponentDataType(componentType), Normalized(normalized), ByteOffset(byteOffset), MinValues(std::move(minValues)), MaxValues(std::move(maxValues))
			{

			}

		private:
			/****************************************************************************
			**                Private Function
			*****************************************************************************/

			/****************************************************************************
			**                Private Property
			*****************************************************************************/

		};

		/****************************************************************************
		*				  			BufferBuilder
		****************************************************************************/
		/* @class     BufferBuilder
		*  @brief     Buffer Builder
		*****************************************************************************/
		class BufferBuilder final
		{
			using FunctionGeneratorID = std::function<std::string(const BufferBuilder&)>;
		public:
			/****************************************************************************
			**                Public Function
			*****************************************************************************/
			const asset::GLTFBuffer&     AddBuffer(const char* bufferID = nullptr);
			const asset::GLTFBufferView& AddBufferView(Optional<asset::BufferViewTarget> target = {});
			const asset::GLTFBufferView& AddBufferView(const void* data, size_t byteLength, Optional<size_t> byteStride = {}, Optional<asset::BufferViewTarget> target = {});
			template<typename T>
			const asset::GLTFBufferView& AddBufferView(const std::vector<T>& data, Optional<size_t> byteStride = {}, Optional<asset::BufferViewTarget> target = {})
			{
				return AddBufferView(data.data(), data.size() * sizeof(T), byteStride, target);
			}

			const asset::GLTFAccessor& AddAccessor(const void* data, size_t count, AccessorDesc accessorDesc);
			template<typename T>
			const asset::GLTFAccessor& AddAccessor(const std::vector<T>& data, AccessorDesc accessorDesc)
			{
				const auto accessorTypeSize = asset::GLTFAccessor::GetTypeCount(accessorDesc.AccessorDataType);

				if (data.size() & accessorTypeSize)
				{
					throw error::InvalidGLTFException("vector size is not a multiple if accessor type size");
				}

				return AddAccessor(data.data(), data.size() / accessorTypeSize, std::move(accessorDesc));
			}
			void AddAccessors(const void* data, size_t count, size_t byteStride, const AccessorDesc* desc, size_t descCount, std::string* outputIDs = nullptr);

			void Output(GLTFDocument& document);
			/****************************************************************************
			**                Public Property
			*****************************************************************************/
			const asset::GLTFBuffer    & GetCurrentBuffer    () const;
			const asset::GLTFBufferView& GetCurrentBufferView() const;
			const asset::GLTFAccessor  & GetCurrentAccessor  () const;

			size_t GetBufferCount    () const;
			size_t GetBufferViewCount() const;
			size_t GetAccessorCount  () const;

			ResourceWriter& GetResourceWriter();
			const ResourceWriter& GetResourceWriter() const;
			/****************************************************************************
			**                Constructor and Destructor
			*****************************************************************************/
			BufferBuilder(std::unique_ptr<ResourceWriter>&& resourceWriter);
			BufferBuilder(std::unique_ptr<ResourceWriter>&& resourceWriter,
				FunctionGeneratorID functionGeneratorBufferID,
				FunctionGeneratorID functionGeneratorBufferViewID,
				FunctionGeneratorID functionGeneratorAccessorID);

		private:
			/****************************************************************************
			**                Private Function
			*****************************************************************************/

			/****************************************************************************
			**                Private Property
			*****************************************************************************/
			const asset::GLTFAccessor& AddAccessor(size_t count, AccessorDesc desc);

			std::unique_ptr<ResourceWriter> _resourceWriter;

			IndexedContainer<asset::GLTFBuffer>     _buffers;
			IndexedContainer<asset::GLTFBufferView> _bufferViews;
			IndexedContainer<asset::GLTFAccessor>   _accessors;

			FunctionGeneratorID _functionGeneratorBufferID;
			FunctionGeneratorID _functionGeneratorBufferViewID;
			FunctionGeneratorID _functionGeneratorAccessorID;
		};
	}
}
#endif