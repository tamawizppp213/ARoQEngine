//////////////////////////////////////////////////////////////////////////////////
///  @file   WindowsOS.hpp
///  @brief  OS�����܂Ƃ߂��N���X. ���W�X�g���L�[�̎擾�Ȃǂ��s���܂�.
///  @author toide
///  @date   2024/06/23 23:42:51
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef WINDOWS_OS_HPP
#define WINDOWS_OS_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/GUString.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace platform::windows
{
	/****************************************************************************
	*				  			   WindowsOS
	*************************************************************************//**
	/*  @brief   OS�����܂Ƃ߂��N���X
	*****************************************************************************/
	class WindowsOS
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief     ���W�X�g���L�[��T���܂�.�@
		*  @param[in] void* hKey ���[�g�L�[
		*  @param[in] const tstring& subKey �T�u�L�[
		*  @param[in] const tstring& valueName �l��
		*  @param[out] tstring& �L�[�̒l
		*  @return    bool �L�[�����݂��邩�ǂ���
		*************************************************************************/
		static bool QueryRegistryKey(void* hKey, const gu::tstring& subKey, const gu::tstring& valueName, gu::tstring& outData);

		/*!**********************************************************************
		*  @brief     Registry�L�[�ɑΉ�����l���擾���܂�. ���̊֐���User��CurrentUser�ɑ��݂���L�[�݂̂�T���܂�.
		*  @param[in] const gu::tstring& �L�[�����݂���ꏊ
		*  @param[in] const gu::tstring& �L�[��
		*  @param[out]tstring& �L�[�̒l
		*  @return    bool �����������ǂ���
		*************************************************************************/
		static bool GetRegistryKeyValue(const gu::tstring& keyPlace, const gu::tstring& valueName, gu::tstring& outValue);

		/*!**********************************************************************
		*  @brief     Registry�L�[�ɑΉ�����l��ݒ肵�܂�. ���̊֐���User��CurrentUser�ɑ��݂���L�[�݂̂�T���܂�.
		*  @param[in] const gu::tstring& �L�[�����݂���ꏊ
		*  @param[in] const gu::tstring& �L�[��
		*  @param[in] tstring& �L�[�̒l
		*  @return    bool �����������ǂ���
		*************************************************************************/
		static bool SetRegistryKeyValue(const gu::tstring& keyPlace, const gu::tstring& valueName, const gu::tstring& value);

		/*!**********************************************************************
		*  @brief     Registry�L�[�ɑΉ�����l���폜���܂�. ���̊֐���User��CurrentUser�ɑ��݂���L�[�݂̂�T���܂�.
		*  @param[in] const gu::tstring& �L�[�����݂���ꏊ
		*  @param[in] const gu::tstring& �L�[��
		*  @return    bool �����������ǂ���
		*************************************************************************/
		static bool DeleteRegistryKeyValue(const gu::tstring& keyPlace, const gu::tstring& valueName);

		/*!**********************************************************************
		*  @brief     Registry�L�[���̂��폜���܂�. ���̊֐���User��CurrentUser�ɑ��݂���L�[�݂̂�T���܂�.
		*  @param[in] const gu::tstring& �L�[�����݂���ꏊ
		*  @param[in] const bool ���w�̃L�[���폜���邩
		*  @return    bool �����������ǂ���
		*************************************************************************/
		static bool DeleteRegistryKey(const gu::tstring& keyPlace, const bool isRecursive);

		/*!**********************************************************************
		*  @brief     OS��ID���擾���܂�
		*  @param[in] void
		*  @return    tstring ID��
		*************************************************************************/
		static gu::tstring GetOperatingSystemID();

		/*!**********************************************************************
		*  @brief     �Ō�̃G���[�R�[�h���擾���܂�
		*  @param[in] void
		*  @return    gu::uint32 �G���[�R�[�h
		*************************************************************************/
		static gu::uint32 GetLastError();

		/*!**********************************************************************
		*  @brief     �Ō�̃G���[�R�[�h��ݒ肵�܂�
		*  @param[in] void
		*  @return    const gu::uint32 �G���[�R�[�h
		*************************************************************************/
		static void SetLastError(const gu::uint32 errorCode);

		/*!**********************************************************************
		*  @brief     COM���C�u���������������܂�
		*  @param[in] const bool �}���`�X���b�h���g�p���邩
		*  @return    bool
		*************************************************************************/
		static bool CoInitialize(const bool useMultiThread = false);

		/*!**********************************************************************
		*  @brief     COM���C�u�������I�����܂�
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		static void CoUninitialize();

		/*!**********************************************************************
		*  @brief     �w�肳�ꂽ�t�@�C������DLL�����[�h��, ���̃n���h����Ԃ��܂�. @n
		*             https://qiita.com/tera1707/items/727e7ad4dc9f3205c137
		*  @param[in] const tstring& fiieName : ���[�h������DLL�̃t�@�C����
		*  @return    void* : ���[�h����DLL�̃n���h��
		*************************************************************************/
		static void* GetDLLHandle(const gu::tstring& fileName);

		/*!**********************************************************************
		*  @brief     �w�肳�ꂽ�t�@�C������DLL�����������������܂�
		*  @param[in] void* handle : ���[�h����DLL�̃n���h��
		*  @return    bool ����
		*************************************************************************/
		static bool FreeDLLHandle(void* handle);

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
}

#endif