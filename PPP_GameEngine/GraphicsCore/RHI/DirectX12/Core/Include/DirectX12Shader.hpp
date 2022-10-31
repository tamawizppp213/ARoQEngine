//////////////////////////////////////////////////////////////////////////////////
///             @file   DirectX12Shader.hpp
///             @brief  DirectX12 Compile Shader
///             @author Toide Yutaro
///             @date   2022_03_14
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef DIRECTX12_SHADER_HPP
#define DIRECTX12_SHADER_HPP
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIShader.hpp"
#include "DirectX12Core.hpp"
#include <string>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
struct _D3D_SHADER_MACRO;
struct SimpleGraphicPipeline
{
	BlobComPtr VertexShader;
	BlobComPtr PixelShader;
};

//////////////////////////////////////////////////////////////////////////////////
//                         DirectX12 Shader Function (inline)
//////////////////////////////////////////////////////////////////////////////////
/****************************************************************************
*				  			    RHIShader
*************************************************************************//**
*  @class     RHIShader
*  @brief     Shader
*****************************************************************************/
namespace rhi::directX12
{
	class RHIShader : public core::RHIShader
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		void Compile(const std::wstring& fileName, const std::wstring& entryPoint, const std::wstring& target) override;
		void LoadBinary(const std::wstring& fileName) override ;
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		BlobComPtr GetDxBlob() const noexcept { return _dxBlob; }
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		RHIShader() = default;
		~RHIShader() = default;
	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/

		/****************************************************************************
		**                Private Member Variables
		*****************************************************************************/
		BlobComPtr _dxBlob = nullptr;
	};
}

BlobComPtr CompileShader(
	const std::wstring& fileName,
	const _D3D_SHADER_MACRO* defines,
	const std::string& entryPoint,
	const std::string& target);

BlobComPtr CompileShader(
	const std::wstring& fileName,
	const std::wstring& entryPoint,
	const std::wstring& target);

BlobComPtr LoadBinary(const std::wstring& fileName);
#endif