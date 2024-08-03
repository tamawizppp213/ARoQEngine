//////////////////////////////////////////////////////////////////////////////////
///  @file   JsonWritePolicy.hpp
///  @brief  Json�t�@�C���̏������ݕ��j
///  @author toide
///  @date   2024/07/30 1:27:00
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef JSON_WRITE_POLICY_HPP
#define JSON_WRITE_POLICY_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/GUClassUtility.hpp"
#include "GameUtility/Base/Include/GUString.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace gu::file::json
{
	/****************************************************************************
	*				  			   JsonWritePolicy
	****************************************************************************/
	/* @brief  Json�t�@�C���̏������ݕ��j
	*****************************************************************************/
	class JsonWritePolicy : public gu::NonCopyable
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief        �������ꕶ���ǉ����܂�. 
		*  @param[inout] gu::tstring& : ���͕�����
		*  @param[in]    const tchar   : �ǉ�����������
		*  @return       void
		*************************************************************************/
		__forceinline static void WriteChar(gu::tstring& string, const tchar character)
		{
			string += character;
		}

		/*!**********************************************************************
		*  @brief        �������ǉ����܂�.
		*  @param[inout] gu::tstring& : ���͕�����
		*  @param[in]    const gu::tstring : �ǉ�������������
		*  @return       void
		*************************************************************************/
		__forceinline static void WriteString(gu::tstring& string, const gu::tstring& addString)
		{
			string += addString;
		}

		/*!**********************************************************************
		*  @brief        ���s��ǉ����܂�.
		*  @param[inout] gu::tstring& : ���͕�����
		*  @return       void
		*************************************************************************/
		__forceinline static void WriteLineTerminator(gu::tstring& string)
		{
			string += SP("\r\n");
		}

		/*!**********************************************************************
		*  @brief        �X�y�[�X��ǉ����܂�.
		*  @param[inout] gu::tstring& : ���͕�����
		*  @return       void
		*************************************************************************/
		__forceinline static void WriteSpace(gu::tstring& string)
		{
			string += SP(" ");
		}

		/*!**********************************************************************
		*  @brief        �X�y�[�X��ǉ����܂�.
		*  @param[inout] gu::tstring& : ���͕�����
		*  @param[in]    gu::int32     : �ǉ��������X�y�[�X�̐�
		*  @return       void
		*************************************************************************/
		__forceinline static void WriteTab(gu::tstring& string, const gu::int32 count)
		{
			for (gu::int32 i = 0; i < count; ++i)
			{
				string += SP("\t");
			}
		}

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

		#pragma endregion 

		#pragma region Private Property

		#pragma endregion 

	};

}
#endif