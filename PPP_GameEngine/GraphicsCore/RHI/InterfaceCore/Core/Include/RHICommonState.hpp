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
#include <string>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::core
{
#pragma region API
	/****************************************************************************
	*				  			APIVersion
	*************************************************************************//**
	*  @enum      APIVersion
	* 
	*  @brief     Graphics api version (Add as needed.)
	*****************************************************************************/
	enum class APIVersion : std::uint8_t
	{
		Unknown    = 0,
		DirectX12  = 1,
		Vulkan     = 2
	};
#pragma endregion           API
#pragma region CommandList
	/****************************************************************************
	*				  			CommandListType
	*************************************************************************//**
	*  @enum      CommandListType
	*  @brief     Command list (graphics, compute, or copy)
	*****************************************************************************/
	enum class CommandListType
	{
		Unknown,    // For Initialize
		Graphics,   // Graphics command list (directX12 api includes all command list type (use this) )
		Compute,    // Compute command list
		Copy,       // Copy command list
		CountOfType
	};
#pragma endregion   CommandList
#pragma region Index
	/****************************************************************************
	*				  			IndexType
	*************************************************************************//**
	*  @enum      IndexType
	*  @brief     UINT 32 or 16. This value shows byte count.
	*****************************************************************************/
	enum class IndexType
	{
		UInt32 = 4,
		UInt16 = 2,
	};
	/****************************************************************************
	*				  			InputFormat
	*************************************************************************//**
	*  @enum      InputFormat
	*  @brief     Input layout format.  
	*****************************************************************************/
	enum class InputFormat
	{
		Unknown,
		R32_FLOAT,          // 4  byte format
		R32G32_FLOAT,       // 8  byte format
		R32G32B32_FLOAT,    // 12 byte format
		R32G32B32A32_FLOAT, // 16 byte format
		R32G32B32A32_INT,   // 16 byte format
	};
	class InputFormatSizeOf
	{
	public :
		InputFormatSizeOf() = delete;
		static size_t Get(const core::InputFormat inputFormat)
		{
			switch (inputFormat)
			{
				case InputFormat::R32G32_FLOAT      : return 8;
				case InputFormat::R32G32B32_FLOAT   : return 12;
				case InputFormat::R32G32B32A32_INT  :
				case InputFormat::R32G32B32A32_FLOAT: return 16;
				case InputFormat::R32_FLOAT         : return 4;
				default: return 0;
			}
		}
	};
#pragma endregion         Index
#pragma region Pixel
	enum class ShadingRate
	{
		K_1x1,
		K_1x2,
		K_2x1,
		K_2x2,
		K_2x4,
		K_4x2,
		K_4x4
	};
	/****************************************************************************
	*				  			PixelFormat
	*************************************************************************//**
	*  @class     PixelFormat
	*  @brief     Render pixel format 
	*****************************************************************************/
	enum class PixelFormat
	{
		Unknown,   
		R8G8B8A8_UNORM,
		B8G8R8A8_UNORM,
		R10G10B10A2_UNORM,
		R32G32B32A32_FLOAT,
		R16G16B16A16_FLOAT,
		D32_FLOAT,
		R32_FLOAT,
		D24_UNORM_S8_UINT,
		R32G32B32_FLOAT,
		B8G8R8A8_UNORM_SRGB,
		BC1_UNORM,
		CountOfPixelFormat
	};
	/****************************************************************************
	*				  			PixelFormatSizeOf
	*************************************************************************//**
	*  @class     PixelFormatSizeOf
	*  @brief     Get pixel size. Call the static function "Get"
	*****************************************************************************/
	class PixelFormatSizeOf
	{
	public:
		PixelFormatSizeOf() = delete;
		static size_t Get(const core::PixelFormat pixelFormat)
		{
			switch (pixelFormat)
			{
			case PixelFormat::R16G16B16A16_FLOAT:
				return 8;
			case PixelFormat::R8G8B8A8_UNORM:
			case PixelFormat::B8G8R8A8_UNORM:
			case PixelFormat::BC1_UNORM:
			case PixelFormat::D32_FLOAT:
				return 4;
			case PixelFormat::R32G32B32A32_FLOAT:
				return 16;
			case PixelFormat::R32G32B32_FLOAT:
				return 12;
			case PixelFormat::D24_UNORM_S8_UINT:
			case PixelFormat::R10G10B10A2_UNORM:
				return 4;
			default:
				return 0;
			}
		}
		static bool IsDepthOnly(const PixelFormat pixelFormat)
		{
			switch (pixelFormat)
			{
			case PixelFormat::D32_FLOAT: return true;
			default: return false;
			}
		}
	};
	/****************************************************************************
	*				  			ClearValue
	*************************************************************************//**
	*  @class     ClearValue
	*  @brief     Clear value
	*****************************************************************************/
	struct ClearValue
	{
		enum ColorType { Red, Green, Blue, Alpha };
		float        Color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		float        Depth    = 1.0f;
		std::uint8_t Stencil  = 0;

		ClearValue() = default;
		ClearValue(float red, float green, float blue, float alpha)
		{
			Color[0] = red; Color[1] = green; Color[2] = blue; Color[3] = alpha;
		};
		ClearValue(float depth, std::uint8_t stencil)
		{
			Depth = depth; Stencil = stencil;
		}
	};
#pragma endregion         Pixel
#pragma region Shader
	/****************************************************************************
	*				  			ShaderVisibility
	*************************************************************************//**
	*  @enum      ShaderVisibility
	*  @brief     Visible shader stage type
	*****************************************************************************/
	enum class ShaderVisibility : std::uint8_t
	{
		All,           // all      shader stage visible
		Vertex,        // vertex   shader stage only
		Pixel,         // pixel    shader stage only
		Hull,          // hull     shader stage only
		Domain,        // domain   shader stage only
		Geometry,      // geometry shader stage only
		Amplification, // amplification shader stage only,
		Mesh           // mesh shader stage only
	};
	/****************************************************************************
	*				  			ShaderType
	*************************************************************************//**
	*  @enum      ShaderType
	*  @brief     Shader type
	*****************************************************************************/
	enum class ShaderType
	{
		Vertex,           // vertex shader 
		Pixel,            // pixel  shader
		Compute,          // compute shader
		Hull,             // hull shader
		Domain,           // domain shader
		Geometry,         // geometry shader
		Amplification,    // amplication shader
		Mesh,             // mesh shader
		RayGeneration,    // ray tracing ray generation shader
		Intersection,     // ray tracing intersection shader
		AnyHit,           // ray tracing any hit shader
		ClosestHit,       // ray tracing closest hit shader
		Miss              // ray tracing miss shader
	};
	
	struct BlobData
	{
		void*  BufferPointer = nullptr;
		size_t BufferSize = 0;
		BlobData() = default;
		~BlobData() = default;
		BlobData(void* bufferPointer, size_t bufferSize) : BufferPointer(bufferPointer), BufferSize(bufferSize) {};
	};
#pragma endregion        Shader Type
#pragma region Sampler State
	/****************************************************************************
	*				  			SamplerAddressMode
	*************************************************************************//**
	*  @class     SamplerAddressMode
	*  @brief     Texture addressing mode // reference : https://learn.microsoft.com/ja-jp/windows/uwp/graphics-concepts/texture-addressing-modes
	*****************************************************************************/
	enum class SamplerAddressMode : std::uint8_t
	{
		Wrap    = 1, // repeat texture pattern
		Mirror  = 2, // mirror and repeat texture pattern
		Clamp   = 3, // cut over 1.0 and below 0.0
		Border  = 4, // set border color 
	};
	/****************************************************************************
	*				  			BorderColor
	*************************************************************************//**
	*  @class     BorderColor
	*  @brief     Specifies the border color for a sampler
	*****************************************************************************/
	enum class BorderColor : std::uint8_t
	{
		TransparentBlack, // Indicates black, with the alpha component as fully transparent
		OpaqueBlack,      // Indicates black, with the alpha component as fully opaque
		OpaqueWhite       // Indicates white, with the alpha component as fully opaque
	};
	/****************************************************************************
	*				  			FilterMask
	*************************************************************************//**
	*  @class     FilterMask
	*  @brief     Sample mask
	*****************************************************************************/
	enum class FilterMask : std::uint8_t
	{
		Mip = 0x1,
		Mag = 0x2,
		Min = 0x4
	};
	/****************************************************************************
	*				  			FilterOption
	*************************************************************************//**
	*  @class     FilterOption
	*  @brief     Sampling filter option
	*****************************************************************************/
	enum class FilterOption : std::uint8_t
	{
		MinPointMagPointMipPoint    = 0,
		MinPointMagPointMipLinear   = 1,
		MinPointMagLinearMipPoint   = 2,
		MinPointMagLinearMipLinear  = 3,
		MinLinearMagPointMipPoint   = 4,
		MinLinearMagPointMipLinear  = 5,
		MinLinearMagLinearMipPoint  = 6,
		MinLinearMagLinearMipLinear = 7,
		Anisotropy = 8
	};
	/****************************************************************************
	*				  			Default Sampler Type
	*************************************************************************//**
	*  @enum      DefaultSamplerType
	*  @brief     Default Sampler state (Point, Linear or Anisotropic + Wrap or Clamp)
	*****************************************************************************/
	enum DefaultSamplerType
	{
		SamplerPointWrap,         // Point  + Wrap
		SamplerPointClamp,        // Point  + Clamp
		SamplerLinearWrap,        // Linear + Wrap
		SamplerLinearClamp,       // Linear + Clamp
		SamplerAnisotropicWrap,   // Anisotropic + Wrap
		SamplerAnisotropicClamp   // Anisotropic + Clamp
	};
	/****************************************************************************
	*				  			SamplerInfo
	*************************************************************************//**
	*  @class     SamplerInfo
	*  @brief     Custum sampler state. (Normally you should use default sampler type, and call GetDefaultSampler)
	*****************************************************************************/
	struct SamplerInfo
	{
	public:
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		FilterOption        Filter        = FilterOption::MinPointMagPointMipPoint;
		SamplerAddressMode  AddressModeU  = SamplerAddressMode::Wrap;
		SamplerAddressMode  AddressModeV  = SamplerAddressMode::Wrap;
		SamplerAddressMode  AddressModeW  = SamplerAddressMode::Wrap;
		BorderColor         Border        = BorderColor::TransparentBlack;
		size_t              MaxAnisotropy = 1;
		float               MipLODBias    = 0.0f;
		float               MinLOD        = 0.0f;
		float               MaxLOD        = FLT_MAX;

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		SamplerInfo() = default;
		explicit SamplerInfo(
			const FilterOption filter,
			const SamplerAddressMode addressU = SamplerAddressMode::Wrap,
			const SamplerAddressMode addressV = SamplerAddressMode::Wrap,
			const SamplerAddressMode addressW = SamplerAddressMode::Wrap,
			const BorderColor border = BorderColor::TransparentBlack,
			float minLOD = 0.0f,
			float maxLOD = FLT_MAX,
			float mipLODBias = 0.0f) :
			Filter(filter),
			AddressModeU(addressU),
			AddressModeV(addressV),
			AddressModeW(addressW),
			Border(border),
			MinLOD(minLOD),
			MaxLOD(maxLOD),
			MipLODBias(mipLODBias)
		{
		}

		explicit SamplerInfo(
			const std::uint32_t maxAnisotropy,
			const SamplerAddressMode addressU = SamplerAddressMode::Wrap,
			const SamplerAddressMode addressV = SamplerAddressMode::Wrap,
			const SamplerAddressMode addressW = SamplerAddressMode::Wrap,
			const BorderColor border = BorderColor::TransparentBlack) :
			Filter(FilterOption::Anisotropy),
			AddressModeU(addressU),
			AddressModeV(addressV),
			AddressModeW(addressW),
			MaxAnisotropy(maxAnisotropy),
			Border(border)
		{
		}

		static SamplerInfo GetDefaultSampler(DefaultSamplerType type);
	};

#pragma endregion Sampler State
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
		BlendOperator ColorOperator    = BlendOperator::Add;   // Color Blend Type
		BlendOperator AlphaOperator    = BlendOperator::Add;   // Alpha Blend Type
		BlendFactor   DestinationAlpha = BlendFactor::Zero;    // 
		BlendFactor   Destination      = BlendFactor::Zero;
		BlendFactor   SourceAlpha      = BlendFactor::One;
		BlendFactor   Source           = BlendFactor::One;
		ColorMask     ColorMask        = ColorMask::All;

		bool Enable = false;
		BlendProperty() = default;

		BlendProperty(BlendOperator colorOperator, BlendOperator alphaOperator, BlendFactor destAlpha, BlendFactor dest,
			BlendFactor srcAlpha, BlendFactor src, core::ColorMask colorMask = ColorMask::All, bool enable = false) :
			ColorOperator(colorOperator), AlphaOperator(alphaOperator), DestinationAlpha(destAlpha), Destination(dest), SourceAlpha(srcAlpha),
			Source(src), ColorMask(colorMask), Enable(enable) { };
		
		static BlendProperty NoColorWrite();
		static BlendProperty OverWrite();
		static BlendProperty AlphaBlend();
		
	};
#pragma endregion        Blend State
#pragma region Rasterizer State
	/****************************************************************************
	*				  			MultiSample
	*************************************************************************//**
	*  @class     MultiSample
	*  @brief     Basically use count1 , in use MSAA, we use count4 
	*****************************************************************************/
	enum class MultiSample : std::uint8_t
	{
		Count1 = 1,       // 1
		Count2 = 2,       // 2
		Count4 = 4,       // 4
		Count8 = 8,       // 8
		Count16 = 16,     // 16
		Count32 = 32,     // 32
		Count64 = 64      // 64
	};
	class MultiSampleSizeOf
	{
	public: 
		MultiSampleSizeOf() = default;
		static size_t Get(const MultiSample sample) { return static_cast<size_t>(sample); }
	};
	/****************************************************************************
	*				  			CullingMode
	*************************************************************************//**
	*  @class     CullingMode
	*  @brief     Culling mode (left hand coordinate)
	*****************************************************************************/
	enum class CullingMode : std::uint8_t
	{
		None,  // all face render
		Front, // front culling
		Back   // back  culling
	};
	/****************************************************************************
	*				  			FrontFace
	*************************************************************************//**
	*  @class     FrontFace
	*  @brief     Polygon front face
	*****************************************************************************/
	enum class FrontFace : std::uint8_t
	{
		CounterClockwise, // for right hand coordinate
		Clockwise,        // for left  hand coordinate
	};
	/****************************************************************************
	*				  			FillMode
	*************************************************************************//**
	*  @class     FillMode
	*  @brief     Polygon fill mode
	*****************************************************************************/
	enum class FillMode
	{
		WireFrame, // wire frame polygon 
		Solid,     // fill polygon face
		Point,     // point cloud. (only vulkan API)
	};

#pragma endregion   Rasterizer State
#pragma region DepthStencilState
	/****************************************************************************
	*				  			CompareOperator
	*************************************************************************//**
	*  @enum      CompareOperator
	*  @brief     Compare operator
	*****************************************************************************/
	enum class CompareOperator : std::uint8_t
	{
		Never,
		Less,
		Equal,
		LessEqual,
		Greater,
		NotEqual,
		GreaterEqual,
		Always
	};
	/****************************************************************************
	*				  			StencilOperator
	*************************************************************************//**
	*  @class     StencilOperator
	*  @brief     Stencil operator
	*****************************************************************************/
	enum class StencilOperator : std::uint8_t
	{
		Keep,
		Zero,
		Replace,
		IncrementAndClamp,
		DecrementAndClamp,
		Invert,
		IncrementAndWrap,
		DecrementAndWrap
	};
	/****************************************************************************
	*				  			StencilOperatorInfo
	*************************************************************************//**
	*  @class     StencilOperatorInfo
	*  @brief     StencilOperatorInfo
	*****************************************************************************/
	struct StencilOperatorInfo
	{
		CompareOperator CompareOperator   = CompareOperator::Always;
		StencilOperator FailOperator      = StencilOperator::Keep;
		StencilOperator PassOperator      = StencilOperator::Keep;
		StencilOperator DepthFailOperator = StencilOperator::Keep;
		StencilOperatorInfo() = default;
	};
#pragma endregion  DepthStencilState
#pragma region InputAssemblyState
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

	#pragma endregion InputAssemblyState
#pragma region GPUResource
	enum class ResourceDimension : std::uint8_t
	{
		Buffer,
		Dimension1D,
		Dimension2D,
		Dimension3D
	};

	enum class ResourceType : std::uint8_t
	{
		Unknown                           = 0,
		Buffer                            = 1,
		Texture1D                         = 2,
		Texture1DArray                    = 3,
		Texture2D                         = 4,
		Texture2DArray                    = 5,
		Texture2DMultiSample              = 6,
		Texture2DArrayMultiSample         = 7,
		Texture3D                         = 8,
		TextureCube                       = 9,
		TextureCubeArray                  = 10,
		RaytracingAccelerationStructure   = 11
	};

	/****************************************************************************
	*				  			ResourceLayout
	*************************************************************************//**
	*  @class     ResourceState
	*  @brief     How to use resource
	*****************************************************************************/
	enum class ResourceState : std::uint32_t
	{
		Common,
		GeneralRead,
		VertexAndConstantBuffer,
		IndexBuffer,
		UnorderedAccess,
		RenderTarget,
		DepthStencil,
		CopyDestination,
		CopySource,
		Present,
		Indirected,
		RayTracingAccelerationStructure,
		ShadingRateSource,
		CountOfResourceLayout
	};
	/****************************************************************************
	*				  			MemoryHeap
	*************************************************************************//**
	*  @class     MemoryHeap
	*  @brief     memory type
	*****************************************************************************/
	enum class MemoryHeap : std::uint8_t
	{
		Default, // Memory area visible only from GPU  
		Upload,  // Memory area visible to CPU and GPU (Read from GPU is used for one time.)
		Readback,// 
		Custom   // for directX12
	};

	enum class DescriptorHeapType
	{
		CBV,         // const buffer view
		SRV,         // shader resource view
		UAV,         // unordered access view
		SAMPLER,     // dynamic sampler state

		// directX12 api only heap type
		RTV,         // render target (only directX12)
		DSV,         // depth stencil (only directX12)
	};

	struct DefaultHeapCount
	{
		std::uint32_t CBVDescCount = 0;
		std::uint32_t SRVDescCount = 0;
		std::uint32_t UAVDescCount = 0;
		std::uint32_t SamplerDescCount = 0;
		std::uint32_t RTVDescCount = 0;
		std::uint32_t DSVDescCount = 0;
	};
	/****************************************************************************
	*				  			ResourceViewType
	*************************************************************************//**
	*  @enum      ResourceViewType
	*  @brief     How to read gpu resource buffer
	*****************************************************************************/
	enum class ResourceViewType : std::uint32_t
	{
		Unknown,
		ConstantBuffer       , // 256 alighment buffer
		Sampler              , // dynamic sampler state
		Texture              , // readonly texture resource 
		RWTexture            , // read-write texture resource
		Buffer               , // readonly buffer resource
		RWBuffer             , // read-write buffer resource
		StructuredBuffer     , // readonly 
		RWStructuredBuffer   , // read-write buffer resoure
		AccelerationStructure, // raytracing acceleration structure
		ShadingRateSource    ,
		RenderTarget         , // render target
		DepthStencil         , // depth and stencil 
	};

	/****************************************************************************
	*				  			ResourceUsage
	*************************************************************************//**
	*  @enum      ResourceUsage
	*  @brief     resource 
	*****************************************************************************/
	enum class ResourceUsage : std::uint32_t
	{
		None = 0,
		VertexBuffer   = 0x1,
		IndexBuffer    = 0x2,
		ConstantBuffer = 0x4,
		RenderTarget   = 0x8, // allow render target
		DepthStencil   = 0x10, // allow depth stencil
		UnorderedAccess = 0x20, // allow unordered access 
	};
	inline ResourceUsage operator | (const ResourceUsage& left, const ResourceUsage& right)
	{
		return static_cast<ResourceUsage>(static_cast<std::uint32_t>(left) | static_cast<std::uint32_t>(right));
	}
	inline ResourceUsage operator & (const ResourceUsage& left, const ResourceUsage& right)
	{
		return static_cast<ResourceUsage>( static_cast<std::uint32_t>(left) & static_cast<std::uint32_t>(right));
	}

	inline bool EnumHas(const ResourceUsage& left, const ResourceUsage& right)
	{
		if ((left & right) == right) return true;
		return false;
	}


#pragma region GPUBuffer
	/****************************************************************************
	*				  			BufferType
	*************************************************************************//**
	*  @enum      BufferType
	*  @brief     buffer type 
	*****************************************************************************/
	enum class BufferType
	{
		Vertex,   // Vertex buffers
		Index,    // Index buffer 
		Constant, // 256 byte alignment buffer
		Upload,   // CPU readonly buffer
		Default,  // GPU readonly buffer
	};

	/****************************************************************************
	*				  			GPUBufferInfo
	*************************************************************************//**
	*  @class     GPUBufferInfo
	*  @brief     GPUBufferInfo
	*****************************************************************************/
	struct GPUBufferMetaData
	{
	public:
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		size_t         Stride        = 0;                           // element byte size
		size_t         Count         = 0;                           // element array count 
		size_t         ByteSize      = 0;
		ResourceType   ResourceType  = ResourceType::Unknown;       // GPU resource type
		ResourceUsage  ResourceUsage = ResourceUsage::None;         // how to use resource 
		ResourceState  State         = ResourceState::GeneralRead; // resource layout
		MemoryHeap     HeapType      = MemoryHeap::Default;         // memory heap type
		BufferType     BufferType    = BufferType::Upload;          // static or dynamic buffer
		void*          InitData      = nullptr; // Init Data

		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		bool IsCPUAccessible() const
		{
			return HeapType == MemoryHeap::Upload || HeapType == MemoryHeap::Readback
			|| (HeapType == MemoryHeap::Custom);
		}

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		GPUBufferMetaData() = default;
		GPUBufferMetaData(size_t stride, size_t count, core::ResourceUsage usage, ResourceState layout, MemoryHeap heapType, core::BufferType bufferType, void* initData = nullptr);
		/****************************************************************************
		**                Static Function
		*****************************************************************************/
		static GPUBufferMetaData UploadBuffer  (const size_t stride, const size_t count, const MemoryHeap heap = MemoryHeap::Upload, void* initData = nullptr);
		static GPUBufferMetaData DefaultBuffer (const size_t stride, const size_t count, const MemoryHeap heap = MemoryHeap::Default, void* initData = nullptr);
		static GPUBufferMetaData ConstantBuffer(const size_t stride, const size_t count, const MemoryHeap heap = MemoryHeap::Upload , const ResourceState state = ResourceState::Common, void* initData = nullptr); // auto alignment 
		static GPUBufferMetaData VertexBuffer  (const size_t stride, const size_t count, const MemoryHeap heap = MemoryHeap::Default, const ResourceState state = ResourceState::GeneralRead, void* initData = nullptr);
		static GPUBufferMetaData IndexBuffer   (const size_t stride, const size_t count, const MemoryHeap heap = MemoryHeap::Default, const ResourceState state = ResourceState::Common, void* initData = nullptr);
	private:
		size_t CalcConstantBufferByteSize(const size_t byteSize) { return (byteSize + 255) & ~255; }
	};
#pragma endregion         GPUBuffer
#pragma region GPUTexture
	
	
	/****************************************************************************
	*				  			GPUTextureMetaData
	*************************************************************************//**
	*  @class     GPUTextureMetaData
	*  @brief     GPU total infomation
	*****************************************************************************/
	struct GPUTextureMetaData
	{
	public:
	
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		size_t            Width            = 1;                              // texture width
		size_t            Height           = 1;                              // texture height
		size_t            DepthOrArraySize = 1;                              // texture depth or array size
		size_t            ByteSize         = 0;                              // total byte size
		size_t            MipLevels        = 1;                              // mipmap levels
		PixelFormat       PixelFormat      = PixelFormat::Unknown;           // pixel color format 
		MultiSample       Sample           = MultiSample::Count1;            // multi sample count
		ResourceDimension Dimension        = ResourceDimension::Dimension1D; // texture resource dimension
		ResourceType      ResourceType     = ResourceType::Unknown;          // GPU resource type
		ResourceUsage     ResourceUsage    = ResourceUsage::None;            // how to use resource 
		ClearValue        ClearColor       = ClearValue();                   // clear color 
		ResourceState     State            = ResourceState::GeneralRead;    // resource layout
		MemoryHeap        HeapType         = MemoryHeap::Default;            // gpu heap type
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		bool IsCPUAccessible() const
		{
			return HeapType == MemoryHeap::Upload || HeapType == MemoryHeap::Readback
				|| (HeapType == MemoryHeap::Custom);
		}
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		GPUTextureMetaData() = default;
		
		/****************************************************************************
		**                Static Function
		*****************************************************************************/
		static GPUTextureMetaData Texture1D                (const size_t width, const core::PixelFormat format, const size_t mipLevels = 1, const core::ResourceUsage usage = core::ResourceUsage::None);
		static GPUTextureMetaData Texture1DArray           (const size_t width, const size_t length, const core::PixelFormat format, const size_t mipLevels = 1, const core::ResourceUsage usage = core::ResourceUsage::None);
		static GPUTextureMetaData Texture2D                (const size_t width, const size_t height, const core::PixelFormat format, const size_t mipLevels = 1, const core::ResourceUsage usage = core::ResourceUsage::None);
		static GPUTextureMetaData Texture2DArray           (const size_t width, const size_t height, const size_t length, const core::PixelFormat format, const size_t mipLevels = 1, const core::ResourceUsage usage = core::ResourceUsage::None);
		static GPUTextureMetaData Texture3D                (const size_t width, const size_t height, const size_t depth, const core::PixelFormat format, const size_t mipLevels = 1, const core::ResourceUsage usage = core::ResourceUsage::None);
		static GPUTextureMetaData Texture2DMultiSample     (const size_t width, const size_t height, const core::PixelFormat format, const core::MultiSample sample, const size_t mipLevels = 1, const core::ResourceUsage usage = core::ResourceUsage::None);
		static GPUTextureMetaData Texture2DArrayMultiSample(const size_t width, const size_t height, const size_t length, const core::PixelFormat format, const core::MultiSample sample, const size_t mipLevels = 1, const core::ResourceUsage usage = core::ResourceUsage::None);
		static GPUTextureMetaData CubeMap                  (const size_t width, const size_t height, const core::PixelFormat format, const size_t mipLevels = 1, const core::ResourceUsage usage = core::ResourceUsage::None);
		static GPUTextureMetaData CubeMapArray             (const size_t width, const size_t height, const size_t length, const core::PixelFormat format, const size_t mipLevels = 1, const core::ResourceUsage usage = core::ResourceUsage::None);
		static GPUTextureMetaData RenderTarget             (const size_t width, const size_t height, const core::PixelFormat format, const core::ClearValue& clearValue = core::ClearValue());
		static GPUTextureMetaData RenderTargetMultiSample  (const size_t width, const size_t height, const core::PixelFormat format, const core::MultiSample sample, const core::ClearValue& clearValue = ClearValue());
		static GPUTextureMetaData DepthStencil             (const size_t width, const size_t height, const core::PixelFormat format, const core::ClearValue& clearValue = core::ClearValue());
		static GPUTextureMetaData DepthStencilMultiSample  (const size_t width, const size_t height, const core::PixelFormat format, const core::MultiSample sample, const core::ClearValue& clearValue = ClearValue());

	private:
		inline void CalculateByteSize() { ByteSize =  Width * Height * (Dimension == ResourceDimension::Dimension3D ? DepthOrArraySize : 1) * PixelFormatSizeOf::Get(PixelFormat) * MultiSampleSizeOf::Get(Sample); }
	};

#pragma endregion        GPUTexture
#pragma region RayTracing
	/****************************************************************************
	*				  			RayTracingGeometryFlags
	*************************************************************************//**
	*  @enum      RayTracingGeometryFlags
	*  @brief     Specifies flags for raytracing geometry
	*****************************************************************************/
	enum class RayTracingGeometryFlags
	{
		None,                         // No options specified  
		Opaque,                       // When ray encounter this geometry, the geometry acts as if no any hit shader is present.
		NoDuplicateAnyHitInvocation   // require that intersections be reported to the any hit shader at most once. This flag enables that guarantee for the given geometry, potentially with some performance impact.
	};
	/****************************************************************************
	*				  		 RayTracingInstanceFlags
	*************************************************************************//**
	*  @enum      RayTracingInstanceFlags
	*  @brief     Flags for a raytracing acceleration structure instance
	*****************************************************************************/
	enum class RayTracingInstanceFlags
	{
		None                          = 0x0,
		TriangleCullDisable           = 0x1, // Disables front/back face culling for this instance.
		TriangleFrontCounterClockwise = 0x2, // This flag reverses front and back facings, which is useful if the applicationfs natural winding order differs from the default (left coordinate). 
		ForceOpaque                   = 0x4, // All geometry in the BLAS Opaque
		ForceNonOpaque                = 0x8  // All geometry in the BLAS Non Opaque
	};
	/****************************************************************************
	*				  		 BuildAccelerationStructureFlags
	*************************************************************************//**
	*  @enum      BuildAccelerationStructureFlags
	*  @brief     How to build acceleration structure
	*****************************************************************************/
	enum class BuildAccelerationStructureFlags
	{
		None = 0x0000,
		AllowUpdate     = 0x0001, // Support update, 
		AllowCompaction = 0x0002, // Compact AS 
		PreferFastTrace = 0x0004, // RayTracing Performance Maximize. (AS Build time will increase)
		PreferFastBuild = 0x0008, // Build Time Minimize.
		MinimizeMemory  = 0x0010, // Minimize Memory. RayTracing Performance will decrease
		PreformUpdate   = 0x0020, // Update AS
	};
	/****************************************************************************
	*				  	RayTracingASPrebuildInfo
	*************************************************************************//**
	*  @struct    RayTracingASPrebuildInfo
	*  @brief     Acceleration Structure memory data size
	*****************************************************************************/
	struct RayTracingASPrebuildInfo
	{
		std::uint64_t AccelerationStructureSize = 0;
		std::uint64_t BuildScratchDataSize      = 0;
		std::uint64_t UpdateScratchDataSize    = 0;
	};
	
#pragma endregion RayTracing
#pragma endregion GPUResource
#pragma region Render Pass
	enum class AttachmentLoad : std::uint8_t
	{
		Clear,    // at the beginning of a render path, erase already existing data with a specific value
		Load,     
		DontCare
	};
	enum class AttachmentStore
	{
		Store,
		DontCare
	};
	/****************************************************************************
	*				  			Attachment
	*************************************************************************//**
	*  @struct    Attachment
	*  @brief     attachment
	*****************************************************************************/
	struct Attachment
	{
	public:
		/****************************************************************************
		**                Static Function
		*****************************************************************************/
		static Attachment RenderTarget( const PixelFormat format,
			const ResourceState  initialState  = ResourceState::RenderTarget,
			const ResourceState  finalState    = ResourceState::Present,
			const AttachmentLoad load          = AttachmentLoad::Clear,
			const AttachmentStore store        = AttachmentStore::Store )
		{
			return Attachment(format, initialState, finalState, load, store, AttachmentLoad::DontCare, AttachmentStore::DontCare, MultiSample::Count1);
		}

		static Attachment RenderTargetMultiSample( const PixelFormat format, const MultiSample sample,
			const ResourceState   initialState  = ResourceState::RenderTarget,
			const ResourceState   finalState    = ResourceState::Present,
			const AttachmentLoad  load          = AttachmentLoad::Clear,
			const AttachmentStore store         = AttachmentStore::Store
		)
		{
			return Attachment(format, initialState, finalState, load, store, AttachmentLoad::DontCare, AttachmentStore::DontCare, sample);
		}

		static Attachment DepthStencil(const PixelFormat format,
			const ResourceState   initialState  = ResourceState::Common,
			const ResourceState   finalState    = ResourceState::DepthStencil,
			const AttachmentLoad  load          = AttachmentLoad::Clear,
			const AttachmentStore store         = AttachmentStore::Store,
			const AttachmentLoad  stencilLoad   = AttachmentLoad::DontCare,
			const AttachmentStore stencilStore  = AttachmentStore::DontCare
		)
		{
			return Attachment(format, initialState, finalState, load, store, stencilLoad, stencilStore, MultiSample::Count1);
		}

		static Attachment DepthStencilMultiSample(const PixelFormat format, const MultiSample sample,
			const ResourceState   initialState = ResourceState::DepthStencil,
			const ResourceState   finalState   = ResourceState::GeneralRead,
			const AttachmentLoad  load         = AttachmentLoad::Clear,
			const AttachmentStore store        = AttachmentStore::Store,
			const AttachmentLoad  stencilLoad  = AttachmentLoad::DontCare,
			const AttachmentStore stencilStore = AttachmentStore::DontCare
		)
		{
			return Attachment(format, initialState, finalState, load, store, stencilLoad, stencilStore, sample);
		}

		/****************************************************************************
		**                Public Function
		*****************************************************************************/

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		PixelFormat     Format        = PixelFormat::Unknown;         // pixel format
		MultiSample     SampleCount   = MultiSample::Count1;          // multi sample count (default: single sample count)
		ResourceState   InitialLayout = ResourceState::Common;       // initial resource layout  
		ResourceState  FinalLayout    = ResourceState::Present;      // final desired resource layout
		AttachmentLoad  LoadOp        = AttachmentLoad::Clear;        // at the beginning of a render path, erase already existing data with a specific value
		AttachmentStore StoreOp       = AttachmentStore::Store;       // at the end of a render pass, save data on memory
		AttachmentLoad  StencilLoad   = AttachmentLoad::DontCare;     // stencil; 
		AttachmentStore StencilStore  = AttachmentStore::DontCare;
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		Attachment() = default;
		Attachment(
			const PixelFormat     format,
			const ResourceState   initialState,
			const ResourceState   finalState,
			const AttachmentLoad  load,
			const AttachmentStore store,
			const AttachmentLoad  stencilLoad,
			const AttachmentStore stencilStore,
			const MultiSample     sample = MultiSample::Count1
		) : Format(format), SampleCount(sample), InitialLayout(initialState), FinalLayout(finalState), LoadOp(load), StoreOp(store), StencilLoad(stencilLoad), StencilStore(stencilStore){ };

		
	};
#pragma endregion       Render Pass
#pragma region Window Surface
	/****************************************************************************
	*				  			Viewport 
	*************************************************************************//**
	*  @class     Viewport 
	*  @brief     Rect Viewport 
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
		long Left   = 0; // Left window position
		long Top    = 0; // top window position
		long Right  = 0; // right window position
		long Bottom = 0; // bottom window position
		ScissorRect() = default;
		ScissorRect(long left, long top, long right, long bottom)
		{
			this->Left = left; this->Top = top; this->Right = right; this->Bottom = bottom;
		}
	};
	/****************************************************************************
	*				  			WindowInfo
	*************************************************************************//**
	*  @struct    WindowInfo
	*  @brief     Window size and window handle pointer
	*****************************************************************************/
	struct WindowInfo
	{
		size_t Width     = 0;       // window width
		size_t Height    = 0;       // window height
		void*  Handle    = nullptr; // window handle pointer 
		void*  HInstance = nullptr; // window instance for Windows API

		WindowInfo()  = default;

		WindowInfo(size_t width, size_t height, void* handle, void* hInstance = nullptr)
		{
			this->Width = width; this->Height = height; this->Handle = handle; this->HInstance = hInstance;
		}

	};
#pragma endregion    Window Surface
}
#endif