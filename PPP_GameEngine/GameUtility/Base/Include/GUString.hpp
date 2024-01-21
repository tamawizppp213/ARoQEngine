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
#include <string>
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
			*  @brief :  文字列を代入します
			/*----------------------------------------------------------------------*/
			void Assign(const Char* string);
			void Assign(const Char* string, const uint64 length) noexcept;

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
			*  @brief :  指定した文字列がこの文字列内に存在するかを判断します. 
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
			/*----------------------------------------------------------------------*/
			uint64 FindFirstIndexOf(const Char* string, const uint64 startIndex = 0, const bool useCaseSensitivity = true) const;
			uint64 FindFirstIndexOf(const StringBase<Char, CharByte>& string, const uint64 startIndex = 0, const bool useCaseSensititivity = true) const;

			/*----------------------------------------------------------------------
			*  @brief :  文字列を検索し, 最後に見つかったの文字のインデックスを返します.
			*            見つからなかった場合は-1, stringが空文字列である場合は0
			/*----------------------------------------------------------------------*/
			uint64 FindLastIndexOf(const Char* string, const uint64 startIndex = NPOS, const uint64 count = NPOS, const bool useCaseSensitivity = true) const;
			uint64 FindLastIndexOf(const StringBase<Char, CharByte>& string, const uint64 startIndex = NPOS, const uint64 count = NPOS, const bool useCaseSensitivity = true) const;

			/****************************************************************************
			**                Public Member Variables
			*****************************************************************************/
			static constexpr uint64 NPOS = static_cast<uint64>(-1);

#pragma region Property
			/*----------------------------------------------------------------------
			*  @brief :  文字列が空かどうかを判定します
			/*----------------------------------------------------------------------*/
			inline bool IsEmpty() const;

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
			StringBase<Char, CharByte>& operator=(const Char* right) { Assign(right)    ; return *this; }
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

			explicit StringBase(const StringBase<Char, CharByte>& string) : StringBase<Char, CharByte>()
			{
				Copy(string);
			}

			explicit StringBase(StringBase<Char, CharByte>&& string) noexcept : StringBase<Char, CharByte>()
			{
				Move(string);
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
			void CopyTo(const StringBase<Char, CharByte>& destination);

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
		void StringBase<Char, CharByte>::CopyTo(const StringBase<Char, CharByte>& destination)
		{
			if (this == &destination) { return; }

			if (IsSSO())
			{
				Memory::Copy(&destination._data.SSO, this->_data.SSO, sizeof(SSOString));
			}
			else
			{
				if (destination._data.NonSSO.Pointer != nullptr) { delete[] this->_data.NonSSO.Pointer; }
				destination._data.NonSSO.Capacity = this->_data.NonSSO.Capacity;
				destination._data.NonSSO.Size     = this->_data.NonSSO.Size;
				destination._data.NonSSO.Pointer  = new Char[this->_data.NonSSO.Size + 1];
			}
		}

		/*----------------------------------------------------------------------
		*  @brief :  メモリをコピーする
		/*----------------------------------------------------------------------*/
		template<class Char, int CharByte>
		void StringBase<Char, CharByte>::Move(StringBase<Char, CharByte>&& source) noexcept
		{
			Memory::Copy(this, source, sizeof(source));
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
	
	using string  = std::string;

	using wstring = std::wstring;
#if NEED_WIDE_CHAR
	using tstring = std::wstring;
#else 
	using tstring = std::u16string;
#endif

}

#endif