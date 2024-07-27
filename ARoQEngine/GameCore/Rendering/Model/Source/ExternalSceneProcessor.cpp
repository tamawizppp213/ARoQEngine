//////////////////////////////////////////////////////////////////////////////////
///  @file   ExternalSceneProcessor.cpp
///  @brief  temp
///  @author toide
///  @date   2024/07/23 0:30:38
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Rendering/Model/Include/ExternalSceneProcessor.hpp"
#include "GameCore/Rendering/Model/External/MMD/Include/PMDSceneProcessor.hpp"
#include "GameCore/Rendering/Model/External/MMD/Include/PMXSceneProcessor.hpp"
#include "Platform/Core/Include/CoreFileSystem.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace engine;
using namespace engine::file;
//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor

#pragma endregion Constructor and Destructor

#pragma region Public Function
/*!**********************************************************************
*  @brief     各拡張子に基づいてシーンを読み込みます
*  @param[in] const gu::tstring& ファイルパス
*  @return    gu::SharedPointer<file::sf::ISceneProcessor>
*************************************************************************/
gu::SharedPointer<file::sf::ISceneProcessor> ExternalSceneProcessor::Import(const gu::tstring& filePath)
{
	const auto extension = platform::core::IFileSystem::Get()->GetExtension(filePath);

	gu::SharedPointer<sf::ISceneProcessor> processor = nullptr;

	if (extension == SP(".pmx"))
	{
		processor = gu::MakeShared<pmx::PMXSceneProcessor>();
	}
	else
	{
		return nullptr;
	}

	processor->Import(filePath);

	return processor;
}

/*!**********************************************************************
*  @brief     各拡張子に基づいてシーンを保存します
*  @param[in] const gu::tstring& ファイルパス
*  @return    bool
*************************************************************************/
void ExternalSceneProcessor::Export([[maybe_unused]]const gu::tstring& filePath)
{

}
#pragma endregion Public Function