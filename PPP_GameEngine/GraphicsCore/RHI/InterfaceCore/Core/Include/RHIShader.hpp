//////////////////////////////////////////////////////////////////////////////////
///             @file   RHIShader.hpp
///             @brief  RHIShader
///             @author Toide Yutaro
///             @date   2022_10_31
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef RHI_SHADER_HPP
#define RHI_SHADER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommonState.hpp"
#include "GameUtility/Base/Include/ClassUtility.hpp"
#include <string>
#include <memory>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

/****************************************************************************
*				  			    RHIShader
*************************************************************************//**
*  @class     RHIShader
*  @brief     Shader
*****************************************************************************/
namespace rhi::core
{
	class RHIDevice;

	class RHIShader : public NonCopyable
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		virtual void Compile(const std::wstring& fileName, const std::wstring& entryPoint, const std::wstring& target) = 0;
		virtual void LoadBinary(const std::wstring& fileName) = 0;
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		void* GetBufferPointer() const noexcept { return _bufferPointer; }
		std::uint64_t GetBufferByteSize() const { return _bufferByteSize; }
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		RHIShader() = default;
		virtual ~RHIShader() = default;
		RHIShader(const std::shared_ptr<RHIDevice>& device) : _device(device) {};
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		std::shared_ptr<RHIDevice> _device = nullptr;
		void*         _bufferPointer  = nullptr;
		std::uint64_t _bufferByteSize = 0;
	};

}
#endif