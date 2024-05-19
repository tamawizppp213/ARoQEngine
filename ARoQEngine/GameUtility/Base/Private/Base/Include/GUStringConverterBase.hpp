//////////////////////////////////////////////////////////////////////////////////
///  @file   GUStringConverterBase.hpp
///  @brief  文字列を変換します. @n
/// 　　　　   UTF8 : CodePointを1byte～4byteの可変長バイト列に変換したもの. @n
///          UTF16
///          符号化に対する参考資料: https://contentsviewer.work/Master/Programming/unicode-utf-conversion/unicode-utf-conversion
///  @author toide
///  @date   2024/05/18 1:47:53
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GU_STRING_CONVERTER_BASE_HPP
#define GU_STRING_CONVERTER_BASE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/GUType.hpp"
#include "GameUtility/Base/Include/GUClassUtility.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace gu::details::string
{
	/****************************************************************************
	*				 StringConvertResult 		 
	*************************************************************************//**
	/*  @brief  文字列変更の結果を表す列挙型
	*****************************************************************************/
	enum class StringConvertResult
	{
		Success         = 0, //!< 成功
		SourceExhausted = 1, //!< バッファ終端でマルチバイト文字が途切れた
		TargetExhausted = 2, //!< ターゲットバッファが不足している
		SourceIllegal   = 3, //!< ソースバッファ内に不正な文字が含まれている
	};

	/****************************************************************************
	*				  			 GUStringConverterBase
	*************************************************************************//**
	/*  @brief  文字列を変換するクラスです. @n
	*****************************************************************************/
	class StringConverterBase
	{
	private:
		using UTF8  = uint8;
		using UTF16 = uint16;
		using UTF32 = uint32;

		struct Options
		{
			/*! @brief 変換に使用したソースバッファの要素数*/
			gu::int32 ConvertedSourceLength = 0;

			/*! @brief 変換に使用したターゲットバッファの要素数*/
			gu::int32 ConvertedTargetLength = 0;

			/*! @brief 文字数*/
			gu::int32 CharacterCount = 0;

			/*! @brief 不正文字数*/
			gu::int32 InvalidCharacterCount = 0;

			/*! @brief 不正文字を見つけたときにこの文字に置換する*/
			UTF32 ReplacementChar = 0;
		};

	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief     UTF-8文字列をUTF-16文字列に変換します.
		*  @param[in] const UTF8*      変換元の文字列
		*  @param[in] const gu::uint64 変換元の文字列のサイズ
		*  @param[in] UTF16*           変換先の文字列
		*  @param[in] gu::uint64       変換先の文字列の長さ
		*  @param[inout] Options&      オプション
		*  @return    StringConvertResult 変換結果
		*************************************************************************/
		static StringConvertResult ConvertUTF8ToUTF16(const UTF8* source, uint64 sourceLength, UTF16* target, gu::uint64 targetLength, Options& options);

		/*!**********************************************************************
		*  @brief     UTF-8文字列をUTF-32文字列に変換します.
		*  @param[in] const UTF8*      変換元の文字列
		*  @param[in] const gu::uint64 変換元の文字列のサイズ
		*  @param[in] UTF32*           変換先の文字列
		*  @param[in] gu::uint64       変換先の文字列の長さ
		*  @param[inout] Options&      オプション
		*  @return    StringConvertResult 変換結果
		*************************************************************************/
		static StringConvertResult ConvertUTF8ToUTF32(const UTF8* source, uint64 sourceLength, UTF32* target, gu::uint64 targetLength, Options& options);
		
		/*!**********************************************************************
		*  @brief     UTF-16文字列をUTF-8文字列に変換します.
		*  @param[in] const UTF16*      変換元の文字列
		*  @param[in] const gu::uint64 変換元の文字列のサイズ
		*  @param[in] UTF8*           変換先の文字列
		*  @param[in] gu::uint64       変換先の文字列の長さ
		*  @param[inout] Options&      オプション
		*  @return    StringConvertResult 変換結果
		*************************************************************************/
		static StringConvertResult ConvertUTF16ToUTF8(const UTF16* source, uint64 sourceLength, UTF8* target, gu::uint64 targetLength, Options& options);

		/*!**********************************************************************
		*  @brief     UTF-16文字列をUTF-32文字列に変換します.
		*  @param[in] const UTF16*      変換元の文字列
		*  @param[in] const gu::uint64 変換元の文字列のサイズ
		*  @param[in] UTF32*           変換先の文字列
		*  @param[in] gu::uint64       変換先の文字列の長さ
		*  @param[inout] Options&      オプション
		*  @return    StringConvertResult 変換結果
		*************************************************************************/
		static StringConvertResult ConvertUTF16ToUTF32(const UTF16* source, uint64 sourceLength, UTF32* target, gu::uint64 targetLength, Options& options);
		
		/*!**********************************************************************
		*  @brief     UTF-32文字列をUTF-8文字列に変換します.
		*  @param[in] const UTF32*      変換元の文字列
		*  @param[in] const gu::uint64 変換元の文字列のサイズ
		*  @param[in] UTF8*           変換先の文字列
		*  @param[in] gu::uint64       変換先の文字列の長さ
		*  @param[inout] Options&      オプション
		*  @return    StringConvertResult 変換結果
		*************************************************************************/
		static StringConvertResult ConvertUTF32ToUTF8(const UTF32* source, uint64 sourceLength, UTF8* target, gu::uint64 targetLength, Options& options);

		/*!**********************************************************************
		*  @brief     UTF-32文字列をUTF-8文字列に変換します.
		*  @param[in] const UTF32*     変換元の文字列
		*  @param[in] const gu::uint64 変換元の文字列のサイズ
		*  @param[in] UTF16*           変換先の文字列
		*  @param[in] gu::uint64       変換先の文字列の長さ
		*  @param[inout] Options&      オプション
		*  @return    StringConvertResult 変換結果
		*************************************************************************/
		static StringConvertResult ConvertUTF32ToUTF16(const UTF32* source, uint64 sourceLength, UTF16* target, gu::uint64 targetLength, Options& options);
		#pragma endregion 

		#pragma region Public Property

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

		#pragma region Convert Char Function
		/*!**********************************************************************
		*  @brief        UTF-8文字をUTF-32に変換します. (1文字分). CodePointは, U+0000～U+10FFFFまでの範囲の値です.
		*  @param[in]    const UTF8**    変換元の文字列
		*  @param[in]    const UTF8*     変換元の終端文字
		*  @param[out]   UTF32&          変換先の文字
		*  @param[inout] Options&        オプション
		*  @return       StringConvertResult 変換結果
		*************************************************************************/
		static StringConvertResult CodePointUTF8ToUTF32(const UTF8** sourceStart, const UTF8* sourceEnd, UTF32& target, Options& options);
		
		/*!**********************************************************************
		*  @brief        UTF-16文字をUTF-32に変換します.
		*  @param[in]    const UTF16**    変換元の文字列
		*  @param[in]    
		*  @param[out]   gu::char32*      変換先の文字列
		*  @param[inout] Options&         オプション
		*  @return       StringConvertResult 変換結果
		*************************************************************************/
		static StringConvertResult CodePointUTF16ToUTF32(const UTF16** sourceStart, const UTF16* sourceEnd, UTF32& target, Options& options);

		/*!**********************************************************************
		*  @brief        UTF-32文字をUTF-8に変換します.
		*  @param[in]    const UTF32     変換元の文字
		*  @param[in]    const UTF8**    変換先の文字列ポインタ
		*  @param[out]   const UTF8*     変換先の終端文字
		*  @param[inout] Options&        オプション
		*  @return       StringConvertResult 変換結果
		*************************************************************************/
		static StringConvertResult CodePointUTF32ToUTF8(UTF32 source, UTF8** targetStart, const UTF8* targetEnd, Options& options);

		/*!**********************************************************************
		*  @brief        UTF-32文字をUTF-16に変換します.
		*  @param[in]    const UTF32     変換元の文字
		*  @param[in]    const UTF16**   変換先の文字列ポインタ
		*  @param[out]   const UTF16*    変換先の終端文字
		*  @param[inout] Options&        オプション
		*  @return       StringConvertResult 変換結果
		*************************************************************************/
		static StringConvertResult CodePointUTF32ToUTF16(UTF32 source, UTF16** targetStart, const UTF16* targetEnd, Options& options);
		#pragma endregion Covert Char Function

		/*!**********************************************************************
		*  @brief        適切なUTF8コーデックかどうか
		*  @param[in]    const UTF8* 変換元
		*  @param[in]    const uint64 文字列長さ
		*  @return       bool
		*************************************************************************/
		static bool IsLegalUTF8(const UTF8* source, const uint64 length);

		/*!**********************************************************************
		*  @brief        不正文字を許容するかどうか. (ReplacementCharが0であれば不正文字は一切許容されない)
		*  @param[in]    const Options& option
		*  @return       bool trueで不正文字を許容しない. falseで不正文字を許容
		*************************************************************************/
		__forceinline static bool IsStrictConversion(const Options& option) { return (option.ReplacementChar == 0); }
		#pragma endregion 

		#pragma region Private Property
		/*! @brief 読み取り可能なUTF32の最大値*/
		static constexpr UTF32 MAX_LEGAL_UTF32 = 0x0010FFFF;

		/*! @brief 不正文字が見つかった時の置換文字*/
		static constexpr UTF32 REPLACEMENT_CHAR = 0x0000FFFD;

		/*! @brief UTF32 CodePointを2つの16bitに変換する場合の上位CodeUnitの下限*/
		static constexpr UTF32 SURROGATE_HIGH_START = 0x0000D800;

		/*! @brief UTF32 CodePointを2つの16bitに変換する場合の上位CodeUnitの上限*/
		static constexpr UTF32 SURROGATE_HIGH_END = 0x0000DBFF;

		/*! @brief UTF32 CodePointを2つの16bitに変換する場合の下位CodeUnitの下限*/
		static constexpr UTF32 SURROGATE_LOW_START = 0x0000DC00;

		/*! @brief UTF32 CodePointを2つの16bitに変換する場合の下位CodeUnitの上限*/
		static constexpr UTF32 SURROGATE_LOW_END = 0x0000DFFF;

		/*! @brief UTF32 CodePointを2つの16bitに変換する場合の上位CodeUnitのシフト量*/
		static constexpr UTF32 SURROGATE_HIGH_SHIFT = 10;

		/*! @brief UTF32 CodePointを2つの16bitに変換する場合の下位CodeUnitのベース*/
		static constexpr UTF32 SURROGATE_LOW_BASE = 0x00010000UL;

		/*! @brief UTF32 CodePointを2つの16bitに変換する場合のUTF32→UTF16変換時の下位マスク*/
		static constexpr UTF32 SURROGATE_LOW_MASK = 0x3FFUL;

		/*! @brief Basic Multilingual Plane */
		static constexpr UTF32 MAX_BMP = 0x0000FFFF;
		#pragma endregion 

	};

}
#endif