//////////////////////////////////////////////////////////////////////////////////
///  @file   CoreFileSystem.hpp
///  @brief  �t�@�C���̓ǂݏ�����, �t�@�C�������݂��邩�̊m�F, �f�B���N�g���̍쐬��폜�Ȃǂ̊�{�I�ȃt�@�C������̋@�\��񋟂��܂�.
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
	/*! @brief : �t�@�C���̓ǂݏ�����, �t�@�C�������݂��邩�̊m�F, �f�B���N�g���̍쐬��폜�Ȃǂ̊�{�I�ȃt�@�C������̋@�\��񋟂��܂�.
	*****************************************************************************/
	class IFileSystem : public gu::NonCopyAndMove
	{
	public:
		/*!**********************************************************************
		*  @brief     �V���O���g���C���X�^���X���擾���܂�. (��{�I�Ƀ����o�ϐ����g��Ȃ�����, ���Ɉ��e���͂���܂���)
		*  @param[in] const bool �����I�ɏ��������s���܂�. (Default: false)
		*  @return    gu::SharedPointer<IFileSystem> �t�@�C���V�X�e���̃C���X�^���X
		*************************************************************************/
		static gu::SharedPointer<IFileSystem> Get(const bool forceInitialize = false);

		#pragma region Public Function
		/*!**********************************************************************
		*  @brief     �t�@�C�����J���ēǂݍ��݂܂�. ���̎�, ���s���̏󋵂ɉ����ē���/�񓯊��������Ő؂�ւ��܂�.  
		*  @param[in] const tchar* �t�@�C���p�X
		*  @param[in] const bool   �t�@�C���̏������݂��s���܂�.
		*  @param[in] const bool   �񓯊��ǂݍ��݂����s���邩��ݒ肵�܂� (�f�t�H���g�͓����ǂݍ���)
		*  @return    IFileHandle* �t�@�C���̃n���h��
		*************************************************************************/
		virtual gu::SharedPointer<file::IFileHandle> OpenRead(const gu::tchar* fileName, const bool allowWrite = false, const bool useAsync = false) = 0;

		/*!**********************************************************************
		*  @brief     �t�@�C�����J���ď������݂܂�. ���̎�, ���s���̏󋵂ɉ����ē���/�񓯊��������Ő؂�ւ��܂�
		*  @param[in] const tchar* �t�@�C���p�X
		*  @param[in] const bool   ���ɏ������܂ꂽ�������㏑������, ���ɒǉ����邩�����߂܂� (Default: �V�K�㏑��)
		*  @param[in] const bool   �t�@�C���̉{�����s���܂�. (Default : false)
		*   @param[in] const bool  �񓯊��������݂����s���邩��ݒ肵�܂� (�f�t�H���g�͓�����������)
		*  @return    IFileHandle* �t�@�C���̃n���h��
		*************************************************************************/
		virtual gu::SharedPointer<file::IFileHandle> OpenWrite(const gu::tchar* fileName, const bool append = false, const bool allowRead = false, const bool useAsync = false) = 0;

		/*!**********************************************************************
		*  @brief �w�肵���t�@�C�������݂��邩���m�F���܂�. 
		*  @param[in] const tchar* �t�@�C���p�X
		*  @return    bool �t�@�C�������݂���ꍇ, true��Ԃ��܂�.
		*************************************************************************/
		virtual bool FileExists(const gu::tchar* filePath) const = 0;

		/*!**********************************************************************
		*  @brief �w�肵���t�@�C�������݂��邩���m�F���܂�.
		*  @param[in] const tchar* �f�B���N�g��
		*  @return    bool �t�@�C�������݂���ꍇ, true��Ԃ��܂�.
		*************************************************************************/
		virtual bool DirectoryExists(const gu::tchar* directory) const = 0;

		/*!**********************************************************************
		*  @brief     �f�B���N�g�����쐬���܂�. ���Ƀf�B���N�g�������݂���ꍇ��true��Ԃ��܂�.  
		*  @param[in] const tchar* �f�B���N�g��
		*  @return    bool �f�B���N�g�����쐬�o�����ꍇ, ���Ƀf�B���N�g�������݂���ꍇ��true��Ԃ��܂�
		*************************************************************************/
		virtual bool CreateDirectory(const gu::tchar* directory) const = 0;

		/*!**********************************************************************
		*  @brief     �f�B���N�g�����폜���܂�.
		*  @param[in] const tchar* �f�B���N�g��
		*  @return    bool �f�B���N�g�����폜�o�����ꍇ��true��Ԃ��܂�
		*************************************************************************/
		virtual bool DeleteDirectory(const gu::tchar* directory) const = 0;

		/*!**********************************************************************
		*  @brief     �t�@�C�����폜���܂�.
		*  @param[in] const tchar* �t�@�C���p�X
		*  @return    bool �f�B���N�g�����쐬�o�����ꍇ��true��Ԃ��܂�
		*************************************************************************/
		virtual bool DeleteFile(const gu::tchar* filePath) const = 0;

		/*!**********************************************************************
		*  @brief     �t�@�C�����R�s�[���܂�
		*  @param[in] const tchar* �R�s�[�O�̃t�@�C���p�X
		*  @param[in] const tchar* �R�s�[��̃t�@�C���p�X
		*  @param[in] const bool   �㏑���������邩��ݒ肵�܂�
		*  @return    bool �t�@�C�����R�s�[�ł����ꍇ��true��Ԃ��܂�
		*************************************************************************/
		virtual bool CopyFile(const gu::tchar* from, const gu::tchar* to, const bool overwrite) const = 0;

		/*!**********************************************************************
		*  @brief     �t�@�C�����ړ����܂�
		*  @param[in] const tchar* �ړ��O�̃t�@�C���p�X
		*  @param[in] const tchar* �ړ���̃t�@�C���p�X
		*  @return    bool �t�@�C�����ړ��ł����ꍇ��true��Ԃ��܂�
		*************************************************************************/
		virtual bool MoveFile(const gu::tchar* from, const gu::tchar* to) const = 0;

		/*!**********************************************************************
		*  @brief     �ǂݎ�肾�������o���Ȃ��t�@�C�����ǂ���
		*  @param[in] const tchar* �t�@�C���p�X
		*  @return    bool Readonly�ł����true
		*************************************************************************/
		virtual bool IsReadOnly(const gu::tchar* filePath) const = 0;

		/*!**********************************************************************
		*  @brief     �t�@�C��, �������̓f�B���N�g���̃����N(�Q��)�̎�ނ�Ԃ��܂�.
		*  @param[in] const tchar* �t�@�C���p�X, �������̓f�B���N�g��
		*  @return    file::LinkType �����N�̎��
		*************************************************************************/
		virtual file::LinkType LinkType(const gu::tchar* path) const = 0;

		/*!**********************************************************************
		*  @brief     �w�肵���t�@�C���p�X�̊g���q���擾���܂�
		*  @param[in] const tchar* �t�@�C���p�X
		*  @return    const tchar* �g���q�ł�. (������Ȃ��ꍇ��nullptr��Ԃ��܂�)
		*************************************************************************/
		virtual gu::tstring GetExtension(const gu::tchar* filePath) const = 0;

		/*!**********************************************************************
		*  @brief     �f�B���N�g��(�t�@�C�����̎�O�܂�)���擾���܂�.
		*  @param[in] const tchar* �t�@�C���p�X
		*  @return    const char* �t�@�C������O�܂ł̃f�B���N�g��
		*************************************************************************/
		virtual gu::tstring GetDirectory(const gu::tchar* filePath) const = 0;

		/*!**********************************************************************
		*  @brief     ���g�̃v���Z�X�Ɋւ����ƃf�B���N�g�����擾���܂�.
		*  @param[in] void
		*  @return    gu::tstring ��ƃf�B���N�g��
		*************************************************************************/
		virtual gu::tstring GetProcessDirectory() const = 0;

		/*!**********************************************************************
		*  @brief     �Ώۂ̃f�B���N�g�������Ƀn�[�h�����N���쐬���܂�. @n
		*             �n�[�h�����N��, �t�@�C���̓��e�����L����t�@�C���̂��Ƃł�.
		*  @param[in] const tchar* �����N���s���������̃t�@�C���p�X
		*  @param[in] const tchar* �����N���쐬�������t�@�C���p�X
		*  @return    bool
		*************************************************************************/
		virtual bool CreateHardLink(const gu::tchar* sourcePath, const gu::tchar* newHardLinkPath) const = 0;

		/*!**********************************************************************
		*  @brief     �Ώۂ̃f�B���N�g�������ɃV���{���b�N�����N���쐬���܂�. @n
		*             �V���{���b�N�����N��, �t�@�C���̃p�X���Q�Ƃ���t�@�C���̂��Ƃł�.
		*  @param[in] const tchar* �����N���s���������̃t�@�C���p�X
		*  @param[in] const tchar* �����N���쐬�������t�@�C���p�X
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