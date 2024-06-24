//////////////////////////////////////////////////////////////////////////////////
///  @file   WindowsFileSystem.cpp
///  @brief  ファイル走査用の同期/非同期IO
///  @author toide
///  @date   2024/05/05 11:50:57
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "Platform/Windows/Include/WindowsFileSystem.hpp"
#include "Platform/Windows/Private/File/Include/WindowsFileHandle.hpp"
#include "Platform/Windows/Private/File/Include/WindowsFileHandleWithBuffering.hpp"
#include <Windows.h>
#include <winioctl.h>
#include <Shlwapi.h>

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
#pragma comment(lib, "shlwapi.lib")

using namespace platform::windows;
using namespace gu;
namespace
{
	static constexpr gu::uint32 INVALID_ID = static_cast<gu::uint32>(-1);

	// Windows.hで見つからなかったので, 自作で定義 
	typedef struct _REPARSE_DATA_BUFFER {
		ULONG  ReparseTag;
		USHORT ReparseDataLength;
		USHORT Reserved;
		union {
			struct {
				USHORT SubstituteNameOffset;
				USHORT SubstituteNameLength;
				USHORT PrintNameOffset;
				USHORT PrintNameLength;
				ULONG  Flags;
				WCHAR  PathBuffer[1];
			} SymbolicLinkReparseBuffer;
			struct {
				USHORT SubstituteNameOffset;
				USHORT SubstituteNameLength;
				USHORT PrintNameOffset;
				USHORT PrintNameLength;
				WCHAR  PathBuffer[1];
			} MountPointReparseBuffer;
			struct {
				UCHAR DataBuffer[1];
			} GenericReparseBuffer;
		} DUMMYUNIONNAME;
	} REPARSE_DATA_BUFFER, * PREPARSE_DATA_BUFFER;
}

bool IFileSystem::IS_LONG_PATH_ENABLED = false;

//////////////////////////////////////////////////////////////////////////////////
//                              UnDefine
//////////////////////////////////////////////////////////////////////////////////
#undef CreateDirectory
#undef DeleteDirectory
#undef DeleteFile
#undef CopyFile
#undef MoveFile
#undef CreateHardLink
#undef CreateSymbolicLink

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor
IFileSystem::IFileSystem() : core::IFileSystem()
{
	/*----------------------------------------------------------------------
	*         長いパスが有効かどうかを確認します.
	*----------------------------------------------------------------------*/
	HKEY keyHandle = NULL;

	// レジストリキーを開きます.
	auto result = ::RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"SYSTEM\\CurrentControlSet\\Control\\FileSystem", 0, KEY_READ, &keyHandle);
	
	if (result == ERROR_SUCCESS)
	{
		DWORD longPathEnabled = 0;
		DWORD size = sizeof(longPathEnabled);

		// 長いパスが有効かどうかを確認します.
		result = ::RegQueryValueExW(keyHandle, L"LongPathsEnabled", NULL, NULL, reinterpret_cast<LPBYTE>(&longPathEnabled), &size);
		if (result == ERROR_SUCCESS)
		{
			IS_LONG_PATH_ENABLED = longPathEnabled != 0;
		}

		// レジストリキーを閉じます.
		::RegCloseKey(keyHandle);
	}
}

IFileSystem::~IFileSystem()
{

}

#pragma endregion Constructor and Destructor

#pragma region Public Function
/*!**********************************************************************
*  @brief     ファイルを開いて読み込みます
*  @param[in] const tchar* ファイルパス
*  @param[in] const bool   ファイルの書き込みを行います.
*  @return    IFileHandle* ファイルのハンドル
*************************************************************************/
gu::SharedPointer<platform::core::file::IFileHandle> IFileSystem::OpenRead(const gu::tchar* fileName, const bool allowWrite, const bool useAsync)
{
	/*----------------------------------------------------------------------
	*         フラグ設定
	*----------------------------------------------------------------------*/
	const uint32 accessFlag = GENERIC_READ;
	const uint32 createFlag = OPEN_EXISTING;
	const uint32 shareFlag = FILE_SHARE_READ | (allowWrite ? FILE_SHARE_WRITE : 0);
	const uint32 asyncFlag = useAsync ? FILE_FLAG_OVERLAPPED : 0;

	/*----------------------------------------------------------------------
	*         ファイルを開きます.
	*----------------------------------------------------------------------*/
	const auto fileHandle = ::CreateFileW(fileName, accessFlag, shareFlag, NULL, createFlag, FILE_ATTRIBUTE_NORMAL | asyncFlag, NULL);
	if (fileHandle != INVALID_HANDLE_VALUE)
	{
		const auto result = gu::MakeShared<windows::file::IFileHandle>(fileHandle, accessFlag, createFlag, asyncFlag);

		return result->IsValid() ? result : nullptr;
	}
	else
	{
		return nullptr;
	}
}

/*!**********************************************************************
*  @brief     ファイルを開いて書き込みます.
*  @param[in] const tchar* ファイルパス
*  @param[in] const bool   既に書き込まれた部分を上書きせず, 後ろに追加するかを決めます (Default: 新規上書き)
*  @param[in] const bool   ファイルの閲覧も行います. (Default : false)
*  @return    IFileHandle* ファイルのハンドル
*************************************************************************/
gu::SharedPointer<platform::core::file::IFileHandle> IFileSystem::OpenWrite(const gu::tchar* fileName, const bool append, const bool allowRead, const bool useAsync)
{
	/*----------------------------------------------------------------------
	*         フラグ設定
	*----------------------------------------------------------------------*/
	const uint32 accessFlag = GENERIC_WRITE | (allowRead ? GENERIC_READ : 0);
	const uint32 createFlag = append ? OPEN_ALWAYS : CREATE_ALWAYS;
	const uint32 shareFlag  = (allowRead ? FILE_SHARE_READ : 0);
	const uint32 asyncFlag  = useAsync ? FILE_FLAG_OVERLAPPED : 0;

	/*----------------------------------------------------------------------
	*         ファイルを開きます.
	*----------------------------------------------------------------------*/
	const auto fileHandle = ::CreateFileW(fileName, accessFlag, shareFlag, NULL, createFlag, FILE_ATTRIBUTE_NORMAL | asyncFlag, NULL);
	if (fileHandle != INVALID_HANDLE_VALUE)
	{
		const auto result = gu::MakeShared<windows::file::IFileHandle>(fileHandle, accessFlag, createFlag, asyncFlag);
		
		if (result->IsValid())
		{
			// 上書きモードの場合は, ファイルの最後に移動することで後から追加できるようにします. 
			if (append) 
			{
				result->SeekFromEnd();
			}
			return result;
		}
		else
		{
			return nullptr;
		}
	}
	else
	{
		return nullptr;
	}
}


/*!**********************************************************************
*  @brief 指定したファイルが存在するかを確認します.
*  @param[in] const tchar* ファイルパス
*  @return    bool ファイルが存在する場合, trueを返します.
*************************************************************************/
bool IFileSystem::FileExists(const gu::tchar* filePath) const
{
	const auto result = ::GetFileAttributesW(filePath);

	return (result != INVALID_ID) && !(result & FILE_ATTRIBUTE_DIRECTORY);
}

/*!**********************************************************************
*  @brief 指定したファイルが存在するかを確認します.
*  @param[in] const tchar* ディレクトリ
*  @return    bool ファイルが存在する場合, trueを返します.
*************************************************************************/
bool IFileSystem::DirectoryExists(const gu::tchar* directory) const
{
	const auto result = ::GetFileAttributesW(directory);

	return (result != INVALID_ID) && (result & FILE_ATTRIBUTE_DIRECTORY);
}

/*!**********************************************************************
*  @brief     ディレクトリを作成します. 既にディレクトリが存在する場合もtrueを返します.
*  @param[in] const tchar* ディレクトリ
*  @return    bool ディレクトリを作成出来た場合, 既にディレクトリが存在する場合にtrueを返します
*************************************************************************/
bool IFileSystem::CreateDirectory(const gu::tchar* directory) const
{
	return (!!::CreateDirectoryW(directory, NULL)) || ::GetLastError() == ERROR_ALREADY_EXISTS;
}

/*!**********************************************************************
*  @brief     ディレクトリを削除します.
*  @param[in] const tchar* ディレクトリ
*  @return    bool ディレクトリを削除出来た場合にtrueを返します
*************************************************************************/
bool IFileSystem::DeleteDirectory(const gu::tchar* directory) const
{
	::RemoveDirectoryW(directory);

	const auto lastError = ::GetLastError();
	const bool succeeded = !DirectoryExists(directory);
	if (!succeeded)
	{
		::SetLastError(lastError);
	}
	return succeeded;
}

/*!**********************************************************************
*  @brief     ファイルを削除します.
*  @param[in] const tchar* ファイルパス
*  @return    bool ディレクトリを作成出来た場合にtrueを返します
*************************************************************************/
bool IFileSystem::DeleteFile(const gu::tchar* filePath) const
{
	return !!::DeleteFileW(filePath);
}

/*!**********************************************************************
*  @brief     ファイルを移動します
*  @param[in] const tchar* 移動前のファイルパス
*  @param[in] const tchar* 移動後のファイルパス
*  @param[in] const bool   上書きを許可するかを設定します
*  @return    bool ファイルが移動できた場合にtrueを返します
*************************************************************************/
bool IFileSystem::CopyFile(const gu::tchar* from, const gu::tchar* to, const bool overwrite) const
{
	return !!::CopyFileW(from, to, !overwrite);
}

/*!**********************************************************************
*  @brief     ファイルを移動します
*  @param[in] const tchar* 移動前のファイルパス
*  @param[in] const tchar* 移動後のファイルパス
*  @return    bool ファイルが移動できた場合にtrueを返します
*************************************************************************/
bool IFileSystem::MoveFile(const gu::tchar* from, const gu::tchar* to) const
{
	return !!::MoveFileW(from, to);
}

/*!**********************************************************************
*  @brief     読み取りだけしか出来ないファイルかどうか
*  @param[in] const tchar* ファイルパス
*  @return    bool Readonlyであればtrue
*************************************************************************/
bool IFileSystem::IsReadOnly(const gu::tchar* filePath) const
{
	const auto result = ::GetFileAttributesW(filePath);

	return (result != INVALID_ID) && (result & FILE_ATTRIBUTE_READONLY);
}

/*!**********************************************************************
*  @brief     ファイル, もしくはディレクトリのリンク(参照)の種類を返します.
*  @param[in] const tchar* ファイルパス, もしくはディレクトリ
*  @return    file::LinkType リンクの種類
*************************************************************************/
platform::core::file::LinkType IFileSystem::LinkType(const tchar* path) const
{
	using enum platform::core::file::LinkType;

	/*-------------------------------------------------------------------
	-        ディレクトリかファイルかを判定します.
	---------------------------------------------------------------------*/
	const auto attribute = ::GetFileAttributesW(path);

	if (attribute == INVALID_FILE_ATTRIBUTES)
	{
		return NotExist;
	}

	DWORD desiredAccess = NULL;
	DWORD flagsAndAttributes = NULL;

	// ディレクトリ
	if (attribute & FILE_ATTRIBUTE_DIRECTORY)
	{
		desiredAccess = FILE_LIST_DIRECTORY;
		flagsAndAttributes = FILE_FLAG_BACKUP_SEMANTICS;
	}
	// ファイル
	else
	{
		desiredAccess = GENERIC_READ;
		flagsAndAttributes = FILE_ATTRIBUTE_NORMAL;
	}

	const auto fileHandle = ::CreateFileW(path, desiredAccess, FILE_SHARE_READ, NULL, OPEN_EXISTING, flagsAndAttributes, NULL);

	/*-------------------------------------------------------------------
	-        ファイルが存在しない
	---------------------------------------------------------------------*/
	if (fileHandle == INVALID_HANDLE_VALUE)
	{
		return NotExist;
	}

	/*-------------------------------------------------------------------
	-        ファイル情報を取得して, ハードリンクです
	---------------------------------------------------------------------*/
	BY_HANDLE_FILE_INFORMATION fileInfo = {};
	if (!::GetFileInformationByHandle(fileHandle, &fileInfo))
	{
		::CloseHandle(fileHandle);
		return NotExist;
	}

	/*-------------------------------------------------------------------
	-        リンクの種類の判別
	---------------------------------------------------------------------*/
	// シンボリックリンク or ジャンクション
	if (attribute & FILE_ATTRIBUTE_REPARSE_POINT)
	{
		gu::int8* buffer = new gu::int8[MAXIMUM_REPARSE_DATA_BUFFER_SIZE];

		DWORD dwRet = NULL;
		if (!DeviceIoControl(fileHandle, FSCTL_GET_REPARSE_POINT, NULL, 0, buffer, sizeof(buffer), &dwRet, NULL))
		{
			::OutputDebugStringW(L"Unable to get reparse point information");
			::CloseHandle(fileHandle);
			delete[] buffer;
			return Unknown;
		}

		// シンボリックリンクやジャンクションの確認
		REPARSE_DATA_BUFFER* reparseBuffer = (REPARSE_DATA_BUFFER*)buffer;

		platform::core::file::LinkType result = Unused;
		if (reparseBuffer->ReparseTag == IO_REPARSE_TAG_MOUNT_POINT)
		{
			result = Junction;
		}
		else if (reparseBuffer->ReparseTag == IO_REPARSE_TAG_SYMLINK)
		{
			result = Symbolic;
		}

		delete[] buffer;
		::CloseHandle(fileHandle);
		return result;
	}

	// ハードリンク
	if (fileInfo.nNumberOfLinks > 1)
	{
		return Hard;
	}
	else
	{
		return Unused;
	}

}

/*!**********************************************************************
*  @brief     指定したファイルパスの拡張子を取得します
*  @param[in] const tchar* ファイルパス
*  @return    const tchar* 拡張子です. (見つからない場合はnullptrを返します)
*************************************************************************/
gu::tstring IFileSystem::GetExtension(const gu::tchar* filePath) const
{
	return gu::tstring(::PathFindExtensionW(filePath));
}

/*!**********************************************************************
*  @brief     ディレクトリ(ファイル名の手前まで)を取得します.
*  @param[in] const tchar* ファイルパス
*  @return    const char* ファイル名手前までのディレクトリ
*************************************************************************/
gu::tstring IFileSystem::GetDirectory(const gu::tchar* filePath) const
{
	const auto stringPath = tstring(filePath);
	const auto index      = stringPath.ReverseFind(SP("/"));
	return stringPath.SubString(0, index + 1);
}

/*!**********************************************************************
*  @brief     自身のプロセスに関する作業ディレクトリを取得します.
*  @param[in] void
*  @return    gu::tstring 作業ディレクトリ
*************************************************************************/
gu::tstring IFileSystem::GetProcessDirectory() const
{
	if (IS_LONG_PATH_ENABLED)
	{
		static constexpr DWORD MAX_LONG_PATH_LENGTH = 32767;
		thread_local static gu::tchar buffer[MAX_LONG_PATH_LENGTH] = {};
		
		const auto charLength = ::GetCurrentDirectoryW(MAX_LONG_PATH_LENGTH, buffer);
		return gu::tstring(buffer, charLength);
	}
	else
	{
		thread_local static gu::tchar buffer[MAX_PATH] = {};
		::GetCurrentDirectoryW(MAX_PATH, buffer);

		const auto charLength = ::GetCurrentDirectoryW(MAX_PATH, buffer);
		return gu::tstring(buffer, charLength);
	}
}

/*!**********************************************************************
*  @brief     対象のディレクトリ直下にハードリンクを作成します. @n
*             ハードリンクは, ファイルの内容を共有するファイルのことです.
*  @param[in] const tchar* リンクを行いたい元のファイルパス
*  @param[in] const tchar* リンクを作成したいファイルパス
*  @return    bool
*************************************************************************/
bool IFileSystem::CreateHardLink(const gu::tchar* sourcePath, const gu::tchar* newHardLinkPath) const
{
	return !!::CreateHardLinkW(newHardLinkPath, sourcePath, NULL);
}

/*!**********************************************************************
*  @brief     対象のディレクトリ直下にシンボリックリンクを作成します. @n
*             シンボリックリンクは, ファイルのパスを参照するファイルのことです.
*  @param[in] const tchar* リンクを行いたい元のファイルパス
*  @param[in] const tchar* リンクを作成したいファイルパス
*  @return    bool
*************************************************************************/
bool IFileSystem::CreateSymbolicLink(const gu::tchar* sourcePath, const gu::tchar* newSymbolicLinkPath) const
{
	return !!::CreateSymbolicLinkW(newSymbolicLinkPath, sourcePath, 0);
}
#pragma endregion Public Function
