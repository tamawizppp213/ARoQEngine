//////////////////////////////////////////////////////////////////////////////////
///             @file   AudioSource3D.cpp
///             @brief  AudioSource3D.cpp
///             @author Toide Yutaro
///             @date   2021_01_13
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Audio/Include/AudioSource3D.hpp"
#include "GameCore/Audio/Include/AudioMaster.hpp"
#include <xaudio2.h>
#include <x3daudio.h>

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gc::audio;
#define STEREO_AUDIO 2

//////////////////////////////////////////////////////////////////////////////////
//                             Implement
//////////////////////////////////////////////////////////////////////////////////
using namespace gm;
AudioSource3D::AudioSource3D(const AudioMasterPtr& audioMaster) : AudioSource(audioMaster)
{
	memset(&_emitter, 0, sizeof(_emitter));
	memset(&_listener, 0, sizeof(_listener));

	SetEmitterFront(Float3(0.0f, 0.0f, -1.0f));
	SetEmitterUp(Float3(0.0f, 1.0f, 0.0f));
	SetEmitterPosition(Float3(0.0f, 0.0f, 1.0f));

	SetListenerFront(Float3(0.0f, 0.0f, 1.0f));
	SetListenerUp(Float3(0.0f, 1.0f, 0.0f));
	SetListenerPosition(Float3(0.0f, 0.0f, -1.0f));

	_emitter.ChannelCount = 1;
	_emitter.CurveDistanceScaler = 1.0f;
	_emitter.DopplerScaler = 1.0f;
	_emitter.InnerRadius = 1;
}

AudioSource3D::~AudioSource3D()
{

}

/****************************************************************************
*                       Play
*************************************************************************//**
*  @fn        bool AudioSource3D::Play()
*  @brief     AudioSource Play
*  @param[in] void
*  @return 　　bool
*****************************************************************************/
bool AudioSource3D::Play()
{
	if (!ApplyChange()) { return false; }
	if (!AudioSource::Play()) { return false; }
	return true;
}

/****************************************************************************
*                       ApplyChange
*************************************************************************//**
*  @fn        void AudioSource3D::ApplyChange()
*  @brief     Apply the changes to the listener and audio source that you set.
*  @param[in] void
*  @return 　　void
*****************************************************************************/
bool AudioSource3D::ApplyChange()
{
	if (!IsExistedSourceVoice()) { return false; }

	/*-------------------------------------------------------------------
	-              Create Audio Listener
	---------------------------------------------------------------------*/
	X3DAUDIO_LISTENER listener;
	memset(&listener, 0, sizeof(listener));

	listener.OrientFront = _listener.Front;
	listener.OrientTop = _listener.Up;
	listener.Position = _listener.Position;
	listener.Velocity = _listener.Velocity;

	/*-------------------------------------------------------------------
	-              Create Audio Emitter
	---------------------------------------------------------------------*/
	X3DAUDIO_EMITTER emitter;
	memset(&emitter, 0, sizeof(emitter));

	emitter.OrientFront = _emitter.Front;
	emitter.OrientTop = _emitter.Up;
	emitter.Position = _emitter.Position;
	emitter.Velocity = _emitter.Velocity;
	emitter.ChannelCount = _emitter.ChannelCount;
	emitter.ChannelRadius = _emitter.ChannelRadius;
	emitter.CurveDistanceScaler = _emitter.CurveDistanceScaler;
	emitter.DopplerScaler = _emitter.DopplerScaler;
	emitter.InnerRadius = _emitter.InnerRadius;
	emitter.InnerRadiusAngle = _emitter.InnerRadiusAngle;

	/*-------------------------------------------------------------------
	-              Create 3DSound
	---------------------------------------------------------------------*/
	Audio3DParameter parameter = Calculate3DSound(&listener, &emitter);
	if (!IsExistedSourceVoice()) { return false; }

	_sourceVoice->SetOutputMatrix(nullptr, parameter.SourceChannel, parameter.DestinationChannel, parameter.Matrix);
	_sourceVoice->SetFrequencyRatio(parameter.Doppler);
	return true;
}

/****************************************************************************
*                       Calculate3DSound
*************************************************************************//**
*  @fn        Audio3DParameter& AudioSource3D::Calculate3DSound(const X3DAUDIO_LISTENER* listener, const X3DAUDIO_EMITTER* emitter)
*  @brief     Apply the changes to the listener and audio source that you set.
*  @param[in] const X3DAUDIO_LISTENER* listener
*  @param[in] const X3DAUDIO_EMITTER* emitter
*  @return 　　void
*****************************************************************************/
Audio3DParameter AudioSource3D::Calculate3DSound(const X3DAUDIO_LISTENER* listener, const X3DAUDIO_EMITTER* emitter)
{
	X3DAUDIO_DSP_SETTINGS dsp = { 0 };
	FLOAT32 matrix[12] = { 0 };
	dsp.SrcChannelCount = 1;
	dsp.DstChannelCount = STEREO_AUDIO;
	dsp.pMatrixCoefficients = matrix;

	///*-------------------------------------------------------------------
	//-              CalculateX3DAudio
	//---------------------------------------------------------------------*/
	//X3DAudioCalculate(
	//	AudioMaster::GetX3DAudioInterface(),
	//	listener,
	//	emitter,
	//	X3DAUDIO_CALCULATE_MATRIX | X3DAUDIO_CALCULATE_DOPPLER,
	//	&dsp);

	/*-------------------------------------------------------------------
	-              Copy dsp
	---------------------------------------------------------------------*/
	Audio3DParameter result;
	result.Doppler                   = dsp.DopplerFactor;
	result.DestinationChannel        = dsp.DstChannelCount;
	result.EmitterToListenerAngle    = dsp.EmitterToListenerAngle;
	result.EmitterToListenerDistance = dsp.EmitterToListenerDistance;
	result.EmitterVelocity           = dsp.EmitterVelocityComponent;
	result.ListenerVelocity          = dsp.ListenerVelocityComponent;
	result.LPFDirectCoefficient      = dsp.LPFDirectCoefficient;
	result.LPFReverbCoefficient      = dsp.LPFReverbCoefficient;
	memcpy(result.Matrix, dsp.pMatrixCoefficients, sizeof(matrix));
	result.ReverbLevel   = dsp.ReverbLevel;
	result.SourceChannel = dsp.SrcChannelCount;

	return result;
}
#pragma region Public Function
#pragma region Property
/****************************************************************************
*                       SetListenerFront
*************************************************************************//**
*  @fn        void AudioSource3D::SetListenerFront(const DirectX::XMFLOAT3& front)
*  @brief     Set Lister side front vector (local)
*  @param[in] const DirectX::XMFLOAT3& front
*  @return 　　void
*****************************************************************************/
void AudioSource3D::SetListenerFront(const Float3& front)
{
	this->_listener.Front = front;
}

/****************************************************************************
*                       SetListenerUp
*************************************************************************//**
*  @fn        void AudioSource3D::SetListenerUp(const DirectX::XMFLOAT3& up)
*  @brief     Set Lister side up vector
*  @param[in] const DirectX::XMFLOAT3& up
*  @return 　　void: This value must be orthogonal to the front vector.
*****************************************************************************/
void AudioSource3D::SetListenerUp(const Float3& up)
{
	this->_listener.Up = up;
}

/****************************************************************************
*                       SetListenerPosition
*************************************************************************//**
*  @fn        void AudioSource3D::SetListenerPosition(const DirectX::XMFLOAT3& position)
*  @brief     Set Lister side position
*  @param[in] const DirectX::XMFLOAT3& position
*  @return 　　void
*****************************************************************************/
void AudioSource3D::SetListenerPosition(const Float3& position)
{
	this->_listener.Position = position;
}

/****************************************************************************
*                       SetListenerVelocity
*************************************************************************//**
*  @fn        void AudioSource3D::SetListenerVelocity(const DirectX::XMFLOAT3& velocity)
*  @brief     Set Lister side velocity
*  @param[in] const DirectX::XMFLOAT3& velocity
*  @return 　　void
*****************************************************************************/
void AudioSource3D::SetListenerVelocity(const Float3& velocity)
{
	this->_listener.Velocity = velocity;
}

/****************************************************************************
*                       SetEmitterFront
*************************************************************************//**
*  @fn        void AudioSource3D::SetEmitterFront(const DirectX::XMFLOAT3& front)
*  @brief     Set Emitter side front vector
*  @param[in] const DirectX::XMFLOAT3& front
*  @return 　　void
*****************************************************************************/
void AudioSource3D::SetEmitterFront(const Float3& front)
{
	this->_emitter.Front = front;
}

/****************************************************************************
*                       SetEmitterUp
*************************************************************************//**
*  @fn        void AudioSource3D::SetEmitterUp(const DirectX::XMFLOAT3& up)
*  @brief     Set Emitter side up vector
*  @param[in] const DirectX::XMFLOAT3& up
*  @return 　　void
*****************************************************************************/
void AudioSource3D::SetEmitterUp(const Float3& up)
{
	this->_emitter.Up = up;
}

/****************************************************************************
*                       SetEmitterPosition
*************************************************************************//**
*  @fn        void AudioSource3D::SetEmitterPosition(const DirectX::XMFLOAT3& position)
*  @brief     Set Emitter side position
*  @param[in] const DirectX::XMFLOAT3& position
*  @return 　　void
*****************************************************************************/
void AudioSource3D::SetEmitterPosition(const Float3& position)
{
	this->_emitter.Position = position;
}

/****************************************************************************
*                       SetEmitterVelocity
*************************************************************************//**
*  @fn        void AudioSource3D::SetEmitterVelocity(const DirectX::XMFLOAT3& velocity)
*  @brief     Set Emitter side velocity
*  @param[in] const DirectX::XMFLOAT3& velocity
*  @return 　　void
*****************************************************************************/
void AudioSource3D::SetEmitterVelocity(const Float3& velocity)
{
	this->_emitter.Velocity = velocity;
}

/****************************************************************************
*                       SetEmitterDopplerLevel
*************************************************************************//**
*  @fn        void AudioSource3D::SetEmitterDopplerLevel(float level)
*  @brief     Set Emitter side doppler level
*  @param[in] float level
*  @return 　　void
*****************************************************************************/
void AudioSource3D::SetEmitterDopplerLevel(float level)
{
	this->_emitter.DopplerScaler = level;
}

/****************************************************************************
*                       SetEmitterRadius
*************************************************************************//**
*  @fn        void AudioSource3D::SetEmitterRadius(const DirectX::XMFLOAT3& radius)
*  @brief     Defining the range of sound
*  @param[in] float radius
*  @return 　　void
*****************************************************************************/
void AudioSource3D::SetEmitterRadius(float radius)
{
	this->_emitter.InnerRadius = radius;
}
#pragma endregion Property
#pragma endregion Public Function