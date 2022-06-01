//////////////////////////////////////////////////////////////////////////////////
///             @file   SampleTexture.hpp
///             @brief  Texture Sample
///             @author Toide Yutaro
///             @date   2022_06_01
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef SAMPLE_TEXTURE_HPP
#define SAMPLE_TEXTURE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "MainGame/Core/Include/Scene.hpp"
#include "GameCore/Core/Include/ResourceManager.hpp"
#include <memory>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
struct Texture;
namespace ui
{
	class Image;
	class UIRenderer;
}
namespace sample
{
	
	/****************************************************************************
	*				  			SampleEmpty
	*************************************************************************//**
	*  @class     SampleEmpty
	*  @brief     Empty sample
	*****************************************************************************/
	class SampleTexture : public Scene
	{
		using UIRendererPtr = std::unique_ptr<ui::UIRenderer>;
		using ImagePtr      = std::unique_ptr<ui::Image>;
		using TexturePtr    = std::unique_ptr<Texture>;
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		void Initialize(GameTimer* gameTimer) override;
		void Update() override;
		void Draw() override;
		void Terminate() override;
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		SampleTexture();
		~SampleTexture();
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		void LoadMaterials() override;
		void OnKeyboardInput() override;
		void OnMouseInput() override;
		void OnGamePadInput() override;
		void ExecuteSceneTransition() override;
		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		UIRendererPtr _renderer;
		ImagePtr      _image;
		TexturePtr    _texture;
		ResourceManager& _resourceManager = ResourceManager::Instance();
	};
}
#endif