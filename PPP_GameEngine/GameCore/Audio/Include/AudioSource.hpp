//////////////////////////////////////////////////////////////////////////////////
///             @file   AudioSource.hpp
///             @brief  Default Audio class (for 2D)
///             @author Toide Yutaro
///             @date   2020_01/12
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef AUDIO_SOURCE_HPP
#define AUDIO_SOURCE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include <string>
#include <memory>
#include <wrl.h>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
class AudioClip;
struct IXAudio2SourceVoice;
//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
/*************************************************************************//**
*  @enum      SoundType
*****************************************************************************/
enum class SoundType
{
	BGM,
	BGS,
	ME,
	SE,
	NONE
};
/****************************************************************************
*				  			AudioSource
*************************************************************************//**
*  @class     AudioSource
*  @brief     Audio Source
*****************************************************************************/
class AudioSource
{
protected:
	using AudioClipPtr   = AudioClip*; // é¿ëÃÇÕunordered mapÇ≈ä«óùÇµÇƒÇ¢ÇÈÇΩÇﬂ, é©ìÆÇ≈îjä¸Ç∑ÇÈïKóvÇ»Çµ.
	using SourceVoucePtr =IXAudio2SourceVoice*;
public:
	/****************************************************************************
	**                Public Function
	*****************************************************************************/
	virtual bool Play();
	bool Stop();
	bool Replay();
	bool Pause();
	bool ExitLoop();
	bool LoadSound(const std::wstring& filePath, SoundType soundType, float volume = 1.0f);

	/****************************************************************************
	**                Public Member Variables
	*****************************************************************************/
	bool IsLoop() const;
	bool IsPlaying();

	/*-------------------------------------------------------------------
	-              Pitch
	---------------------------------------------------------------------*/
	float GetPitch();
	void SetPitch  (float pitch);
	void SetMaxPitch(float maxPitch);
	void AdjustPitch(float diffPitch);
	/*-------------------------------------------------------------------
	-              Volume
	---------------------------------------------------------------------*/
	float GetVolume();
	bool  SetVolume(float volume);
	void  AdjustVolume(float diffVolume);

	bool SetPan(float pan);
	bool IsUseReverb(bool useReverb);

	/****************************************************************************
	**                Constructor and Destructor
	*****************************************************************************/
	AudioSource();
	~AudioSource();
	AudioSource(const AudioSource&) = default;
	AudioSource& operator=(const AudioSource&) = default;
	AudioSource(AudioSource&&) = default;
	AudioSource& operator=(AudioSource&&) = default;
protected:
	/****************************************************************************
	**                Protected Function
	*****************************************************************************/
	void FlushAudioData();
	bool IsExistedSourceVoice();
	bool IsRemainedSourceBufferQueue();
	bool LoadAudioClip(const std::wstring& filePath);
	bool CreateSourceVoice();
	bool CreateReverb();
	bool SelectSoundType(SoundType soundType);

	/****************************************************************************
	**                Protected Member Variables
	*****************************************************************************/
	AudioClipPtr   _audioClip = nullptr;
	SourceVoucePtr _sourceVoice = nullptr;
	SoundType _soundType = SoundType::NONE;
	bool  _isLoop;
	float _maxVolume;
	float _maxPitch;
	static float _volumeRatio[4];
};

#endif