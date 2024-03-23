//////////////////////////////////////////////////////////////////////////////////
//              @file   IESProfiler.cpp
///             @brief  IES Profile parser
///             @author Copyright(c) Pocol. All right reserved.
///                     Partially edit by Toide Reference : DirectX12 é¿ëHÉKÉCÉh 
///             @date   2023_03_10
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/IESProfiler.hpp"
#include "GameUtility/Math/Include/GMMath.hpp"
#include <iostream>
#include <fstream>

//////////////////////////////////////////////////////////////////////////////////
//                             Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gc::rendering;

//////////////////////////////////////////////////////////////////////////////////
//                             Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor
IESProfiler::~IESProfiler()
{
	
}
#pragma endregion Constructor and Destructor

#pragma region Main Function
void IESProfiler::Load(const std::wstring& name)
{
	LoadLamp(name);
	SetUpTexture();
}

void IESProfiler::LoadLamp(const std::wstring& name)
{
	/*-------------------------------------------------------------------
	-			Open file
	---------------------------------------------------------------------*/
	std::ifstream stream;
	stream.open(name, std::ios::in);

	if (!stream.is_open())
	{
		throw std::runtime_error("failed to open IES profile filepath");
	}

	/*-------------------------------------------------------------------
	-			Load format
	---------------------------------------------------------------------*/
	std::string format = "";
	stream >> format;
	if (!IsProperIESFormat(format))
	{
		throw std::runtime_error("unknown IES format type");
	}

	/*-------------------------------------------------------------------
	-			Load light source info
	---------------------------------------------------------------------*/
	while (true)
	{
		// end of stream check
		if (!stream)      { break; }
		if (stream.eof()) { break; }
		
		// Skip this file until founding information on the light source 
		// Additional information is ignored.
		std::string tiltInfo = "";
		stream >> tiltInfo;
		if (tiltInfo != "TILT=NONE") { continue; } // É`ÉãÉgäpÇÕí èÌéÊÇËàµÇÌÇ»Ç¢Ç∆JISãKäiÇ…Ç®Ç¢ÇƒÇÕéwíËÇ≥ÇÍÇƒÇ¢ÇÈ.

		/*-------------------------------------------------------------------
		-			Load lamp count 
		---------------------------------------------------------------------*/
		int lampCount = 0;
		stream >> lampCount;
		
		// Currently, only load the lamp count 1.
		if (lampCount != 1)
		{
			throw std::runtime_error("You can only load the lamp count 1");
		}

		/*-------------------------------------------------------------------
		-			Load Lumen and Angle count
		---------------------------------------------------------------------*/
		int angleCountVertical = 0, angleCountHorizontal = 0, futureUse = 0;

		stream >> _lamp.Lumen;             // lumen 
		stream >> _lamp.Multiplier;        // type of luminosity
		stream >> angleCountVertical;      // vertical angle count
		stream >> angleCountHorizontal;    // horizontal angle count

		_lamp.AngleVertical  .resize(angleCountVertical);
		_lamp.AngleHorizontal.resize(angleCountHorizontal);
		_lamp.Candera        .resize(angleCountVertical * angleCountHorizontal);

		/*-------------------------------------------------------------------
		-			Load Lamp config
		---------------------------------------------------------------------*/
		int photometricType, unitType;
		stream >> photometricType;
		stream >> unitType;
		stream >> _lamp.ShapeWidth;
		stream >> _lamp.ShapeLength;
		stream >> _lamp.ShapeHeight;
		stream >> _lamp.BallastFactor;
		stream >> futureUse;
		stream >> _lamp.InputWatts;
		_lamp.ShapeWidth      = fabs(_lamp.ShapeWidth);   // ïâÇ…Ç»Ç¡ÇƒÇ¢ÇÈÇ±Ç∆Ç™Ç†ÇÈÇÃÇ≈ê‚ëŒílÇ≈. 
		_lamp.ShapeLength     = fabs(_lamp.ShapeLength);
		_lamp.ShapeHeight     = fabs(_lamp.ShapeHeight);
		_lamp.PhotometricType = static_cast<PlaneType>(photometricType);
		_lamp.LampUnitType    = static_cast<UnitType> (unitType);

		// skip new line.
		stream.ignore(2048, '\n');

		/*-------------------------------------------------------------------
		-			Load Vertical angle
		---------------------------------------------------------------------*/
		for (int i = 0; i < angleCountVertical; ++i)
		{
			stream >> _lamp.AngleVertical[i];
		}

		/*-------------------------------------------------------------------
		-			Load Horizontal angle
		---------------------------------------------------------------------*/
		for (int i = 0; i < angleCountHorizontal; ++i)
		{
			stream >> _lamp.AngleHorizontal[i];
		}

		/*-------------------------------------------------------------------
		-			Load Candera (row: vertical data, column: horizontal data)
		---------------------------------------------------------------------*/
		_lamp.MaxCandera = 0.0f;
		float value = 0.0f;
		for (int i = 0; i < angleCountHorizontal; ++i)
		{
			for (int j = 0; j < angleCountVertical; ++j)
			{
				stream >> value;
				const auto candera = value * _lamp.Multiplier;
				_lamp.Candera[i * angleCountHorizontal + j] = candera;
				_lamp.MaxCandera = _lamp.MaxCandera > candera ? _lamp.MaxCandera : candera;
			}
		}

		// skip new line.
		stream.ignore(2048, '\n');
	}

	stream.close();
}

void IESProfiler::SetUpTexture()
{
	
}

bool IESProfiler::IsProperIESFormat(const std::string& formatName)
{
	if (formatName == "IESNA:LM-63-2002") { _version = IESVersion::IES_2002; return true; }
	if (formatName == "IESNA:LM-63-1995") { _version = IESVersion::IES_1995; return true; }
	if (formatName == "IESNA91")          { _version = IESVersion::IES_1991; return true; }
	return false;
}
#pragma endregion Main Function
