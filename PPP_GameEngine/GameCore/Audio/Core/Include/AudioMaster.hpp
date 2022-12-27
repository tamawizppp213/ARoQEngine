//////////////////////////////////////////////////////////////////////////////////
///             @file   AudioMaster.hpp
///             @brief  AudioMaster.hpp
///             @author Toide Yutaro
///             @date   2021_01_03
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef AUDIO_MASTER_HPP
#define AUDIO_MASTER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include <wrl.h>
#include <memory>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
struct IXAudio2;
struct IXAudio2MasteringVoice;

namespace gc::audio
{
	/****************************************************************************
	*				  			AudioMaster
	*************************************************************************//**
	*  @class     AudioMaster
	*  @brief     Audio master
	*****************************************************************************/
	class AudioMaster
	{
	public:
		using IXAudio2Ptr = Microsoft::WRL::ComPtr<IXAudio2>;
		using IXAudio2MasteringVoicePtr = IXAudio2MasteringVoice*;
		using X3DAudioHandler = unsigned char[20];
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		IXAudio2Ptr               GetAudioInterface() const noexcept;
		IXAudio2MasteringVoicePtr GetMasteringVoice() const noexcept;
		const X3DAudioHandler& GetX3DAudioInterface();

		bool GetVolumeParamters(float* peakLevels, float* rmsParams, const UINT32 channel);
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		AudioMaster();

		~AudioMaster();

	private:
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/

		/****************************************************************************
		**                Private Function
		*****************************************************************************/
		bool CreateXAudio2();
		bool CreateMasteringVoice();
		bool CreateX3DAudio();
		bool CreateVolumeMeter();

		/****************************************************************************
		**                Private Member Variables
		*****************************************************************************/
		/* @brief: XAudio interface*/
		IXAudio2Ptr _xAudio = nullptr;

		/* @brief : Final output destination for audio*/
		IXAudio2MasteringVoicePtr _masteringVoice = nullptr;

		/* @brief : X3DAudio interface*/
		X3DAudioHandler _x3dAudio = {};

		/* @brief : This flag is used under Window 7.*/
		unsigned int    _debugFlag = 0;

		/* @brief : Initialized flags*/
		bool _hasInitialized = false;

	};
}
#endif