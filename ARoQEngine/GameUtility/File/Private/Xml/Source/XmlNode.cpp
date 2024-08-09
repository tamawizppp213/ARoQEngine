//////////////////////////////////////////////////////////////////////////////////
///  @file   XmlNode.cpp
///  @brief  temp
///  @author toide
///  @date   2024/08/06 23:59:27
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/File/Private/Xml/Include/XmlNode.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gu;
using namespace gu::file::xml;

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor

#pragma endregion Constructor and Destructor

#pragma region Public Function
/*!***********************************************************************
*  @brief      子のノードも含めてメモリを解放します.
*  @param[in]  void
*  @return     void
**************************************************************************/
void XmlNode::PushChild(const gu::tstring& tag, const gu::tstring& content, const gu::DynamicArray<XmlAttribute>& attributes)
{
	auto newNode = new XmlNode();
	newNode->_tag        = tag;
	newNode->_content    = content;
	newNode->_attributes.Reserve(attributes.Size());
	for (const auto& attribute : attributes)
	{
		newNode->_attributes.Push(attribute);
	}

	if (_children.Size() != 0)
	{
		_children[_children.Size() - 1]->_nextNode = newNode;
	}
	_children.Push(newNode);
}

/*!***********************************************************************
*  @brief      子のノードも含めてメモリを解放します.
*  @param[in]  void
*  @return     void
**************************************************************************/
void XmlNode::Delete()
{
	for (uint64 i = 0; i < _children.Size(); i++)
	{
		if (_children[i] != nullptr)
		{
			delete _children[i];
		}
	}

	_children.Clear();
}
/*!***********************************************************************
*  @brief      指定したキーに対応するノードを取得します
*  @param[in]  void
*  @return     const XmlNode* ： 指定したキーに対応するノードを取得します.
**************************************************************************/
XmlNode* XmlNode::Child(const tstring& key)
{
	for (auto& node : _children)
	{
		if (node->_tag == key)
		{
			return node;
		}
	}
	return nullptr;
}

/*!***********************************************************************
*  @brief      指定したキーに対応するノードを取得します
*  @param[in]  void
*  @return     const XmlNode* ： 指定したキーに対応するノードを取得します.
**************************************************************************/
const XmlNode* XmlNode::Child(const tstring& key) const
{
	for (const auto& node : _children)
	{
		if (node->_tag == key)
		{
			return node;
		}
	}
	return nullptr;
}

/*!***********************************************************************
*  @brief      Attributeを取得します
*  @param[in]  const gu::tstring& tag
*  @return     gu::tstring attribute
**************************************************************************/
gu::tstring XmlNode::GetAttribute(const gu::tstring& tag) const
{
	for (const auto& attribute : _attributes)
	{
		if (attribute.Tag() == tag)
		{
			return attribute.Value();
		}
	}
	return SP("");
}
#pragma endregion Public Function