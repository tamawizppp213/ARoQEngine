//////////////////////////////////////////////////////////////////////////////////
///  @file   WindowsProcess.cpp
///  @brief  temp
///  @author toide
///  @date   2024/06/15 22:48:34
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "Platform/Windows/Include/WindowsProcess.hpp"
#include "GameUtility/Base/Include/GUString.hpp"
#include "GameUtility/Container/Include/GUDynamicArray.hpp"
#include <Windows.h>
#include <TlHelp32.h>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace platform::windows;
using namespace gu;
using namespace platform;

namespace
{
	bool   g_IsJobObjectSet = false;
	HANDLE g_JobHandle      = nullptr;
}

#undef CreateProcess

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor

#pragma endregion Constructor and Destructor

#pragma region Public Function
/*!**********************************************************************
*  @brief     �w�肳�ꂽ�t�@�C������DLL�����[�h��, ���̃n���h����Ԃ��܂�. @n
*             https://qiita.com/tera1707/items/727e7ad4dc9f3205c137
*  @param[in] const tstring& fiieName : ���[�h������DLL�̃t�@�C����
*  @return    void* : ���[�h����DLL�̃n���h��
*************************************************************************/
void* CoreProcess::GetDLLHandle(const gu::tstring& fileName) const
{
	return ::LoadLibraryW(fileName.CString());
}

/*!**********************************************************************
*  @brief     �w�肳�ꂽ�t�@�C������DLL�����������������܂�
*  @param[in] void* handle : ���[�h����DLL�̃n���h��
*  @return    bool ����
*************************************************************************/
bool CoreProcess::FreeDLLHandle(void* handle) const
{
	return !!::FreeLibrary((HMODULE)handle);
}

/*!**********************************************************************
*  @brief     �{�v���Z�X�̃v���Z�XID��Ԃ��܂�.
*  @param[in] void
*  @return    uint32 �v���Z�XID
*************************************************************************/
gu::uint32 CoreProcess::GetCurrentProcessID() const
{
	return ::GetCurrentProcessId();
}

/*!**********************************************************************
*  @brief     ���̊֐��̌Ăяo�����Ɍ��݂̃X���b�h�����s����Ă����v���Z�b�T�̐�(���ł͂���܂���)���擾
*  @param[in] void
*  @return    uint32 �v���Z�b�T��
*************************************************************************/
gu::uint32 CoreProcess::GetCurrentProcessorNumber() const
{
#if _WIN32_WINNT >= 0x0601
	PROCESSOR_NUMBER processorNumber = {};
	::GetCurrentProcessorNumberEx(&processorNumber);
	return (processorNumber.Group << 6) + processorNumber.Number;
#else
	return ::GetCurrentProcessorNumber();
#endif
}

/*!**********************************************************************
*  @brief     OS���ǂ̃v���Z�b�T�ŃX���b�h, �v���Z�X�����s����ׂ�����ݒ肵�܂�
*  @param[in] const uint64 affinity : �v���Z�b�T�̃r�b�g�}�X�N
*  @return    bool ����
*************************************************************************/
bool CoreProcess::SetProcessAffinity(const gu::uint64 affnity) const
{
	return !!::SetProcessAffinityMask(::GetCurrentProcess(), affnity);
}

/*!**********************************************************************
*  @brief     Process�̐�����Primary�X���b�h���쐬���邽�߂̊֐�. �Ăяo�����v���Z�X�̃Z�L�����e�B�E�R���e�L�X�g���Ŏw�肳�ꂽ���s�t�@�C�����N�����܂�
*  @param[in] const tstring& : ���s����v���O�����̃p�X
*  @param[in] const tstring& : �v���O�����ɓn���p�����[�^
*  @param[in] const bool     : �v���Z�X���f�^�b�`���Ď��s���邩�ǂ���, �V�����v���Z�X���Ǝ��̃E�B���h�E�ŋN�����܂�
*  @param[in] const bool     : �v���Z�X���\���Ŏ��s���邩�ǂ���, �^�X�N�o�[�ōŏ�������܂�.
*  @param[out] uint32*       : �v���Z�XID
*  @param[in] const int32    : �v���Z�X�̗D��x 
*  @return    bool
*************************************************************************/
core::ProcessHandle CoreProcess::CreateProcess(const gu::tstring& programPath, const gu::tstring& parameters,
	const bool launchDetached, const core::ProcessWindowActivate activate,
	gu::uint32* outProcessID, const core::ProcessPriority priority) const
{
	/*-------------------------------------------------------------------
	-            �v���Z�X�쐬���̏������D��x�ݒ�
	---------------------------------------------------------------------*/
	uint32 creationFlags = NORMAL_PRIORITY_CLASS;
	switch (priority)
	{
		case core::ProcessPriority::Idle:
		{
			creationFlags = IDLE_PRIORITY_CLASS;
			break;
		}
		case core::ProcessPriority::Low:
		{
			creationFlags = BELOW_NORMAL_PRIORITY_CLASS;
			break;
		}
		case core::ProcessPriority::Normal:
		{
			creationFlags = NORMAL_PRIORITY_CLASS;
			break;
		}
		case core::ProcessPriority::AboveNormal:
		{
			creationFlags = ABOVE_NORMAL_PRIORITY_CLASS;
			break;
		}
		case core::ProcessPriority::High:
		{
			creationFlags = HIGH_PRIORITY_CLASS;
			break;
		}
		case core::ProcessPriority::RealTime:
		{
			creationFlags = REALTIME_PRIORITY_CLASS;
			break;
		}
		default:
		{
			break;
		}
	}

	if (launchDetached)
	{
		creationFlags |= DETACHED_PROCESS;
	}

	/*-------------------------------------------------------------------
	-            Window�쐬�ɂ�����E�B���h�E�t���O
	---------------------------------------------------------------------*/
	uint32 windowFlags = 0;
	uint16 showWindow  = SW_HIDE;

	if (activate == core::ProcessWindowActivate::ReallyHidden)
	{
		windowFlags = STARTF_USESHOWWINDOW;
	}
	else if (activate == core::ProcessWindowActivate::Hidden)
	{
		windowFlags = STARTF_USESHOWWINDOW;
		showWindow  = SW_SHOWMINNOACTIVE;
	}
	else
	{
		showWindow = SW_SHOW;
	}

	/*-------------------------------------------------------------------
	-            Start up Info�̏�����
	---------------------------------------------------------------------*/
	STARTUPINFO startupInfo = 
	{
		sizeof(STARTUPINFO), 
		NULL,
		NULL,                   // �f�X�N�g�b�v�̖��O
		NULL,                   // �R���\�[���v���Z�X�̃^�C�g��
		(::DWORD)CW_USEDEFAULT, // X
		(::DWORD)CW_USEDEFAULT, // Y
		(::DWORD)CW_USEDEFAULT, // Width
		(::DWORD)CW_USEDEFAULT, // Height
		(::DWORD)0, (::DWORD)0, (::DWORD)0,
		(::DWORD)windowFlags,
		showWindow,
		0, 
		NULL,
		NULL,
		NULL,
		NULL
	};

	// �R�}���h���C���̍쐬
	tstring commandLine = tstring(programPath);

	if (parameters != nullptr)
	{
		commandLine += SP(' ');
		commandLine += tstring(parameters);
	}

	/*-------------------------------------------------------------------
	-            Start up Info�̏�����
	---------------------------------------------------------------------*/
	PROCESS_INFORMATION processInfo;

	if (!::CreateProcessW(NULL, commandLine.CString(), nullptr, nullptr, FALSE, (::DWORD)windowFlags, NULL, NULL, &startupInfo, &processInfo))
	{
		printf("CreateProcess failed (%d).\n", GetLastError());

		if (outProcessID != nullptr)
		{
			*outProcessID = 0;
		}

		return core::ProcessHandle();
	}

	if (outProcessID != nullptr)
	{
		*outProcessID = processInfo.dwProcessId;
	}

	::CloseHandle(processInfo.hThread);

	return core::ProcessHandle(processInfo.hProcess);
}

/*!**********************************************************************
*  @brief     ���ɍ쐬���ꂽ�v���Z�X�n���h�����J���܂�
*  @param[in] const gu::uint32 processID : �v���Z�XID
*  @return    ProcessHandle : �v���Z�X�n���h��
*************************************************************************/
core::ProcessHandle CoreProcess::OpenProcessHandle(const gu::uint32 processID) const
{
	return core::ProcessHandle(::OpenProcess(PROCESS_ALL_ACCESS, FALSE, processID));
}

/*!**********************************************************************
*  @brief     �v���Z�X�����, ���\�[�X��������邽�߂Ɏg�p���܂�. �v���Z�X
*  @param[in] const gu::uint32 processID : �v���Z�XID
*  @return    void
*************************************************************************/
void CoreProcess::CloseProcessHandle(core::ProcessHandle& handle) const
{
	if (handle.IsValid())
	{
		::CloseHandle(handle.GetHandle());
		handle.Reset();
	}
}

/*!**********************************************************************
*  @brief     �v���Z�X�������I�����܂�. ���̍�, killTree��true�̏ꍇ, �q�v���Z�X���I�����܂�
*  @param[in] ProcessHandle& : �v���Z�X�n���h��
*  @param[in] const bool : �q�v���Z�X���I�����邩�ǂ���
*  @return    void
*************************************************************************/
void CoreProcess::TerminateProcess(core::ProcessHandle& handle, const bool killTree) const
{
	if (killTree)
	{
		::TerminateProcess(handle.GetHandle(), 0);
	}
	else
	{
		::TerminateProcess(handle.GetHandle(), 0);
	}
}

/*!**********************************************************************
*  @brief      �v���Z�X�����s����. ���^�[���E�R�[�h, �W���o��, �W���G���[��Ԃ�. ���̌Ăяo���̓v���Z�X���߂�܂Ńu���b�N�����.
*  @param[in]  const tstring&  : ���s����v���O�����̃p�X
*  @param[in]  const tstring&  : �v���O�����ɓn���p�����[�^
*  @param[out] int32*          : return�R�[�h
*  @param[out] const tstring*  : �W���o��
*  @param[out] const tstring*  : �W���G���[
*  @param[in]  const tstring*  : �I�v�V������Working�f�B���N�g��
*  @param[in]  const bool      : �e�v���Z�X�ƈꏏ�Ƀv���Z�X���I������悤�ɂ���ɂ�true�ɂ��܂��B
*  @return     bool ����
*************************************************************************/
bool CoreProcess::ExecuteProcess(const gu::tstring& programPath, const gu::tstring& parameters, gu::int32* outReturnCode, gu::tstring* stdOutput, gu::tstring* stdError, gu::tstring* optionWorkingDirectory, const bool shouldEndWithParentProcess)
{
	/*-------------------------------------------------------------------
	-            �W���u�I�u�W�F�N�g�̐ݒ�
	---------------------------------------------------------------------*/
	if (shouldEndWithParentProcess && !g_IsJobObjectSet)
	{
		g_JobHandle = CreateJobObjectW(NULL, NULL);

		if (g_JobHandle == NULL)
		{
			printf("Failed to create job object (%d).\n", GetLastError());
		}
		else
		{
			JOBOBJECT_EXTENDED_LIMIT_INFORMATION limitInformation = {};
			Memory::Zero(&limitInformation, sizeof(JOBOBJECT_EXTENDED_LIMIT_INFORMATION));

			limitInformation.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;
			if(!SetInformationJobObject(g_JobHandle, JobObjectExtendedLimitInformation, &limitInformation, sizeof(JOBOBJECT_EXTENDED_LIMIT_INFORMATION)))
			{
				printf("Failed to set information job object (%d).\n", GetLastError());
			}
		}

		g_IsJobObjectSet = true;
	}

	/*-------------------------------------------------------------------
	-           �E�B���h�E�N�����̐ݒ� (�^�X�N�o�[�ɕ\��)
	---------------------------------------------------------------------*/
	STARTUPINFOEX startupInfo = {};
	Memory::Zero(&startupInfo, sizeof(STARTUPINFOEX));
	startupInfo.StartupInfo.cb          = sizeof(STARTUPINFOEX);
	startupInfo.StartupInfo.dwX         = CW_USEDEFAULT;
	startupInfo.StartupInfo.dwY         = CW_USEDEFAULT;
	startupInfo.StartupInfo.dwXSize     = CW_USEDEFAULT;
	startupInfo.StartupInfo.dwYSize     = CW_USEDEFAULT;
	startupInfo.StartupInfo.dwFlags     = STARTF_USESHOWWINDOW;
	startupInfo.StartupInfo.wShowWindow = SW_SHOWMINNOACTIVE;
	startupInfo.StartupInfo.hStdInput   = ::GetStdHandle(STD_INPUT_HANDLE);

	/*-------------------------------------------------------------------
	-          �W���o�͂ƕW���G���[�̃p�C�v�̐ݒ�
	---------------------------------------------------------------------*/
	HANDLE stdOutRead   = nullptr;
	HANDLE stdErrorRead = nullptr;
	
	if (stdOutput != nullptr || stdError != nullptr)
	{
		//�W���n���h�����g�p
		startupInfo.StartupInfo.dwFlags |= STARTF_USESTDHANDLES;

		// �쐬�����p�C�v���q�v���Z�X�Ɍp�������悤�ɃZ�L�����e�B������ݒ�
		SECURITY_ATTRIBUTES securityAttributes = {};
		Memory::Zero(&securityAttributes, sizeof(SECURITY_ATTRIBUTES));
		securityAttributes.nLength        = sizeof(SECURITY_ATTRIBUTES);
		securityAttributes.bInheritHandle = TRUE;

		Confirm(::CreatePipe(&stdOutRead  , &startupInfo.StartupInfo.hStdOutput, &securityAttributes, 0));
		Confirm(::CreatePipe(&stdErrorRead, &startupInfo.StartupInfo.hStdError , &securityAttributes, 0));

		DynamicArray<uint8> attributeList = {};
		uint64 bufferSize = 0;

		// �o�b�t�@�T�C�Y�̌��� InitializeProcThreadAttributeList�͊�{False���Ԃ�܂�(�d�l)
		if(!InitializeProcThreadAttributeList(NULL, 1, 0, &bufferSize) && GetLastError() == ERROR_INSUFFICIENT_BUFFER && bufferSize)
		{
			attributeList.Resize(bufferSize);
			startupInfo.lpAttributeList = (LPPROC_THREAD_ATTRIBUTE_LIST)attributeList.Data();
			Confirm(InitializeProcThreadAttributeList(startupInfo.lpAttributeList, 1, 0, &bufferSize));

			if (startupInfo.lpAttributeList != NULL)
			{
				HANDLE inheritHandles[] = { startupInfo.StartupInfo.hStdOutput, startupInfo.StartupInfo.hStdError };
				Confirm(UpdateProcThreadAttribute(startupInfo.lpAttributeList, 0, PROC_THREAD_ATTRIBUTE_HANDLE_LIST, inheritHandles, sizeof(inheritHandles), NULL, NULL));
			}
		}
	}

	/*-------------------------------------------------------------------
	-          �R�}���h���C���̍쐬
	---------------------------------------------------------------------*/
	bool result = false;
	tstring commandLine = tstring(programPath);

	if (parameters != nullptr)
	{
		commandLine += SP(' ');
		commandLine += tstring(parameters);
	}

	uint32 createFlags = NORMAL_PRIORITY_CLASS | DETACHED_PROCESS;
	if(startupInfo.lpAttributeList != NULL)
	{
		createFlags |= EXTENDED_STARTUPINFO_PRESENT;
	}

	/*-------------------------------------------------------------------
	-          �v���Z�X�̍쐬
	---------------------------------------------------------------------*/
	PROCESS_INFORMATION processInformation = {};
	if (CreateProcessW(NULL, commandLine.CString(), NULL, NULL, TRUE, createFlags, NULL, optionWorkingDirectory->CString(), &startupInfo.StartupInfo, &processInformation))
	{
		if (shouldEndWithParentProcess && g_JobHandle)
		{
			if (!AssignProcessToJobObject(g_JobHandle, processInformation.hProcess))
			{
				printf("Failed to assign process to job object (%d).\n", GetLastError());
			}
		}

		if (stdOutRead != NULL)
		{
			HANDLE   readHandles[2] = { stdOutRead, stdErrorRead };
			tstring* outStrings[2]  = { stdOutput, stdError };
			DynamicArray<uint8> pipeBuffer[2] = {};

			auto ReadPipes = [&]()
			{
				for (int32 i = 0; i < 2; ++i)
				{
					if (readHandles[i] && outStrings[i])
					{
						DynamicArray<uint8> binaryBuffer = {};
						ReadPipe(readHandles[i], binaryBuffer);
						pipeBuffer[i].Reserve(pipeBuffer[i].Size() + binaryBuffer.Size());
						for(uint64 j = 0; j < binaryBuffer.Size(); ++j)
						{
							pipeBuffer[i].Push(binaryBuffer[j]); // Append�֐�����čœK�����������ǂ�
						}
					}
				}
			};

			core::ProcessHandle processHandle(processInformation.hProcess);
			do
			{
				ReadPipes();
				::Sleep(0);
			} while (IsProcessRunning(processHandle));

			ReadPipes();
		}
		else
		{
			::WaitForSingleObject(processInformation.hProcess, INFINITE);
		}

		if (outReturnCode != nullptr)
		{
			Confirm(::GetExitCodeProcess(processInformation.hProcess, (DWORD*)outReturnCode));
		}

		::CloseHandle(processInformation.hThread);
		::CloseHandle(processInformation.hProcess);
		result = true;
	}
	else
	{
		const auto errorCode = ::GetLastError();
		_RPT0(_CRT_WARN, "CreateProcess failed (%d).\n", errorCode);

		if (outReturnCode != nullptr)
		{
			*outReturnCode = errorCode;
		}
	}


	/*-------------------------------------------------------------------
	-          �v���Z�X�̏I��
	---------------------------------------------------------------------*/
	if (startupInfo.StartupInfo.hStdOutput != nullptr)
	{
		CloseHandle(startupInfo.StartupInfo.hStdOutput);
	}
	if(startupInfo.StartupInfo.hStdError != nullptr)
	{
		CloseHandle(startupInfo.StartupInfo.hStdError);
	}
	if(stdOutRead != nullptr)
	{
		::CloseHandle(stdOutRead);
	}
	if(stdErrorRead != nullptr)
	{
		::CloseHandle(stdErrorRead);
	}
	if(startupInfo.lpAttributeList != nullptr)
	{
		DeleteProcThreadAttributeList(startupInfo.lpAttributeList);
	}

	return result;
}

/*!**********************************************************************
*  @brief     �Ǘ��҂Ƃ��ăv���Z�X�����s���܂�. �K�v�ɉ����ĊǗ��҂ւ̏��i���s���܂�.
*  @param[in]  const tstring&  : ���s����v���O�����̃p�X
*  @param[in]  const tstring&  : �v���O�����ɓn���p�����[�^
*  @param[out] int32*          : return�R�[�h
*  @return     bool ����
*************************************************************************/
bool CoreProcess::ExecuteElevatedProcess(const gu::tstring* programPath, const gu::tstring& parameter, gu::int32* outReturnCode) const
{
	/*-------------------------------------------------------------------
	-            Start up Info�̏�����
	---------------------------------------------------------------------*/
	SHELLEXECUTEINFO shellExecuteInfo = {};
	ZeroMemory(&shellExecuteInfo, sizeof(SHELLEXECUTEINFO));

	shellExecuteInfo.cbSize       = sizeof(SHELLEXECUTEINFO);
	shellExecuteInfo.fMask        = SEE_MASK_UNICODE | SEE_MASK_NOCLOSEPROCESS;
	shellExecuteInfo.lpFile       = programPath->CString();
	shellExecuteInfo.lpVerb       = TEXT("runas");
	shellExecuteInfo.lpParameters = parameter.CString();
	shellExecuteInfo.nShow        = SW_SHOW;

	if (!::ShellExecuteEx(&shellExecuteInfo))
	{
		return false;
	}

	::WaitForSingleObject(shellExecuteInfo.hProcess, INFINITE);
	if(outReturnCode != nullptr)
	{
		Confirm(::GetExitCodeProcess(shellExecuteInfo.hProcess, (DWORD*)outReturnCode));
	}

	Confirm(::CloseHandle(shellExecuteInfo.hProcess));
	return true;
}

/*!**********************************************************************
*  @brief      �Ǘ��҂Ƃ��ăv���Z�X�����s���܂�. �K�v�ɉ����ĊǗ��҂ւ̏��i���s���܂�.
*  @param[in]  const tstring&  : ���s����v���O�����̃p�X
*  @param[in]  const tstring&  : �v���O�����ɓn���p�����[�^
*  @return     ProcessHandle
*************************************************************************/
core::ProcessHandle CoreProcess::ExecuteElevatedProcess(const gu::tstring* programPath, const gu::tstring& parameter) const
{
	/*-------------------------------------------------------------------
	-            Start up Info�̏�����
	---------------------------------------------------------------------*/
	SHELLEXECUTEINFO shellExecuteInfo = {};
	ZeroMemory(&shellExecuteInfo, sizeof(SHELLEXECUTEINFO));

	shellExecuteInfo.cbSize       = sizeof(SHELLEXECUTEINFO);
	shellExecuteInfo.fMask        = SEE_MASK_UNICODE | SEE_MASK_NOCLOSEPROCESS;
	shellExecuteInfo.lpFile       = programPath->CString();
	shellExecuteInfo.lpVerb       = TEXT("runas");
	shellExecuteInfo.lpParameters = parameter.CString();
	shellExecuteInfo.nShow        = SW_SHOW;

	if (!::ShellExecuteEx(&shellExecuteInfo))
	{
		return core::ProcessHandle();
	}

	return core::ProcessHandle(shellExecuteInfo.hProcess);

}

/*!**********************************************************************
*  @brief      �v���Z�X�n���h���𒼐ړn����, �v���Z�X���s���ł��邩��Ԃ��܂�.
*  @param[in] const ProcessHandle& : �v���Z�X�n���h��
*  @return    bool �v���Z�X���s���ł��邩�ǂ���
*************************************************************************/
bool CoreProcess::IsProcessRunning(const core::ProcessHandle& handle) const
{
	bool isRunning = true;

	uint32 result = ::WaitForSingleObject(handle.GetHandle(), 0);

	if (result != WAIT_TIMEOUT)
	{
		isRunning = false;
	}

	return isRunning;
}

/*!**********************************************************************
*  @brief     �v���Z�XID���g����, �v���Z�X�����s���ł��邩��Ԃ��܂�
*  @param[in] const gu::uint32 : �v���Z�XID
*  @return    bool �v���Z�X���s���ł��邩�ǂ���
*************************************************************************/
bool CoreProcess::IsProcessRunning(const gu::uint32 processID) const
{
	bool isRunning = true;

	const auto handle = ::OpenProcess(SYNCHRONIZE, FALSE, processID);
	
	if (handle == NULL)
	{
		isRunning = false;
	}
	else
	{
		const auto result = ::WaitForSingleObject(handle, 0);
		if (result != WAIT_TIMEOUT)
		{
			isRunning = false;
		}
		::CloseHandle(handle);
	}

	return isRunning;
}

/*!**********************************************************************
*  @brief     �v���Z�X�n���h���𒼐ړn����, �v���Z�X���s���ł��邩��Ԃ��܂�.
*  @param[in] const gu::tstring& �v���Z�X��
*  @return    bool �v���Z�X���s���ł��邩�ǂ���
*************************************************************************/
bool CoreProcess::IsProcessRunning(const gu::tstring& processName) const
{
	/*-------------------------------------------------------------------
	-           �g���q(.exe)�̒ǉ�
	---------------------------------------------------------------------*/
	tstring processNameExe = processName;
	if (processNameExe.Contains(SP(".exe"), false))
	{
		processNameExe += tstring(TEXT(".exe"));
	}

	/*-------------------------------------------------------------------
	-           �X�i�b�v�V���b�g�̎擾
	---------------------------------------------------------------------*/
	HANDLE snapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (snapshot != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 processEntry;
		processEntry.dwSize = sizeof(PROCESSENTRY32);

		/*-------------------------------------------------------------------
		-           �v���Z�X�̎擾
		---------------------------------------------------------------------*/
		if (::Process32First(snapshot, &processEntry))
		{
			do
			{
				if (processNameExe == processEntry.szExeFile)
				{
					::CloseHandle(snapshot);
					return true;
				}
			} while (::Process32Next(snapshot, &processEntry));
		}
	}

	/*-------------------------------------------------------------------
	-           �n���h���̏I��
	---------------------------------------------------------------------*/
	::CloseHandle(snapshot);
	return false;
}


/*!**********************************************************************
*  @brief     �v���Z�X��ҋ@���܂�
*  @param[in] const ProcessHandle& : �v���Z�X�n���h��
*  @param[in] const gu::uint32 : �ҋ@��millisecond. �f�t�H���g�͖����ɑ҂�������
*  @return    void
*************************************************************************/
void CoreProcess::WaitForProcess(const core::ProcessHandle& handle, gu::uint32 milliseconds) const
{
	::WaitForSingleObject(handle.GetHandle(), milliseconds);
}

/*!**********************************************************************
*  @brief      �w�肵���v���Z�X�̏I����Ԃ��擾���܂�
*  @param[in]  const ProcessHandle& : �v���Z�X�n���h��
*  @param[out] gu::uint32* : �I���R�[�h
*  @return     bool
*************************************************************************/
bool CoreProcess::GetProcessExitCode(const core::ProcessHandle& handle, gu::uint32* exitCode) const
{
	DWORD code = 0;

	if (::GetExitCodeProcess(handle.GetHandle(), &code) && code != STILL_ACTIVE)
	{
		if (code)
		{
			*exitCode = code;
		}

		return true;
	}
	else
	{
		return false;
	}
}

/*!**********************************************************************
*  @brief     �v���Z�X�����擾
*  @param[in] const gu::uint32 �v���Z�XID
*  @return    tstring
*************************************************************************/
gu::tstring CoreProcess::GetProcessName(const gu::uint32 processID) const
{
	const auto handle = ::OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, processID);

	if (handle == NULL)
	{
		return tstring();
	}

	tstring output = SP("");
	const uint32 bufferSize = 4096;
	tchar buffer[bufferSize] = {};
	int32 outSize = bufferSize;

	if(QueryFullProcessImageName(handle, 0, buffer, (PDWORD)(&outSize)))
	{
		output = tstring(buffer);
	}
	
	::CloseHandle(handle);
	return output;
}

/*!**********************************************************************
*  @brief     �v���Z�X�̗D��x��ݒ肵�܂�
*  @param[in] ProcessHandle& : �v���Z�X�n���h��
*  @param[in] const ProcessPriority : �v���Z�X�̗D��x
*  @return    bool
*************************************************************************/
bool CoreProcess::SetProcessPriority(core::ProcessHandle& handle, const core::ProcessPriority priority) const
{
	/*-------------------------------------------------------------------
	-            �v���Z�X�쐬���̏������D��x�ݒ�
	---------------------------------------------------------------------*/
	DWORD creationFlags = NORMAL_PRIORITY_CLASS;
	switch (priority)
	{
		case core::ProcessPriority::Idle:
		{
			creationFlags = IDLE_PRIORITY_CLASS;
			break;
		}
		case core::ProcessPriority::Low:
		{
			creationFlags = BELOW_NORMAL_PRIORITY_CLASS;
			break;
		}
		case core::ProcessPriority::Normal:
		{
			creationFlags = NORMAL_PRIORITY_CLASS;
			break;
		}
		case core::ProcessPriority::AboveNormal:
		{
			creationFlags = ABOVE_NORMAL_PRIORITY_CLASS;
			break;
		}
		case core::ProcessPriority::High:
		{
			creationFlags = HIGH_PRIORITY_CLASS;
			break;
		}
		case core::ProcessPriority::RealTime:
		{
			creationFlags = REALTIME_PRIORITY_CLASS;
			break;
		}
		default:
		{
			break;
		}
	}

	if (handle.IsValid())
	{
		return !!::SetPriorityClass(handle.GetHandle(), creationFlags);
	}
	else
	{
		return false;
	}
}

/*!**********************************************************************
*  @brief     �����̃p�C�v���쐬���܂�. �p�C�v�͓ǂݎ��Ə������݂̗������T�|�[�g���܂�
*  @param[in] void** readPipe : �ǂݎ��p�C�v
*  @param[in] void** writePipe : �������݃p�C�v
*  @param[in] cosnt bool writePipeInheritable : �������݃p�C�v���q�v���Z�X�ɂ��p���\�ɂ��邩�ǂ���. �q�v���Z�X���������݉\�ɂ��邩�ǂ���
*  @return    bool
*************************************************************************/
bool CoreProcess::CreatePipe(void** readPipe, void** writePipe, const bool writePipeInheritable) const
{
	SECURITY_ATTRIBUTES attributes = {sizeof(SECURITY_ATTRIBUTES), NULL, true};

	if(!::CreatePipe((PHANDLE)readPipe, (PHANDLE)writePipe, &attributes, 0))
	{
		return false;
	}

	if(!::SetHandleInformation(writePipeInheritable ? *(PHANDLE*)writePipe : *(PHANDLE*)readPipe, HANDLE_FLAG_INHERIT, 0))
	{
		return false;
	}

	return true;
}

/*!**********************************************************************
*  @brief     �����̃p�C�v���I�����܂�
*  @param[in] void** readPipe : �ǂݎ��p�C�v
*  @param[in] void** writePipe : �������݃p�C�v
*  @return    void
*************************************************************************/
void CoreProcess::ClosePipe(void** readPipe, void** writePipe) const
{
	if (readPipe != nullptr && readPipe != INVALID_HANDLE_VALUE)
	{
		::CloseHandle(readPipe);
	}

	if (writePipe != nullptr && writePipe != INVALID_HANDLE_VALUE)
	{
		::CloseHandle(writePipe);
	}
}

/*!**********************************************************************
*  @brief     �w�肳�ꂽ�p�C�v����, �f�[�^�̓ǂݎ����s���܂�.
*  @attention �����R�[�h��UTF-8��z�肵�Ă��܂�.�����_�ł͕����R�[�h�̕ϊ��͍s���Ă��܂���.
*  @param[in] void* �ǂݎ����s���p�C�v
*  @param[out void* �ǂݎ�����f�[�^
*  @param[in] gu::uint64 �ǂݎ��o�C�g��
*  @return    bool ����
*************************************************************************/
bool CoreProcess::ReadPipe(void* readPipe, void* outData, gu::uint64 byteSize) const
{
	/*-------------------------------------------------------------------
	-          ���p�\�ȃf�[�^�����݂���ꍇ, �p�C�v����f�[�^��ǂݎ��܂�
	---------------------------------------------------------------------*/
	uint32 readBytes = 0;
	if (::ReadFile(readPipe, outData, byteSize, (::DWORD*)&readBytes, NULL) == FALSE)
	{
		return false;
	}

	return true;
}

/*!**********************************************************************
*  @brief     �w�肳�ꂽ�p�C�v����, �f�[�^�̓ǂݎ����s���܂�.
*  @attention �����R�[�h��UTF-8��z�肵�Ă��܂�.�����_�ł͕����R�[�h�̕ϊ��͍s���Ă��܂���.
*  @param[in] void* �ǂݎ����s���p�C�v
*  @param[out gu::DynamicArray<uint8>& �ǂݎ�����o�C�g��
*  @return    bool ����
*************************************************************************/
bool CoreProcess::ReadPipe(void* readPipe, gu::DynamicArray<gu::uint8>& outData) const
{
	/*-------------------------------------------------------------------
	-          �ǂݎ��\�ȃo�C�g�����擾���܂�
	---------------------------------------------------------------------*/
	uint32 bytesAvailable = 0;
	if (::PeekNamedPipe(readPipe, NULL, 0, NULL, (::DWORD*)&bytesAvailable, NULL) == FALSE || bytesAvailable == 0)
	{
		return false;
	}

	/*-------------------------------------------------------------------
	-          ���p�\�ȃf�[�^�����݂���ꍇ, �p�C�v����f�[�^��ǂݎ��܂�
	---------------------------------------------------------------------*/
	outData.Resize(bytesAvailable, false);

	uint32 readBytes = 0;
	if (::ReadFile(readPipe, outData.Data(), bytesAvailable, (::DWORD*)&readBytes, NULL) == FALSE)
	{
		return false;
	}
	return true;
}

/*!**********************************************************************
*  @brief     �w�肳�ꂽ�p�C�v����, �f�[�^�̓ǂݎ����s���܂�.
*  @attention �����R�[�h��UTF-8��z�肵�Ă��܂�.�����_�ł͕����R�[�h�̕ϊ��͍s���Ă��܂���.
*  @param[in] void* ���������s���p�C�v
*  @param[in] const void* �������ރf�[�^
*  @param[in] gu::uint64 �������ރo�C�g��
*  @return    bool ����
*************************************************************************/
bool CoreProcess::WritePipe(void* writePipe, const void* data, gu::uint64 byteSize) const
{
	if(writePipe == nullptr || byteSize == 0)
	{
		return false;
	}

	/*-------------------------------------------------------------------
	-          �p�C�v�Ƀf�[�^���������݂܂�
	---------------------------------------------------------------------*/
	return !!::WriteFile(writePipe, data, byteSize, NULL, NULL) ;
}

#pragma endregion Public Function