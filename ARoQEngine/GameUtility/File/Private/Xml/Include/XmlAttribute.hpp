//////////////////////////////////////////////////////////////////////////////////
///  @file   XmlAttribute.hpp
///  @brief  temp
///  @author toide
///  @date   2024/08/09 0:48:57
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef XML_ATTRIBUTE_HPP
#define XML_ATTRIBUTE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Container/Include/GUDynamicArray.hpp"
#include "GameUtility/Base/Include/GUString.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace gu::file::xml
{

	/****************************************************************************
	*				  			   XmlAttribute
	****************************************************************************/
	/* @brief  temp
	*****************************************************************************/
	class XmlAttribute
	{
	public:
		#pragma region Public Function

		#pragma endregion 

		#pragma region Public Property
		/*!***********************************************************************
		*  @brief      �L�[���擾���܂�
		*  @param[in]  void
		*  @return     const gu::tstring& �F �L�[���擾���܂�.
		**************************************************************************/
		__forceinline const gu::tstring& Tag() const noexcept { return _tag; }

		/*!***********************************************************************
		*  @brief      �o�����[���擾���܂�
		*  @param[in]  void
		*  @return     const gu::tstring& �F �o�����[���擾���܂�.
		**************************************************************************/
		__forceinline const gu::tstring& Value() const noexcept { return _value; }
		#pragma endregion 

		#pragma region Public Operator 

		#pragma endregion 

		#pragma region Public Constructor and Destructor
		/*! @brief �f�t�H���g�R���X�g���N�^*/
		XmlAttribute() = default;

		/*! @brief �^�O�ƒl�ŏ�����*/
		XmlAttribute(const gu::tstring& key, const gu::tstring& value) : _tag(key), _value(value) {}
		#pragma endregion 

	protected:
		#pragma region Protected Constructor and Destructor

		#pragma endregion 

		#pragma region Protected Function

		#pragma endregion 

		#pragma region Protected Property
		/*! @brief �^�O*/
		gu::tstring _tag = SP("");

		/*! @brief �o�����[*/
		gu::tstring _value = SP("");
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