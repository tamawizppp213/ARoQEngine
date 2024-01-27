//////////////////////////////////////////////////////////////////////////////////
///             @file   GUString.hpp
///             @brief  ���I�ȕ�����N���X�ł�. std::string�Ɠ����Ɉ�����N���X
///                     ������
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
		*  @brief     �v���~�e�B�u�Ȍ^�ł���Ƃ������f�̉�, �����gu::uint32�̂悤�ɏ������X�^�[�g�ōs�����Ƃɂ������܂���.
		*****************************************************************************/
		template<class Char, int CharByte = 1>
		class StringBase : public Copyable
		{
		public:
			/****************************************************************************
			**                Public Function
			*****************************************************************************/
			/*----------------------------------------------------------------------
			*  @brief :  �������A�����܂�
			/*----------------------------------------------------------------------*/
			static StringBase<Char, CharByte> Concat(const StringBase<Char, CharByte>& left, const StringBase<Char, CharByte>& right);
			static StringBase<Char, CharByte> Concat(const Char* left, const Char* right);

			/*----------------------------------------------------------------------
			*  @brief :  ������������܂�
			/*----------------------------------------------------------------------*/
			void Assign(const Char* string);
			void Assign(const Char* string, const uint64 length) noexcept;
			void Assign(const StringBase<Char, CharByte>& string) { Assign(string.CString(), string.Size()); }

			/*----------------------------------------------------------------------
			*  @brief :  �������ǉ����܂�
			/*----------------------------------------------------------------------*/
			void Append(const Char* string);
			void Append(const Char* string, const uint64 length);
			void Append(const StringBase<Char, CharByte>& string);

			/*----------------------------------------------------------------------
			*  @brief :  Capacity�⃁�����͓��ɔj�������A������̐擪�݂̂��I�[�����ɕύX���܂�.
			*            �܂��ASSO�Ȃǂ̃��[�h�ؑւ��s���܂���.
			/*----------------------------------------------------------------------*/
			void Clear();

			/*----------------------------------------------------------------------
			*  @brief :  �����������O�Ɋm�ۂ��܂�. (NonSSO�̂Ƃ�����capacity�ȏ�ł���Ίm�ۂ��܂�)
			/*----------------------------------------------------------------------*/
			void Reserve(const uint64 length);

			/*----------------------------------------------------------------------
			*  @brief :  �������u�����܂�
			*
			*  @param[in] const Char* from �u������镶����
			*  @param[in] const Char* to   �u�����镶����
			/*----------------------------------------------------------------------*/
			StringBase<Char, CharByte> Replace(const Char* from, const Char* to, const bool useCaseSensitivity);
			StringBase<Char, CharByte> Replace(const StringBase<Char, CharByte>& from, const StringBase<Char, CharByte>& to, const bool useCaseSensitivity);

			/*----------------------------------------------------------------------
			*  @brief :  �w�肵�������񂪂��̕�������ɑ��݂��邩�𔻒f���܂�.
			*  
			*  @param[in] : const Char* string              ����������
			*  @param[in] : const bool  useCaseSensitivity  �啶���Ə������̋�� 
			* 
			*  @return    : �����񂪑��݂����true, string���󕶎���ł���ꍇ��true�ƂȂ�
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
			*  @brief :  �������������, ���������ŏ��̕����̃C���f�b�N�X��Ԃ��܂�.
			*            ������Ȃ������ꍇ��-1, string���󕶎���ł���ꍇ��0
			*  
			*  @param[in] : const Char* ����������
			*  @param[in] : const uint64 �������J�n����C���f�b�N�X (�ȗ������ꍇ�͐擪���猟�����܂�)
			*  @param[in] : const bool �啶���Ə������̋��
			* 
			*  @return ������Ȃ�������NPOS, string���󕶎���ł���ꍇ��0
			/*----------------------------------------------------------------------*/
			uint64 FindFirstIndexOf(const Char* string, const uint64 startIndex = 0, const bool useCaseSensitivity = true) const;
			uint64 FindFirstIndexOf(const StringBase<Char, CharByte>& string, const uint64 startIndex = 0, const bool useCaseSensititivity = true) const;

			/*----------------------------------------------------------------------
			*  @brief :  �������������, �Ō�Ɍ��������̕����̃C���f�b�N�X��Ԃ��܂�.
			*            ������Ȃ������ꍇ��-1, string���󕶎���ł���ꍇ��0
			/*----------------------------------------------------------------------*/
			uint64 FindLastIndexOf(const Char* string, const uint64 startIndex = NPOS, const uint64 count = NPOS, const bool useCaseSensitivity = true) const;
			uint64 FindLastIndexOf(const StringBase<Char, CharByte>& string, const uint64 startIndex = NPOS, const uint64 count = NPOS, const bool useCaseSensitivity = true) const;

			/*----------------------------------------------------------------------
			*  @brief :  ������̐擪���w�肵��������ƈ�v���邩�𔻒f���܂�
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
			*  @brief :  ������̕���������𒊏o���܂�
			* 
			*  @param[in] const uint64 startIndex �������J�n����C���f�b�N�X
			*  @param[in] const uint64 count      ������(NPOS�̏ꍇ, �����܂Œ��o����)
			* 
			*  @param[in] ���o���ꂽ������
			/*----------------------------------------------------------------------*/
			__forceinline StringBase<Char, CharByte> SubString(const uint64 startIndex, const uint64 count = NPOS) const
			{
				const Char* begin = nullptr;
				const Char* end   = nullptr;
				StringUtility::SubString(CString(), Size(), startIndex, count, &begin, &end);
				return StringBase<Char, CharByte>(begin, end);
			}

			/*----------------------------------------------------------------------
			*  @brief :  ������̐擪����w�肵���������𒊏o���܂�. 
			/*----------------------------------------------------------------------*/
			__forceinline StringBase<Char, CharByte> Left(const uint64 count) const
			{
				const Char* begin = nullptr;
				const Char* end   = nullptr;
				StringUtility::Left(CString(), count, &count, &end);
				return StringBase<Char, CharByte>(begin, end);
			}

			/*----------------------------------------------------------------------
			*  @brief :  ������̖�������w�肵���������𒊏o���܂�.
			/*----------------------------------------------------------------------*/
			StringBase<Char, CharByte> Right(const uint64 count) const
			{
				const Char* begin = nullptr;
				const Char* end   = nullptr;
				StringUtility::Right(CString(), count, &count, &end);
				return StringBase<Char, CharByte>(begin, end);
			}

			/*----------------------------------------------------------------------
			*  @brief :  ��������S�đ啶���ɕϊ������������Ԃ��܂�
			/*----------------------------------------------------------------------*/
			StringBase<Char, CharByte> ToUpper() const;

			/*----------------------------------------------------------------------
			*  @brief :  �啶����S�ď������ɕϊ������������Ԃ��܂�
			/*----------------------------------------------------------------------*/
			StringBase<Char, CharByte> ToLower() const;

			/*----------------------------------------------------------------------
			*  @brief :  ������̐擪�Ɩ����̋󔒂�S�č폜�����������Ԃ��܂�
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
			*  @brief :  �͈̓`�F�b�N�t���̗v�f�A�N�Z�X
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
			*  @brief :  �����񂪋󂩂ǂ����𔻒肵�܂�
			/*----------------------------------------------------------------------*/
			bool IsEmpty() const;

			/*----------------------------------------------------------------------
			*  @brief :  C����Ƃ��Ă̐��̕�����\�����擾���܂�
			/*----------------------------------------------------------------------*/
			const Char* CString() const noexcept;

			/*----------------------------------------------------------------------
			*  @brief :  ������̒������擾���܂�
			/*----------------------------------------------------------------------*/
			uint64 Size() const noexcept { return IsSSOMode() ? _data.SSO.Size >> 1 : _data.NonSSO.Size >> 1; }

			/*----------------------------------------------------------------------
			*  @brief :  ���������Ċm�ۂ����Ɋi�[�ł���ő�̗v�f�����擾���܂�
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

			// �������̂��߂Ƀ`�F�b�N�����Ă���܂���. �`�F�b�N���s���ꍇ��At���g�p���Ă�������.
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
			// @brief : SSO�̃o�b�t�@�T�C�Y
			static constexpr gu::uint64 SSO_CAPACITY = 15;

			/****************************************************************************
			**                Struct
			*****************************************************************************/
			/*----------------------------------------------------------------------
			*  @brief :  �����������Ȃ��Ƃ���new�������Ȃ����߂ɍs��Small String Optimization
			/*----------------------------------------------------------------------*/
			struct SSOString
			{
				// @brief : ������
				uint64 Size; // ---xxxxy : x = size, y:ssoFlag on 0, off 1

				// @brief : SSO�Ŏg�p����16�o�C�g�̕����o�b�t�@ (�Ō�1byte�͏I�[�����ł�)
				Char Buffer[SSO_CAPACITY + 1];
			};

			/*----------------------------------------------------------------------
			*  @brief :  �����������Ȃ��Ƃ���new�������Ȃ����߂ɍs��Small String Optimization
			/*----------------------------------------------------------------------*/
			struct NonSSOString
			{
				// @brief : ������
				uint64 Size;

				// @brief : ���I�m�ۂ��郁�����̐擪�|�C���^
				Char*  Pointer;

				// @brief : ���I�m�ۂ����z��Ɋi�[�ł��镶����̍ő咷��
				uint64 Capacity;
			};

			/*----------------------------------------------------------------------
			*  @brief :  ��������
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
			*  @brief :  ��̔z��ɂ�鏉����Ԃ̍쐬
			/*----------------------------------------------------------------------*/
			__forceinline void Initialize() noexcept
			{
				_data.SSO.Size = 0;
				SetSSOMode();
				Memory::Zero(_data.SSO.Buffer, SSO_CAPACITY + 1);
			}

			/*----------------------------------------------------------------------
			*  @brief :  NonSSO�̏�Ԃł���Ȃ烁������j������
			/*----------------------------------------------------------------------*/
			void Release() noexcept;

			/*----------------------------------------------------------------------
			*  @brief :  ������̃R�s�[
			/*----------------------------------------------------------------------*/
			void CopyFrom(const StringBase<Char, CharByte>& source);

			/*----------------------------------------------------------------------
			*  @brief :  ���������ړ�����
			/*----------------------------------------------------------------------*/
			void Move(StringBase<Char, CharByte>&& source) noexcept;

			/*----------------------------------------------------------------------
			*  @brief :  ������̒����擾
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
			*  @brief :  SSO���g�p���� (�����̕����z����g�p����)���[�h�ɐݒ肵�܂�
			/*----------------------------------------------------------------------*/
			__forceinline void SetSSOMode() { _data.SSO.Size &= ~(0x1);}

			/*----------------------------------------------------------------------
			*  @brief :  SSO���g�p���Ȃ� (new�ŗ̈���m�ۂ���)���[�h�ɐݒ肵�܂�
			/*----------------------------------------------------------------------*/
			__forceinline void SetNonSSOMode() { _data.SSO.Size |= 0x1;}

			/*----------------------------------------------------------------------
			*  @brief :  �����̃r�b�g���g����SSO���g�������񉻂�Ԃ��܂�
			/*----------------------------------------------------------------------*/
			bool IsSSOMode() const noexcept 
			{
				return  !(static_cast<uint8>(_data.SSO.Size) & 0x1);
			}

			/*----------------------------------------------------------------------
			*  @brief :  SSO���g��Ȃ������񂩂�Ԃ��܂�
			/*----------------------------------------------------------------------*/
			bool IsNonSSOMode() const noexcept 
			{ 
				return (static_cast<uint8>(_data.SSO.Size) & 0x1); 
			}

			/*----------------------------------------------------------------------
			*  @brief :  SSO�\���̂ɂ����镶���񒷂�Ԃ��܂�. 
			*            Size�\���̂�, ---xxxxy : x = size, y:ssoFlag (0:sso, 1:nonSSO)�Ƃ��Ĉ�������, �E�V�t�g�������Ă��܂�. 
			/*----------------------------------------------------------------------*/
			__forceinline uint64 GetSSOLength() const noexcept { return _data.SSO.Size >> 1; }

			/*----------------------------------------------------------------------
			*  @brief :  SSO�̕�������ݒ肵�܂�
			/*----------------------------------------------------------------------*/
			__forceinline void SetSSOLength(const uint64 length)
			{
				_data.SSO.Size = (length & 0x7F) << 1;
				_data.SSO.Buffer[length] = '\0'; // �I�[����
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
			// ���蓖��
			Assign(string, CStringLength(string));
		}

		template<class Char, int CharByte>
		void StringBase<Char, CharByte>::Assign(const Char* string, const uint64 length) noexcept
		{
			/*-------------------------------------------------------------------
			-        SSO���g�p���郂�[�h�̏ꍇ��, ���̂܂܎w��̕����������z��ɃR�s�[
			---------------------------------------------------------------------*/
			if (length <= SSO_CAPACITY)
			{
				// �f�[�^��z��Ɋi�[
				Memory::Copy(this->_data.SSO.Buffer, string, length * CharByte);
				// SSO�Ƃ��Đݒ肷��
				SetSSOLength(length);
				SetSSOMode();
			}
			/*-------------------------------------------------------------------
			-        SSO���g�p���Ȃ��ꍇ�́A���Ƃ��Ƃ̕���������j�����āA�������V�K���蓖��
			---------------------------------------------------------------------*/
			else
			{
				// ���Ƃ��Ǝ����Ă��镶����͔j��
				Release();

				// �V�K�f�[�^�̒ǉ�
				_data.NonSSO.Pointer  = new Char[length + 1];
				_data.NonSSO.Capacity = length;
				Memory::Copy(this->_data.NonSSO.Pointer, string, length * CharByte);
				SetNonSSOLength(length);
				
				// SSO���g�p���Ȃ��ݒ�
				SetNonSSOMode();
			}
		}

		/*----------------------------------------------------------------------
		*  @brief :  �������ǉ����܂�
		/*----------------------------------------------------------------------*/
		template<class Char, int CharByte>
		void StringBase<Char, CharByte>::Append(const Char* string, const uint64 length)
		{
			const auto totalLength = Size() + length;
			const auto firstLength = Size();

			/*-------------------------------------------------------------------
			-        Total��SSO�ł������ꍇ��, ���̂܂ܔz��̌��ɒǉ�����
			---------------------------------------------------------------------*/
			if (totalLength <= SSO_CAPACITY)
			{
				Memory::Copy(&this->_data.SSO.Buffer[firstLength], string, length * sizeof(Char));
				SetSSOLength(totalLength);
				SetSSOMode();
				return;
			}

			/*-------------------------------------------------------------------
			-        Total��NonSSO�ł��邪,SSO�ł������ꍇ��, �����I�Ƀq�[�v���쐬����
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
				-        Capacity���ł���΃q�[�v����炸�ɒǉ���, �����łȂ���ΐV�����q�[�v���쐬��, �܂Ƃ߂Ēǉ�����
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
		*  @brief :  Capacity�⃁�����͓��ɔj�������A������̐擪�݂̂��I�[�����ɕύX���܂�. 
		*            �܂��ASSO�Ȃǂ̃��[�h�ؑւ��s���܂���. 
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
		*  @brief :  �����������O�Ɋm�ۂ��܂�. (capacity�ȏ�ł���΃������m�ۂ��܂�. �Â�NonSSO���[�h�Ɏg�p���܂�)
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
		*  @brief :  �������������, ���������ŏ��̕����̃C���f�b�N�X��Ԃ��܂�.
		*            ������Ȃ������ꍇ��-1(npos), string���󕶎���ł���ꍇ��0
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
		*  @brief :  �������������, �Ō�Ɍ��������̕����̃C���f�b�N�X��Ԃ��܂�.
		*            ������Ȃ������ꍇ��-1, string���󕶎���ł���ꍇ��0
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
		*  @brief :  ��������S�đ啶���ɕϊ������������Ԃ��܂�
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
		*  @brief :  �啶����S�ď������ɕϊ������������Ԃ��܂�
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
		*  @brief :  �������A�����܂�
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
		*  @brief :  �������A�����܂�
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
		*  @brief :  �������u�����܂�
		*
		*  @param[in] const Char* from �u������镶����
		*  @param[in] const Char* to   �u�����镶����
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
		*  @brief :  �����񂪋󂩂ǂ����𔻒肵�܂�
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
		*  @brief :  C����Ƃ��Ă̐��̕�����\�����擾���܂�
		/*----------------------------------------------------------------------*/
		template<class Char, int CharByte>
		const Char* StringBase<Char, CharByte>::CString() const noexcept
		{
			return IsSSOMode() ? &_data.SSO.Buffer[0] : _data.NonSSO.Pointer;
		}
#pragma endregion Property
#pragma region Memory
		/*----------------------------------------------------------------------
		*  @brief :  ���������R�s�[����
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
		*  @brief :  ���������R�s�[����
		/*----------------------------------------------------------------------*/
		template<class Char, int CharByte>
		void StringBase<Char, CharByte>::Move(StringBase<Char, CharByte>&& source) noexcept
		{
			Memory::Copy(this, &source, sizeof(source));
			source.Initialize();
		}

		/*----------------------------------------------------------------------
		*  @brief :  NonSSO�̏�Ԃł���Ȃ烁������j������.
		/*----------------------------------------------------------------------*/
		template<class Char, int CharByte>
		void StringBase<Char, CharByte>::Release() noexcept
		{
			if (!IsNonSSOMode()) { return; }
			if (_data.NonSSO.Pointer)
			{
				delete[] (_data.NonSSO.Pointer); // ������������delete��OK����
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