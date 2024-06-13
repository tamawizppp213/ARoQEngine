////////////////////////////////////////////////////////////////////////////////////
/////             @file   ResourceManager.hpp
/////             @brief  ResourceManager
/////             @author Toide Yutaro
/////             @date   2022_03_13
////////////////////////////////////////////////////////////////////////////////////
//#pragma once
//#ifndef RESOURCE_MANAGER_HPP
//#define RESOURCE_MANAGER_HPP
//
////////////////////////////////////////////////////////////////////////////////////
////                             Include
////////////////////////////////////////////////////////////////////////////////////
//#include "GraphicsCore/Engine/Include/GraphicsCoreResourceType.hpp"
//#include <memory>
//#include <string>
////////////////////////////////////////////////////////////////////////////////////
////                              Define
////////////////////////////////////////////////////////////////////////////////////
//struct Texture;
////////////////////////////////////////////////////////////////////////////////////
////                         Template Class
////////////////////////////////////////////////////////////////////////////////////
//namespace gc::audio
//{
//	class AudioClip;
//}
//namespace gc::core
//{
//	/****************************************************************************
//	*				  			TemplateClass
//	****************************************************************************/
//	/* @class     TemplateClass
//	*  @brief     temp
//	*****************************************************************************/
//	class ResourceManager
//	{
//	public:
//		/****************************************************************************
//		**                Public Function
//		*****************************************************************************/
//		//AudioClip& LoadAudio  (const std::wstring& name);
//		const Texture& LoadTexture(const std::wstring& name, TextureType type = TextureType::Texture2D);
//		/*-------------------------------------------------------------------
//		-                        Clear
//		---------------------------------------------------------------------*/
//		void ClearAllResources();
//		void ClearAudioTable();
//		/*-------------------------------------------------------------------
//		-                        Exists
//		---------------------------------------------------------------------*/
//		bool ExistsAudioClip(const std::wstring& name);
//
//		/****************************************************************************
//		**                Public Property
//		*****************************************************************************/
//		gc::audio::AudioClip& GetAudioClip(const std::wstring& name);
//
//		/****************************************************************************
//		**                Constructor and Destructor
//		*****************************************************************************/
//		static ResourceManager& Instance()
//		{
//			static ResourceManager resourceManager;
//			return resourceManager;
//		}
//		ResourceManager(const ResourceManager&) = delete;
//		ResourceManager& operator=(const ResourceManager&) = delete;
//		ResourceManager(ResourceManager&&) = delete;
//		ResourceManager& operator=(ResourceManager&&) = delete;
//	private:
//		/****************************************************************************
//		**                Private Function
//		*****************************************************************************/
//		ResourceManager();
//		~ResourceManager();
//
//		/****************************************************************************
//		**                Private Property
//		*****************************************************************************/
//		struct Implementation;
//		std::unique_ptr<Implementation> _implementation;
//	};
//}
//#endif