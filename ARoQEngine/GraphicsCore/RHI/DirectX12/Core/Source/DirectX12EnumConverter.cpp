//////////////////////////////////////////////////////////////////////////////////
//              @file   DirectX12EnumConverter.cpp
///             @brief  Enum Converter
///             @author Toide Yutaro
///             @date   2022_06_29
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12EnumConverter.hpp"
#include "GameUtility/Container/Include/GUDynamicArray.hpp"
#include <stdexcept>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi;
using namespace rhi::directX12;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
D3D12_MESSAGE_SEVERITY EnumConverter::Convert(const rhi::core::MessageSeverity severity)
{
	switch (severity)
	{
		using enum core::MessageSeverity;
		case Corruption: return D3D12_MESSAGE_SEVERITY_CORRUPTION;
		case Error     : return D3D12_MESSAGE_SEVERITY_ERROR;
		case Warning   : return D3D12_MESSAGE_SEVERITY_WARNING;
		case Info      : return D3D12_MESSAGE_SEVERITY_INFO;
		case Message   : return D3D12_MESSAGE_SEVERITY_MESSAGE;
		default:
			throw std::runtime_error("Not supported message severity type (directX12 api)");
	}
}
#pragma region CommandList
D3D12_COMMAND_LIST_TYPE EnumConverter::Convert(const rhi::core::CommandListType type)
{
	switch (type)
	{
		using enum core::CommandListType;

		case Graphics: return D3D12_COMMAND_LIST_TYPE_DIRECT;
		case Compute : return D3D12_COMMAND_LIST_TYPE_COMPUTE;
		case Copy    : return D3D12_COMMAND_LIST_TYPE_COPY;

		default:
			throw std::runtime_error("Not supported command list type (directX12 api)");
	}
}
#pragma endregion CommandList
#pragma region Shader
D3D12_SHADER_VISIBILITY EnumConverter::Convert(const rhi::core::ShaderVisibleFlag visibility)
{
	switch (visibility)
	{
		using enum core::ShaderVisibleFlag;

		case All          : return D3D12_SHADER_VISIBILITY_ALL;
		case Vertex       : return D3D12_SHADER_VISIBILITY_VERTEX;
		case Pixel        : return D3D12_SHADER_VISIBILITY_PIXEL;
		case Geometry     : return D3D12_SHADER_VISIBILITY_GEOMETRY;
		case Mesh         : return D3D12_SHADER_VISIBILITY_MESH;
		case Amplification: return D3D12_SHADER_VISIBILITY_AMPLIFICATION;
		case Hull         : return D3D12_SHADER_VISIBILITY_HULL;
		case Domain       : return D3D12_SHADER_VISIBILITY_DOMAIN;
		
		default:
			throw std::runtime_error("Not supported shader visibility (directX12 api)");
	}
}

D3D12_ROOT_SIGNATURE_FLAGS EnumConverter::Convert1(const rhi::core::ShaderVisibleFlag visibility)
{
	switch (visibility)
	{
		using enum core::ShaderVisibleFlag;

		case All          : return D3D12_ROOT_SIGNATURE_FLAG_NONE;
		case Vertex       : return D3D12_ROOT_SIGNATURE_FLAG_DENY_VERTEX_SHADER_ROOT_ACCESS;
		case Pixel        : return D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS;
		case Geometry     : return D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;
		case Mesh         : return D3D12_ROOT_SIGNATURE_FLAG_DENY_MESH_SHADER_ROOT_ACCESS;
		case Amplification: return D3D12_ROOT_SIGNATURE_FLAG_DENY_AMPLIFICATION_SHADER_ROOT_ACCESS;
		default:
			throw std::runtime_error("Not supported shader visibility (directX12 api)");
	}
}
#pragma endregion Shader
#pragma region Sampler State
/*-------------------------------------------------------------------
-           Texture addresing mode
---------------------------------------------------------------------*/
D3D12_TEXTURE_ADDRESS_MODE  EnumConverter::Convert(const rhi::core::SamplerAddressMode addressingMode)
{
	return static_cast<D3D12_TEXTURE_ADDRESS_MODE>(addressingMode); // all the same
}

/*-------------------------------------------------------------------
-               Border Color
---------------------------------------------------------------------*/
D3D12_STATIC_BORDER_COLOR EnumConverter::Convert(const rhi::core::BorderColor borderColor)
{
	switch (borderColor)
	{
		using enum core::BorderColor;

		case TransparentBlack: return D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
		case OpaqueWhite     : return D3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE;
		case OpaqueBlack     : return D3D12_STATIC_BORDER_COLOR_OPAQUE_BLACK;

		default:
			throw std::runtime_error("failed to convert static border color (directX12 api)");
	}
}

/*-------------------------------------------------------------------
-               Sampling filter mode when the image is enlarged or shirinked.
---------------------------------------------------------------------*/
D3D12_FILTER EnumConverter::Convert(const rhi::core::FilterOption filter)
{
	switch (filter)
	{
		using enum core::FilterOption;

		case MinPointMagPointMipPoint   : return D3D12_FILTER_MIN_MAG_MIP_POINT;
		case MinPointMagPointMipLinear  : return D3D12_FILTER_MIN_MAG_POINT_MIP_LINEAR;
		case MinPointMagLinearMipPoint  : return D3D12_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;
		case MinPointMagLinearMipLinear : return D3D12_FILTER_MIN_POINT_MAG_MIP_LINEAR;
		case MinLinearMagPointMipPoint  : return D3D12_FILTER_MIN_LINEAR_MAG_MIP_POINT;
		case MinLinearMagPointMipLinear : return D3D12_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
		case MinLinearMagLinearMipPoint : return D3D12_FILTER_MIN_MAG_LINEAR_MIP_POINT;
		case MinLinearMagLinearMipLinear: return D3D12_FILTER_MIN_MAG_MIP_LINEAR;
		case Anisotropy                 : return D3D12_FILTER_ANISOTROPIC;

		default:
			throw std::runtime_error("not supported filter option (directX12 api)");
	}
}
#pragma endregion Sampler State
DXGI_FORMAT  EnumConverter::Convert(const rhi::core::PixelFormat pixelFormat)
{
	switch (pixelFormat)
	{
		using enum core::PixelFormat;

		case R8G8B8A8_UNORM     : return DXGI_FORMAT_R8G8B8A8_UNORM;
		case B8G8R8A8_UNORM     : return DXGI_FORMAT_B8G8R8A8_UNORM;
		case R16G16B16A16_FLOAT : return DXGI_FORMAT_R16G16B16A16_FLOAT;
		case R32G32B32A32_FLOAT : return DXGI_FORMAT_R32G32B32A32_FLOAT;
		case R32G32_FLOAT       : return DXGI_FORMAT_R32G32_FLOAT;
		case R32G32B32_FLOAT    : return DXGI_FORMAT_R32G32B32_FLOAT;
		case D24_UNORM_S8_UINT  : return DXGI_FORMAT_D24_UNORM_S8_UINT;
		case R10G10B10A2_UNORM  : return DXGI_FORMAT_R10G10B10A2_UNORM;
		case D32_FLOAT          : return DXGI_FORMAT_D32_FLOAT;
		case R32_FLOAT          : return DXGI_FORMAT_R32_FLOAT;
		case B8G8R8A8_UNORM_SRGB: return DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
		case BC1_UNORM          : return DXGI_FORMAT_BC1_UNORM;
		case BC2_UNORM          : return DXGI_FORMAT_BC2_UNORM;          
		case BC3_UNORM          : return DXGI_FORMAT_BC3_UNORM;
		case BC4_UNORM          : return DXGI_FORMAT_BC4_UNORM;
		case BC5_UNORM          : return DXGI_FORMAT_BC5_UNORM;
		case BC7_UNORM          : return DXGI_FORMAT_BC7_UNORM;
		case BC6H_UNORM         : return DXGI_FORMAT_BC6H_UF16;
		case Unknown            : return DXGI_FORMAT_UNKNOWN;

		default:
			throw std::runtime_error("not supported Pixel Format type (directX12 api)");
	}
}
DXGI_FORMAT EnumConverter::Convert(const rhi::core::IndexType indexFormat)
{
	switch (indexFormat)
	{
		using enum core::IndexType;

		case UInt32: return DXGI_FORMAT_R32_UINT;
		case UInt16: return DXGI_FORMAT_R16_UINT;
		
		default:
			throw std::runtime_error("not supported Index Format type (directX12 api)");
	}
}
#pragma region BlendState
D3D12_BLEND_OP EnumConverter::Convert(const rhi::core::BlendOperator blendOperator)
{
	return static_cast<D3D12_BLEND_OP>(blendOperator); // all the same
}
D3D12_BLEND   EnumConverter::Convert(const rhi::core::BlendFactor blendFactor)
{
	return static_cast<D3D12_BLEND>(blendFactor); // all the same
}
D3D12_COLOR_WRITE_ENABLE EnumConverter::Convert(const rhi::core::ColorMask colorMask)
{
	return static_cast<D3D12_COLOR_WRITE_ENABLE>(colorMask); // all the same
}
#pragma endregion        BlendState
#pragma region RasterizerState
/*-------------------------------------------------------------------
-               Polygon fill mode
---------------------------------------------------------------------*/
D3D12_FILL_MODE EnumConverter::Convert(const rhi::core::FillMode fillMode)
{
	switch (fillMode)
	{
		using enum core::FillMode;

		case Solid    : return D3D12_FILL_MODE_SOLID;
		case WireFrame: return D3D12_FILL_MODE_WIREFRAME;
		case Point    : throw  std::runtime_error("You must select the Vulkan API to use this mode.");
		default:
			throw std::runtime_error("not supported fill mode type (directX12 api)");
	}
}

/*-------------------------------------------------------------------
-               Polygon culling mode
---------------------------------------------------------------------*/
D3D12_CULL_MODE EnumConverter::Convert(const rhi::core::CullingMode cullingMode)
{
	switch (cullingMode)
	{
		using enum core::CullingMode;

		case None : return D3D12_CULL_MODE_NONE;
		case Front: return D3D12_CULL_MODE_FRONT;
		case Back : return D3D12_CULL_MODE_BACK;
		
		default:
			throw std::runtime_error("not supported culling mode type (directX12 api)");
	}
}

/*-------------------------------------------------------------------
-               Front face mode
---------------------------------------------------------------------*/
bool EnumConverter::Convert(const rhi::core::FrontFace   frontFace)
{
	return frontFace == rhi::core::FrontFace::CounterClockwise;
}
#pragma endregion   RasterizerState
#pragma region DepthStencilState
D3D12_COMPARISON_FUNC EnumConverter::Convert(const core::CompareOperator op)
{
	switch (op)
	{
		using enum core::CompareOperator;

		case Never       : return D3D12_COMPARISON_FUNC_NEVER;
		case Less        : return D3D12_COMPARISON_FUNC_LESS;
		case Equal       : return D3D12_COMPARISON_FUNC_EQUAL;
		case LessEqual   : return D3D12_COMPARISON_FUNC_LESS_EQUAL;
		case Greater     : return D3D12_COMPARISON_FUNC_GREATER;
		case NotEqual    : return D3D12_COMPARISON_FUNC_NOT_EQUAL;
		case GreaterEqual: return D3D12_COMPARISON_FUNC_GREATER_EQUAL;
		case Always      : return D3D12_COMPARISON_FUNC_ALWAYS;
		default:
			throw std::runtime_error("not supported compare operator type (directX12 api)");
	}
}

D3D12_STENCIL_OP EnumConverter::Convert(const core::StencilOperator op)
{
	switch (op)
	{
		using enum core::StencilOperator;

		case Keep             : return D3D12_STENCIL_OP_KEEP;
		case Zero             : return D3D12_STENCIL_OP_ZERO;
		case Replace          : return D3D12_STENCIL_OP_REPLACE;
		case IncrementAndClamp: return D3D12_STENCIL_OP_INCR_SAT;
		case DecrementAndClamp: return D3D12_STENCIL_OP_DECR_SAT;
		case Invert           : return D3D12_STENCIL_OP_INVERT;
		case IncrementAndWrap : return D3D12_STENCIL_OP_INCR;
		case DecrementAndWrap : return D3D12_STENCIL_OP_DECR;
		default:
			throw std::runtime_error("not supported stencil operator type (directX12 api)");
	}
}
#pragma endregion DepthStencilState
#pragma region Input Layout
D3D_PRIMITIVE_TOPOLOGY  EnumConverter::Convert(const rhi::core::PrimitiveTopology primitiveTopology)
{
	return static_cast<D3D_PRIMITIVE_TOPOLOGY>(primitiveTopology);
}
D3D12_PRIMITIVE_TOPOLOGY_TYPE EnumConverter::Convert1(const rhi::core::PrimitiveTopology primitiveTopology)
{
	using enum core::PrimitiveTopology;

	switch (primitiveTopology)
	{
		case PointList    : return D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
		case LineList     : return D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
		case LineStrip    : return D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
		case TriangleList : return D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		case TriangleStrip: return D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		default:
			throw std::runtime_error("not supported primitive topology type (directX12 api)");
	}
}

D3D12_INPUT_CLASSIFICATION EnumConverter::Convert(const rhi::core::InputClassification classification)
{
	using enum core::InputClassification;
	switch (classification)
	{
		case PerVertex  : return D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
		case PerInstance: return D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA;
		default:
			throw std::runtime_error("not supported classication type.");
	}
}
DXGI_FORMAT EnumConverter::Convert(const core::InputFormat inputFormat)
{
	using enum core::InputFormat;

	switch (inputFormat)
	{
		case R32G32_FLOAT      : return DXGI_FORMAT_R32G32_FLOAT;
		case R32G32B32_FLOAT   : return DXGI_FORMAT_R32G32B32_FLOAT;
		case R32G32B32A32_FLOAT: return DXGI_FORMAT_R32G32B32A32_FLOAT;
		case R32G32B32A32_INT  : return DXGI_FORMAT_R32G32B32A32_SINT;
		case R32_FLOAT         : return DXGI_FORMAT_R32_FLOAT;
		default:
			throw std::runtime_error("not supported input format");
	}
}
#pragma endregion      Input Layput
#pragma region GPUResource
D3D12_RESOURCE_FLAGS EnumConverter::Convert(const rhi::core::BufferCreateFlags usage)
{
	using enum core::BufferCreateFlags;

	static gu::DynamicArray<core::BufferCreateFlags> sourcePool =
	{
		None,
		VertexBuffer,
		IndexBuffer,
		ConstantBuffer,
		RenderTarget,
		DepthStencil,
		UnorderedAccess,
		Shared,
		DrawIndirect
	};

	static gu::DynamicArray<D3D12_RESOURCE_FLAGS> targetPool =
	{
		D3D12_RESOURCE_FLAG_NONE,
		D3D12_RESOURCE_FLAG_NONE,
		D3D12_RESOURCE_FLAG_NONE,
		D3D12_RESOURCE_FLAG_NONE,
		D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET,
		D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL,
		D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS,
		D3D12_RESOURCE_FLAG_ALLOW_SIMULTANEOUS_ACCESS,
		D3D12_RESOURCE_FLAG_NONE
	};

	auto res = D3D12_RESOURCE_FLAG_NONE;

	for (size_t index = 0; index < sourcePool.Size(); index++)
	{
		if (gu::HasAnyFlags(usage, sourcePool[index]))
			res = res | targetPool[index];
	}

	if (!gu::HasAnyFlags(usage, ShaderResource))
	{
		res |= D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE;
	}

	return res;
}

D3D12_RESOURCE_FLAGS EnumConverter::Convert(const rhi::core::TextureCreateFlags usage)
{
	using enum core::TextureCreateFlags;

	static gu::DynamicArray<core::TextureCreateFlags> sourcePool =
	{
		None,
		RenderTargetable,
		DepthStencilTargetable,
		UnorderedAccess,
	};

	static gu::DynamicArray<D3D12_RESOURCE_FLAGS> targetPool =
	{
		D3D12_RESOURCE_FLAG_NONE,
		D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET,
		D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL,
		D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS,
	};

	auto res = D3D12_RESOURCE_FLAG_NONE;

	for (size_t index = 0; index < sourcePool.Size(); index++)
	{
		if (gu::HasAnyFlags(usage, sourcePool[index]))
			res = res | targetPool[index];
	}

	if (!gu::HasAnyFlags(usage, ShaderResource))
	{
		res |= D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE;
	}

	return res;
}

D3D12_RESOURCE_DIMENSION   EnumConverter::Convert(const rhi::core::ResourceDimension dimension)
{
	using enum core::ResourceDimension;

	switch (dimension)
	{
		case Texture1D: return D3D12_RESOURCE_DIMENSION_TEXTURE1D;
		case Texture2D: return D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		case Texture3D: return D3D12_RESOURCE_DIMENSION_TEXTURE3D;
		case Buffer     : return D3D12_RESOURCE_DIMENSION_BUFFER;
		default:
			throw std::runtime_error("not supported resource dimension (directX12 api)");
	}
}
D3D12_SRV_DIMENSION        EnumConverter::Convert(const rhi::core::ResourceType type)
{
	return static_cast<D3D12_SRV_DIMENSION>(type);
}
D3D12_DESCRIPTOR_HEAP_TYPE EnumConverter::Convert(const rhi::core::DescriptorHeapType heapType)
{
	using enum core::DescriptorHeapType;

	switch (heapType)
	{
		case CBV:
		case SRV:
		case UAV        : return D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		case SAMPLER    : return D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
		case RTV        : return D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		case DSV        : return D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		default:
			throw std::runtime_error("not supported descriptor heap type (directX12 api)");
	}
}
D3D12_DESCRIPTOR_RANGE_TYPE EnumConverter::Convert1(const rhi::core::DescriptorHeapType descriptorType)
{
	using enum core::DescriptorHeapType;
	switch (descriptorType)
	{
		case CBV: return D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
		case SRV: return D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		case UAV: return D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
		case SAMPLER: return D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER;
		default:
			throw std::runtime_error("not supported descriptor range type (directX12 api)");
	}
}
#pragma region GPUBuffer
D3D12_HEAP_TYPE  EnumConverter::Convert(const rhi::core::MemoryHeap memoryHeap)
{
	using enum core::MemoryHeap;
	switch (memoryHeap)
	{
		case Default : return D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_DEFAULT;
		case Upload  : return D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_UPLOAD;
		case Readback: return D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_READBACK;
		case Custom   : return D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_CUSTOM;
		default:
			throw std::runtime_error("not supported heap type (directX12 api)");
	}
}
D3D12_RESOURCE_STATES EnumConverter::Convert(const rhi::core::ResourceState resourceState)
{
	static D3D12_RESOURCE_STATES states[] = {
		D3D12_RESOURCE_STATE_COMMON,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER,
		D3D12_RESOURCE_STATE_INDEX_BUFFER,
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		D3D12_RESOURCE_STATE_COPY_DEST,
		D3D12_RESOURCE_STATE_COPY_SOURCE,
		D3D12_RESOURCE_STATE_PRESENT,
		D3D12_RESOURCE_STATE_INDIRECT_ARGUMENT,
		D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE,
		D3D12_RESOURCE_STATE_SHADING_RATE_SOURCE
	};

	return states[(int)resourceState];
}
#pragma endregion GPUBuffer
#pragma endregion       GPUResource
#pragma region Render Pass
D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE EnumConverter::Convert(const rhi::core::AttachmentLoad op)
{
	switch (op)
	{
		using enum core::AttachmentLoad;

		case Load    : return D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_PRESERVE;
		case Clear   : return D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_CLEAR;
		case DontCare: return D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_DISCARD;
		
		default: 
			throw std::runtime_error("not support render pass beginning type (directX12 api)");
	}
}
D3D12_RENDER_PASS_ENDING_ACCESS_TYPE    EnumConverter::Convert(const rhi::core::AttachmentStore op)
{
	using enum core::AttachmentStore;

	switch (op)
	{
		case Store   : return D3D12_RENDER_PASS_ENDING_ACCESS_TYPE_PRESERVE;
		case DontCare: return D3D12_RENDER_PASS_ENDING_ACCESS_TYPE_DISCARD;
		default:
			throw std::runtime_error("not support render pass ending type (directX12 api)");
	}
}
#pragma endregion Render Pass
#pragma region RayTracing
D3D12_RAYTRACING_GEOMETRY_FLAGS EnumConverter::Convert(const rhi::core::RayTracingGeometryFlags flags)
{
	using enum core::RayTracingGeometryFlags;

	switch (flags)
	{
		case None                        : return D3D12_RAYTRACING_GEOMETRY_FLAG_NONE;
		case Opaque                      : return D3D12_RAYTRACING_GEOMETRY_FLAG_OPAQUE;
		case NoDuplicateAnyHitInvocation : return D3D12_RAYTRACING_GEOMETRY_FLAG_NO_DUPLICATE_ANYHIT_INVOCATION;
		default:
		{
			throw std::runtime_error("not support ray tracing geometry flags");
		}
	}
}

D3D12_RAYTRACING_INSTANCE_FLAGS EnumConverter::Convert(const rhi::core::RayTracingInstanceFlags flags)
{
	using enum core::RayTracingInstanceFlags;

	switch (flags)
	{
		case None                         : return D3D12_RAYTRACING_INSTANCE_FLAG_NONE;
		case TriangleCullDisable          : return D3D12_RAYTRACING_INSTANCE_FLAG_TRIANGLE_CULL_DISABLE;
		case TriangleFrontCounterClockwise: return D3D12_RAYTRACING_INSTANCE_FLAG_TRIANGLE_FRONT_COUNTERCLOCKWISE;
		case ForceOpaque                  : return D3D12_RAYTRACING_INSTANCE_FLAG_FORCE_OPAQUE;
		case ForceNonOpaque               : return D3D12_RAYTRACING_INSTANCE_FLAG_FORCE_NON_OPAQUE;
		default:
			throw std::runtime_error("not support raytracing instance flags (directX12 api)");
	}
}

D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS EnumConverter::Convert(const rhi::core::BuildAccelerationStructureFlags flags)
{
	using enum core::BuildAccelerationStructureFlags;

	switch (flags)
	{
		case None           : return D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_NONE;
		case AllowUpdate    : return D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_ALLOW_UPDATE;
		case AllowCompaction: return D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_ALLOW_COMPACTION;
		case PreformUpdate  : return D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PERFORM_UPDATE;
		case MinimizeMemory : return D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_MINIMIZE_MEMORY;
		case PreferFastBuild: return D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PREFER_FAST_BUILD;
		case PreferFastTrace: return D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PREFER_FAST_TRACE;
		default:
			throw std::runtime_error("not support acceleration structure build flags (directX12 api)");
	}
}
#pragma endregion RayTracing
#pragma region Query
D3D12_QUERY_HEAP_TYPE EnumConverter::Convert(const rhi::core::QueryHeapType heapType)
{
	using enum core::QueryHeapType;

	switch (heapType)
	{
		case Occulusion: return D3D12_QUERY_HEAP_TYPE_OCCLUSION;
		case TimeStamp :  return D3D12_QUERY_HEAP_TYPE_TIMESTAMP;
		case CopyQueueTimeStamp: return D3D12_QUERY_HEAP_TYPE_COPY_QUEUE_TIMESTAMP;
		case PipelineStatistics: return D3D12_QUERY_HEAP_TYPE_PIPELINE_STATISTICS;
		default:
			throw std::runtime_error("not support acceleration query heap type (directX12 api)");
	}
}

D3D12_QUERY_TYPE EnumConverter::Convert1(const rhi::core::QueryHeapType heapType)
{
	using enum core::QueryHeapType;

	switch (heapType)
	{
		case Occulusion        : return D3D12_QUERY_TYPE_OCCLUSION;
		case TimeStamp         : return D3D12_QUERY_TYPE_TIMESTAMP;
		case CopyQueueTimeStamp: return D3D12_QUERY_TYPE_TIMESTAMP;
		case PipelineStatistics: return D3D12_QUERY_TYPE_PIPELINE_STATISTICS;
		default:
			throw std::runtime_error("not support acceleration query heap type (directX12 api)");
	}
}
#pragma endregion Query