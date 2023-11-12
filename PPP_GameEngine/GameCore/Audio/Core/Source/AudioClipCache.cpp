//////////////////////////////////////////////////////////////////////////////////
//              @file   AudioClipCache.cpp
///             @brief  audio clip cache
///             @author Toide Yutaro
///             @date   2022_12_22
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/AudioClipCache.hpp"
#include "../Include/AudioClip.hpp"
#include <functional>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gc::audio;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor
AudioClipCache::AudioClipCache()
{

}

AudioClipCache::~AudioClipCache()
{
	Clear();
}
#pragma endregion Constructor and Destructor

#pragma region Main Function
/****************************************************************************
*                       Load
*************************************************************************//**
*  @fn        AudioClipCache::AudioClipPtr AudioClipCache::Load(const std::wstring& filePath)
*
*  @brief     Load audio clip. (wav file)
*
*  @param[in] const std::wstring& filePath
*
*  @return    AudioClipPtr
*****************************************************************************/
AudioClipCache::AudioClipPtr AudioClipCache::Load(const std::wstring& filePath)
{
	/*-------------------------------------------------------------------
	-           Get hash code
	---------------------------------------------------------------------*/
	size_t hashCode = std::hash<std::wstring>()(filePath);

	/*-------------------------------------------------------------------
	-           Load audio clip
	---------------------------------------------------------------------*/
	if (_audioClipList.contains(hashCode))
	{
		return _audioClipList.at(hashCode);
	}
	else
	{
		auto audioClip = std::make_shared<AudioClip>();
	
		// load wav file
		if (!audioClip->Load(filePath)) { OutputDebugStringA("Failed to load sound file.");  return nullptr; };

		// regist audio clip to the audioClipList;
		_audioClipList[hashCode] = std::move(audioClip);
		return _audioClipList[hashCode];
	}
}

/****************************************************************************
*                       Exist
*************************************************************************//**
*  @fn        bool AudioClipCache::Exist(const std::wstring& filePath)
*
*  @brief     Exist audio clip
*
*  @param[in] const std::wstring& filePath
*
*  @return    bool (true : exists. false : not yet exist)
*****************************************************************************/
bool AudioClipCache::Exist(const std::wstring& filePath)
{
	/*-------------------------------------------------------------------
	-           Get hash code
	---------------------------------------------------------------------*/
	size_t hashCode = std::hash<std::wstring>()(filePath);

	return _audioClipList.contains(hashCode);
}
#pragma endregion Main Function