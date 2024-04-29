//////////////////////////////////////////////////////////////////////////////////
///  @file   RHICommonState.hpp
///  @brief  Render Hardware Interfaceにおいて, 様々なフラグを管理している
///  @author Toide Yutaro
///  @date   2024_04_21
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef RHI_COMMON_STATE_HPP
#define RHI_COMMON_STATE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/GUString.hpp"
#include "GameUtility/Base/Include/GUEnumClassFlags.hpp"
#include "GameUtility/Math/Private/Vector/Include/GMVector3i.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIPixelFormat.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
#pragma warning(disable: 4061)
//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::core
{
	#pragma region API
	/****************************************************************************
	*				  			GraphicsAPI
	*************************************************************************//**
	/*  @brief Graphics APIの種類を指定します. 増やす場合はこちらに追加してください
	*****************************************************************************/
	enum class GraphicsAPI : gu::uint8
	{
		Unknown    = 0, //!< グラフィクスAPIを指定していない 
		DirectX12  = 1, //!< DirectX12
		Vulkan     = 2  //!< Vulkan
	};

	/****************************************************************************
	*				     MessageSeverity
	*************************************************************************//**
	/*  @brief メッセージの深刻度を設定します
	*****************************************************************************/
	enum class MessageSeverity : gu::uint8
	{
		Corruption, //!< 致命的なエラー
		Error,      //!< 標準エラー
		Warning,    //!< 警告
		Info,       //!< Information
		Message     //!< メッセージ
	};

	/****************************************************************************
	*				     RHIDebugCreateInfo
	*************************************************************************//**
	/*  @brief Graphics APIの選択, CPU, GPUデバッガの有無を指定します
	*****************************************************************************/
	struct RHIDebugCreateInfo
	{
		/*! @brief CPUのデバッガ (Releaseモード以外で使用可能)*/
		bool EnableCPUDebugger   = true;

		/*! @brief GPUのデバッガ (Releaseモード以外で使用可能)*/
		bool EnableGPUDebugger   = false;

		/*! @brief GPU用のブレークポイントを設定できるようにするか*/
		bool EnableGPUDebugBreak = false;

		/*! @brief GPU用のブレークポイントを設定する場合にどこまでの深刻度を設定するか*/
		MessageSeverity GPUDebugBreakOnSeverity = MessageSeverity::Error;

		RHIDebugCreateInfo() = default;

		RHIDebugCreateInfo(const bool enableCPUDebugger, const bool enableGPUDebugger, const bool enableGPUDebugBreak, const MessageSeverity severity = MessageSeverity::Error)
			: EnableCPUDebugger(enableCPUDebugger), EnableGPUDebugger(enableGPUDebugger), EnableGPUDebugBreak(enableGPUDebugBreak), GPUDebugBreakOnSeverity(severity)
		{};
	};

	/****************************************************************************
	*				     DisplayAdapterVendorType
	*************************************************************************//**
	/*  @brief 物理デバイスで使用するアダプタのベンダーIDを取得します. 
	*****************************************************************************/
	enum class DisplayAdapterVendorType : gu::uint32
	{
		None      = 0,         //!< Not assigned
		Amd       = 0x1002,    //!<　AMD
		Nvidia    = 0x10DE,    //!< NVIDIA
		Arm       = 0x13B5,    //!< Arm
		Intel     = 0x8086,    //!< Intel
		Apple     = 0x106B,    //!< Apple
		Microsoft = 0x1414,    //!< Microsoft
		Unknown   = 0xffffffff //!< Other vender
	};

	#pragma endregion 

	#pragma region CommandList
	/****************************************************************************
	*				  			CommandListType
	*************************************************************************//**
	*  @enum      CommandListType
	*  @brief     GPU命令を出すためのコマンドリストの種類
	*****************************************************************************/
	enum class CommandListType : gu::uint8
	{
		Unknown,    //!< 初期化の際に使用します
		Graphics,   //!< 描画用のコマンドリスト (directX12では, このコマンドリストでCompute, Copy両方の役割も担うことが出来ます.)
		Compute,    //!< コンピュートシェーダーを使用するコマンドリスト (非同期コンピュートに使用)
		Copy,       //!< GPUリソースのコピーに使用します.
		CountOfType //!< CommandListの種類数
	};
	#pragma endregion 

	#pragma region Window Surface
	/****************************************************************************
	*				  			Viewport 
	*************************************************************************//**
	/*! @brief   描画領域を設定する構造体です.
	*****************************************************************************/
	struct Viewport
	{
		/*! @brief Viewport左上隅を指定する位置をピクセル単位に基づいて指定します*/
		float TopLeftX  = 0.0f;

		/*! @brief Viewport左上隅を指定する位置をピクセル単位に基づいて指定します*/
		float TopLeftY  = 0.0f;

		/*! @brief Viewportの幅をピクセル単位に基づいて指定します*/
		float Width     = 0.0f;

		/*! @brief Viewportの高さをピクセル単位に基づいて指定します*/
		float Height    = 0.0f;

		/*! @brief 最小の深度を0.0f～1.0fの間で指定します.*/
		float MinDepth  = 0.0f;

		/*! @brief 最大の深度を0.0f～1.0fの間で指定します. */
		float MaxDepth  = 1.0f;

		/*! @brief デフォルトコンストラクタ*/
		Viewport() = default;

		/*! @brief Viewportを規定する情報を一通り登録するコンストラクタです*/
		Viewport(float topLeftX, float topLeftY, float width, float height, float minDepth = 0.0f, float maxDepth = 1.0f)
		{
			this->TopLeftX = topLeftX; this->TopLeftY = topLeftY; this->Width = width; this->Height = height; this->MinDepth = minDepth; this->MaxDepth = maxDepth;
		}
	};

	/****************************************************************************
	*				  			ScissorRect
	*************************************************************************//**
	*  @brief   Viewportの中で実際に描画される範囲を指定するシザー矩形
	*  @note    http://angra.blog31.fc2.com/blog-entry-52.html (シザー矩形の表示される領域説明. ただし, 座標系は異なるため注意)
	*****************************************************************************/
	struct ScissorRect
	{
		/*! @brief ビューポートの左端の位置を0として, そこからの描画開始位置をピクセル単位で指定します.*/
		long Left   = 0; // Left window position

		/*! @brief ビューポートの上端の位置を0として, そこからの描画開始位置をピクセル単位で指定します.*/
		long Top    = 0; // top window position

		/*! @brief ビューポートの左端の位置を0として, そこからの描画幅をピクセル単位で指定します.*/
		long Right  = 0;

		/*! @brief ビューポートの上端の位置を0として, そこｋらの描画高さをピクセル単位で指定します*/
		long Bottom = 0; 

		/*! @brief デフォルトコンストラクタ*/
		ScissorRect() = default;

		/*! @brief 描画領域を直接指定するコンストラクタ*/
		ScissorRect(long left, long top, long right, long bottom)
		{
			this->Left = left; this->Top = top; this->Right = right; this->Bottom = bottom;
		}
	};

	/****************************************************************************
	*				  			WindowInfo
	*************************************************************************//**
	*  @brief   ビューポートとは異なり, 純粋なウィンドウのサイズやWindowハンドルを取得するクラスです
	*****************************************************************************/
	struct WindowInfo
	{
		/*! @brief ウィンドウの幅をピクセル単位で指定します*/
		gu::uint64 Width     = 0; 

		/*! @brief ウィンドウの高さをピクセル単位で指定します*/
		gu::uint64 Height    = 0;

		/*! @brief ウィンドウ領域のハンドル*/
		void*  Handle    = nullptr;

		/*! @brief ウィンドウのインスタンスハンドルです (WindowsAPIのみ使用します.)*/
		void*  HInstance = nullptr;

		/*! @brief デフォルトコンストラクタ*/
		WindowInfo()  = default;

		/*! @brief 直接ウィンドウ情報を指定します*/
		WindowInfo(gu::uint64 width, gu::uint64 height, void* handle, void* hInstance = nullptr)
		{
			this->Width = width; this->Height = height; this->Handle = handle; this->HInstance = hInstance;
		}

	};

	#pragma endregion

	#pragma region Pixel

	enum class ShadingRate : gu::uint8
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
	*				  			DisplayOutputFormat
	*************************************************************************//**
	*  @enum      DisplayOutputFormat
	*  @brief     Color format
	*****************************************************************************/
	enum class DisplayOutputFormat : gu::uint8
	{
		SDR_SRGB,
		SDR_Rec709,
		HDR_ACES_1000nit_ST2084,
		HDR_ACES_2000nit_ST2084,
		HDR_ACES_1000nit_ScRGB,
		HDR_ACES_2000nit_ScRGB,
		HDR_Linear_NoToneCurve,
		HDR_Linear_WithToneCurve
	};

	/****************************************************************************
	*				  			DisplayColorGamut
	*************************************************************************//**
	*  @enum      DisplayColorGamut
	*  @brief     Color range
	*             https://uwatechnologies.hatenablog.com/entry/2022/04/09/001938
	* 　　　　　 　　　https://garagefarm.net/jp-blog/what-is-color-space-and-why-you-should-use-aces
	*             https://qiita.com/UWATechnology/items/2a40dbc66bf48041d405
	*****************************************************************************/
	enum class DisplayColorGamut : gu::uint8
	{
		SRGB_D65,    // srgb color format    + white point D65 (Windows標準色域)
		DCIP3_D65,   // dcpi3   color format + white point D65 (映像撮影に使われるカラーフィルムの色域に対応した広範囲の色域を表現できる規格)
		Rec2020_D65, // rec2020 color format + white point D65 (HDR用に使われる色域)
		ACES_D60,    // aces    color format + white point D60 (ダイナミックレンジが広い)  
		ACEScg_D60   // aces cg color format 
	};

	/****************************************************************************
	*				  			HDRDisplayInfo
	*************************************************************************//**
	*  @enum      HDRDisplayInfo
	*  @brief     HDR display settings (RHIDeviceにて設定を行います)
	*             https://qiita.com/dgtanaka/items/672d2e7b3152f4e5ed49
	*****************************************************************************/
	struct HDRDisplayInfo
	{
		DisplayColorGamut   ColorGamut    = DisplayColorGamut::SRGB_D65;
		DisplayOutputFormat DisplayFormat = DisplayOutputFormat::SDR_SRGB;
		float RedPrimary[2]         = {0,0}; // red   xy coordinate in the color space
		float GreenPrimary[2]       = {0,0}; // green xy coordinate in the color space
		float BluePrimary[2]        = {0,0}; // blue  xy coordinate in the color space
		float WhitePoint[2]         = {0,0}; // white xy coordinate in the color space
		float MinLuminance          = 0.0f;  // nits
		float MaxLuminacnce         = 0.0f;  // nits
		float MaxFullFrameLuminance = 0.0f;
		ScissorRect Rect = {};
	};

	/****************************************************************************
	*				  			ClearValue
	*************************************************************************//**
	/*  @brief     画面クリア時に初期化で塗りつぶされる色
	*****************************************************************************/
	struct ClearValue
	{
		/*! @brief 色の種類をRGBAで指定します. */
		enum ColorType { Red, Green, Blue, Alpha };

		/*! @brief DepthStencilの構造体*/
		struct DepthStencilValue
		{
			gu::float32 Depth;
			gu::uint32  Stencil;
		};

		/*!@brief : クリアカラーの種別 (RGBAかデプス/ステンシル)*/
		union ClearType
		{
			gu::float32       Color[4];
			DepthStencilValue DSV;
		} Type ;

		/*! @brief デフォルトコンストラクタ*/
		ClearValue() 
		{
			Type.Color[0] = 1.0f;
			Type.Color[1] = 1.0f;
			Type.Color[2] = 1.0f;
			Type.Color[3] = 1.0f;
		}

		/*! @brief RGBAで指定するクリアカラーの色*/
		ClearValue(const float red, const float green, const float blue, const float alpha)
		{
			Type.Color[0] = red; Type.Color[1] = green; Type.Color[2] = blue; Type.Color[3] = alpha;
		};
		
		/*! @brief 深度値とステンシル値で色を初期化します*/
		explicit ClearValue(const float depth, const gu::uint32 stencil = 0)
		{
			Type.DSV.Depth = depth; Type.DSV.Stencil = stencil; 
		}
	};
	#pragma endregion         Pixel
#pragma region Shader
	/****************************************************************************
	*				  			ShaderVisibleFlag
	*************************************************************************//**
	*  @enum      ShaderVisibleFlag
	*  @brief     Visible shader stage type
	*****************************************************************************/
	enum class ShaderVisibleFlag : gu::int16
	{
		// directX12に準拠しています D3D12_ROOT_SIGNATURE_FLAGS
		Vertex        = 0x0002, // vertex   shader stage only
		Hull          = 0x0004, // hull     shader stage only
		Domain        = 0x0008, // domain   shader stage only
		Geometry      = 0x0010, // geometry shader stage only
		Pixel         = 0x0020, // pixel    shader stage only
		Amplification = 0x0100, // amplification shader stage only,
		Mesh          = 0x0200, // mesh shader stage only
		All = Vertex | Pixel | Hull | Domain | Geometry | Amplification | Mesh, // all shader stage visible
		CountOfPipeline = 7
	};

	__forceinline ShaderVisibleFlag operator | (const ShaderVisibleFlag& left, const ShaderVisibleFlag& right)
	{
		return static_cast<ShaderVisibleFlag>(static_cast<gu::uint8>(left) | static_cast<gu::uint8>(right));
	}
	__forceinline ShaderVisibleFlag operator & (const ShaderVisibleFlag& left, const ShaderVisibleFlag& right)
	{
		return static_cast<ShaderVisibleFlag>(static_cast<gu::uint8>(left) & static_cast<gu::uint8>(right));
	}

	__forceinline bool EnumHas(const ShaderVisibleFlag& left, const ShaderVisibleFlag& right)
	{
		return (left & right) == right;
	}

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
		gu::uint64 BufferSize = 0;
		BlobData() = default;
		~BlobData() = default;
		BlobData(void* bufferPointer, gu::uint64 bufferSize) : BufferPointer(bufferPointer), BufferSize(bufferSize) {};
	};
#pragma endregion        Shader Type
#pragma region Sampler State
	/****************************************************************************
	*				  			SamplerAddressMode
	*************************************************************************//**
	*  @class     SamplerAddressMode
	*  @brief     Texture addressing mode // reference : https://learn.microsoft.com/ja-jp/windows/uwp/graphics-concepts/texture-addressing-modes
	*****************************************************************************/
	enum class SamplerAddressMode : gu::uint8
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
	enum class BorderColor : gu::uint8
	{
		TransparentBlack, // Indicates black, with the alpha component as fully transparent
		OpaqueBlack,      // Indicates black, with the alpha component as fully opaque(完全不透明)
		OpaqueWhite       // Indicates white, with the alpha component as fully opaque
	};

	/****************************************************************************
	*				  			FilterMask
	*************************************************************************//**
	*  @class     FilterMask
	*  @brief     Sample mask
	*****************************************************************************/
	enum class FilterMask : gu::uint8
	{
		Mip = 0x1,
		Mag = 0x2,
		Min = 0x4
	};

	/****************************************************************************
	*				  			FilterOption
	*************************************************************************//**
	*  @class     FilterOption
	*  @brief     Sampling filter option. linear -> linear sampling, point -> point sampling
	*****************************************************************************/
	enum class FilterOption : gu::uint8
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
		SamplerLinearBorder,      /// Linear + Border
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
		FilterOption        Filter        = FilterOption::MinPointMagPointMipPoint; // Specify sampling method for image enlargement/shirinkage*
		SamplerAddressMode  AddressModeU  = SamplerAddressMode::Wrap;               // Texture addressing mode in the U direction
		SamplerAddressMode  AddressModeV  = SamplerAddressMode::Wrap;               // Texture addressing mode in the V direction
		SamplerAddressMode  AddressModeW  = SamplerAddressMode::Wrap;               // Texture addressing mode in the W direction
		BorderColor         Border        = BorderColor::TransparentBlack;          // Border color 
		gu::uint64              MaxAnisotropy = 1;                                      // Max anisotropy
		gu::float32         MipLODBias    = 0.0f;                                   // Defined LOD = normalLOD + bias
		gu::float32         MinLOD        = 0.0f;                                   // Min LOD size
		gu::float32         MaxLOD        = MAX_FLOAT32;                            // Max LOD size: FLT_MAX 上限を指定しない.

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
			float maxLOD = MAX_FLOAT32,
			float mipLODBias = 0.0f) :
			Filter(filter),
			AddressModeU(addressU),
			AddressModeV(addressV),
			AddressModeW(addressW),
			Border(border),
			MipLODBias(mipLODBias),
			MinLOD(minLOD),
			MaxLOD(maxLOD)
		{
		}

		explicit SamplerInfo(
			const gu::uint32 maxAnisotropy,
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
	*             Source : Color to be rendered from now on
	*             Dest   : Color of rendering destination
	*****************************************************************************/
	enum class BlendFactor : gu::uint8
	{
		Zero = 1,             // * 0
		One,                  // * 1
		Source_Color,         // * Rs, *Gs, *Bs, *As
		Inverse_Source_Color, // * (1 - Rs), *(1 - Gs), *(1 - Bs), *(1 - As)
		Source_Alpha,         // * As
		Inverse_Source_Alpha, // * (1 - As)
		Dest_Alpha,           // * Ad
		Inverse_Dest_Alpha,   // * (1 - Ad)
		Dest_Color,           // * Rd, *Gd, *Bd, *Ad
		Inverse_Dest_Color,   // * (1 - Rd), *(1 - Gd), *(1 - Bd), *(1 - Ad)
		Source_Alpha_Saturate,// * mih(1 - Ad, As)
	};

	/****************************************************************************
	*				  			BlendOperator
	*************************************************************************//**
	*  @class     BlendOperator
	*  @brief     Color blend calculate opration (directX12 based)
	*****************************************************************************/
	enum class BlendOperator : gu::uint8
	{
		Add              = 1, // Default color blend operator : destination + source
		Subtract         = 2, // source - destination
		Reverse_Subtract = 3, // destination - source
		Min              = 4, // min(destination, source)
		Max              = 5  // max(destination, source)
	};

	/****************************************************************************
	*				  			ColorMask
	*************************************************************************//**
	*  @class     ColorMask 
	*  @brief     Color mask bit flag
	*****************************************************************************/
	enum class ColorMask : gu::uint8
	{
		None  = 0,   // All Disable 
		Red   = 0x1, // Red WriteEnable
		Green = 0x2, // Green WriteEnable
		Blue  = 0x4, // Blue WriteEnable
		Alpha = 0x8, // Alpha WriteEnable
		RGB   = Red | Green | Blue,
		RG    = Red | Green,
		BA    = Blue | Green,
		All   = Red | Green | Blue | Alpha // AllEnable
	};

	inline ColorMask operator | (const ColorMask& left, const ColorMask& right)
	{
		return static_cast<ColorMask>( static_cast<gu::uint8>(left) | static_cast<gu::uint8>(right));
	}

	/****************************************************************************
	*				  			BlendProperty
	*************************************************************************//**
	*  @class     BlendProperty
	*  @brief     Property
	*             Source      : これからレンダリングする色 (ピクセルシェーダー)
	*             Destination : レンダリング先 (レンダーターゲット)
	*****************************************************************************/
	struct BlendProperty
	{
		BlendOperator ColorOperator    = BlendOperator::Add;   // RGB Color Blend Type 
		BlendOperator AlphaOperator    = BlendOperator::Add;   // Alpha Blend Type
		BlendFactor   DestinationAlpha = BlendFactor::Zero;    // Multiply to Render Target alpha(a) element blend mode
		BlendFactor   DestinationRGB   = BlendFactor::Zero;    // Multiply to Render Target color(rgb) element blend mode
		BlendFactor   SourceAlpha      = BlendFactor::One;     // Multiply to Pixel Shader alpha element(a) blend mode
		BlendFactor   SourceRGB        = BlendFactor::One;     // Multiply to Pixel Shader color element(rgb) blend mode
		ColorMask     ColorMask        = ColorMask::All;       // color mask
		bool AlphaToConverageEnable    = false;                // Multi sample時に使用する. ピクセルシェーダーから出力されたあrぷふぁを取得し, Multi sanling aliasingを適用する

		bool Enable = false; // レンダーターゲットの独立したブレンドを使用するかを設定する

		BlendProperty() = default;

		BlendProperty(BlendOperator colorOperator, BlendOperator alphaOperator, BlendFactor destAlpha, BlendFactor dest,
			BlendFactor srcAlpha, BlendFactor src, core::ColorMask colorMask = ColorMask::All, bool alphaToConverageEnable = false, bool enable = false) :
			ColorOperator(colorOperator), AlphaOperator(alphaOperator), DestinationAlpha(destAlpha), DestinationRGB(dest), SourceAlpha(srcAlpha),
			SourceRGB(src), ColorMask(colorMask), Enable(enable), AlphaToConverageEnable(alphaToConverageEnable) { };
		
		/*----------------------------------------------------------------------
		*  @brief : NoColorWrite (Display the render target as it is)
		/*----------------------------------------------------------------------*/
		static BlendProperty NoColorWrite(const bool useAlphaToCoverage = false);

		/*----------------------------------------------------------------------
		*  @brief : OverWrite (Displays the source as it is.)
		/*----------------------------------------------------------------------*/
		static BlendProperty OverWrite (const bool useAlphaToCoverage = false);

		/*----------------------------------------------------------------------
		*  @brief : Alpha blending : destination * (1 - source.Alpha) + source * 1
		/*----------------------------------------------------------------------*/
		static BlendProperty AlphaBlend(const bool useAlphaToCoverage = false);
	};

#pragma endregion       Blend   State
	#pragma region Rasterizer State
	/****************************************************************************
	*				  			MultiSample
	*************************************************************************//**
	/*  @brief  サンプリングの数を指定します. 基本的にはCount1を使用し, MSAAなどを使用するときはCount4を主に使用します.
	*****************************************************************************/
	enum class MultiSample : gu::uint8
	{
		Count1 = 1,       //!<  1  pixel
		Count2 = 2,       //!<  2  pixel
		Count4 = 4,       //!<  4  pixel
		Count8 = 8,       //!<  8  pixel
		Count16 = 16,     //!<  16 pixel
		Count32 = 32,     //!<  32 pixel 
		Count64 = 64      //!<  64 pixel
	};

	/****************************************************************************
	*				  			CullingMode
	*************************************************************************//**
	/*  @brief    指定された面(表面, 裏面)に対する三角形ポリゴンを描画しないように設定します. @n
	*             指定した面を描画しないようにするため, 描画負荷の軽減に寄与します.
	*****************************************************************************/
	enum class CullingMode : gu::uint8
	{
		None,  //!< 表面・裏面両方とも描画します. 
		Front, //!< 裏面のみ描画し, 表面は描画しません.
		Back   //!< 表面のみ描画し, 裏面は描画しません. 
	};

	/****************************************************************************
	*				  			FrontFace
	*************************************************************************//**
	/*  @brief   前面が反時計回りか時計回りかを決定します.
	*****************************************************************************/
	enum class FrontFace : gu::uint8
	{
		CounterClockwise, //!< 前面を反時計回りに設定します. 主に右手系に対して使用します.
		Clockwise,        //!< 前面を時計回りに設定します.  主に左手系に対して使用します. 
	};

	/****************************************************************************
	*				  			FillMode
	*************************************************************************//**
	/*  @brief  描画の際に, ポリゴンの三角形内部をどのように満たすかを設定します.
	*****************************************************************************/
	enum class FillMode
	{
		WireFrame, //!< 頂点間をワイヤーフレームで満たし, 内部は満たしません.
		Solid,     //!< ポリゴン内部も描画します. 
		Point,     //!< 点群として描画するため, 内部は満たしません.  (only vulkan API)
	};

	/****************************************************************************
	*				  			RasterizerProperty
	*************************************************************************//**
	/*  @brief     シェーダパイプラインにおける, ラスタライザステートに関する設定項目です.
	*****************************************************************************/
	struct RasterizerProperty
	{
		/*! @brief 描画の際に, ポリゴンの三角形内部をどのように満たすかを設定します. */
		FillMode FillType = FillMode::Solid;

		/*! @brief  前面が反時計回りか時計回りかを決定します. 基本は左手系のためClockWiseを指定します. */
		FrontFace FaceType = FrontFace::Clockwise;

		/*! @brief 指定された面(表面, 裏面)に対する三角形ポリゴンを描画しないように設定します*/
		CullingMode CullingType = CullingMode::None;

		/*! @brief 深度の大きさに応じてClamp処理(ピクセルの描画を行わない)を実行するかを決定します. */
		bool UseDepthClamp = true;

		/*! @brief マルチサンプリングによる描画を行うかを設定します. */
		bool UseMultiSample = true;

		/*! @brief DirectX12のみで設定可能です. 線のアンチエイリアスを有効にするかを設定します. @n
		           有効化出来る組み合わせは以下を参照してください https://learn.microsoft.com/ja-jp/windows/win32/api/d3d12/ns-d3d12-d3d12_rasterizer_desc*/
		bool UseAntiAliasLine = false;

		/*! @brief 少しでもピクセルの描画範囲に入ったらラスタライズを実行するようにします.*/
		bool UseConservativeRaster = false;

		// 計算方法 : https://learn.microsoft.com/ja-jp/windows/win32/direct3d11/d3d10-graphics-programming-guide-output-merger-stage-depth-bias?redirectedfrom=MSDN
		// Bias = (float)DepthBias * r + SlopeScaledDepthBias * MaxDepthSlope [unorm];
		// Bias = (float)DepthBias * 2 **(exponent(max z in primitive) - r) + SlopeScaledDepthBias * MaxDepthSlope; [float]
		// rは浮動小数点表現の仮数ビットの数 (float32の場合は23を示します.)
		
		/*! @brief 与えられたピクセルに加えられるDepth値*/
		float DepthBias = 0.0f; 

		/*! @brief MaxDepthSlopeに対する補正項*/
		float  SlopeScaleDepthBias = 0.0f;

		/*! @brief 最終的なBiasに対してClampされるBias値*/
		float  ClampMaxDepthBias = 0.0f;

		/*! @brief デフォルトコンストラクタ*/
		RasterizerProperty() = default;

		/*! @brief 基本設定*/
		RasterizerProperty
		(
			const FrontFace   frontFace,
			const CullingMode cullingMode,
			const FillMode    fillMode,
			const bool  useDepthClamp,
			const bool  useMultiSample,
			const float depthBias           = 0.0f,
			const float slopeScaleDepthBias = 0.0f,
			const float clampMaxDepthBias   = 0.0f
		) :
			FaceType(frontFace), CullingType(cullingMode), FillType(fillMode), 
			UseDepthClamp(useDepthClamp), UseMultiSample(useMultiSample),
			DepthBias(depthBias), SlopeScaleDepthBias(slopeScaleDepthBias), 
			ClampMaxDepthBias(clampMaxDepthBias)
		{
		};

		/*!**********************************************************************
		*  @brief  3Dの三角形ポリゴン内部を埋める基本設定です.
		*************************************************************************/
		static RasterizerProperty Solid
		(
			const bool        useMultiSample      = false, 
			const FrontFace   frontFace           = FrontFace::Clockwise, 
			const CullingMode cullingMode         = CullingMode::None,
			const float       depthBias           = 0.0f,
			const float       slopeScaleDepthBias = 0.0f,
			const float       clampMaxDepthBias   = 0.0f
		);
		
		/*!**********************************************************************
		*  @brief  3Dの三角形ポリゴン内部を埋めず, 線だけを表示する基本設定です. 
		*************************************************************************/
		static RasterizerProperty WireFrame
		(
			const bool        useMultiSample      = false, 
			const FrontFace   frontFace           = FrontFace::Clockwise, 
			const CullingMode cullingMode         = CullingMode::None,
			const float       depthBias           = 0.0f,
			const float       slopeScaleDepthBias = 0.0f,
			const float       clampMaxDepthBias   = 0.0f
		);
	};
	#pragma endregion   Rasterizer State
#pragma region DepthStencilState
	/****************************************************************************
	*				  			CompareOperator
	*************************************************************************//**
	*  @enum      CompareOperator
	*  @brief     Compare operator
	*****************************************************************************/
	enum class CompareOperator : gu::uint8
	{
		Never,          // Always false
		Less,           // reference < test
		Equal,          // reference = test
		LessEqual,      // reference <= test
		Greater,        // reference > test
		NotEqual,       // reference not equal test
		GreaterEqual,   // reference >= test
		Always          // Always true
	};

	/****************************************************************************
	*				  			StencilOperator
	*************************************************************************//**
	*  @class     StencilOperator
	*  @brief     Stencil operator (設定したテストが失敗, または成功した場合に格納されたステンシル値に何が起こるかを指定する) 
	*             良い感じの説明: https://www.asawicki.info/news_1654_stencil_test_explained_using_code
	*****************************************************************************/
	enum class StencilOperator : gu::uint8
	{
		Keep,                 // keep the current value
		Zero,                 // set the value to 0
		Replace,              // sets the value to reference value.
		IncrementAndClamp,    // increments the current value and clamps to the maximum representable unsigned value
		DecrementAndClamp,    // decrement the current value and clamps to 0
		Invert,               // bitwise-inverts the current value
		IncrementAndWrap,     // increment the current value and wrap to 0 when the maximum value would have been exceeded
		DecrementAndWrap      // decrements the current value and wrap to the maximum possible value when the value wourld bo below 0
	};

	/****************************************************************************
	*				  			StencilOperatorInfo
	*************************************************************************//**
	*  @class     StencilOperatorInfo
	*  @brief     StencilOperatorInfo https://www.asawicki.info/news_1654_stencil_test_explained_using_code
	*****************************************************************************/
	struct StencilOperatorInfo
	{
		CompareOperator CompareOperator   = CompareOperator::Always; // Stencil test
		StencilOperator FailOperator      = StencilOperator::Keep;   // Failed stencil action
		StencilOperator PassOperator      = StencilOperator::Keep;   // Succeed stencil action
		StencilOperator DepthFailOperator = StencilOperator::Keep;   // Failed depth test action
		gu::uint32   Reference         = 0;
		StencilOperatorInfo() = default;
	};

	/****************************************************************************
	*				  			DepthStencilProperty
	*************************************************************************//**
	*  @class     DepthStencilProperty
	*  @brief     When the depthStencilState class is used, you use the structure. 
	*****************************************************************************/
	struct DepthStencilProperty
	{
		bool                UseDepthTest       = true;                       // Use depth test
		bool                DepthWriteEnable   = true;                       // Enable to write depth
		bool                StenciWriteEnable  = false;                      // Enable to write Stencil (stencil test: 描画マスクみたいなやつ)  
		bool                UseDepthBoundsTest = false;                      // Use depth bounds test (vulkan api only)https://shikihuiku.wordpress.com/2012/06/27/depth-bounds-test1/
		float               MinDepthBounds     = 0.0f;                       // Min depth bounds test region
		float               MaxDepthBounds     = 0.0f;                       // Max depth bounds test region
		CompareOperator     DepthOperator      = CompareOperator::LessEqual; // Depth test operator
		StencilOperatorInfo Front              = StencilOperatorInfo();      // Use depth test and stencil test results for pixels with surface normals facing the camera
		StencilOperatorInfo Back               = StencilOperatorInfo();      // Use depth test and stencil test results for pixels where the surface normal is away from the camera
	};

#pragma endregion  DepthStencilState
	#pragma region InputAssemblyState
	/****************************************************************************
	*				  			PrimitiveTopology
	*************************************************************************//**
	/* @brief   隣接する各頂点同士の接続方法を指定します
	*  @note    Triangle ListとStripの描画の違い : https://sorceryforce.net/ja/tips/xna-primitive3d-sequencing-triangle
	*****************************************************************************/
	enum class PrimitiveTopology : gu::uint8
	{
		Undefined     = 0,       //!< 未定義です
		PointList     = 1,       //!< 点群データです. 隣接する頂点同士を接続することは行いません. 
		LineList      = 2,       //!< 隣接する頂点同士を線で結びます. この時, 頂点の再利用は行いません.
		LineStrip     = 3,       //!< 隣接する頂点同士を線で結びます. この時, 頂点の再利用は行うようにします.
		TriangleList  = 4,       //!< 隣接する頂点を三角形で結びます. 3Dメッシュを作成する時のモードです. この時頂点の再利用は行いません. 
		TriangleStrip = 5,       //!< 隣接する頂点を三角形で結びます. 3Dメッシュを作成するときのモードです. この時頂点の再利用を行うため頂点数を減らせますが, 1点を共有するポリゴンの描画は出来ません. http://neareal.net/index.php?ComputerGraphics%2FXNA%2FVertex%2FSimpleVertex_3
		CountOfPrimitiveTopology //!< 種類数/
	};

	/****************************************************************************
	*				  			InputClassification
	*************************************************************************//**
	/* @brief   入力レイアウトが頂点ごとに設定されるものか, インスタンスごとに設定されるものかを指定します
	*****************************************************************************/
	enum class InputClassification : gu::uint8
	{
		PerVertex   = 0, //!< 頂点ごとに指定のInputLayoutElementの要素を適用します
		PerInstance = 1, //!< 描画インスタンスごとに指定のInputLayoutElementの要素を適用します.
	};

	#pragma endregion InputAssemblyState

	#pragma region GPUResource
	enum class RootSignatureType : gu::uint8
	{
		Rasterize,
		RayTracingGlobal,
		RayTracingLocal
	};

	/****************************************************************************
	*				  			ResourceDimension
	*************************************************************************//**
	/* @brief   GPUリソースの形式を指定します. バッファなのかn次元テクスチャなのか
	*****************************************************************************/
	enum class ResourceDimension : gu::uint8
	{
		Buffer,    //!< バッファとして使用します
		Texture1D, //!< 1次元テクスチャ
		Texture2D, //!< 2次元テクスチャ
		Texture3D  //!< 3次元テクスチャ
	};

	/****************************************************************************
	*				  		ResourceType
	*************************************************************************//**
	/* @brief   ResourceDimensionの詳細を記述します. 
	*****************************************************************************/
	enum class ResourceType : gu::uint8
	{
		Unknown                           = 0,  //!< 未知のGPUリソースの種類です
		Buffer                            = 1,  //!< バッファとして使用します
		Texture1D                         = 2,  //!< 1次元のテクスチャとして使用します
		Texture1DArray                    = 3,  //!< 1次元のテクスチャ配列として使用します
		Texture2D                         = 4,  //!< 2次元のテクスチャとして使用します
		Texture2DArray                    = 5,  //!< 2次元のテクスチャ配列として使用します
		Texture2DMultiSample              = 6,  //!< 2次元のテクスチャ(マルチサンプリングを使用)を使用します
		Texture2DArrayMultiSample         = 7,  //!< 2次元のテクスチャ配列(マルチサンプリングを使用)を使用します
		Texture3D                         = 8,  //!< 3次元のボリュームテクスチャとして使用します
		TextureCube                       = 9,  //!< キューブマップテクスチャを使用します
		TextureCubeArray                  = 10, //!< キューブマップテクスチャ配列として使用します
		RaytracingAccelerationStructure   = 11  //!< レイトレーシングの加速構造として使用します.
	};

	/****************************************************************************
	*				  			ResourceState
	*************************************************************************//**
	/*  @brief GPUリソースの状態を定義します. @n
	*   リソース状態を変更する際は, PushTransitionBarrierとFlushResourceBarrier(コマンドリスト)を使用してください.  
	*****************************************************************************/
	enum class ResourceState : gu::uint8
	{
		/// 異なるコマンドリストタイプ(Graphics, Compute, Copy)をまたぐリソースをアクセスするために使用します
		Common,  

		/// Uploadヒープに対して必要とされるリソースです
		GeneralRead,

		/// 3Dパイプラインによって頂点バッファ, または定数バッファとしてアクセスされる場合に使用します
		VertexAndConstantBuffer,

		/// 3Dパイプラインによってインデックスバッファとしてアクセスされる場合に使用します
		IndexBuffer,

		/// テクスチャの読み書きをするためのUnorderedAccessビューに対して使用できるステートです
		UnorderedAccess,

		/// レンダーターゲットとして使用されるリソースで, 書き込み専用のステートです
		RenderTarget,

		/// 深度ステンシルビューへの書き込みが可能なときに使用するステートです
		DepthStencil,

		/// コピー操作で出力として使用されるリソースで, 書き込み専用のステートです
		CopyDestination,

		/// コピー操作で入力として使用されるリソースで, 読み込み専用のステートです
		CopySource,

		/// Commonと同義ではありますが, スワップチェインを使う際に使用することを想定しています. 
		Present,

		/// インダイレクト引数として使用されるリソースで, 読み込み専用のステートです
		Indirected,

		/// RayTracingのAcceleration Structureで使用するステートです
		RayTracingAccelerationStructure,

		/// Variable Rate Shadingを使った画像リソースであることを示します. 
		ShadingRateSource,

		/// Pixelシェーダで使用されるリソースで, 読み込み専用のステートです.
		PixelShader,

		/// Pixelシェーダ以外で使用されるリソースで, 読み込み専用のステートです
		NonPixelShader,

		/// ストリーム出力で使用されるリソースで, 書き込み専用のステートです
		StreamOut,

		/// Resolve操作で出力として使用されるリソースです. 
		ResolveDestination,

		/// Resolve操作の入力として使用されるリソースです
		ResolveSource,

		/// ResourceStateの個数
		CountOf
	};

	/****************************************************************************
	*				  			MemoryHeap
	*************************************************************************//**
	/* @brief  GPUのメモリを格納するヒープ領域の種類を決定します. 
	*****************************************************************************/
	enum class MemoryHeap : gu::uint8
	{
		Default, //!< GPU側でのみ見ることが出来るメモリ領域です. この場合, Mapでの読み書きは行えませんが, GPU内の処理で完結する作業は高速です
		Upload,  //!< CPUからGPUにメモリを転送するのに使用します. Mapを使ってCPUが一度書き込み, GPUで一度読み取るデータに最適
		Readback,//!< CPUからの読み取りを行う時に使用する, (GPUが書き込みを行い, それをCPUが読み取るデータに最適)
		Custom   //!< DirectX12用にカスタマイズするヒープ領域
	};

	/****************************************************************************
	*				  			DescriptorHeapType
	*************************************************************************//**
	/* @brief 　リソースを読み取る際のビューについて, ディスクリプタを貯めておくヒープ領域を指定する
	*****************************************************************************/
	enum class DescriptorHeapType : gu::uint8
	{
		CBV,         //!< Constant Buffer View (ユーザーが設定するGPUパラメータなどを設定します. )
		SRV,         //!< Shader Resource View (テクスチャなどを読み取るときに使用します)
		UAV,         //!< Unordered Access View (テクスチャの読み書きが入るときに使用します)  
		SAMPLER,     //!< Dynamic Sampler State (テクスチャのサンプリングする方法を定義するときに使用します (普段はStatic Samplerを使用するため, あまり使用しません))

		RTV,         //!< Render Target View (Swapchainに描画するときに使用します. (DirectX12だけで使用することが可能です))
		DSV,         //!< Depth Stencil View (深度テクスチャを読み書きする際に使用します.  (DirectX12だけで使用することが可能です)
	};

	struct DefaultHeapCount
	{
		gu::uint32 CBVDescCount = 0;     //!< Constant Buffer Viewのディスクリプタヒープの個数
		gu::uint32 SRVDescCount = 0;     //!< Shader Resource View のディスクリプタヒープの個数
		gu::uint32 UAVDescCount = 0;     //!< Unordered Access Viewのディスクリプタヒープの個数
		gu::uint32 SamplerDescCount = 0; //!< Dynamic Sampler Stateのディスクリプタヒープの個数
		gu::uint32 RTVDescCount = 0;     //!< Render Target Viewのディスクリプタヒープの個数
		gu::uint32 DSVDescCount = 0;     //!< Depth Stencil Viewのディスクリプタヒープの個数
	};

	/****************************************************************************
	*				  			ResourceViewType
	*************************************************************************//**
	/* @brief     How to read gpu resource buffer
	*****************************************************************************/
	enum class ResourceViewType : gu::uint8
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

	enum class BindlessResourceType
	{
		Unsupported = 0,
		OnlyRayTracing = 1,
		AllShaderTypes = 2,
	};

	#pragma region GPUBuffer
	/****************************************************************************
	*				  			BufferCreateFlags
	*************************************************************************//**
	/*  @brief  バッファの使用方法について定義しています
	*****************************************************************************/
	enum class BufferCreateFlags : gu::uint32
	{
		None = 0,

		/// バッファへの書き込みは一度だけ行われる
		Static                = 1 << 0,

		/// バッファは時々書き込まれ, GPUは読み取り専用でCPUは書き込み専用となる. 
		Dynamic               = 1 << 1,

		/// バッファの寿命は1フレーム. フレームごとに書き込むか新しいものを作るかの対応が必要となる.
		Volatile              = 1 << 2,

		///　Unorder access viewを作成する
		UnorderedAccess       = 1 << 3,

		/// byte adress buffer : バイト単位でインデックスが作成される読み取り専用バッファ (基本uint32)
		ByteAddress           = 1 << 4,

		/// コピー元として使用するGPUバッファ
		SourceCopy            = 1 << 5,

		/// Stream出力ターゲットとして使用するGPUバッファ
		StreamOutput          = 1 << 6,

		/// DispatchIndirectまたはDrawIndirectが使用する引数を含むバッファを作成する
		DrawIndirect          = 1 << 7,

		/// シェーダ・リソースとしてバインドできるバッファを作成します。 
		/// 頂点バッファのような、通常はシェーダリソースとして使用されないバッファにのみ必要です。
		ShaderResource        = 1 << 8,

		/// CPUに直接アクセス可能なバッファ
		CPUAccessible         = 1 << 9,

		/// バッファは高速VRAMに置く
		FastVRAM              = 1 << 10,

		/// 外部のRHIやプロセスと共有できるバッファを作成する
		Shared                = 1 << 11,

		/// RayTracing用のAcceleration structureを作成する
		AccelerationStructure = 1 << 12,

		/// 頂点バッファとして使用する
		VertexBuffer          = 1 << 13,

		/// インデックスバッファとして使用する
		IndexBuffer           = 1 << 14,

		/// 構造化バッファ
		StructuredBuffer      = 1 << 15,

		/// 定数バッファ
		ConstantBuffer        = 1 << 16,

		/// バッファメモリは、ドライバのエイリアシングによって共有されるのではなく、複数のGPUに対して個別に割り当てられる
		MultiGPUAllocate      = 1 << 17,

		/// バッファをレイトレーシングのアクセラレーション構造を構築するためのスクラッチバッファとして使用できるようにします
		/// バッファのアライメントのみを変更し、他のフラグと組み合わせることができます
		Scratch               = 1 << 18,

		/// レンダーターゲット
		RenderTarget          = 1 << 19, // allow render target

		/// デプスステンシル
		DepthStencil          = 1 << 20, // allow depth stencil

		/// バッファは時々書き込まれ, GPUは読み取り専用でCPUは書き込み専用となる. 
		AnyDynamic = (Dynamic | Volatile),
	};

	ENUM_CLASS_FLAGS(BufferCreateFlags);

	/****************************************************************************
	*				  			GPUBufferInfo
	*************************************************************************//**
	/*  @brief     GPUバッファを作成するために必要な情報をまとめた構造体です
	*****************************************************************************/
	struct GPUBufferMetaData
	{
	public:
		#pragma region Public Member Variables
		/*! @brief 1要素に必要なバイトサイズ*/
		gu::uint32 Stride = 0;
		
		/*! @brief 要素の個数*/
		gu::uint32 Count  = 0;

		/*! @brief GPUリソースの使用方法*/
		BufferCreateFlags Usage = BufferCreateFlags::None;

		/*! @brief ピクセルのフォーマット.基本的には使用しないが, Vulkanのビュー指定に必要となる場合がある */
		PixelFormat Format = PixelFormat::Unknown;

		/*! @brief GPUバッファを作成するときの最初のCPUメモリの領域 */
		void* InitData = nullptr;

		/*! @brief GPUリソースの種類*/
		ResourceType ResourceType = ResourceType::Unknown;

		/*! @brief 現在のリソースのバリア状態*/
		ResourceState State = ResourceState::GeneralRead;
		
		/*! @brief GPUのメモリを格納するヒープ領域の種類を決定します. */
		MemoryHeap HeapType = MemoryHeap::Default;
		
		#pragma endregion 
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief  　　CPUにアクセス可能なGPUBufferリソースかを判定します
		*  @return    bool : trueでCPUアクセス可能
		*************************************************************************/
		__forceinline bool IsCPUAccessible() const
		{
			return HeapType == MemoryHeap::Upload || HeapType == MemoryHeap::Readback
			|| (HeapType == MemoryHeap::Custom);
		}

		/*!**********************************************************************
		*  @brief  　　GPUBufferの全体に占めるバイト数を取得します
		*  @return    gu::uint64 バッファのサイズ
		*************************************************************************/
		__forceinline gu::uint64 GetTotalByte() const { return static_cast<gu::uint64>(Stride * Count); }
		#pragma endregion 

		#pragma region Public Constructor and Destructor
		/*! @brief デフォルトコンストラクタ*/
		GPUBufferMetaData() = default;

		/*! @brief 直接Buffer情報から生成するコンストラクタ*/
		GPUBufferMetaData(gu::uint32 stride, gu::uint32 count, core::BufferCreateFlags usage, ResourceState layout, MemoryHeap heapType, void* initData = nullptr);
		#pragma endregion 

		#pragma region Static Function
		/*!**********************************************************************
		*  @brief  CPUからGPUにメモリを転送するのに使用します. Mapを使ってCPUが一度書き込み, GPUで一度読み取るデータに最適
		*************************************************************************/
		static GPUBufferMetaData UploadBuffer  (const gu::uint32 stride, const gu::uint32 count, const MemoryHeap heap = MemoryHeap::Upload, void* initData = nullptr);
		
		/*!**********************************************************************
		*  @brief  GPU側でのみ見ることが出来るメモリ領域です. この場合, Mapでの読み書きは行えませんが, GPU内の処理で完結する作業は高速です
		*************************************************************************/
		static GPUBufferMetaData DefaultBuffer (const gu::uint32 stride, const gu::uint32 count, const MemoryHeap heap = MemoryHeap::Default, void* initData = nullptr);
		
		/*!**********************************************************************
		*  @brief  定数用のバッファです. これは256 byteのアラインメントが発生します
		*************************************************************************/
		static GPUBufferMetaData ConstantBuffer(const gu::uint32 stride, const gu::uint32 count, const MemoryHeap heap = MemoryHeap::Upload , const ResourceState state = ResourceState::Common, void* initData = nullptr); // auto alignment 
		
		/*!**********************************************************************
		*  @brief  頂点データ用のバッファです
		*************************************************************************/
		static GPUBufferMetaData VertexBuffer  (const gu::uint32 stride, const gu::uint32 count, const MemoryHeap heap = MemoryHeap::Default, const ResourceState state = ResourceState::GeneralRead, void* initData = nullptr);
		
		/*!**********************************************************************
		*  @brief  インデックスデータ用のバッファです
		*************************************************************************/
		static GPUBufferMetaData IndexBuffer   (const gu::uint32 stride, const gu::uint32 count, const MemoryHeap heap = MemoryHeap::Default, const ResourceState state = ResourceState::Common, void* initData = nullptr);
		#pragma endregion 
	private:
		gu::uint64 CalcConstantBufferByteSize(const gu::uint64 byteSize) { return (byteSize + 255) & ~255; }
	};
	#pragma endregion

	#pragma region GPUTexture
	
	/****************************************************************************
	*				  			TextureCreateFlags
	*************************************************************************//**
	/*  @brief  テクスチャの使用方法について定義しています
	*****************************************************************************/
	enum class TextureCreateFlags : gu::uint64
	{
		None = 0,

		/// レンダーターゲットとして使用可能なテクスチャです
		RenderTargetable       = 1ull << 0,

		/// マルチサンプルなテクスチャと通常テクスチャをブレンドする際にResolveを行うテクスチャです
		ResolveTargetable      = 1ull << 1,

		/// Depth Stencilとして使用可能なテクスチャです
		DepthStencilTargetable    = 1ull << 2,

		/// Shader resourceとして使用可能なテクスチャです
		ShaderResource            = 1ull << 3,

		/// sRGBのGamma空間として符号化されているテクスチャです.  
		SRGB                      = 1ull << 4,

		/// CPUによって記述可能なテクスチャデータです
		CPUWritable               = 1ull << 5,

		/// タイリングされていないテクスチャとして使用
		NoTiling                  = 1ull << 6,

		/// 動画用のデコーダとして使用します
		VideoDecode               = 1ull << 7,

		/// テクスチャが書き込まれる可能性があるときに使用します
		Dynamic                   = 1ull << 8,

		/// レンダーパスのアタッチメントとして使用されます
		InputAttachmentRead       = 1ull << 9,

		///視線の中心に高い解像度をおくテクスチャのエンコーディングを使用する際に使用
		Foveation                 = 1ull << 10,

		/// Volume Textureのタイリング用
		Tiling3D                  = 1ull << 11,

		/// GPUやCPUのバックエンドを持たず, TileBasedDifferred Renderingのタイルメモリ上のみに存在します. 
		Memoryless                = 1ull << 12,

		/// ShaderResourceViewを作成しないようにします
		DisableSRVCreation        = 1ull << 13,

		/// Unordered access viewを使用可能です
		UnorderedAccess           = 1ull << 14,

		/// スワップチェインでバックバッファとして使用可能です
		Presentable               = 1ull << 15,

		/// CPUのReadBackリソースとして使用可能です.
		CPUReadback               = 1ull << 16,

		/// FastVRAMを使用する
		FastVRAM                  = 1ull << 17,

		/// 仮想メモリに割り当てられるテクスチャ
		Virtual                   = 1ull << 18,

		/// レンダーターゲットは高速なClearを使用しない
		NoFastClear               = 1ull << 19,

		/// DepthStencilのResolve用のテクスチャです
		DepthStencilResolveTarget = 1ull << 20,

		/// このテクスチャがストリーミング可能な2Dテクスチャであり、テクスチャストリーミングプールのバジェットにカウントされるべきであることを示すために使用されるフラグ
		Streamable                = 1ull << 21,

		/// Atomic操作が使用可能 (64bit)
		Atomic64Compatible        = 1ull << 22,

		/// Atomic操作が使用可能
		AtomicCompatible          = 1ull << 23,

		/// 外部アクセスが使用可能
		External                  = 1ull << 24,

		/// 予約リソース
		ReservedResource          = 1ull << 25
	};

	ENUM_CLASS_FLAGS(TextureCreateFlags);

	/****************************************************************************
	*				  			GPUTextureMetaData
	*************************************************************************//**
	/*  @brief   テクスチャの作成情報を一通り管理している構造体
	*****************************************************************************/
	struct GPUTextureMetaData
	{
	public:
		#pragma region Public Member Variables
		/*! @brief 画面クリア時の初期化で塗りつぶされる色 (RenderTarget, DepthStencilなどに使用します.)*/
		ClearValue ClearColor = ClearValue();

		/*! @brief GPUリソースの使用方法を定義します.*/
		TextureCreateFlags Usage = TextureCreateFlags::None;

		/*! @brief テクスチャの幅です. (pixel単位で指定します)*/
		gu::uint32 Width = 1;

		/*! @brief テクスチャの高さです. (pixel単位で指定します)*/
		gu::uint32 Height = 1;

		/*! @brief 2Dテクスチャの配列であればArraySizeが適用され, 3Dテクスチャの場合はDepthが適用されます. */
		gu::uint16 DepthOrArraySize = 1;

		/*! @brief ミップマップの個数 (2を指定すると, ミップマップの値が0と1のものを使用するという意味です)*/
		gu::uint8  MipMapLevels = 1;

		/*! @brief GPUリソースの状態を定義します. (状態変更はコマンドリストのPushTransitionBarrierとFlushResourceBarriersを使用)*/
		ResourceState State = ResourceState::GeneralRead; 

		/*! @brief 1ピクセルの型定義*/
		PixelFormat PixelFormat = PixelFormat::Unknown;

		/*! @brief サンプリングの個数を定義します*/
		MultiSample Sample = MultiSample::Count1;

		/*! @brief GPUリソースの次元定義 (バッファなのかn次元テクスチャなのか)*/
		ResourceDimension Dimension = ResourceDimension::Texture2D;

		/*! @brief ResourceDimensionの詳細です*/
		ResourceType ResourceType = ResourceType::Unknown;

		/*! @brief GPUのメモリを格納するヒープ領域の種類を決定します. */
		MemoryHeap HeapType = MemoryHeap::Default;
		#pragma endregion

		#pragma region Static Function
		/*!**********************************************************************
		*  @brief  　 1次元のテクスチャの基本設定を返します.  
		*  @param[in] const gu::uint32               テクスチャの横幅 [pixel]
		*  @param[in] const core::PixelFormal        1ピクセルあたりの型を定義します
		*  @param[in] const gu::uint8                ミップマップ (Defaultは1)
		*  @param[in] const core::TextureCreateFlags テクスチャの使用方法 (DefaultはNone)
		*  @return    GPUTextureMetaData
		*************************************************************************/
		static GPUTextureMetaData Texture1D 
		(
			const gu::uint32 width, 
			const core::PixelFormat format, 
			const gu::uint8 mipLevels = 1, 
			const core::TextureCreateFlags usage = core::TextureCreateFlags::None
		);

		/*!**********************************************************************
		*  @brief  　 1次元のテクスチャ配列の基本設定を返します.
		*  @param[in] const gu::uint32               テクスチャの横幅 [pixel]
		*  @param[in] const gu::uint16               テクスチャの配列数
		*  @param[in] const core::PixelFormal        1ピクセルあたりの型を定義します
		*  @param[in] const gu::uint8                ミップマップ (Defaultは1)
		*  @param[in] const core::TextureCreateFlags テクスチャの使用方法 (DefaultはNone)
		*  @return    GPUTextureMetaData
		*************************************************************************/
		static GPUTextureMetaData Texture1DArray 
		(
			const gu::uint32 width, 
			const gu::uint16 length, 
			const core::PixelFormat format,
			const gu::uint8 mipLevels = 1, 
			const core::TextureCreateFlags usage = core::TextureCreateFlags::None
		);

		/*!**********************************************************************
		*  @brief  　 2次元のテクスチャの基本設定を返します.
		*  @param[in] const gu::uint32               テクスチャの横幅 [pixel]
		*  @param[in] const gu::uint32               テクスチャの高さ [pixel]
		*  @param[in] const core::PixelFormal        1ピクセルあたりの型を定義します
		*  @param[in] const gu::uint8                ミップマップ (Defaultは1)
		*  @param[in] const core::TextureCreateFlags テクスチャの使用方法 (DefaultはNone)
		*  @return    GPUTextureMetaData
		*************************************************************************/
		static GPUTextureMetaData Texture2D
		(
			const gu::uint32 width,
			const gu::uint32 height, 
			const core::PixelFormat format,
			const gu::uint8 mipLevels = 1, 
			const core::TextureCreateFlags usage = core::TextureCreateFlags::None
		);

		/*!**********************************************************************
		*  @brief  　 2次元のテクスチャ配列の基本設定を返します.
		*  @param[in] const gu::uint32               テクスチャの横幅 [pixel]
		*  @param[in] const gu::uint32               テクスチャの高さ [pixel]
		*  @param[in] const gu::uint16               テクスチャの配列数
		*  @param[in] const core::PixelFormal        1ピクセルあたりの型を定義します
		*  @param[in] const gu::uint8                ミップマップ (Defaultは1)
		*  @param[in] const core::TextureCreateFlags テクスチャの使用方法 (DefaultはNone)
		*  @return    GPUTextureMetaData
		*************************************************************************/
		static GPUTextureMetaData Texture2DArray
		(
			const gu::uint32 width, 
			const gu::uint32 height, 
			const gu::uint16 length, 
			const core::PixelFormat format, 
			const gu::uint8 mipLevels = 1, 
			const core::TextureCreateFlags usage = core::TextureCreateFlags::None
		);

		/*!**********************************************************************
		*  @brief  　 3次元のテクスチャの基本設定を返します.
		*  @param[in] const gu::uint32               テクスチャの横幅 [pixel]
		*  @param[in] const gu::uint32               テクスチャの高さ [pixel]
		*  @param[in] const gu::uint16               テクスチャの奥行 [pixel]
		*  @param[in] const core::PixelFormal        1ピクセルあたりの型を定義します
		*  @param[in] const gu::uint8                ミップマップ (Defaultは1)
		*  @param[in] const core::TextureCreateFlags テクスチャの使用方法 (DefaultはNone)
		*  @return    GPUTextureMetaData
		*************************************************************************/
		static GPUTextureMetaData Texture3D
		(
			const gu::uint32 width, 
			const gu::uint32 height, 
			const gu::uint16 depth, 
			const core::PixelFormat format, 
			const gu::uint8 mipLevels = 1,
			const core::TextureCreateFlags usage = core::TextureCreateFlags::None
		);

		/*!**********************************************************************
		*  @brief  　 2次元のテクスチャ(マルチサンプリングの指定あり)の基本設定を返します.
		*  @param[in] const gu::uint32               テクスチャの横幅 [pixel]
		*  @param[in] const gu::uint32               テクスチャの高さ [pixel]
		*  @param[in] const core::PixelFormal        1ピクセルあたりの型を定義します
		*  @param[in] const core::MultiSample        何ピクセルのサンプリングを行うか
		*  @param[in] const gu::uint8                ミップマップ (Defaultは1)
		*  @param[in] const core::TextureCreateFlags テクスチャの使用方法 (DefaultはNone)
		*  @return    GPUTextureMetaData
		*************************************************************************/
		static GPUTextureMetaData Texture2DMultiSample
		(
			const gu::uint32 width, 
			const gu::uint32 height,
			const core::PixelFormat format, 
			const core::MultiSample sample, 
			const gu::uint8 mipLevels = 1, 
			const core::TextureCreateFlags usage = core::TextureCreateFlags::None
		);

		/*!**********************************************************************
		*  @brief  　 2次元のテクスチャ配列(マルチサンプリングの指定あり)の基本設定を返します.
		*  @param[in] const gu::uint32               テクスチャの横幅 [pixel]
		*  @param[in] const gu::uint32               テクスチャの高さ [pixel]
		*  @param[in] const gu::uint16               テクスチャ配列数
		*  @param[in] const core::PixelFormal        1ピクセルあたりの型を定義します
		*  @param[in] const core::MultiSample        何ピクセルのサンプリングを行うか
		*  @param[in] const gu::uint8                ミップマップ (Defaultは1)
		*  @param[in] const core::TextureCreateFlags テクスチャの使用方法 (DefaultはNone)
		*  @return    GPUTextureMetaData
		*************************************************************************/
		static GPUTextureMetaData Texture2DArrayMultiSample
		(
			const gu::uint32 width, 
			const gu::uint32 height, 
			const gu::uint16 length, 
			const core::PixelFormat format, 
			const core::MultiSample sample, 
			const gu::uint8 mipLevels = 1,
			const core::TextureCreateFlags usage = core::TextureCreateFlags::None
		);

		/*!**********************************************************************
		*  @brief  　 キューブマップの基本設定を返します.
		*  @param[in] const gu::uint32               テクスチャの横幅 [pixel]
		*  @param[in] const gu::uint32               テクスチャの高さ [pixel]
		*  @param[in] const core::PixelFormal        1ピクセルあたりの型を定義します
		*  @param[in] const gu::uint8                ミップマップ (Defaultは1)
		*  @param[in] const core::TextureCreateFlags テクスチャの使用方法 (DefaultはNone)
		*  @return    GPUTextureMetaData
		*************************************************************************/
		static GPUTextureMetaData CubeMap
		(
			const gu::uint32 width,
			const gu::uint32 height, 
			const core::PixelFormat format, 
			const gu::uint8 mipLevels = 1,
			const core::TextureCreateFlags usage = core::TextureCreateFlags::None
		);

		/*!**********************************************************************
		*  @brief  　 キューブマップ配列の基本設定を返します.
		*  @param[in] const gu::uint32               テクスチャの横幅 [pixel]
		*  @param[in] const gu::uint32               テクスチャの高さ [pixel]
		*  @param[in] const gu::uint16               テクスチャ配列数
		*  @param[in] const core::PixelFormal        1ピクセルあたりの型を定義します
		*  @param[in] const gu::uint8                ミップマップ (Defaultは1)
		*  @param[in] const core::TextureCreateFlags テクスチャの使用方法 (DefaultはNone)
		*  @return    GPUTextureMetaData
		*************************************************************************/
		static GPUTextureMetaData CubeMapArray
		(
			const gu::uint32 width, 
			const gu::uint32 height,
			const gu::uint16 length, 
			const core::PixelFormat format, 
			const gu::uint8 mipLevels = 1, 
			const core::TextureCreateFlags usage = core::TextureCreateFlags::None
		);

		/*!**********************************************************************
		*  @brief  　 レンダーターゲットの基本設定を返します.
		*  @param[in] const gu::uint32               テクスチャの横幅 [pixel]
		*  @param[in] const gu::uint32               テクスチャの高さ [pixel]
		*  @param[in] const core::PixelFormal        1ピクセルあたりの型を定義します
		*  @param[in] const core::ClearValue         クリアカラー
		*  @return    GPUTextureMetaData
		*************************************************************************/
		static GPUTextureMetaData RenderTarget
		(
			const gu::uint32 width, 
			const gu::uint32 height, 
			const core::PixelFormat format, 
			const core::ClearValue& clearValue = core::ClearValue()
		);

		/*!**********************************************************************
		*  @brief  　 レンダーターゲットの基本設定を返します.
		*  @param[in] const gu::uint32               テクスチャの横幅 [pixel]
		*  @param[in] const gu::uint32               テクスチャの高さ [pixel]
		*  @param[in] const core::PixelFormal        1ピクセルあたりの型を定義します
		*  @param[in] const core::MultiSample        何ピクセルのサンプリングを行うか
		*  @param[in] const core::ClearValue         クリアカラー
		*  @return    GPUTextureMetaData
		*************************************************************************/
		static GPUTextureMetaData RenderTargetMultiSample
		(
			const gu::uint32 width, 
			const gu::uint32 height,
			const core::PixelFormat format, 
			const core::MultiSample sample, 
			const core::ClearValue& clearValue = ClearValue()
		);

		/*!**********************************************************************
		*  @brief  　 デプスステンシルの基本設定を返します.
		*  @param[in] const gu::uint32               テクスチャの横幅 [pixel]
		*  @param[in] const gu::uint32               テクスチャの高さ [pixel]
		*  @param[in] const core::PixelFormal        1ピクセルあたりの型を定義します
		*  @param[in] const core::ClearValue         クリアカラー
		*  @return    GPUTextureMetaData
		*************************************************************************/
		static GPUTextureMetaData DepthStencil 
		(
			const gu::uint32 width,
			const gu::uint32 height,
			const core::PixelFormat format,
			const core::ClearValue& clearValue = core::ClearValue()
		);

		/*!**********************************************************************
		*  @brief  　 デプスステンシルの(マルチサンプルの指定あり)基本設定を返します.
		*  @param[in] const gu::uint32               テクスチャの横幅 [pixel]
		*  @param[in] const gu::uint32               テクスチャの高さ [pixel]
		*  @param[in] const core::PixelFormal        1ピクセルあたりの型を定義します
		*  @param[in] const core::MultiSample        何ピクセルのサンプリングを行うか
		*  @param[in] const core::ClearValue         クリアカラー
		*  @return    GPUTextureMetaData
		*************************************************************************/
		static GPUTextureMetaData DepthStencilMultiSample 
		(
			const gu::uint32 width, 
			const gu::uint32 height, 
			const core::PixelFormat format, 
			const core::MultiSample sample, 
			const core::ClearValue& clearValue = ClearValue()
		);

		#pragma endregion 

	private:
		inline gu::uint64 CalculateByteSize() { 
			return  Width * Height * (Dimension == ResourceDimension::Texture3D ? DepthOrArraySize : 1) * PixelFormatInfo::GetConst(PixelFormat).BlockBytes * static_cast<gu::uint64>(Sample); 
		}
	};

	/****************************************************************************
	*				  			GPUTextureCopyInfo
	*************************************************************************//**
	/*  @brief   テクスチャをGPU上でコピーする際に使用する構造体
	*****************************************************************************/
	struct GPUTextureCopyInfo
	{
		/*! @brief コピーしたいテクスチャのバイトサイズ. その際, 0を指定すると全体をコピーするという意味になります.*/
		gm::Vector3i<gu::uint32> TexelSize = gm::Vector3i<gu::uint32>::Zero();

		/*! @brief コピー先の読み取り開始テクセル位置. 基本は0に設定されます*/
		gm::Vector3i<gu::uint32> DestinationInitTexelPosition = gm::Vector3i<gu::uint32>::Zero();

		/*! @brief コピー元の読み取り開始テクセル位置. 基本は0に設定されます*/
		gm::Vector3i<gu::uint32> SourceInitTexelPosition = gm::Vector3i<gu::uint32>::Zero();
 
		/*! @brief コピー先のMipmapの初期値*/
		gu::uint32 DestinationInitMipMap = 0;

		/*! @brief コピー元のMipmapの初期値*/
		gu::uint32 SourceInitMipMap      = 0;

		//! @brief コピー先の, テクスチャ配列上のテクスチャ種類の初期値
		gu::uint16 DestinationInitArraySlice = 0;

		//! @brief コピー元の, テクスチャ配列上のテクスチャ種類の初期値
		gu::uint16 SourceInitArraySlice = 0;

		/*! @brief テクスチャ配列の場合におけるテクスチャの種類*/
		gu::uint16 ArraySliceCount = 1;

		/*! @brief ミップマップの個数*/
		gu::uint8  MipMapCount = 1;
	};
	#pragma endregion 

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
		TriangleFrontCounterClockwise = 0x2, // This flag reverses front and back facings, which is useful if the application’s natural winding order differs from the default (left coordinate). 
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
		gu::uint64 AccelerationStructureSize = 0;
		gu::uint64 BuildScratchDataSize      = 0;
		gu::uint64 UpdateScratchDataSize    = 0;
	};
	
	#pragma endregion 

	#pragma endregion GPUResource
#pragma region Render Pass
	enum class AttachmentLoad : gu::uint8
	{
		Clear,    // at the beginning of a render path, erase already existing data with a specific value
		Load,     
		DontCare
	};
	enum class AttachmentStore : gu::uint8
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
		ResourceState   InitialLayout = ResourceState::Common;        // initial resource layout  
		ResourceState   FinalLayout   = ResourceState::Present;       // final desired resource layout
		PixelFormat     Format        = PixelFormat::Unknown;         // pixel format
		MultiSample     SampleCount   = MultiSample::Count1;          // multi sample count (default: single sample count)
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
#pragma region Query
	/****************************************************************************
	*				  			QueryType
	*************************************************************************//**
	*  @enum      QueryType
	*  @brief     GPU情報の計測のために使用するHeapの設定
	*****************************************************************************/
	enum class QueryHeapType : gu::uint8
	{
		Occulusion,         // 遮蔽されていたら0, それ以外は遮蔽されていないことを示す
		TimeStamp,          // GPU, CPUのタイムスタンプに使用します
		PipelineStatistics, // Graphics pipelineの統計を示します
		CopyQueueTimeStamp, // copy queue用のタイムスタンプ
		CountOf,
	};

	/****************************************************************************
	*				  			GPUTimingCalibrationTimestamp
	*************************************************************************//**
	*  @enum      GPUTimingCalibrationTimestamp
	*  @brief     GPUとCPUの時間計測
	*****************************************************************************/
	struct GPUTimingCalibrationTimestamp
	{
		gu::uint64 GPUMicroseconds = 0;
		gu::uint64 CPUMicroseconds = 0;

		GPUTimingCalibrationTimestamp() = default;

		GPUTimingCalibrationTimestamp(const gu::uint64 gpuMicroseconds, const gu::uint64 cpuMicroseconds)
		{
			GPUMicroseconds = gpuMicroseconds;
			CPUMicroseconds = cpuMicroseconds;
		}
	};


#pragma endregion Query
}
#endif