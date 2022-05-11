//////////////////////////////////////////////////////////////////////////////////
///             @file   OBJParser.hpp
///             @brief  OBJ File Parser ! 読み込みの高速化が必須　(200kB程度のオブジェクトでさえ数分かかる)
///             @author Toide Yutaro
///             @date   2022_05_07
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef OBJ_PARSER_HPP
#define OBJ_PARSER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Math/Include/GMVector.hpp"
#include <Windows.h>
#include <vector>
#include <string>
#include <map>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace obj
{
	using namespace gm;
	/*-------------------------------------------------------------------
	-          Face
	---------------------------------------------------------------------*/
	struct OBJVertex
	{
		Float3  Position;
		Float3  Normal;
		Float2  UV;
		Float4  Color;
	};
	/*-------------------------------------------------------------------
	-          Face
	---------------------------------------------------------------------*/
	struct Face
	{
		int PositionID[3];
		int TextureID[3];
		int NormalID[3];
		std::string MaterialName;
	};

	/*-------------------------------------------------------------------
	-          Texture map
	---------------------------------------------------------------------*/
	enum TextureMapFlag
	{
		None = 0x0000,
		Diffuse  = 0x0001,
		Ambient  = 0x0002,
		Emissive = 0x0004,
		Bump     = 0x0008
	};
	struct OBJMaterial
	{
		std::string Name;
		Float3 Diffuse;
		float  Alpha;
		Float3 Specular;
		float  SpecularPower;
		Float3 Ambient;
		float  Emissive;
		float  RefractiveIndex;
		int    IlluminateType;
		float  Roughness;
		float  Metalic;
		Float3 Sheen;
		float  ClearCoatThickness;
		float  ClearCoatRoughness;
		float  Anisotropy;
		float  AnisotropyRotation;
		
		std::string DiffuseMapName;
		std::string SpecularMapName;
		std::string AmbientMapName;
		std::string EmissiveMapName;
		std::string BumpMapName;
		std::string NormalMapName;
		std::string TransparentMapName;
		//std::string ReflectionMapName[6];
		std::string SpecularPowerMapName;
		std::string OpacityMapName;
		std::string DisplacementMapName;
		std::string DecalMapName;
		std::string RoughnessMapName;
		std::string MetalicMapName;
		std::string SheenMapName;
		std::string RMAMapName;
	};
	/****************************************************************************
	*				  			VMDFile
	*************************************************************************//**
	*  @class     VMDFile
	*  @brief     VMDFile
	*****************************************************************************/
	class OBJFile
	{
		using HashID = size_t;
	public:
		
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		bool Load(const std::wstring& filePath);
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		std::string ModelName;
		std::string Directory;
		std::vector<OBJVertex>   Vertices;
		std::map<std::string, std::vector<UINT32>> Indices;
		std::vector<OBJMaterial> Materials;
		
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		OBJFile() = default;
		~OBJFile() {};
		OBJFile(const OBJFile&)            = delete;
		OBJFile& operator=(const OBJFile&) = delete;
		OBJFile(OBJFile&&)                 = default;
		OBJFile& operator=(OBJFile&&)      = default;
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		void LoadMaterial(const std::string& filePath);

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
	private:
		/****************************************************************************
		**                Private Member Variables
		*****************************************************************************/

	};
}
#endif