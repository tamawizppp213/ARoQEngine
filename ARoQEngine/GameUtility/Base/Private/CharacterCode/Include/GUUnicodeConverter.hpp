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
	*				 UnicodeConvertResult 		 
	****************************************************************************/
	/*  @brief  文字列変更の結果を表す列挙型
	*****************************************************************************/
	enum class UnicodeConvertResult
	{
		Success         = 0, //!< 成功
		SourceExhausted = 1, //!< バッファ終端でマルチバイト文字が途切れた
		TargetExhausted = 2, //!< ターゲットバッファが不足している
		SourceIllegal   = 3, //!< ソースバッファ内に不正な文字が含まれている
	};

	/****************************************************************************
	*				  			 GUStringConverterBase
	****************************************************************************/
	/*  @brief  文字列を変換するクラスです. @n
	*****************************************************************************/
	class UnicodeConverter
	{
	public:
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
			uint32 ReplacementChar = 0;
		};

		#pragma region Public Function
		/*!**********************************************************************
		*  @brief     UTF-8文字列をUTF-16文字列に変換します.
		*  @param[in] const uint8*      変換元の文字列
		*  @param[in] const gu::uint64 変換元の文字列のサイズ
		*  @param[in] uint16*           変換先の文字列
		*  @param[in] gu::uint64       変換先の文字列の長さ
		*  @param[inout] Options&      オプション
		*  @return    UnicodeConvertResult 変換結果
		*************************************************************************/
		static UnicodeConvertResult ConvertUTF8ToUTF16(const uint8* source, uint64 sourceLength, uint16* target, gu::uint64 targetLength, Options& options);

		/*!**********************************************************************
		*  @brief     UTF-8文字列をUTF-32文字列に変換します.
		*  @param[in] const uint8*      変換元の文字列
		*  @param[in] const gu::uint64 変換元の文字列のサイズ
		*  @param[in] uint32*           変換先の文字列
		*  @param[in] gu::uint64       変換先の文字列の長さ
		*  @param[inout] Options&      オプション
		*  @return    UnicodeConvertResult 変換結果
		*************************************************************************/
		static UnicodeConvertResult ConvertUTF8ToUTF32(const uint8* source, uint64 sourceLength, uint32* target, gu::uint64 targetLength, Options& options);
		
		/*!**********************************************************************
		*  @brief     UTF-16文字列をUTF-8文字列に変換します.
		*  @param[in] const uint16*      変換元の文字列
		*  @param[in] const gu::uint64 変換元の文字列のサイズ
		*  @param[in] uint8*           変換先の文字列
		*  @param[in] gu::uint64       変換先の文字列の長さ
		*  @param[inout] Options&      オプション
		*  @return    UnicodeConvertResult 変換結果
		*************************************************************************/
		static UnicodeConvertResult ConvertUTF16ToUTF8(const uint16* source, uint64 sourceLength, uint8* target, gu::uint64 targetLength, Options& options);

		/*!**********************************************************************
		*  @brief     UTF-16文字列をUTF-32文字列に変換します.
		*  @param[in] const uint16*      変換元の文字列
		*  @param[in] const gu::uint64 変換元の文字列のサイズ
		*  @param[in] uint32*           変換先の文字列
		*  @param[in] gu::uint64       変換先の文字列の長さ
		*  @param[inout] Options&      オプション
		*  @return    UnicodeConvertResult 変換結果
		*************************************************************************/
		static UnicodeConvertResult ConvertUTF16ToUTF32(const uint16* source, uint64 sourceLength, uint32* target, gu::uint64 targetLength, Options& options);
		
		/*!**********************************************************************
		*  @brief     UTF-32文字列をUTF-8文字列に変換します.
		*  @param[in] const uint32*      変換元の文字列
		*  @param[in] const gu::uint64 変換元の文字列のサイズ
		*  @param[in] uint8*           変換先の文字列
		*  @param[in] gu::uint64       変換先の文字列の長さ
		*  @param[inout] Options&      オプション
		*  @return    UnicodeConvertResult 変換結果
		*************************************************************************/
		static UnicodeConvertResult ConvertUTF32ToUTF8(const uint32* source, uint64 sourceLength, uint8* target, gu::uint64 targetLength, Options& options);

		/*!**********************************************************************
		*  @brief     UTF-32文字列をUTF-8文字列に変換します.
		*  @param[in] const uint32*     変換元の文字列
		*  @param[in] const gu::uint64 変換元の文字列のサイズ
		*  @param[in] uint16*           変換先の文字列
		*  @param[in] gu::uint64       変換先の文字列の長さ
		*  @param[inout] Options&      オプション
		*  @return    UnicodeConvertResult 変換結果
		*************************************************************************/
		static UnicodeConvertResult ConvertUTF32ToUTF16(const uint32* source, uint64 sourceLength, uint16* target, gu::uint64 targetLength, Options& options);
		
		/*!**********************************************************************
		*  @brief        UTF8における先行バイトを考慮した文字列長を取得します
		*  @param[in]    const uint8* 対象文字列
		*  @param[in]    const uint64 文字列長さ
		*  @param[in]    const bool 失敗したときにエラーを出すかどうか
		*  @param[out]   uint64& 文字列長
		*  @return       UnicodeConvertResult 変換結果
		*************************************************************************/
		static UnicodeConvertResult GetUTF8CharacterLength(const uint8* source, const uint64 sourceLength, uint64& outLength, const bool strict);
		
		/*!**********************************************************************
		*  @brief        UTF16における先行バイトを考慮した文字列長を取得します
		*  @param[in]    const uint16* 対象文字列
		*  @param[in]    const uint64 文字列長さ
		*  @param[in]    const bool 失敗したときにエラーを出すかどうか
		*  @param[out]   uint64& 文字列長
		*  @return       UnicodeConvertResult 変換結果
		*************************************************************************/
		static UnicodeConvertResult GetUTF16CharacterLength(const uint16* source, const uint64 sourceLength, uint64& outLength, const bool strict);

		/*!**********************************************************************
		*  @brief        UTF8文字の後続バイト数を取得します
		*  @param[in]    const uint8* 文字の先頭文字のポインタ
		*  @param[in]    const uint8* バッファの終端文字のポインタ
		*  @param[out]   uint64& 後続バイト数 (ASCII文字等, 後続が無い場合は0が格納される)
		*  @param[in]    const bool trueの場合, 不正文字のチェックを行います (不正文字が見つかった場合はSourceIllegalを返す)
		*  @return       UnicodeConvertResult 変換結果
		*************************************************************************/
		static UnicodeConvertResult GetUTF8TrailingBytes(const uint8* source, const uint8* sourceEnd, uint64& outExtraByteCount, const bool strict);

		/*!**********************************************************************
		*  @brief     UTF16 のサロゲートペアであるかを確認する @n
		*             サロゲートペアは, JIS第3, 4水準の漢字をすべてサポートするための拡張 (4バイト). @n
		*             従来のUnicodeで未使用の0xD800～0xDBFFを上位サロゲート0xDC00～0xDFFFを下位サロゲートと規定して4バイト文字を表現する　https://codezine.jp/article/detail/1592
		*  @param[in] const void* チェック対象の先頭文字のポインタ
		*  @param[in] const void* チェック対象の終端文字のポインタ
		*  @param[in] const bool trueの場合, 不正文字のチェックを行います (不正文字が見つかった場合はSourceIllegalを返す)
		*  @param[out] bool* サロゲートペアであるかどうか
		*  @return    UnicodeConvertResult 変換結果
		*************************************************************************/
		static UnicodeConvertResult CheckUTF16Surrogate(const uint16* sourceStart, const uint16* sourceEnd, const bool strict, bool* outSurrogate);
		
		/*!**********************************************************************
		*  @brief     UTF16の上位サロゲートであるかを確認する
		*  @param[in] const uint16 チェック対象の文字
		*  @return    bool
		*************************************************************************/
		static bool CheckUTF16HighSurrogate(const uint16 codeUnit) { return SURROGATE_HIGH_START <= codeUnit && codeUnit <= SURROGATE_HIGH_END; };
		
		/*!**********************************************************************
		*  @brief     UTF16の下位サロゲートであるかを確認する
		*  @param[in] const uint16 チェック対象の文字
		*  @return    bool
		*************************************************************************/
		static bool CheckUTF16LowSurrogate (const uint16 codeUnit) { return SURROGATE_LOW_START <= codeUnit && codeUnit <= SURROGATE_LOW_END; };
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
		*  @param[in]    const uint8**    変換元の文字列
		*  @param[in]    const uint8*     変換元の終端文字
		*  @param[out]   uint32&          変換先の文字
		*  @param[inout] Options&        オプション
		*  @return       UnicodeConvertResult 変換結果
		*************************************************************************/
		static UnicodeConvertResult CodePointUTF8ToUTF32(const uint8** sourceStart, const uint8* sourceEnd, uint32& target, [[maybe_unused]]Options& options);
		
		/*!**********************************************************************
		*  @brief        UTF-16文字をUTF-32に変換します.
		*  @param[in]    const uint16**    変換元の文字列
		*  @param[in]    
		*  @param[out]   gu::char32*      変換先の文字列
		*  @param[inout] Options&         オプション
		*  @return       UnicodeConvertResult 変換結果
		*************************************************************************/
		static UnicodeConvertResult CodePointUTF16ToUTF32(const uint16** sourceStart, const uint16* sourceEnd, uint32& target, Options& options);

		/*!**********************************************************************
		*  @brief        UTF-32文字をUTF-8に変換します.
		*  @param[in]    const uint32     変換元の文字
		*  @param[in]    const uint8**    変換先の文字列ポインタ
		*  @param[out]   const uint8*     変換先の終端文字
		*  @param[inout] Options&        オプション
		*  @return       UnicodeConvertResult 変換結果
		*************************************************************************/
		static UnicodeConvertResult CodePointUTF32ToUTF8(uint32 source, uint8** targetStart, const uint8* targetEnd, Options& options);

		/*!**********************************************************************
		*  @brief        UTF-32文字をUTF-16に変換します.
		*  @param[in]    const uint32     変換元の文字
		*  @param[in]    const uint16**   変換先の文字列ポインタ
		*  @param[out]   const uint16*    変換先の終端文字
		*  @param[inout] Options&        オプション
		*  @return       UnicodeConvertResult 変換結果
		*************************************************************************/
		static UnicodeConvertResult CodePointUTF32ToUTF16(uint32 source, uint16** targetStart, const uint16* targetEnd, Options& options);
		#pragma endregion Covert Char Function

		/*!**********************************************************************
		*  @brief        適切なUTF8コーデックかどうか
		*  @param[in]    const uint8* 変換元
		*  @param[in]    const uint64 文字列長さ
		*  @return       bool
		*************************************************************************/
		static bool IsLegalUTF8(const uint8* source, const uint64 length);

		/*!**********************************************************************
		*  @brief        不正文字を許容するかどうか. (ReplacementCharが0であれば不正文字は一切許容されない)
		*  @param[in]    const Options& option
		*  @return       bool trueで不正文字を許容しない. falseで不正文字を許容
		*************************************************************************/
		__forceinline static bool IsStrictConversion(const Options& option) { return (option.ReplacementChar == 0); }
		#pragma endregion 

		#pragma region Private Property
		/*! @brief 読み取り可能なUTF32の最大値*/
		static constexpr uint32 MAX_LEGAL_UTF32 = 0x0010FFFF;

		/*! @brief 不正文字が見つかった時の置換文字*/
		static constexpr uint32 REPLACEMENT_CHAR = 0x0000FFFD;

		/*! @brief UTF32 CodePointを2つの16bitに変換する場合の上位CodeUnitの下限*/
		static constexpr uint32 SURROGATE_HIGH_START = 0x0000D800;

		/*! @brief UTF32 CodePointを2つの16bitに変換する場合の上位CodeUnitの上限*/
		static constexpr uint32 SURROGATE_HIGH_END = 0x0000DBFF;

		/*! @brief UTF32 CodePointを2つの16bitに変換する場合の下位CodeUnitの下限*/
		static constexpr uint32 SURROGATE_LOW_START = 0x0000DC00;

		/*! @brief UTF32 CodePointを2つの16bitに変換する場合の下位CodeUnitの上限*/
		static constexpr uint32 SURROGATE_LOW_END = 0x0000DFFF;

		/*! @brief UTF32 CodePointを2つの16bitに変換する場合の上位CodeUnitのシフト量*/
		static constexpr uint32 SURROGATE_HIGH_SHIFT = 10;

		/*! @brief UTF32 CodePointを2つの16bitに変換する場合の下位CodeUnitのベース*/
		static constexpr uint32 SURROGATE_LOW_BASE = 0x00010000UL;

		/*! @brief UTF32 CodePointを2つの16bitに変換する場合のUTF32→UTF16変換時の下位マスク*/
		static constexpr uint32 SURROGATE_LOW_MASK = 0x3FFUL;

		/*! @brief Basic Multilingual Plane */
		static constexpr uint32 MAX_BMP = 0x0000FFFF;
		#pragma endregion 

	};

}
#endif