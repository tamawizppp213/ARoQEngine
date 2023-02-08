//////////////////////////////////////////////////////////////////////////////////
///             @file   ScreenCapture.cpp
///             @brief  Texture saving. 
///                     If you press the PrintScreen button, The texture is saved in the current time directory.
///                     This class is used when executing in the debug mode 
///             @author Toide Yutaro
///             @date   2023_02_07
//////////////////////////////////////////////////////////////////////////////////
#pragma once

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/ScreenCapture.hpp"
#include "GameCore/Input/Include/Keyboard.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUTexture.hpp"
#include <iostream>
#include <ctime>

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gc::rendering;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor 

#pragma endregion Constructor and Destructor

#pragma region Main Function
bool ScreenCapture::Capture(const TexturePtr& texture)
{
	const auto currentTime = GetCurrentDataTime();

	return true;
}

bool ScreenCapture::Capture(const std::vector<TexturePtr>& textures)
{
	const auto currentTime = GetCurrentDataTime();
	return true;
}

bool ScreenCapture::Capture(const FrameBufferPtr& frameBuffer)
{
	const auto currentTime = GetCurrentDataTime();
	return true;
}

std::wstring ScreenCapture::GetCurrentDataTime()
{
	const auto time = std::time(nullptr);
	const auto now  = std::localtime(&time);

	const auto str = std::to_wstring(now->tm_year + 1900) + L"_" +
		std::to_wstring(now->tm_mon) + L"_" +
		std::to_wstring(now->tm_hour) + L"_" +
		std::to_wstring(now->tm_min) + L"_" +
		std::to_wstring(now->tm_sec);

	return str;
}
#pragma endregion Main Function


