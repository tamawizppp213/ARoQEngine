//////////////////////////////////////////////////////////////////////////////////
///  @file   WindowsFileSystem.cpp
///  @brief  �t�@�C�������p�̓���/�񓯊�IO
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

	// Windows.h�Ō�����Ȃ������̂�, ����Œ�` 
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
	*         �����p�X���L�����ǂ������m�F���܂�.
	*----------------------------------------------------------------------*/
	HKEY keyHandle = NULL;

	// ���W�X�g���L�[���J���܂�.
	auto result = ::RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"SYSTEM\\CurrentControlSet\\Control\\FileSystem", 0, KEY_READ, &keyHandle);
	
	if (result == ERROR_SUCCESS)
	{
		DWORD longPathEnabled = 0;
		DWORD size = sizeof(longPathEnabled);

		// �����p�X���L�����ǂ������m�F���܂�.
		result = ::RegQueryValueExW(keyHandle, L"LongPathsEnabled", NULL, NULL, reinterpret_cast<LPBYTE>(&longPathEnabled), &size);
		if (result == ERROR_SUCCESS)
		{
			IS_LONG_PATH_ENABLED = longPathEnabled != 0;
		}

		// ���W�X�g���L�[����܂�.
		::RegCloseKey(keyHandle);
	}
}

IFileSystem::~IFileSystem()
{

}

#pragma endregion Constructor and Destructor

#pragma region Public Function
/*!**********************************************************************
*  @brief     �t�@�C�����J���ēǂݍ��݂܂�
*  @param[in] const tchar* �t�@�C���p�X
*  @param[in] const bool   �t�@�C���̏������݂��s���܂�.
*  @return    IFileHandle* �t�@�C���̃n���h��
*************************************************************************/
gu::SharedPointer<platform::core::file::IFileHandle> IFileSystem::OpenRead(const gu::tchar* fileName, const bool allowWrite, const bool useAsync)
{
	/*----------------------------------------------------------------------
	*         �t���O�ݒ�
	*----------------------------------------------------------------------*/
	const uint32 accessFlag = GENERIC_READ;
	const uint32 createFlag = OPEN_EXISTING;
	const uint32 shareFlag = FILE_SHARE_READ | (allowWrite ? FILE_SHARE_WRITE : 0);
	const uint32 asyncFlag = useAsync ? FILE_FLAG_OVERLAPPED : 0;

	/*----------------------------------------------------------------------
	*         �t�@�C�����J���܂�.
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
*  @brief     �t�@�C�����J���ď������݂܂�.
*  @param[in] const tchar* �t�@�C���p�X
*  @param[in] const bool   ���ɏ������܂ꂽ�������㏑������, ���ɒǉ����邩�����߂܂� (Default: �V�K�㏑��)
*  @param[in] const bool   �t�@�C���̉{�����s���܂�. (Default : false)
*  @return    IFileHandle* �t�@�C���̃n���h��
*************************************************************************/
gu::SharedPointer<platform::core::file::IFileHandle> IFileSystem::OpenWrite(const gu::tchar* fileName, const bool append, const bool allowRead, const bool useAsync)
{
	/*----------------------------------------------------------------------
	*         �t���O�ݒ�
	*----------------------------------------------------------------------*/
	const uint32 accessFlag = GENERIC_WRITE | (allowRead ? GENERIC_READ : 0);
	const uint32 createFlag = append ? OPEN_ALWAYS : CREATE_ALWAYS;
	const uint32 shareFlag  = (allowRead ? FILE_SHARE_READ : 0);
	const uint32 asyncFlag  = useAsync ? FILE_FLAG_OVERLAPPED : 0;

	/*----------------------------------------------------------------------
	*         �t�@�C�����J���܂�.
	*----------------------------------------------------------------------*/
	const auto fileHandle = ::CreateFileW(fileName, accessFlag, shareFlag, NULL, createFlag, FILE_ATTRIBUTE_NORMAL | asyncFlag, NULL);
	if (fileHandle != INVALID_HANDLE_VALUE)
	{
		const auto result = gu::MakeShared<windows::file::IFileHandle>(fileHandle, accessFlag, createFlag, asyncFlag);
		
		if (result->IsValid())
		{
			// �㏑�����[�h�̏ꍇ��, �t�@�C���̍Ō�Ɉړ����邱�ƂŌォ��ǉ��ł���悤�ɂ��܂�. 
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
*  @brief �w�肵���t�@�C�������݂��邩���m�F���܂�.
*  @param[in] const tchar* �t�@�C���p�X
*  @return    bool �t�@�C�������݂���ꍇ, true��Ԃ��܂�.
*************************************************************************/
bool IFileSystem::FileExists(const gu::tchar* filePath) const
{
	const auto result = ::GetFileAttributesW(filePath);

	return (result != INVALID_ID) && !(result & FILE_ATTRIBUTE_DIRECTORY);
}

/*!**********************************************************************
*  @brief �w�肵���t�@�C�������݂��邩���m�F���܂�.
*  @param[in] const tchar* �f�B���N�g��
*  @return    bool �t�@�C�������݂���ꍇ, true��Ԃ��܂�.
*************************************************************************/
bool IFileSystem::DirectoryExists(const gu::tchar* directory) const
{
	const auto result = ::GetFileAttributesW(directory);

	return (result != INVALID_ID) && (result & FILE_ATTRIBUTE_DIRECTORY);
}

/*!**********************************************************************
*  @brief     �f�B���N�g�����쐬���܂�. ���Ƀf�B���N�g�������݂���ꍇ��true��Ԃ��܂�.
*  @param[in] const tchar* �f�B���N�g��
*  @return    bool �f�B���N�g�����쐬�o�����ꍇ, ���Ƀf�B���N�g�������݂���ꍇ��true��Ԃ��܂�
*************************************************************************/
bool IFileSystem::CreateDirectory(const gu::tchar* directory) const
{
	return (!!::CreateDirectoryW(directory, NULL)) || ::GetLastError() == ERROR_ALREADY_EXISTS;
}

/*!**********************************************************************
*  @brief     �f�B���N�g�����폜���܂�.
*  @param[in] const tchar* �f�B���N�g��
*  @return    bool �f�B���N�g�����폜�o�����ꍇ��true��Ԃ��܂�
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
*  @brief     �t�@�C�����폜���܂�.
*  @param[in] const tchar* �t�@�C���p�X
*  @return    bool �f�B���N�g�����쐬�o�����ꍇ��true��Ԃ��܂�
*************************************************************************/
bool IFileSystem::DeleteFile(const gu::tchar* filePath) const
{
	return !!::DeleteFileW(filePath);
}

/*!**********************************************************************
*  @brief     �t�@�C�����ړ����܂�
*  @param[in] const tchar* �ړ��O�̃t�@�C���p�X
*  @param[in] const tchar* �ړ���̃t�@�C���p�X
*  @param[in] const bool   �㏑���������邩��ݒ肵�܂�
*  @return    bool �t�@�C�����ړ��ł����ꍇ��true��Ԃ��܂�
*************************************************************************/
bool IFileSystem::CopyFile(const gu::tchar* from, const gu::tchar* to, const bool overwrite) const
{
	return !!::CopyFileW(from, to, !overwrite);
}

/*!**********************************************************************
*  @brief     �t�@�C�����ړ����܂�
*  @param[in] const tchar* �ړ��O�̃t�@�C���p�X
*  @param[in] const tchar* �ړ���̃t�@�C���p�X
*  @return    bool �t�@�C�����ړ��ł����ꍇ��true��Ԃ��܂�
*************************************************************************/
bool IFileSystem::MoveFile(const gu::tchar* from, const gu::tchar* to) const
{
	return !!::MoveFileW(from, to);
}

/*!**********************************************************************
*  @brief     �ǂݎ�肾�������o���Ȃ��t�@�C�����ǂ���
*  @param[in] const tchar* �t�@�C���p�X
*  @return    bool Readonly�ł����true
*************************************************************************/
bool IFileSystem::IsReadOnly(const gu::tchar* filePath) const
{
	const auto result = ::GetFileAttributesW(filePath);

	return (result != INVALID_ID) && (result & FILE_ATTRIBUTE_READONLY);
}

/*!**********************************************************************
*  @brief     �t�@�C��, �������̓f�B���N�g���̃����N(�Q��)�̎�ނ�Ԃ��܂�.
*  @param[in] const tchar* �t�@�C���p�X, �������̓f�B���N�g��
*  @return    file::LinkType �����N�̎��
*************************************************************************/
platform::core::file::LinkType IFileSystem::LinkType(const tchar* path) const
{
	using enum platform::core::file::LinkType;

	/*-------------------------------------------------------------------
	-        �f�B���N�g�����t�@�C�����𔻒肵�܂�.
	---------------------------------------------------------------------*/
	const auto attribute = ::GetFileAttributesW(path);

	if (attribute == INVALID_FILE_ATTRIBUTES)
	{
		return NotExist;
	}

	DWORD desiredAccess = NULL;
	DWORD flagsAndAttributes = NULL;

	// �f�B���N�g��
	if (attribute & FILE_ATTRIBUTE_DIRECTORY)
	{
		desiredAccess = FILE_LIST_DIRECTORY;
		flagsAndAttributes = FILE_FLAG_BACKUP_SEMANTICS;
	}
	// �t�@�C��
	else
	{
		desiredAccess = GENERIC_READ;
		flagsAndAttributes = FILE_ATTRIBUTE_NORMAL;
	}

	const auto fileHandle = ::CreateFileW(path, desiredAccess, FILE_SHARE_READ, NULL, OPEN_EXISTING, flagsAndAttributes, NULL);

	/*-------------------------------------------------------------------
	-        �t�@�C�������݂��Ȃ�
	---------------------------------------------------------------------*/
	if (fileHandle == INVALID_HANDLE_VALUE)
	{
		return NotExist;
	}

	/*-------------------------------------------------------------------
	-        �t�@�C�������擾����, �n�[�h�����N�ł�
	---------------------------------------------------------------------*/
	BY_HANDLE_FILE_INFORMATION fileInfo = {};
	if (!::GetFileInformationByHandle(fileHandle, &fileInfo))
	{
		::CloseHandle(fileHandle);
		return NotExist;
	}

	/*-------------------------------------------------------------------
	-        �����N�̎�ނ̔���
	---------------------------------------------------------------------*/
	// �V���{���b�N�����N or �W�����N�V����
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

		// �V���{���b�N�����N��W�����N�V�����̊m�F
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

	// �n�[�h�����N
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
*  @brief     �w�肵���t�@�C���p�X�̊g���q���擾���܂�
*  @param[in] const tchar* �t�@�C���p�X
*  @return    const tchar* �g���q�ł�. (������Ȃ��ꍇ��nullptr��Ԃ��܂�)
*************************************************************************/
gu::tstring IFileSystem::GetExtension(const gu::tchar* filePath) const
{
	return gu::tstring(::PathFindExtensionW(filePath));
}

/*!**********************************************************************
*  @brief     �f�B���N�g��(�t�@�C�����̎�O�܂�)���擾���܂�.
*  @param[in] const tchar* �t�@�C���p�X
*  @return    const char* �t�@�C������O�܂ł̃f�B���N�g��
*************************************************************************/
gu::tstring IFileSystem::GetDirectory(const gu::tchar* filePath) const
{
	const auto stringPath = tstring(filePath);
	const auto index      = stringPath.ReverseFind(SP("/"));
	return stringPath.SubString(0, index + 1);
}

/*!**********************************************************************
*  @brief     ���g�̃v���Z�X�Ɋւ����ƃf�B���N�g�����擾���܂�.
*  @param[in] void
*  @return    gu::tstring ��ƃf�B���N�g��
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
*  @brief     �Ώۂ̃f�B���N�g�������Ƀn�[�h�����N���쐬���܂�. @n
*             �n�[�h�����N��, �t�@�C���̓��e�����L����t�@�C���̂��Ƃł�.
*  @param[in] const tchar* �����N���s���������̃t�@�C���p�X
*  @param[in] const tchar* �����N���쐬�������t�@�C���p�X
*  @return    bool
*************************************************************************/
bool IFileSystem::CreateHardLink(const gu::tchar* sourcePath, const gu::tchar* newHardLinkPath) const
{
	return !!::CreateHardLinkW(newHardLinkPath, sourcePath, NULL);
}

/*!**********************************************************************
*  @brief     �Ώۂ̃f�B���N�g�������ɃV���{���b�N�����N���쐬���܂�. @n
*             �V���{���b�N�����N��, �t�@�C���̃p�X���Q�Ƃ���t�@�C���̂��Ƃł�.
*  @param[in] const tchar* �����N���s���������̃t�@�C���p�X
*  @param[in] const tchar* �����N���쐬�������t�@�C���p�X
*  @return    bool
*************************************************************************/
bool IFileSystem::CreateSymbolicLink(const gu::tchar* sourcePath, const gu::tchar* newSymbolicLinkPath) const
{
	return !!::CreateSymbolicLinkW(newSymbolicLinkPath, sourcePath, 0);
}
#pragma endregion Public Function
