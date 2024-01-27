//////////////////////////////////////////////////////////////////////////////////
///             @file   PMDParser.hpp
///             @brief  PMD File Parser
///             @author Toide Yutaro
///             @date   2022_05_05
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef PMD_PARSER_HPP
#define PMD_PARSER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Math/Include/GMVector.hpp"
#include "GameUtility/Base/Include/GUString.hpp"
#include <Windows.h>
#include <vector>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace pmd
{
	using namespace gm;

	struct PMDHeader
	{
		char  Signature[3];
		float Version;
		gu::string ModelName;
		gu::string ModelEnglishName;
		gu::string ModelComment;
		gu::string ModelEnglishComment;

		void Read(FILE* filePtr);
		void ReadExtension(FILE* filePtr);
	};

	struct PMDVertex
	{
		Float3 Position;
		Float3 Normal;
		Float2 UV;
		UINT16 BoneIndex[2];
		UINT8  BoneWeight;
		UINT8  EdgeInvisible;

		void Read(FILE* filePtr);
	};

	struct PMDMaterial
	{
		Float4      Diffuse;
		float       SpecularPower;
		Float3      Specular;
		Float3      Ambient;
		UINT8       ToonID;
		UINT8       EdgeFlag;
		UINT32      IndexCount;
		gu::string TextureFileName;
		gu::string SphereFileName;

		void Read(FILE* filePtr, const gu::string& directory);

	private:
		void ReadTextureName(const gu::string& directory, const gu::string& texture);
	};

	struct PMDBone
	{
		gu::string BoneName;
		UINT16      ParentBoneID; // If there is no Parent, this value needs to set 0xFFFF
		UINT16      ChildBoneID;
		UINT8       BoneType;
		UINT16      IKBoneID;     // If there is no Parent, this value needs to set 0;
		Float3      BoneHeadPosition;
		gu::string EnglishBoneName;

		void Read(FILE* filePtr);
		void ReadExtension(FILE* filePtr);
	};

	struct PMDBoneIK
	{
		using ChainList = std::vector<UINT16>;

		UINT16    IKBoneID;
		UINT16    IKTargetBoneID;
		UINT8     IKChainLength;
		UINT16    IterationCount;
		float     AngleLimit;
		ChainList Chains;

		void Read(FILE* filePtr);

		~PMDBoneIK() { Chains.clear(); Chains.shrink_to_fit(); }
	};

	enum class FacePart : UINT8
	{
		Base,
		EyeBrow,
		Eye,
		Rip,
		Other
	};

	struct PMDFaceExpression
	{
		using FaceVertexList = std::vector<Float3>;
		using FaceIndexList = std::vector<UINT32>;

		gu::string    FaceExpressionName;
		UINT32         VertexNum;
		FacePart       FaceExpressionType;
		FaceVertexList Vertices;
		FaceIndexList  Indices;
		gu::string    FaceExpressionEnglishName;

		void Read(FILE* filePtr);
		void ReadExtension(FILE* filePtr);

		~PMDFaceExpression()
		{
			Vertices.clear(); Vertices.shrink_to_fit();
			Indices.clear(); Indices.shrink_to_fit();
		}
	};

	struct PMDBoneDisplayName
	{
		gu::string BoneDisplayName;
		gu::string BoneDisplayEnglishName;
		void Read(FILE* filePtr);
		void ReadExtension(FILE* filePtr);
	};
	struct PMDBoneDisplay
	{
		UINT16 BoneIndex;
		UINT8  BoneDisplayIndex;
		void Read(FILE* filePtr);
	};
	struct PMDRigidBody
	{
		enum class PMDRigidBodyShape : UINT8
		{
			Sphere,
			Box,
			Capsule
		};

		enum class PMDRigidBodyCalcType : UINT8
		{
			Static,
			Dynamic,
			DynamicAdjustBone
		};

		gu::string           RigidBodyName;
		UINT16                RelationBoneIndex;
		UINT8                 GroupIndex;
		UINT16                GroupTarget;
		PMDRigidBodyShape     RigidBodyShapeType;
		Float3                BodyShape;
		Float3                Position;
		Float3                Rotation;
		float                 Mass;
		float                 DampingTranslate;
		float                 DampingRotation;
		float                 Elasticity;
		float                 Friction;
		PMDRigidBodyCalcType  RigidBodyCalcType;

		void Read(FILE* filePtr);
	};

	struct PMDJoint
	{
		gu::string JointName;
		UINT32      RigidBodyA;
		UINT32      RigidBodyB;
		Float3      JointTranslation;
		Float3      JointRotation;
		Float3      TranslationMin;
		Float3      TranslationMax;
		Float3      RotationMin; 
		Float3      RotationMax;
		Float3      SpringTranslationFactor;
		Float3      SpringRotationFactor;

		void Read(FILE* filePtr);
	};

	/****************************************************************************
	*				  			PMDFile
	*************************************************************************//**
	*  @class     PMDFile
	*  @brief     PMD file
	*****************************************************************************/
	class PMDFile
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		bool Load(const gu::wstring& filePath);
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		PMDHeader   Header;
		std::vector<PMDVertex>          Vertices;
		std::vector<UINT16>             Indices;
		std::vector<PMDMaterial>        Materials;
		std::vector<PMDBone>            Bones;
		std::vector<PMDBoneIK>          BoneIKs;
		std::vector<PMDFaceExpression>  FaceExpressions;
		std::vector<UINT16>             FaceLabelIndices;
		std::vector<PMDBoneDisplayName> BoneDisplayNameList;
		std::vector<PMDBoneDisplay>     BoneDisplayList;
		std::vector<PMDRigidBody>       RigidBodies;
		std::vector<PMDJoint>           Joints;
		std::vector<gu::string>        ToonTextureList;
		gu::string Directory;

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		PMDFile() = default;
		~PMDFile();
		PMDFile(const PMDFile&)            = delete;
		PMDFile& operator=(const PMDFile&) = delete;
		PMDFile(PMDFile&&)                 = default;
		PMDFile& operator=(PMDFile&&)      = default;
	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/
		void ReadVertices           (FILE* filePtr);
		void ReadIndices            (FILE* filePtr);
		void ReadMaterials          (FILE* filePtr);
		void ReadBones              (FILE* filePtr);
		void ReadBoneIKs            (FILE* filePtr);
		void ReadFaceExpressions    (FILE* filePtr);
		void ReadBoneDisplayNameList(FILE* filePtr);
		void ReadBoneDisplayList    (FILE* filePtr);
		void ReadLocalizeData       (FILE* filePtr);
		void ReadToonTextures       (FILE* filePtr);
		void ReadPhysics            (FILE* filePtr);
		
		/****************************************************************************
		**                Private Member Variables
		*****************************************************************************/
	};

}
#endif