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
*  @brief     �e�g���q�Ɋ�Â��ăV�[����ǂݍ��݂܂�
*  @param[in] const gu::tstring& �t�@�C���p�X
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
*  @brief     �e�g���q�Ɋ�Â��ăV�[����ۑ����܂�
*  @param[in] const gu::tstring& �t�@�C���p�X
*  @return    bool
*************************************************************************/
void ExternalSceneProcessor::Export([[maybe_unused]]const gu::tstring& filePath)
{

}
#pragma endregion Public Function