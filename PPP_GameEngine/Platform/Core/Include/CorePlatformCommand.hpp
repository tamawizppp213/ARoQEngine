//////////////////////////////////////////////////////////////////////////////////
///             @file   CorePlatformCommand.hpp
///             @brief  Windows.hで記述されているような関数をまとめたクラス
///             @author toide
///             @date   2023/10/30 2:39:01
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef CORE_PLATFORM_COMMAND_HPP
#define CORE_PLATFORM_COMMAND_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "CoreCommonState.hpp"
#include "GameUtility/Base/Include/ClassUtility.hpp"
#include "GameUtility/Base/Include/GUUUID.hpp"
#include <memory>

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace platform::core
{
	/****************************************************************************
	*				  			   CorePlatformCommand
	*************************************************************************//**
	*  @class     CorePlatformCommand
	*  @brief     Windows.hで記述されているような関数をまとめたクラス
	*****************************************************************************/
	class PlatformCommand : public NonCopyable
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
#pragma region Debuggging Command
		/*----------------------------------------------------------------------
		*  @brief : This function checks debug mode.
		/*----------------------------------------------------------------------*/
		virtual bool UseDebugger() const = 0;

		/*----------------------------------------------------------------------
		*  @brief : This function puts on the debug break
		/*----------------------------------------------------------------------*/
		virtual void PutDebugBreak() const = 0;

#pragma endregion Debugging Command
		/*----------------------------------------------------------------------
		*  @brief : This function issues the UUID which specifies the object.
		/*----------------------------------------------------------------------*/
		virtual gu::UUID IssueUUID() = 0;

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		PlatformCommand() = default;

		virtual ~PlatformCommand() = default;
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
	};

}
#endif