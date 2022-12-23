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
		using SourceVoucePtr = std::shared_ptr<IXAudio2SourceVoice>;
		using AudioMasterPtr = std::shared_ptr<AudioMaster>;
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/* @brief : Load wav file. SoundType : BGM, BGS -> Loop On, ME , SE -> Loop Off */
		virtual bool SetUp(const AudioClipPtr& audioClip, const SoundType soundType, const float volume = 1.0f);

		/* @brief : Play sound. */
		virtual bool Play();

		bool Stop();
		bool Replay();
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
		void SetPitch(float pitch);
		void SetMaxPitch(float maxPitch);
		void AdjustPitch(float diffPitch);

		/*-------------------------------------------------------------------
		-              Volume
		---------------------------------------------------------------------*/
		float GetVolume() const;
		bool  SetVolume(float volume);
		void  AdjustVolume(float diffVolume);

		bool SetPan(float pan);
		bool IsUseReverb(bool useReverb);

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
		void FlushAudioData();
		bool CreateSourceVoice();
		bool CreateReverb();
		bool SelectIsLoop(const SoundType soundType);

		/* @brief : is Existed source voice (true -> exist, false -> not exist)*/
		bool IsExistedSourceVoice() const { return  _sourceVoice != nullptr; };

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		AudioClipPtr   _audioClip = nullptr;
		SourceVoucePtr _sourceVoice = nullptr;
		AudioMasterPtr _audioMaster = nullptr;
		SoundType _soundType = SoundType::NONE;
		bool  _isLoop    = false;
		float _maxVolume = 0.0f;
		float _maxPitch  = 0.0f;

		/* @brief : Has loaded sound file. (true : load, false : not yet)*/
		bool  _hasLoaded = false;
		static float _volumeRatio[4];
	};
}
#endif