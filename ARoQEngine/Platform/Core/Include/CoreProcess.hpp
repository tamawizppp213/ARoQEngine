//////////////////////////////////////////////////////////////////////////////////
///  @file   CorePlatformProcess.hpp
///  @brief  OS�̃v���Z�X�𑀍삷��N���X
///  @author toide
///  @date   2024/06/15 22:31:36
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef CORE_PROCESS_HPP
#define CORE_PROCESS_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/GUClassUtility.hpp"
#include "GameUtility/Base/Include/GUSmartPointer.hpp"	
#include "GameUtility/Base/Include/GUString.hpp"
#include "GameUtility/Container/Include/GUDynamicArray.hpp"
#include "Platform/Core/Private/Process/Include/CoreProcessSetting.hpp"

#undef CreateProcess
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace platform::core
{
	/****************************************************************************
	*				  			ProcessHandle
	****************************************************************************/
	/*! @brief OS�̃v���Z�X�������n���h��
	*****************************************************************************/
	struct ProcessHandle
	{
	public:
		/*! @brief �n���h���̎擾*/
		void* GetHandle() const { return _handle; }

		/*! @brief �n���h���̃��Z�b�g*/
		void Reset() { _handle = nullptr; }

		/*! @brief �n���h���̗L����*/
		bool IsValid() const { return _handle != nullptr; }

		/*! @brief �f�t�H���g�R���X�g���N�^*/
		ProcessHandle() = default;

		/*! @brief �������R���X�g���N�^*/
		ProcessHandle(void* handle) : _handle(handle) {}

		/*! @brief �f�X�g���N�^*/

	protected:
		/*! @brief Platform�ŗL�̃n���h���|�C���^*/
		void* _handle = nullptr;
	};

	/****************************************************************************
	*				  			   CoreProcess
	****************************************************************************/
	/*! @brief OS�̃v���Z�X�𑀍삷��N���X
	*****************************************************************************/
	class CoreProcess : public gu::NonCopyAndMove
	{
	public:
		/*!**********************************************************************
		*  @brief     �V���O���g���C���X�^���X���擾���܂�. (��{�I�Ƀ����o�ϐ����g��Ȃ�����, ���Ɉ��e���͂���܂���)
		*  @param[in] const bool �����I�ɏ��������s���܂�. (Default: false)
		*  @return    gu::SharedPointer<CorePlatformProcess> �t�@�C���V�X�e���̃C���X�^���X
		*************************************************************************/
		static gu::SharedPointer<CoreProcess> Get(const bool forceInitialize = false);

		#pragma region Public Function
		
		#pragma region DLL Function
		/*!**********************************************************************
		*  @brief     �w�肳�ꂽ�t�@�C������DLL�����[�h��, ���̃n���h����Ԃ��܂�. @n
		*             https://qiita.com/tera1707/items/727e7ad4dc9f3205c137
		*  @param[in] const tstring& fiieName : ���[�h������DLL�̃t�@�C����
		*  @return    void* : ���[�h����DLL�̃n���h��
		*************************************************************************/
		virtual void* GetDLLHandle(const gu::tstring& fileName) const = 0;

		/*!**********************************************************************
		*  @brief     �w�肳�ꂽ�t�@�C������DLL�����������������܂�
		*  @param[in] void* handle : ���[�h����DLL�̃n���h��
		*  @return    bool ����
		*************************************************************************/
		virtual bool FreeDLLHandle(void* handle) const = 0;

		#pragma endregion DLL Function

		#pragma region Process Function

		/*!**********************************************************************
		*  @brief     �{�v���Z�X�̃v���Z�XID��Ԃ��܂�.
		*  @param[in] void
		*  @return    uint32 �v���Z�XID
		*************************************************************************/
		virtual gu::uint32 GetCurrentProcessID() const = 0;

		/*!**********************************************************************
		*  @brief     ���̊֐��̌Ăяo�����Ɍ��݂̃X���b�h�����s����Ă����v���Z�b�T�̐�(���ł͂���܂���)���擾
		*  @param[in] void
		*  @return    uint32 �v���Z�b�T��
		*************************************************************************/
		virtual gu::uint32 GetCurrentProcessorNumber() const = 0;

		/*!**********************************************************************
		*  @brief     OS���ǂ̃v���Z�b�T�ŃX���b�h, �v���Z�X�����s����ׂ�����ݒ肵�܂�
		*  @param[in] const uint64 affnity : �v���Z�b�T�̃r�b�g�}�X�N
		*  @return    bool
		*************************************************************************/
		virtual bool SetProcessAffinity(const gu::uint64 affnity) const = 0;

		/*!**********************************************************************
		*  @brief     Process�̐�����Primary�X���b�h���쐬���邽�߂̊֐�. �Ăяo�����v���Z�X�̃Z�L�����e�B�E�R���e�L�X�g���Ŏw�肳�ꂽ���s�t�@�C�����N�����܂�
		*  @param[in] const tstring&                     : ���s����v���O�����̃p�X
		*  @param[in] const tstring&                     : �v���O�����ɓn���p�����[�^
		*  @param[in] const bool                         : �v���Z�X���f�^�b�`���Ď��s���邩�ǂ���, �V�����v���Z�X���Ǝ��̃E�B���h�E�ŋN�����܂�
		*  @param[in] const core::ProcessWindowActivate  : �v���Z�X�̃E�B���h�E�̕\�� 
		*  @param[out] uint32*                           : �v���Z�XID
		*  @param[in] const ProcessPriority              : �v���Z�X�̗D��x
		*  @return    bool
		*************************************************************************/
		virtual ProcessHandle CreateProcess(const gu::tstring& programPath, const gu::tstring& parameters, 
			const bool launchDetached, const core::ProcessWindowActivate activate, 
			gu::uint32* outProcessID, const ProcessPriority priority) const = 0;

		/*!**********************************************************************
		*  @brief     ���ɍ쐬���ꂽ�v���Z�X�n���h�����J���܂�
		*  @param[in] const gu::uint32 processID : �v���Z�XID
		*  @return    ProcessHandle : �v���Z�X�n���h��
		*************************************************************************/
		virtual ProcessHandle OpenProcessHandle(const gu::uint32 processID) const = 0;

		/*!**********************************************************************
		*  @brief     �v���Z�X�����, ���\�[�X��������邽�߂Ɏg�p���܂�. 
		*  @param[in] const gu::uint32 processID : �v���Z�XID
		*  @return    void
		*************************************************************************/
		virtual void CloseProcessHandle(ProcessHandle& handle) const = 0;

		/*!**********************************************************************
		*  @brief     �v���Z�X�������I�����܂�. ���̍�, killTree��true�̏ꍇ, �q�v���Z�X���I�����܂�
		*  @param[in] ProcessHandle& : �v���Z�X�n���h��
		*  @param[in] const bool : �q�v���Z�X���I�����邩�ǂ���
		*  @return    void
		*************************************************************************/
		virtual void TerminateProcess(ProcessHandle& handle, const bool killTree) const = 0;

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
		virtual bool ExecuteProcess(const gu::tstring& programPath, const gu::tstring& parameters, gu::int32* outReturnCode, gu::tstring* stdOutput, gu::tstring* stdError, gu::tstring* optionWorkingDirectory = nullptr, const bool shouldEndWithParentProcess = false) = 0;

		/*!**********************************************************************
		*  @brief     �Ǘ��҂Ƃ��ăv���Z�X�����s���܂�. �K�v�ɉ����ĊǗ��҂ւ̏��i���s���܂�. 
		*  @param[in]  const tstring&  : ���s����v���O�����̃p�X
		*  @param[in]  const tstring&  : �v���O�����ɓn���p�����[�^
		*  @param[out] int32*          : return�R�[�h
		*  @return     bool ����
		*************************************************************************/
		virtual bool ExecuteElevatedProcess(const gu::tstring* programPath, const gu::tstring& parameter, gu::int32* outReturnCode) const = 0;

		/*!**********************************************************************
		*  @brief      �Ǘ��҂Ƃ��ăv���Z�X�����s���܂�. �K�v�ɉ����ĊǗ��҂ւ̏��i���s���܂�.
		*  @param[in]  const tstring&  : ���s����v���O�����̃p�X
		*  @param[in]  const tstring&  : �v���O�����ɓn���p�����[�^
		*  @return     ProcessHandle
		*************************************************************************/
		virtual ProcessHandle ExecuteElevatedProcess(const gu::tstring* programPath, const gu::tstring& parameter) const = 0;

		/*!**********************************************************************
		*  @brief     �v���Z�X�n���h���𒼐ړn����, �v���Z�X���s���ł��邩��Ԃ��܂�.
		*  @param[in] const ProcessHandle& : �v���Z�X�n���h��
		*  @return    bool �v���Z�X���s���ł��邩�ǂ���
		*************************************************************************/
		virtual bool IsProcessRunning(const ProcessHandle& handle) const = 0;

		/*!**********************************************************************
		*  @brief     �v���Z�XID���g����, �v���Z�X�����s���ł��邩��Ԃ��܂�
		*  @param[in] const gu::uint32 : �v���Z�XID
		*  @return    bool �v���Z�X���s���ł��邩�ǂ���
		*************************************************************************/
		virtual bool IsProcessRunning(const gu::uint32 processID) const = 0;

		/*!**********************************************************************
		*  @brief     �v���Z�X�n���h���𒼐ړn����, �v���Z�X���s���ł��邩��Ԃ��܂�.
		*  @param[in] const gu::tstring& �v���Z�X��
		*  @return    bool �v���Z�X���s���ł��邩�ǂ���
		*************************************************************************/
		virtual bool IsProcessRunning(const gu::tstring& processName) const = 0;

		/*!**********************************************************************
		*  @brief     �v���Z�X��ҋ@���܂�
		*  @param[in] const ProcessHandle& : �v���Z�X�n���h��
		*  @param[in] const gu::uint32 : �ҋ@��millisecond. �f�t�H���g�͖����ɑ҂�������
		*  @return    void
		*************************************************************************/
		virtual void WaitForProcess(const ProcessHandle& handle, gu::uint32 milliseconds = 0xFFFFFFFF) const = 0;

		/*!**********************************************************************
		*  @brief      �w�肵���v���Z�X�̏I����Ԃ��擾���܂�
		*  @param[in]  const ProcessHandle& : �v���Z�X�n���h��
		*  @param[out] gu::uint32* : �I���R�[�h
		*  @return     bool 
		*************************************************************************/
		virtual bool GetProcessExitCode(const ProcessHandle& handle, gu::uint32* exitCode) const = 0;

		/*!**********************************************************************
		*  @brief     �v���Z�X�����擾
		*  @param[in] const gu::uint32 �v���Z�XID
		*  @return    tstring 
		*************************************************************************/
		virtual gu::tstring GetProcessName(const gu::uint32 processID) const = 0;

		/*!**********************************************************************
		*  @brief     �v���Z�X�̗D��x��ݒ肵�܂�
		*  @param[in] ProcessHandle& : �v���Z�X�n���h��
		*  @param[in] const ProcessPriority : �v���Z�X�̗D��x
		*  @return    bool
		*************************************************************************/
		virtual bool SetProcessPriority(ProcessHandle& handle, const ProcessPriority priority) const = 0;

		#pragma endregion Process Function

		#pragma region Pipe Function
		/*!**********************************************************************
		*  @brief     �����̃p�C�v���쐬���܂�. �p�C�v�͓ǂݎ��Ə������݂̗������T�|�[�g���܂�
		*  @param[in] void** readPipe : �ǂݎ��p�C�v
		*  @param[in] void** writePipe : �������݃p�C�v
		*  @param[in] cosnt bool writePipeInheritable : �������݃p�C�v���q�v���Z�X�ɂ��p���\�ɂ��邩�ǂ���. �q�v���Z�X���������݉\�ɂ��邩�ǂ���
		*  @return    bool
		*************************************************************************/
		virtual bool CreatePipe(void** readPipe, void** writePipe, const bool writePipeInheritable) const = 0;

		/*!**********************************************************************
		*  @brief     �����̃p�C�v���I�����܂�
		*  @param[in] void** readPipe : �ǂݎ��p�C�v
		*  @param[in] void** writePipe : �������݃p�C�v
		*  @return    void
		*************************************************************************/
		virtual void ClosePipe(void** readPipe, void** writePipe) const = 0;

		/*!**********************************************************************
		*  @brief     �w�肳�ꂽ�p�C�v����, �f�[�^�̓ǂݎ����s���܂�.
		*  @attention �����R�[�h��UTF-8��z�肵�Ă��܂�.�����_�ł͕����R�[�h�̕ϊ��͍s���Ă��܂���.
		*  @param[in] void* �ǂݎ����s���p�C�v
		*  @param[out void* �ǂݎ�����f�[�^
		*  @param[in] gu::uint64 �ǂݎ��o�C�g��
		*  @return    bool ����
		*************************************************************************/
		virtual bool ReadPipe(void* readPipe, void* outData, gu::uint64 byteSize) const = 0;

		/*!**********************************************************************
		*  @brief     �w�肳�ꂽ�p�C�v����, �f�[�^�̓ǂݎ����s���܂�.
		*  @attention �����R�[�h��UTF-8��z�肵�Ă��܂�.�����_�ł͕����R�[�h�̕ϊ��͍s���Ă��܂���.
		*  @param[in] void* �ǂݎ����s���p�C�v
		*  @param[out gu::DynamicArray<uint8>& �ǂݎ�����o�C�g��
		*  @return    bool ����
		*************************************************************************/
		virtual bool ReadPipe(void* readPipe, gu::DynamicArray<gu::uint8>& outData) const = 0;

		/*!**********************************************************************
		*  @brief     �w�肳�ꂽ�p�C�v����, �f�[�^�̓ǂݎ����s���܂�.
		*  @attention �����R�[�h��UTF-8��z�肵�Ă��܂�.�����_�ł͕����R�[�h�̕ϊ��͍s���Ă��܂���.
		*  @param[in] void* ���������s���p�C�v
		*  @param[in] const void* �������ރf�[�^
		*  @param[in] gu::uint64 �������ރo�C�g��
		*  @return    bool ����
		*************************************************************************/
		virtual bool WritePipe(void* writePipe, const void* data, gu::uint64 byteSize) const = 0;

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