//////////////////////////////////////////////////////////////////////////////////
///  @file   CoreFileSystem.hpp
///  @brief  ファイルの読み書きや, ファイルが存在するかの確認, ディレクトリの作成や削除などの基本的なファイル操作の機能を提供します.
///  @author toide
///  @date   2024/05/05 10:32:18
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef CORE_FILE_SYSTEM_HPP
#define CORE_FILE_SYSTEM_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/GUClassUtility.hpp"
#include "GameUtility/Base/Include/GUString.hpp"
#include "GameUtility/Base/Include/GUSmartPointer.hpp"
#include "Platform/Core/Private/File/Include/CoreFileCommonState.hpp"
#include "Platform/Core/Private/File/Include/CoreFileHandle.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                              Undifined
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

namespace platform::core
{
	/****************************************************************************
	*				  			   IFileSystem
	****************************************************************************/
	/*! @brief : ファイルの読み書きや, ファイルが存在するかの確認, ディレクトリの作成や削除などの基本的なファイル操作の機能を提供します.
	*****************************************************************************/
	class IFileSystem : public gu::NonCopyAndMove
	{
	public:
		/*!**********************************************************************
		*  @brief     シングルトンインスタンスを取得します. (基本的にメンバ変数を使わないため, 特に悪影響はありません)
		*  @param[in] const bool 強制的に初期化を行います. (Default: false)
		*  @return    gu::SharedPointer<IFileSystem> ファイルシステムのインスタンス
		*************************************************************************/
		static gu::SharedPointer<IFileSystem> Get(const bool forceInitialize = false);

		#pragma region Public Function
		/*!**********************************************************************
		*  @brief     ファイルを開いて読み込みます. この時, 実行時の状況に応じて同期/非同期を自動で切り替えます.  
		*  @param[in] const tchar* ファイルパス
		*  @param[in] const bool   ファイルの書き込みを行います.
		*  @param[in] const bool   非同期読み込みを実行するかを設定します (デフォルトは同期読み込み)
		*  @return    IFileHandle* ファイルのハンドル
		*************************************************************************/
		virtual gu::SharedPointer<file::IFileHandle> OpenRead(const gu::tchar* fileName, const bool allowWrite = false, const bool useAsync = false) = 0;

		/*!**********************************************************************
		*  @brief     ファイルを開いて書き込みます. この時, 実行時の状況に応じて同期/非同期を自動で切り替えます
		*  @param[in] const tchar* ファイルパス
		*  @param[in] const bool   既に書き込まれた部分を上書きせず, 後ろに追加するかを決めます (Default: 新規上書き)
		*  @param[in] const bool   ファイルの閲覧も行います. (Default : false)
		*   @param[in] const bool  非同期書き込みを実行するかを設定します (デフォルトは同期書き込み)
		*  @return    IFileHandle* ファイルのハンドル
		*************************************************************************/
		virtual gu::SharedPointer<file::IFileHandle> OpenWrite(const gu::tchar* fileName, const bool append = false, const bool allowRead = false, const bool useAsync = false) = 0;

		/*!**********************************************************************
		*  @brief 指定したファイルが存在するかを確認します. 
		*  @param[in] const tchar* ファイルパス
		*  @return    bool ファイルが存在する場合, trueを返します.
		*************************************************************************/
		virtual bool FileExists(const gu::tchar* filePath) const = 0;

		/*!**********************************************************************
		*  @brief 指定したファイルが存在するかを確認します.
		*  @param[in] const tchar* ディレクトリ
		*  @return    bool ファイルが存在する場合, trueを返します.
		*************************************************************************/
		virtual bool DirectoryExists(const gu::tchar* directory) const = 0;

		/*!**********************************************************************
		*  @brief     ディレクトリを作成します. 既にディレクトリが存在する場合もtrueを返します.  
		*  @param[in] const tchar* ディレクトリ
		*  @return    bool ディレクトリを作成出来た場合, 既にディレクトリが存在する場合にtrueを返します
		*************************************************************************/
		virtual bool CreateDirectory(const gu::tchar* directory) const = 0;

		/*!**********************************************************************
		*  @brief     ディレクトリを削除します.
		*  @param[in] const tchar* ディレクトリ
		*  @return    bool ディレクトリを削除出来た場合にtrueを返します
		*************************************************************************/
		virtual bool DeleteDirectory(const gu::tchar* directory) const = 0;

		/*!**********************************************************************
		*  @brief     ファイルを削除します.
		*  @param[in] const tchar* ファイルパス
		*  @return    bool ディレクトリを作成出来た場合にtrueを返します
		*************************************************************************/
		virtual bool DeleteFile(const gu::tchar* filePath) const = 0;

		/*!**********************************************************************
		*  @brief     ファイルをコピーします
		*  @param[in] const tchar* コピー前のファイルパス
		*  @param[in] const tchar* コピー後のファイルパス
		*  @param[in] const bool   上書きを許可するかを設定します
		*  @return    bool ファイルがコピーできた場合にtrueを返します
		*************************************************************************/
		virtual bool CopyFile(const gu::tchar* from, const gu::tchar* to, const bool overwrite) const = 0;

		/*!**********************************************************************
		*  @brief     ファイルを移動します
		*  @param[in] const tchar* 移動前のファイルパス
		*  @param[in] const tchar* 移動後のファイルパス
		*  @return    bool ファイルが移動できた場合にtrueを返します
		*************************************************************************/
		virtual bool MoveFile(const gu::tchar* from, const gu::tchar* to) const = 0;

		/*!**********************************************************************
		*  @brief     読み取りだけしか出来ないファイルかどうか
		*  @param[in] const tchar* ファイルパス
		*  @return    bool Readonlyであればtrue
		*************************************************************************/
		virtual bool IsReadOnly(const gu::tchar* filePath) const = 0;

		/*!**********************************************************************
		*  @brief     ファイル, もしくはディレクトリのリンク(参照)の種類を返します.
		*  @param[in] const tchar* ファイルパス, もしくはディレクトリ
		*  @return    file::LinkType リンクの種類
		*************************************************************************/
		virtual file::LinkType LinkType(const gu::tchar* path) const = 0;

		/*!**********************************************************************
		*  @brief     指定したファイルパスの拡張子を取得します
		*  @param[in] const tchar* ファイルパス
		*  @return    const tchar* 拡張子です. (見つからない場合はnullptrを返します)
		*************************************************************************/
		virtual gu::tstring GetExtension(const gu::tchar* filePath) const = 0;

		/*!**********************************************************************
		*  @brief     ディレクトリ(ファイル名の手前まで)を取得します.
		*  @param[in] const tchar* ファイルパス
		*  @return    const char* ファイル名手前までのディレクトリ
		*************************************************************************/
		virtual gu::tstring GetDirectory(const gu::tchar* filePath) const = 0;

		/*!**********************************************************************
		*  @brief     自身のプロセスに関する作業ディレクトリを取得します.
		*  @param[in] void
		*  @return    gu::tstring 作業ディレクトリ
		*************************************************************************/
		virtual gu::tstring GetProcessDirectory() const = 0;

		/*!**********************************************************************
		*  @brief     対象のディレクトリ直下にハードリンクを作成します. @n
		*             ハードリンクは, ファイルの内容を共有するファイルのことです.
		*  @param[in] const tchar* リンクを行いたい元のファイルパス
		*  @param[in] const tchar* リンクを作成したいファイルパス
		*  @return    bool
		*************************************************************************/
		virtual bool CreateHardLink(const gu::tchar* sourcePath, const gu::tchar* newHardLinkPath) const = 0;

		/*!**********************************************************************
		*  @brief     対象のディレクトリ直下にシンボリックリンクを作成します. @n
		*             シンボリックリンクは, ファイルのパスを参照するファイルのことです.
		*  @param[in] const tchar* リンクを行いたい元のファイルパス
		*  @param[in] const tchar* リンクを作成したいファイルパス
		*  @return    bool
		*************************************************************************/
		virtual bool CreateSymbolicLink(const gu::tchar* sourcePath, const gu::tchar* newSymbolicLinkPath) const = 0;
		#pragma endregion 

		#pragma region Public Property

		#pragma endregion 

		#pragma region Public Operator 

		#pragma endregion 

		#pragma region Public Constructor and Destructor

		#pragma endregion 

	protected:
		#pragma region Protected Constructor and Destructor
		IFileSystem() = default;

		virtual ~IFileSystem() = default;
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

}
#endif