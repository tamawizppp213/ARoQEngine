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
	*				 StringConvertResult 		 
	*************************************************************************//**
	/*  @brief  ������ύX�̌��ʂ�\���񋓌^
	*****************************************************************************/
	enum class StringConvertResult
	{
		Success         = 0, //!< ����
		SourceExhausted = 1, //!< �o�b�t�@�I�[�Ń}���`�o�C�g�������r�؂ꂽ
		TargetExhausted = 2, //!< �^�[�Q�b�g�o�b�t�@���s�����Ă���
		SourceIllegal   = 3, //!< �\�[�X�o�b�t�@���ɕs���ȕ������܂܂�Ă���
	};

	/****************************************************************************
	*				  			 GUStringConverterBase
	*************************************************************************//**
	/*  @brief  �������ϊ�����N���X�ł�. @n
	*****************************************************************************/
	class StringConverterBase
	{
	private:
		using UTF8  = uint8;
		using UTF16 = uint16;
		using UTF32 = uint32;

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
			UTF32 ReplacementChar = 0;
		};

	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief     UTF-8�������UTF-16������ɕϊ����܂�.
		*  @param[in] const UTF8*      �ϊ����̕�����
		*  @param[in] const gu::uint64 �ϊ����̕�����̃T�C�Y
		*  @param[in] UTF16*           �ϊ���̕�����
		*  @param[in] gu::uint64       �ϊ���̕�����̒���
		*  @param[inout] Options&      �I�v�V����
		*  @return    StringConvertResult �ϊ�����
		*************************************************************************/
		static StringConvertResult ConvertUTF8ToUTF16(const UTF8* source, uint64 sourceLength, UTF16* target, gu::uint64 targetLength, Options& options);

		/*!**********************************************************************
		*  @brief     UTF-8�������UTF-32������ɕϊ����܂�.
		*  @param[in] const UTF8*      �ϊ����̕�����
		*  @param[in] const gu::uint64 �ϊ����̕�����̃T�C�Y
		*  @param[in] UTF32*           �ϊ���̕�����
		*  @param[in] gu::uint64       �ϊ���̕�����̒���
		*  @param[inout] Options&      �I�v�V����
		*  @return    StringConvertResult �ϊ�����
		*************************************************************************/
		static StringConvertResult ConvertUTF8ToUTF32(const UTF8* source, uint64 sourceLength, UTF32* target, gu::uint64 targetLength, Options& options);
		
		/*!**********************************************************************
		*  @brief     UTF-16�������UTF-8������ɕϊ����܂�.
		*  @param[in] const UTF16*      �ϊ����̕�����
		*  @param[in] const gu::uint64 �ϊ����̕�����̃T�C�Y
		*  @param[in] UTF8*           �ϊ���̕�����
		*  @param[in] gu::uint64       �ϊ���̕�����̒���
		*  @param[inout] Options&      �I�v�V����
		*  @return    StringConvertResult �ϊ�����
		*************************************************************************/
		static StringConvertResult ConvertUTF16ToUTF8(const UTF16* source, uint64 sourceLength, UTF8* target, gu::uint64 targetLength, Options& options);

		/*!**********************************************************************
		*  @brief     UTF-16�������UTF-32������ɕϊ����܂�.
		*  @param[in] const UTF16*      �ϊ����̕�����
		*  @param[in] const gu::uint64 �ϊ����̕�����̃T�C�Y
		*  @param[in] UTF32*           �ϊ���̕�����
		*  @param[in] gu::uint64       �ϊ���̕�����̒���
		*  @param[inout] Options&      �I�v�V����
		*  @return    StringConvertResult �ϊ�����
		*************************************************************************/
		static StringConvertResult ConvertUTF16ToUTF32(const UTF16* source, uint64 sourceLength, UTF32* target, gu::uint64 targetLength, Options& options);
		
		/*!**********************************************************************
		*  @brief     UTF-32�������UTF-8������ɕϊ����܂�.
		*  @param[in] const UTF32*      �ϊ����̕�����
		*  @param[in] const gu::uint64 �ϊ����̕�����̃T�C�Y
		*  @param[in] UTF8*           �ϊ���̕�����
		*  @param[in] gu::uint64       �ϊ���̕�����̒���
		*  @param[inout] Options&      �I�v�V����
		*  @return    StringConvertResult �ϊ�����
		*************************************************************************/
		static StringConvertResult ConvertUTF32ToUTF8(const UTF32* source, uint64 sourceLength, UTF8* target, gu::uint64 targetLength, Options& options);

		/*!**********************************************************************
		*  @brief     UTF-32�������UTF-8������ɕϊ����܂�.
		*  @param[in] const UTF32*     �ϊ����̕�����
		*  @param[in] const gu::uint64 �ϊ����̕�����̃T�C�Y
		*  @param[in] UTF16*           �ϊ���̕�����
		*  @param[in] gu::uint64       �ϊ���̕�����̒���
		*  @param[inout] Options&      �I�v�V����
		*  @return    StringConvertResult �ϊ�����
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
		*  @brief        UTF-8������UTF-32�ɕϊ����܂�. (1������). CodePoint��, U+0000�`U+10FFFF�܂ł͈̔͂̒l�ł�.
		*  @param[in]    const UTF8**    �ϊ����̕�����
		*  @param[in]    const UTF8*     �ϊ����̏I�[����
		*  @param[out]   UTF32&          �ϊ���̕���
		*  @param[inout] Options&        �I�v�V����
		*  @return       StringConvertResult �ϊ�����
		*************************************************************************/
		static StringConvertResult CodePointUTF8ToUTF32(const UTF8** sourceStart, const UTF8* sourceEnd, UTF32& target, Options& options);
		
		/*!**********************************************************************
		*  @brief        UTF-16������UTF-32�ɕϊ����܂�.
		*  @param[in]    const UTF16**    �ϊ����̕�����
		*  @param[in]    
		*  @param[out]   gu::char32*      �ϊ���̕�����
		*  @param[inout] Options&         �I�v�V����
		*  @return       StringConvertResult �ϊ�����
		*************************************************************************/
		static StringConvertResult CodePointUTF16ToUTF32(const UTF16** sourceStart, const UTF16* sourceEnd, UTF32& target, Options& options);

		/*!**********************************************************************
		*  @brief        UTF-32������UTF-8�ɕϊ����܂�.
		*  @param[in]    const UTF32     �ϊ����̕���
		*  @param[in]    const UTF8**    �ϊ���̕�����|�C���^
		*  @param[out]   const UTF8*     �ϊ���̏I�[����
		*  @param[inout] Options&        �I�v�V����
		*  @return       StringConvertResult �ϊ�����
		*************************************************************************/
		static StringConvertResult CodePointUTF32ToUTF8(UTF32 source, UTF8** targetStart, const UTF8* targetEnd, Options& options);

		/*!**********************************************************************
		*  @brief        UTF-32������UTF-16�ɕϊ����܂�.
		*  @param[in]    const UTF32     �ϊ����̕���
		*  @param[in]    const UTF16**   �ϊ���̕�����|�C���^
		*  @param[out]   const UTF16*    �ϊ���̏I�[����
		*  @param[inout] Options&        �I�v�V����
		*  @return       StringConvertResult �ϊ�����
		*************************************************************************/
		static StringConvertResult CodePointUTF32ToUTF16(UTF32 source, UTF16** targetStart, const UTF16* targetEnd, Options& options);
		#pragma endregion Covert Char Function

		/*!**********************************************************************
		*  @brief        �K�؂�UTF8�R�[�f�b�N���ǂ���
		*  @param[in]    const UTF8* �ϊ���
		*  @param[in]    const uint64 �����񒷂�
		*  @return       bool
		*************************************************************************/
		static bool IsLegalUTF8(const UTF8* source, const uint64 length);

		/*!**********************************************************************
		*  @brief        �s�����������e���邩�ǂ���. (ReplacementChar��0�ł���Εs�������͈�؋��e����Ȃ�)
		*  @param[in]    const Options& option
		*  @return       bool true�ŕs�����������e���Ȃ�. false�ŕs�����������e
		*************************************************************************/
		__forceinline static bool IsStrictConversion(const Options& option) { return (option.ReplacementChar == 0); }
		#pragma endregion 

		#pragma region Private Property
		/*! @brief �ǂݎ��\��UTF32�̍ő�l*/
		static constexpr UTF32 MAX_LEGAL_UTF32 = 0x0010FFFF;

		/*! @brief �s�������������������̒u������*/
		static constexpr UTF32 REPLACEMENT_CHAR = 0x0000FFFD;

		/*! @brief UTF32 CodePoint��2��16bit�ɕϊ�����ꍇ�̏��CodeUnit�̉���*/
		static constexpr UTF32 SURROGATE_HIGH_START = 0x0000D800;

		/*! @brief UTF32 CodePoint��2��16bit�ɕϊ�����ꍇ�̏��CodeUnit�̏��*/
		static constexpr UTF32 SURROGATE_HIGH_END = 0x0000DBFF;

		/*! @brief UTF32 CodePoint��2��16bit�ɕϊ�����ꍇ�̉���CodeUnit�̉���*/
		static constexpr UTF32 SURROGATE_LOW_START = 0x0000DC00;

		/*! @brief UTF32 CodePoint��2��16bit�ɕϊ�����ꍇ�̉���CodeUnit�̏��*/
		static constexpr UTF32 SURROGATE_LOW_END = 0x0000DFFF;

		/*! @brief UTF32 CodePoint��2��16bit�ɕϊ�����ꍇ�̏��CodeUnit�̃V�t�g��*/
		static constexpr UTF32 SURROGATE_HIGH_SHIFT = 10;

		/*! @brief UTF32 CodePoint��2��16bit�ɕϊ�����ꍇ�̉���CodeUnit�̃x�[�X*/
		static constexpr UTF32 SURROGATE_LOW_BASE = 0x00010000UL;

		/*! @brief UTF32 CodePoint��2��16bit�ɕϊ�����ꍇ��UTF32��UTF16�ϊ����̉��ʃ}�X�N*/
		static constexpr UTF32 SURROGATE_LOW_MASK = 0x3FFUL;

		/*! @brief Basic Multilingual Plane */
		static constexpr UTF32 MAX_BMP = 0x0000FFFF;
		#pragma endregion 

	};

}
#endif