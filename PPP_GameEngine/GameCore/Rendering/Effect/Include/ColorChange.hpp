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
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Buffer.hpp"
#include <memory>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
class PipelineState;
//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
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
class ColorChange
{
	using PipelineStatePtr = std::unique_ptr<PipelineState>;
	using MeshBufferPtr    = std::unique_ptr<MeshBuffer[]>;
public:
	/****************************************************************************
	**                Public Function
	*****************************************************************************/
	void StartOn(ColorChangeType type, const std::wstring& addName = L"");
	void OnResize(int newWidth, int newHeight);
	void Draw(GPUResource* renderTarget);
	void ShutDown();
	/****************************************************************************
	**                Public Member Variables
	*****************************************************************************/

	/****************************************************************************
	**                Constructor and Destructor
	*****************************************************************************/
	ColorChange();
	~ColorChange();
	ColorChange(const ColorChange&)            = delete;
	ColorChange& operator=(const ColorChange&) = delete;
	ColorChange(ColorChange&&)                 = default;
	ColorChange& operator=(ColorChange&&)      = default;
protected:
	/****************************************************************************
	**                Protected Function
	*****************************************************************************/
	void PrepareVertexAndIndexBuffer();
	void PreparePipelineState(ColorChangeType type);
	/****************************************************************************
	**                Protected Member Variables
	*****************************************************************************/
	std::wstring     _addName = L"";
	ColorBuffer      _colorBuffer;
	PipelineStatePtr _pipelineState = nullptr;
	MeshBufferPtr    _meshBuffer = nullptr;
};

#endif