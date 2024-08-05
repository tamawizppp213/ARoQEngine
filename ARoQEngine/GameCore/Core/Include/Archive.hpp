//////////////////////////////////////////////////////////////////////////////////
///  @file   Archive.hpp
///  @brief  temp
///  @author toide
///  @date   2024/08/03 23:47:56
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef ARCHIVE_HPP
#define ARCHIVE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/File/Private/Json/Include/JsonObject.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
namespace gu::file
{
	class Json;
}

/*! @brief キーバリューペアを生成します.*/
#define MAKE_ARCHIVE_PAIR(member) engine::Archive::MakePair(gu::tstring(SP(#member)), &member)
//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace engine
{
	/****************************************************************************
	*				  			ArchiveMode
	****************************************************************************/
	/* @brief  アーカイブを行う際の用途を表す列挙体
	*****************************************************************************/
	enum class ArchiveMode : gu::uint8
	{
		/*! @brief 何も行っていない*/
		None,

		/*! @brief ロード*/
		Load,

		/*! @brief セーブ*/
		Save,
	};

	/****************************************************************************
	*				  			OutputFileType
	****************************************************************************/
	/* @brief  出力するファイルの種類を表す列挙体. Json形式のみ対応
	*****************************************************************************/
	enum class OutputFileType : gu::uint8
	{
		/*! @brief Json*/
		Json
	};

	/****************************************************************************
	*				  			   Archive
	****************************************************************************/
	/* @brief  シリアライズ可能なオブジェクトの情報をファイルに保存することで, @n
	           エンジンを再起動しても情報を保持することができるクラス. Json形式で保存します. 暗号化まではまだ行いません.
	*****************************************************************************/
	class Archive
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief     Jsonファイルを対象のファイルパスから読み込みます. 
		*  @param[in] const gu::tstring& filePath : 読み込みたいJsonファイルのパス
		*  @return    bool
		*************************************************************************/
		bool Load(const gu::tstring& filePath);

		/*!**********************************************************************
		*  @brief     Jsonファイルを対象のファイルパスから読み込みます.
		*  @param[in] const gu::tstring& filePath : 読み込みたいJsonファイルのパス
		*  @return    bool
		*************************************************************************/
		bool Save(const gu::tstring& filePath);

		#pragma region MakePair
		__forceinline static gu::Pair<gu::tstring, gu::int8*> MakePair(const gu::tstring& key, gu::int8* value)
		{
			return gu::Pair<gu::tstring, gu::int8*>(key, value);
		}

		__forceinline static gu::Pair<gu::tstring, gu::int16*> MakePair(const gu::tstring& key, gu::int16* value)
		{
			return gu::Pair<gu::tstring, gu::int16*>(key, value);
		}

		__forceinline static gu::Pair<gu::tstring, gu::int32*> MakePair(const gu::tstring& key, gu::int32* value)
		{
			return gu::Pair<gu::tstring, gu::int32*>(key, value);
		}

		__forceinline static gu::Pair<gu::tstring, gu::int64*> MakePair(const gu::tstring& key, gu::int64* value)
		{
			return gu::Pair<gu::tstring, gu::int64*>(key, value);
		}

		__forceinline static gu::Pair<gu::tstring, gu::uint8*> MakePair(const gu::tstring& key, gu::uint8* value)
		{
			return gu::Pair<gu::tstring, gu::uint8*>(key, value);
		}

		__forceinline static gu::Pair<gu::tstring, gu::uint16*> MakePair(const gu::tstring& key, gu::uint16* value)
		{
			return gu::Pair<gu::tstring, gu::uint16*>(key, value);
		}

		__forceinline static gu::Pair<gu::tstring, gu::uint32*> MakePair(const gu::tstring& key, gu::uint32* value)
		{
			return gu::Pair<gu::tstring, gu::uint32*>(key, value);
		}

		__forceinline static gu::Pair<gu::tstring, gu::uint64*> MakePair(const gu::tstring& key, gu::uint64* value)
		{
			return gu::Pair<gu::tstring, gu::uint64*>(key, value);
		}

		__forceinline static gu::Pair<gu::tstring, gu::float32*> MakePair(const gu::tstring& key, gu::float32* value)
		{
			return gu::Pair<gu::tstring, gu::float32*>(key, value);
		}

		__forceinline static gu::Pair<gu::tstring, gu::double64*> MakePair(const gu::tstring& key, gu::double64* value)
		{
			return gu::Pair<gu::tstring, gu::double64*>(key, value);
		}

		__forceinline static gu::Pair<gu::tstring, bool*> MakePair(const gu::tstring& key, bool* value)
		{
			return gu::Pair<gu::tstring, bool*>(key, value);
		}

		__forceinline static gu::Pair<gu::tstring, gu::tstring*> MakePair(const gu::tstring& key, gu::tstring* value)
		{
			return gu::Pair<gu::tstring, gu::tstring*>(key, value);
		}

		#pragma endregion MakePair

		#pragma endregion 

		#pragma region Public Property
		/*!**********************************************************************
		*  @brief     現在のアーカイブ状態を取得します.
		*  @param[in] void
		*  @return    ArchiveState アーカイブ状態
		*************************************************************************/
		__forceinline ArchiveMode GetMode() const { return _mode; }

		/*!**********************************************************************
		*  @brief     読み込み中かどうかを判定します.
		*  @param[in] void
		*  @return    bool
		*************************************************************************/
		__forceinline bool IsLoadMode() const { return _mode == ArchiveMode::Load; }
		
		/*!**********************************************************************
		*  @brief     保存中かどうかを判定します.
		*  @param[in] void
		*  @return    bool
		*************************************************************************/
		__forceinline bool IsSaveMode() const { return _mode == ArchiveMode::Save; }

		#pragma endregion 

		#pragma region Public Operator 
		/*! @brief Jsonオブジェクトに数値を登録します [int8]*/
		Archive& operator << (const gu::Pair<gu::tstring, gu::int8*>& pair);

		/*! @brief Jsonオブジェクトに数値を登録します [int16]*/
		Archive& operator << (const gu::Pair<gu::tstring, gu::int16*>& pair);

		/*! @brief Jsonオブジェクトに数値を登録します [int32]*/
		Archive& operator << (const gu::Pair<gu::tstring, gu::int32*>& pair);

		/*! @brief Jsonオブジェクトに数値を登録します [int64]*/
		Archive& operator << (const gu::Pair<gu::tstring, gu::int64*>& pair);

		/*! @brief Jsonオブジェクトに数値を登録します [uint8]*/
		Archive& operator << (const gu::Pair<gu::tstring, gu::uint8*>& pair);

		/*! @brief Jsonオブジェクトに数値を登録します [uint16]*/
		Archive& operator << (const gu::Pair<gu::tstring, gu::uint16*>& pair);

		/*! @brief Jsonオブジェクトに数値を登録します [uint32]*/
		Archive& operator << (const gu::Pair<gu::tstring, gu::uint32*>& pair);

		/*! @brief Jsonオブジェクトに数値を登録します [uint64]*/
		Archive& operator << (const gu::Pair<gu::tstring, gu::uint64*>& pair);

		/*! @brief Jsonオブジェクトに数値を登録します [float]*/
		Archive& operator << (const gu::Pair<gu::tstring, gu::float32*>& pair);

		/*! @brief Jsonオブジェクトに数値を登録します [double]*/
		Archive& operator << (const gu::Pair<gu::tstring, gu::double64*>& pair);

		/*! @brief Jsonオブジェクトに数値を登録します [bool]*/
		Archive& operator << (const gu::Pair<gu::tstring, bool*>& pair);

		/*! @brief Jsonオブジェクトに文字列を登録します*/
		Archive& operator << (const gu::Pair<gu::tstring, gu::tstring*>& pair);

		#pragma endregion 

		#pragma region Public Constructor and Destructor
		/*! @brief デフォルトコンストラクタ*/
		Archive() = default;

		/*! @brief デストラクタ*/
	    ~Archive() = default;

		/*! @brief モード選択*/
		Archive(const ArchiveMode mode, const OutputFileType fileType = OutputFileType::Json);

		/*! @brief コピーコンストラクタ*/
		Archive(const Archive& ) = default;

		/*! @brief コピー演算子*/
		Archive& operator=(const Archive& ) = default;

		/*! @brief ムーブコンストラクタ*/
		Archive(Archive&& ) = default;

		#pragma endregion 

	protected:
		#pragma region Protected Constructor and Destructor

		#pragma endregion 

		#pragma region Protected Function

		#pragma endregion 

		#pragma region Protected Property

		/*! @brief アーカイブ状態*/
		ArchiveMode _mode = ArchiveMode::None;

		/*! @brief Jsonファイル*/
		gu::SharedPointer<gu::file::Json> _json = nullptr;

		/*! @brief Json用の書き込みオブジェクト*/
		gu::SharedPointer<gu::file::json::JsonObject> _object = nullptr;
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