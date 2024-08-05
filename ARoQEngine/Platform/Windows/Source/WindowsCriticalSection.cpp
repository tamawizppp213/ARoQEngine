//////////////////////////////////////////////////////////////////////////////////
///  @file   WindowsCriticalSection.cpp
///  @brief  OS�ɂ�����, �f�[�^�̋�����h�����߂̃N���e�B�J���Z�N�V������񋟂��܂�.@n
///          ��̃v���Z�X���Ŏg�p�ł�, �v���Z�X�Ԃŋ��L���邱�Ƃ͏o���Ȃ�.
///  @author toide
///  @date   2024/06/22 0:19:52
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "Platform/Windows/Include/WindowsCriticalSection.hpp"
#include <Windows.h>

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace platform::windows;
using namespace gu;

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
static_assert(sizeof(CRITICAL_SECTION) == sizeof(details::criticalSection::Info));

#pragma region Constructor and Destructor
/*! @brief �f�t�H���g�R���X�g���N�^*/
CoreCriticalSection::CoreCriticalSection()
{
	InitializeCriticalSection((LPCRITICAL_SECTION)&_criticalSectionInfo);
	SetCriticalSectionSpinCount((LPCRITICAL_SECTION)&_criticalSectionInfo, 4000);
}

/*! @brief �f�X�g���N�^*/
CoreCriticalSection::~CoreCriticalSection()
{
	DeleteCriticalSection((LPCRITICAL_SECTION)&_criticalSectionInfo);
}
#pragma endregion Constructor and Destructor

#pragma region Public Function

/*!**********************************************************************
*  @brief     �X���b�h�̔r��������s�����߂�, �Ăяo���X���b�h�����b�N���܂�. @n
*             ���̃X���b�h�����b�N���Ă���ꍇ��, ���b�N�����������܂őҋ@���܂�.
*  @param[in] void
*  @return    void
*************************************************************************/
void CoreCriticalSection::Lock()
{
	EnterCriticalSection((LPCRITICAL_SECTION)&_criticalSectionInfo);
}

/*!**********************************************************************
*  @brief     �X���b�h�̔r��������s�����߂�, �Ăяo���X���b�h�����b�N���܂�. @n
*             ���̃X���b�h�����b�N���Ă���ꍇ��, ������false��Ԃ��܂�.
*  @param[in] void
*  @return    bool ���b�N�����������ꍇ��true
* ************************************************************************/
bool CoreCriticalSection::TryLock()
{
	return !!TryEnterCriticalSection((LPCRITICAL_SECTION)&_criticalSectionInfo);
}

/*!**********************************************************************
*  @brief     Lock�֐��Ń��b�N�����X���b�h�������, �����̕ϐ���ǂݏ����\�ɂ��܂�
*  @param[in] void
*  @return    void
*************************************************************************/
void CoreCriticalSection::Unlock()
{
	LeaveCriticalSection((LPCRITICAL_SECTION)&_criticalSectionInfo);
}


#pragma endregion Public Function