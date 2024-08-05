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

/*! @brief �L�[�o�����[�y�A�𐶐����܂�.*/
#define MAKE_ARCHIVE_PAIR(member) engine::Archive::MakePair(gu::tstring(SP(#member)), &member)
//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace engine
{
	/****************************************************************************
	*				  			ArchiveMode
	****************************************************************************/
	/* @brief  �A�[�J�C�u���s���ۂ̗p�r��\���񋓑�
	*****************************************************************************/
	enum class ArchiveMode : gu::uint8
	{
		/*! @brief �����s���Ă��Ȃ�*/
		None,

		/*! @brief ���[�h*/
		Load,

		/*! @brief �Z�[�u*/
		Save,
	};

	/****************************************************************************
	*				  			OutputFileType
	****************************************************************************/
	/* @brief  �o�͂���t�@�C���̎�ނ�\���񋓑�. Json�`���̂ݑΉ�
	*****************************************************************************/
	enum class OutputFileType : gu::uint8
	{
		/*! @brief Json*/
		Json
	};

	/****************************************************************************
	*				  			   Archive
	****************************************************************************/
	/* @brief  �V���A���C�Y�\�ȃI�u�W�F�N�g�̏����t�@�C���ɕۑ����邱�Ƃ�, @n
	           �G���W�����ċN�����Ă�����ێ����邱�Ƃ��ł���N���X. Json�`���ŕۑ����܂�. �Í����܂ł͂܂��s���܂���.
	*****************************************************************************/
	class Archive
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief     Json�t�@�C����Ώۂ̃t�@�C���p�X����ǂݍ��݂܂�. 
		*  @param[in] const gu::tstring& filePath : �ǂݍ��݂���Json�t�@�C���̃p�X
		*  @return    bool
		*************************************************************************/
		bool Load(const gu::tstring& filePath);

		/*!**********************************************************************
		*  @brief     Json�t�@�C����Ώۂ̃t�@�C���p�X����ǂݍ��݂܂�.
		*  @param[in] const gu::tstring& filePath : �ǂݍ��݂���Json�t�@�C���̃p�X
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
		*  @brief     ���݂̃A�[�J�C�u��Ԃ��擾���܂�.
		*  @param[in] void
		*  @return    ArchiveState �A�[�J�C�u���
		*************************************************************************/
		__forceinline ArchiveMode GetMode() const { return _mode; }

		/*!**********************************************************************
		*  @brief     �ǂݍ��ݒ����ǂ����𔻒肵�܂�.
		*  @param[in] void
		*  @return    bool
		*************************************************************************/
		__forceinline bool IsLoadMode() const { return _mode == ArchiveMode::Load; }
		
		/*!**********************************************************************
		*  @brief     �ۑ������ǂ����𔻒肵�܂�.
		*  @param[in] void
		*  @return    bool
		*************************************************************************/
		__forceinline bool IsSaveMode() const { return _mode == ArchiveMode::Save; }

		#pragma endregion 

		#pragma region Public Operator 
		/*! @brief Json�I�u�W�F�N�g�ɐ��l��o�^���܂� [int8]*/
		Archive& operator << (const gu::Pair<gu::tstring, gu::int8*>& pair);

		/*! @brief Json�I�u�W�F�N�g�ɐ��l��o�^���܂� [int16]*/
		Archive& operator << (const gu::Pair<gu::tstring, gu::int16*>& pair);

		/*! @brief Json�I�u�W�F�N�g�ɐ��l��o�^���܂� [int32]*/
		Archive& operator << (const gu::Pair<gu::tstring, gu::int32*>& pair);

		/*! @brief Json�I�u�W�F�N�g�ɐ��l��o�^���܂� [int64]*/
		Archive& operator << (const gu::Pair<gu::tstring, gu::int64*>& pair);

		/*! @brief Json�I�u�W�F�N�g�ɐ��l��o�^���܂� [uint8]*/
		Archive& operator << (const gu::Pair<gu::tstring, gu::uint8*>& pair);

		/*! @brief Json�I�u�W�F�N�g�ɐ��l��o�^���܂� [uint16]*/
		Archive& operator << (const gu::Pair<gu::tstring, gu::uint16*>& pair);

		/*! @brief Json�I�u�W�F�N�g�ɐ��l��o�^���܂� [uint32]*/
		Archive& operator << (const gu::Pair<gu::tstring, gu::uint32*>& pair);

		/*! @brief Json�I�u�W�F�N�g�ɐ��l��o�^���܂� [uint64]*/
		Archive& operator << (const gu::Pair<gu::tstring, gu::uint64*>& pair);

		/*! @brief Json�I�u�W�F�N�g�ɐ��l��o�^���܂� [float]*/
		Archive& operator << (const gu::Pair<gu::tstring, gu::float32*>& pair);

		/*! @brief Json�I�u�W�F�N�g�ɐ��l��o�^���܂� [double]*/
		Archive& operator << (const gu::Pair<gu::tstring, gu::double64*>& pair);

		/*! @brief Json�I�u�W�F�N�g�ɐ��l��o�^���܂� [bool]*/
		Archive& operator << (const gu::Pair<gu::tstring, bool*>& pair);

		/*! @brief Json�I�u�W�F�N�g�ɕ������o�^���܂�*/
		Archive& operator << (const gu::Pair<gu::tstring, gu::tstring*>& pair);

		#pragma endregion 

		#pragma region Public Constructor and Destructor
		/*! @brief �f�t�H���g�R���X�g���N�^*/
		Archive() = default;

		/*! @brief �f�X�g���N�^*/
	    ~Archive() = default;

		/*! @brief ���[�h�I��*/
		Archive(const ArchiveMode mode, const OutputFileType fileType = OutputFileType::Json);

		/*! @brief �R�s�[�R���X�g���N�^*/
		Archive(const Archive& ) = default;

		/*! @brief �R�s�[���Z�q*/
		Archive& operator=(const Archive& ) = default;

		/*! @brief ���[�u�R���X�g���N�^*/
		Archive(Archive&& ) = default;

		#pragma endregion 

	protected:
		#pragma region Protected Constructor and Destructor

		#pragma endregion 

		#pragma region Protected Function

		#pragma endregion 

		#pragma region Protected Property

		/*! @brief �A�[�J�C�u���*/
		ArchiveMode _mode = ArchiveMode::None;

		/*! @brief Json�t�@�C��*/
		gu::SharedPointer<gu::file::Json> _json = nullptr;

		/*! @brief Json�p�̏������݃I�u�W�F�N�g*/
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