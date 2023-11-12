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
#include "../../Effect/Include/AudioFader.hpp"
#include "GameUtility/Base/Include/ClassUtility.hpp"
#include <string>
#include <memory>
#include <wrl.h>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
struct IXAudio2SourceVoice;
//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace gc::audio
{
	class AudioMaster;
	class AudioClip;

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
	*  @brief     Play, Stop, Pause sound file.
	*****************************************************************************/
	class AudioSource : public NonCopyable
	{
	protected:
		
		using AudioClipPtr = std::shared_ptr<AudioClip>; // é¿ëÃÇÕunordered mapÇ≈ä«óùÇµÇƒÇ¢ÇÈÇΩÇﬂ, é©ìÆÇ≈îjä¸Ç∑ÇÈïKóvÇ»Çµ.
		using SourceVoicePtr = IXAudio2SourceVoice*;
		using AudioMasterPtr = std::shared_ptr<AudioMaster>;
	
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/* @brief : Load wav file. SoundType : BGM, BGS -> Loop On, ME , SE -> Loop Off */
		virtual bool SetUp(const AudioClipPtr& audioClip, const SoundType soundType, const float volume = 1.0f);

		/* @brief : This function is used to perform a fade.*/
		virtual void Update([[maybe_unused]]const float deltaTime);

		/* @brief : Play sound. (Playback from the beginning at any time)
		            ("loopIntervalSeconds equals 0.0f" means "Playback to end at 0.0f")*/
		virtual bool Play(const float loopBeginSeconds = 0.0f, const float loopIntervalSeconds = 0.0f);

		/* @brief : Stop sound*/
		bool Stop();

		/* @brief : Restart sound. Play from the middle part*/
		bool Replay();

		/* @brief: Pause sound*/
		bool Pause();

		bool ExitLoop();

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		/* @brief : (true-> loop, false -> not loop)*/
		bool IsLoop() const { return _isLoop; };

		/* @brief : Is playing sound (true-> playing, false -> not playing)*/
		bool IsPlaying();

		/*-------------------------------------------------------------------
		-              Pitch
		---------------------------------------------------------------------*/
		float GetPitch();
		void  SetPitch(float pitch);        // newPitch = pitch (0 <= pitch <= maxPitch)
		void  SetMaxPitch(float maxPitch);  
		void  AdjustPitch(float diffPitch); // newPitch = current pitch + diffpitch

		/*-------------------------------------------------------------------
		-              Volume
		---------------------------------------------------------------------*/
		float GetVolume() const;
		bool  SetVolume(float volume);
		void  AdjustVolume(float diffVolume);

		/* @brief : Set max volume limit (0.0f < maxVolume < XAUDIO2_MAX_VOLUME_LEVEL, 16777216.0f)*/
		bool SetMaxVolumeLimit(const float maxVolume);

		bool SetPan(float pan);

		bool IsUseReverb(bool useReverb);

		/*-------------------------------------------------------------------
		-              XAudio2 Property
		---------------------------------------------------------------------*/
		SourceVoicePtr GetSourceVoice() { return _sourceVoice; }

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		AudioSource(const std::shared_ptr<AudioMaster>& audioMaster);

		AudioSource(const std::shared_ptr<AudioMaster>& audioMaster, const AudioClipPtr& audioClip, const SoundType soundType, const float volume = 1.0f);

		~AudioSource();
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		void FlushAudioData(const float loopBeginSeconds = 0.0f, const float loopIntervalSeconds = 0.0f);
		bool CreateSourceVoice();
		bool CreateReverb();
		bool SelectIsLoop(const SoundType soundType);
		bool SafeClearSourceBuffer();

		/* @brief : is Existed source voice (true -> exist, false -> not exist)*/
		bool IsExistedSourceVoice() const { return  _sourceVoice != nullptr; };

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		/* @brief : Fader. Play and Stop function are called once, start fade in and fade out.*/
		AudioFader _fader;

		/* @brief : wav sound data config*/
		AudioClipPtr   _audioClip  = nullptr;

		/* @brief : sound source voice.*/
		SourceVoicePtr _sourceVoice = nullptr;

		/* @brief : audio mastering.*/
		AudioMasterPtr _audioMaster = nullptr;

		SoundType _soundType = SoundType::NONE;

		/*-------------------------------------------------------------------
		-              Max limit
		---------------------------------------------------------------------*/
		float _maxPitch  = 0.0f;
		float _maxVolume = 2.0f;

		/*-------------------------------------------------------------------
		-              State flags
		---------------------------------------------------------------------*/
		/* @brief : Has loaded sound file. (true : load, false : not yet)*/
		bool  _hasLoaded = false;

		/* @brief : Whether to use loop playback*/
		bool  _isLoop    = false;

		static constexpr float DEFAULT_VOLUME = 1.0f;
	};
}
#endif