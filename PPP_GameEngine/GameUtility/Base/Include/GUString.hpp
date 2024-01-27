//////////////////////////////////////////////////////////////////////////////////
///             @file   GUString.hpp
///             @brief  動的な文字列クラスです. std::stringと同等に扱えるクラス
///                     未実装
///             @author toide
///             @date   2023/11/08 0:50:57
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GU_STRING_HPP
#define GU_STRING_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Private/Base/Include/GUStringUtility.hpp"
#include "../../Memory/Include/GUMemory.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace gu
{
	namespace details
	{
		/****************************************************************************
		*				  			   string
		*************************************************************************//**
		*  @class     string
		*  @brief     プリミティブな型であるという判断の下, 特例でgu::uint32のように小文字スタートで行うことにいたしました.
		*****************************************************************************/
		template<class Char, int CharByte = 1>
		class StringBase : public Copyable
		{
		public:
			/****************************************************************************
			**                Public Function
			*****************************************************************************/
			/*----------------------------------------------------------------------
			*  @brief :  文字列を連結します
			/*----------------------------------------------------------------------*/
			static StringBase<Char, CharByte> Concat(const StringBase<Char, CharByte>& left, const StringBase<Char, CharByte>& right);
			static StringBase<Char, CharByte> Concat(const Char* left, const Char* right);

			/*----------------------------------------------------------------------
			*  @brief :  文字列を代入します
			/*----------------------------------------------------------------------*/
			void Assign(const Char* string);
			void Assign(const Char* string, const uint64 length) noexcept;
			void Assign(const StringBase<Char, CharByte>& string) { Assign(string.CString(), string.Size()); }

			/*----------------------------------------------------------------------
			*  @brief :  文字列を追加します
			/*----------------------------------------------------------------------*/
			void Append(const Char* string);
			void Append(const Char* string, const uint64 length);
			void Append(const StringBase<Char, CharByte>& string);

			/*----------------------------------------------------------------------
			*  @brief :  Capacityやメモリは特に破棄せず、文字列の先頭のみを終端文字に変更します.
			*            また、SSOなどのモード切替も行いません.
			/*----------------------------------------------------------------------*/
			void Clear();

			/*----------------------------------------------------------------------
			*  @brief :  メモリを事前に確保します. (NonSSOのときかつcapacity以上であれば確保します)
			/*----------------------------------------------------------------------*/
			void Reserve(const uint64 length);

			/*----------------------------------------------------------------------
			*  @brief :  文字列を置換します
			*
			*  @param[in] const Char* from 置換される文字列
			*  @param[in] const Char* to   置換する文字列
			/*----------------------------------------------------------------------*/
			StringBase<Char, CharByte> Replace(const Char* from, const Char* to, const bool useCaseSensitivity);
			StringBase<Char, CharByte> Replace(const StringBase<Char, CharByte>& from, const StringBase<Char, CharByte>& to, const bool useCaseSensitivity);

			/*----------------------------------------------------------------------
			*  @brief :  指定した文字列がこの文字列内に存在するかを判断します.
			*  
			*  @param[in] : const Char* string              検索文字列
			*  @param[in] : const bool  useCaseSensitivity  大文字と小文字の区別 
			* 
			*  @return    : 文字列が存在すればtrue, stringが空文字列である場合はtrueとなる
			/*----------------------------------------------------------------------*/
			__forceinline bool Contains(const Char* string, const bool useCaseSensitivity) const
			{
				return FindFirstIndexOf(string, 0, useCaseSensitivity) >= 0;
			}
			__forceinline bool Contains(const StringBase<Char, CharByte>& string, const bool useCaseSensitivity) const
			{
				return FindFirstIndexOf(string, 0, useCaseSensitivity);
			}

			/*----------------------------------------------------------------------
			*  @brief :  文字列を検索し, 見つかった最初の文字のインデックスを返します.
			*            見つからなかった場合は-1, stringが空文字列である場合は0
			*  
			*  @param[in] : const Char* 検索文字列
			*  @param[in] : const uint64 検索を開始するインデックス (省略した場合は先頭から検索します)
			*  @param[in] : const bool 大文字と小文字の区別
			* 
			*  @return 見つからなかったらNPOS, stringが空文字列である場合は0
			/*----------------------------------------------------------------------*/
			uint64 FindFirstIndexOf(const Char* string, const uint64 startIndex = 0, const bool useCaseSensitivity = true) const;
			uint64 FindFirstIndexOf(const StringBase<Char, CharByte>& string, const uint64 startIndex = 0, const bool useCaseSensititivity = true) const;

			/*----------------------------------------------------------------------
			*  @brief :  文字列を検索し, 最後に見つかったの文字のインデックスを返します.
			*            見つからなかった場合は-1, stringが空文字列である場合は0
			/*----------------------------------------------------------------------*/
			uint64 FindLastIndexOf(const Char* string, const uint64 startIndex = NPOS, const uint64 count = NPOS, const bool useCaseSensitivity = true) const;
			uint64 FindLastIndexOf(const StringBase<Char, CharByte>& string, const uint64 startIndex = NPOS, const uint64 count = NPOS, const bool useCaseSensitivity = true) const;

			/*----------------------------------------------------------------------
			*  @brief :  文字列の先頭が指定した文字列と一致するかを判断します
			/*----------------------------------------------------------------------*/
			__forceinline bool IsFirstMatch(const Char* string, const bool useCaseSensitivity = true)
			{
				return StringUtility::IsFirstMatch(CString(), Size(), string, StringUtility::Length(string), useCaseSensitivity);
			}
			__forceinline bool IsFirstMatch(const StringBase<Char, CharByte>& string, const bool useCaseSensitivity = true)
			{
				return StringUtility::IsFirstMatch(CString(), Size(), string.CString(), string.Size(), useCaseSensitivity);
			}

			/*----------------------------------------------------------------------
			*  @brief :  文字列の部分文字列を抽出します
			* 
			*  @param[in] const uint64 startIndex 検索を開始するインデックス
			*  @param[in] const uint64 count      文字数(NPOSの場合, 末尾まで抽出する)
			* 
			*  @param[in] 抽出された文字列
			/*----------------------------------------------------------------------*/
			__forceinline StringBase<Char, CharByte> SubString(const uint64 startIndex, const uint64 count = NPOS) const
			{
				const Char* begin = nullptr;
				const Char* end   = nullptr;
				StringUtility::SubString(CString(), Size(), startIndex, count, &begin, &end);
				return StringBase<Char, CharByte>(begin, end);
			}

			/*----------------------------------------------------------------------
			*  @brief :  文字列の先頭から指定した文字数を抽出します. 
			/*----------------------------------------------------------------------*/
			__forceinline StringBase<Char, CharByte> Left(const uint64 count) const
			{
				const Char* begin = nullptr;
				const Char* end   = nullptr;
				StringUtility::Left(CString(), count, &count, &end);
				return StringBase<Char, CharByte>(begin, end);
			}

			/*----------------------------------------------------------------------
			*  @brief :  文字列の末尾から指定した文字数を抽出します.
			/*----------------------------------------------------------------------*/
			StringBase<Char, CharByte> Right(const uint64 count) const
			{
				const Char* begin = nullptr;
				const Char* end   = nullptr;
				StringUtility::Right(CString(), count, &count, &end);
				return StringBase<Char, CharByte>(begin, end);
			}

			/*----------------------------------------------------------------------
			*  @brief :  小文字を全て大文字に変換した文字列を返します
			/*----------------------------------------------------------------------*/
			StringBase<Char, CharByte> ToUpper() const;

			/*----------------------------------------------------------------------
			*  @brief :  大文字を全て小文字に変換した文字列を返します
			/*----------------------------------------------------------------------*/
			StringBase<Char, CharByte> ToLower() const;

			/*----------------------------------------------------------------------
			*  @brief :  文字列の先頭と末尾の空白を全て削除した文字列を返します
			/*----------------------------------------------------------------------*/
			StringBase<Char, CharByte> Trim() const
			{
				Char*  begin  = nullptr;
				uint64 length = 0;
				StringUtility::Trim(CString(), Size(), &begin, &length);
				return StringBase<Char, CharByte>(begin, length);
			}

#pragma region Convert number
			int8   ToInt8(const uint64 radix = 0) const;
			int16  ToInt16(const uint64 radix = 0) const;
			int32  ToInt32(const uint64 radix = 0) const;
			int64  ToInt64(const uint64 radix = 0) const;
			uint8  ToUInt8 (const uint64 radix = 0) const;
			uint16 ToUInt16(const uint64 radix = 0) const;
			uint32 ToUInt32(const uint64 radix = 0) const;
			uint64 ToUInt64(const uint64 radix = 0) const;

			bool TryToInt8  (int8* outValue, const uint64 radix = 0) const;
			bool TryToInt16 (int16* outValue, const uint64 radix = 0) const;
			bool TryToInt32 (int32* outValue, const uint64 radix = 0) const;
			bool TryToInt64 (int64* outValue, const uint64 radix = 0) const;
			bool TryToUInt8 (uint8* outValue, const uint64 radix = 0) const;
			bool TryToUInt16(uint16* outValue, const uint64 radix = 0) const;
			bool TryToUInt32(uint32* outValue, const uint64 radix = 0) const;
			bool TryToUInt64(uint64* outValue, const uint64 radix = 0) const;
#pragma endregion Convert number
			/****************************************************************************
			**                Public Member Variables
			*****************************************************************************/
			static constexpr uint64 NPOS = static_cast<uint64>(-1);

#pragma region Property
			/*----------------------------------------------------------------------
			*  @brief :  範囲チェック付きの要素アクセス
			/*----------------------------------------------------------------------*/
			__forceinline Char& At(const uint64 index)
			{
				Check(0 <= index && index <= Size());
				return GetBuffer()[index];
			}
			__forceinline const Char& At(const uint64 index) const noexcept
			{
				Check(0 <= index && index <= Size());
				return GetBuffer()[index];
			}

			/*----------------------------------------------------------------------
			*  @brief :  文字列が空かどうかを判定します
			/*----------------------------------------------------------------------*/
			bool IsEmpty() const;

			/*----------------------------------------------------------------------
			*  @brief :  C言語としての生の文字列表現を取得します
			/*----------------------------------------------------------------------*/
			const Char* CString() const noexcept;

			/*----------------------------------------------------------------------
			*  @brief :  文字列の長さを取得します
			/*----------------------------------------------------------------------*/
			uint64 Size() const noexcept { return IsSSOMode() ? _data.SSO.Size >> 1 : _data.NonSSO.Size >> 1; }

			/*----------------------------------------------------------------------
			*  @brief :  メモリを再確保せずに格納できる最大の要素数を取得します
			/*----------------------------------------------------------------------*/
			uint64 Capacity() const noexcept { return IsSSOMode() ? SSO_CAPACITY : _data.NonSSO.Capacity; }
#pragma endregion Property

#pragma region Operator Function
			StringBase<Char, CharByte>& operator=(const StringBase<Char, CharByte>& right) { Assign(right); return *this; }
			StringBase<Char, CharByte>& operator=(const Char* right) { Assign(right); return *this; }
			StringBase<Char, CharByte>& operator=(const Char  right) { Assign(&right, 1); return *this; }

			StringBase<Char, CharByte>& operator+=(const StringBase<Char, CharByte>& right)
			{
				Append(right.CString(), right.Size());
				return *this;
			}
			StringBase<Char, CharByte>& operator+=(const Char* right)
			{
				Append(right, CStringLength(right));
				return *this;
			}
			StringBase<Char, CharByte>& operator+=(const Char right)
			{
				Append(&right, 1);
				return *this;
			}

			explicit operator bool() const noexcept { return !IsEmpty(); }

			// 高速化のためにチェックを入れておりません. チェックを行う場合はAtを使用してください.
			__forceinline Char& operator[](const uint64 index) noexcept
			{
				return GetBuffer()[index];
			}

			__forceinline const Char& operator[](const uint64 index) const noexcept
			{
				return GetBuffer()[index];
			}

			__forceinline bool operator==(const StringBase<Char, CharByte>& right) const 
			{
				return StringUtility::Compare(CString(), Size(), right.CString(), right.Size(), NPOS, true) == 0;
			}
			__forceinline bool operator==(const Char* right)
			{
				return StringUtility::Compare(CString(), Size(), right, StringUtility::Length(right), NPOS, true) == 0;
			}
			__forceinline bool operator!=(const StringBase<Char, CharByte>& right) const
			{
				return StringUtility::Compare(CString(), Size(), right.CString(), right.Size(), NPOS, true) != 0;
			}
			__forceinline bool operator!=(const Char* right)
			{
				return StringUtility::Compare(CString(), Size(), right, StringUtility::Length(right), NPOS, true) != 0;
			}
			__forceinline StringBase<Char, CharByte> operator+(const StringBase<Char, CharByte>& right) const
			{
				return Concat(this->CString(), right.CString());
			}
			__forceinline StringBase<Char, CharByte> operator+(const Char* right) const
			{
				return Concat(this->CString(), right);
			}
#pragma endregion Operator Function

			/****************************************************************************
			**                Constructor and Destructor
			*****************************************************************************/
#pragma region Constructor and Destructor
			StringBase() { Initialize(); }

			StringBase(const Char* string) : StringBase<Char, CharByte>() { Assign(string); }

			StringBase(const Char* string, const gu::uint64 length) : StringBase<Char, CharByte>() { Assign(string, length); }

			StringBase(const StringBase<Char, CharByte>& string, const uint64 beginIndex) : StringBase<Char, CharByte>()
			{
				Assign(string.CString() + beginIndex, string.Size());
			}

			StringBase(const Char* begin, const Char* end) : StringBase<Char, CharByte>()
			{
				Assign(begin, static_cast<uint64>(end - begin));
			}

			StringBase(const StringBase<Char, CharByte>& string) : StringBase<Char, CharByte>()
			{
				CopyFrom(string);
			}

			explicit StringBase(StringBase<Char, CharByte>&& string) noexcept : StringBase<Char, CharByte>()
			{
				Move(std::move(string));
			}

			~StringBase() 
			{
				Release(); 
			}

#pragma endregion Constructor and Destructor


		private:
			// @brief : SSOのバッファサイズ
			static constexpr gu::uint64 SSO_CAPACITY = 15;

			/****************************************************************************
			**                Struct
			*****************************************************************************/
			/*----------------------------------------------------------------------
			*  @brief :  文字数が少ないときはnewをさせないために行うSmall String Optimization
			/*----------------------------------------------------------------------*/
			struct SSOString
			{
				// @brief : 文字列長
				uint64 Size; // ---xxxxy : x = size, y:ssoFlag on 0, off 1

				// @brief : SSOで使用する16バイトの文字バッファ (最後1byteは終端文字です)
				Char Buffer[SSO_CAPACITY + 1];
			};

			/*----------------------------------------------------------------------
			*  @brief :  文字数が少ないときはnewをさせないために行うSmall String Optimization
			/*----------------------------------------------------------------------*/
			struct NonSSOString
			{
				// @brief : 文字数
				uint64 Size;

				// @brief : 動的確保するメモリの先頭ポインタ
				Char*  Pointer;

				// @brief : 動的確保した配列に格納できる文字列の最大長さ
				uint64 Capacity;
			};

			/*----------------------------------------------------------------------
			*  @brief :  文字列情報
			/*----------------------------------------------------------------------*/
			union StringData
			{
				SSOString    SSO;
				NonSSOString NonSSO;
			};

			/****************************************************************************
			**                Private Function
			*****************************************************************************/
#pragma region Utility

#pragma endregion Utility

#pragma region Memory
			/*----------------------------------------------------------------------
			*  @brief :  空の配列による初期状態の作成
			/*----------------------------------------------------------------------*/
			__forceinline void Initialize() noexcept
			{
				_data.SSO.Size = 0;
				SetSSOMode();
				Memory::Zero(_data.SSO.Buffer, SSO_CAPACITY + 1);
			}

			/*----------------------------------------------------------------------
			*  @brief :  NonSSOの状態であるならメモリを破棄する
			/*----------------------------------------------------------------------*/
			void Release() noexcept;

			/*----------------------------------------------------------------------
			*  @brief :  文字列のコピー
			/*----------------------------------------------------------------------*/
			void CopyFrom(const StringBase<Char, CharByte>& source);

			/*----------------------------------------------------------------------
			*  @brief :  メモリを移動する
			/*----------------------------------------------------------------------*/
			void Move(StringBase<Char, CharByte>&& source) noexcept;

			/*----------------------------------------------------------------------
			*  @brief :  文字列の長さ取得
			/*----------------------------------------------------------------------*/
			__forceinline uint64 CStringLength(const Char* string)
			{
				const Char* temp = nullptr;
				for (temp = string; *temp; ++temp);
				return temp - string;
			}

			__forceinline       Char* GetBuffer()                { return IsSSOMode() ? _data.SSO.Buffer : _data.NonSSO.Pointer; }
			__forceinline const Char* GetBuffer() const noexcept { return IsSSOMode() ? _data.SSO.Buffer : _data.NonSSO.Pointer; }
#pragma endregion Memory
#pragma region SSO operation
			/*----------------------------------------------------------------------
			*  @brief :  SSOを使用する (既存の文字配列を使用する)モードに設定します
			/*----------------------------------------------------------------------*/
			__forceinline void SetSSOMode() { _data.SSO.Size &= ~(0x1);}

			/*----------------------------------------------------------------------
			*  @brief :  SSOを使用しない (newで領域を確保する)モードに設定します
			/*----------------------------------------------------------------------*/
			__forceinline void SetNonSSOMode() { _data.SSO.Size |= 0x1;}

			/*----------------------------------------------------------------------
			*  @brief :  末尾のビットを使ってSSOを使う文字列化を返します
			/*----------------------------------------------------------------------*/
			bool IsSSOMode() const noexcept 
			{
				return  !(static_cast<uint8>(_data.SSO.Size) & 0x1);
			}

			/*----------------------------------------------------------------------
			*  @brief :  SSOを使わない文字列かを返します
			/*----------------------------------------------------------------------*/
			bool IsNonSSOMode() const noexcept 
			{ 
				return (static_cast<uint8>(_data.SSO.Size) & 0x1); 
			}

			/*----------------------------------------------------------------------
			*  @brief :  SSO構造体における文字列長を返します. 
			*            Size構造体は, ---xxxxy : x = size, y:ssoFlag (0:sso, 1:nonSSO)として扱うため, 右シフトを加えています. 
			/*----------------------------------------------------------------------*/
			__forceinline uint64 GetSSOLength() const noexcept { return _data.SSO.Size >> 1; }

			/*----------------------------------------------------------------------
			*  @brief :  SSOの文字長を設定します
			/*----------------------------------------------------------------------*/
			__forceinline void SetSSOLength(const uint64 length)
			{
				_data.SSO.Size = (length & 0x7F) << 1;
				_data.SSO.Buffer[length] = '\0'; // 終端文字
			}
			__forceinline void SetNonSSOLength(const uint64 length) noexcept
			{
				_data.NonSSO.Size = length << 1;
				_data.NonSSO.Pointer[length] = '\0';
			}
#pragma endregion SSO operation

			/****************************************************************************
			**                Private Member Variables
			*****************************************************************************/
			union StringData _data;
		};

#pragma region Implement
#pragma region Main Function
		template<class Char, int CharByte>
		void StringBase<Char, CharByte>::Assign(const Char* string)
		{
			// 割り当て
			Assign(string, CStringLength(string));
		}

		template<class Char, int CharByte>
		void StringBase<Char, CharByte>::Assign(const Char* string, const uint64 length) noexcept
		{
			/*-------------------------------------------------------------------
			-        SSOを使用するモードの場合は, そのまま指定の文字数だけ配列にコピー
			---------------------------------------------------------------------*/
			if (length <= SSO_CAPACITY)
			{
				// データを配列に格納
				Memory::Copy(this->_data.SSO.Buffer, string, length * CharByte);
				// SSOとして設定する
				SetSSOLength(length);
				SetSSOMode();
			}
			/*-------------------------------------------------------------------
			-        SSOを使用しない場合は、もともとの文字列を一回破棄して、文字列を新規割り当て
			---------------------------------------------------------------------*/
			else
			{
				// もともと持っている文字列は破棄
				Release();

				// 新規データの追加
				_data.NonSSO.Pointer  = new Char[length + 1];
				_data.NonSSO.Capacity = length;
				Memory::Copy(this->_data.NonSSO.Pointer, string, length * CharByte);
				SetNonSSOLength(length);
				
				// SSOを使用しない設定
				SetNonSSOMode();
			}
		}

		/*----------------------------------------------------------------------
		*  @brief :  文字列を追加します
		/*----------------------------------------------------------------------*/
		template<class Char, int CharByte>
		void StringBase<Char, CharByte>::Append(const Char* string, const uint64 length)
		{
			const auto totalLength = Size() + length;
			const auto firstLength = Size();

			/*-------------------------------------------------------------------
			-        TotalがSSOであった場合は, そのまま配列の後ろに追加する
			---------------------------------------------------------------------*/
			if (totalLength <= SSO_CAPACITY)
			{
				Memory::Copy(&this->_data.SSO.Buffer[firstLength], string, length * sizeof(Char));
				SetSSOLength(totalLength);
				SetSSOMode();
				return;
			}

			/*-------------------------------------------------------------------
			-        TotalがNonSSOであるが,SSOであった場合は, 強制的にヒープを作成する
			---------------------------------------------------------------------*/
			if (IsSSOMode())
			{
				auto temp = new Char[totalLength + 1];
				_data.NonSSO.Capacity = totalLength;
				Memory::Copy(&temp[0], this->_data.SSO.Buffer, firstLength * sizeof(Char));
				Memory::Copy(&temp[firstLength], string, length * sizeof(Char));
				_data.NonSSO.Pointer = temp;
			}
			else
			{
				/*-------------------------------------------------------------------
				-        Capacity内であればヒープを作らずに追加し, そうでなければ新しくヒープを作成し, まとめて追加する
				---------------------------------------------------------------------*/
				if (totalLength < _data.NonSSO.Capacity)
				{
					Memory::Copy(&this->_data.NonSSO.Pointer[firstLength], string, length * sizeof(Char));
				}
				else
				{
					auto temp = new Char[totalLength + 1];
					_data.NonSSO.Capacity = totalLength;
					Memory::Copy(&temp[0], this->_data.NonSSO.Pointer, firstLength * sizeof(Char));
					Memory::Copy(&temp[firstLength], string, length * sizeof(Char));
					Release();
					_data.NonSSO.Pointer = temp;
				}
			}

			SetNonSSOLength(totalLength);
			SetNonSSOMode();
		}

		template<class Char, int CharByte>
		void StringBase<Char, CharByte>::Append(const StringBase<Char, CharByte>& string)
		{
			Append(string.CString(), string.Size());
		}

		template<class Char, int CharByte>
		void StringBase<Char, CharByte>::Append(const Char* string)
		{
			Append(string, CStringLength(string));
		}

		/*----------------------------------------------------------------------
		*  @brief :  Capacityやメモリは特に破棄せず、文字列の先頭のみを終端文字に変更します. 
		*            また、SSOなどのモード切替も行いません. 
		/*----------------------------------------------------------------------*/
		template<class Char, int CharByte>
		void StringBase<Char, CharByte>::Clear()
		{
			if (IsSSOMode())
			{
				SetSSOLength(0);
				SetSSOMode();
			}
			else
			{
				SetNonSSOLength(0);
				SetNonSSOMode();
			}
		}

		/*----------------------------------------------------------------------
		*  @brief :  メモリを事前に確保します. (capacity以上であればメモリ確保します. 暗にNonSSOモードに使用します)
		/*----------------------------------------------------------------------*/
		template<class Char, int CharByte>
		void StringBase<Char, CharByte>::Reserve(const uint64 length)
		{
			if (length <= Capacity()) { return; }

			auto temp = new Char[length + 1];

			if (IsNonSSOMode() && Capacity() > 0)
			{
				Memory::Copy(temp, this->_data.NonSSO.Pointer, CharByte * Size());
				delete[](_data.NonSSO.Pointer);
			}

			_data.NonSSO.Capacity = length;
			_data.NonSSO.Pointer  = temp;
		}

		/*----------------------------------------------------------------------
		*  @brief :  文字列を検索し, 見つかった最初の文字のインデックスを返します.
		*            見つからなかった場合は-1(npos), stringが空文字列である場合は0
		/*----------------------------------------------------------------------*/
		template<class Char, int CharByte>
		uint64 StringBase<Char, CharByte>::FindFirstIndexOf(const Char* string, const uint64 startIndex, const bool useCaseSensitivity) const
		{
			return StringUtility::FindFirstIndexOf(CString(), Size(), string, StringUtility::Length(string), startIndex, useCaseSensitivity);
		}

		template<class Char, int CharByte>
		uint64 StringBase<Char, CharByte>::FindFirstIndexOf(const StringBase<Char, CharByte>& string, const uint64 startIndex, const bool useCaseSensitivity) const
		{
			return StringUtility::FindFirstIndexOf(CString(), Size(), string.CString(), string.Size(), startIndex, useCaseSensitivity);
		}

		/*----------------------------------------------------------------------
		*  @brief :  文字列を検索し, 最後に見つかったの文字のインデックスを返します.
		*            見つからなかった場合は-1, stringが空文字列である場合は0
		/*----------------------------------------------------------------------*/
		template<class Char, int CharByte>
		uint64 StringBase<Char, CharByte>::FindLastIndexOf(const Char* string, const uint64 startIndex, const uint64 count, const bool useCaseSensitivity) const
		{
			return StringUtility::FindLastIndexOf(CString(), Size(), string, StringUtility::Length(string), startIndex, count, useCaseSensitivity);
		}

		template<class Char, int CharByte>
		uint64 StringBase<Char, CharByte>::FindLastIndexOf(const StringBase<Char, CharByte>& string, const uint64 startIndex, const uint64 count, const bool useCaseSensitivity) const
		{
			return StringUtility::FindLastIndexOf(CString(), Size(), string, string.Size(), startIndex, count, useCaseSensitivity);
		}

		/*----------------------------------------------------------------------
		*  @brief :  小文字を全て大文字に変換した文字列を返します
		/*----------------------------------------------------------------------*/
		template<class Char, int CharByte>
		StringBase<Char, CharByte> StringBase<Char, CharByte>::ToUpper() const
		{
			StringBase<Char, CharByte> result(CString(), Size());
			for (uint64 i = 0; i < Size(); ++i)
			{
				result[i] = StringUtility::ToUpper<Char>(result[i]);
			}
			return result;
		}

		/*----------------------------------------------------------------------
		*  @brief :  大文字を全て小文字に変換した文字列を返します
		/*----------------------------------------------------------------------*/
		template<class Char, int CharByte>
		StringBase<Char, CharByte> StringBase<Char, CharByte>::ToLower() const
		{
			StringBase<Char, CharByte> result(CString(), Size());
			for (uint64 i = 0; i < Size(); ++i)
			{
				result[i] = StringUtility::ToLower<Char>(result[i]);
			}
			return result;
		}

		/*----------------------------------------------------------------------
		*  @brief :  文字列を連結します
		/*----------------------------------------------------------------------*/
		template<typename Char, int CharByte>
		StringBase<Char, CharByte> StringBase<Char, CharByte>::Concat(const StringBase<Char, CharByte>& left, const StringBase<Char, CharByte>& right)
		{
			StringBase<Char, CharByte> string;
			string.Reserve(left.Size()   + right.Size());
			string.Append(left.CString() , left .Size());
			string.Append(right.CString(), right.Size());
			return string;
		}

		/*----------------------------------------------------------------------
		*  @brief :  文字列を連結します
		/*----------------------------------------------------------------------*/
		template<typename Char, int CharByte>
		StringBase<Char, CharByte> StringBase<Char, CharByte>::Concat(const Char* left, const Char* right)
		{
			StringBase<Char, CharByte> string;

			const auto leftLength  = StringUtility::Length(left);
			const auto rightLength = StringUtility::Length(right);
			string.Reserve(leftLength + rightLength);
			string.Append(left, leftLength);
			string.Append(right, rightLength);
			return string;
		}


		/*----------------------------------------------------------------------
		*  @brief :  文字列を置換します
		*
		*  @param[in] const Char* from 置換される文字列
		*  @param[in] const Char* to   置換する文字列
		/*----------------------------------------------------------------------*/
		template<typename Char, int CharByte>
		StringBase<Char, CharByte> StringBase<Char, CharByte>::Replace(const Char* from, const Char* to, const bool useCaseSensitivity)
		{
			StringBase<Char, CharByte> result;
			result.Reserve(Size());

			uint64 position       = 0;
			uint64 startIndex     = 0;
			const auto fromLength = StringUtility::Length(from);
			const auto toLength   = StringUtility::Length(to);

			if (fromLength > 0)
			{
				do
				{
					position = StringUtility::FindFirstIndexOf(from, Size(), from, fromLength, startIndex, useCaseSensitivity);

					if (position >= 0)
					{
						result.Append(CString() + startIndex, position - startIndex);
						result.Append(to, toLength);
						startIndex = position + fromLength;
					}

				} while (position >= 0);

			}

			result.Append(CString() + startIndex, Size() - startIndex);
		}
		template<typename Char, int CharByte>
		StringBase<Char, CharByte> StringBase<Char, CharByte>::Replace(const StringBase<Char, CharByte>& from, const StringBase<Char, CharByte>& to, const bool useCaseSensitivity)
		{
			StringBase<Char, CharByte> result;
			result.Reserve(Size());

			uint64 position   = 0;
			uint64 startIndex = 0;
			
			if (from.Size() > 0)
			{
				do
				{
					position = StringUtility::FindFirstIndexOf(CString(), Size(), from.CString(), from.Size(), startIndex, useCaseSensitivity);

					if (position >= 0)
					{
						result.Append(CString() + startIndex, position - startIndex);
						result.Append(to.CString(), to.Size());
						startIndex = position + from.Size();
					}

				} while (position >= 0);

			}

			result.Append(CString() + startIndex, Size() - startIndex);
		}

#pragma region Covert Number
#define TO_INT_DEF(Type, Func)                                               \
		const  Char* begin = nullptr;                                        \
		const  Char* end   = nullptr;                                        \
		uint64 length      = 0;                                              \
		NumberConversionResult result = NumberConversionResult::Success;     \
		StringUtility::Trim(CString(), Size(), &begin, &length);             \
		Type num = StringUtility::Func(begin, length, radix, &end, &result); \
		if (result == NumberConversionResult::ArgumentsError)                \
		{                                                                    \
			Ensure(0);                                                       \
		}                                                                    \
		if (result == NumberConversionResult::FormatError)                   \
		{                                                                    \
			Ensure(0);                                                       \
		}                                                                    \
		if (result == NumberConversionResult::Overflow)                      \
		{                                                                    \
			Ensure(0);                                                       \
		}                                                                    \
		Ensure(end == begin + length);                                       \
		return num;

#define TRY_TO_INT_DEF(Type, Func)                                           \
		const  Char* begin = nullptr;                                        \
		const  Char* end   = nullptr;                                        \
		uint64 length      = 0;                                              \
		NumberConversionResult result = NumberConversionResult::Success;     \
		StringUtility::Trim(CString(), Size(), &begin, &length);             \
		Type num = StringUtility::Func(begin, length, radix, &end, &result); \
		if(end != begin + length)                  {return false;}           \
        if(res != NumberConversionResult::Success) {return false;}           \
		if(outValue != nullptr){*outValue = num;}                            \
		return num;

		template<class Char, int CharByte>
		int8   StringBase<Char, CharByte>::ToInt8(const uint64 radix) const
		{
			TO_INT_DEF(int8, ToInt8);
		}

		template<class Char, int CharByte>
		int16  StringBase<Char, CharByte>::ToInt16(const uint64 radix) const
		{
			TO_INT_DEF(int16, ToInt16);
		}

		template<class Char, int CharByte>
		int32  StringBase<Char, CharByte>::ToInt32(const uint64 radix) const
		{
			TO_INT_DEF(int32, ToInt32);
		}

		template<class Char, int CharByte>
		int64  StringBase<Char, CharByte>::ToInt64(const uint64 radix) const
		{
			TO_INT_DEF(int64, ToInt64);
		}

		template<class Char, int CharByte>
		uint8  StringBase<Char, CharByte>::ToUInt8(const uint64 radix) const
		{
			TO_INT_DEF(uint8, ToUInt8);
		}

		template<class Char, int CharByte>
		uint16 StringBase<Char, CharByte>::ToUInt16(const uint64 radix) const
		{
			TO_INT_DEF(uint16, ToUInt16);
		}

		template<class Char, int CharByte>
		uint32 StringBase<Char, CharByte>::ToUInt32(const uint64 radix) const
		{
			TO_INT_DEF(uint32, ToUInt32);
		}

		template<class Char, int CharByte>
		uint64 StringBase<Char, CharByte>::ToUInt64(const uint64 radix) const
		{
			TO_INT_DEF(uint64, ToUInt64);
		}

		template<class Char, int CharByte>
		bool StringBase<Char, CharByte>::TryToInt8(int8* outValue, const uint64 radix) const
		{
			TRY_TO_INT_DEF(int8, ToInt8);
		}

		template<class Char, int CharByte>
		bool StringBase<Char, CharByte>::TryToInt16(int16* outValue, const uint64 radix) const
		{
			TRY_TO_INT_DEF(int16, ToInt16);
		}

		template<class Char, int CharByte>
		bool StringBase<Char, CharByte>::TryToInt32(int32* outValue, const uint64 radix) const
		{
			TRY_TO_INT_DEF(int32, ToInt32);
		}

		template<class Char, int CharByte>
		bool StringBase<Char, CharByte>::TryToInt64(int64* outValue, const uint64 radix) const
		{
			TRY_TO_INT_DEF(int64, ToInt64);
		}

		template<class Char, int CharByte>
		bool StringBase<Char, CharByte>::TryToUInt8(uint8* outValue, const uint64 radix) const
		{
			TRY_TO_INT_DEF(uint8, ToUInt8);
		}

		template<class Char, int CharByte>
		bool StringBase<Char, CharByte>::TryToUInt16(uint16* outValue, const uint64 radix) const
		{
			TRY_TO_INT_DEF(int16, ToUInt16);
		}

		template<class Char, int CharByte>
		bool StringBase<Char, CharByte>::TryToUInt32(uint32* outValue, const uint64 radix) const
		{
			TRY_TO_INT_DEF(uint32, ToUInt32);
		}

		template<class Char, int CharByte>
		bool StringBase<Char, CharByte>::TryToUInt64(uint64* outValue, const uint64 radix) const
		{
			TRY_TO_INT_DEF(uint64, ToUInt64);
		}
#pragma endregion Convert Number
#pragma endregion Main Function
#pragma region Property
		/*----------------------------------------------------------------------
		*  @brief :  文字列が空かどうかを判定します
		/*----------------------------------------------------------------------*/
		template<class Char, int CharByte>
		bool StringBase<Char, CharByte>::IsEmpty() const
		{
			if (IsSSOMode())
			{
				return GetSSOLength() == 0;
			}
			else
			{
				return _data.NonSSO.Pointer == nullptr || _data.NonSSO.Size <= 0;
			}
		}

		/*----------------------------------------------------------------------
		*  @brief :  C言語としての生の文字列表現を取得します
		/*----------------------------------------------------------------------*/
		template<class Char, int CharByte>
		const Char* StringBase<Char, CharByte>::CString() const noexcept
		{
			return IsSSOMode() ? &_data.SSO.Buffer[0] : _data.NonSSO.Pointer;
		}
#pragma endregion Property
#pragma region Memory
		/*----------------------------------------------------------------------
		*  @brief :  メモリをコピーする
		/*----------------------------------------------------------------------*/
		template<class Char, int CharByte>
		void StringBase<Char, CharByte>::CopyFrom(const StringBase<Char, CharByte>& source)
		{
			if (this == &source) { return; }

			if (source.IsSSOMode())
			{
				Memory::Copy(&this->_data.SSO, &source._data.SSO, sizeof(SSOString));
			}
			else
			{
				if (this->_data.NonSSO.Pointer != nullptr) { delete[] this->_data.NonSSO.Pointer; }

				this->_data.NonSSO.Capacity = source._data.NonSSO.Capacity;
				this->_data.NonSSO.Size     = source._data.NonSSO.Size;
				this->_data.NonSSO.Pointer  = new Char[source._data.NonSSO.Size + 1];
				SetNonSSOMode();
			}
		}

		/*----------------------------------------------------------------------
		*  @brief :  メモリをコピーする
		/*----------------------------------------------------------------------*/
		template<class Char, int CharByte>
		void StringBase<Char, CharByte>::Move(StringBase<Char, CharByte>&& source) noexcept
		{
			Memory::Copy(this, &source, sizeof(source));
			source.Initialize();
		}

		/*----------------------------------------------------------------------
		*  @brief :  NonSSOの状態であるならメモリを破棄する.
		/*----------------------------------------------------------------------*/
		template<class Char, int CharByte>
		void StringBase<Char, CharByte>::Release() noexcept
		{
			if (!IsNonSSOMode()) { return; }
			if (_data.NonSSO.Pointer)
			{
				delete[] (_data.NonSSO.Pointer); // もしかしたらdeleteでOKかも
			}
		}

#pragma endregion Memory

#pragma endregion Implement
	}
	
	using string    = gu::details::StringBase<char, 1>;
	using wstring   = gu::details::StringBase<wchar, 2>;
	using u8string  = gu::details::StringBase<char8, 1>;
	using u16string = gu::details::StringBase<char16, 2>;
	using u32string = gu::details::StringBase<char32, 4>;

#if NEED_WIDE_CHAR
	using tstring = gu::wstring;
#else 
	using tstring = gu::u16string;
#endif

}

#endif