//////////////////////////////////////////////////////////////////////////////////
//              Title:  GameTimer.hpp
//            Content:  Time Measurement Function 
//             Author:  Toide Yutaro (Reference: 3DGame Programming with DirectX12)
//             Create:  2020_11_08
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GAME_TIMER_HPP
#define GAME_TIMER_HPP
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
struct HWND__;
using HWND = HWND__*;

//////////////////////////////////////////////////////////////////////////////////
//                         GameTimer Class
//////////////////////////////////////////////////////////////////////////////////
class GameTimer
{
public:
	/****************************************************************************
	**                Public Function
	*****************************************************************************/
	GameTimer();
	~GameTimer() = default;

	float TotalTime() const; // in seconds
	float DeltaTime() const; // in seconds

	void Reset(); // Call before message loop
	void Start(); // Call when unpaused
	void Stop();  // Call when paused
	void Tick();  // Call every frame

	void AverageFrame(const HWND& hwnd);
	/****************************************************************************
	**                Public Member Variables
	*****************************************************************************/

private:
	/****************************************************************************
	**                Private Function
	*****************************************************************************/

	/****************************************************************************
	**                Private Member Variables
	*****************************************************************************/
	double _secondsPerCount;
	double _deltaTime;

	__int64 _baseTime;
	__int64 _pausedTime;
	__int64 _stopTime;
	__int64 _previousTime;
	__int64 _currentTime;

	bool _stopped;
};
#endif