//////////////////////////////////////////////////////////////////////////////////
///             @file   UIButton.hpp
///             @brief  Button object
///             @author Toide Yutaro
///             @date   2023_01_07
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef UIBUTTON_HPP
#define UIBUTTON_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "UIImage.hpp"
#include "GameCore/Input/Include/Mouse.hpp"
#include <memory>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace gc::ui
{
	
	/****************************************************************************
	*				  			    Button
	*************************************************************************//**
	*  @class     Button
	*  @brief     UI : Button (Click Detection)
	*****************************************************************************/
	class Button : public Image
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/* @brief : cursolPosition : Screen space. Trigger mode*/
		virtual bool OnClick(const MouseButton mouseButton = MouseButton::LEFT) const;

		/* @brief : Returns true for the entire duration of the click.*/
		virtual bool IsPress(const MouseButton mouseButton = MouseButton::LEFT) const;
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		bool GetIsInteractive() const { return _isInteractive; };

		void SetIsInteractive(const bool isInteractive) { _isInteractive = isInteractive; }

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		Button(const std::shared_ptr<Mouse>& mouse);

		~Button();

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		bool DetectClick() const;

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		bool _isInteractive = true;
		
		std::shared_ptr<Mouse> _mouse = nullptr;

	};
}
#endif