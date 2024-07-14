//////////////////////////////////////////////////////////////////////////////////
///             @file   DirectX12GPUShaderState.hpp
///             @brief  DirectX12GPUShaderState.hpp
///             @author Toide Yutaro
///             @date   2022_07_02
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef DIRECTX12_GPU_SHADER_STATE_HPP
#define DIRECTX12_GPU_SHADER_STATE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/PipelineState/Include/GPUShaderState.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Core.hpp"
#include <d3d12.h>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
struct IDxcBlob;

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::directX12
{

	/****************************************************************************
	*				  			GPUShaderState
	****************************************************************************/
	/*  @brief シェーダーコードを格納するクラスです.
	*****************************************************************************/
	class GPUShaderState : public rhi::core::GPUShaderState
	{
	public:	
		#pragma region Public Function
		
		/*!**********************************************************************
		*  @brief     HLSLファイルをリアルタイムにコンパイルします. これにより, シェーダーコードが生成されます.
		*  @param[in] const ShaderCompilerOption& option : シェーダーコンパイル時の設定項目
		*  @return    void
		*************************************************************************/
		virtual void Compile(const core::ShaderCompilerOption& option) override;

		// @brief : Offline Compile, already compiled fileName(filePath)
		void LoadBinary(const core::ShaderType type, const gu::tstring& fileName) override ;
		
		#pragma endregion

		#pragma region Public Property

		/*!**********************************************************************
		*  @brief     DirectX12のソースコードのBlobデータを返します.
		*  @param[in] void
		*  @return    BlobComPtr : DirectX12のソースコード
		*************************************************************************/
		BlobComPtr GetSourceBlob() const noexcept { return _sourceBlob; }

		/*!**********************************************************************
		*  @brief     DirectX12のReflectionのBlobデータを返します.
		*  @param[in] void
		*  @return    BlobComPtr : DirectX12のReflectionのBlob
		*************************************************************************/
		BlobComPtr GetReflectionBlob() const noexcept { return _reflectionBlob; }

		/*!**********************************************************************
		*  @brief     DirectX12で使用するShaderByteCodeを返します.
		*  @param[in] void
		*  @return    D3D12_SHADER_BYTECODE
		*************************************************************************/
		D3D12_SHADER_BYTECODE GetDxShader() const { return D3D12_SHADER_BYTECODE(_blobData.Pointer, _blobData.ByteSize); }

		#pragma endregion

		#pragma region Public Constructor and Destructor

		/*! @brief デフォルトコンストラクタ*/
		GPUShaderState() = default;

		/*! @brief デストラクタ*/
		~GPUShaderState() = default;

		/*! @brief 論理デバイスで初期化するコンストラクタ*/
		explicit GPUShaderState(
			const gu::SharedPointer<core::RHIDevice>& device) : rhi::core::GPUShaderState(device) {};

		#pragma endregion

	protected:
		#pragma region Protected Constructor and Destructor
		#pragma endregion

		#pragma region Protected Function
		/*!**********************************************************************
		*  @brief     ShaderModel6.0以上で動作します.DXILという中間言語にコンパイルします 
		*  @param[in] const core::ShaderCompilerOption	
		*  @return    void
		*************************************************************************/
		void DXILCompile(const core::ShaderCompilerOption& option);
		
		/*!**********************************************************************
		*  @brief     ShaderModel6.0以上で動作します. https://simoncoenen.com/blog/programming/graphics/DxcCompiling
		*  @param[in] const core::ShaderCompilerOption& 
		*  @param[out]gu::DynamicArray<const gu::tchar*>& コマンドラインの引数 
		*  @return    void
		*************************************************************************/
		void SetupDXILArguments(const core::ShaderCompilerOption& option, gu::DynamicArray<const gu::tchar*>& arguments);

		BlobComPtr DxCompile(const gu::tstring& fileName, const D3D_SHADER_MACRO* defines, const gu::tstring& entryPoint, const gu::tstring& target);
		
		#pragma endregion

		#pragma region Protected Property
		/*! @brief ソースコードのバッファ*/
		BlobComPtr _sourceBlob = nullptr;

		/*! @brief リフレクションデータを保持するバッファ*/
		BlobComPtr _reflectionBlob = nullptr;
 
		#pragma endregion
	};
}
#endif