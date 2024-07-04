//////////////////////////////////////////////////////////////////////////////////
//              @file   CoreWindow.cpp
///             @brief  window 
///             @author Toide Yutaro
///             @date   2023_09_13
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../../Core/Include/CoreWindowMessageHandler.hpp"
#include "../../Core/Include/CoreWindow.hpp"
#include "GameUtility/Base/Include/Screen.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace platform::core;
using namespace platform;
//////////////////////////////////////////////////////////////////////////////////
//                              Implement
/////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor
CoreWindowMessageHandler::CoreWindowMessageHandler()
{

}
CoreWindowMessageHandler::~CoreWindowMessageHandler()
{

}
#pragma endregion Constructor and Destructor

#pragma region Public Function

bool CoreWindowMessageHandler::OnSizeChanged(const CoreWindowPtr& window, const gu::uint32 width, const gu::uint32 height)
{
	if (_isResizing) { return false; }

	// Resize��::SetWindowPos�ɂ���Ă����Resize���Ă΂��̂�h���܂�.
	_isResizing = true;

	// ����̈ʒu���擾����.
	gu::int32 x = 0, y = 0;
	window->GetWindowPostion(x, y);

	// �T�C�Y�ύX
	window->Resize(x, y, width, height);

	// ��X�����܂�.
	Screen::SetScreenWidth(width);
	Screen::SetScreenHeight(height);

	_isResizing = false;
	return true;
}
#pragma endregion Public Function