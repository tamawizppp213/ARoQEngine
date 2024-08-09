//////////////////////////////////////////////////////////////////////////////////
///  @file   XmlFile.cpp
///  @brief  temp
///  @author toide
///  @date   2024/08/08 0:48:39
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/File/Include/Xml.hpp"
#include "Platform/Core/Include/CoreFileSystem.hpp"
#include "GameUtility/Base/Include/GUStringConverter.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gu;
using namespace gu::file;
using namespace platform::core;
//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor

#pragma endregion Constructor and Destructor

#pragma region Public Function
/*!**********************************************************************
*  @brief     �t�@�C����ǂݍ��݂܂�
*  @param[in] void
*  @return    bool
*************************************************************************/
bool Xml::Load([[maybe_unused]]const gu::tstring& filePath, [[maybe_unused]]const bool useAsync)
{
	/*-------------------------------------------------------------------
	-            �t�@�C���n���h�����擾
	---------------------------------------------------------------------*/
	const auto fileHandle = IFileSystem::Get()->OpenRead(filePath.CString(), true, useAsync);

	if (!fileHandle)
	{
		return false;
	}

	/*-------------------------------------------------------------------
	-            �t�@�C���̑S��������擾����
	---------------------------------------------------------------------*/
	const auto fileSize = fileHandle->Size();
	gu::tstring rawData  =SP("");
	{
		DynamicArray<char8> byteArray(fileSize, true);

		fileHandle->Read(byteArray.Data(), sizeof(char) * fileSize);

		// �����񃁃������ב΍�
		u8string utf8String(byteArray.Data(), fileSize);

		rawData = StringConverter::ConvertUTF8ToTString(utf8String);
	}

	return true;
}

/*!**********************************************************************
*  @brief     �t�@�C�����������݂܂�
*  @param[in] void
*  @return    bool
*************************************************************************/
bool Xml::Save([[maybe_unused]] const gu::tstring& filePath, [[maybe_unused]] const bool useAsync)
{
	return true;
}

/*!**********************************************************************
*  @brief     ����������Xml�t�@�C���̊e�m�[�h��������܂�.
*  @param[in] void
*  @return    void
*************************************************************************/
void Xml::Clear()
{
	if (_rootNode == nullptr)
	{
		return;
	}

	_rootNode->Delete();
	delete _rootNode;
	_rootNode = nullptr;
}
#pragma endregion Public Function