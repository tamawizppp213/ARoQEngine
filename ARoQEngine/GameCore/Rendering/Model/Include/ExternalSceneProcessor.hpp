//////////////////////////////////////////////////////////////////////////////////
///  @file   SceneImporter.hpp
///  @brief  外部3Dモデル, 3Dシーンのデータファイルを読み込みます.　
///  @author toide
///  @date   2024/07/21 22:32:34
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef EXTERNAL_SCENE_PROCESSOR_HPP
#define EXTERNAL_SCENE_PROCESSOR_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/GUClassUtility.hpp"
#include "GameUtility/Base/Include/GUSmartPointer.hpp"
#include "GameUtility/Base/Include/GUString.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
namespace engine::file::sf
{
	class ISceneProcessor;
	struct SFScene;
}

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace engine
{
	/****************************************************************************
	*				  			 ExternalSceneProcessor
	****************************************************************************/
	/* @brief  外部ファイルのシーン読み込みを行うクラス
	*****************************************************************************/
	class ExternalSceneProcessor : public gu::NonCopyable
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief     各拡張子に基づいてシーンを読み込みます
		*  @param[in] const gu::tstring& ファイルパス
		*  @return    gu::SharedPointer<file::sf::ISceneProcessor>
		*************************************************************************/
		static gu::SharedPointer<file::sf::ISceneProcessor> Import(const gu::tstring& filePath);

		/*!**********************************************************************
		*  @brief     各拡張子に基づいてシーンを保存します
		*  @param[in] const gu::tstring& ファイルパス
		*  @return    bool
		*************************************************************************/
		static void Export(const gu::tstring& filePath);

		#pragma endregion 

		#pragma region Public Property

		#pragma endregion 

		#pragma region Public Operator 

		#pragma endregion 

		#pragma region Public Constructor and Destructor
		/*! @brief デフォルトコンストラクタ*/
		ExternalSceneProcessor() = default;

		/*! @brief デストラクタ*/
		~ExternalSceneProcessor() = default;
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