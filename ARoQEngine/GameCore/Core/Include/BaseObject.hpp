//////////////////////////////////////////////////////////////////////////////////
///  @file   BaseObject.hpp
///  @brief  �V���A���C�Y�Ȃǂ̊�{�I�ȋ@�\�����N���X
///  @author toide
///  @date   2024/06/29 0:13:19
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef BASE_OBJECT_HPP
#define BASE_OBJECT_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace engine
{

	/****************************************************************************
	*				  			   BaseObject
	*************************************************************************//**
	/*  @brief  �V���A���C�Y�Ȃǂ̊�{�I�ȋ@�\�����N���X
	*****************************************************************************/
	class BaseObject
	{
	public:
		#pragma region Public Function
		/*!***********************************************************************
		*  @brief      ����������Ă��邩�𒲂ׂ܂�
		*  @param[in]  void
		*  @return     const bool �F true�ŏ������ς݂�����.
		**************************************************************************/

		#pragma endregion 

		#pragma region Public Property

		#pragma endregion 

		#pragma region Public Operator 

		#pragma endregion 

		#pragma region Public Constructor and Destructor

		#pragma endregion 

	protected:

		#pragma region Protected Constructor and Destructor

		/*! @brief �f�t�H���g�R���X�g���N�^*/
		BaseObject() = default;

		/*! @brief �f�X�g���N�^*/
		virtual ~BaseObject() = default;

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