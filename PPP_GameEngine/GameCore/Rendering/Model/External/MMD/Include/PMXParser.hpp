//////////////////////////////////////////////////////////////////////////////////
///             @file   PMXParser.hpp
///             @brief  PMXParser
///             @author Toide Yutaro
///             @date   2022_05_05
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef PMX_PARSER_HPP
#define PMX_PARSER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Math/Include/GMVector.hpp"
#include <Windows.h>
#include <vector>
#include <string>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Class
//////////////////////////////////////////////////////////////////////////////////
namespace pmx
{
	using namespace gm;

	enum class PMXEncode : UINT8
	{
		UTF16 = 0,
		UTF8 = 1
	};

	/****************************************************************************
	*				  			PMXHeader
	*************************************************************************//**
	*  @class     PMXHeader
	*  @brief     Define Index Size
	*****************************************************************************/
	struct PMXSetting
	{
		char       Signature[4];
		float      Version;            // ver 2.0 / 2.1
		UINT8      DataSize;
		PMXEncode  Encode;             // 0: UTF16, 1: UTF8
		UINT8      AddUVCount;         // 0 ~ 4
		UINT8      VertexIndexSize;    // 1, 2, or 4
		UINT8      TextureIndexSize;   // 1, 2, or 4
		UINT8      MaterialIndexSize;  // 1, 2, or 4
		UINT8      BoneIndexSize;      // 1, 2, or 4
		UINT8      FaceIndexSize;      // 1, 2, or 4
		UINT8      RigidBodyIndexSize; // 1, 2, or 4

		void Read(FILE* file);
	};
	/****************************************************************************
	*				  			PMXInfo
	*************************************************************************//**
	*  @class     PMXInfo
	*  @brief     Model name and comment
	*****************************************************************************/
	struct PMXInfo
	{
		std::string ModelName;
		std::string EngliseModelName;
		std::string Comment;
		std::string EnglishComment;

		bool Read(FILE* file, const PMXSetting* setting);
	};

	enum class PMXVertexWeight : UINT8
	{
		BDEF1, // boneIndices[0]
		BDEF2, // boneIndices[0-1] + boneWeights[0]  (PMD Format weight(0.0-1.0))
		BDEF4, // boneIndices[0-3] + boneWeights[0-3] (may not weight total = 1.0)
		SDEF,  // boneIndices[0-1] + boneWeights[0] + SDefC + SDefR0 + SDefR1
		QDEF   // boneIndices[0-3] + boneWeights[0-3]
	};
	/****************************************************************************
	*				  			PMXVertex
	*************************************************************************//**
	*  @class     PMXVertex
	*  @brief     Vertex
	*****************************************************************************/
	struct PMXVertex
	{
		Float3          Position;
		Float3          Normal;
		Float2          UV;
		Float4          AddUV[4];      // mainly use for effect
		PMXVertexWeight WeightType;
		INT32           BoneIndices[4];
		float           BoneWeights[4];
		Float3          SDefC;
		Float3          SDefR0;
		Float3          SDefR1;

		float EdgeMagnitude;

		bool Read(FILE* file, const PMXSetting* setting);
	};
	/****************************************************************************
	*				  			PMXMaterial
	*************************************************************************//**
	*  @class     PMXMaterial
	*  @brief     Material
	*****************************************************************************/
	struct PMXMaterial
	{
		enum class PMXDrawModeFlags : UINT8
		{
			BothSurface = 0x01,
			GroundShadow = 0x02,
			CastSelfShadow = 0x04,
			ReceiveSelfShadow = 0x08,
			DrawEdge = 0x10,
			VertexColor = 0x20, // ver2.1 only
			DrawPoint = 0x40, // ver2.1 only
			DrawLine = 0x80  // ver2.1 only
		};
		enum class PMXSphereMapMode : UINT8
		{
			None,
			Multiply,
			Addition,
			SubTexture
		};
		enum class PMXToonTextureMode :UINT8
		{
			Separate,
			Common
		};

		std::string        MaterialName;
		std::string        EnglishName;
		Float4             Diffuse;
		float              SpecularPower;
		Float3             Specular;
		Float3             Ambient;
		PMXDrawModeFlags   DrawMode;
		Float4             EdgeColor;
		float              EdgeSize;
		INT32              TextureIndex;
		INT32              SphereMapTextureIndex;
		PMXSphereMapMode   SphereMapMode;
		PMXToonTextureMode ToonTextureMode;
		INT32              ToonTextureIndex;
		std::string        Memo;
		INT32              FaceIndicesCount;

		void Read(FILE* file, const PMXSetting* setting);
	};
	/****************************************************************************
	*				  			PMXIKLink
	*************************************************************************//**
	*  @class     PMXIKLine
	*  @brief     IK link bone index
	*****************************************************************************/
	struct PMXIKLink
	{
		INT32 LinkTarget;
		unsigned char EnableLimit;
		//When Enable Limit is 1, the variations is needed.
		Float3   AngleMin; // radian 
		Float3   AngleMax; // radian

		void Read(FILE* filePtr, const PMXSetting* setting);
	};
	/****************************************************************************
	*				  			PMXBone
	*************************************************************************//**
	*  @class     PMXBone
	*  @brief     Bone
	*****************************************************************************/
	struct PMXBone
	{
		enum class PMXBoneFlag : UINT16
		{
			TargetShowMode     = 0x0001, // Connection destination PMD child bone specification) (0: coodinate offset specify, 1: bone specify)
			AllowRotate        = 0x0002, // allow rotation
			AllowTranslate     = 0x0004, // allow translation
			Visible            = 0x0008, // display
			AllowControl       = 0x0010, // allow control
			IKBone             = 0x0020, // IK
			AppendLocal        = 0x0080, // local axis(0: user deformation value / IK link / multiple assignment 1: parent's local deformation amount )
			AppendRotate       = 0x0100, // append rotation
			AppendTranslate    = 0x0200, // append translate
			FixedAxis          = 0x0400, // fixed axis
			LocalAxis          = 0x0800, // local axis
			DeformAfterPhisics = 0x1000, // deform after phisics
			DeformOuterParent  = 0x2000, // deform outer parent
		};
		std::string BoneName;
		std::string EnglishName;
		Float3      Position;
		INT32       ParentBoneIndex;
		INT32       DeformDepth;
		PMXBoneFlag BoneFlag;
		Float3      PositionOffset;
		INT32       LinkBoneIndex;
		INT32       AppendBoneIndex;
		float       AppendWeight;
		Float3      FixedAxis;         // PMXBoneFlag: FixedAxis is enabled
		Float3      LocalAxis_X;       // PMXBoneFlag: LocalAxis is enabled 
		Float3      LocalAxis_Z;       // PMXBoneFlag: LocalAxis is enabled
		INT32       KeyValue;          // PMXBoneFlag: DeformAfterPhisics is enabled
		INT32       IKTargetBoneIndex; // PMXBoneFlag: IKBone is enabled
		INT32       IKIterationCount;  // PMXBoneFlag: IKBone is enabled
		float       IKAngleLimit;      // PMXBoneFlag: IKBone is enabled (radian)
		std::vector<PMXIKLink> IKLinks;

		void Read(FILE* filePtr, const PMXSetting* setting);
		~PMXBone() { IKLinks.clear(); IKLinks.shrink_to_fit(); }
	};
	/****************************************************************************
	*				  			PMXMorph
	*************************************************************************//**
	*  @class     PMXMoroh 
	*  @brief     incrementÅ@data for morphing
	*****************************************************************************/
	enum class PMXMorphType : UINT8
	{
		Group,
		Position,
		Bone,
		UV,
		AddUV1,
		AddUV2,
		AddUV3,
		AddUV4,
		Material,
		Flip,     // ver 2,1 only
		Impulse   // ver 2.1 only
	};
	enum class PMXFacePart : UINT8
	{
		Base,
		Eyebrow,
		Eye,
		Mouth,
		Other,
	};
	struct PMXMorph
	{
		struct PositionMorph
		{
			INT32    VertexIndex;
			Float3   Position;
		};
		struct UVMorph
		{
			INT32    VertexIndex;
			Float4   UV;
		};
		struct BoneMorph
		{
			INT32    BoneIndex;
			Float3   Position;
			Float4   Quaternion;
		};
		struct MaterialMorph
		{
			enum class OpType : UINT8
			{
				Mutiply,
				Addition
			};

			INT32    MaterialIndex;
			OpType   OpType;
			Float4   Diffuse;
			Float3   Specular;
			float    SpecularPower;
			Float3   Ambient;
			Float4   EdgeColor;
			float    EdgeSize;
			Float4   TextureFactor;
			Float4   SphereMapFactor;
			Float4   ToonTextureFactor;
		};
		struct GroupMorph
		{
			INT32 MorphIndex;
			float Weight;
		};
		struct FlipMorph
		{
			INT32 MorphIndex;
			float Weight;
		};
		struct ImpulseMorph
		{
			INT32    RigidBodyIndex;
			UINT8    LocalFlag;         // 0: off, 1: on
			Float3   TranslateVelocity;
			Float3   RotateTorque;
		};

		std::string  Name;
		std::string  EnglishName;
		PMXFacePart  FacePart;
		PMXMorphType MorphType;
		std::vector<PositionMorph> PositionMorphs;
		std::vector<UVMorph>       UVMorphs;
		std::vector<BoneMorph>     BoneMorphs;
		std::vector<MaterialMorph> MaterialMorphs;
		std::vector<GroupMorph>    GroupMorphs;
		std::vector<FlipMorph>     FlipMorphs;
		std::vector<ImpulseMorph>  ImpulseMorphs;

		void Read(FILE* filePtr, const PMXSetting* setting);
		~PMXMorph()
		{
			PositionMorphs.clear(); PositionMorphs.shrink_to_fit();
			UVMorphs      .clear(); UVMorphs      .shrink_to_fit();
			BoneMorphs    .clear(); BoneMorphs    .shrink_to_fit();
			MaterialMorphs.clear(); MaterialMorphs.shrink_to_fit();
			GroupMorphs   .clear(); GroupMorphs   .shrink_to_fit();
			FlipMorphs    .clear(); FlipMorphs    .shrink_to_fit();
			ImpulseMorphs .clear(); ImpulseMorphs .shrink_to_fit();
		};
	};
	/****************************************************************************
	*				  			PMXDisplayFrame
	*************************************************************************//**
	*  @class     PMXDisplayFrame
	*  @brief     Display frame
	*****************************************************************************/
	struct PMXDisplayFrame
	{

		enum class TargetType : UINT8
		{
			BoneIndex,
			MorphIndex
		};
		struct     Target
		{
			TargetType Type;
			INT32      Index;
		};
		enum class FrameType : UINT8
		{
			DefaultFrame,
			SpecialFrame
		};

		std::string         Name;
		std::string         EnglishName;
		FrameType           Flag;
		std::vector<Target> Targets;
		void Read(FILE* file, const PMXSetting* setting);
		~PMXDisplayFrame()
		{
			Targets.clear(); Targets.shrink_to_fit();
		}
	};
	/****************************************************************************
	*				  			PMXRigidBody
	*************************************************************************//**
	*  @class     PMXRigidBody
	*  @brief     RigidBody
	*****************************************************************************/
	struct PMXRigidBody
	{
		enum class PMXRigidBodyShape : UINT8
		{
			Sphere,
			Box,
			Capsule
		};
		enum class PMXPhysicsCalcType : UINT8
		{
			Static,
			Dynamic,
			DynamicAndBoneMerge
		};
		std::string           Name;
		std::string           EnglishName;
		INT32                 BoneIndex;
		UINT8                 Group;
		UINT16                CollisionGroup;
		PMXRigidBodyShape     Shape;
		Float3                ShapeSize;
		Float3                Translation;
		Float3                Rotation;            // radian
		float                 Mass;
		float                 DampingTranslation;
		float                 DampingRotation;
		float                 Repulsion;
		float                 Friction;
		PMXPhysicsCalcType    RigidBodyCalcType;

		void Read(FILE* filePtr, const PMXSetting* setting);
	};
	/****************************************************************************
	*				  			PMXJoint
	*************************************************************************//**
	*  @class     PMXJoint
	*  @brief     Joint
	*****************************************************************************/
	enum class PMXJointType : UINT8
	{
		Generic6DOFSpring,
		Generic6DOF,
		Point2Point,
		ConeTwist,
		Slider,
		Hinge
	};
	struct PMXJoint
	{
		std::string  Name;
		std::string  EnglishName;
		PMXJointType JointType;
		INT32        RigidBodyIndex_A;
		INT32        RigidBodyIndex_B;
		Float3       Translation;
		Float3       Rotation;
		Float3       TranslationMin;
		Float3       TranslationMax;
		Float3       RotationMin;             // radian
		Float3       RotationMax;             // radian
		Float3       SpringTranslationFactor;
		Float3       SpringRotationFactor;

		void Read(FILE* filePtr, const PMXSetting* setting);
	};

	/****************************************************************************
	*				  			PMXSoftBody
	*************************************************************************//**
	*  @class     PMXSoftBody
	*  @brief     SoftBody
	*****************************************************************************/
	enum class PMXSoftBodyType : UINT8
	{
		TriangleMesh,
		Rope
	};
	enum class PMXSoftBodyMask : UINT8
	{
		BLink = 0x01,
		Cluster = 0x02,
		HybridLink = 0x04
	};
	enum class PMXAeroModel : INT32
	{
		kAeroModelV_TwoSided,
		kAeroModelV_OneSided,
		kAeroModelF_TwoSided,
		kAeroModelF_OneSided
	};
	struct PMXSoftBodyAnchorRigidBody
	{
		INT32 RigidBodyIndex;
		INT32 VertexIndex;
		UINT8 NearMode;
		void Read(FILE* filePtr, const PMXSetting* setting);
	};
	struct PMXSoftBody
	{
		struct PMXSoftBodyCluster
		{
			float SRHR_CL;    // Soft vs rigid hardness      [0,1];
			float SKHR_CL;    // Soft vs kinetic hardness    [0,1];
			float SSHR_CL;    // Soft vs soft hardness       [0,1]
			float SR_SPLT_CL; // Soft vs rigid impulse split [0,1];
			float SK_SPLT_CL; // Soft vs rigid impulse split [0,1];
			float SS_SPLT_CL; // Soft vs rigid impulse split [0,1];
		};
		struct PMXSoftBodyIteration
		{
			INT32 V_Iterations;
			INT32 P_Iterations;
			INT32 D_Iterations;
			INT32 C_Iterations;
		};
		struct PMXSoftBodyMaterial
		{
			float LST;
			float AST;
			float VST;
		};
		struct PMXSoftBodyConfig
		{
			float VCF; // Velocities correction factor (default V_point);
			float DP;  // Damping coefficient factor;
			float DG;  // Drag coefficient;
			float LF;  // Lift coefficent;
			float PR;  // Pressure coefficient;
			float VC;  // Volume conversation coefficient;
			float DF;  // Dynamic friction coefficient;
			float MT;  // Pose matching coefficient;
			float CHR; // Rigid contacts hardness;
			float KHR; // Kinetic contacts hardness;
			float SHR; // Soft contacts hardness;
			float AHR; // Anchors hardness;
		};
		std::string          Name;
		std::string          EnglishName;
		PMXSoftBodyType      SoftBodyType;
		INT32                MaterialIndex;
		UINT8                Group;
		UINT16               CollisionGroup;
		PMXSoftBodyMask      MaskFlag;
		INT32                BoneLinkLength;
		INT32                ClustersCount;
		float                TotalMass;
		float                CollisionMargin;
		PMXAeroModel         AeroModel;
		PMXSoftBodyConfig    Config;
		PMXSoftBodyCluster   Cluster;
		PMXSoftBodyIteration Iteration;
		PMXSoftBodyMaterial  Material;
		std::vector<PMXSoftBodyAnchorRigidBody> Anchor;
		std::vector<INT32>   VertexIndices;

		void Read(FILE* filePtr, const PMXSetting* setting);
		~PMXSoftBody()
		{
			Anchor.clear(); Anchor.shrink_to_fit();
			VertexIndices.clear(); VertexIndices.shrink_to_fit();
		}
	};

	/****************************************************************************
	*				  			PMXFile
	*************************************************************************//**
	*  @class     PMXFile
	*  @brief     temp
	*****************************************************************************/
	class PMXFile
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		bool Load(const std::wstring& filePath);
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		PMXSetting                   Setting;
		PMXInfo                      Infomation;
		std::vector<PMXVertex>       Vertices;
		std::vector<UINT32>          Indices;
		std::vector<std::string>     TexturePathList;
		std::vector<PMXMaterial>     Materials;
		std::vector<PMXBone>         Bones;
		std::vector<PMXMorph>        Morphs;
		std::vector<PMXDisplayFrame> DisplayFrames;
		std::vector<PMXRigidBody>    RigidBodies;
		std::vector<PMXJoint>        Joints;
		std::vector<PMXSoftBody>     SoftBodies;
		std::string Directory;
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		PMXFile() = default;
		~PMXFile();
		PMXFile(const PMXFile&)            = delete;
		PMXFile& operator=(const PMXFile&) = delete;
		PMXFile(PMXFile&&)                 = default;
		PMXFile& operator=(PMXFile&&)      = default;
	private:
		/****************************************************************************
		**                Private Member Variables
		*****************************************************************************/
		void ReadVertices     (FILE* filePtr);
		void ReadIndices      (FILE* filePtr);
		void ReadTextureList  (FILE* filePtr);
		void ReadMaterials    (FILE* filePtr);
		void ReadBones        (FILE* filePtr);
		void ReadMorphs       (FILE* filePtr);
		void ReadDisplayFrames(FILE* filePtr);
		void ReadRigidBodies  (FILE* filePtr);
		void ReadJoints(FILE* filePtr);
		void ReadSoftBodies(FILE* filePtr);
	};

}

#endif