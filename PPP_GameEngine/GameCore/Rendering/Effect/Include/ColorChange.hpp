//////////////////////////////////////////////////////////////////////////////////
///             @file   ColorChange.hpp
///             @brief  Color change post effect
///             @author Toide Yutaro
///             @date   2022_03_11
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef COLOR_CHANGE_HPP
#define COLOR_CHANGE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "FullScreenEffector.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace gc
{

	enum ColorChangeType
	{
		None,
		Monochrome,
		Sepia,
		GrayScale,
		Binary,
		Invert,
		CountOfType,
	};
	/****************************************************************************
	*				  			ColorChange
	*************************************************************************//**
	*  @class     Color Change
	*  @brief     color change post effect
	*****************************************************************************/
	class ColorChange : public IFullScreenEffector
	{
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
		ColorChange();

		~ColorChange();

		ColorChange(const ColorChangeType type, const LowLevelGraphicsEnginePtr& engine, const gu::wstring& addName = L"");

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		void PreparePipelineState(const gu::wstring& addName) override;
		
		void PrepareResourceView() override;
		
		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		ColorChangeType _colorType = ColorChangeType::None;
	};
}
#endif