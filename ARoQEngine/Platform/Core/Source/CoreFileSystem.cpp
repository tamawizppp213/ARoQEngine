//////////////////////////////////////////////////////////////////////////////////
///  @file   CoreFileSystem.cpp
///  @brief  �t�@�C���̓ǂݏ�����, �t�@�C�������݂��邩�̊m�F, �f�B���N�g���̍쐬��폜�Ȃǂ̊�{�I�ȃt�@�C������̋@�\��񋟂��܂�.
///  @author toide
///  @date   2024/05/11 19:14:45
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "Platform/Core/Include/CoreFileSystem.hpp"
#include "Platform/Core/Include/CorePlatformMacros.hpp"
#include PLATFORM_COMPILED_HEADER(Platform, FileSystem.hpp)
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
*  @return    gu::SharedPointer<IFileSystem> �t�@�C���V�X�e���̃C���X�^���X
*************************************************************************/
gu::SharedPointer<platform::core::IFileSystem> platform::core::IFileSystem::Get(const bool forceInitialize)
{
	static gu::SharedPointer<platform::core::IFileSystem> instance = nullptr;
	
	if (!instance || forceInitialize)
	{
		instance = gu::StaticPointerCast<platform::core::IFileSystem>
			      (gu::MakeShared<PLATFORM_CLASS(IFileSystem)>());
	}
	
	return instance;

}
#pragma endregion Public Function