//////////////////////////////////////////////////////////////////////////////////
///             @file   AudioManager.cpp
///             @brief  Audio Master Voice Management.
///             @author Toide Yutaro
///             @date   2021_01_03
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Include/Audio/AudioMaster.hpp"
#include <xaudio2.h>
#include <xaudio2fx.h>
#include <x3daudio.h>
#pragma comment(lib, "xaudio2.lib")

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
AudioMaster::IXAudio2Ptr AudioMaster::_xAudio                    = nullptr;
AudioMaster::IXAudio2MasteringVoicePtr AudioMaster::_masterVoice = nullptr;
X3DAUDIO_HANDLE AudioMaster::_x3dAudio                           = { 0 };
UINT32 AudioMaster::_debugFlag    = 0;
bool AudioMaster::_hasInitialized = true;

//////////////////////////////////////////////////////////////////////////////////
//                             Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Public Function
/****************************************************************************
*                       Initialize
*************************************************************************//**
*  @fn        bool AudioMaster::Initialize()
*  @brief     Initialize Audio Interface
*  @param[in] void
*  @return 　　bool
*****************************************************************************/
bool AudioMaster::Initialize()
{
	if (_hasInitialized) { return false; }
	if (!CreateXAudio2())     { return false; }
	if (!CreateMasterVoice()) { return false; }
	if (!CreateX3DAudio())    { return false; }

	return true;
}

/****************************************************************************
*                       Finalize
*************************************************************************//**
*  @fn        bool AudioMaster::Finalize()
*  @brief     Finalize Audio Interface
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void AudioMaster::Finalize()
{
	/*-------------------------------------------------------------------
	-              Destroy Master Voice
	---------------------------------------------------------------------*/
	_masterVoice->DestroyVoice();
	_masterVoice = NULL;

	/*-------------------------------------------------------------------
	-              Release XAudio2 Object
	---------------------------------------------------------------------*/
	if (_xAudio != NULL)
	{
		_xAudio->Release();
		_xAudio = nullptr;
	}

	/*-------------------------------------------------------------------
	-              Destroy COM Library
	---------------------------------------------------------------------*/
	CoUninitialize();
}
/****************************************************************************
*                       GetAudioInterface
*************************************************************************//**
*  @fn        IXAudio2* AudioMaster::GetAudioInterface() const
*  @brief     Get XAudio2 Interface
*  @param[in] void
*  @return 　　IXAudio2*
*****************************************************************************/
const AudioMaster::IXAudio2Ptr& AudioMaster::GetAudioInterface()
{
	return _xAudio;
}

/****************************************************************************
*                       GetMasteringVoice
*************************************************************************//**
*  @fn        IXAudio2MasteringVoice* AudioMaster::GetMasteringVoice() const
*  @brief     Get Mastering Voice
*  @param[in] void
*  @return 　　IXAudio2MasteringVoice*
*****************************************************************************/
const AudioMaster::IXAudio2MasteringVoicePtr& AudioMaster::GetMasteringVoice()
{
	return _masterVoice;
}

/****************************************************************************
*                       GetX3DAudioInterface
*************************************************************************//**
*  @fn        X3DAUDIO_HANDLE& AudioMaster::GetX3DAudioInterface() const
*  @brief     Get X3DAudio2 Interface
*  @param[in] void
*  @return 　　X3DAUDIO_HANDLE&
*****************************************************************************/
const AudioMaster::X3DAudioHandler& AudioMaster::GetX3DAudioInterface()
{
	return _x3dAudio;
}
#pragma endregion Public Function
#pragma region Protected Function
/****************************************************************************
*                       CreateXAudio2
*************************************************************************//**
*  @fn        bool AudioMaster::CreateXAudio2()
*  @brief     Create XAudio
*  @param[in] void
*  @return 　　bool
*****************************************************************************/
bool AudioMaster::CreateXAudio2()
{
	/*-------------------------------------------------------------------
	-              COM initialize
	---------------------------------------------------------------------*/
	if (FAILED(CoInitializeEx(NULL, COINIT_MULTITHREADED)))
	{
		MessageBox(NULL, L"can't initialize COM.", L"Warning", MB_ICONWARNING);
		return false;
	}

	/*-------------------------------------------------------------------
	-              Check debug flag
	---------------------------------------------------------------------*/
#ifdef _DEBUG
	_debugFlag |= XAUDIO2_DEBUG_ENGINE;
#endif

	/*-------------------------------------------------------------------
	-              Create XAudio2
	---------------------------------------------------------------------*/
	if (!FAILED(XAudio2Create(AudioMaster::_xAudio.GetAddressOf(), _debugFlag)))
	{
		MessageBox(NULL, L"can't initialize COM.", L"Warning", MB_ICONWARNING);
		CoUninitialize();
		return false;
	}

	return true;
}

/****************************************************************************
*                       CreateMasterVoice
*************************************************************************//**
*  @fn        bool AudioMaster::CreateMasterVoice()
*  @brief     Create Master Voice for Initialize
*  @param[in] void
*  @return 　　bool
*****************************************************************************/
bool AudioMaster::CreateMasterVoice()
{
	if (FAILED(_xAudio->CreateMasteringVoice(&_masterVoice)))
	{
		MessageBox(NULL, L"can't create Mastering Voice.", L"Warning", MB_ICONWARNING);

		if (_xAudio != nullptr)
		{
			_xAudio->Release();
			_xAudio = nullptr;
		}

		CoUninitialize();
		return false;
	}
	return true;
}

/****************************************************************************
*                       CreateX3DAudio
*************************************************************************//**
*  @fn        bool AudioMaster::CreateX3DAudio()
*  @brief     Create X3DAudio for Initialize
*  @param[in] void
*  @return 　　bool
*****************************************************************************/
bool AudioMaster::CreateX3DAudio()
{
	if (FAILED(X3DAudioInitialize(SPEAKER_STEREO, X3DAUDIO_SPEED_OF_SOUND, _x3dAudio)))
	{
		MessageBox(NULL, L"can't initialize x3dAudio.", L"Warning", MB_ICONWARNING);
		return false;
	}
	return true;
}

#pragma endregion Protected Function