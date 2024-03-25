//////////////////////////////////////////////////////////////////////////////////
///             @file   Mosaic.hpp
///             @brief  Mosaic post effect
///             @author Toide Yutaro
///             @date   2023_02_23
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef MOSAIC_HPP
#define MOSAIC_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "FullScreenEffector.hpp"
#include "GameUtility/Math/Include/GMVector.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace gc
{
	/****************************************************************************
	*				  			ColorChange
	*************************************************************************//**
	*  @class     Color Change
	*  @brief     color change post effect
	*****************************************************************************/
	class Mosaic : public IFullScreenEffector
	{
	protected:
		struct MosaicInfo
		{
			gm::Float2 WindowSize = {0,0};
			float BlockSize       = 0.0f;
			float Padding         = 0.0f;
		};
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/* @brief : Resize frame buffer (Not implement)*/
		void OnResize(int newWidth, int newHeight) override;

		/*@brief : Render to back buffer*/
		void Draw() override;
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		Mosaic();

		~Mosaic();

		Mosaic(const LowLevelGraphicsEnginePtr& engine, const float blockSize = 10.0f, const gu::tstring& addName = SP(""));

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		void PrepareBuffer(const float blockSize, const gu::tstring& addName);

		void PreparePipelineState(const gu::tstring& addName) override;
		
		void PrepareResourceView() override;
		
		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		MosaicInfo _mosaicInfo = {};
	};
}
#endif