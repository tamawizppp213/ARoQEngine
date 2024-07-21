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
namespace engine
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
	****************************************************************************/
	/* @class     Color Change
	*  @brief     color change post effect
	*****************************************************************************/
	class ColorChange : public IFullScreenEffector
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/* @brief : Resize frame buffer (Not implement)*/
		virtual void OnResize(gu::uint32 newWidth, gu::uint32 newHeight) override;

		/*@brief : Render to back buffer*/
		virtual void Draw() override;
		/****************************************************************************
		**                Public Property
		*****************************************************************************/

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		ColorChange();

		~ColorChange();

		ColorChange(const ColorChangeType type, const LowLevelGraphicsEnginePtr& engine, const gu::tstring& addName = SP(""));

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		void PreparePipelineState(const gu::tstring& addName) override;
		
		/****************************************************************************
		**                Protected Property
		*****************************************************************************/
		ColorChangeType _colorType = ColorChangeType::None;
	};
}
#endif