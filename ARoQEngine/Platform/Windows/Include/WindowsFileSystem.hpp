//////////////////////////////////////////////////////////////////////////////////
///  @file   WindowsFileSystem.hpp
///  @brief  �t�@�C���̓ǂݏ�����, �t�@�C�������݂��邩�̊m�F, �f�B���N�g���̍쐬��폜�Ȃǂ̊�{�I�ȃt�@�C������̋@�\��񋟂��܂�.
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
	/*! @brief     �t�@�C���̓ǂݏ�����, �t�@�C�������݂��邩�̊m�F, �f�B���N�g���̍쐬��폜�Ȃǂ̊�{�I�ȃt�@�C������̋@�\��񋟂��܂�.
	*****************************************************************************/
	class IFileSystem : public platform::core::IFileSystem
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief     �t�@�C�����J���ēǂݍ��݂܂�. ���̎�, ���s���̏󋵂ɉ����ē���/�񓯊��������Ő؂�ւ��܂�.  
		*  @param[in] const tstring& �t�@�C���p�X
		*  @param[in] const bool   �t�@�C���̏������݂��s���܂�.
		*  @param[in] const bool   �񓯊��ǂݍ��݂����s���邩��ݒ肵�܂� (�f�t�H���g�͓����ǂݍ���)
		*  @return    IFileHandle* �t�@�C���̃n���h��
		*************************************************************************/
		virtual gu::SharedPointer<core::file::IFileHandle> OpenRead(const gu::tstring& fileName, const bool allowWrite = false, const bool useAsync = false) override;

		/*!**********************************************************************
		*  @brief     �t�@�C�����J���ď������݂܂�. ���̎�, ���s���̏󋵂ɉ����ē���/�񓯊��������Ő؂�ւ��܂�
		*  @param[in] const tstring& �t�@�C���p�X
		*  @param[in] const bool   ���ɏ������܂ꂽ�������㏑������, ���ɒǉ����邩�����߂܂� (Default: �V�K�㏑��)
		*  @param[in] const bool   �t�@�C���̉{�����s���܂�. (Default : false)
		*   @param[in] const bool  �񓯊��������݂����s���邩��ݒ肵�܂� (�f�t�H���g�͓�����������)
		*  @return    IFileHandle* �t�@�C���̃n���h��
		*************************************************************************/
		virtual gu::SharedPointer<core::file::IFileHandle> OpenWrite(const gu::tstring& fileName, const bool append = false, const bool allowRead = false, const bool useAsync = false) override;

		/*!**********************************************************************
		*  @brief �w�肵���t�@�C�������݂��邩���m�F���܂�.
		*  @param[in] const tstring& �t�@�C���p�X
		*  @return    bool �t�@�C�������݂���ꍇ, true��Ԃ��܂�.
		*************************************************************************/
		virtual bool FileExists(const gu::tstring& filePath) const override;

		/*!**********************************************************************
		*  @brief �w�肵���t�@�C�������݂��邩���m�F���܂�.
		*  @param[in] const tstring& �f�B���N�g��
		*  @return    bool �t�@�C�������݂���ꍇ, true��Ԃ��܂�.
		*************************************************************************/
		virtual bool DirectoryExists(const gu::tstring& directory) const override;

		/*!**********************************************************************
		*  @brief     �f�B���N�g�����쐬���܂�. ���Ƀf�B���N�g�������݂���ꍇ��true��Ԃ��܂�.
		*  @param[in] const tstring& �f�B���N�g��
		*  @return    bool �f�B���N�g�����쐬�o�����ꍇ, ���Ƀf�B���N�g�������݂���ꍇ��true��Ԃ��܂�
		*************************************************************************/
		virtual bool CreateDirectory(const gu::tstring& directory) const override;

		/*!**********************************************************************
		*  @brief     �f�B���N�g�����폜���܂�.
		*  @param[in] const tstring& �f�B���N�g��
		*  @return    bool �f�B���N�g�����쐬�o�����ꍇ��true��Ԃ��܂�
		*************************************************************************/
		virtual bool DeleteDirectory(const gu::tstring& directory) const override;

		/*!**********************************************************************
		*  @brief     �f�B���N�g�����폜���܂�.
		*  @param[in] const tstring& �f�B���N�g��
		*  @return    bool �f�B���N�g�����폜�o�����ꍇ��true��Ԃ��܂�
		*************************************************************************/
		virtual bool DeleteFile(const gu::tstring& filePath) const override;

		/*!**********************************************************************
		*  @brief     �t�@�C�����R�s�[���܂�
		*  @param[in] const tstring& �R�s�[�O�̃t�@�C���p�X
		*  @param[in] const tstring& �R�s�[��̃t�@�C���p�X
		*  @param[in] const bool   �㏑���������邩��ݒ肵�܂�
		*  @return    bool �t�@�C�����R�s�[�ł����ꍇ��true��Ԃ��܂�
		*************************************************************************/
		virtual bool CopyFile(const gu::tstring& from, const gu::tstring& to, const bool overwrite) const override;

		/*!**********************************************************************
		*  @brief     �t�@�C�����ړ����܂�
		*  @param[in] const tstring& �ړ��O�̃t�@�C���p�X
		*  @param[in] const tstring& �ړ���̃t�@�C���p�X
		*  @return    bool �t�@�C�����ړ��ł����ꍇ��true��Ԃ��܂�
		*************************************************************************/
		virtual bool MoveFile(const gu::tstring& from, const gu::tstring& to) const override;

		/*!**********************************************************************
		*  @brief     �ǂݎ�肾�������o���Ȃ��t�@�C�����ǂ���
		*  @param[in] const tstring& �t�@�C���p�X
		*  @return    bool Readonly�ł����true
		*************************************************************************/
		virtual bool IsReadOnly(const gu::tstring& filePath) const override;

		/*!**********************************************************************
		*  @brief     �t�@�C��, �������̓f�B���N�g���̃����N(�Q��)�̎�ނ�Ԃ��܂�.
		*  @param[in] const tstring& �t�@�C���p�X, �������̓f�B���N�g��
		*  @return    file::LinkType �����N�̎��
		*************************************************************************/
		virtual platform::core::file::LinkType LinkType(const gu::tstring& path) const override;

		/*!**********************************************************************
		*  @brief     �w�肵���t�@�C���p�X�̊g���q���擾���܂�
		*  @param[in] const tstring& �t�@�C���p�X
		*  @return    gu::tstring �g���q�ł�. (������Ȃ��ꍇ��nullptr��Ԃ��܂�)
		*************************************************************************/
		virtual gu::tstring GetExtension(const gu::tstring& filePath) const override;

		/*!**********************************************************************
		*  @brief     �f�B���N�g��(�t�@�C�����̎�O�܂�)���擾���܂�. 
		*  @param[in] const tstring& �t�@�C���p�X
		*  @return    gu::tstring�t�@�C������O�܂ł̃f�B���N�g��
		*************************************************************************/
		virtual gu::tstring GetDirectory(const gu::tstring& filePath) const override;

		/*!**********************************************************************
		*  @brief     �f�B���N�g������苎���ăt�@�C�������擾���܂�
		*  @param[in] const tstring& �f�B���N�g�����܂߂��t�@�C����
		*  @return    gu::tstring
		*************************************************************************/
		virtual gu::tstring GetFileName(const gu::tstring& filePath) const override;

		/*!**********************************************************************
		*  @brief     ���g�̃v���Z�X�Ɋւ����ƃf�B���N�g�����擾���܂�. 
		*  @param[in] void
		*  @return    gu::tstring ��ƃf�B���N�g��
		*************************************************************************/
		virtual gu::tstring GetProcessDirectory() const override;

		/*!**********************************************************************
		*  @brief     �Ώۂ̃f�B���N�g�������Ƀn�[�h�����N���쐬���܂�. @n
		*             �n�[�h�����N��, �t�@�C���̓��e�����L����t�@�C���̂��Ƃł�.
		*  @param[in] const tstring& �����N���s���������̃t�@�C���p�X
		*  @param[in] const tstring& �����N���쐬�������t�@�C���p�X
		*  @return    bool
		*************************************************************************/
		virtual bool CreateHardLink(const gu::tstring& sourcePath, const gu::tstring& newHardLinkPath) const override;

		/*!**********************************************************************
		*  @brief     �Ώۂ̃f�B���N�g�������ɃV���{���b�N�����N���쐬���܂�. @n
		*             �V���{���b�N�����N��, �t�@�C���̃p�X���Q�Ƃ���t�@�C���̂��Ƃł�.
		*  @param[in] const tstring& �����N���s���������̃t�@�C���p�X
		*  @param[in] const tstring& �����N���쐬�������t�@�C���p�X
		*  @return    bool
		*************************************************************************/
		virtual bool CreateSymbolicLink(const gu::tstring& sourcePath, const gu::tstring& newSymbolicLinkPath) const override;
		#pragma endregion 

		#pragma region Public Property

		#pragma endregion 

		#pragma region Public Operator 
		/*!**********************************************************************
		*  @brief     ������������g�p�ł��邩 (32767�����܂Ŏg�p�\��)
		*  @param[in] void
		*  @return    true�̏ꍇ32765�����܂ŉ\��, false�̏ꍇ260�܂ŉ\
		*************************************************************************/
		bool IsLongPathEnabled() const { return IS_LONG_PATH_ENABLED; }

		#pragma endregion 

		#pragma region Public Constructor and Destructor
		/*! @brief �f�t�H���g�R���X�g���N�^*/
		IFileSystem();

		/*! @brief �f�X�g���N�^*/
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
		/*! true�̏ꍇ32767�����܂ŉ\��, false�̏ꍇ260�܂ŉ\ */
		static bool IS_LONG_PATH_ENABLED;
		#pragma endregion 

	};
}

#endif