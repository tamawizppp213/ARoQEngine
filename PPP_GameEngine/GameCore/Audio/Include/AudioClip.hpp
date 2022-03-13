//////////////////////////////////////////////////////////////////////////////////
///             @file   AudioClip.hpp
///             @brief  Audio Clip for audio play (.wav data)
///             @author Toide Yutaro
///             @date   2021_03_12
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef AUDIO_CLIP_HPP
#define AUDIO_CLIP_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include <string>
#include <memory>
#include <Windows.h>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         AudioClip
//////////////////////////////////////////////////////////////////////////////////

/****************************************************************************
*				  		AudioClip
*************************************************************************//**
*  @class     Audio Clip
*  @brief     Save audio data (current .wav only)
*****************************************************************************/
class AudioClip
{
public:
	/****************************************************************************
	**                Public Function
	*****************************************************************************/
	bool LoadFromFile(const std::wstring& filePath);

	/****************************************************************************
	**                Public Member Variables
	*****************************************************************************/
	const std::wstring&            GetFilePath()     const;
	const size_t                   GetSoundSize()    const;
	const std::shared_ptr<BYTE[]>& GetSoundData()    const;
	const WAVEFORMATEX&            GetFileFormatEx() const;
	/****************************************************************************
	**                Constructor and Destructor
	*****************************************************************************/
	AudioClip();
	~AudioClip();
	AudioClip(const AudioClip&)            = default;
	AudioClip& operator=(const AudioClip&) = default;
	AudioClip(AudioClip&&)                 = default;
	AudioClip& operator=(AudioClip&&)      = default;
protected:
	/****************************************************************************
	**                Protected Function
	*****************************************************************************/

	/****************************************************************************
	**                Protected Member Variables
	*****************************************************************************/
	std::wstring            _filePath  = L"";
	size_t                  _soundSize = 0;
	WAVEFORMATEX            _waveFormatEx;
	std::shared_ptr<BYTE[]> _soundData = nullptr;

};

#endif