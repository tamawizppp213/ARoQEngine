//////////////////////////////////////////////////////////////////////////////////
///  @file   PMDDefines.cpp
///  @brief  PMDで使用する定義をまとめたヘッダファイル
///  @author toide
///  @date   2024/05/11 21:56:28
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Rendering/Model/External/MMD/Private/Include/PMDDefines.hpp"
#include "Platform/Core/Include/CoreFileSystem.hpp"
#include "GameUtility/Base/Include/GUStringConverter.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace engine::file::pmd::details;
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
}
//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor

#pragma endregion Constructor and Destructor

#pragma region Public Function
/*!**********************************************************************
*  @brief     正しいファイルヘッダになっているか (Pmdというヘッダ名になっているか)
*  @param[in] void
*  @return    bool : Pmdというヘッダ名になっている場合にtrueを返します.
*************************************************************************/
bool PMDHeader::IsValid() const
{
	return Signature[0] == SP('P') && Signature[1] == SP('m') && Signature[2] == SP('d');
}

/*!**********************************************************************
*  @brief     拡張以外のデータ(English)を読み込む
*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
*  @return    void
*************************************************************************/
void PMDHeader::Read(const gu::SharedPointer<IFileHandle>& fileHandle)
{
	fileHandle->Read(&Signature[0], sizeof(char) * _countof(Signature));
	fileHandle->Read(&Version     , sizeof(float));

	NameJapanese    = ReadString(fileHandle, 20);
	CommentJapanese = ReadString(fileHandle, 256);
}

/*!**********************************************************************
*  @brief     拡張データ(English)を読み込む
*  @param[in] const gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
*  @return    void
*************************************************************************/
void PMDHeader::ReadExtension(const gu::SharedPointer<IFileHandle>& fileHandle)
{
	NameEnglish    = ReadString(fileHandle, 20);
	CommentEnglish = ReadString(fileHandle, 256);
}

/*!**********************************************************************
*  @brief     拡張以外のデータ(English)を読み込む
*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
*  @return    void
*************************************************************************/
void PMDVertex::Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle)
{
	fileHandle->Read(&Position     , sizeof(gm::Float3));
	fileHandle->Read(&Normal       , sizeof(gm::Float3));
	fileHandle->Read(&UV           , sizeof(gm::Float2));
	fileHandle->Read(&BoneIndex    , sizeof(gu::uint16) * 2);
	fileHandle->Read(&BoneWeight   , sizeof(gu::uint8));
	fileHandle->Read(&EdgeInvisible, sizeof(gu::uint8));
}

/*!**********************************************************************
*  @brief     マテリアルを読み込みます
*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
*  @param[in] const gu::tstring& ディレクトリ
*  @return    void
*************************************************************************/
void PMDMaterial::Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle, const tstring& directory)
{
	fileHandle->Read(&Diffuse      , sizeof(gm::Float4));
	fileHandle->Read(&SpecularPower, sizeof(gu::float32));
	fileHandle->Read(&Specular     , sizeof(gm::Float3));
	fileHandle->Read(&Ambient      , sizeof(gm::Float3));
	fileHandle->Read(&ToonTextureID, sizeof(gu::uint8));
	fileHandle->Read(&EdgeFlag     , sizeof(gu::uint8));
	fileHandle->Read(&IndexCount   , sizeof(gu::uint32));
	
	gu::tstring textureName = ReadString(fileHandle, 20);
	
	// テクスチャが空であるならば、何もしない
	if (textureName.IsEmpty()) { return; }

	// テクスチャのパスを作成 (SphereMapもTextureも同一階層であるという仮定が入っているため, 違う場合は修正が必要です. )
	const auto star = textureName.Find('*');
	if(star != gu::tstring::NPOS)
	{
		TexturePath   = directory + textureName.SubString(0, star - 1);
		SphereMapPath = directory + textureName.SubString(star + 1, textureName.Size() - star - 1);
	}
	else
	{
		TexturePath = directory + textureName;
	}
}

/*!**********************************************************************
*  @brief     ボーンを読み込みます
*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
*  @param[in] const gu::tstring& ディレクトリ
*  @return    void
*************************************************************************/
void PMDBone::Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle)
{
	NameJapanese = ReadString(fileHandle, 20);
	fileHandle->Read(&ParentBoneID , sizeof(gu::uint16));
	fileHandle->Read(&ChildBoneID  , sizeof(gu::uint16));
	fileHandle->Read(&BoneType     , sizeof(gu::uint8));
	fileHandle->Read(&IKBoneID     , sizeof(gu::uint16));
	fileHandle->Read(&HeadPosition , sizeof(gm::Float3));
}

/*!**********************************************************************
*  @brief     拡張データ(English)を読み込む
*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
*  @return    void
*************************************************************************/
void PMDBone::ReadExtension(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle)
{
	NameEnglish = ReadString(fileHandle, 20);
}

/*!**********************************************************************
*  @brief     ボーンIKを読み込みます
*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
*  @param[in] const gu::tstring& ディレクトリ
*  @return    void
*************************************************************************/
void PMDBoneIK::Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle)
{
	fileHandle->Read(&SelfIKBoneID, sizeof(gu::uint16));
	fileHandle->Read(&TargetBoneID, sizeof(gu::uint16));

	gu::uint8 chainLength;
	fileHandle->Read(&chainLength   , sizeof(gu::uint8));
	fileHandle->Read(&IterationCount, sizeof(gu::uint16));
	fileHandle->Read(&AngleLimit    , sizeof(gu::float32));

	Chains.Resize(static_cast<uint64>(chainLength));
	for (auto& chain : Chains)
	{
		fileHandle->Read(&chain, sizeof(gu::uint16));
	};
}

/*!**********************************************************************
*  @brief     表情を読み込みます
*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
*  @return    void
*************************************************************************/
void PMDFaceExpression::Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle)
{
	NameJapanese = ReadString(fileHandle, 20);
	
	gu::uint32 vertexCount = 0;
	fileHandle->Read(&vertexCount, sizeof(gu::uint32));
	fileHandle->Read(&Part, sizeof(gu::uint8));

	Vertices.Resize(vertexCount);
	Indices .Resize(vertexCount);
	for(gu::uint32 i = 0; i < vertexCount; ++i)
	{
		fileHandle->Read(&Indices [i], sizeof(gu::uint32));
		fileHandle->Read(&Vertices[i], sizeof(gm::Float3));
	}
}

/*!**********************************************************************
*  @brief     拡張データ(English)を読み込む
*  @param[in] const gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
*  @return    void
*************************************************************************/
void PMDFaceExpression::ReadExtension(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle)
{
	NameEnglish = ReadString(fileHandle, 20);
}

/*!**********************************************************************
*  @brief     ボーンの表示名を読み込みます
*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
*  @return    void
*************************************************************************/
void PMDBoneDisplayName::Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle)
{
	NameJapanese = ReadString(fileHandle, 50);
}

/*!**********************************************************************
*  @brief     拡張データ(English)を読み込む
*  @param[in] const gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
*  @return    void
*************************************************************************/
void PMDBoneDisplayName::ReadExtension(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle)
{
	NameEnglish = ReadString(fileHandle, 50);
}

/*!**********************************************************************
*  @brief     ボーンの対応表を読み込みます
*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
*  @return    void
*************************************************************************/
void PMDBoneNameTable::Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle)
{
	fileHandle->Read(&BoneID   , sizeof(gu::uint16));
	fileHandle->Read(&DisplayID, sizeof(gu::uint8));
}

/*!**********************************************************************
*  @brief     剛体の設定を読み込みます
*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
*  @return    void
*************************************************************************/
void PMDRigidBody::Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle)
{
	Name = ReadString(fileHandle, 20);
	
	fileHandle->Read(&BoneID          , sizeof(gu::uint16));
	fileHandle->Read(&GroupID         , sizeof(gu::uint8));
	fileHandle->Read(&MaskID          , sizeof(gu::uint16));
	fileHandle->Read(&Shape           , sizeof(gu::uint8));
	fileHandle->Read(&Size            , sizeof(gm::Float3));
	fileHandle->Read(&Position        , sizeof(gm::Float3));
	fileHandle->Read(&Orientation     , sizeof(gm::Float3));
	fileHandle->Read(&Mass            , sizeof(gu::float32));
	fileHandle->Read(&DampingTranslate, sizeof(gu::float32));
	fileHandle->Read(&DampingAngular  , sizeof(gu::float32));
	fileHandle->Read(&Restitution     , sizeof(gu::float32));
	fileHandle->Read(&Friction        , sizeof(gu::float32));
	fileHandle->Read(&Type            , sizeof(gu::uint8));
}

/*!**********************************************************************
*  @brief     関節の設定を読み込みます
*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
*  @return    void
*************************************************************************/
void PMDJoint::Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle)
{
	Name = ReadString(fileHandle, 20);

	fileHandle->Read(&RigidBodyA         , sizeof(gu::uint32));
	fileHandle->Read(&RigidBodyB         , sizeof(gu::uint32));
	fileHandle->Read(&Position           , sizeof(gm::Float3));
	fileHandle->Read(&Orientation        , sizeof(gm::Float3));
	fileHandle->Read(&TranslationLimitMin, sizeof(gm::Float3));
	fileHandle->Read(&TranslationLimitMax, sizeof(gm::Float3));
	fileHandle->Read(&OrientationLimitMin, sizeof(gm::Float3));
	fileHandle->Read(&OrientationLimitMax, sizeof(gm::Float3));
	fileHandle->Read(&SpringTranslation  , sizeof(gm::Float3));
	fileHandle->Read(&SpringOrientation  , sizeof(gm::Float3));

}
#pragma endregion Public Function