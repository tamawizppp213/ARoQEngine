//////////////////////////////////////////////////////////////////////////////////
///             @file   AudioClipCache.hpp
///             @brief  Audio clip cache
///             @author Toide Yutaro
///             @date   2022_12_22
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef AUDIO_CLIP_CACHE_HPP
#define AUDIO_CLIP_CACHE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/GUClassUtility.hpp"
#include <string>
#include <memory>
#include "GameUtility/Container/Include/GUSortedMap.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         AudioClip
//////////////////////////////////////////////////////////////////////////////////

namespace gc::audio
{
	class AudioClip;
	/****************************************************************************
	*				  		AudioClipCache
	*************************************************************************//**
	*  @class     AudioClipCache
	*  @brief     audio clip resource cache. (Basically, This class is used by ResourceManager class.)
	*****************************************************************************/
	class AudioClipCache : public gu::NonCopyable
	{
		using AudioClipPtr = std::shared_ptr<AudioClip>;
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/* @brief : Load audio clip*/
		AudioClipPtr Load(const std::wstring& filePath);

		// @brief : Exist audio clip
		bool Exist(const std::wstring& filePath);

		/* @brief : Clear audio clip list.*/
		void Clear() { _audioClipList.Clear(); };

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		AudioClipCache();
		~AudioClipCache();

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		gu::SortedMap<std::uint64_t, AudioClipPtr> _audioClipList = {};

	};
}
#endif