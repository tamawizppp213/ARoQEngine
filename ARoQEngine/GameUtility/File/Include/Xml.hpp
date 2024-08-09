//////////////////////////////////////////////////////////////////////////////////
///  @file   Xml.hpp
///  @brief  XML�t�@�C���̓ǂݍ��݂⏑�����݂��s���N���X�ł�. 
///  @author toide
///  @date   2024/08/06 23:36:53
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef XML_HPP
#define XML_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/File/Private/Xml/Include/XmlNode.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace gu::file
{
	/****************************************************************************
	*				  			   Xml
	****************************************************************************/
	/* @brief  temp
	*****************************************************************************/
	class Xml
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief     �t�@�C����ǂݍ��݂܂�
		*  @param[in] void
		*  @return    bool
		*************************************************************************/
		bool Load(const gu::tstring& filePath, const bool useAsync = false);

		/*!**********************************************************************
		*  @brief     �t�@�C�����������݂܂�
		*  @param[in] void
		*  @return    bool
		*************************************************************************/
		bool Save(const gu::tstring& filePath, const bool useAsync = false);

		/*!**********************************************************************
		*  @brief     ����������Xml�t�@�C���̊e�m�[�h��������܂�.
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		void Clear();
		#pragma endregion 

		#pragma region Public Property
		/*!**********************************************************************
		*  @brief     RootNode���擾���܂�. ������ċA�I�ɂ��ǂ邱�Ƃ�, XML�t�@�C���̏����擾�ł��܂�.
		*  @param[in] void
		*  @return    XmlNode* : RootNode
		*************************************************************************/
		__forceinline xml::XmlNode* GetRootNode() noexcept { return _rootNode; }

		/*!**********************************************************************
		*  @brief     RootNode��ݒ肵�܂�. ������ċA�I�ɂ��ǂ邱�Ƃ�, XML�t�@�C���̏����擾�ł��܂�.
		*  @param[in] void
		*  @return    const XmlNode* : RootNode
		*************************************************************************/
		__forceinline const xml::XmlNode* GetRootNode() const noexcept{ return _rootNode; }

		#pragma endregion 

		#pragma region Public Operator 

		#pragma endregion 

		#pragma region Public Constructor and Destructor
		/*! @brief �p�X�������ƂȂ���̃N���X���쐬����*/
		Xml() = default;

		/*! @brief �f�X�g���N�^*/
		~Xml()
		{
			Clear();
		}

		/*! @brief �R�s�[�R���X�g���N�^*/
		Xml(const Xml&) = delete;

		/*! @brief �R�s�[���Z�q*/
		Xml& operator=(const Xml&) = delete;

		/*! @brief ���[�u�R���X�g���N�^*/
		Xml(Xml&&) = default;

		/*! @brief ���[�u���Z�q*/
		Xml& operator=(Xml&&) = default;
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
		/*!**********************************************************************
		*  @brief     �s�K�v�ȕ�����(�R�����g, ��)�̐��`���s���܂�. 
		*  @param[out]gu::DynamicArray<gu::tstring>& input 
		*  @return    void
		*************************************************************************/

		#pragma endregion 

		#pragma region Private Property
		/*! @brief XML�̐e�m�[�h*/
		xml::XmlNode* _rootNode = nullptr;

		#pragma endregion 

	};
}

#endif