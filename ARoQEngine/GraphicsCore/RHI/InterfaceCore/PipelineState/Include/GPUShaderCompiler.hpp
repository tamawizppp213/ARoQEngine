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
		None = 0,
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