//////////////////////////////////////////////////////////////////////////////////
///             @file   DepthOfField.hpp
///             @brief  DepthOfField
///             @author Toide Yutaro
///             @date   2022_05_01
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef DEPTH_OF_FIELD_HPP
#define DEPTH_OF_FIELD_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/DirectX12/Core/Include/DirectX12Buffer.hpp"
#include <memory>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
/****************************************************************************
*				  			Dof
*************************************************************************//**
*  @class     Dof
*  @brief     Depth Of Field (linear depth) 
*  @note      ñ‚ëËì_: zPrepassÇ™çÏÇÁÇÍÇƒÇ»Ç¢. 
*****************************************************************************/
class Dof
{
protected:
	struct BlurParameter
	{
		float TextureSize[2];
		float Radius;
	};
	struct ClipSize
	{
		float Near;
		float Far;
	};
	using  ClipSizePtr   = std::unique_ptr<UploadBuffer>;
	using  BlurBufferPtr = std::unique_ptr<UploadBuffer>;

public:
	/****************************************************************************
	**                Public Function
	*****************************************************************************/
	void StartOn(float width, float height, float radius = 4.0f, float nearClip = 0.2f, float farClip = 0.8f, const std::wstring& addname = L"");
	void OnResize(float newWidth, float newHeight);
	void Draw(GPUResource* renderTarget, GPUResource* zPrepass);
	void ShutDown();
	/****************************************************************************
	**                Public Member Variables
	*****************************************************************************/
	void SetUpBlurParameter(float width, float height, float radius = 8.0f);
	void SetUpClipSize(float nearClip = 0.2f, float farClip = 0.8f);
	/****************************************************************************
	**                Constructor and Destructor
	*****************************************************************************/
	Dof();
	Dof(const Dof&)            = delete;
	Dof& operator=(const Dof&) = delete;
	Dof(Dof&&)                 = default;
	Dof& operator=(Dof&&)      = default;
	~Dof();
protected:
	/****************************************************************************
	**                Protected Function
	*****************************************************************************/
	void PrepareBlurParameterBuffer(float width, float height, float radius);
	void PrepareClipSizeBuffer(float nearClip, float farClip);
	void PreparePipelineState();

	/****************************************************************************
	**                Protected Member Variables
	*****************************************************************************/
	BlurBufferPtr _blurParameterBuffer  = nullptr;
	BlurParameter _blurParameter;
	ClipSizePtr   _clipSizeBuffer       = nullptr;
	ClipSize      _clipSize;
	ColorBuffer   _colorBuffer[6];
	std::wstring  _addName = L"";
};

#endif