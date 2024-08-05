//////////////////////////////////////////////////////////////////////////////////
///             @file   AudioFader.cpp
///             @brief  Fade in Fade out
///             @author Toide Yutaro
///             @date   2022_12_22
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/AudioFader.hpp"
#define XAUDIO2_HELPER_FUNCTIONS
#include <xaudio2.h>
#include <assert.h>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gc::audio;

//////////////////////////////////////////////////////////////////////////////////
//                              Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor
AudioFader::AudioFader(const float startVolume, const float targetVolume, const float targetTime)
	: _startVolume(startVolume), _targetVolume(targetVolume), _targetTime(targetTime)
{
	assert(_targetVolume >= 0);
	assert(_targetTime   >= 0);
	assert(_startVolume  >= 0);

	_state = FadeState::Started;
}
#pragma endregion Constructor and Destructor

#pragma region Main Function
/****************************************************************************
*                       Update
****************************************************************************/
*  @fn        float AudioFader::Update(const float deltaTime)
*
*  @brief     Update volume. Return current volume.
*
*  @param[in] const float deltaTime (in seconds)
*
*  @return Å@Å@void
*****************************************************************************/
void AudioFader::Update(const float deltaTime)
{
	/*-------------------------------------------------------------------
	-              Start or Completed fade check.
	---------------------------------------------------------------------*/
	if (_state == FadeState::None     ) { return; }
	if (_state == FadeState::Completed) { return; }

	/*-------------------------------------------------------------------
	-              Update current time
	---------------------------------------------------------------------*/
	assert(deltaTime >= 0);

	_currentTime += deltaTime;

	/*-------------------------------------------------------------------
	-              Manage fader state
	---------------------------------------------------------------------*/
	if (_currentTime >= _targetTime) 
	{
		_state = FadeState::Completed;
		_currentVolume =  _targetVolume; 
	}
	else
	{
		_state = FadeState::Fading;
	}

	/*-------------------------------------------------------------------
	-              Calculate x^2 curve (Ç–Ç∆Ç‹Ç∏)
	---------------------------------------------------------------------*/
	const auto timeRatio = (_targetTime - _currentTime) / _targetTime; // 0.0f Å` 1.0f
	const auto timeRatio2 = timeRatio * timeRatio;

	_currentVolume = _targetVolume * timeRatio2 + (1.0f - timeRatio2) * _startVolume;
}
#pragma endregion Main Function