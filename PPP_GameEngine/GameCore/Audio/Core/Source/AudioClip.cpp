//////////////////////////////////////////////////////////////////////////////////
///             @file   AudioClip.cpp
///             @brief  Audio clip.cpp
///             @author Toide Yutaro
///             @date   2022_03_12
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Audio/Core/Include/AudioClip.hpp"
#include "GameCore/Audio/Private/Include/WavDecoder.hpp"
#include "GameUtility/File/Include/FileSystem.hpp"
#include <memory>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gc::audio;

//////////////////////////////////////////////////////////////////////////////////
//                             Implement
//////////////////////////////////////////////////////////////////////////////////
AudioClip::AudioClip()
{
	ZeroMemory(&_waveFormatEx, sizeof(_waveFormatEx));
}

AudioClip::~AudioClip()
{

}

/****************************************************************************
*                          Load
*************************************************************************//**
*  @fn        bool AudioClip::Load(const std::wstring& filePath)
* 
*  @brief     Load wav file
* 
*  @param[in] const std::wstring& filePath
* 
*  @return Å@Å@bool
*****************************************************************************/
bool AudioClip::Load(const std::wstring& filePath)
{
	/*-------------------------------------------------------------------
	-    Select the appropriate sound loading function for each extension
	---------------------------------------------------------------------*/
	std::wstring extension = file::FileSystem::GetExtension(filePath);
	if (extension == L"wav")
	{
		WavDecoder wavDecoder;
		wavDecoder.LoadFromFile(filePath);
		_waveFormatEx = wavDecoder.GetFileFormatEx();
		_filePath     = wavDecoder.GetFilePath();
		_soundSize    = wavDecoder.GetWaveSize();
		_soundData    = wavDecoder.GetWaveData();

		return true;
	}
	else
	{
		OutputDebugStringA("The file format cannot be read.");
		return false;
	}
}
/****************************************************************************
*                       GetFilePath
*************************************************************************//**
*  @fn        const std::wstring& AudioClip::GetFilePath() const
* 
*  @brief     Return file path
* 
*  @param[in] void
* 
*  @return    std::wstring&
*****************************************************************************/
const std::wstring& AudioClip::GetFilePath() const
{
	return _filePath;
}

/****************************************************************************
*                       GetSoundSize
*************************************************************************//**
*  @fn        const size_t AudioClip::GetSoundSize() const
* 
*  @brief     Return sound size (only waveData Byte Size)
* 
*  @param[in] void
* 
*  @return    size_t
*****************************************************************************/
const size_t AudioClip::GetSoundSize() const
{
	return _soundSize;
}

/****************************************************************************
*                       GetSoundSize
*************************************************************************//**
*  @fn        const size_t AudioClip::GetSoundSize() const
* 
*  @brief     Return sound size (only waveData Byte Size)
* 
*  @param[in] void
* 
*  @return    size_t
*****************************************************************************/
const WAVEFORMATEX& AudioClip::GetFileFormatEx() const
{
	return _waveFormatEx;
}
/****************************************************************************
*                       GetSoundData
*************************************************************************//**
*  @fn        const size_t AudioClip::GetSoundData() const noexcept
*  @brief     Return file path
*  @param[in] void
*  @return    const std::shared_ptr<BYTE[]>&
*****************************************************************************/
std::shared_ptr<BYTE[]> AudioClip::GetSoundData() const noexcept
{
	return _soundData;
}
