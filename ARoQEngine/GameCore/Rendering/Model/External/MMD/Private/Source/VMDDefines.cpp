//////////////////////////////////////////////////////////////////////////////////
///  @file   VMDDefines.cpp
///  @brief  VMDで使用する定義をまとめたヘッダファイル.
///  @author toide
///  @date   2024/06/05 23:58:12
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Rendering/Model/External/MMD/Private/Include/VMDDefines.hpp"
#include "Platform/Core/Include/CoreFileSystem.hpp"
#include "GameUtility/Base/Include/GUStringConverter.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gc::file::vmd;
using namespace platform::core::file;
using namespace gu;

namespace
{
	gu::tstring ReadString(const gu::SharedPointer<IFileHandle>& fileHandle, const gu::uint64 length)
	{
		gu::string result(length, true);

		fileHandle->Read(result.CString(), sizeof(gu::char8) * length);

		return StringConverter::ConvertStringToTString(result);
	}

	void WriteString(const gu::SharedPointer<IFileHandle>& fileHandle, const gu::tstring& input, const int32 requiredLength)
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
//                               Class
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor

#pragma endregion Constructor and Destructor

#pragma region Public Function
/*!**********************************************************************
*  @brief     有効なVMDファイルか
*  @param[in] void
*  @return    bool
*************************************************************************/
bool VMDHeader::IsValid() const
{
	return Header.Contains(SP("Vocaloid Motion Data"), true);
}

/*!**********************************************************************
*  @brief     VMDファイルをもとにデータ読み込み
*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
*  @return    void
*************************************************************************/
void VMDHeader::Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle)
{
	Header    = ReadString(fileHandle, 30);
	ModelName = ReadString(fileHandle, 20);
}

/*!**********************************************************************
*  @brief     VMDファイルをもとにデータ書き込み
*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
*  @return    void
*************************************************************************/
void VMDHeader::Write(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle)
{
	WriteString(fileHandle, Header,    30);
	WriteString(fileHandle, ModelName, 20);
}

/*!**********************************************************************
*  @brief     VMDファイルをもとにデータ読み込み
*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
*  @return    void
*************************************************************************/
void VMDBoneKeyFrame::Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle)
{
	Name = ReadString(fileHandle, 15);
	fileHandle->Read(&Frame             , sizeof(uint32));
	fileHandle->Read(&Translation       , sizeof(gm::Float3));
	fileHandle->Read(&Quaternion        , sizeof(gm::Float4));
	fileHandle->Read(BazierInterpolation, sizeof(uint8) * 64);
}

/*!**********************************************************************
*  @brief     VMDファイルをもとにデータ書き込み
*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
*  @return    void
*************************************************************************/
void VMDBoneKeyFrame::Write(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle)
{
	WriteString(fileHandle, Name, 15);
	fileHandle->Write(&Frame             , sizeof(uint32));
	fileHandle->Write(&Translation       , sizeof(gm::Float3));
	fileHandle->Write(&Quaternion        , sizeof(gm::Float4));
	fileHandle->Write(BazierInterpolation, sizeof(uint8) * 64);
}

/*!**********************************************************************
*  @brief     VMDファイルをもとにデータ読み込み
*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
*  @return    void
*************************************************************************/
void VMDFaceKeyFrame::Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle)
{
	Name = ReadString(fileHandle, 15);
	fileHandle->Read(&Frame , sizeof(uint32));
	fileHandle->Read(&Weight, sizeof(float32));
}

/*!**********************************************************************
*  @brief     VMDファイルをもとにデータ書き込み
*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
*  @return    void
*************************************************************************/
void VMDFaceKeyFrame::Write(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle)
{
	WriteString(fileHandle, Name, 15);
	fileHandle->Write(&Frame , sizeof(uint32));
	fileHandle->Write(&Weight, sizeof(float32));
}

/*!**********************************************************************
*  @brief     VMDファイルをもとにデータ読み込み
*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
*  @return    void
*************************************************************************/
void VMDCameraKeyFrame::Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle)
{
	fileHandle->Read(&Frame, sizeof(uint32));
	fileHandle->Read(&Distance     , sizeof(float32));
	fileHandle->Read(&Position     , sizeof(gm::Float3));
	fileHandle->Read(&Rotation     , sizeof(gm::Float3));
	fileHandle->Read(Interpolation , sizeof(uint8) * 24);
	fileHandle->Read(&ViewAngle    , sizeof(uint32));

	uint8 value = 0;
	fileHandle->Read(&value, sizeof(uint8));
	IsPerspective = !value; // 0: true, 1: false...(なぜ)
}

/*!**********************************************************************
*  @brief     VMDファイルをもとにデータ書き込み
*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
*  @return    void
*************************************************************************/
void VMDCameraKeyFrame::Write(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle)
{
	fileHandle->Write(&Frame, sizeof(uint32));
	fileHandle->Write(&Distance     , sizeof(float32));
	fileHandle->Write(&Position     , sizeof(gm::Float3));
	fileHandle->Write(&Rotation     , sizeof(gm::Float3));
	fileHandle->Write(Interpolation , sizeof(uint8) * 24);
	fileHandle->Write(&ViewAngle    , sizeof(uint32));

	uint8 value = !IsPerspective; 
	fileHandle->Write(&value, sizeof(uint8));
}

/*!**********************************************************************
*  @brief     VMDファイルをもとにデータ読み込み
*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
*  @return    void
*************************************************************************/
void VMDLightKeyFrame::Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle)
{
	fileHandle->Read(&Frame   , sizeof(uint32));
	fileHandle->Read(&Color   , sizeof(gm::Float3));
	fileHandle->Read(&Position, sizeof(gm::Float3));
}

/*!**********************************************************************
*  @brief     VMDファイルをもとにデータ書き込み
*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
*  @return    void
*************************************************************************/
void VMDLightKeyFrame::Write(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle)
{
	fileHandle->Write(&Frame   , sizeof(uint32));
	fileHandle->Write(&Color   , sizeof(gm::Float3));
	fileHandle->Write(&Position, sizeof(gm::Float3));
}

/*!**********************************************************************
*  @brief     VMDファイルをもとにデータ読み込み
*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
*  @return    void
*************************************************************************/
void VMDSelfShadowKeyFrame::Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle)
{
	fileHandle->Read(&Frame     , sizeof(uint32));
	fileHandle->Read(&ShadowType, sizeof(uint8));
	fileHandle->Read(&Distance  , sizeof(float32));
}

/*!**********************************************************************
*  @brief     VMDファイルをもとにデータ書き込み
*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
*  @return    void
*************************************************************************/
void VMDSelfShadowKeyFrame::Write(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle)
{
	fileHandle->Write(&Frame     , sizeof(uint32));
	fileHandle->Write(&ShadowType, sizeof(uint8));
	fileHandle->Write(&Distance  , sizeof(float32));
}

/*!**********************************************************************
*  @brief     VMDファイルをもとにデータ読み込み
*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
*  @return    void
*************************************************************************/
void VMDIKEnable::Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle)
{
	Name = ReadString(fileHandle, 20);
	fileHandle->Read(&IsEnable, sizeof(bool));
}

/*!**********************************************************************
*  @brief     VMDファイルをもとにデータ書き込み
*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
*  @return    void
*************************************************************************/
void VMDIKEnable::Write(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle)
{
	WriteString(fileHandle, Name, 20);
	fileHandle->Write(&IsEnable, sizeof(bool));
}

/*!**********************************************************************
*  @brief     VMDファイルをもとにデータ読み込み
*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
*  @return    void
*************************************************************************/
void VMDIKKeyFrame::Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle)
{
	fileHandle->Read(&Frame, sizeof(uint32));
	fileHandle->Read(&IsDisplay, sizeof(bool));

	uint32 ikCount;
	fileHandle->Read(&ikCount, sizeof(ikCount));
	IKEnables.Resize(ikCount);
	for (auto& ikEnable : IKEnables)
	{
		ikEnable.Read(fileHandle);
	}
}

/*!**********************************************************************
*  @brief     VMDファイルをもとにデータ書き込み
*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
*  @return    void
*************************************************************************/
void VMDIKKeyFrame::Write(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle)
{
	fileHandle->Write(&Frame, sizeof(uint32));
	fileHandle->Write(&IsDisplay, sizeof(bool));

	uint32 ikCount = static_cast<uint32>(IKEnables.Size());
	fileHandle->Write(&ikCount, sizeof(ikCount));
	for (auto& ikEnable : IKEnables)
	{
		ikEnable.Write(fileHandle);
	}
}

#pragma endregion Public Function