//////////////////////////////////////////////////////////////////////////////////
///  @file   DirectX12GPUDiagnosticBuffer.hpp
///  @brief  コマンドキューのGPUの進捗状況をトラッキングする専用バッファ (DirectX12のみで使用します.)
///  @author toide
///  @date   2024/04/06 19:05:15
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef DIRECTX12_GPU_DIAGNOSTIC_BUFFER_HPP
#define DIRECTX12_GPU_DIAGNOSTIC_BUFFER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "DirectX12GPUBuffer.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace rhi::directX12
{
	/****************************************************************************
	*                GPUDiagnosticBuffer
	*************************************************************************//**
	/*  @class     GPUDiagnosticBuffer
	*   @brief     コマンドキューのGPUの進捗状況をトラッキングする専用バッファ (DirectX12のみで使用します.)
	*****************************************************************************/
	class GPUDiagnosticBuffer : public GPUBuffer
	{
	public:
		#pragma region Public Function

		#pragma endregion 

		#pragma region Public Property
		/*!**********************************************************************
		*  @brief  コンテキストのバイトサイズを取得します
		*************************************************************************/
		__forceinline gu::uint32 GetBreadCrumbsContextByteSize() const { return _breadCrumbsContextByteSize; }

		/*!**********************************************************************
		*  @brief  パンくずリストのOffset [Byte]を取得します
		*************************************************************************/
		__forceinline gu::uint32 GetBreadCrumbsOffset() const { return _breadCrumbsOffset; }

		/*!**********************************************************************
		*  @brief  パンくずリスト自体のバイトサイズを取得します
		*************************************************************************/
		__forceinline gu::uint32 GetBreadCrumbsByteSize() const { return _breadCrumbsByteSize; }

		/*!**********************************************************************
		*  @brief  診断情報のOffset [Byte]を取得します
		*************************************************************************/
		__forceinline gu::uint32 GetDiagnosticOffset() const { return _diagnosticOffset; }

		/*!**********************************************************************
		*  @brief  診断情報自体のバイトサイズを取得します
		*************************************************************************/
		__forceinline gu::uint32 GetDiagnosticByteSize() const { return _diagnosticByteSize; }

		/*!**********************************************************************
		*  @brief  コンテキストのバイトサイズを設定します
		*************************************************************************/
		__forceinline void SetBreadCrumbsContextByteSize(const gu::uint32 context) { _breadCrumbsContextByteSize = context; }

		/*!**********************************************************************
		*  @brief  パンくずリストのOffset [Byte]を設定します
		*************************************************************************/
		__forceinline void SetBreadCrumbsOffset(const gu::uint32 offset) { _breadCrumbsOffset = offset; }

		/*!**********************************************************************
		*  @brief  パンくずリスト自体のバイトサイズを取得します
		*************************************************************************/
		__forceinline void SetBreadCrumbsByteSize(const gu::uint32 byteSize) { _breadCrumbsByteSize = byteSize; }

		/*!**********************************************************************
		*  @brief  診断情報のOffset [Byte]を設定します
		*************************************************************************/
		__forceinline void SetDiagnosticOffset(const gu::uint32 offset) { _diagnosticOffset = offset; }

		/*!**********************************************************************
		*  @brief  診断情報自体のバイトサイズを設定します
		*************************************************************************/
		__forceinline void SetDiagnosticByteSize(const gu::uint32 byteSize) { _diagnosticByteSize = byteSize; }
		#pragma endregion 

		#pragma region Public Operator 

		#pragma endregion 

		#pragma region Public Constructor and Destructor
		/*! @brief デフォルトコンストラクタ*/
		GPUDiagnosticBuffer();

		/*! @brief デストラクタ*/
		~GPUDiagnosticBuffer();

		/*! @brief デバイスとバッファ情報をもとに作成されるバッファ*/
		GPUDiagnosticBuffer(const gu::SharedPointer<core::RHIDevice>& device, const core::GPUBufferMetaData& metaData, const gu::tstring& name);
		#pragma endregion 

	protected:
		#pragma region Protected Constructor and Destructor

		#pragma endregion 

		#pragma region Protected Function

		#pragma endregion 

		#pragma region Protected Property
		/*! @brief コンテキストのバイトサイズ*/
		gu::uint32 _breadCrumbsContextByteSize = 0;

		/*! @brief パンくずリストのOffset [Byte]*/
		gu::uint32 _breadCrumbsOffset = 0;

		/*! @brief パンくずリスト自体のByteSize*/
		gu::uint32 _breadCrumbsByteSize = 0;

		/*! @brief 診断情報のOffset byte*/
		gu::uint32 _diagnosticOffset = 0;

		/*! @brief 診断情報自体のBytreSize*/
		gu::uint32 _diagnosticByteSize = 0;
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