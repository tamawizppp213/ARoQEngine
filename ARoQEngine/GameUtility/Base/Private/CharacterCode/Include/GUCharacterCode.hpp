//////////////////////////////////////////////////////////////////////////////////
///  @file   GUCharacterCode.hpp
///  @brief  文字列のコードを扱うクラスのヘッダファイル
///  @author toide
///  @date   2024/05/24 0:03:40
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GU_CHARACTER_CODE_HPP
#define GU_CHARACTER_CODE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/GUType.hpp"
#include "GameUtility/Base/Include/GUClassUtility.hpp"
#include "GameUtility/Base/Include/GUSmartPointer.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace gu::details::string
{
	/****************************************************************************
	*				  			 CharacterCodeType
	*************************************************************************//**
	/*   @brief     扱える文字コードの種類を記述します
	*****************************************************************************/
	enum class CharacterCodeType
	{
		Unknown,
		UTF8, 
		UTF16,
		UTF32,
		ShiftJIS,
		ASCII,
		ANSI,
	};

	struct CharacterEncodeResult
	{
		/*! @brief 消費要素数 (入力要素数に対して足りなければ変換エラーがある)*/
		uint64 ConsumedElementCount = 0;

		/*! @brief 変換結果のバイト数*/
		uint64 OutputByteSize = 0;

		/*! @brief 変換結果の文字数*/
		uint64 OutputCharacterCount = 0;
	};


	struct CharacterDecodeResult
	{
		/*! @brief 消費バイト数 (入力バイト数に対して足りなければ変換エラーがある)*/
		uint64 ConsumedByteSize = 0;

		/*! @brief 変換結果のバイト数*/
		uint64 OutputByteSize = 0;

		/*! @brief 変換結果の文字数*/
		uint64 OutputCharacterCount = 0;
	};


	/****************************************************************************
	*				  			   CharacterCode
	*************************************************************************//**
	/*   @brief     文字コードのエンコードやでコードを扱う基底クラス
	*****************************************************************************/
	class CharacterCode : public NonCopyable
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief     環境依存のワイド文字列で使用される文字コードを取得します
		*  @param[in] void
		*  @return    SharedPointer<CharacterCode> 文字コード
		*************************************************************************/
		static SharedPointer<CharacterCode> WideStringCharacterCode();

		/*!**********************************************************************
		*  @brief     TStringで使用される文字コードを取得します
		*  @param[in] void
		*  @return    SharedPointer<CharacterCode> 文字コード
		*************************************************************************/
		static SharedPointer<CharacterCode> TStringCharacterCode();

		/*!**********************************************************************
		*  @brief     指定した文字コードを取得します
		*  @param[in] const CharacterCodeType 文字コードの種類
		*  @return    SharedPointer<CharacterCode> 文字コード
		*************************************************************************/
		static SharedPointer<CharacterCode> GetCharacterCode(const CharacterCodeType codeType, const bool useBOM = false, const bool isBigEndian = true);

		/*!**********************************************************************
		*  @brief     UTF16から指定した文字コードに変換します
		*  @param[in] const uint16* input 変換元の文字列
		*  @param[in] const uint64 inputByteSize 変換元の文字列のバイト数
		*  @param[in] uint8* output 変換後の文字列
		*  @param[in] const uint64 outputElementSize 変換後の文字列の要素数
		*  @param[in] CharacterDecodeResult* result 変換結果
		*  @return    const tchar* 文字コード名
		*************************************************************************/
		virtual bool FromUTF16(const uint16* input, const uint64 inputElementSize, uint8* output, const uint64 outputByteSize, CharacterEncodeResult* result) const = 0;

		/*!**********************************************************************
		*  @brief     UTF32から指定した文字コードに変換します
		*  @param[in] const uint32* input 変換元の文字列
		*  @param[in] const uint64 inputByteSize 変換元の文字列のバイト数
		*  @param[in] uint8* output 変換後の文字列
		*  @param[in] const uint64 outputElementSize 変換後の文字列の要素数
		*  @param[in] CharacterDecodeResult* result 変換結果
		*  @return    const tchar* 文字コード名
		*************************************************************************/
		virtual bool FromUTF32(const uint32* input, const uint64 inputByteSize, uint8* output, const uint64 outputElementSize, CharacterEncodeResult* result) const = 0;

		/*!**********************************************************************
		*  @brief     指定した文字コードからUTF16に変換します
		*  @param[in] const uint8* input 変換元の文字列
		*  @param[in] const uint64 inputByteSize 変換元の文字列のバイト数
		*  @param[in] uint16* output 変換後の文字列
		*  @param[in] const uint64 outputElementSize 変換後の文字列の要素数
		*  @param[in] CharacterDecodeResult* result 変換結果
		*  @return    const tchar* 文字コード名
		*************************************************************************/
		virtual bool ToUTF16(const uint8* input, const uint64 inputByteSize, uint16* output, const uint64 outputElementSize, CharacterDecodeResult* result) const = 0;

		/*!**********************************************************************
		*  @brief     指定した文字コードからUTF32に変換します
		*  @param[in] const uint8* input 変換元の文字列
		*  @param[in] const uint64 inputByteSize 変換元の文字列のバイト数
		*  @param[in] uint32* output 変換後の文字列
		*  @param[in] const uint64 outputElementSize 変換後の文字列の要素数
		*  @param[in] CharacterDecodeResult* result 変換結果
		*  @return    const tchar* 文字コード名
		*************************************************************************/
		virtual bool ToUTF32(const uint8* input, const uint64 inputByteSize, uint32* output, const uint64 outputElementSize, CharacterDecodeResult* result) const = 0;

		#pragma endregion 

		#pragma region Public Property
		/*!**********************************************************************
		*  @brief     文字コード名を取得します
		*  @param[in] void 
		*  @return    const tchar* 文字コード名
		*************************************************************************/
		virtual const tchar* GetName() const = 0;

		/*!**********************************************************************
		*  @brief     可変長なバイト数の文字コードが存在するため, 最小のバイト数を取得します
		*  @param[in] void
		*  @return    const uint8 可変長文字コードの最小バイト数
		*************************************************************************/
		virtual uint8 GetMinByte() const = 0;

		/*!**********************************************************************
		*  @brief     可変長なバイト数の文字コードが存在するため, 最大のバイト数を取得します
		*  @param[in] void
		*  @return    const uint8 可変長文字コードの最大バイト数
		*************************************************************************/
		virtual uint8 GetMaxByte() const = 0;

		/*!**********************************************************************
		*  @brief     文字列長を取得します
		*  @param[in] const void* buffer 文字列
		*  @param[in] const uint64 bufferSize 文字列のバイト数
		*  @return    const uint64 文字数
		*************************************************************************/
		virtual uint64 GetCharacterLength(const void* buffer, const uint64 bufferSize) const = 0;

		/*!**********************************************************************
		*  @brief     エンコードを識別するためのBOM(Byte order mark)を取得します
		*  @param[in] void
		*  @return    const uint8* BOM
		*************************************************************************/
		virtual const uint8* GetBOM() const = 0;

		/*!**********************************************************************
		*  @brief     指定されたバッファの先頭がマルチバイトコードやサロゲートペアの先行文字である場合、追加で読むべき文字列長さを返します。
		*  @param[in] const void* buffer 文字列
		*  @param[in] const uint64 bufferSize 文字列のバイト数
		*  @return    const tchar* 文字コード名
		*************************************************************************/
		virtual uint64 GetReadExtraLength(const void* buffer, const uint64 bufferSize) const = 0;

		#pragma endregion 

		#pragma region Public Operator 

		#pragma endregion 

		#pragma region Public Constructor and Destructor

		#pragma endregion 

	protected:
		#pragma region Protected Constructor and Destructor

		#pragma endregion 

		#pragma region Protected Property

		#pragma endregion

		#pragma region Protected Function

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