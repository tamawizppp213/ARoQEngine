//////////////////////////////////////////////////////////////////////////////////
///             @file   RHIMemory.hpp
///             @brief  memory
///             @author Toide Yutaro
///             @date   2022_09_21
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef RHI_MEMORY_HPP
#define RHI_MEMORY_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommonState.hpp"
#include "GameUtility/Base/Include/GUClassUtility.hpp"
#include "GameUtility/Base/Include/GUSmartPointer.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                              Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::core
{
	class RHIDevice;
	/****************************************************************************
	*				  			RHIMemory
	****************************************************************************/
	/* @class     RHIMemory
	*  @brief     Memory buffer
	*****************************************************************************/
	class RHIMemory : public gu::NonCopyable
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/

		/****************************************************************************
		**                Public Property
		*****************************************************************************/
		MemoryHeap GetMemoryType() const noexcept { return _memoryType; }

		std::uint64_t GetMemorySize() const { return _memorySize; }

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
	
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		RHIMemory() = default;

		virtual ~RHIMemory() = default;

		explicit RHIMemory(const gu::SharedPointer<RHIDevice>& device, const MemoryHeap memoryHeapType, const std::uint64_t memorySize) 
			: _device(device), _memoryType(memoryHeapType), _memorySize(memorySize) {};
		
		/****************************************************************************
		**                Protected Property
		*****************************************************************************/
		MemoryHeap _memoryType = MemoryHeap::Default; 

		std::uint64_t _memorySize = 0;

		gu::SharedPointer<RHIDevice> _device = nullptr;
	};
}

#endif