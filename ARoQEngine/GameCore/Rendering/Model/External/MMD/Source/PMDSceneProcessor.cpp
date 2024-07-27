//////////////////////////////////////////////////////////////////////////////////
///  @file   PMDSceneProcessor.cpp
///  @brief  temp
///  @author toide
///  @date   2024/07/23 0:43:27
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Rendering/Model/External/MMD/Include/PMDSceneProcessor.hpp"
#include "GameCore/Rendering/Model/External/MMD/Include/PMDParser.hpp"
#include "GameUtility/Base/Include/GUSmartPointer.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace engine::file::pmd;
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
*  @return    gu::SharedPointer<SFScene>
*************************************************************************/
gu::SharedPointer<sf::SFScene> PMDSceneProcessor::Import(const gu::tstring& filePath)
{
	const auto file = gu::MakeShared<PMDFile>();
	
	if (!file->Read(filePath))
	{
		return nullptr;
	}

	return nullptr;
}

/*!**********************************************************************
*  @brief     �e�g���q�Ɋ�Â��ăV�[����ۑ����܂�
*  @param[in] const gu::tstring& �t�@�C���p�X
*  @return    bool
*************************************************************************/
void PMDSceneProcessor::Export([[maybe_unused]]const gu::tstring& filePath)
{
	
}
#pragma endregion Public Function