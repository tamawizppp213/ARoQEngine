//////////////////////////////////////////////////////////////////////////////////
///  @file   XmlNode.hpp
///  @brief  temp
///  @author toide
///  @date   2024/08/06 23:56:55
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef XML_NODE_HPP
#define XML_NODE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "XmlAttribute.hpp"
#include "GameUtility/Container/Include/GUDynamicArray.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace gu::file::xml
{
	/****************************************************************************
	*				  			   XmlNode
	****************************************************************************/
	/* @brief  temp
	*****************************************************************************/
	class XmlNode
	{
	public:
		#pragma region Public Function
		/*!***********************************************************************
		*  @brief      �q�̃m�[�h���܂߂ă�������������܂�.
		*  @param[in]  void
		*  @return     void
		**************************************************************************/
		void PushChild(const gu::tstring& tag, const gu::tstring& content, const gu::DynamicArray<XmlAttribute>& attributes);

		/*!***********************************************************************
		*  @brief      �q�̃m�[�h���܂߂ă�������������܂�.
		*  @param[in]  void
		*  @return     void
		**************************************************************************/
		void Delete();
		#pragma endregion 

		#pragma region Public Property
		/*!***********************************************************************
		*  @brief      ���m�[�h���擾���܂�
		*  @param[in]  void
		*  @return     const XmlNode* �F �q�m�[�h���擾���܂�.
		**************************************************************************/
		__forceinline const XmlNode* Next() const noexcept { return _nextNode; }

		/*!***********************************************************************
		*  @brief      �w�肵���L�[�ɑΉ�����m�[�h���擾���܂�
		*  @param[in]  void
		*  @return     const XmlNode* �F �w�肵���L�[�ɑΉ�����m�[�h���擾���܂�.
		**************************************************************************/
		XmlNode* Child(const tstring& key);

		/*!***********************************************************************
		*  @brief      �w�肵���L�[�ɑΉ�����m�[�h���擾���܂�
		*  @param[in]  void
		*  @return     const XmlNode* �F �w�肵���L�[�ɑΉ�����m�[�h���擾���܂�.
		**************************************************************************/
		const XmlNode* Child(const tstring& key) const;

		/*!***********************************************************************
		*  @brief      �q�m�[�h���擾���܂�
		*  @param[in]  void
		*  @return     const DynamicArray<XmlNode*>& �F �q�m�[�h���擾���܂�.
		**************************************************************************/
		__forceinline const DynamicArray<XmlNode*>& Children() const noexcept { return _children; }

		/*!***********************************************************************
		*  @brief      Attribute���擾���܂�
		*  @param[in]  const gu::tstring& tag
		*  @return     gu::tstring attribute
		**************************************************************************/
		gu::tstring GetAttribute(const gu::tstring& tag) const;

		/*!***********************************************************************
		*  @brief      Attribute��ݒ肵�܂�
		*  @param[in]  const gu::DynamicArray<XmlAttribute> attribute
		*  @return     void
		**************************************************************************/
		__forceinline void SetAttribute(const gu::DynamicArray<XmlAttribute>& attributes)
		{
			_attributes.Reserve(attributes.Size());
			for (const auto& attribute : attributes)
			{
				_attributes.Push(attribute);
			}
		}

		/*!***********************************************************************
		*  @brief      Content��ݒ肵�܂�
		*  @param[in]  const gu::tstring& content
		*  @return     void
		**************************************************************************/
		__forceinline void SetContent(const gu::tstring& content)
		{
			_content = content;
		}

		/*!***********************************************************************
		*  @brief      Content��ݒ肵�܂�
		*  @param[in]  void
		*  @return     const gu::tstring& content
		**************************************************************************/
		__forceinline gu::tstring GetContent() const
		{
			return _content;
		}

		/*!***********************************************************************
		*  @brief      Tag��ݒ肵�܂�
		*  @param[in]  void
		*  @return     const gu::tstring& Tag
		**************************************************************************/
		__forceinline gu::tstring GetTag() const
		{
			return _tag;
		}

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

		/*! @brief �q�m�[�h�͕������\�������邽��, children �m�[�h�ɂ���*/
		DynamicArray<XmlNode*> _children = {};

		/*! @brief �{�m�[�h�ɂ�����L�[�o�����[�y�A�̃��X�g (���������������)*/
		DynamicArray<XmlAttribute> _attributes = {};

		/*! @brief �{�m�[�h�ɂ�����L�[*/
		gu::tstring _tag = SP("");

		/*! @brief �{�m�[�h�̒��g*/
		gu::tstring _content = SP("");

		/*! @brief ���̃m�[�h*/
		XmlNode* _nextNode = nullptr;
		#pragma endregion 

	};
}

#endif