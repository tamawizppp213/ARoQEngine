//////////////////////////////////////////////////////////////////////////////////
///  @file   GUAscii.hpp
///  @brief  Asciiコードを扱うクラスのヘッダファイル
///  @author toide
///  @date   2024/05/24 0:01:23
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GU_ASCII_HPP
#define GU_ASCII_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GUCharacterCode.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace gu::details::string
{
	/****************************************************************************
	*				  			   GUAsciiEncoder
	*************************************************************************//**
	/*  @class     GUAsciiEncoder
	*   @brief     temp
	*****************************************************************************/
	class Ascii : public CharacterCode
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief     UTF16から指定した文字コードに変換します
		*  @param[in] const uint16* input 変換元の文字列
		*  @param[in] const uint64 inputElementSize 変換元の文字列の要素数
		*  @param[in] uint8* output 変換後の文字列
		*  @param[in] const uint64 outputByteSize 変換後の文字列のバイト数
		*  @param[in] CharacterDecodeResult* result 変換結果
		*  @return    const tchar* 文字コード名
		*************************************************************************/
		virtual bool FromUTF16(const uint16* input, const uint64 inputElementSize, uint8* output, const uint64 outputByteSize, CharacterEncodeResult* result) const override;

		/*!**********************************************************************
		*  @brief     UTF32から指定した文字コードに変換します
		*  @param[in] const uint32* input 変換元の文字列
		*  @param[in] const uint64 inputElementSize 変換元の文字列の要素数
		*  @param[in] uint8* output 変換後の文字列
		*  @param[in] const uint64 outputByteSize 変換後の文字列のバイト数
		*  @param[in] CharacterDecodeResult* result 変換結果
		*  @return    const tchar* 文字コード名
		*************************************************************************/
		virtual bool FromUTF32(const uint32* input, const uint64 inputByteSize, uint8* output, const uint64 outputElementSize, CharacterEncodeResult* result) const override;

		/*!**********************************************************************
		*  @brief     指定した文字コードからUTF16に変換します
		*  @param[in] const uint8* input 変換元の文字列
		*  @param[in] const uint64 inputByteSize 変換元の文字列のバイト数
		*  @param[in] uint16* output 変換後の文字列
		*  @param[in] const uint64 outputElementSize 変換後の文字列の要素数
		*  @param[in] CharacterDecodeResult* result 変換結果
		*  @return    const tchar* 文字コード名
		*************************************************************************/
		virtual bool ToUTF16(const uint8* input, const uint64 inputByteSize, uint16* output, const uint64 outputElementSize, CharacterDecodeResult* result) const override;

		/*!**********************************************************************
		*  @brief     指定した文字コードからUTF32に変換します
		*  @param[in] const uint8* input 変換元の文字列
		*  @param[in] const uint64 inputByteSize 変換元の文字列のバイト数
		*  @param[in] uint32* output 変換後の文字列
		*  @param[in] const uint64 outputElementSize 変換後の文字列の要素数
		*  @param[in] CharacterDecodeResult* result 変換結果
		*  @return    const tchar* 文字コード名
		*************************************************************************/
		virtual bool ToUTF32(const uint8* input, const uint64 inputByteSize, uint32* output, const uint64 outputElementSize, CharacterDecodeResult* result) const override;
		#pragma endregion 

		#pragma region Public Property
		/*!**********************************************************************
		*  @brief     文字コード名を取得します
		*  @param[in] void
		*  @return    const tchar* 文字コード名
		*************************************************************************/
		__forceinline virtual const tchar* GetName() const override { return SP("ASCII"); }

		/*!**********************************************************************
		*  @brief     可変長なバイト数の文字コードが存在するため, 最小のバイト数を取得します
		*  @param[in] void
		*  @return    const uint8 可変長文字コードの最小バイト数
		*************************************************************************/
		__forceinline virtual uint8 GetMinByte() const override { return 1; }

		/*!**********************************************************************
		*  @brief     可変長なバイト数の文字コードが存在するため, 最大のバイト数を取得します
		*  @param[in] void
		*  @return    const uint8 可変長文字コードの最大バイト数
		*************************************************************************/
		__forceinline virtual uint8 GetMaxByte() const override { return 1; }

		/*!**********************************************************************
		*  @brief     文字列長を取得します
		*  @param[in] void
		*  @return    const uint64 文字数
		*************************************************************************/
		__forceinline virtual uint64 GetCharacterLength(const void* buffer, const uint64 bufferSize) const override 
		{
			return buffer ? (uint64)bufferSize : 0;
		};
		
		/*!**********************************************************************
		*  @brief     エンコードを識別するためのBOM(Byte order mark)を取得します
		*  @param[in] void
		*  @return    const uint8* BOM
		*************************************************************************/
		virtual const uint8* GetBOM() const override { return nullptr; };
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