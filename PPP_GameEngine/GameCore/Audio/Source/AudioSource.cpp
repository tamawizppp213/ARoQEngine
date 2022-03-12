//////////////////////////////////////////////////////////////////////////////////
///             @file   AudioSource.cpp
///             @brief  AudioSource.cpp
///             @author Toide Yutaro
///             @date   2021_01_12
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Audio/Include/AudioSource.hpp"
#include "GameCore/Audio/Include/AudioMaster.hpp"
#include "GameCore/Audio/Include/WavDecoder.hpp"
#include "GameCore/Audio/Include/AudioClip.hpp"
#include "GameCore/Core/Include/ResourceManager.hpp"
#include <xaudio2.h>
#include <xaudio2fx.h>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
static constexpr float DEFAULT_VOLUME = 1.0f;
static constexpr float MAX_VOLUME = 2.0f;

//////////////////////////////////////////////////////////////////////////////////
//                             Implement
//////////////////////////////////////////////////////////////////////////////////
float AudioSource::_volumeRatio[] = { DEFAULT_VOLUME ,DEFAULT_VOLUME ,DEFAULT_VOLUME ,DEFAULT_VOLUME };

AudioSource::AudioSource()
{
	_maxVolume = MAX_VOLUME;
	_maxPitch = XAUDIO2_DEFAULT_FREQ_RATIO;
	_isLoop = false;
	
}
AudioSource::~AudioSource()
{

}
#pragma region Public Function
/****************************************************************************
*                       Play
*************************************************************************//**
*  @fn        bool AudioSource::Play()
*  @brief     Play audio data. If it has been paused before, it will resume from the memory position.
*  @param[in] void
*  @return 　　bool
*****************************************************************************/
bool AudioSource::Play()
{
	if (!IsRemainedSourceBufferQueue())
	{
		_sourceVoice->FlushSourceBuffers();
		FlushAudioData();
	}
	else
	{
		_sourceVoice->Stop(0);
		_sourceVoice->FlushSourceBuffers();
	}

	_sourceVoice->Start(0);
	return true;
}
/****************************************************************************
*                       Pause
*************************************************************************//**
*  @fn        bool AudioSource::Pause()
*  @brief     Sound Pause.
*  @param[in] void
*  @return 　　bool
*****************************************************************************/
bool AudioSource::Pause()
{
	if (IsRemainedSourceBufferQueue())
	{
		_sourceVoice->Stop(0);
		return true;
	}
	else { return false; }
}
/****************************************************************************
*                       Stop
*************************************************************************//**
*  @fn        bool AudioSource::Stop()
*  @brief     Sound Stop.
*  @param[in] void
*  @return 　　bool
*****************************************************************************/
bool AudioSource::Stop()
{
	if (IsRemainedSourceBufferQueue())
	{
		_sourceVoice->Stop(0);
		_sourceVoice->FlushSourceBuffers();
		return true;
	}
	else { return false; }
}
/****************************************************************************
*                       Replay
*************************************************************************//**
*  @fn        bool AudioSource::Replay()
*  @brief     Sound Replay.
*  @param[in] void
*  @return 　　bool
*****************************************************************************/
bool AudioSource::Replay()
{
	if (IsRemainedSourceBufferQueue())
	{
		Stop();
		Play();
		return true;
	}
	else { return false; }
}
/****************************************************************************
*                       ExitLoop
*************************************************************************//**
*  @fn        bool AudioSource::ExitLoop()
*  @brief     ExitLoop.
*  @param[in] void
*  @return 　　bool
*****************************************************************************/
bool AudioSource::ExitLoop()
{
	if (!_isLoop)
	{
		return false;
	}

	if (IsRemainedSourceBufferQueue())
	{
		_sourceVoice->ExitLoop();
		return true;
	}
	else { return false; }
}
/****************************************************************************
*                       LoadSound
*************************************************************************//**
*  @fn        bool AudioSource::LoadSound(const std::wstring& filePath, SoundType soundType, float volume)
*  @brief     Load Sound
*  @param[in] const std::wstring& filePath
*  @param[in] SoundType soundType
*  @param[in] float volume : (0.0f (Attenuation) ～ 1.0f (Default) ～ 2.0f (Amplifier))
*  @return 　　bool
*****************************************************************************/
bool AudioSource::LoadSound(const std::wstring& filePath, SoundType soundType, float volume)
{
	if (!LoadAudioClip(filePath))    { return false; }
	if (!CreateSourceVoice())        { return false; }
	if (!CreateReverb())             { return false; }
	if (!SelectSoundType(soundType)) { return false; }
	if (!SetVolume(volume)) { return false; }
	return true;
}
/****************************************************************************
*                       SetPan
*************************************************************************//**
*  @fn        bool AudioSource::SetPan()
*  @brief     Changes the balance of the sound output from the headset or speakers.
*  @param[in] void
*  @return 　　bool
*  @details   [-1.0] left only [ 0.0] both [+1.0] right only   \n
*             If you set a value such as -0.75f, the output will be 75% left and 25% right.
*****************************************************************************/
bool AudioSource::SetPan(float pan)
{
	if (!IsExistedSourceVoice()) { return false; }

	/*-------------------------------------------------------------------
	-              Threshold
	---------------------------------------------------------------------*/
	if (pan > 1.0f) { pan = 1.0f; }
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
*                       IsLoop
*************************************************************************//**
*  @fn        bool AudioSource::IsLoop()
*  @brief     Is Loop (true-> loop, false -> not loop)
*  @param[in] void
*  @return 　　bool
*****************************************************************************/
bool AudioSource::IsLoop() const
{
	return this->_isLoop;
}

/****************************************************************************
*                       IsPlaying
*************************************************************************//**
*  @fn        bool AudioSource::IsPlaying()
*  @brief     Is playing (true-> playing, false -> not playing)
*  @param[in] void
*  @return 　　bool
*****************************************************************************/
bool AudioSource::IsPlaying()
{
	return IsRemainedSourceBufferQueue();
}

/****************************************************************************
*                       GetPitch
*************************************************************************//**
*  @fn        float AudioSource::GetPitch()
*  @brief     Get Current Pitch
*  @param[in] void
*  @return 　　float
*****************************************************************************/
float AudioSource::GetPitch()
{
	float pitch = XAUDIO2_MIN_FREQ_RATIO;
	if (IsExistedSourceVoice())
	{
		_sourceVoice->GetVolume(&pitch);
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
*  @fn        float AudioSource::GetVolume()
*  @brief     Get Current Volume
*  @param[in] void
*  @return 　　float
*****************************************************************************/
float AudioSource::GetVolume()
{
	float volume = 0.0f;
	if (IsExistedSourceVoice())
	{
		_sourceVoice->GetVolume(&volume);
	}
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
	if (IsExistedSourceVoice())
	{
		float adjustVolume = volume * _volumeRatio[(int)_soundType];
		_sourceVoice->SetVolume(max(min(adjustVolume, _maxVolume), 0));
		return true;
	}
	else
	{
		::OutputDebugString(L"Couldn't set volume");
		return false;
	}

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
#pragma endregion Public Function
#pragma region Protected Function
/****************************************************************************
*                       FlushAudioData
*************************************************************************//**
*  @fn        void AudioSource::FlushAudioData()
*  @brief     Flush Audio Data
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void AudioSource::FlushAudioData()
{
	XAUDIO2_BUFFER buffer;
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = static_cast<UINT32>(_audioClip->GetSoundSize());
	buffer.pAudioData = _audioClip->GetSoundData().get();
	buffer.LoopCount  = IsLoop() ? XAUDIO2_LOOP_INFINITE : 0;
	buffer.Flags      = XAUDIO2_END_OF_STREAM;

	// Nullptr check has been done in advance.
	if (_sourceVoice)_sourceVoice->SubmitSourceBuffer(&buffer);
}

/****************************************************************************
*                       IsExistedSourceVoice
*************************************************************************//**
*  @fn        void AudioSource::IsExistedSourceVoice()
*  @brief     is Existed source voice (true -> exist, false -> not exist)
*  @param[in] void
*  @return 　　bool
*****************************************************************************/
bool AudioSource::IsExistedSourceVoice()
{
	if (_sourceVoice == nullptr)
	{
		//OutputDebugString(L"SourceVoice is nullptr.");
		return false;
	}

	return true;
}

/****************************************************************************
*                       IsRemainedSourceBufferQueue
*************************************************************************//**
*  @fn        void AudioSource::IsRemainedSourceBufferQueue)
*  @brief     is remained source buffer queue (true -> remain, false -> not remain)
*  @param[in] void
*  @return 　　bool
*****************************************************************************/
bool AudioSource::IsRemainedSourceBufferQueue()
{
	/*-------------------------------------------------------------------
	-              Nullptr check
	---------------------------------------------------------------------*/
	if (!IsExistedSourceVoice()) { return false; }

	/*-------------------------------------------------------------------
	-              Check buffer queue
	---------------------------------------------------------------------*/
	XAUDIO2_VOICE_STATE xState;
	_sourceVoice->GetState(&xState);

	if (xState.BuffersQueued == 0)
	{
		return false;
	}

	return true;
}

/****************************************************************************
*                       LoadAudioClip
*************************************************************************//**
*  @fn        bool AudioSource::LoadAudioClip(const std::wstring& filePath)
*  @brief     Load Audio Clip
*  @param[in] void
*  @return 　　bool
*****************************************************************************/
bool AudioSource::LoadAudioClip(const std::wstring& filePath)
{
	/*-------------------------------------------------------------------
	-               If the file is loaded once, read from it
	---------------------------------------------------------------------*/
	if (ResourceManager::Instance().ExistsAudioClip(filePath))
	{
		_audioClip = std::make_shared<AudioClip>(ResourceManager::Instance().GetAudioClip(filePath));
		return true;
	}

	/*-------------------------------------------------------------------
	-               Create Audio Clip
	---------------------------------------------------------------------*/
	AudioClip& audioClip = ResourceManager::Instance().GetAudioClip(filePath);
	if (audioClip.LoadFromFile(filePath))
	{
		_audioClip = std::make_shared<AudioClip>(audioClip);
		return true;
	}
	else
	{
		::OutputDebugString(L"couldn't create audio clip");
		return false;
	}
}

/****************************************************************************
*                       CreateSourceVoice
*************************************************************************//**
*  @fn        bool AudioSource::CreateSourceVoice()
*  @brief     Load Xaudio SourceVoice
*  @param[in] void
*  @return 　　bool
*****************************************************************************/
bool AudioSource::CreateSourceVoice()
{
	/*-------------------------------------------------------------------
	-               Create Source Voice
	---------------------------------------------------------------------*/
	Microsoft::WRL::ComPtr<IXAudio2> xAudio = AudioMaster::GetAudioInterface();

	HRESULT hresult = xAudio->CreateSourceVoice(&_sourceVoice, &_audioClip->GetFileFormatEx());
	if (FAILED(hresult))
	{
		MessageBox(NULL, L"Couldn't create source voice.", L"Warning", MB_ICONWARNING);
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
*  @fn        bool AudioSource::SelectSoundType()
*  @brief     Change the number of loops by SoundType.
*  @param[in] void
*  @return 　　bool
*****************************************************************************/
bool AudioSource::SelectSoundType(SoundType soundType)
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
#pragma endregion Protected Function