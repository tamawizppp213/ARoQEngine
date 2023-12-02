//////////////////////////////////////////////////////////////////////////////////
//              @file   WindowsError.cpp
///             @brief  Log windows error code and error sentence for debugging
///             @author Toide Yutaro
///             @date   2023_09_13
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../../Windows/Include/WindowsError.hpp"
#if PLATFORM_OS_WINDOWS
#include <stdexcept>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace platform;
using namespace platform::windows;

//////////////////////////////////////////////////////////////////////////////////
//                              Implement
//////////////////////////////////////////////////////////////////////////////////
void ErrorLogger::Log(HWND hwnd)
{
	const std::uint32_t errorCode = ::GetLastError();

	if (errorCode == 0) { return; }
	LPVOID messageBuffer = NULL;
	FormatMessage
	(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | // �e�L�X�g�̃��������蓖�Ă�v������
		FORMAT_MESSAGE_FROM_SYSTEM | // �G���[���b�Z�[�W��Windows���p�ӂ��Ă���
		FORMAT_MESSAGE_IGNORE_INSERTS,   // ���̈����𖳎����ăG���[�R�[�h�ɑ΂���G���[���b�Z�[�W���쐬����
		NULL, errorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&messageBuffer, 0, NULL
	);

	MessageBox(hwnd, (LPCTSTR)messageBuffer, NULL, MB_OK | MB_ICONINFORMATION);
	LocalFree(messageBuffer);
}

#endif