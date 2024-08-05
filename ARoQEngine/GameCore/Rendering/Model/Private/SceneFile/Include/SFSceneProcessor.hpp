//////////////////////////////////////////////////////////////////////////////////
///  @file   SFSceneImporter.hpp
///  @brief  temp
///  @author toide
///  @date   2024/07/23 0:06:38
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef SF_SCENE_PROCESSOR_HPP
#define SF_SCENE_PROCESSOR_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/GUString.hpp"
#include "GameUtility/Base/Include/GUSmartPointer.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace engine::file::sf
{
	struct SFScene;

	/****************************************************************************
	*				  			   SFSceneImporter
	****************************************************************************/
	/* @brief  temp
	*****************************************************************************/
	class ISceneProcessor
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief     各拡張子に基づいてシーンを読み込みます
		*  @param[in] const gu::tstring& ファイルパス
		*  @return    gu::SharedPointer<SFScene>
		*************************************************************************/
		virtual gu::SharedPointer<SFScene> Import(const gu::tstring& filePath) = 0;

		/*!**********************************************************************
		*  @brief     各拡張子に基づいてシーンを保存します
		*  @param[in] const gu::tstring& ファイルパス
		*  @return    bool
		*************************************************************************/
		virtual void Export(const gu::tstring& filePath) = 0;

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