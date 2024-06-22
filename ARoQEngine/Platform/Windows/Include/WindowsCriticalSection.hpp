//////////////////////////////////////////////////////////////////////////////////
///  @file   WindowsCriticalSection.hpp
///  @brief  OS�ɂ�����, �f�[�^�̋�����h�����߂̃N���e�B�J���Z�N�V������񋟂��܂�.@n
///          ��̃v���Z�X���Ŏg�p�ł�, �v���Z�X�Ԃŋ��L���邱�Ƃ͏o���Ȃ�.
///  @author toide
///  @date   2024/06/21 23:51:23
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef WINDOWS_CRITICAL_SECTION_HPP
#define WINDOWS_CRITICAL_SECTION_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/GUClassUtility.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace platform::windows
{
	namespace details::criticalSection
	{
		/* @brief Windows���w�b�_�ɒǉ��������Ȃ�����*/
		struct Info
		{
			/* @brief ��{�����o�ϐ����͎̂g�p���Ȃ�. */
			void* Opaque1[1] = {};

			/*! @brief ��{�����o�ϐ����͎̂g�p���Ȃ�*/
			long Opaque2[2] = {};

			/*! @brief ��{�����o�ϐ����͎̂g�p���Ȃ�*/
			void* Opaque3[3] = {};
		};
	}

	/****************************************************************************
	*				  			   WindowsCriticalSection
	*************************************************************************//**
	/*  @class     WindowsCriticalSection
	*   @brief     OS�ɂ�����, �f�[�^�̋�����h�����߂̃N���e�B�J���Z�N�V������񋟂��܂�.@n
	*              ��̃v���Z�X���Ŏg�p�ł�, �v���Z�X�Ԃŋ��L���邱�Ƃ͏o���Ȃ�.
	*****************************************************************************/
	class CoreCriticalSection : public gu::NonCopyable
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief     �X���b�h�̔r��������s�����߂�, �Ăяo���X���b�h�����b�N���܂�. @n
		*             ���̃X���b�h�����b�N���Ă���ꍇ��, ���b�N�����������܂őҋ@���܂�.
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		void Lock();

		/*!**********************************************************************
		*  @brief     �X���b�h�̔r��������s�����߂�, �Ăяo���X���b�h�����b�N���܂�. @n
		*             ���̃X���b�h�����b�N���Ă���ꍇ��, ������false��Ԃ��܂�.
		*  @param[in] void
		*  @return    bool ���b�N�����������ꍇ��true
		* ************************************************************************/
		bool TryLock();

		/*!**********************************************************************
		*  @brief     Lock�֐��Ń��b�N�����X���b�h�������, �����̕ϐ���ǂݏ����\�ɂ��܂�
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		void Unlock();

		#pragma endregion 

		#pragma region Public Property

		#pragma endregion 

		#pragma region Public Operator 

		#pragma endregion 

		#pragma region Public Constructor and Destructor
		/*! @brief �f�t�H���g�R���X�g���N�^*/
		CoreCriticalSection();

		/*! @brief �f�X�g���N�^*/
		~CoreCriticalSection();

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
		details::criticalSection::Info _criticalSectionInfo = {};

		#pragma endregion 

	};
}

#endif