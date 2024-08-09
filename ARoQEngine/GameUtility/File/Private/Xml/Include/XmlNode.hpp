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
		*  @brief      子のノードも含めてメモリを解放します.
		*  @param[in]  void
		*  @return     void
		**************************************************************************/
		void PushChild(const gu::tstring& tag, const gu::tstring& content, const gu::DynamicArray<XmlAttribute>& attributes);

		/*!***********************************************************************
		*  @brief      子のノードも含めてメモリを解放します.
		*  @param[in]  void
		*  @return     void
		**************************************************************************/
		void Delete();
		#pragma endregion 

		#pragma region Public Property
		/*!***********************************************************************
		*  @brief      次ノードを取得します
		*  @param[in]  void
		*  @return     const XmlNode* ： 子ノードを取得します.
		**************************************************************************/
		__forceinline const XmlNode* Next() const noexcept { return _nextNode; }

		/*!***********************************************************************
		*  @brief      指定したキーに対応するノードを取得します
		*  @param[in]  void
		*  @return     const XmlNode* ： 指定したキーに対応するノードを取得します.
		**************************************************************************/
		XmlNode* Child(const tstring& key);

		/*!***********************************************************************
		*  @brief      指定したキーに対応するノードを取得します
		*  @param[in]  void
		*  @return     const XmlNode* ： 指定したキーに対応するノードを取得します.
		**************************************************************************/
		const XmlNode* Child(const tstring& key) const;

		/*!***********************************************************************
		*  @brief      子ノードを取得します
		*  @param[in]  void
		*  @return     const DynamicArray<XmlNode*>& ： 子ノードを取得します.
		**************************************************************************/
		__forceinline const DynamicArray<XmlNode*>& Children() const noexcept { return _children; }

		/*!***********************************************************************
		*  @brief      Attributeを取得します
		*  @param[in]  const gu::tstring& tag
		*  @return     gu::tstring attribute
		**************************************************************************/
		gu::tstring GetAttribute(const gu::tstring& tag) const;

		/*!***********************************************************************
		*  @brief      Attributeを設定します
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
		*  @brief      Contentを設定します
		*  @param[in]  const gu::tstring& content
		*  @return     void
		**************************************************************************/
		__forceinline void SetContent(const gu::tstring& content)
		{
			_content = content;
		}

		/*!***********************************************************************
		*  @brief      Contentを設定します
		*  @param[in]  void
		*  @return     const gu::tstring& content
		**************************************************************************/
		__forceinline gu::tstring GetContent() const
		{
			return _content;
		}

		/*!***********************************************************************
		*  @brief      Tagを設定します
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

		/*! @brief 子ノードは複数持つ可能性があるため, children ノードにした*/
		DynamicArray<XmlNode*> _children = {};

		/*! @brief 本ノードにおけるキーバリューペアのリスト (こちらも複数存在)*/
		DynamicArray<XmlAttribute> _attributes = {};

		/*! @brief 本ノードにおけるキー*/
		gu::tstring _tag = SP("");

		/*! @brief 本ノードの中身*/
		gu::tstring _content = SP("");

		/*! @brief 次のノード*/
		XmlNode* _nextNode = nullptr;
		#pragma endregion 

	};
}

#endif