//////////////////////////////////////////////////////////////////////////////////
///             @file   AudioSubmix.cpp
///             @brief  AudioSubmix.cpp
///             @author Toide Yutaro
///             @date   2022_12_24
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/AudioSubmix.hpp"
#include "../Include/AudioMaster.hpp"
#include "../Include/AudioSource3D.hpp"
#include <stdexcept>
#include <xaudio2.h>
#include <xaudio2fx.h>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gc::audio;

//////////////////////////////////////////////////////////////////////////////////
//                             Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor
AudioSubmix::AudioSubmix(const AudioMasterPtr& audioMaster, const std::uint32_t channel, const std::uint32_t samplingRate)
	: _audioMaster(audioMaster), _channel(channel), _samplingRate(samplingRate)
{
	const auto xAudio = _audioMaster->GetAudioInterface();
	if (FAILED(xAudio->CreateSubmixVoice(&_submixVoice, _channel, _samplingRate))) { return; }
}

AudioSubmix::~AudioSubmix()
{
	if (_submixVoice){ _submixVoice->DestroyVoice(); }
}

#pragma endregion Constructor and Destructor

#pragma region Main Function
/****************************************************************************
*                       Regist
*************************************************************************//**
*  @fn        void AudioSubmix::Regist(const std::shared_ptr<AudioSource>& audioSource)
*
*  @brief     Regist
*
*  @param[in] const std::shared_ptr<AudioSource>& audioSource
*
*  @return 　　void
*****************************************************************************/
void AudioSubmix::Regist(const std::shared_ptr<AudioSource>& audioSource)
{
	if (!audioSource) { return; }

	/*-------------------------------------------------------------------
	-               Get Source Voice
	--------------------------------------------------------------------*/
	const auto sourceVoice = audioSource->GetSourceVoice();
	if (!sourceVoice) { return; }

	/*-------------------------------------------------------------------
	-               Regist this submixVoice -> source voice
	---------------------------------------------------------------------*/
	XAUDIO2_SEND_DESCRIPTOR sendDesc = { 0, _submixVoice };
	XAUDIO2_VOICE_SENDS     sendList = { 1, &sendDesc };
	if (FAILED(sourceVoice->SetOutputVoices(&sendList)))
	{
		throw std::runtime_error("Failed to regist output voice");
	}
}

void AudioSubmix::Reverb(bool isOn)
{
	if (!_hasCreatedReverve) { CreateReverb(); }

	if (isOn)
	{
		_submixVoice->EnableEffect(0);
	}
	else
	{
		_submixVoice->DisableEffect(0);
	}
}
#pragma endregion Main Function

#pragma region Set up Function
/****************************************************************************
*                       CreateReverb
*************************************************************************//**
*  @fn        bool AudioSource::CreateReverb()
*  @brief     Create Reverb
*  @param[in] void
*  @return 　　bool
*****************************************************************************/
bool AudioSubmix::CreateReverb()
{
	if (_hasCreatedReverve) { return true; }

	/*-------------------------------------------------------------------
	-              Create reverb XAPO effect
	---------------------------------------------------------------------*/
	IUnknown* reverb;
	if (FAILED(XAudio2CreateReverb(&reverb)))
	{
		OutputDebugStringA("Couldn't create reverb.");
		return false;
	}

	XAUDIO2_EFFECT_DESCRIPTOR effectDesc =
	{
		.pEffect        = reverb,
		.InitialState   = false,
		.OutputChannels = 2 // ここ1かも.
	};
	
	XAUDIO2_EFFECT_CHAIN effectChain = 
	{
		.EffectCount = 1,
		.pEffectDescriptors = &effectDesc
	};
	
	/*-------------------------------------------------------------------
	-              Set effect chain to submix voice
	---------------------------------------------------------------------*/
	if(FAILED(_submixVoice->SetEffectChain(&effectChain)))
	{
		OutputDebugStringA("Coundn't set effect chain");
		return false;
	};

	reverb->Release();
	_hasCreatedReverve = true;
	return true;
}
#pragma endregion Set up Function