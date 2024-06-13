//////////////////////////////////////////////////////////////////////////////////
///  @file    GUParse.hpp
///  @brief   対象の文字列を検索し, 指定した数値を読み取ります. @n
///           読み込み順序が定まっていない文字列(環境変数の取得など)に対する値の読み込みに効果を発揮します. @n 
/// 　　　      数値データが連続して配置されていたり, 読み込み順序が決まっているような構造の場合には, FileSystemクラスを利用することをお勧めします. @n
///           この理由は, streamポインタ自身のメモリをずらすことは行わず, streamの先頭から文字列検索が行われるため, 読み込み速度が大きく低下するためです.@n           
///           Value -> 読み込み順序が決定されていないものを探すときに使用します. @n
///           文字列全体を探し, Matchに合う文字列の箇所の後に続く数値を読み取ります. (DesireToFind=100) @n 
///           Bool  -> Valueと同じ使い方です. @n 
///           Trueの場合の認識文字列はTrue, Yes, On, 1 (大文字小文字区別なし) @n
///           Falseの場合の認識文字列はFalse, No, Off, 0 (大文字小文字区別なし)
/// 　　　　　　　　　　　　　
///  @author toide
///  @date   2024/02/28 0:22:43
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GU_PARSE_HPP
#define GU_PARSE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GUType.hpp"
#include "GUClassUtility.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
namespace gu
{
	namespace details::string
	{
		template<class Char, int CharSize> class StringBase;
	}
	using tstring = details::string::StringBase<tchar, 2>; // 1byteも含める場合は要調整
}

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace gu
{
	/****************************************************************************
	*				  			   GUParse
	****************************************************************************/
	/* @class   GUParse
	*   @brief   読み込み順序が定まっていない文字列(環境変数の取得など)に対する値の読み込みに効果を発揮します. @n
	*            Value -> 読み込み順序が決定されていないものを探すときに使用します.@n
    *            文字列全体を探し, Matchに合う文字列の箇所の後に続く数値を読み取ります. (DesireToFind=100)@n
	*	         Trueの場合の認識文字列はTrue, Yes, On, 1 (大文字小文字区別なし) @n
    *            Falseの場合の認識文字列はFalse, No, Off, 0 (大文字小文字区別なし)
	*****************************************************************************/
	class Parse final
	{
	public:
		#pragma region Constructor and Destructor
		/*!***********************************************************************
		*  @brief   文字列全体を探し, Matchに合う文字列の箇所の後に続く数値を読み取ります. @n
		*           型の判定は引数に入れた型で自動判別してしまうため, 事前に型は決めておいてください.
		**************************************************************************/
		static bool Value(const tstring& stream, const tstring& match, int8  & value);
		static bool Value(const tstring& stream, const tstring& match, int16 & value);
		static bool Value(const tstring& stream, const tstring& match, int32 & value);
		static bool Value(const tstring& stream, const tstring& match, int64 & value);
		static bool Value(const tstring& stream, const tstring& match, uint8 & value);
		static bool Value(const tstring& stream, const tstring& match, uint16& value);
		static bool Value(const tstring& stream, const tstring& match, uint32& value);
		static bool Value(const tstring& stream, const tstring& match, uint64& value);

		/*!***********************************************************************
		*  @brief     文字列全体を探し, Matchに合う文字列の箇所の後に続くBool値を取得します @n
		*             検索にひっかかかる文字列は以下の通りです @n
		*             True  : True, Yes, On, 1 @n
		*             False : False, No, Off, 0 
		*  @param[in]  const tstring 対象の文字列
		*  @param[in]  const tstring 探したい文字列
		*  @param[out] bool  読み込んだboolの値がtrueかfalseか
		* 
		* 　@return 文字列が見つからない場合のみfalseを返し, 読み込みが成功したらtrueを返します. 
		**************************************************************************/
		static bool Bool(const tstring& stream, const tstring& match, bool& onOff);

		/*!***********************************************************************
		*  @brief  文字列全体を探し, Parameter名が文字列中に含まれるかを検索します@n
		*           文字列は大文字小文字を区別します.
		**************************************************************************/
		static bool Contains(const tstring& stream, const tstring& parameter);

		#pragma endregion 

		#pragma region Public Property

		#pragma endregion 

		#pragma region Public Constructor and Destructor
		#pragma endregion
		
	private:
		#pragma region Private Function
		#pragma endregion

		#pragma region Private Property

		#pragma endregion 
	};
}

#endif