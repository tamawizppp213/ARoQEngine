//////////////////////////////////////////////////////////////////////////////////
///  @file   CoreProcess.cpp
///  @brief  OS�̃v���Z�X�𑀍삷��N���X
///  @author toide
///  @date   2024/06/20 23:46:21
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "Platform/Core/Include/CoreProcess.hpp"
#include "Platform/Core/Include/CorePlatformMacros.hpp"
#include PLATFORM_COMPILED_HEADER(Platform, Process.hpp)
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor

#pragma endregion Constructor and Destructor

#pragma region Public Function
/*!**********************************************************************
*  @brief     �V���O���g���C���X�^���X���擾���܂�. (��{�I�Ƀ����o�ϐ����g��Ȃ�����, ���Ɉ��e���͂���܂���)
*  @param[in] const bool �����I�ɏ��������s���܂�. (Default: false)
*  @return    gu::SharedPointer<IProcess> �v���Z�X�̃C���X�^���X
* ************************************************************************/
gu::SharedPointer<platform::core::CoreProcess> platform::core::CoreProcess::Get(const bool forceInitialize)
{
	static gu::SharedPointer<platform::core::CoreProcess> instance = nullptr;
	
	if (!instance || forceInitialize)
	{
		instance = gu::StaticPointerCast<platform::core::CoreProcess>
			      (gu::MakeShared<PLATFORM_CLASS(CoreProcess)>());
	}
	
	return instance;

}
#pragma endregion Public Function