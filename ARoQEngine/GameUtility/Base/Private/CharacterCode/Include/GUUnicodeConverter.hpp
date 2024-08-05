//////////////////////////////////////////////////////////////////////////////////
///  @file   GUStringConverterBase.hpp
///  @brief  �������ϊ����܂�. @n
/// �@�@�@�@   UTF8 : CodePoint��1byte�`4byte�̉ϒ��o�C�g��ɕϊ���������. @n
///          UTF16
///          �������ɑ΂���Q�l����: https://contentsviewer.work/Master/Programming/unicode-utf-conversion/unicode-utf-conversion
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
	/*  @brief  ������ύX�̌��ʂ�\���񋓌^
	*****************************************************************************/
	enum class UnicodeConvertResult
	{
		Success         = 0, //!< ����
		SourceExhausted = 1, //!< �o�b�t�@�I�[�Ń}���`�o�C�g�������r�؂ꂽ
		TargetExhausted = 2, //!< �^�[�Q�b�g�o�b�t�@���s�����Ă���
		SourceIllegal   = 3, //!< �\�[�X�o�b�t�@���ɕs���ȕ������܂܂�Ă���
	};

	/****************************************************************************
	*				  			 GUStringConverterBase
	****************************************************************************/
	/*  @brief  �������ϊ�����N���X�ł�. @n
	*****************************************************************************/
	class UnicodeConverter
	{
	public:
		struct Options
		{
			/*! @brief �ϊ��Ɏg�p�����\�[�X�o�b�t�@�̗v�f��*/
			gu::int32 ConvertedSourceLength = 0;

			/*! @brief �ϊ��Ɏg�p�����^�[�Q�b�g�o�b�t�@�̗v�f��*/
			gu::int32 ConvertedTargetLength = 0;

			/*! @brief ������*/
			gu::int32 CharacterCount = 0;

			/*! @brief �s��������*/
			gu::int32 InvalidCharacterCount = 0;

			/*! @brief �s���������������Ƃ��ɂ��̕����ɒu������*/
			uint32 ReplacementChar = 0;
		};

		#pragma region Public Function
		/*!**********************************************************************
		*  @brief     UTF-8�������UTF-16������ɕϊ����܂�.
		*  @param[in] const uint8*      �ϊ����̕�����
		*  @param[in] const gu::uint64 �ϊ����̕�����̃T�C�Y
		*  @param[in] uint16*           �ϊ���̕�����
		*  @param[in] gu::uint64       �ϊ���̕�����̒���
		*  @param[inout] Options&      �I�v�V����
		*  @return    UnicodeConvertResult �ϊ�����
		*************************************************************************/
		static UnicodeConvertResult ConvertUTF8ToUTF16(const uint8* source, uint64 sourceLength, uint16* target, gu::uint64 targetLength, Options& options);

		/*!**********************************************************************
		*  @brief     UTF-8�������UTF-32������ɕϊ����܂�.
		*  @param[in] const uint8*      �ϊ����̕�����
		*  @param[in] const gu::uint64 �ϊ����̕�����̃T�C�Y
		*  @param[in] uint32*           �ϊ���̕�����
		*  @param[in] gu::uint64       �ϊ���̕�����̒���
		*  @param[inout] Options&      �I�v�V����
		*  @return    UnicodeConvertResult �ϊ�����
		*************************************************************************/
		static UnicodeConvertResult ConvertUTF8ToUTF32(const uint8* source, uint64 sourceLength, uint32* target, gu::uint64 targetLength, Options& options);
		
		/*!**********************************************************************
		*  @brief     UTF-16�������UTF-8������ɕϊ����܂�.
		*  @param[in] const uint16*      �ϊ����̕�����
		*  @param[in] const gu::uint64 �ϊ����̕�����̃T�C�Y
		*  @param[in] uint8*           �ϊ���̕�����
		*  @param[in] gu::uint64       �ϊ���̕�����̒���
		*  @param[inout] Options&      �I�v�V����
		*  @return    UnicodeConvertResult �ϊ�����
		*************************************************************************/
		static UnicodeConvertResult ConvertUTF16ToUTF8(const uint16* source, uint64 sourceLength, uint8* target, gu::uint64 targetLength, Options& options);

		/*!**********************************************************************
		*  @brief     UTF-16�������UTF-32������ɕϊ����܂�.
		*  @param[in] const uint16*      �ϊ����̕�����
		*  @param[in] const gu::uint64 �ϊ����̕�����̃T�C�Y
		*  @param[in] uint32*           �ϊ���̕�����
		*  @param[in] gu::uint64       �ϊ���̕�����̒���
		*  @param[inout] Options&      �I�v�V����
		*  @return    UnicodeConvertResult �ϊ�����
		*************************************************************************/
		static UnicodeConvertResult ConvertUTF16ToUTF32(const uint16* source, uint64 sourceLength, uint32* target, gu::uint64 targetLength, Options& options);
		
		/*!**********************************************************************
		*  @brief     UTF-32�������UTF-8������ɕϊ����܂�.
		*  @param[in] const uint32*      �ϊ����̕�����
		*  @param[in] const gu::uint64 �ϊ����̕�����̃T�C�Y
		*  @param[in] uint8*           �ϊ���̕�����
		*  @param[in] gu::uint64       �ϊ���̕�����̒���
		*  @param[inout] Options&      �I�v�V����
		*  @return    UnicodeConvertResult �ϊ�����
		*************************************************************************/
		static UnicodeConvertResult ConvertUTF32ToUTF8(const uint32* source, uint64 sourceLength, uint8* target, gu::uint64 targetLength, Options& options);

		/*!**********************************************************************
		*  @brief     UTF-32�������UTF-8������ɕϊ����܂�.
		*  @param[in] const uint32*     �ϊ����̕�����
		*  @param[in] const gu::uint64 �ϊ����̕�����̃T�C�Y
		*  @param[in] uint16*           �ϊ���̕�����
		*  @param[in] gu::uint64       �ϊ���̕�����̒���
		*  @param[inout] Options&      �I�v�V����
		*  @return    UnicodeConvertResult �ϊ�����
		*************************************************************************/
		static UnicodeConvertResult ConvertUTF32ToUTF16(const uint32* source, uint64 sourceLength, uint16* target, gu::uint64 targetLength, Options& options);
		
		/*!**********************************************************************
		*  @brief        UTF8�ɂ������s�o�C�g���l�����������񒷂��擾���܂�
		*  @param[in]    const uint8* �Ώە�����
		*  @param[in]    const uint64 �����񒷂�
		*  @param[in]    const bool ���s�����Ƃ��ɃG���[���o�����ǂ���
		*  @param[out]   uint64& ������
		*  @return       UnicodeConvertResult �ϊ�����
		*************************************************************************/
		static UnicodeConvertResult GetUTF8CharacterLength(const uint8* source, const uint64 sourceLength, uint64& outLength, const bool strict);
		
		/*!**********************************************************************
		*  @brief        UTF16�ɂ������s�o�C�g���l�����������񒷂��擾���܂�
		*  @param[in]    const uint16* �Ώە�����
		*  @param[in]    const uint64 �����񒷂�
		*  @param[in]    const bool ���s�����Ƃ��ɃG���[���o�����ǂ���
		*  @param[out]   uint64& ������
		*  @return       UnicodeConvertResult �ϊ�����
		*************************************************************************/
		static UnicodeConvertResult GetUTF16CharacterLength(const uint16* source, const uint64 sourceLength, uint64& outLength, const bool strict);

		/*!**********************************************************************
		*  @brief        UTF8�����̌㑱�o�C�g�����擾���܂�
		*  @param[in]    const uint8* �����̐擪�����̃|�C���^
		*  @param[in]    const uint8* �o�b�t�@�̏I�[�����̃|�C���^
		*  @param[out]   uint64& �㑱�o�C�g�� (ASCII������, �㑱�������ꍇ��0���i�[�����)
		*  @param[in]    const bool true�̏ꍇ, �s�������̃`�F�b�N���s���܂� (�s�����������������ꍇ��SourceIllegal��Ԃ�)
		*  @return       UnicodeConvertResult �ϊ�����
		*************************************************************************/
		static UnicodeConvertResult GetUTF8TrailingBytes(const uint8* source, const uint8* sourceEnd, uint64& outExtraByteCount, const bool strict);

		/*!**********************************************************************
		*  @brief     UTF16 �̃T���Q�[�g�y�A�ł��邩���m�F���� @n
		*             �T���Q�[�g�y�A��, JIS��3, 4�����̊��������ׂăT�|�[�g���邽�߂̊g�� (4�o�C�g). @n
		*             �]����Unicode�Ŗ��g�p��0xD800�`0xDBFF����ʃT���Q�[�g0xDC00�`0xDFFF�����ʃT���Q�[�g�ƋK�肵��4�o�C�g������\������@https://codezine.jp/article/detail/1592
		*  @param[in] const void* �`�F�b�N�Ώۂ̐擪�����̃|�C���^
		*  @param[in] const void* �`�F�b�N�Ώۂ̏I�[�����̃|�C���^
		*  @param[in] const bool true�̏ꍇ, �s�������̃`�F�b�N���s���܂� (�s�����������������ꍇ��SourceIllegal��Ԃ�)
		*  @param[out] bool* �T���Q�[�g�y�A�ł��邩�ǂ���
		*  @return    UnicodeConvertResult �ϊ�����
		*************************************************************************/
		static UnicodeConvertResult CheckUTF16Surrogate(const uint16* sourceStart, const uint16* sourceEnd, const bool strict, bool* outSurrogate);
		
		/*!**********************************************************************
		*  @brief     UTF16�̏�ʃT���Q�[�g�ł��邩���m�F����
		*  @param[in] const uint16 �`�F�b�N�Ώۂ̕���
		*  @return    bool
		*************************************************************************/
		static bool CheckUTF16HighSurrogate(const uint16 codeUnit) { return SURROGATE_HIGH_START <= codeUnit && codeUnit <= SURROGATE_HIGH_END; };
		
		/*!**********************************************************************
		*  @brief     UTF16�̉��ʃT���Q�[�g�ł��邩���m�F����
		*  @param[in] const uint16 �`�F�b�N�Ώۂ̕���
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
		*  @brief        UTF-8������UTF-32�ɕϊ����܂�. (1������). CodePoint��, U+0000�`U+10FFFF�܂ł͈̔͂̒l�ł�.
		*  @param[in]    const uint8**    �ϊ����̕�����
		*  @param[in]    const uint8*     �ϊ����̏I�[����
		*  @param[out]   uint32&          �ϊ���̕���
		*  @param[inout] Options&        �I�v�V����
		*  @return       UnicodeConvertResult �ϊ�����
		*************************************************************************/
		static UnicodeConvertResult CodePointUTF8ToUTF32(const uint8** sourceStart, const uint8* sourceEnd, uint32& target, [[maybe_unused]]Options& options);
		
		/*!**********************************************************************
		*  @brief        UTF-16������UTF-32�ɕϊ����܂�.
		*  @param[in]    const uint16**    �ϊ����̕�����
		*  @param[in]    
		*  @param[out]   gu::char32*      �ϊ���̕�����
		*  @param[inout] Options&         �I�v�V����
		*  @return       UnicodeConvertResult �ϊ�����
		*************************************************************************/
		static UnicodeConvertResult CodePointUTF16ToUTF32(const uint16** sourceStart, const uint16* sourceEnd, uint32& target, Options& options);

		/*!**********************************************************************
		*  @brief        UTF-32������UTF-8�ɕϊ����܂�.
		*  @param[in]    const uint32     �ϊ����̕���
		*  @param[in]    const uint8**    �ϊ���̕�����|�C���^
		*  @param[out]   const uint8*     �ϊ���̏I�[����
		*  @param[inout] Options&        �I�v�V����
		*  @return       UnicodeConvertResult �ϊ�����
		*************************************************************************/
		static UnicodeConvertResult CodePointUTF32ToUTF8(uint32 source, uint8** targetStart, const uint8* targetEnd, Options& options);

		/*!**********************************************************************
		*  @brief        UTF-32������UTF-16�ɕϊ����܂�.
		*  @param[in]    const uint32     �ϊ����̕���
		*  @param[in]    const uint16**   �ϊ���̕�����|�C���^
		*  @param[out]   const uint16*    �ϊ���̏I�[����
		*  @param[inout] Options&        �I�v�V����
		*  @return       UnicodeConvertResult �ϊ�����
		*************************************************************************/
		static UnicodeConvertResult CodePointUTF32ToUTF16(uint32 source, uint16** targetStart, const uint16* targetEnd, Options& options);
		#pragma endregion Covert Char Function

		/*!**********************************************************************
		*  @brief        �K�؂�UTF8�R�[�f�b�N���ǂ���
		*  @param[in]    const uint8* �ϊ���
		*  @param[in]    const uint64 �����񒷂�
		*  @return       bool
		*************************************************************************/
		static bool IsLegalUTF8(const uint8* source, const uint64 length);

		/*!**********************************************************************
		*  @brief        �s�����������e���邩�ǂ���. (ReplacementChar��0�ł���Εs�������͈�؋��e����Ȃ�)
		*  @param[in]    const Options& option
		*  @return       bool true�ŕs�����������e���Ȃ�. false�ŕs�����������e
		*************************************************************************/
		__forceinline static bool IsStrictConversion(const Options& option) { return (option.ReplacementChar == 0); }
		#pragma endregion 

		#pragma region Private Property
		/*! @brief �ǂݎ��\��UTF32�̍ő�l*/
		static constexpr uint32 MAX_LEGAL_UTF32 = 0x0010FFFF;

		/*! @brief �s�������������������̒u������*/
		static constexpr uint32 REPLACEMENT_CHAR = 0x0000FFFD;

		/*! @brief UTF32 CodePoint��2��16bit�ɕϊ�����ꍇ�̏��CodeUnit�̉���*/
		static constexpr uint32 SURROGATE_HIGH_START = 0x0000D800;

		/*! @brief UTF32 CodePoint��2��16bit�ɕϊ�����ꍇ�̏��CodeUnit�̏��*/
		static constexpr uint32 SURROGATE_HIGH_END = 0x0000DBFF;

		/*! @brief UTF32 CodePoint��2��16bit�ɕϊ�����ꍇ�̉���CodeUnit�̉���*/
		static constexpr uint32 SURROGATE_LOW_START = 0x0000DC00;

		/*! @brief UTF32 CodePoint��2��16bit�ɕϊ�����ꍇ�̉���CodeUnit�̏��*/
		static constexpr uint32 SURROGATE_LOW_END = 0x0000DFFF;

		/*! @brief UTF32 CodePoint��2��16bit�ɕϊ�����ꍇ�̏��CodeUnit�̃V�t�g��*/
		static constexpr uint32 SURROGATE_HIGH_SHIFT = 10;

		/*! @brief UTF32 CodePoint��2��16bit�ɕϊ�����ꍇ�̉���CodeUnit�̃x�[�X*/
		static constexpr uint32 SURROGATE_LOW_BASE = 0x00010000UL;

		/*! @brief UTF32 CodePoint��2��16bit�ɕϊ�����ꍇ��UTF32��UTF16�ϊ����̉��ʃ}�X�N*/
		static constexpr uint32 SURROGATE_LOW_MASK = 0x3FFUL;

		/*! @brief Basic Multilingual Plane */
		static constexpr uint32 MAX_BMP = 0x0000FFFF;
		#pragma endregion 

	};

}
#endif