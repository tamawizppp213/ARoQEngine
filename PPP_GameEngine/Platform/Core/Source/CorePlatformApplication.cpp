//////////////////////////////////////////////////////////////////////////////////
//              @file   WindowsError.cpp
///             @brief  Log windows error code and error sentence for debugging
///             @author Toide Yutaro
///             @date   2023_09_13
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../../Core/Include/CorePlatformApplication.hpp"
#include "../../Core/Include/CoreWindowMessageHandler.hpp"
#include "../../Core/Include/CorePlatformMacros.hpp"
#include "../../Windows/Include/WindowsPlatformApplication.hpp"
#include <stdexcept>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace platform::core;
using namespace platform;
//////////////////////////////////////////////////////////////////////////////////
//                              Implement
/////////////////////////////////////////////////////////////////////////////////
gu::SharedPointer<PlatformApplication> PlatformApplication::Create(const PlatformType type)
{
	switch (type)
	{
		case PlatformType::Windows:
		{
			#if PLATFORM_OS_WINDOWS
			return gu::StaticPointerCast<PlatformApplication>(gu::MakeShared<platform::windows::PlatformApplication>());
			#endif
		}
		default:
		{
			throw std::runtime_error("Unknown platform");
		}
	}
}

gu::SharedPointer<PlatformApplication> PlatformApplication::Create()
{
#if PLATFORM_OS_WINDOWS
	return gu::StaticPointerCast<PlatformApplication>(gu::MakeShared<windows::PlatformApplication>());
#else
	OutputDebugStringA("platform application is nullptr");
	return nullptr;
#endif
}