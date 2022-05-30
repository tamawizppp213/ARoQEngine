//////////////////////////////////////////////////////////////////////////////////
//              @file   OBJParser.cpp
///             @brief  OBJ File Parser
///             @author Toide Yutaro
///             @date   2022_05_07
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Rendering/Model/External/OBJ/Include/OBJParser.hpp"
#include "GameUtility/File/Include/FileSystem.hpp"
#include "GameUtility/File/Include/UnicodeUtility.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace obj;
//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
bool OBJFile::Load(const std::wstring& filePath)
{
	/*-------------------------------------------------------------------
	-             Open File
	---------------------------------------------------------------------*/
	std::ifstream stream(filePath);
	if (!stream.is_open())
	{
		std::cerr << "The file cannot be read," << std::endl;
	}

	Directory = file::FileSystem::GetDirectory(unicode::ToUtf8String(filePath));

	/*-------------------------------------------------------------------
	-             Prepare the variables
	---------------------------------------------------------------------*/
	std::vector<Float3> positions;
	std::vector<Float3> normals;
	std::vector<Float2> uvs;
	std::vector<Face>   faces;
	std::string         materialName = "";

	/*-------------------------------------------------------------------
	-           The OBJ format does not have a data sequence rule,
	-           so once all the information is read from the file
	---------------------------------------------------------------------*/
	while (!stream.eof())
	{
		std::string line;
		std::getline(stream, line);
		/*-------------------------------------------------------------------
		-          Reduce the load of string-to-number conversion.
		---------------------------------------------------------------------*/
		std::stringstream stringStream(line);
		std::string prefix;
		stringStream >> prefix; // Separated by whitespace so prefix can be obtained.

		/*-------------------------------------------------------------------
		-          Perform loading for each prefix.
		---------------------------------------------------------------------*/
		/*-------------------------------------------------------------------
		-          Material
		---------------------------------------------------------------------*/
		if (prefix == "") { continue; }

		char firstChar = prefix[0];
		switch (firstChar)
		{
			case 'v':
			{
				if (prefix[1] == 'n') 
				{ 
					Float3 temp; 
					stringStream << temp.x << temp.y << temp.z;
					normals.push_back(temp);
				}
				else if (prefix[1] == 't')
				{
					Float2 temp;
					stringStream << temp.x << temp.y;
					uvs.push_back(temp);
				}
				else
				{
					Float3 temp;
					stringStream << temp.x << temp.y << temp.z;
					positions.push_back(temp);
				}
				break;
			}
			case 'f':
			{
				Face face = { {-1,-1,-1}, {-1,-1,-1}, {-1,-1,-1} };
				face.MaterialName = materialName;
				for (int i = 0; i < 3; ++i) // 4頂点1ポリゴンの場合は定義してないので注意
				{
					std::string text;
					stringStream >> text;
					std::stringstream fileStream(text);

					// Retrieves the vertex and normal vector numbers that make up a face.
					// vertex
					{
						std::string vTemp;
						std::getline(fileStream, vTemp, '/');
						face.PositionID[i] = std::stoi(vTemp) - 1;
					}
					// uv
					{
						std::string uvTemp;
						std::getline(fileStream, uvTemp, '/');
						if (uvTemp != "")
						{
							face.TextureID[i] = std::stoi(uvTemp) - 1;
						}

					}
					// normal
					{
						std::string nTemp;
						std::getline(fileStream, nTemp, '/');
						if (nTemp != "")
						{
							face.NormalID[i] = std::stoi(nTemp) - 1;
						}
					}
				}

				faces.push_back(face);
				break;
			}
			case 'u':
			{
				if (prefix == "usemtl")
				{
					stringStream >> materialName;
				}
				break;
			}
			case 'm':
			{
				if (prefix == "mtllib")
				{
					std::string name;
					stringStream >> name;
					LoadMaterial(Directory + name);
				}
				break;
			}
			case 'l':
			{
				break;
			}
			case '#':
			{
				break;
			}
		}

		/*-------------------------------------------------------------------
		-       Create the final vertex array from the read face information.
		---------------------------------------------------------------------*/
		std::map<std::string, UINT32> vertexID;
		for (const auto& face : faces)
		{
			bool hasTexture = face.TextureID[0] >= 0 ? true : false;
			bool hasNormal  = face.NormalID [0] >= 0 ? true : false;

			for (int i = 0; i < 3; ++i)
			{
				/*-------------------------------------------------------------------
				-          Prepare to determine if there is a duplicate.
				---------------------------------------------------------------------*/
				std::string key ="";
				std::ostringstream stringOut;
				               { stringOut << std::setfill('0') << std::setw(5) << face.PositionID[i]; key += stringOut.str(); }
				if (hasTexture){ stringOut << std::setfill('0') << std::setw(5) << face.TextureID[i]; key += stringOut.str();}
				if (hasNormal) { stringOut << std::setfill('0') << std::setw(5) << face.NormalID[i]; key += stringOut.str();}
				

				/*-------------------------------------------------------------------
				-          Check duplicate
				---------------------------------------------------------------------*/
				if (vertexID.count(key) > 0)
				{
					Indices[materialName].push_back(vertexID[key]);
				}
				else
				{
					// add new vertex
					OBJVertex vertex;
					vertex.Position = positions[face.PositionID[i]];
					vertex.Normal   = hasNormal ? normals[face.NormalID[i]] : Float3(0.0f, 0.0f, 0.0f);
					vertex.Color    = Float4(1.0f, 1.0f, 1.0f, 1.0f);
					vertex.UV       = hasTexture ? uvs[face.TextureID[i]] : Float2(0.0f, 0.0f);
					Vertices.push_back(vertex);

					Indices[materialName].push_back(static_cast<UINT32>(Vertices.size() - 1));
					vertexID[key]   = static_cast<UINT32>(Vertices.size() - 1);
				}
			}
		}
	}


	return true;
}

void OBJFile::LoadMaterial(const std::string& filePath)
{
	/*-------------------------------------------------------------------
	-             Open Material File
	---------------------------------------------------------------------*/
	std::ifstream stream(filePath);
	if (!stream.is_open())
	{
		std::cerr << "The file cannot be read," << std::endl;
	}

	UINT32 index = 0;
	while (!stream.eof())
	{
		std::string line;
		std::getline(stream, line);
		/*-------------------------------------------------------------------
		-          Reduce the load of string-to-number conversion.
		---------------------------------------------------------------------*/
		std::stringstream stringStream(line);
		std::string prefix;
		stringStream >> prefix; // Separated by whitespace so prefix can be obtained.
		if (prefix == "") { continue; }

		// More emphasis on clarity than a list of "if". 
		char firstChar = prefix[0];
		switch (firstChar)
		{
			/*-------------------------------------------------------------------
			-          Create new material
			---------------------------------------------------------------------*/
			case 'n':
			{
				if (prefix == "newmtl")
				{
					Materials.push_back(OBJMaterial());
					index = (UINT32)Materials.size() - 1;
					stringStream >> Materials[index].Name;
				}
				else if (prefix == "norm")
				{
					stringStream >> Materials[index].NormalMapName;
				}
				break;
			}
			/*-------------------------------------------------------------------
			-          Set material infomation
			---------------------------------------------------------------------*/
			case 'K':
			{
				if      (prefix[1] == 'd') { stringStream >> Materials[index].Diffuse.x >> Materials[index].Diffuse.y >> Materials[index].Diffuse.z; }
				else if (prefix[1] == 'a') { stringStream >> Materials[index].Ambient.x >> Materials[index].Ambient.y >> Materials[index].Ambient.z; }
				else if (prefix[1] == 's') { stringStream >> Materials[index].Specular.x >> Materials[index].Specular.y >> Materials[index].Specular.z; }
				else if (prefix[1] == 'e') { stringStream >> Materials[index].Emissive; }
				break;
			}
			/*-------------------------------------------------------------------
			-          Set material coeffecient
			---------------------------------------------------------------------*/
			case 'N':
			{
				if      (prefix[1] == 'i') { stringStream >> Materials[index].RefractiveIndex; }
				else if (prefix[1] == 's') { stringStream >> Materials[index].SpecularPower; }
				break;
			}
			/*-------------------------------------------------------------------
			-          Set Diffuse Alpha
			---------------------------------------------------------------------*/
			case 'd':
			{
				if      (prefix == "disp" ) { stringStream >> Materials[index].DisplacementMapName; }
				else if (prefix == "decal") { stringStream >> Materials[index].DecalMapName; }
				else                        { stringStream >> Materials[index].Alpha; }
				break;
			}
			/*-------------------------------------------------------------------
			-          Set Trace (Alpha)
			---------------------------------------------------------------------*/
			case 'T':
			{
				float tr = 0.0f;
				stringStream >> tr;
				Materials[index].Alpha = 1.0f - tr;
				break;
			}
			/*-------------------------------------------------------------------
			-          Set Physics Parameter
			---------------------------------------------------------------------*/
			case 'P':
			{
				if      (prefix[1] == 'r') { stringStream >> Materials[index].Roughness; }
				else if (prefix[1] == 'm') { stringStream >> Materials[index].Metalic; }
				else if (prefix[1] == 's') { stringStream >> Materials[index].Sheen.x >> Materials[index].Sheen.y >> Materials[index].Sheen.z; }
				else if (prefix == "Pcr")  { stringStream >> Materials[index].ClearCoatRoughness; }
				else if (prefix[1] == 'c') { stringStream >> Materials[index].ClearCoatThickness; }
				break;
			}
			case 'a':
			{
				if      (prefix == "anisor") { stringStream >> Materials[index].AnisotropyRotation; }
				else if (prefix == "aniso")  { stringStream >> Materials[index].Anisotropy;}
				break;
			}
			case 'i':
			{
				if (prefix == "illum")       { stringStream >> Materials[index].IlluminateType;}
				break;
			}
			case 'm':
			{
				if      (prefix == "map_Kd")   { stringStream >> Materials[index].DiffuseMapName; }
				else if (prefix == "map_Ks")   { stringStream >> Materials[index].SpecularMapName;}
				else if (prefix == "map_Ka")   { stringStream >> Materials[index].AmbientMapName; }
				else if (prefix == "map_Ke")   { stringStream >> Materials[index].EmissiveMapName; }
				else if (prefix == "map_Ns")   { stringStream >> Materials[index].SpecularPowerMapName; }
				else if (prefix == "map_d" )   { stringStream >> Materials[index].TransparentMapName; }
				else if (prefix == "map_Pr")   { stringStream >> Materials[index].RoughnessMapName; }
				else if (prefix == "map_Pm")   { stringStream >> Materials[index].MetalicMapName; }
				else if (prefix == "map_Ps")   { stringStream >> Materials[index].SheenMapName; }
				else if (prefix == "map_RMA")  { stringStream >> Materials[index].RMAMapName; }
				else if (prefix == "map_bump") { stringStream >> Materials[index].BumpMapName; }
				break;
			}
			case '#': { break; }
			// 非標準
			case 'b':
			{
				if (prefix == "bump") { stringStream >> Materials[index].BumpMapName; }
			}
		}
	}
}