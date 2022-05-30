//////////////////////////////////////////////////////////////////////////////////
///             @file   IESProfiler.hpp
///             @brief  IES Profile parser
///             @author Toide Yutaro
///             @date   2022_05_10
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef IES_PROFILER_HPP
#define IES_PROFILER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
enum class PlaneType
{
	C = 1, // theta , phi coodinate
	B = 2, // alpha , beta coodinate
	A = 3, // xy coodinate
};
enum class UnitType
{
	Feet = 1,
	Meter = 2,
};
struct Lamp
{
	float     Lumen;
	float     Multiplier;
	PlaneType PhotometricType;
	UnitType  LampUnitType;
	float     ShapeWidth;
	float     ShapeLength;
	float     ShapeHeight;
	float     BallastFactor;
	float     InputWatts;

};
#endif