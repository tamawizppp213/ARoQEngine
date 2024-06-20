//////////////////////////////////////////////////////////////////////////////////
///  @file   WindowsProcess.hpp
///  @brief  OSのプロセスを操作するクラス
///  @author toide
///  @date   2024/06/15 22:46:06
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef WINDOWS_PROCESS_HPP
#define WINDOWS_PROCESS_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "Platform/Core/Include/CoreProcess.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace platform::windows
{
	/****************************************************************************
	*				  			   CoreProcess
	****************************************************************************/
	/*! @brief OSのプロセスを操作するクラス
	*****************************************************************************/
	class CoreProcess : public platform::core::CoreProcess
	{
	public:

		#pragma region Public Function

		#pragma region DLL Function
		/*!**********************************************************************
		*  @brief     指定されたファイル名のDLLをロードし, そのハンドルを返します. @n
		*             https://qiita.com/tera1707/items/727e7ad4dc9f3205c137
		*  @param[in] const tstring& fiieName : ロードしたいDLLのファイル名
		*  @return    void* : ロードしたDLLのハンドル
		*************************************************************************/
		virtual void* GetDLLHandle(const gu::tstring& fileName) const override;

		/*!**********************************************************************
		*  @brief     指定されたファイル名のDLLをメモリから解放します
		*  @param[in] void* handle : ロードしたDLLのハンドル
		*  @return    bool 結果
		*************************************************************************/
		virtual bool FreeDLLHandle(void* handle) const override;

		#pragma endregion DLL Function

		#pragma region Process Function

		/*!**********************************************************************
		*  @brief     本プロセスのプロセスIDを返します. 
		*  @param[in] void
		*  @return    uint32 プロセスID
		*************************************************************************/
		virtual gu::uint32 GetCurrentProcessID() const override;

		/*!**********************************************************************
		*  @brief     この関数の呼び出し中に現在のスレッドが実行されていたプロセッサの数(個数ではありません)を取得
		*  @param[in] void
		*  @return    uint32 プロセッサ数
		*************************************************************************/
		virtual gu::uint32 GetCurrentProcessorNumber() const override;

		/*!**********************************************************************
		*  @brief     OSがどのプロセッサでスレッド, プロセスを実行するべきかを設定します
		*  @param[in] const uint64 affinity : プロセッサのビットマスク
		*  @return    bool 結果
		*************************************************************************/
		virtual bool SetProcessAffinity(const gu::uint64 affinity) const override;

		/*!**********************************************************************
		*  @brief     Processの生成とPrimaryスレッドを作成するための関数. 呼び出したプロセスのセキュリティ・コンテキスト内で指定された実行ファイルを起動します
		*  @param[in] const tstring&                     : 実行するプログラムのパス
		*  @param[in] const tstring&                     : プログラムに渡すパラメータ
		*  @param[in] const bool                         : プロセスをデタッチして実行するかどうか, 新しいプロセスが独自のウィンドウで起動します
		*  @param[in] const core::ProcessWindowActivate  : プロセスのウィンドウの表示
		*  @param[out] uint32*                           : プロセスID
		*  @param[in] const ProcessPriority              : プロセスの優先度
		*  @return    bool
		*************************************************************************/
		virtual core::ProcessHandle CreateProcess(const gu::tstring& programPath, const gu::tstring& parameters,
			const bool launchDetached, const core::ProcessWindowActivate activate,
			gu::uint32* outProcessID , const core::ProcessPriority priority) const override;

		/*!**********************************************************************
		*  @brief     既に作成されたプロセスハンドルを開きます
		*  @param[in] const gu::uint32 processID : プロセスID
		*  @return    ProcessHandle : プロセスハンドル
		*************************************************************************/
		virtual core::ProcessHandle OpenProcessHandle(const gu::uint32 processID) const override;

		/*!**********************************************************************
		*  @brief     プロセスを閉じて, リソースを解放するために使用します. プロセス
		*  @param[in] const gu::uint32 processID : プロセスID
		*  @return    void
		*************************************************************************/
		virtual void CloseProcessHandle(core::ProcessHandle& handle) const override;

		/*!**********************************************************************
		*  @brief     プロセスを強制終了します. その際, killTreeがtrueの場合, 子プロセスも終了します
		*  @param[in] ProcessHandle& : プロセスハンドル
		*  @param[in] const bool : 子プロセスも終了するかどうか
		*  @return    void
		*************************************************************************/
		virtual void TerminateProcess(core::ProcessHandle& handle, const bool killTree) const override;

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
		virtual bool ExecuteProcess(const gu::tstring& programPath, const gu::tstring& parameters, gu::int32* outReturnCode, gu::tstring* stdOutput, gu::tstring* stdError, gu::tstring* optionWorkingDirectory = nullptr, const bool shouldEndWithParentProcess = false) override;

		/*!**********************************************************************
		*  @brief     管理者としてプロセスを実行します. 必要に応じて管理者への昇格も行います.
		*  @param[in]  const tstring&  : 実行するプログラムのパス
		*  @param[in]  const tstring&  : プログラムに渡すパラメータ
		*  @param[out] int32*          : returnコード
		*  @return     bool 結果
		*************************************************************************/
		virtual bool ExecuteElevatedProcess(const gu::tstring* programPath, const gu::tstring& parameter, gu::int32* outReturnCode) const override;

		/*!**********************************************************************
		*  @brief      管理者としてプロセスを実行します. 必要に応じて管理者への昇格も行います.
		*  @param[in]  const tstring&  : 実行するプログラムのパス
		*  @param[in]  const tstring&  : プログラムに渡すパラメータ
		*  @return     ProcessHandle
		*************************************************************************/
		virtual core::ProcessHandle ExecuteElevatedProcess(const gu::tstring* programPath, const gu::tstring& parameter) const override;

		/*!**********************************************************************
		*  @brief     プロセスハンドルを直接渡して, プロセス実行中であるかを返します.
		*  @param[in] const ProcessHandle& : プロセスハンドル
		*  @return    bool プロセス実行中であるかどうか
		*************************************************************************/
		virtual bool IsProcessRunning(const core::ProcessHandle& handle) const override;

		/*!**********************************************************************
		*  @brief     プロセスIDを使って, プロセスが実行中であるかを返します
		*  @param[in] const gu::uint32 : プロセスID
		*  @return    bool プロセス実行中であるかどうか
		*************************************************************************/
		virtual bool IsProcessRunning(const gu::uint32 processID) const override;

		/*!**********************************************************************
		*  @brief     プロセスハンドルを直接渡して, プロセス実行中であるかを返します.
		*  @param[in] const gu::tstring& プロセス名
		*  @return    bool プロセス実行中であるかどうか
		*************************************************************************/
		virtual bool IsProcessRunning(const gu::tstring& processName) const override;

		/*!**********************************************************************
		*  @brief     プロセスを待機します
		*  @param[in] const ProcessHandle& : プロセスハンドル
		*  @param[in] const gu::uint32 : 待機のmillisecond. デフォルトは無限に待ち続ける
		*  @return    void
		*************************************************************************/
		virtual void WaitForProcess(const core::ProcessHandle& handle, gu::uint32 milliseconds = 0xFFFFFFFF) const override;

		/*!**********************************************************************
		*  @brief      指定したプロセスの終了状態を取得します
		*  @param[in]  const ProcessHandle& : プロセスハンドル
		*  @param[out] gu::uint32* : 終了コード
		*  @return     bool
		*************************************************************************/
		virtual bool GetProcessExitCode(const core::ProcessHandle& handle, gu::uint32* exitCode) const override;

		/*!**********************************************************************
		*  @brief     プロセス名を取得
		*  @param[in] const gu::uint32 プロセスID
		*  @return    tstring
		*************************************************************************/
		virtual gu::tstring GetProcessName(const gu::uint32 processID) const override;

		/*!**********************************************************************
		*  @brief     プロセスの優先度を設定します
		*  @param[in] ProcessHandle& : プロセスハンドル
		*  @param[in] const ProcessPriority : プロセスの優先度
		*  @return    bool
		*************************************************************************/
		virtual bool SetProcessPriority(core::ProcessHandle& handle, const core::ProcessPriority priority) const override;

		#pragma endregion Process Function

		#pragma region Pipe Function
		/*!**********************************************************************
		*  @brief     無名のパイプを作成します. パイプは読み取りと書き込みの両方をサポートします
		*  @param[in] void** readPipe : 読み取りパイプ
		*  @param[in] void** writePipe : 書き込みパイプ
		*  @param[in] cosnt bool writePipeInheritable : 書き込みパイプを子プロセスにも継承可能にするかどうか. 子プロセスも書き込み可能にするかどうか
		*  @return    bool
		*************************************************************************/
		virtual bool CreatePipe(void** readPipe, void** writePipe, const bool writePipeInheritable) const override;

		/*!**********************************************************************
		*  @brief     無名のパイプを終了します
		*  @param[in] void** readPipe : 読み取りパイプ
		*  @param[in] void** writePipe : 書き込みパイプ
		*  @return    void
		*************************************************************************/
		virtual void ClosePipe(void** readPipe, void** writePipe) const override;

		/*!**********************************************************************
		*  @brief     指定されたパイプから, データの読み取りを行います.
		*  @attention 文字コードはUTF-8を想定しています.現時点では文字コードの変換は行っていません.
		*  @param[in] void* 読み取りを行うパイプ
		*  @param[out void* 読み取ったデータ
		*  @param[in] gu::uint64 読み取るバイト数
		*  @return    bool 結果
		*************************************************************************/
		virtual bool ReadPipe(void* readPipe, void* outData, gu::uint64 byteSize) const override ;

		/*!**********************************************************************
		*  @brief     指定されたパイプから, データの読み取りを行います.
		*  @attention 文字コードはUTF-8を想定しています.現時点では文字コードの変換は行っていません.
		*  @param[in] void* 読み取りを行うパイプ
		*  @param[out gu::DynamicArray<uint8>& 読み取ったバイト列
		*  @return    bool 結果
		*************************************************************************/
		virtual bool ReadPipe(void* readPipe, gu::DynamicArray<gu::uint8>& outData) const override;

		/*!**********************************************************************
		*  @brief     指定されたパイプから, データの読み取りを行います.
		*  @attention 文字コードはUTF-8を想定しています.現時点では文字コードの変換は行っていません.
		*  @param[in] void* 書き取りを行うパイプ
		*  @param[in] const void* 書き込むデータ
		*  @param[in] gu::uint64 書き込むバイト数
		*  @return    bool 結果
		*************************************************************************/
		virtual bool WritePipe(void* writePipe, const void* data, gu::uint64 byteSize) const override;

		#pragma endregion Pipe Function

		#pragma endregion 

		#pragma region Public Property

		#pragma endregion 

		#pragma region Public Operator 

		#pragma endregion 

		#pragma region Public Constructor and Destructor

		#pragma endregion 

	protected:
		#pragma region Protected Constructor and Destructor

		#pragma endregion 

		#pragma region Protected Function

		#pragma endregion 

		#pragma region Protected Property

		#pragma endregion

	private:
		#pragma region Private Constructor and Destructor

		#pragma endregion

		#pragma region Private Function

		#pragma endregion 

		#pragma region Private Property

		#pragma endregion 

	};
};
#endif