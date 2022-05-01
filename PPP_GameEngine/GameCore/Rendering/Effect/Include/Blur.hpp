//////////////////////////////////////////////////////////////////////////////////
///             @file   Blur.hpp
///             @brief  Blur effect
///             @author Toide Yutaro
///             @date   2022_05_01
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef BLUR_HPP
#define BLUR_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/DirectX12/Core/Include/DirectX12Buffer.hpp"
#include "GameUtility/Math/Include/GMVector.hpp"
#include <memory>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////

/****************************************************************************
*				  			GaussianBlur
*************************************************************************//**
*  @class     GaussianBlur
*  @brief     Gaussian + HalfDownSampling + Bilinear Filter
*****************************************************************************/
class GaussianBlur
{
	struct BlurParameter
	{
		gm::Float4 Weights[2];
	};
	struct TextureSizeParameter
	{
		int OriginalTexture[2];
		int XBlurTexture[2];
		int YBlurTexture[2];
	};
	using BlurParameterPtr     = std::unique_ptr<UploadBuffer>;
	using TextureSizeBufferPtr = std::unique_ptr<UploadBuffer>;
public:
	/****************************************************************************
	**                Public Function
	*****************************************************************************/
	void StartOn(int width, int height, const std::wstring& addName = L"");
	void OnResize(int newWidth, int newHeight);
	void Draw(GPUResource* renderTarget);
	void ShutDown();
	/****************************************************************************
	**                Public Member Variables
	*****************************************************************************/
	void SetUpWeightTable(float sigma);
	/****************************************************************************
	**                Constructor and Destructor
	*****************************************************************************/
	GaussianBlur();
	~GaussianBlur();
	GaussianBlur(const GaussianBlur&)            = delete;
	GaussianBlur& operator=(const GaussianBlur&) = delete;
	GaussianBlur(GaussianBlur&&)                 = default;
	GaussianBlur& operator=(GaussianBlur&&)      = default;
protected:
	/****************************************************************************
	**                Protected Function
	*****************************************************************************/
	void PrepareBlurParameters();
	void PrepareTextureSizeBuffer(int width, int height);
	void PreparePipelineState();
	/****************************************************************************
	**                Protected Member Variables
	*****************************************************************************/
	std::wstring     _addName = L"";
	TextureSizeParameter _textureSize;
	BlurParameterPtr     _blurParameter     = nullptr;
	TextureSizeBufferPtr _textureSizeBuffer = nullptr;
	ColorBuffer          _colorBuffer[4];
};

#endif