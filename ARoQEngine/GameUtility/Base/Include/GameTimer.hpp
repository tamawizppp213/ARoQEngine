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

//////////////////////////////////////////////////////////////////////////////////
//                         GameTimer Class
//////////////////////////////////////////////////////////////////////////////////
class GameTimer
{
public:
	/****************************************************************************
	**                Public Function
	*****************************************************************************/
	/* @brief : Return the elapsed time up to the current time in seconds. */
	float TotalTime() const;

	/* @brief : Return deltaTime in seconds. */
	float DeltaTime() const;

	/* @brief : Reset game timer. Call before message loop*/
	void Reset();

	/* @brief : Start or Restart game timer. If stopped before, the pause time is measured.*/
	void Start();

	/* @brief : Stop timer*/
	void Stop();

	/* @brief : Record deltaTime and previous frame. This function needs to be called every frame*/
	void Tick();

	/* @brief : Show sverage fps at window*/
	void AverageFrame(const void* hwnd);

	/****************************************************************************
	**                Public Property
	*****************************************************************************/

	/****************************************************************************
	**                Constructor and Destructor
	*****************************************************************************/
	GameTimer();
	~GameTimer() = default;
private:
	/****************************************************************************
	**                Private Function
	*****************************************************************************/

	/****************************************************************************
	**                Private Property
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