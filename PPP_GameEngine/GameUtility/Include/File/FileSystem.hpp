//////////////////////////////////////////////////////////////////////////////////
///             @file   FileUtility.hpp
///             @brief  File Utility
///             @author Toide Yutaro
///             @date   2022_03_12
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef FILE_UTILITY_HPP
#define FILE_UTILITY_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include <string>
#include <iostream>

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace file
{
	/****************************************************************************
	*				  			FileUtility
	*************************************************************************//**
	*  @class     FileUtility
	*  @brief     File 
	*****************************************************************************/
	class FileSystem
	{
	public:
		static bool ExistsFile(const std::wstring& path);
		static bool ExistsFile(const std::string& path);

		static std::wstring GetExtension(const std::wstring& path);
		static std::string  GetExtension(const std::string& path);
		
		static std::wstring GetDirectory(const std::wstring& path);
		static std::string  GetDirectory(const std::string& path);

		static std::wstring GetFileName(const std::wstring& path, bool useExtension = true);
		static std::string  GetFileName(const std::string& path, bool useExtension = true);

	};
}
#endif