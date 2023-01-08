//////////////////////////////////////////////////////////////////////////////////
///             @file   UISlider.hpp
///             @brief  Slider object
///             @author Toide Yutaro
///             @date   2022_12_27
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef UISLIDER_HPP
#define UISLIDER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "UIImage.hpp"
#include <memory>
#include <vector>
#include <functional>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::core
{
	class GPUResourceView;
	class GPUResourceCache;
};

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace gc::ui
{
	struct UIResource
	{
	private:
		using ResourceViewPtr = std::shared_ptr<rhi::core::GPUResourceView>;
	public:
		ResourceViewPtr ResourceView;
		Image           Image;
	};
	/****************************************************************************
	*				  			    Slider
	*************************************************************************//**
	*  @class     Slider
	*  @brief     UI : Slider (åªç›ÇÕç∂Ç©ÇÁâEÇ‹Ç≈ÇÃîÕàÕÇÃÇ›ëŒâûÇµÇƒÇ‹Ç∑. è„â∫ï˚å¸ÇÕç°å„í«â¡Ç∑ÇÈó\íËÇ≈Ç∑.)
	*****************************************************************************/
	class Slider
	{
		using ListenerFunction = std::function<void(float)>;
		using FunctionPtr      = std::shared_ptr<ListenerFunction>;
		using ResourceCachePtr = std::shared_ptr<rhi::core::GPUResourceCache>;
		using ResourceViewPtr  = std::shared_ptr<rhi::core::GPUResourceView>;
	public:
		/****************************************************************************
		**                Enum
		*****************************************************************************/
		enum RenderType
		{
			BackGround,
			Color,
			CountOf
		};

		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		void CreateInScreenSpace(const gm::Float3& position,
			const gm::Float2& rectSize,
			const gm::Float4& fillColor = gm::Float4(1, 1, 1, 1));

		void CreateInNDCSpace(
			const gm::Float3& position  = gm::Float3(0, 0, 0),
			const gm::Float2& rectSize  = gm::Float2(2, 2),
			const gm::Float4& fillColor = gm::Float4(1, 1, 1, 1));

		/* @brief : Add the difference to the current slider value */
		void Add(const float difference);

		/* @brief : Add step size to the current slider value*/
		void Step(const bool isPlus); // + or -. add stepsize

		// onValueChanged function
		void AddListener   (FunctionPtr function) {_functionList.push_back(function); };
		void RemoveListener(FunctionPtr function) { std::erase(_functionList, function); }; // C++20

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		const UIResource& GetRenderResource(const RenderType type) { return _resourceList[type]; };

		/* ColorÇÃèÍçáÇÕResources/Preset/NullAlbedoMap.pngÇéwíËÇ∑ÇÈÇ∆ó«Ç¢Ç≈Ç∑.*/
		void SetTexture(const RenderType type, const ResourceViewPtr& view) { _resourceList[type].ResourceView = view; }

		/*-------------------------------------------------------------------
		-             Slider value
		---------------------------------------------------------------------*/
		float GetValue   () const { return _value; }
		float GetMaxValue() const { return _maxValue; }
		float GetMinValue() const { return _minValue; }

		void SetValue   (float value);
		void SetMaxValue(const float value) { _maxValue = value; }
		void SetMinValue(const float value) { _minValue = value; }

		void SetStepSize(const float stepSize) { _stepSize = stepSize; }

		void SetFillColor(const gm::Float4& fillColor);

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		Slider();

		Slider(const float value, const float maxValue = 100, const float minValue = 0, const float stepSize = 1.0f);

		~Slider();

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		/* ílÇ™ïœâªÇµÇΩÇ∆Ç´ìoò^ÇµÇƒÇ¢ÇÈä÷êîÇèáî‘Ç…é¿çsÇ∑ÇÈ.*/
		bool OnValueChanged(const float newValue);

		bool InitializeSliderCanvas(const bool isNDC, const gm::Float3& position, const gm::Float2& rectSize, const gm::Float4 fillColor);

		void RecreateSliderUI();
		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		bool _isInteractive = true;

		// slider value
		float _minValue = 0.0f;
		float _maxValue = 100.0f;
		float _value    = 50.0f;
		
		// The amount to adjust the value by, when using a controller or keyboard
		float _stepSize = 1.0f;

		gm::Float3 _leftEdgePosition   = { 0,0,0 };
		gm::Float2 _fullRectSize       = { 0,0 };
		gm::Float4 _fillColor          = {1,1,1,1};

		bool _initialized = false;

		// callback function list (ä÷êîìoò^éûÇÕïKÇ∏voidå^Çï‘ÇËílÇ∆Ç∑ÇÈfloatà¯êî(slider value)ÇéùÇ¡ÇΩä÷êîÇê›íËÇµÇƒÇ≠ÇæÇ≥Ç¢. )
		// When registering a function, be sure to set a function with a float argument (slider value) with return void. 
		std::vector<FunctionPtr> _functionList = {};

		std::vector<UIResource> _resourceList = {}; // 0: Background image, 1: Fill rectangle
	};
}
#endif