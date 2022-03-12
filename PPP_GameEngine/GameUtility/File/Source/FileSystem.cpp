//////////////////////////////////////////////////////////////////////////////////
//              @file   FileSystem.cpp
///             @brief  File system
///             @author Toide Yutaro
///             @date   2022_03_12
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/File/Include/FileSystem.hpp"
#include <Windows.h>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace file;
static constexpr int INVALID_ID = -1;
//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
/****************************************************************************
*							ExistsFile
*************************************************************************//**
*  @fn        bool FileSystem::ExistsFile(const std::wstring& filePath)
*  @brief     Exists file
*  @param[in] const std::wstring& filePath
*  @return 　　bool
*****************************************************************************/
bool FileSystem::ExistsFile(const std::wstring& filePath)
{
	DWORD attribute = ::GetFileAttributesW(filePath.c_str());
	return ((attribute != INVALID_ID) && (attribute & FILE_ATTRIBUTE_DIRECTORY) == 0);
}
bool FileSystem::ExistsFile(const std::string& filePath)
{
	DWORD attribute = ::GetFileAttributesA(filePath.c_str());
	return ((attribute != INVALID_ID) && (attribute & FILE_ATTRIBUTE_DIRECTORY) == 0);
}
/****************************************************************************
*							GetExtension
*************************************************************************//**
*  @fn        std::wstring FileSystem::GetExtension(const std::wstring& path)
*  @brief     Get File Extension
*  @param[in] const std::wstring& filePath
*  @return 　　std::wstring
*****************************************************************************/
std::wstring FileSystem::GetExtension(const std::wstring& path)
{
	UINT index = static_cast<UINT>(path.find_last_of(L"."));
	return path.substr((UINT64)index + 1, path.length() - index);
}
std::string FileSystem::GetExtension(const std::string& path)
{
	UINT index = static_cast<UINT>(path.find_last_of("."));
	return path.substr((UINT64)index + 1, path.length() - index);
}
/****************************************************************************
*							GetDirectory
*************************************************************************//**
*  @fn        std::wstring FileSystem::GetDirectory(const std::wstring& path)
*  @brief     Get File Directory
*  @param[in] const std::wstring& filePath
*  @return 　　std::wstring
*****************************************************************************/
std::wstring FileSystem::GetDirectory(const std::wstring& path)
{
	UINT index = static_cast<UINT>(path.find_last_of(L'/'));
	return path.substr(0, (UINT64)index + 1);
}
std::string FileSystem::GetDirectory(const std::string& path)
{
	UINT index = static_cast<UINT>(path.find_last_of(L'/'));
	return path.substr(0, (UINT64)index + 1);
}
/****************************************************************************
*							GetFileName
*************************************************************************//**
*  @fn        std::wstring FileSystem::GetFileName(const std::wstring& path)
*  @brief     Get File Name
*  @param[in] const std::wstring& filePath
*  @return 　　std::wstring
*****************************************************************************/
std::wstring FileSystem::GetFileName(const std::wstring& path, bool useExtension)
{
	UINT index = static_cast<UINT>(path.rfind('/'));

	std::wstring fileName = path.substr((UINT64)index + 1, path.size() - index - 1);
	if (!useExtension)
	{
		fileName = fileName.substr(0, fileName.find_last_of('.'));
	}
	return fileName;
}
std::string FileSystem::GetFileName(const std::string& path, bool useExtension)
{
	UINT index = static_cast<UINT>(path.rfind('/'));
	std::string fileName = path.substr((UINT64)index + 1, path.size() - index - 1);
	if (!useExtension)
	{
		fileName = fileName.substr(0, fileName.find_last_of('.'));
	}
	return fileName;
}
