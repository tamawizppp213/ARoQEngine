//////////////////////////////////////////////////////////////////////////////////
///             @file   AudioMaster.hpp
///             @brief  AudioMaster.hpp
///             @author Toide Yutaro
///             @date   2021_01_03
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef AUDIO_SUBMIX_HPP
#define AUDIO_SUBMIX_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include <memory>
#include <vector>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
struct IXAudio2SubmixVoice;
struct IXAudio2SourceVoice;

namespace gc::audio
{
	class AudioMaster;
	class AudioSource;

	/****************************************************************************
	*				  			AudioSubmix
	****************************************************************************/
	/* @class     AudioSubmix
	*  @brief     Manage AudioSource in one place
	*****************************************************************************/
	class AudioSubmix
	{
		using SubmixVoicePtr = IXAudio2SubmixVoice*;
		using SourceVoicePtr = IXAudio2SourceVoice*;
		using AudioMasterPtr = std::shared_ptr<AudioMaster>;
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/* @brief : Regist source voice*/
		void Regist(const std::shared_ptr<AudioSource>& sourceVoice);
		
		/* @brief : Use Reverve*/
		void Reverb(bool isOn);

		/****************************************************************************
		**                Public Property
		*****************************************************************************/
		SubmixVoicePtr GetSubmixVoice() { return _submixVoice; }

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		AudioSubmix() = default;

		AudioSubmix(const AudioMasterPtr& audioMaster, const std::uint32_t channel, const std::uint32_t samplingRate);

		~AudioSubmix();

	protected:
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/

		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		
		/****************************************************************************
		**                Protected Property
		*****************************************************************************/
		SubmixVoicePtr _submixVoice  = nullptr;
		AudioMasterPtr _audioMaster  = nullptr;
		std::uint32_t  _channel      = 0;
		std::uint32_t  _samplingRate = 0;
		bool _hasCreatedReverve = false;

	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/
		bool CreateReverb();
	};
}
#endif