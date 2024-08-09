//////////////////////////////////////////////////////////////////////////////////
///  @file   Xml.hpp
///  @brief  XMLファイルの読み込みや書き込みを行うクラスです. 
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
		*  @brief     ファイルを読み込みます
		*  @param[in] void
		*  @return    bool
		*************************************************************************/
		bool Load(const gu::tstring& filePath, const bool useAsync = false);

		/*!**********************************************************************
		*  @brief     ファイルを書き込みます
		*  @param[in] void
		*  @return    bool
		*************************************************************************/
		bool Save(const gu::tstring& filePath, const bool useAsync = false);

		/*!**********************************************************************
		*  @brief     メモリからXmlファイルの各ノードを解放します.
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		void Clear();
		#pragma endregion 

		#pragma region Public Property
		/*!**********************************************************************
		*  @brief     RootNodeを取得します. これを再帰的にたどることで, XMLファイルの情報を取得できます.
		*  @param[in] void
		*  @return    XmlNode* : RootNode
		*************************************************************************/
		__forceinline xml::XmlNode* GetRootNode() noexcept { return _rootNode; }

		/*!**********************************************************************
		*  @brief     RootNodeを設定します. これを再帰的にたどることで, XMLファイルの情報を取得できます.
		*  @param[in] void
		*  @return    const XmlNode* : RootNode
		*************************************************************************/
		__forceinline const xml::XmlNode* GetRootNode() const noexcept{ return _rootNode; }

		#pragma endregion 

		#pragma region Public Operator 

		#pragma endregion 

		#pragma region Public Constructor and Destructor
		/*! @brief パスを持つことなく空のクラスを作成する*/
		Xml() = default;

		/*! @brief デストラクタ*/
		~Xml()
		{
			Clear();
		}

		/*! @brief コピーコンストラクタ*/
		Xml(const Xml&) = delete;

		/*! @brief コピー演算子*/
		Xml& operator=(const Xml&) = delete;

		/*! @brief ムーブコンストラクタ*/
		Xml(Xml&&) = default;

		/*! @brief ムーブ演算子*/
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
		*  @brief     不必要な文字列(コメント, 空白)の成形を行います. 
		*  @param[out]gu::DynamicArray<gu::tstring>& input 
		*  @return    void
		*************************************************************************/

		#pragma endregion 

		#pragma region Private Property
		/*! @brief XMLの親ノード*/
		xml::XmlNode* _rootNode = nullptr;

		#pragma endregion 

	};
}

#endif