//////////////////////////////////////////////////////////////////////////////////
///             @file   ScreenCapture.cpp
///             @brief  Texture saving. 
///                     If you press the PrintScreen button, The texture is saved in the current time directory.
///                     This class is used when executing in the debug mode 
///             @author Toide Yutaro
///             @date   2023_02_07
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/ScreenCapture.hpp"
#include "GraphicsCore/Engine/Include/LowLevelGraphicsEngine.hpp"
#include "GameCore/Input/Include/Keyboard.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUTexture.hpp"
#include "GameUtility/File/Include/FileSystem.hpp"
#include <iostream>
#include <ctime>

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gc::rendering;
using namespace rhi::core;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor 

#pragma endregion Constructor and Destructor

#pragma region Main Function
bool ScreenCapture::Capture(const TexturePtr& texture)
{
	if (!IsTriggerPrintScreenKey()) { return false; }
	if (!texture)                   { return false; }

	const auto commandList  = _engine->GetCommandList(CommandListType::Graphics);
	const auto commandQueue = _engine->GetCommandQueue(CommandListType::Graphics);

	const auto filePath = CreateFileDirectoryAndPath();
	texture->Save(filePath, commandList, commandQueue);
	
	commandList->BeginRecording(true);
	commandList->BeginRenderPass(_engine->GetDrawContinueRenderPass(), _engine->GetFrameBuffer(_engine->GetCurrentFrameIndex()));
	return true;
}

bool ScreenCapture::Capture(const std::vector<TexturePtr>& textures)
{
	if (!IsTriggerPrintScreenKey()) { return false; }

	for (size_t i = 0; i < textures.size(); ++i)
	{
		if (!textures[i]) { return false; }

		const auto commandList  = _engine->GetCommandList(CommandListType::Graphics);
		const auto commandQueue = _engine->GetCommandQueue(CommandListType::Graphics);

		const auto filePath = CreateFileDirectoryAndPath(i);
		textures[i]->Save(filePath, _engine->GetCommandList(rhi::core::CommandListType::Graphics), _engine->GetCommandQueue(CommandListType::Graphics));
		
		commandList->BeginRecording(true);
		commandList->BeginRenderPass(_engine->GetDrawContinueRenderPass(), _engine->GetFrameBuffer(_engine->GetCurrentFrameIndex()));
	}
	
	return true;
}

bool ScreenCapture::Capture(const FrameBufferPtr& frameBuffer)
{
	if (!IsTriggerPrintScreenKey()) { return false; }

	const auto currentTime = GetCurrentDataTime();
	return true;
}

std::wstring ScreenCapture::CreateFileDirectoryAndPath(const size_t index)
{
	const auto currentTime   = GetCurrentDataTime();
	const auto fileDirectory = L"Resources/Capture/" + currentTime;
	const auto filePath      = fileDirectory + L"/" + std::to_wstring(index) + L".png";
	if (!file::FileSystem::MakeDirectory(fileDirectory)) { throw std::runtime_error("failed to create directory"); }
	return filePath;
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

bool ScreenCapture::IsTriggerPrintScreenKey()
{
	return _keyboard->IsTrigger(DIK_SYSRQ);
}
#pragma endregion Main Function


