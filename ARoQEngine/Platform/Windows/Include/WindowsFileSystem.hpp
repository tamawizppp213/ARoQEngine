//////////////////////////////////////////////////////////////////////////////////
///  @file   WindowsFileSystem.hpp
///  @brief  ファイルの読み書きや, ファイルが存在するかの確認, ディレクトリの作成や削除などの基本的なファイル操作の機能を提供します.
///  @author toide
///  @date   2024/05/05 10:36:15
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef WINDOWS_FILE_SYSTEM_HPP
#define WINDOWS_FILE_SYSTEM_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "Platform/Core/Include/CoreFileSystem.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace platform::windows
{
	/****************************************************************************
	*				  			   IFileSystem
	****************************************************************************/
	/*! @brief     ファイルの読み書きや, ファイルが存在するかの確認, ディレクトリの作成や削除などの基本的なファイル操作の機能を提供します.
	*****************************************************************************/
	class IFileSystem : public platform::core::IFileSystem
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief     ファイルを開いて読み込みます. この時, 実行時の状況に応じて同期/非同期を自動で切り替えます.  
		*  @param[in] const tstring& ファイルパス
		*  @param[in] const bool   ファイルの書き込みを行います.
		*  @param[in] const bool   非同期読み込みを実行するかを設定します (デフォルトは同期読み込み)
		*  @return    IFileHandle* ファイルのハンドル
		*************************************************************************/
		virtual gu::SharedPointer<core::file::IFileHandle> OpenRead(const gu::tstring& fileName, const bool allowWrite = false, const bool useAsync = false) override;

		/*!**********************************************************************
		*  @brief     ファイルを開いて書き込みます. この時, 実行時の状況に応じて同期/非同期を自動で切り替えます
		*  @param[in] const tstring& ファイルパス
		*  @param[in] const bool   既に書き込まれた部分を上書きせず, 後ろに追加するかを決めます (Default: 新規上書き)
		*  @param[in] const bool   ファイルの閲覧も行います. (Default : false)
		*   @param[in] const bool  非同期書き込みを実行するかを設定します (デフォルトは同期書き込み)
		*  @return    IFileHandle* ファイルのハンドル
		*************************************************************************/
		virtual gu::SharedPointer<core::file::IFileHandle> OpenWrite(const gu::tstring& fileName, const bool append = false, const bool allowRead = false, const bool useAsync = false) override;

		/*!**********************************************************************
		*  @brief 指定したファイルが存在するかを確認します.
		*  @param[in] const tstring& ファイルパス
		*  @return    bool ファイルが存在する場合, trueを返します.
		*************************************************************************/
		virtual bool FileExists(const gu::tstring& filePath) const override;

		/*!**********************************************************************
		*  @brief 指定したファイルが存在するかを確認します.
		*  @param[in] const tstring& ディレクトリ
		*  @return    bool ファイルが存在する場合, trueを返します.
		*************************************************************************/
		virtual bool DirectoryExists(const gu::tstring& directory) const override;

		/*!**********************************************************************
		*  @brief     ディレクトリを作成します. 既にディレクトリが存在する場合もtrueを返します.
		*  @param[in] const tstring& ディレクトリ
		*  @return    bool ディレクトリを作成出来た場合, 既にディレクトリが存在する場合にtrueを返します
		*************************************************************************/
		virtual bool CreateDirectory(const gu::tstring& directory) const override;

		/*!**********************************************************************
		*  @brief     ディレクトリを削除します.
		*  @param[in] const tstring& ディレクトリ
		*  @return    bool ディレクトリを作成出来た場合にtrueを返します
		*************************************************************************/
		virtual bool DeleteDirectory(const gu::tstring& directory) const override;

		/*!**********************************************************************
		*  @brief     ディレクトリを削除します.
		*  @param[in] const tstring& ディレクトリ
		*  @return    bool ディレクトリを削除出来た場合にtrueを返します
		*************************************************************************/
		virtual bool DeleteFile(const gu::tstring& filePath) const override;

		/*!**********************************************************************
		*  @brief     ファイルをコピーします
		*  @param[in] const tstring& コピー前のファイルパス
		*  @param[in] const tstring& コピー後のファイルパス
		*  @param[in] const bool   上書きを許可するかを設定します
		*  @return    bool ファイルがコピーできた場合にtrueを返します
		*************************************************************************/
		virtual bool CopyFile(const gu::tstring& from, const gu::tstring& to, const bool overwrite) const override;

		/*!**********************************************************************
		*  @brief     ファイルを移動します
		*  @param[in] const tstring& 移動前のファイルパス
		*  @param[in] const tstring& 移動後のファイルパス
		*  @return    bool ファイルが移動できた場合にtrueを返します
		*************************************************************************/
		virtual bool MoveFile(const gu::tstring& from, const gu::tstring& to) const override;

		/*!**********************************************************************
		*  @brief     読み取りだけしか出来ないファイルかどうか
		*  @param[in] const tstring& ファイルパス
		*  @return    bool Readonlyであればtrue
		*************************************************************************/
		virtual bool IsReadOnly(const gu::tstring& filePath) const override;

		/*!**********************************************************************
		*  @brief     ファイル, もしくはディレクトリのリンク(参照)の種類を返します.
		*  @param[in] const tstring& ファイルパス, もしくはディレクトリ
		*  @return    file::LinkType リンクの種類
		*************************************************************************/
		virtual platform::core::file::LinkType LinkType(const gu::tstring& path) const override;

		/*!**********************************************************************
		*  @brief     指定したファイルパスの拡張子を取得します
		*  @param[in] const tstring& ファイルパス
		*  @return    gu::tstring 拡張子です. (見つからない場合はnullptrを返します)
		*************************************************************************/
		virtual gu::tstring GetExtension(const gu::tstring& filePath) const override;

		/*!**********************************************************************
		*  @brief     ディレクトリ(ファイル名の手前まで)を取得します. 
		*  @param[in] const tstring& ファイルパス
		*  @return    gu::tstringファイル名手前までのディレクトリ
		*************************************************************************/
		virtual gu::tstring GetDirectory(const gu::tstring& filePath) const override;

		/*!**********************************************************************
		*  @brief     ディレクトリを取り去ってファイル名を取得します
		*  @param[in] const tstring& ディレクトリを含めたファイル名
		*  @return    gu::tstring
		*************************************************************************/
		virtual gu::tstring GetFileName(const gu::tstring& filePath) const override;

		/*!**********************************************************************
		*  @brief     自身のプロセスに関する作業ディレクトリを取得します. 
		*  @param[in] void
		*  @return    gu::tstring 作業ディレクトリ
		*************************************************************************/
		virtual gu::tstring GetProcessDirectory() const override;

		/*!**********************************************************************
		*  @brief     対象のディレクトリ直下にハードリンクを作成します. @n
		*             ハードリンクは, ファイルの内容を共有するファイルのことです.
		*  @param[in] const tstring& リンクを行いたい元のファイルパス
		*  @param[in] const tstring& リンクを作成したいファイルパス
		*  @return    bool
		*************************************************************************/
		virtual bool CreateHardLink(const gu::tstring& sourcePath, const gu::tstring& newHardLinkPath) const override;

		/*!**********************************************************************
		*  @brief     対象のディレクトリ直下にシンボリックリンクを作成します. @n
		*             シンボリックリンクは, ファイルのパスを参照するファイルのことです.
		*  @param[in] const tstring& リンクを行いたい元のファイルパス
		*  @param[in] const tstring& リンクを作成したいファイルパス
		*  @return    bool
		*************************************************************************/
		virtual bool CreateSymbolicLink(const gu::tstring& sourcePath, const gu::tstring& newSymbolicLinkPath) const override;
		#pragma endregion 

		#pragma region Public Property

		#pragma endregion 

		#pragma region Public Operator 
		/*!**********************************************************************
		*  @brief     長い文字列を使用できるか (32767文字まで使用可能か)
		*  @param[in] void
		*  @return    trueの場合32765文字まで可能で, falseの場合260まで可能
		*************************************************************************/
		bool IsLongPathEnabled() const { return IS_LONG_PATH_ENABLED; }

		#pragma endregion 

		#pragma region Public Constructor and Destructor
		/*! @brief デフォルトコンストラクタ*/
		IFileSystem();

		/*! @brief デストラクタ*/
		~IFileSystem();
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
		/*! trueの場合32767文字まで可能で, falseの場合260まで可能 */
		static bool IS_LONG_PATH_ENABLED;
		#pragma endregion 

	};
}

#endif