//////////////////////////////////////////////////////////////////////////////////
///             @file   VMDParser.hpp
///             @brief  VMDParser
///             @author Toide Yutaro
///             @date   2022_03_11
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef VMD_PARSER_HPP
#define VMD_PARSER_HPP

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
//                              Class
//////////////////////////////////////////////////////////////////////////////////
namespace vmd
{
	using namespace gm;
#pragma pack(1)
	struct VMDHeader
	{
		std::string Header;
		std::string ModelName;

		void Read(FILE* filePtr);
		void Write(FILE* filePtr);
	};

	struct VMDBoneKeyFrame
	{
		std::string BoneName;
		UINT32      Frame;
		Float3      Translation;
		Float4      Quaternion;
		UINT8       BazierInterpolation[64]; // 0 Å` 127

		void Read(FILE* filePtr);
		void Write(FILE* filePtr);
	};

	struct VMDFaceKeyFrame
	{
		std::string Name;
		UINT32      Frame;
		float       Weight;

		void Read (FILE* filePtr);
		void Write(FILE* filePtr);
	};

	struct VMDCameraKeyFrame
	{
		UINT32 Frame;
		float  Distance;
		Float3 Position;
		Float3 Rotation;
		UINT8  Interpolation[24];
		UINT32 ViewAngle;
		UINT8  IsPerspective; // Ç‡ÇµÇ©ÇµÇΩÇÁ2Byteï™å„Ç…í«â¡Ç∑ÇÈâ¬î\ê´Ç†ÇË
		//UINT8  Unknowns[2];

		void Read(FILE* filePtr);
		void Write(FILE* filePtr);
	};

	struct VMDLightKeyFrame
	{
		UINT32 Frame;
		Float3 Color;
		Float3 Position;

		void Read(FILE* filePtr);
		void Write(FILE* filePtr);
	};

	enum class VMDShadowType : UINT8
	{
		Off,
		Mode1,
		Mode2
	};

	struct VMDSelfShadowKeyFrame
	{
		UINT32        Frame;
		VMDShadowType ShadowType;
		float         Distance;

		void Read(FILE* filePtr);
		void Write(FILE* filePtr);
	};

	struct VMDIKEnable
	{
		std::string IKName;
		bool        Enable;
		void Read(FILE* filePtr);
	};

	struct VMDIKKeyFrame
	{
		UINT32  Frame;
		bool    Display;
		std::vector<VMDIKEnable> IKEnables;

		void Read(FILE* filePtr);
		void Write(FILE* filePtr);

		~VMDIKKeyFrame()
		{
			IKEnables.clear(); IKEnables.shrink_to_fit();
		}
	};

	/****************************************************************************
	*				  			VMDFile
	*************************************************************************//**
	*  @class     VMDFile
	*  @brief     VMDFile
	*****************************************************************************/
	class VMDFile
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		bool Load(const std::wstring& filePath);
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		VMDHeader Header;
		std::string Directory;
		std::vector<VMDBoneKeyFrame  >     BoneFrames;
		std::vector<VMDFaceKeyFrame  >     FaceFrames;
		std::vector<VMDCameraKeyFrame>     CameraFrames;
		std::vector<VMDLightKeyFrame>      LightFrames;
		std::vector<VMDSelfShadowKeyFrame> ShadowFrames;
		std::vector<VMDIKKeyFrame>         IKFrames;
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		VMDFile() = default;
		~VMDFile();
		VMDFile(const VMDFile&)            = delete;
		VMDFile& operator=(const VMDFile&) = delete;
		VMDFile(VMDFile&&)                 = default;
		VMDFile& operator=(VMDFile&&)      = default;
	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/

		/****************************************************************************
		**                Private Member Variables
		*****************************************************************************/
	};
#pragma pack()
}
#endif