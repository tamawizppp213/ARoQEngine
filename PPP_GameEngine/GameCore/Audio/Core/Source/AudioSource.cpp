//////////////////////////////////////////////////////////////////////////////////
///             @file   AudioSource.cpp
///             @brief  AudioSource.cpp
///             @author Toide Yutaro
///             @date   2022_12_23
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/AudioSource.hpp"
#include "../Include/AudioMaster.hpp"
#include "../../Private/Include/WavDecoder.hpp"
#include "../Include/AudioClip.hpp"
#include "GameCore/Core/Include/ResourceManager.hpp"

#define XAUDIO2_HELPER_FUNCTIONS
#include <xaudio2.h>
#include <xaudio2fx.h>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gc::audio;

namespace
{
	constexpr float WAIT_FADE_TIME = 0.01f;
}
//////////////////////////////////////////////////////////////////////////////////
//                             Implement
//////////////////////////////////////////////////////////////////////////////////
AudioSource::AudioSource(const std::shared_ptr<AudioMaster>& audioMaster)
	: _audioMaster(audioMaster), _maxPitch(XAUDIO2_DEFAULT_FREQ_RATIO), _maxVolume(2.0f)
{
	_isLoop = false;	
}

AudioSource::AudioSource(const std::shared_ptr<AudioMaster>& audioMaster, const AudioClipPtr& audioClip, const SoundType soundType, const float volume)
	:_audioMaster(audioMaster), _maxPitch(XAUDIO2_DEFAULT_FREQ_RATIO), _maxVolume(2.0f)
{
	SetUp(audioClip, soundType, volume);
}

AudioSource::~AudioSource()
{
	_audioClip  .reset();
	
	if (_sourceVoice)
	{
		Stop();
		_sourceVoice->DestroyVoice();
	}

	_audioMaster.reset();
}
#pragma region Public Function
/****************************************************************************
*                       Update
*************************************************************************//**
*  @fn        void AudioSource::Update(const float deltaTime)
*
*  @brief     This function is used to perform a fade
*
*  @param[in] const float deltaTime in seconds.
*
*  @return    void
*****************************************************************************/
void AudioSource::Update(const float deltaTime)
{
	
}

/****************************************************************************
*                       Play
*************************************************************************//**
*  @fn        bool AudioSource::Play()
* 
*  @brief     Play sound. (Playback from the beginning at any time
* 
*  @param[in] const float loopBeginSeconds
*  @param[in] const float loopIntervalSeconds (Playback to end at 0.0f)
* 
*  @return    bool
*****************************************************************************/
bool AudioSource::Play(const float loopBeginSeconds, const float loopIntervalSeconds)
{
	if (IsPlaying())
	{
		if (!SafeClearSourceBuffer()) { return false; };
	}
	else
	{
		_sourceVoice->FlushSourceBuffers();
	}

	IsLoop() ? FlushAudioData(loopBeginSeconds, loopIntervalSeconds) : FlushAudioData();

	_sourceVoice->Start(0);
	return true;
}
/****************************************************************************
*                       Pause
*************************************************************************//**
*  @fn        bool AudioSource::Pause()
* 
*  @brief     Sound Pause.
* 
*  @param[in] void
* 
*  @return 　　bool
*****************************************************************************/
bool AudioSource::Pause()
{
	if (!IsPlaying()) { return false; }
	
	_sourceVoice->Stop(XAUDIO2_PLAY_TAILS);
	return true;
}
/****************************************************************************
*                       Stop
*************************************************************************//**
*  @fn        bool AudioSource::Stop()
* 
*  @brief     Sound Stop.
* 
*  @param[in] void
* 
*  @return 　　bool
*****************************************************************************/
bool AudioSource::Stop()
{
	if (!IsPlaying()) { return false; }

	return SafeClearSourceBuffer();
}
/****************************************************************************
*                       Replay
*************************************************************************//**
*  @fn        bool AudioSource::Replay()
* 
*  @brief     Restart sound. Play from the middle part
* 
*  @param[in] void
* 
*  @return 　　bool
*****************************************************************************/
bool AudioSource::Replay()
{
	if (!IsPlaying()) { return false; }
	
	_sourceVoice->Start(0);
	return true;
}

/****************************************************************************
*                       ExitLoop
*************************************************************************//**
*  @fn        bool AudioSource::ExitLoop()
* 
*  @brief     ExitLoop.
* 
*  @param[in] void
* 
*  @return    bool
*****************************************************************************/
bool AudioSource::ExitLoop()
{
	if (!_isLoop)     { return false; }

	if (!IsPlaying()) { return false; }
	
	_sourceVoice->ExitLoop();
	return true;
}

/****************************************************************************
*                       SetUp
*************************************************************************//**
*  @fn        bool AudioSource::SetUp(const AudioClipPtr& audioClip, const SoundType soundType, const float volume)
* 
*  @brief     Set audio clip.
* 
*  @param[in] const AudioClipPtr& audioClip
* 
*  @param[in] const SoundType soundType
* 
*  @param[in] const float volume : (0.0f (Attenuation) ～ 1.0f (Default) ～ 2.0f (Amplifier))
* 
*  @return 　　bool
*****************************************************************************/
bool AudioSource::SetUp(const AudioClipPtr& audioClip, const SoundType soundType, const float volume)
{
	/*-------------------------------------------------------------------
	-                 Load check. To reduce processing load
	---------------------------------------------------------------------*/
	if (_hasLoaded) { OutputDebugStringA("Already loaded."); return false; }

	/*-------------------------------------------------------------------
	-                Set up audio clip
	---------------------------------------------------------------------*/
	_audioClip = audioClip;

	/*-------------------------------------------------------------------
	-                Create source voice
	---------------------------------------------------------------------*/
	if (!CreateSourceVoice())        { return false; }

	/*-------------------------------------------------------------------
	-            BGM, BLS -> Loop On, SE, ME -> Loop Off
	---------------------------------------------------------------------*/
	if (!SelectIsLoop(soundType)) { return false; }

	/*-------------------------------------------------------------------
	-            Set up audio volume
	---------------------------------------------------------------------*/
	if (!SetVolume(volume)) { return false; }

	SetPitch(1.0f);

	_hasLoaded = true;
	return true;
}

/****************************************************************************
*                       SetPan
*************************************************************************//**
*  @fn        bool AudioSource::SetPan(float pan)
* 
*  @brief     Changes the balance of the sound output from the headset or speakers.
* 
*  @param[in] void
* 
*  @return 　　bool
* 
*  @details   [-1.0] left only [ 0.0] both [+1.0] right only   \n
*             If you set a value such as -0.75f, the output will be 75% left and 25% right.
*****************************************************************************/
bool AudioSource::SetPan(float pan)
{
	if (!IsExistedSourceVoice()) { return false; }

	/*-------------------------------------------------------------------
	-              Threshold
	---------------------------------------------------------------------*/
	if (pan > 1.0f)  { pan = 1.0f; }
	if (pan < -1.0f) { pan = -1.0f; }

	/*-------------------------------------------------------------------
	-              Get Voice Details
	---------------------------------------------------------------------*/
	XAUDIO2_VOICE_DETAILS details;
	_sourceVoice->GetVoiceDetails(&details);

	int channelSize = details.InputChannels * 2;
	float* matrix = new float[channelSize];
	float halfPan = pan / 2.0f;

	/*-------------------------------------------------------------------
	-              Set left and right panning
	---------------------------------------------------------------------*/
	for (int i = 0; i < channelSize; ++i)
	{
		if (i % 2 == 0)
		{
			matrix[i] = 0.5f - halfPan;
		}
		else if (i % 2 == 1)
		{
			matrix[i] = 0.5f + halfPan;
		}

	}

	_sourceVoice->SetOutputMatrix(nullptr, details.InputChannels, 2, matrix);

	delete[] matrix;
	return true;

}

/****************************************************************************
*                       IsUseReverb
*************************************************************************//**
*  @fn        bool AudioSource::IsUseReverb()
*  @brief     Sets the enable flag for Reverb (reverberation effect).
*  @param[in] bool isReverb
*  @return 　　void
*****************************************************************************/
bool AudioSource::IsUseReverb(bool isReverb)
{
	if (!IsExistedSourceVoice()) { return false; }

	if (isReverb)
	{
		_sourceVoice->EnableEffect(0);
	}
	else
	{
		_sourceVoice->DisableEffect(0);
	}
	return true;
}


/****************************************************************************
*                       IsPlaying
*************************************************************************//**
*  @fn        bool AudioSource::IsPlaying()
* 
*  @brief     Is playing (true-> playing, false -> not playing)
* 
*  @param[in] void
* 
*  @return 　　bool
*****************************************************************************/
bool AudioSource::IsPlaying()
{
	/*-------------------------------------------------------------------
	-              Nullptr check
	---------------------------------------------------------------------*/
	if (!IsExistedSourceVoice()) { return false; }

	/*-------------------------------------------------------------------
	-              Acquire source voice state
	---------------------------------------------------------------------*/
	XAUDIO2_VOICE_STATE xState = {};

#if (_WIN32_WINNT >= _WIN32_WINNT_WIN8)
	_sourceVoice->GetState(&xState, XAUDIO2_VOICE_NOSAMPLESPLAYED);
#else
	_sourceVoice->GetState(&xState);
#endif

	// xState.BuffersQueued == 0 -> Finish playing audio, xState.BuffersQueued > 0 -> playing.
	// buffersQueued : Number of buffers remaining that need to be played
	return xState.BuffersQueued > 0;
}

/****************************************************************************
*                       GetPitch
*************************************************************************//**
*  @fn        float AudioSource::GetPitch()
* 
*  @brief     Get Current Pitch
* 
*  @param[in] void
* 
*  @return 　　float
*****************************************************************************/
float AudioSource::GetPitch()
{
	float pitch = XAUDIO2_MIN_FREQ_RATIO;
	if (IsExistedSourceVoice())
	{
		_sourceVoice->GetFrequencyRatio(&pitch);
	}
	return pitch;
}

/****************************************************************************
*                       SetPitch
*************************************************************************//**
*  @fn        void AudioSource::SetPitch(float pitch)
*  @brief     Set Current Pitch
*  @param[in] void
*  @return 　　float
*****************************************************************************/
void AudioSource::SetPitch(float pitch)
{
	if (IsExistedSourceVoice())
	{
		_sourceVoice->SetFrequencyRatio(max(min(pitch, _maxPitch), XAUDIO2_MIN_FREQ_RATIO));
	}
	else
	{
		::OutputDebugString(L"Couldn't set pitch");
	}
}

/****************************************************************************
*                       SetMaxPitch
*************************************************************************//**
*  @fn        void AudioSource::SetMaxPitch(float maxPitch)
*  @brief     Set Max Pitch
*  @param[in] float maxPitch
*  @return 　　void
*****************************************************************************/
void AudioSource::SetMaxPitch(float maxPitch)
{
	_maxPitch = max(min(maxPitch, XAUDIO2_DEFAULT_FREQ_RATIO), XAUDIO2_MIN_FREQ_RATIO);
}
/****************************************************************************
*                       AdjustPitch
*************************************************************************//**
*  @fn        void AudioSource::AdjustPitch(float diffPitch)
*  @brief     This function will add or subtract from the current pitch.
*  @param[in] void
*  @return 　　void
*  @details   This function will not increase the pitch beyond the maximum pitch that has been set.
*             This function will not increase the pitch beyond the set maximum pitch, nor will it decrease it below 0.0005.
*****************************************************************************/
void AudioSource::AdjustPitch(float diffPitch)
{
	SetPitch(GetPitch() + diffPitch);
}

#pragma endregion Pitch
#pragma region Volume
/****************************************************************************
*                       GetVolume
*************************************************************************//**
*  @fn        float AudioSource::GetVolume() const]
*
*  @brief     Get Current Volume
* 
*  @param[in] void
* 
*  @return 　　float
*****************************************************************************/
float AudioSource::GetVolume() const
{
	if (!IsExistedSourceVoice()) { return 0.0f; }
	
	float volume = 0.0f;
	_sourceVoice->GetVolume(&volume);
	return volume;
}

/****************************************************************************
*                       SetVolume
*************************************************************************//**
*  @fn        void AudioSource::SetVolume(float volume)
*  @brief     Set Current Volume
*  @param[in] float volume
*  @return 　　void
*****************************************************************************/
bool AudioSource::SetVolume(float volume)
{
	if (!IsExistedSourceVoice()) { return false; }
	
	// 0 <= adjustVolume <= _maxVolume
	const auto result = _sourceVoice->SetVolume(max(min(volume, _maxVolume), 0.0f));
	return result == TRUE ? true : false;
}

/****************************************************************************
*                       AdjustVolume
*************************************************************************//**
*  @fn        void AudioSource::AdjustVolume(float diffVolume)
*  @brief     Adjust Current Volume
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void AudioSource::AdjustVolume(float diffVolume)
{
	SetVolume(GetVolume() + diffVolume);
}

bool AudioSource::SetMaxVolumeLimit(const float maxVolume)
{
	if (maxVolume < 0.0f)                     { return false; }
	if (maxVolume > XAUDIO2_MAX_VOLUME_LEVEL) { return false; }
	
	_maxVolume = maxVolume;
	return true;
}
#pragma endregion Public Function
#pragma region Protected Function
/****************************************************************************
*                       FlushAudioData
*************************************************************************//**
*  @fn        void AudioSource::FlushAudioData()
* 
*  @brief     Flush Audio Data
* 
*  @param[in] void
* 
*  @return    void
*****************************************************************************/
void AudioSource::FlushAudioData(const float loopBeginSeconds, const float loopIntervalSeconds)
{
	/* If loopEndSeconds is set to 0.0f, it will loop until the end of the sound*/
	
	XAUDIO2_BUFFER buffer =
	{
		.Flags      = XAUDIO2_END_OF_STREAM,                // tell the source voice not to expect any data after this buffer
		.AudioBytes = static_cast<UINT32>(_audioClip->GetSoundSize()),
		.pAudioData = _audioClip->GetSoundData().get(),
		.PlayBegin  = 0, // play begin sample count
		.PlayLength = 0, // All sample play
		.LoopBegin  = (UINT32)(loopBeginSeconds    * _audioClip->GetSamplingFrequency()),
		.LoopLength = (UINT32)(loopIntervalSeconds * _audioClip->GetSamplingFrequency()),
		.LoopCount  = IsLoop() ? (UINT32)XAUDIO2_LOOP_INFINITE : 0,
		.pContext   = nullptr
	};
	
	// Adding waveform data to the source voice 
	// Max : 64 XAUDIO2_MAX_QUEUEED_BUFFERS
	if (_sourceVoice) { _sourceVoice->SubmitSourceBuffer(&buffer); }
}


/****************************************************************************
*                       CreateSourceVoice
*************************************************************************//**
*  @fn        bool AudioSource::CreateSourceVoice()
* 
*  @brief     Load Xaudio SourceVoice
* 
*  @param[in] void
* 
*  @return    bool
*****************************************************************************/
bool AudioSource::CreateSourceVoice()
{
	/*-------------------------------------------------------------------
	-               Get XAudio2
	---------------------------------------------------------------------*/
	const auto xAudio = _audioMaster->GetAudioInterface();

	if (!xAudio) { OutputDebugStringA("Mastering Voice is nullptr"); return false; }

	/*-------------------------------------------------------------------
	-               Create Source Voice
	---------------------------------------------------------------------*/
	// Todo : 後で色々設定を追加する. 
	HRESULT hresult = xAudio->CreateSourceVoice(&_sourceVoice, &_audioClip->GetFileFormatEx());
	
	if (FAILED(hresult))
	{
		OutputDebugStringA("Couldn't create source voice.");
		return false;
	}

	return true;
}

/****************************************************************************
*                       CreateReverb
*************************************************************************//**
*  @fn        bool AudioSource::CreateReverb()
*  @brief     Create Reverb
*  @param[in] void
*  @return 　　bool
*****************************************************************************/
bool AudioSource::CreateReverb()
{
	IUnknown* reverb;
	if (FAILED(XAudio2CreateReverb(&reverb)))
	{
		MessageBox(NULL, L"Couldn't create reverb.", L"Warning", MB_ICONWARNING);
		return false;
	}

	XAUDIO2_EFFECT_DESCRIPTOR effectDesc ={};
	effectDesc.InitialState   = false;
	effectDesc.pEffect        = reverb;
	effectDesc.OutputChannels = 1;

	XAUDIO2_EFFECT_CHAIN effectChain = {};
	effectChain.EffectCount = 1;
	effectChain.pEffectDescriptors = &effectDesc;
	_sourceVoice->SetEffectChain(&effectChain);

	return true;
}

/****************************************************************************
*                       SelectSoundType
*************************************************************************//**
*  @fn        bool AudioSource::SelectIsLoop(const SoundType soundType)
* 
*  @brief     Change the number of loops by SoundType.
* 
*  @param[in] void
* 
*  @return 　　bool
*****************************************************************************/
bool AudioSource::SelectIsLoop(const SoundType soundType)
{
	switch (soundType)
	{
		case SoundType::BGM:
		case SoundType::BGS:
			_isLoop = true;
			break;
		case SoundType::ME:
		case SoundType::SE:
			_isLoop = false;
			break;
		case SoundType::NONE:
		default:
			MessageBox(NULL, L"Please set sound type", L"Warning", MB_ICONWARNING);
			return false;
	}

	_soundType = soundType;
	return true;
}

bool AudioSource::SafeClearSourceBuffer()
{
	if (FAILED(_sourceVoice->Stop(0))){return false;};
	if (FAILED(_sourceVoice->FlushSourceBuffers())) { return false; }
	return true;
}
#pragma endregion Protected Function