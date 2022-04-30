//////////////////////////////////////////////////////////////////////////////////
///             @file   PMXFormat.hpp
///             @brief  PMXFile Format
///             @author Toide Yutaro
///             @date   2022_04_27
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef PMX_FORMAT_HPP
#define PMX_FORMAT_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/DirectX12/Core/Include/DirectX12VertexTypes.hpp"
#include <string>
#include <vector>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                              Class
//////////////////////////////////////////////////////////////////////////////////
namespace pmx
{
	constexpr int PMX_BONE_MATRIX_SIZE = 512;

#pragma pack(1)
	struct PMXHeader
	{
		enum class PMXEncode : UINT8
		{
			UTF16 = 0,
			UTF8 = 1
		};
		char      Signature[4];
		float     Version;            // ver 2.0 / 2.1
		UINT8     DataSize;
		PMXEncode Encode;             // 0: UTF16, 1: UTF8
		UINT8     AddUVCount;         // 0 ~ 4
		UINT8     VertexIndexSize;    // 1, 2, or 4
		UINT8     TextureIndexSize;   // 1, 2, or 4
		UINT8     MaterialIndexSize;  // 1, 2, or 4
		UINT8     BoneIndexSize;      // 1, 2, or 4
		UINT8     FaceIndexSize;      // 1, 2, or 4
		UINT8     RigidBodyIndexSize; // 1, 2, or 4 
	};

	struct PMXInfo
	{
		std::string ModelName;
		std::string EngliseModelName;
		std::string Comment;
		std::string EnglishComment;
	};

	enum class PMXVertexWeight : UINT8
	{
		BDEF1, // boneIndices[0]
		BDEF2, // boneIndices[0-1] + boneWeights[0]  (PMD Format weight(0.0-1.0))
		BDEF4, // boneIndices[0-3] + boneWeights[0-3] (may not weight total = 1.0)
		SDEF,  // boneIndices[0-1] + boneWeights[0] + SDefC + SDefR0 + SDefR1
		QDEF   // boneIndices[0-3] + boneWeights[0-3]
	};

	struct PMXVertex
	{
		gm::Float3          Position;
		gm::Float3          Normal;
		gm::Float2          UV;
		gm::Float4          AddUV[4];      // mainly use for effect
		PMXVertexWeight     WeightType;
		INT32               BoneIndices[4];
		float               BoneWeights[4];
		gm::Float3          SDefC;
		gm::Float3          SDefR0;
		gm::Float3          SDefR1;
		float               EdgeMagnitude;

	};

	struct PMXMesh
	{
		UINT32 Indices[3];
	};

	struct PMXTexture
	{
		std::string TextureName;
	};

	enum class PMXDrawModeFlags : UINT8
	{
		BothSurface       = 0x01,
		GroundShadow      = 0x02,
		CastSelfShadow    = 0x04, 
		ReceiveSelfShadow = 0x08,
		DrawEdge          = 0x10,
		VertexColor       = 0x20, // ver2.1 only
		DrawPoint         = 0x40, // ver2.1 only
		DrawLine          = 0x80  // ver2.1 only
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
		Common    // toom01.bmp Å` toon10.bmp
	};

	struct PMXMaterial
	{
		std::string        MaterialName;
		std::string        EnglishName;
		gm::Float4         Diffuse;                  // rgba
		gm::Float3         Specular;                 // rgb
		float              SpecularPower;             
		gm::Float3         Ambient;                  // rgb
		PMXDrawModeFlags   DrawMode;
		gm::Float4         EdgeColor;
		float              EdgeSize;
		INT32              TextureIndex;             
		INT32              SphereMapTextureIndex;
		PMXSphereMapMode   SphereMapMode;
		PMXToonTextureMode ToonTextureMode;
		INT32              ToonTextureIndex;
		std::string        Memo;
		INT32              FaceIndicesCount;        // 3 * n num

	};

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

	struct PMXIKLink
	{
		INT32 IKBoneIndex;
		unsigned char EnableLimit;

		//When Enable Limit is 1, the variations is needed.
		gm::Float3   AngleMin; // radian 
		gm::Float3   AngleMax; // radian
	};

	struct PMXBone
	{
		std::string BoneName;
		std::string EnglishName;
		gm::Float3  Position;
		INT32       ParentBoneIndex;
		INT32       DeformDepth;
		PMXBoneFlag BoneFlag;
		gm::Float3  PositionOffset;
		INT32       LinkBoneIndex;
		INT32       AppendBoneIndex;
		float       AppendWeight;
		gm::Float3  FixedAxis;         // PMXBoneFlag: FixedAxis is enabled
		gm::Float3  LocalAxis_X;       // PMXBoneFlag: LocalAxis is enabled 
		gm::Float3  LocalAxis_Z;       // PMXBoneFlag: LocalAxis is enabled
		INT32       KeyValue;          // PMXBoneFlag: DeformAfterPhisics is enabled
		INT32       IKTargetBoneIndex; // PMXBoneFlag: IKBone is enabled
		INT32       IKIterationCount;  // PMXBoneFlag: IKBone is enabled
		float       IKAngleLimit;      // PMXBoneFlag: IKBone is enabled (radian)

		std::vector<PMXIKLink> IKLinks;
	};

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

	struct PMXMorph
	{
		enum class PMXFacePart : UINT8
		{
			Base,
			Eyebrow,
			Eye,
			Mouth,
			Other
		};
		struct PositionMorph
		{
			INT32      VertexIndex;
			gm::Float3 Position;
		};
		struct UVMorph
		{
			INT32      VertexIndex;
			gm::Float4 UV;
		};
		struct BoneMorph
		{
			INT32      BoneIndex;
			gm::Float3 Position;
			gm::Float4 Quaternion;
		};
		struct MaterialMorph
		{
			enum class OpType : UINT8
			{
				Multiply,
				Addition
			};
			INT32        MaterialIndex;
			OpType       OpType;
			gm::Float4   Diffuse;
			gm::Float3   Specular;
			float        SpecularPower;
			gm::Float3   Ambient;
			gm::Float4   EdgeColor;
			float        EdgeSize;
			gm::Float4   TextureFactor;
			gm::Float4   SphereMapFactor;
			gm::Float4   ToonTextureFactor;
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
			INT32      RigidBodyIndex;
			UINT8      LocalFlag;// 0: off, 1: on
			gm::Float3 TranslateVelocity;
			gm::Float3 RotateTorque;
		};

		std::string Name;
		std::string EnglishName;
		PMXFacePart FacePart;
		PMXMorphType MorphType;

		std::vector<PositionMorph> PositionMorphs;
		std::vector<UVMorph>       UVMorphs;
		std::vector<BoneMorph>     BoneMorphs;
		std::vector<MaterialMorph> MaterialMorphs;
		std::vector<GroupMorph>    GroupMorphs;
		std::vector<FlipMorph>     FlipMorphs;
		std::vector<ImpulseMorph>  ImpulseMorphs;
	};

	struct PMXDisplayFrame
	{
		std::string Name;
		std::string EnglishName;

		enum class TargetType : UINT8
		{
			BoneIndex,
			MorphIndex
		};

		struct Target
		{
			TargetType Type;
			INT32      Index;
		};

		enum class FrameType : UINT8
		{
			DefaultFrame,
			SpecialFrame
		};

		FrameType           Flag;
		std::vector<Target> Targets;
	};

	struct PMXRigidBody
	{
		enum class PMXRigidBodyShape : UINT8
		{
			Sphere,
			Box,
			Capsule
		};

		enum class PMXRigidBodyOperation : UINT8
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
		gm::Float3            ShapeSize;
		gm::Float3            Translation;
		gm::Float3            Rotation;            // raduan
		float                 Mass;
		float                 DampingTranslation;
		float                 DampingRotation;
		float                 Repulsion;
		float                 Friction;
		PMXRigidBodyOperation RigidBodyOperation;

	};

	enum class PMXJointType : UINT8
	{
		SpringDOF6,
		DOF6,
		P2P,
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
		gm::Float3   Translation;
		gm::Float3   Rotation;
		gm::Float3   TranslationMin;
		gm::Float3   TranslationMax;
		gm::Float3   RotationMin;             // radian
		gm::Float3   RotationMax;             // radian
		gm::Float3   SpringTranslationFactor;
		gm::Float3   SpringRotationFactor;
	};

	struct PMXSoftBody
	{
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

		struct PMXSoftBodyAnchorRigidBody
		{
			INT32 RigidBodyIndex;
			INT32 VertexIndex;
			UINT8 NearMode; // 0: off , 1: on
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
	};
#pragma pack(0)
}
#endif