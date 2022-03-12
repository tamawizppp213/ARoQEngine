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
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
struct IXAudio2;
struct IXAudio2MasteringVoice;

/****************************************************************************
*				  			AudioMaster
*************************************************************************//**
*  @class     AudioMaster
*  @brief     Audio master
*****************************************************************************/
class AudioMaster
{
public:
	using IXAudio2Ptr               = Microsoft::WRL::ComPtr<IXAudio2>;
	using IXAudio2MasteringVoicePtr = IXAudio2MasteringVoice*;
	using X3DAudioHandler           = unsigned char[20];
public:
	/****************************************************************************
	**                Public Function
	*****************************************************************************/
	static bool Initialize();
	static void Finalize();

	/****************************************************************************
	**                Public Member Variables
	*****************************************************************************/
	static const IXAudio2Ptr&               GetAudioInterface();
	static const IXAudio2MasteringVoicePtr& GetMasteringVoice();
	static const X3DAudioHandler&           GetX3DAudioInterface();
	/****************************************************************************
	**                Constructor and Destructor
	*****************************************************************************/
	AudioMaster() = default;
	~AudioMaster() = default;

private:
	/****************************************************************************
	**                Constructor and Destructor
	*****************************************************************************/
	
	/****************************************************************************
	**                Private Function
	*****************************************************************************/
	static bool CreateXAudio2();
	static bool CreateMasterVoice();
	static bool CreateX3DAudio();
	/****************************************************************************
	**                Private Member Variables
	*****************************************************************************/
	static IXAudio2Ptr _xAudio;
	static IXAudio2MasteringVoicePtr _masterVoice;
	static X3DAudioHandler _x3dAudio;
	static unsigned int    _debugFlag;
	static bool _hasInitialized;

};
#endif