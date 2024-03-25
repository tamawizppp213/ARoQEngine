//////////////////////////////////////////////////////////////////////////////////
///             @file   IESProperty.hpp
///             @brief  IES Profile property
///                     https://ieslibrary.com/en/home (IES light library)
///                     https://vdocuments.mx/ies-specification-lm-63-2002.html?page=3
///             @author Toide Yutaro
///             @date   2023_03_10
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef IES_PROPERTY_HPP
#define IES_PROPERTY_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include <vector>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace gc::rendering
{
	/****************************************************************************
	*				  			IESVersion
	*************************************************************************//**
	*  @enum      IESVersion
	*
	*  @brief     IES profiler version
	*****************************************************************************/
	enum class IESVersion
	{
		IES_1986, // IES LM-63-1986 (使用しません.)
		IES_1991, // IES LM-63-1991
		IES_1995, // IES LM-63-1995
		IES_2002  // IES LM-63-2002
	};

	/****************************************************************************
	*				  			PlaneType
	*************************************************************************//**
	*  @enum      PlaneType
	*
	*  @brief     Coordinate system for light distribution measurement.
	*****************************************************************************/
	enum class PlaneType
	{
		C = 1, // Θ and φ coodinate. This type is used to architectual lights and road lights
		B = 2, // α and β coodinate. This type is used to adjustable outdoor area, sport lighting luminaries.
		A = 3, // x and y coodinate. This type is used to automobile headlights and signal lights.
	};

	/****************************************************************************
	*				  			UnitType
	*************************************************************************//**
	*  @enum      UnitType
	*
	*  @brief     Feet or meter.
	*****************************************************************************/
	enum class UnitType
	{
		Feet  = 1,
		Meter = 2,
	};

	/****************************************************************************
	*				  			Lamp
	*************************************************************************//**
	*  @struct      Lamp
	*
	*  @brief       This struct is used for photometric light.
	*****************************************************************************/
	struct Lamp
	{
	public:
		float              Lumen;           // Lumen: Total luminous flux value per light source. (basically light source count 1.) 
		float              Multiplier;      // Type of luminosity. Absolute luminous intensity is set 1. Relative luminous intensity per 1000 [lm], product of the number of light sources and the luminous flux value is divided by 1000, 
		PlaneType          PhotometricType; // Measurement coordinates.
		UnitType           LampUnitType;    // Unit type : meter or feet. If you wanna to convert unit type, you call ConvertUnitType function. 
		float              ShapeWidth;      // light souce width (always ? 0)
		float              ShapeLength;     // light source length (奥行) (always > 0)
		float              ShapeHeight;     // light source height        (always > 0)
		float              BallastFactor;   // Ballast Output Coefficient. (供試安定器の光出力 / 試験用安定器の光出力)
		float              InputWatts;      // 定格消費電力 (W)
		std::vector<float> AngleVertical   = {}; // Vertical Angle data (A or B plane: -90° or 0° -> 90°, C plane: 0° or 90° -> 90° or 180°)
		std::vector<float> AngleHorizontal = {}; // Horizontal Angle data (0° -> 0°(全ての平面に対して横方向に対称), 90°(四分儀において対称), 180°(0～180で平面に関して対称), 360°(対称性なし))
		std::vector<float> Candera         = {}; // Candera value
		float MaxCandera;

		~Lamp()
		{
			AngleVertical  .clear(); AngleVertical  .shrink_to_fit();
			AngleHorizontal.clear(); AngleHorizontal.shrink_to_fit();
			Candera.clear(); Candera.shrink_to_fit();
		}

		void ConvertUnitType(const UnitType type)
		{
			if (type == LampUnitType) { return; }

			if (type == UnitType::Feet)
			{
				const float feet = 1.0f / 0.3048f;
				LampUnitType = type;
				ShapeWidth   = feet * ShapeWidth;
				ShapeLength  = feet * ShapeLength;
				ShapeHeight  = feet * ShapeHeight;
			}
			if (type == UnitType::Meter)
			{
				const float meter = 0.3048f; // [ft -> m]
				LampUnitType = type;
				ShapeWidth   = meter * ShapeWidth;
				ShapeLength  = meter * ShapeLength;
				ShapeHeight  = meter * ShapeHeight;
			}
		}
	};
}
#endif