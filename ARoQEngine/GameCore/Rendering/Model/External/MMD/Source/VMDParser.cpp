//////////////////////////////////////////////////////////////////////////////////
//              @file   VMDParser.cpp
///             @brief  VMD File Parser
///             @author Toide Yutaro
///             @date   2022_05_05
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Rendering/Model/External/MMD/Include/VMDParser.hpp"
#include "GameUtility/File/Include/UnicodeUtility.hpp"
#include "GameUtility/File/Include/FileSystem.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
namespace
{
	constexpr int INVALID_VALUE = -1;
}
using namespace vmd;
using namespace file;
void ReadVMDString(FILE* filePtr, std::string* string, UINT32 bufferSize);
//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
bool VMDFile::Load(const std::wstring& filePath)
{
	/*-------------------------------------------------------------------
	-             Open File
	---------------------------------------------------------------------*/
	FILE* filePtr = file::FileSystem::OpenFile(filePath);
	if (filePtr == nullptr) { std::cerr << "Invalid vmd file" << std::endl; return false; }
	Directory    = file::FileSystem::GetDirectory(unicode::ToUtf8String(filePath));
	/*-------------------------------------------------------------------
	-             Read Header
	---------------------------------------------------------------------*/
	Header.Read(filePtr);
	/*-------------------------------------------------------------------
	-             Read BoneFrames
	---------------------------------------------------------------------*/
	UINT32 boneFrameCount = 0;
	fread_s(&boneFrameCount, sizeof(boneFrameCount), sizeof(UINT32), 1, filePtr);
	BoneFrames.resize(boneFrameCount);
	for (auto& boneFrame : BoneFrames) { boneFrame.Read(filePtr); }
	/*-------------------------------------------------------------------
	-             Read FaceFrames
	---------------------------------------------------------------------*/
	UINT32 faceFrameCount = 0;
	fread_s(&faceFrameCount, sizeof(faceFrameCount), sizeof(UINT32), 1, filePtr);
	FaceFrames.resize(faceFrameCount);
	for (auto& faceFrame : FaceFrames) { faceFrame.Read(filePtr); }
	/*-------------------------------------------------------------------
	-             Read CameraFrames
	---------------------------------------------------------------------*/
	if (!feof(filePtr))
	{
		UINT32 cameraFrameCount = 0;
		fread_s(&cameraFrameCount, sizeof(cameraFrameCount), sizeof(UINT32), 1, filePtr);
		CameraFrames.resize(cameraFrameCount);
		for (auto& cameraFrame : CameraFrames)
		{ 
			cameraFrame.Read(filePtr); 
		}
	}

	/*-------------------------------------------------------------------
	-             Read LightFrames
	---------------------------------------------------------------------*/
	if (!feof(filePtr))
	{
		UINT32 lightFrameCount = 0;
		fread_s(&lightFrameCount, sizeof(lightFrameCount), sizeof(UINT32), 1, filePtr);
		LightFrames.resize(lightFrameCount);
		for (auto& lightFrame : LightFrames)
		{
			lightFrame.Read(filePtr);
		}
	}

	/*-------------------------------------------------------------------
	-             Read ShadowFrames
	---------------------------------------------------------------------*/
	if (!feof(filePtr))
	{
		UINT32 shadowFrameCount;
		fread_s(&shadowFrameCount, sizeof(shadowFrameCount), sizeof(UINT32), 1, filePtr);
		ShadowFrames.resize(shadowFrameCount);
		for (auto& shadowFrame : ShadowFrames) { shadowFrame.Read(filePtr); }
	}

	/*-------------------------------------------------------------------
	-             Read IKFrames
	---------------------------------------------------------------------*/
	if (!feof(filePtr))
	{
		UINT32 ikFrameCount = 0;
		fread_s(&ikFrameCount, sizeof(ikFrameCount), sizeof(UINT32), 1, filePtr);
		IKFrames.resize(ikFrameCount);
		for (auto& ikFrame : IKFrames) { ikFrame.Read(filePtr); }
	}

	return true;
}
VMDFile::~VMDFile()
{
	BoneFrames.clear(); BoneFrames.shrink_to_fit();
	FaceFrames.clear(); FaceFrames.shrink_to_fit();
	CameraFrames.clear(); CameraFrames.shrink_to_fit();
	LightFrames.clear(); LightFrames.shrink_to_fit();
	ShadowFrames.clear(); ShadowFrames.shrink_to_fit();
	IKFrames.clear(); IKFrames.shrink_to_fit();
}
#pragma region EachReadFunction
void VMDHeader            ::Read(FILE* filePtr)
{
	ReadVMDString(filePtr, &Header   , 30);
	ReadVMDString(filePtr, &ModelName, 20);

}
void VMDBoneKeyFrame      ::Read(FILE* filePtr)
{
	ReadVMDString(filePtr, &BoneName, 15);
	fread_s(&Frame              , sizeof(Frame              ), sizeof(UINT32), 1, filePtr);
	fread_s(&Translation        , sizeof(Translation        ), sizeof(Float3), 1, filePtr);
	fread_s(&Quaternion         , sizeof(Quaternion         ), sizeof(Float4), 1, filePtr);
	fread_s(&BazierInterpolation, sizeof(BazierInterpolation), sizeof(UINT8), 64, filePtr);
}
void VMDFaceKeyFrame      ::Read(FILE* filePtr)
{
	ReadVMDString(filePtr, &Name, 15);
	fread_s(&Frame , sizeof(Frame ), sizeof(UINT32), 1, filePtr);
	fread_s(&Weight, sizeof(Weight), sizeof(float ), 1, filePtr);
}
void VMDCameraKeyFrame    ::Read(FILE* filePtr)
{
	fread_s(&Frame        , sizeof(Frame        ), sizeof(UINT32), 1, filePtr);
	fread_s(&Distance     , sizeof(Distance     ), sizeof(float ), 1, filePtr);
	fread_s(&Position     , sizeof(Position     ), sizeof(Float3), 1, filePtr);
	fread_s(&Rotation     , sizeof(Rotation     ), sizeof(Float3), 1, filePtr);
	fread_s(&Interpolation, sizeof(Interpolation), sizeof(UINT8 ), 24, filePtr);
	fread_s(&ViewAngle    , sizeof(ViewAngle    ), sizeof(UINT32), 1, filePtr);
	fread_s(&IsPerspective, sizeof(IsPerspective), sizeof(IsPerspective), 1, filePtr);
	//fread_s(&Unknowns, sizeof(Unknowns), sizeof(UINT8), 2, filePtr);
}
void VMDLightKeyFrame     ::Read(FILE* filePtr)
{
	fread_s(&Frame, sizeof(Frame), sizeof(UINT32), 1, filePtr);
	fread_s(&Color, sizeof(Color), sizeof(Float3), 1, filePtr);
	fread_s(&Position, sizeof(Position), sizeof(Float3), 1, filePtr);
}
void VMDSelfShadowKeyFrame::Read(FILE* filePtr)
{
	fread_s(&Frame     , sizeof(Frame     ), sizeof(UINT32       ), 1, filePtr);
	fread_s(&ShadowType, sizeof(ShadowType), sizeof(VMDShadowType), 1, filePtr);
	fread_s(&Distance  , sizeof(Distance  ), sizeof(float        ), 1, filePtr);
}
void VMDIKEnable          ::Read(FILE* filePtr)
{
	ReadVMDString(filePtr, &IKName, 20);
	fread_s(&Enable, sizeof(Enable), sizeof(bool), 1, filePtr);
}
void VMDIKKeyFrame        ::Read(FILE* filePtr)
{
	fread_s(&Frame, sizeof(Frame), sizeof(UINT32), 1, filePtr);
	fread_s(&Display, sizeof(Display), sizeof(bool), 1, filePtr);
	
	UINT32 ikCount;
	fread_s(&ikCount, sizeof(ikCount), sizeof(UINT32), 1, filePtr);
	IKEnables.resize(ikCount);
	for (auto& ikEnable : IKEnables)
	{
		ikEnable.Read(filePtr);
	}
}
#pragma endregion EachReadFunction
#pragma region   EachWriteFunction

#pragma endregion EachWriteFunction
void ReadVMDString(FILE* filePtr, std::string* string, UINT32 bufferSize)
{
	std::string utf8String(bufferSize, '\0');
	fread_s(utf8String.data(), sizeof(char8_t) * utf8String.size(), sizeof(char8_t), utf8String.size(), filePtr);
	*string = utf8String;
}