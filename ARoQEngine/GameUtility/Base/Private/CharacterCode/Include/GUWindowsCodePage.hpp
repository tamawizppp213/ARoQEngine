//////////////////////////////////////////////////////////////////////////////////
///  @file   GUWindowsCodePage.hpp
///  @brief  Windowsのコードページを扱うクラス
///  @author toide
///  @date   2024/05/27 23:20:06
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GU_WINDOWS_CODE_PAGE_HPP
#define GU_WINDOWS_CODE_PAGE_HPP

#if defined(_WIN32) || defined(_WIN64)

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/GUCharacterCode.hpp"
#define NOMINMAX
#include <Windows.h>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace gu::details::string
{
	/****************************************************************************
	*				  			   GUWindowsCodePage
	*************************************************************************//**
	/*  @brief   Windowsのコードページを扱うクラス
	*****************************************************************************/
	class WindowsCodePage : public CharacterCode
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
		virtual bool FromUTF16(const uint16* input, const uint64 inputElementSize, uint8* output,  const uint64 outputByteSize, CharacterEncodeResult* result) const override;

		/*!**********************************************************************
		*  @brief     UTF32から指定した文字コードに変換します
		*  @param[in] const uint32* input 変換元の文字列
		*  @param[in] const uint64 inputElementSize 変換元の文字列の要素数
		*  @param[in] uint8* output 変換後の文字列
		*  @param[in] const uint64 outputByteSize 変換後の文字列のバイト数
		*  @param[in] CharacterDecodeResult* result 変換結果
		*  @return    const tchar* 文字コード名
		*************************************************************************/
		virtual bool FromUTF32(const uint32* input, const uint64 inputElementSize, uint8* output, const uint64 outputByteSize, CharacterEncodeResult* result) const override;

		/*!**********************************************************************
		*  @brief     指定した文字コードからUTF16に変換します
		*  @param[in] const uint8* input 変換元の文字列
		*  @param[in] const uint64 inputByteSize 変換元の文字列のバイト数
		*  @param[in] uint16* output 変換後の文字列
		*  @param[in] const uint64 outputElementSize 変換後の文字列の要素数
		*  @param[in] CharacterDecodeResult* result 変換結果
		*  @return    const tchar* 文字コード名
		*************************************************************************/
		virtual bool ToUTF16(const uint8* input, const uint64 inputByteSize, uint16* output,  const uint64 outputElementSize, CharacterDecodeResult* result) const override;

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
		*  @brief     文字コード名を取得します[Windows Onlyなため, wcharをそのままtcharに設定しています]
		*  @param[in] void
		*  @return    const tchar* 文字コード名
		*************************************************************************/
		__forceinline virtual const tchar* GetName() const override { return _codePageInfo.CodePageName; }

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
		__forceinline virtual uint8 GetMaxByte() const override { return static_cast<uint8>(_codePageInfo.MaxCharSize); }

		/*!**********************************************************************
		*  @brief     文字列長を取得します
		*  @param[in] const void* buffer 文字列
		*  @param[in] const uint64 bufferSize 文字列のバイト数
		*  @return    const uint64 文字数
		*************************************************************************/
		virtual uint64 GetCharacterLength(const void* buffer, const uint64 bufferSize) const override;

		/*!**********************************************************************
		*  @brief     エンコードを識別するためのBOM(Byte order mark)を取得します
		*  @param[in] void
		*  @return    const uint8* BOM
		*************************************************************************/
		virtual const uint8* GetBOM() const override { return nullptr; };

		/*!**********************************************************************
		*  @brief     指定されたバッファの先頭がマルチバイトコードやサロゲートペアの先行文字である場合、追加で読むべき文字列長さを返します。
		*  @param[in] const void* buffer 文字列
		*  @param[in] const uint64 bufferSize 文字列のバイト数
		*  @return    const tchar* 文字コード名
		*************************************************************************/
		virtual uint64 GetLeadExtraLength([[maybe_unused]] const void* buffer, [[maybe_unused]] const uint64 bufferSize) const override;
		#pragma endregion 

		#pragma region Public Operator 

		#pragma endregion 

		#pragma region Public Constructor and Destructor
		/*! @brief デフォルトコンストラクタ*/
		WindowsCodePage() = default;

		/*! @brief デストラクタ*/
		~WindowsCodePage() = default;

		/*! @brief コードページを指定して*/
		WindowsCodePage(const uint32 codePage);

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
		/*! @brief コードページの情報*/
		CPINFOEX _codePageInfo = {};

		#pragma endregion 

	};
}
#endif
#endif