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
#include "Platform/Core/Include/CoreFileSystem.hpp"
#include "GameUtility/Base/Include/GUStringConverter.hpp"
#include "GameUtility/Base/Include/GUCharacterCodeConverter.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gc::file::vmd;
using namespace platform::core;
using namespace gu;

namespace
{
	gu::tstring ReadString(const gu::SharedPointer<file::IFileHandle>& fileHandle, const gu::uint64 length)
	{
		gu::string result(length, true);

		fileHandle->Read(result.CString(), sizeof(gu::char8) * length);

		return StringConverter::ConvertStringToTString(result);
	}

	void WriteString(const gu::SharedPointer<file::IFileHandle>& fileHandle, const gu::tstring& input, const int32 requiredLength)
	{
		gu::string result = StringConverter::ConvertTStringToString(input);

		fileHandle->Write(result.CString(), sizeof(gu::char8) * result.Size());

		if (requiredLength >= result.Size())
		{
			fileHandle->Seek(fileHandle->Tell() + requiredLength - result.Size());
		}
		else
		{
			throw "Invalid string length.";
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor
VMDFile::~VMDFile()
{
	BoneFrames      .Clear(); BoneFrames      .ShrinkToFit();
	FaceFrames      .Clear(); FaceFrames      .ShrinkToFit();
	CameraFrames    .Clear(); CameraFrames    .ShrinkToFit();
	LightFrames     .Clear(); LightFrames     .ShrinkToFit();
	SelfShadowFrames.Clear(); SelfShadowFrames.ShrinkToFit();
	IKFrames        .Clear(); IKFrames        .ShrinkToFit();
}
#pragma endregion Constructor and Destructor

#pragma region Public Function

/*!**********************************************************************
*  @brief     VMDファイルを読み込む関数
*  @param[in] gu::tsring& ファイルパス
*  @return    bool
*************************************************************************/
bool VMDFile::Read(const gu::tstring& filePath)
{
	/*-------------------------------------------------------------------
	-             ファイルハンドルの取得
	---------------------------------------------------------------------*/
	const auto fileHandle = IFileSystem::Get()->OpenRead(filePath.CString());

	// ファイルが開けなかった場合はfalseを返す
	if (!fileHandle)
	{
		return false;
	}

	// ディレクトリの取得
	Directory = IFileSystem::Get()->GetDirectory(filePath.CString());

	/*-------------------------------------------------------------------
	-            ヘッダ情報の読み込み
	---------------------------------------------------------------------*/
	Header.Read(fileHandle);

	if (!Header.IsValid())
	{
		return false;
	}

	/*-------------------------------------------------------------------
	-           ボーンのキーフレーム情報を格納する構造体です. 
	---------------------------------------------------------------------*/
	int32 boneFrameCount = 0;
	fileHandle->Read(&boneFrameCount, sizeof(int32));

	BoneFrames.Resize(boneFrameCount);
	for (int32 i = 0; i < boneFrameCount; ++i)
	{
		BoneFrames[i].Read(fileHandle);
	}

	/*-------------------------------------------------------------------
	-           表情のキーフレーム情報を格納する構造体です.
	---------------------------------------------------------------------*/
	int32 faceFrameCount = 0;
	fileHandle->Read(&faceFrameCount, sizeof(int32));

	FaceFrames.Resize(faceFrameCount);
	for (int32 i = 0; i < faceFrameCount; ++i)
	{
		FaceFrames[i].Read(fileHandle);
	}

	/*-------------------------------------------------------------------
	-           カメラのキーフレーム情報を格納する構造体です.
	---------------------------------------------------------------------*/
	int32 cameraFrameCount = 0;
	fileHandle->Read(&cameraFrameCount, sizeof(int32));

	CameraFrames.Resize(cameraFrameCount);
	for (int32 i = 0; i < cameraFrameCount; ++i)
	{
		CameraFrames[i].Read(fileHandle);
	}

	/*-------------------------------------------------------------------
	-           照明のキーフレーム情報を格納する構造体です.
	---------------------------------------------------------------------*/
	int32 lightFrameCount = 0;
	fileHandle->Read(&lightFrameCount, sizeof(int32));

	LightFrames.Resize(lightFrameCount);
	for (int32 i = 0; i < lightFrameCount; ++i)
	{
		LightFrames[i].Read(fileHandle);
	}

	/*-------------------------------------------------------------------
	-           セルフ影のキーフレーム情報を格納する構造体です.
	---------------------------------------------------------------------*/
	int32 selfShadowFrameCount = 0;
	fileHandle->Read(&selfShadowFrameCount, sizeof(int32));

	SelfShadowFrames.Resize(selfShadowFrameCount);
	for (int32 i = 0; i < selfShadowFrameCount; ++i)
	{
		SelfShadowFrames[i].Read(fileHandle);
	}

	/*-------------------------------------------------------------------
	-           IKのキーフレーム情報を格納する構造体です.
	---------------------------------------------------------------------*/
	int32 ikFrameCount = 0;
	fileHandle->Read(&ikFrameCount, sizeof(int32));

	IKFrames.Resize(ikFrameCount);
	for (int32 i = 0; i < ikFrameCount; ++i)
	{
		IKFrames[i].Read(fileHandle);
	}

	return true;
}

/*!**********************************************************************
*  @brief     VMDファイルを書き込む関数
*  @param[in] gu::tsring& ファイルパス
*  @return    void
*************************************************************************/
bool VMDFile::Write(const gu::tstring& filePath)
{
	/*-------------------------------------------------------------------
	-             ファイルハンドルの取得
	---------------------------------------------------------------------*/
	const auto fileHandle = IFileSystem::Get()->OpenWrite(filePath.CString());

	// ファイルが開けなかった場合はfalseを返す
	if (!fileHandle)
	{
		return false;
	}

	// ディレクトリの取得
	Directory = IFileSystem::Get()->GetDirectory(filePath.CString());

	/*-------------------------------------------------------------------
	-            ヘッダ情報の読み込み
	---------------------------------------------------------------------*/
	Header.Write(fileHandle);

	if (!Header.IsValid())
	{
		return false;
	}

	/*-------------------------------------------------------------------
	-           ボーンのキーフレーム情報を格納する構造体です.
	---------------------------------------------------------------------*/
	int32 boneFrameCount = static_cast<int32>(BoneFrames.Size());
	fileHandle->Write(&boneFrameCount, sizeof(int32));

	for (int32 i = 0; i < boneFrameCount; ++i)
	{
		BoneFrames[i].Write(fileHandle);
	}

	/*-------------------------------------------------------------------
	-           表情のキーフレーム情報を格納する構造体です.
	---------------------------------------------------------------------*/
	int32 faceFrameCount = static_cast<int32>(FaceFrames.Size());
	fileHandle->Write(&faceFrameCount, sizeof(int32));

	for (int32 i = 0; i < faceFrameCount; ++i)
	{
		FaceFrames[i].Write(fileHandle);
	}

	/*-------------------------------------------------------------------
	-           カメラのキーフレーム情報を格納する構造体です.
	---------------------------------------------------------------------*/
	int32 cameraFrameCount = static_cast<int32>(CameraFrames.Size());
	fileHandle->Write(&cameraFrameCount, sizeof(int32));

	for (int32 i = 0; i < cameraFrameCount; ++i)
	{
		CameraFrames[i].Write(fileHandle);
	}

	/*-------------------------------------------------------------------
	-           照明のキーフレーム情報を格納する構造体です.
	---------------------------------------------------------------------*/
	int32 lightFrameCount = static_cast<int32>(LightFrames.Size());
	fileHandle->Write(&lightFrameCount, sizeof(int32));

	for (int32 i = 0; i < lightFrameCount; ++i)
	{
		LightFrames[i].Write(fileHandle);
	}

	/*-------------------------------------------------------------------
	-           セルフ影のキーフレーム情報を格納する構造体です.
	---------------------------------------------------------------------*/
	int32 selfShadowFrameCount = static_cast<int32>(SelfShadowFrames.Size());
	fileHandle->Write(&selfShadowFrameCount, sizeof(int32));

	for (int32 i = 0; i < selfShadowFrameCount; ++i)
	{
		SelfShadowFrames[i].Write(fileHandle);
	}

	/*-------------------------------------------------------------------
	-           IKのキーフレーム情報を格納する構造体です.
	---------------------------------------------------------------------*/
	int32 ikFrameCount = static_cast<int32>(IKFrames.Size());
	fileHandle->Write(&ikFrameCount, sizeof(int32));

	for (int32 i = 0; i < ikFrameCount; ++i)
	{
		IKFrames[i].Write(fileHandle);
	}

	return true;
}
#pragma endregion Public Function