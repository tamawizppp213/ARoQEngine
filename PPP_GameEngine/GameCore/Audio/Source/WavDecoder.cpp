//////////////////////////////////////////////////////////////////////////////////
///             @file   WaveDecoder.cpp
///             @brief  Wave File Write and Read
///             @author Toide Yutaro
///             @date   2022_03_12
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Audio/Include/WavDecoder.hpp"
#include "GameUtility/File/Include/FileSystem.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
#pragma comment(lib, "winmm.lib")

//////////////////////////////////////////////////////////////////////////////////
//                              Implement
//////////////////////////////////////////////////////////////////////////////////
WavDecoder::WavDecoder()
{
	ZeroMemory(&_waveFormatEx, sizeof(_waveFormatEx));
}
#pragma region Public Function
/****************************************************************************
*                       LoadFromFile
*************************************************************************//**
*  @fn        bool WavDecoder::LoadFromFile(const std::wstring& filePath)
*  @brief     Open Wav File
*  @param[in] const std::wstring& filePath
*  @return 　　bool
*****************************************************************************/
#pragma endregion Public Function
#pragma region Private Function
/****************************************************************************
*                       Open
*************************************************************************//**
*  @fn        bool WavDecoder::Open(const std::wstring& filePath)
*  @brief     Open Wav File
*  @param[in] const std::wstring& filePath
*  @return 　　bool
*****************************************************************************/
bool WavDecoder::Open(const std::wstring& filePath)
{
	MMIOINFO mmioInfo;
	memset(&mmioInfo, 0, sizeof(MMIOINFO));

	/*-------------------------------------------------------------------
	-              Is Wav File Path
	---------------------------------------------------------------------*/
	std::wstring extension = file::FileSystem::GetExtension(filePath);
	if (extension != L"wav")
	{
		MessageBox(NULL, L"can't read wavFile", L"Warning", MB_ICONWARNING);
		return false;
	}

	/*-------------------------------------------------------------------
	-              File Open
	---------------------------------------------------------------------*/
	LPWSTR path = (LPWSTR)filePath.c_str();
	if (!(_handle = mmioOpen(path, &mmioInfo, MMIO_READ)))
	{
		MessageBox(NULL, L"can't create MMIO.", L"Warning", MB_ICONWARNING);
		return false;
	}

	_filePath = filePath;
	return true;
}
#pragma endregion Private Function