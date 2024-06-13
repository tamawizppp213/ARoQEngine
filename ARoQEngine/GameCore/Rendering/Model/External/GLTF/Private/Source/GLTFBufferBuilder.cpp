//////////////////////////////////////////////////////////////////////////////////
//              @file   GLTFBufferBuilder.cpp
///             @brief  GLTF Buffer Builder
///             @author Copyright (c) Microsoft Corporation. All rights reserved.
///                     Licensed under the MIT License. 
///             @date   2022_05_23
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Rendering/Model/External/GLTF/Private/Include/GLTFBufferBuilder.hpp"
#include "GameCore/Rendering/Model/External/GLTF/Private/Include/ResourceWriter.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gltf;
using namespace gltf::detail;
using namespace gltf::detail::asset;
//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
namespace
{
    size_t GetPadding(size_t offset, size_t alignment)
    {
        const auto padAlign = offset % alignment;
        const auto pad      = padAlign ? alignment - padAlign : 0U;
        return pad;
    }

    size_t GetPadding(size_t offset, ComponentType componentType)
    {
        return GetPadding(offset, asset::GLTFAccessor::GetComponentTypeSize(componentType));
    }


    size_t GetAlignment(const AccessorDesc& desc)
    {
        return asset::GLTFAccessor::GetComponentTypeSize(desc.ComponentDataType);
    }
}

BufferBuilder::BufferBuilder(std::unique_ptr<ResourceWriter>&& resourceWriter) : BufferBuilder(std::move(resourceWriter), {}, {}, {})
{

}

BufferBuilder::BufferBuilder(std::unique_ptr<ResourceWriter>&& resourceWriter,
    FunctionGeneratorID functionGeneratorBufferID,
    FunctionGeneratorID functionGeneratorBufferViewID,
    FunctionGeneratorID functionGeneratorAccessorID) : _resourceWriter(std::move(resourceWriter)),
    _functionGeneratorBufferID    (std::move(functionGeneratorBufferID)),
    _functionGeneratorBufferViewID(std::move(functionGeneratorBufferViewID)),
    _functionGeneratorAccessorID  (std::move(functionGeneratorAccessorID)){}
/****************************************************************************
*                       AddBuffer
****************************************************************************/
/* @fn        const asset::GLTFBuffer& BufferBuilder::AddBuffer(const char* bufferID)
*  @brief     Add buffer
*  @param[in] const char* bufferID Name
*  @return 　　const asset::GLTFBuffer&
*****************************************************************************/
const asset::GLTFBuffer& BufferBuilder::AddBuffer(const char* bufferID)
{
    GLTFBuffer buffer;

    if (bufferID) { buffer.ID = bufferID; }
    else if (_functionGeneratorBufferID) { buffer.ID = _functionGeneratorBufferID(*this); }

    buffer.ByteLength = 0U;
    auto& bufferRef   = _buffers.Append(std::move(buffer), AppendIdPolicy::GenerateOnEmpty);
    bufferRef.URI     = _resourceWriter->GenerateBufferURI(bufferRef.ID);
    return bufferRef;
}
/****************************************************************************
*                       AddBufferView
****************************************************************************/
/* @fn        const asset::GLTFBufferView& BufferBuilder::AddBufferView(Optional<asset::BufferViewTarget> target)
*  @brief     Add buffer view
*  @param[in] Optional<asset::BufferViewTarget> target
*  @return 　　const asset::GLTFBufferView&
*****************************************************************************/
const asset::GLTFBufferView& BufferBuilder::AddBufferView(Optional<asset::BufferViewTarget> target)
{
    GLTFBuffer&    buffer = _buffers.Back();
    GLTFBufferView bufferView;

    if (_functionGeneratorBufferViewID)
    {
        bufferView.ID = _functionGeneratorBufferViewID(*this);
    }

    bufferView.BufferID   = buffer.ID;
    bufferView.ByteOffset = buffer.ByteLength;
    bufferView.ByteLength = 0U;
    bufferView.Target     = target;
    
    return _bufferViews.Append(std::move(bufferView), AppendIdPolicy::GenerateOnEmpty);
}
/****************************************************************************
*                       AddBufferView
****************************************************************************/
/* @fn        const asset::GLTFBufferView& BufferBuilder::AddBufferView(const void* data, size_t byteLength, Optional<size_t> byteStride, Optional<asset::BufferViewTarget> target)
*  @brief     Add buffer view
*  @param[in] const void* data
*  @param[in] size_t byteLength[
*  @param[in] size_t Optional<size_t> byteStride
*  @param[in] Optional<asset::BufferViewTarget> target
*  @return 　　const asset::GLTFBufferView&
*****************************************************************************/
const asset::GLTFBufferView& BufferBuilder::AddBufferView(const void* data, size_t byteLength, Optional<size_t> byteStride, Optional<asset::BufferViewTarget> target)
{
    GLTFBuffer& buffer = _buffers.Back();
    GLTFBufferView bufferView;

    if (_functionGeneratorBufferViewID)
    {
        bufferView.ID = _functionGeneratorBufferViewID(*this);
    }

    bufferView.BufferID = buffer.ID;
    bufferView.ByteOffset = buffer.ByteLength;
    bufferView.ByteLength = byteLength;
    bufferView.ByteStride = byteStride;
    bufferView.Target     = target;
    
    buffer.ByteLength = bufferView.ByteOffset + bufferView.ByteLength;

    if (_resourceWriter)
    {
        _resourceWriter->Write(bufferView, data);
    }
    return _bufferViews.Append(std::move(bufferView), AppendIdPolicy::GenerateOnEmpty);
}
/****************************************************************************
*                       AddAccessor
****************************************************************************/
/* @fn        const asset::GLTFAccessor& BufferBuilder::AddAccessor(const void* data, size_t count, AccessorDesc accessorDesc)
*  @brief     Add Accessor
*  @param[in] const void* data
*  @param[in] size_t count
*  @param[in] AccessorDesc accessorDesc
*  @return 　　const asset::GLTFAccessor&
*****************************************************************************/
const asset::GLTFAccessor& BufferBuilder::AddAccessor(const void* data, size_t count, AccessorDesc accessorDesc)
{
    GLTFBuffer& buffer         = _buffers.Back();
    GLTFBufferView& bufferView = _bufferViews.Back();

    // If the bufferView has not yet been written to then ensure it is correctly aligned for this accessor's component type
    if (bufferView.ByteLength == 0U)
    {
        bufferView.ByteOffset += ::GetPadding(bufferView.ByteOffset, accessorDesc.ComponentDataType);
    }

    accessorDesc.ByteOffset = bufferView.ByteLength;
    const GLTFAccessor& accessor = AddAccessor(count, std::move(accessorDesc));

    bufferView.ByteLength += accessor.GetByteLength();
    buffer.ByteLength      = bufferView.ByteOffset + bufferView.ByteLength;

    if (_resourceWriter)
    {
        _resourceWriter->Write(bufferView, data, accessor);
    }

    return accessor;
}
/****************************************************************************
*                       GetCurrentBuffer
****************************************************************************/
/* @fn        const asset::GLTFBuffer& BufferBuilder::GetCurrentBuffer() const
*  @brief     Get Current Buffer
*  @param[in] void
*  @return 　　const asset::GLTFBuffer&
*****************************************************************************/
const asset::GLTFBuffer& BufferBuilder::GetCurrentBuffer() const
{
    return _buffers.Back();
}
/****************************************************************************
*                       GetCurrentBufferView
****************************************************************************/
/* @fn        const asset::GLTFBufferView& BufferBuilder::GetCurrentBufferView() const
*  @brief     Get Current BufferView
*  @param[in] void
*  @return 　　const asset::GLTFBufferView&
*****************************************************************************/
const asset::GLTFBufferView& BufferBuilder::GetCurrentBufferView() const
{
    return _bufferViews.Back();
}
/****************************************************************************
*                       GetCurrentAccessor
****************************************************************************/
/* @fn        const asset::GLTFAccessor& BufferBuilder::GetCurrentAccessor() const
*  @brief     Get Current Accessor
*  @param[in] void
*  @return 　　const asset::GLTFAccessor&
*****************************************************************************/
const asset::GLTFAccessor& BufferBuilder::GetCurrentAccessor() const
{
    return _accessors.Back();
}
/****************************************************************************
*                       GetBufferCount
****************************************************************************/
/* @fn        size_t BufferBuilder::GetBufferCount() const
*  @brief     Get Buffer Count
*  @param[in] void
*  @return 　　size_t
*****************************************************************************/
size_t BufferBuilder::GetBufferCount() const
{
    return _buffers.Size();
}
/****************************************************************************
*                       GetBufferViewCount
****************************************************************************/
/* @fn        size_t BufferBuilder::GetBufferViewCount() const
*  @brief     Get Buffer View Count
*  @param[in] void
*  @return 　　size_t
*****************************************************************************/
size_t BufferBuilder::GetBufferViewCount() const
{
    return _bufferViews.Size();
}
/****************************************************************************
*                       GetAccessirCount
****************************************************************************/
/* @fn        size_t BufferBuilder::GetAccessorCount() const
*  @brief     Get Accessor Count
*  @param[in] void
*  @return 　　size_t
*****************************************************************************/
size_t BufferBuilder::GetAccessorCount() const
{
    return _accessors.Size();
}
/****************************************************************************
*                       GetResourceWriter
****************************************************************************/
/* @fn        ResourceWriter& BufferBuilder::GetResourceWriter()
*  @brief     Get Resource Writer
*  @param[in] void
*  @return 　　ResourceWriter& 
*****************************************************************************/
ResourceWriter& BufferBuilder::GetResourceWriter()
{
    return *_resourceWriter;
}
/****************************************************************************
*                       GetResourceWriter
****************************************************************************/
/* @fn        const ResourceWriter& BufferBuilder::GetResourceWriter() const
*  @brief     Get Resource Writer
*  @param[in] void
*  @return 　　const ResourceWriter&
*****************************************************************************/
const ResourceWriter& BufferBuilder::GetResourceWriter() const
{
    return *_resourceWriter;
}
/****************************************************************************
*                       AddAccessors
****************************************************************************/
/* @fn        void BufferBuilder::AddAccessors(const void* data, size_t count, size_t byteStride, const AccessorDesc* desc, size_t descCount, std::string* outputIDs)
*  @brief     Add acessors
*  @param[in] const void* data
*  @param[in] size_t count
*  @param[in] size_t byteStride
*  @param[in] const AccessorDesc* desc, 
*  @param[in] size_t descCount, 
*  @param[out] std::string* outputIDs
*  @return 　　const ResourceWriter&
*****************************************************************************/
void BufferBuilder::AddAccessors(const void* data, size_t count, size_t byteStride, const AccessorDesc* pDescs, size_t descCount, std::string* outputIDs)
{
    GLTFBuffer& buffer = _buffers.Back();
    GLTFBufferView& bufferView = _bufferViews.Back();

    if (count == 0 || pDescs == nullptr || descCount == 0)
    {
        throw error::InvalidGLTFException("invalid parameters specified");
    }

    for (size_t i = 0; i < descCount; ++i)
    {
        if (!pDescs[i].IsValid())
        {
            throw error::InvalidGLTFException("invalid AccessorDesc specified in pDescs");
        }
    }

    if (bufferView.ByteLength != 0U)
    {
        throw error::InvalidGLTFException("current buffer view already has written data - this interface doesn't support appending to an existing buffer view");
    }

    size_t extent;

    if (byteStride == 0)
    {
        if (descCount > 1)
        {
            throw error::InvalidGLTFException("glTF 2.0 specification denotes that byte stride must be >= 4 when a buffer view is accessed by more than one accessor");
        }

        extent = count * GLTFAccessor::GetComponentTypeSize(pDescs[0].ComponentDataType) * GLTFAccessor::GetTypeCount(pDescs[0].AccessorDataType);
    }
    else
    {
        extent = count * byteStride;

        // Ensure all accessors fit within the buffer view's extent.
        const size_t lastElement = (count - 1) * (bufferView.ByteStride ? bufferView.ByteStride.Get() : 0U);

        for (size_t i = 0; i < descCount; ++i)
        {
            const size_t accessorSize = GLTFAccessor::GetTypeCount(pDescs[i].AccessorDataType) * GLTFAccessor::GetComponentTypeSize(pDescs[i].ComponentDataType);
            const size_t accessorEnd = lastElement + pDescs[i].ByteOffset + accessorSize;

            if (extent < accessorEnd)
            {
                throw error::InvalidGLTFException("specified accessor does not fit within the currently defined buffer view");
            }
        }
    }

    // Calculate the max alignment.
    size_t alignment = 1;
    for (size_t i = 0; i < descCount; ++i)
    {
        alignment = std::max(alignment, GetAlignment(pDescs[i]));
    }

    bufferView.ByteStride = byteStride;
    bufferView.ByteLength = extent;
    bufferView.ByteOffset += ::GetPadding(bufferView.ByteOffset, alignment);

    buffer.ByteLength = bufferView.ByteOffset + bufferView.ByteLength;

    for (size_t i = 0; i < descCount; ++i)
    {
        AddAccessor(count, pDescs[i]);

        if (outputIDs != nullptr)
        {
            outputIDs[i] = GetCurrentAccessor().ID;
        }
    }

    if (_resourceWriter)
    {
        _resourceWriter->Write(bufferView, data);
    }
}
/****************************************************************************
*                       Output
****************************************************************************/
/* @fn        void BufferBuilder::Output(GLTFDocument& document)
*  @brief     Output
*  @param[in] GLTFDocument& document
*  @return 　　void
*****************************************************************************/
void BufferBuilder::Output(GLTFDocument& document)
{
    for (auto& buffer : _buffers.Elements())
    {
        document.Buffers.Append(std::move(buffer), AppendIdPolicy::ThrowOnEmpty);
    }

    _buffers.Clear();

    for (auto& bufferView : _bufferViews.Elements())
    {
        document.BufferViews.Append(std::move(bufferView), AppendIdPolicy::ThrowOnEmpty);
    }

    _bufferViews.Clear();

    for (auto& accessor : _accessors.Elements())
    {
        document.Accessors.Append(std::move(accessor), AppendIdPolicy::ThrowOnEmpty);
    }

    _accessors.Clear();
}

const asset::GLTFAccessor& BufferBuilder::AddAccessor(size_t count, AccessorDesc desc)
{
    GLTFBuffer& buffer = _buffers.Back();
    GLTFBufferView& bufferView = _bufferViews.Back();

    if (buffer.ID != bufferView.BufferID)
    {
        throw error::InvalidGLTFException("bufferView.bufferId does not match buffer.id");
    }

    if (count == 0)
    {
        throw error::GLTFException("Invalid accessor count: 0");
    }

    if (desc.AccessorDataType == AccessorType::Type_Unknown)
    {
        throw error::GLTFException("Invalid accessorType: TYPE_UNKNOWN");
    }

    if (desc.ComponentDataType == ComponentType::Component_Unknown)
    {
        throw error::GLTFException("Invalid componentType: COMPONENT_UNKNOWN");
    }

    const auto accessorTypeSize = GLTFAccessor::GetTypeCount(desc.AccessorDataType);
    size_t componentTypeSize    = GLTFAccessor::GetComponentTypeSize(desc.ComponentDataType);

    // Only check for a valid number of min and max values if they exist
    if ((!desc.MinValues.empty() || !desc.MaxValues.empty()) &&
        ((desc.MinValues.size() != accessorTypeSize) || (desc.MaxValues.size() != accessorTypeSize)))
    {
        throw error::InvalidGLTFException("the number of min and max values must be equal to the number of elements to be stored in the accessor");
    }

    if (desc.ByteOffset % componentTypeSize != 0)
    {
        throw error::InvalidGLTFException("asccessor offset within buffer view must be a multiple of the component size");
    }

    if ((desc.ByteOffset + bufferView.ByteOffset) % componentTypeSize != 0)
    {
        throw error::InvalidGLTFException("accessor offset within buffer must be a multiple of the component size");
    }

    GLTFAccessor accessor;

    if (_functionGeneratorAccessorID)
    {
        accessor.ID = _functionGeneratorAccessorID(*this);
    }

    accessor.BufferViewID = bufferView.ID;
    accessor.Count             = count;
    accessor.ByteOffset        = desc.ByteOffset;
    accessor.AccessorDataType  = desc.AccessorDataType;
    accessor.ComponentDataType = desc.ComponentDataType;
    accessor.IsNormalized = desc.Normalized;

    // TODO: make accessor min & max members be vectors of doubles
    accessor.MinValue = desc.MinValues;
    accessor.MaxValue = desc.MaxValues;

    return _accessors.Append(std::move(accessor), AppendIdPolicy::GenerateOnEmpty);
}