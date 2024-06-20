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
*  @brief     指定されたファイル名のDLLをロードし, そのハンドルを返します. @n
*             https://qiita.com/tera1707/items/727e7ad4dc9f3205c137
*  @param[in] const tstring& fiieName : ロードしたいDLLのファイル名
*  @return    void* : ロードしたDLLのハンドル
*************************************************************************/
void* CoreProcess::GetDLLHandle(const gu::tstring& fileName) const
{
	return ::LoadLibraryW(fileName.CString());
}

/*!**********************************************************************
*  @brief     指定されたファイル名のDLLをメモリから解放します
*  @param[in] void* handle : ロードしたDLLのハンドル
*  @return    bool 結果
*************************************************************************/
bool CoreProcess::FreeDLLHandle(void* handle) const
{
	return !!::FreeLibrary((HMODULE)handle);
}

/*!**********************************************************************
*  @brief     本プロセスのプロセスIDを返します.
*  @param[in] void
*  @return    uint32 プロセスID
*************************************************************************/
gu::uint32 CoreProcess::GetCurrentProcessID() const
{
	return ::GetCurrentProcessId();
}

/*!**********************************************************************
*  @brief     この関数の呼び出し中に現在のスレッドが実行されていたプロセッサの数(個数ではありません)を取得
*  @param[in] void
*  @return    uint32 プロセッサ数
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
*  @brief     OSがどのプロセッサでスレッド, プロセスを実行するべきかを設定します
*  @param[in] const uint64 affinity : プロセッサのビットマスク
*  @return    bool 結果
*************************************************************************/
bool CoreProcess::SetProcessAffinity(const gu::uint64 affnity) const
{
	return !!::SetProcessAffinityMask(::GetCurrentProcess(), affnity);
}

/*!**********************************************************************
*  @brief     Processの生成とPrimaryスレッドを作成するための関数. 呼び出したプロセスのセキュリティ・コンテキスト内で指定された実行ファイルを起動します
*  @param[in] const tstring& : 実行するプログラムのパス
*  @param[in] const tstring& : プログラムに渡すパラメータ
*  @param[in] const bool     : プロセスをデタッチして実行するかどうか, 新しいプロセスが独自のウィンドウで起動します
*  @param[in] const bool     : プロセスを非表示で実行するかどうか, タスクバーで最小化されます.
*  @param[out] uint32*       : プロセスID
*  @param[in] const int32    : プロセスの優先度 
*  @return    bool
*************************************************************************/
core::ProcessHandle CoreProcess::CreateProcess(const gu::tstring& programPath, const gu::tstring& parameters,
	const bool launchDetached, const core::ProcessWindowActivate activate,
	gu::uint32* outProcessID, const core::ProcessPriority priority) const
{
	/*-------------------------------------------------------------------
	-            プロセス作成時の初期化優先度設定
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
	-            Window作成におけるウィンドウフラグ
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
	-            Start up Infoの初期化
	---------------------------------------------------------------------*/
	STARTUPINFO startupInfo = 
	{
		sizeof(STARTUPINFO), 
		NULL,
		NULL,                   // デスクトップの名前
		NULL,                   // コンソールプロセスのタイトル
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

	// コマンドラインの作成
	tstring commandLine = tstring(programPath);

	if (parameters != nullptr)
	{
		commandLine += SP(' ');
		commandLine += tstring(parameters);
	}

	/*-------------------------------------------------------------------
	-            Start up Infoの初期化
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
*  @brief     既に作成されたプロセスハンドルを開きます
*  @param[in] const gu::uint32 processID : プロセスID
*  @return    ProcessHandle : プロセスハンドル
*************************************************************************/
core::ProcessHandle CoreProcess::OpenProcessHandle(const gu::uint32 processID) const
{
	return core::ProcessHandle(::OpenProcess(PROCESS_ALL_ACCESS, FALSE, processID));
}

/*!**********************************************************************
*  @brief     プロセスを閉じて, リソースを解放するために使用します. プロセス
*  @param[in] const gu::uint32 processID : プロセスID
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
*  @brief     プロセスを強制終了します. その際, killTreeがtrueの場合, 子プロセスも終了します
*  @param[in] ProcessHandle& : プロセスハンドル
*  @param[in] const bool : 子プロセスも終了するかどうか
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
*  @brief      プロセスを実行する. リターン・コード, 標準出力, 標準エラーを返す. この呼び出しはプロセスが戻るまでブロックされる.
*  @param[in]  const tstring&  : 実行するプログラムのパス
*  @param[in]  const tstring&  : プログラムに渡すパラメータ
*  @param[out] int32*          : returnコード
*  @param[out] const tstring*  : 標準出力
*  @param[out] const tstring*  : 標準エラー
*  @param[in]  const tstring*  : オプションのWorkingディレクトリ
*  @param[in]  const bool      : 親プロセスと一緒にプロセスが終了するようにするにはtrueにします。
*  @return     bool 結果
*************************************************************************/
bool CoreProcess::ExecuteProcess(const gu::tstring& programPath, const gu::tstring& parameters, gu::int32* outReturnCode, gu::tstring* stdOutput, gu::tstring* stdError, gu::tstring* optionWorkingDirectory, const bool shouldEndWithParentProcess)
{
	/*-------------------------------------------------------------------
	-            ジョブオブジェクトの設定
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
	-           ウィンドウ起動時の設定 (タスクバーに表示)
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
	-          標準出力と標準エラーのパイプの設定
	---------------------------------------------------------------------*/
	HANDLE stdOutRead   = nullptr;
	HANDLE stdErrorRead = nullptr;
	
	if (stdOutput != nullptr || stdError != nullptr)
	{
		//標準ハンドルを使用
		startupInfo.StartupInfo.dwFlags |= STARTF_USESTDHANDLES;

		// 作成されるパイプが子プロセスに継承されるようにセキュリティ属性を設定
		SECURITY_ATTRIBUTES securityAttributes = {};
		Memory::Zero(&securityAttributes, sizeof(SECURITY_ATTRIBUTES));
		securityAttributes.nLength        = sizeof(SECURITY_ATTRIBUTES);
		securityAttributes.bInheritHandle = TRUE;

		Confirm(::CreatePipe(&stdOutRead  , &startupInfo.StartupInfo.hStdOutput, &securityAttributes, 0));
		Confirm(::CreatePipe(&stdErrorRead, &startupInfo.StartupInfo.hStdError , &securityAttributes, 0));

		DynamicArray<uint8> attributeList = {};
		uint64 bufferSize = 0;

		// バッファサイズの決定 InitializeProcThreadAttributeListは基本Falseが返ります(仕様)
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
	-          コマンドラインの作成
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
	-          プロセスの作成
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
							pipeBuffer[i].Push(binaryBuffer[j]); // Append関数作って最適化した方が良い
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
	-          プロセスの終了
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
*  @brief     管理者としてプロセスを実行します. 必要に応じて管理者への昇格も行います.
*  @param[in]  const tstring&  : 実行するプログラムのパス
*  @param[in]  const tstring&  : プログラムに渡すパラメータ
*  @param[out] int32*          : returnコード
*  @return     bool 結果
*************************************************************************/
bool CoreProcess::ExecuteElevatedProcess(const gu::tstring* programPath, const gu::tstring& parameter, gu::int32* outReturnCode) const
{
	/*-------------------------------------------------------------------
	-            Start up Infoの初期化
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
*  @brief      管理者としてプロセスを実行します. 必要に応じて管理者への昇格も行います.
*  @param[in]  const tstring&  : 実行するプログラムのパス
*  @param[in]  const tstring&  : プログラムに渡すパラメータ
*  @return     ProcessHandle
*************************************************************************/
core::ProcessHandle CoreProcess::ExecuteElevatedProcess(const gu::tstring* programPath, const gu::tstring& parameter) const
{
	/*-------------------------------------------------------------------
	-            Start up Infoの初期化
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
*  @brief      プロセスハンドルを直接渡して, プロセス実行中であるかを返します.
*  @param[in] const ProcessHandle& : プロセスハンドル
*  @return    bool プロセス実行中であるかどうか
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
*  @brief     プロセスIDを使って, プロセスが実行中であるかを返します
*  @param[in] const gu::uint32 : プロセスID
*  @return    bool プロセス実行中であるかどうか
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
*  @brief     プロセスハンドルを直接渡して, プロセス実行中であるかを返します.
*  @param[in] const gu::tstring& プロセス名
*  @return    bool プロセス実行中であるかどうか
*************************************************************************/
bool CoreProcess::IsProcessRunning(const gu::tstring& processName) const
{
	/*-------------------------------------------------------------------
	-           拡張子(.exe)の追加
	---------------------------------------------------------------------*/
	tstring processNameExe = processName;
	if (processNameExe.Contains(SP(".exe"), false))
	{
		processNameExe += tstring(TEXT(".exe"));
	}

	/*-------------------------------------------------------------------
	-           スナップショットの取得
	---------------------------------------------------------------------*/
	HANDLE snapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (snapshot != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 processEntry;
		processEntry.dwSize = sizeof(PROCESSENTRY32);

		/*-------------------------------------------------------------------
		-           プロセスの取得
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
	-           ハンドルの終了
	---------------------------------------------------------------------*/
	::CloseHandle(snapshot);
	return false;
}


/*!**********************************************************************
*  @brief     プロセスを待機します
*  @param[in] const ProcessHandle& : プロセスハンドル
*  @param[in] const gu::uint32 : 待機のmillisecond. デフォルトは無限に待ち続ける
*  @return    void
*************************************************************************/
void CoreProcess::WaitForProcess(const core::ProcessHandle& handle, gu::uint32 milliseconds) const
{
	::WaitForSingleObject(handle.GetHandle(), milliseconds);
}

/*!**********************************************************************
*  @brief      指定したプロセスの終了状態を取得します
*  @param[in]  const ProcessHandle& : プロセスハンドル
*  @param[out] gu::uint32* : 終了コード
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
*  @brief     プロセス名を取得
*  @param[in] const gu::uint32 プロセスID
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
*  @brief     プロセスの優先度を設定します
*  @param[in] ProcessHandle& : プロセスハンドル
*  @param[in] const ProcessPriority : プロセスの優先度
*  @return    bool
*************************************************************************/
bool CoreProcess::SetProcessPriority(core::ProcessHandle& handle, const core::ProcessPriority priority) const
{
	/*-------------------------------------------------------------------
	-            プロセス作成時の初期化優先度設定
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
*  @brief     無名のパイプを作成します. パイプは読み取りと書き込みの両方をサポートします
*  @param[in] void** readPipe : 読み取りパイプ
*  @param[in] void** writePipe : 書き込みパイプ
*  @param[in] cosnt bool writePipeInheritable : 書き込みパイプを子プロセスにも継承可能にするかどうか. 子プロセスも書き込み可能にするかどうか
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
*  @brief     無名のパイプを終了します
*  @param[in] void** readPipe : 読み取りパイプ
*  @param[in] void** writePipe : 書き込みパイプ
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
*  @brief     指定されたパイプから, データの読み取りを行います.
*  @attention 文字コードはUTF-8を想定しています.現時点では文字コードの変換は行っていません.
*  @param[in] void* 読み取りを行うパイプ
*  @param[out void* 読み取ったデータ
*  @param[in] gu::uint64 読み取るバイト数
*  @return    bool 結果
*************************************************************************/
bool CoreProcess::ReadPipe(void* readPipe, void* outData, gu::uint64 byteSize) const
{
	/*-------------------------------------------------------------------
	-          利用可能なデータが存在する場合, パイプからデータを読み取ります
	---------------------------------------------------------------------*/
	uint32 readBytes = 0;
	if (::ReadFile(readPipe, outData, byteSize, (::DWORD*)&readBytes, NULL) == FALSE)
	{
		return false;
	}

	return true;
}

/*!**********************************************************************
*  @brief     指定されたパイプから, データの読み取りを行います.
*  @attention 文字コードはUTF-8を想定しています.現時点では文字コードの変換は行っていません.
*  @param[in] void* 読み取りを行うパイプ
*  @param[out gu::DynamicArray<uint8>& 読み取ったバイト列
*  @return    bool 結果
*************************************************************************/
bool CoreProcess::ReadPipe(void* readPipe, gu::DynamicArray<gu::uint8>& outData) const
{
	/*-------------------------------------------------------------------
	-          読み取り可能なバイト数を取得します
	---------------------------------------------------------------------*/
	uint32 bytesAvailable = 0;
	if (::PeekNamedPipe(readPipe, NULL, 0, NULL, (::DWORD*)&bytesAvailable, NULL) == FALSE || bytesAvailable == 0)
	{
		return false;
	}

	/*-------------------------------------------------------------------
	-          利用可能なデータが存在する場合, パイプからデータを読み取ります
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
*  @brief     指定されたパイプから, データの読み取りを行います.
*  @attention 文字コードはUTF-8を想定しています.現時点では文字コードの変換は行っていません.
*  @param[in] void* 書き取りを行うパイプ
*  @param[in] const void* 書き込むデータ
*  @param[in] gu::uint64 書き込むバイト数
*  @return    bool 結果
*************************************************************************/
bool CoreProcess::WritePipe(void* writePipe, const void* data, gu::uint64 byteSize) const
{
	if(writePipe == nullptr || byteSize == 0)
	{
		return false;
	}

	/*-------------------------------------------------------------------
	-          パイプにデータを書き込みます
	---------------------------------------------------------------------*/
	return !!::WriteFile(writePipe, data, byteSize, NULL, NULL) ;
}

#pragma endregion Public Function