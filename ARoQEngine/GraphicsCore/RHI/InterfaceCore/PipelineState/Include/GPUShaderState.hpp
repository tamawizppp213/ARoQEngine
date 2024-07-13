//////////////////////////////////////////////////////////////////////////////////
///             @file   GPUBlendState.hpp
///             @brief  Blend State
///             @author Toide Yutaro
///             @date   2022_06_28
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GPU_SHADER_STATE_HPP
#define GPU_SHADER_STATE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GPUShaderCompiler.hpp"
#include "GPUState.hpp"


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
	*				  			GPUShaderState
	****************************************************************************/
	/* @brief  シェーダーコードを格納するクラスです.
	*****************************************************************************/
	class GPUShaderState : public GPUState
	{
	public:
		/*! @brief シェーダーモデルの最新バージョン (適宜更新してください)*/
		static constexpr float NEWEST_VERSION = 6.8f;

		#pragma region Public Function
		/*!**********************************************************************
		*  @brief     HLSLファイルをリアルタイムにコンパイルします. これにより, シェーダーコードが生成されます.
		*  @param[in] const ShaderCompilerOption& option : シェーダーコンパイル時の設定項目
		*  @return    void
		*************************************************************************/
		virtual void Compile(const ShaderCompilerOption& option) = 0;

		// @brief : Offline Compile, already compiled fileName(filePath)
		virtual void LoadBinary(const core::ShaderType type, const gu::tstring& fileName) = 0;
		#pragma endregion

		#pragma region Public Property

		/*!**********************************************************************
		*  @brief     シェーダーコードの情報を返します. 
		*  @param[in] void
		*  @return    const ShaderCode& : シェーダーコードの情報
		*************************************************************************/
		__forceinline const ShaderCode& GetBlobData() const noexcept { return _blobData; }
		
		/*!**********************************************************************
		*  @brief     シェーダーコードの先頭ポインタを返します
		*  @param[in] void
		*  @return    void* : シェーダーコードの先頭ポインタ
		*************************************************************************/
		[[nodiscard]] __forceinline void* Get() const noexcept { return _blobData.Pointer; }
		
		/*!**********************************************************************
		*  @brief     シェーダーコード自体のバイト数を返します. 
		*  @param[in] void
		*  @return    gu::uint64 : シェーダーコードのバイト数
		*************************************************************************/
		__forceinline gu::uint64 GetByteSize() const noexcept { return _blobData.ByteSize; }
		
		/*!**********************************************************************
		*  @brief     シェーダーの種類を指定します. 
		*  @param[in] void
		*  @return    ShaderType : シェーダーの種類
		*************************************************************************/
		__forceinline ShaderType GetShaderType() const noexcept { return _shaderType; }
		
		/*!**********************************************************************
		*  @brief     シェーダーのバージョンを指定します. 
		*  @param[in] void
		*  @return    float バージョン
		*************************************************************************/
		__forceinline gu::float32 GetShaderVersion () const noexcept { return _version; }

		/*!**********************************************************************
		*  @brief     設定されたシェーダーがRayTracingで使用可能なシェーダーか
		*  @param[in] void
		*  @return    bool 
		*************************************************************************/
		bool IsRayTracingShader() const;

		#pragma endregion

		#pragma region Public Constructor and Destructor

		#pragma endregion

	protected:
		#pragma region Protected Constructor and Destructor

		/*! @brief デフォルトコンストラクタ*/
		GPUShaderState() = default;

		/*! @brief デストラクタ*/
		~GPUShaderState() = default;

		/*! @brief デバイスを使って作成します. */
		explicit GPUShaderState(
			const gu::SharedPointer<core::RHIDevice>& device) : GPUState(device){};

		#pragma endregion

		#pragma region Protected Function
		gu::tstring GetShaderTypeName(ShaderType shaderType);

		gu::tstring Format(float version);

		/*!**********************************************************************
		*  @brief     有効なシェーダー設定かどうかを返します
		*  @param[in] void
		*  @return    bool
		*************************************************************************/
		bool IsValidShaderType() const noexcept;

		#pragma endregion

		#pragma region Protected Property

		/*! @brief シェーダーコードの情報 */
		ShaderCode _blobData = {};

		/*! @brief シェーダーの現在のバージョン*/
		float _version = 6.0f;

		/*! @brief シェーダーの種類*/
		ShaderType  _shaderType = ShaderType::Vertex;

		#pragma endregion
	};
}
#endif