//////////////////////////////////////////////////////////////////////////////////
///  @file   GPUShaderCompiler.hpp
///  @brief  temp
///  @author toide
///  @date   2024/07/13 11:48:22
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GPU_SHADER_COMPILER_HPP
#define GPU_SHADER_COMPILER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommonState.hpp"
#include "GameUtility/Container/Include/GUDynamicArray.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace rhi::core
{
	/****************************************************************************
	*				  			ShaderCompilerFlags
	****************************************************************************/
	/* @brief  コンパイルする際の最適化フラグ. 
	*****************************************************************************/
	enum class ShaderCompilerFlags : gu::uint64
	{
		None                 = 0,
		Debug                = 1 << 0, //!< コンパイラに, デバッグファイル/行/型/シンボルを出力コードに挿入するように指示
		SkipValidation       = 1 << 1, //!< シェーダーの検証を無効化します. 
		LowLevelOptimization = 1 << 2, //!< コンパイラは実行時間とコードサイズのバランスを取るように最適化O1)
		WarningAsErrors      = 1 << 3, //!< 警告をエラーとして扱います
		NoWarnings           = 1 << 4, //!< 警告を出力しない
		StrictMode           = 1 << 4, //!< 厳格モードを使用します
		PreferFlowControl    = 1 << 5, //!< 条件分岐などのフロー制御構造を優先します
		AvoidFlowControl     = 1 << 6, //!< 条件分岐などのフロー制御構造を避けます
		GenerateDebugSymbol  = 1 << 7, //!< PDB出力の際に使用します. デバッグシンボルを生成します
		DebugNameForSource   = 1 << 8, //!< デバッグPDBを出力する場合, ハッシュにソースファイルとバイナリを指定します
		DebugNameForBinary   = 1 << 9, //!< デバッグPDBを出力する場合, ハッシュにバイナリファイル名を使用します.
		AutoBindingSpace     = 1 << 10,//!< 自動でBindingSpaceを決定する
	};

	ENUM_CLASS_FLAGS(ShaderCompilerFlags);

	/****************************************************************************
	*				  	   ShaderCompileOption
	****************************************************************************/
	/* @brief  シェーダーコンパイル時の設定項目 (アラインメント対策を行いたいが, これまでの呼び出しと同じ順番で初期化できるようにしている)
	*****************************************************************************/
	struct ShaderCompilerOption
	{
	public:
		static constexpr gu::float32 NEWEST_VERSION = 6.6f;
		#pragma region Public Function

		#pragma endregion 

		#pragma region Public Property
		/*! @brief シェーダーの種類*/
		ShaderType Type = ShaderType::Vertex;

		/*! @brief ファイル名*/
		gu::tstring FileName = SP("");

		/*! @brief エントリポイント*/
		gu::tstring EntryPoint = SP("main");

		/*! @brief includeを行うディレクトリのリスト*/
		gu::DynamicArray<gu::tstring> IncludeDirectories = {};

		/*! @brief defineマクロを使用するリスト*/
		gu::DynamicArray<gu::tstring> Defines = {};

		/*! @brief シェーダーのコンパイル時の設定項目*/
		ShaderCompilerFlags OptimizationFlags = ShaderCompilerFlags::None;

		/*! @brief シェーダーバージョン*/
		gu::float32 Version = NEWEST_VERSION;

		/*! @brief デバッグ内容を出力するディレクトリ*/
		gu::tstring DebugOutputDirectory = SP("");


		#pragma endregion 

		#pragma region Public Operator 

		#pragma endregion 

		#pragma region Public Constructor and Destructor
		
		#pragma endregion 

	protected:
		#pragma region Protected Constructor and Destructor

		#pragma endregion 

		#pragma region Protected Function

		#pragma endregion 

		#pragma region Protected Property

		#pragma endregion

	private:
		#pragma region Private Constructor and Destructor

		#pragma endregion

		#pragma region Private Function

		#pragma endregion 

		#pragma region Private Property

		#pragma endregion 

	};
}

#endif