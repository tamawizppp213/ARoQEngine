//////////////////////////////////////////////////////////////////////////////////
///             @file   OBJUTILS.hpp
///             @brief  OBJ File Utility
///             @author Toide Yutaro
///             @date   2022_05_05
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef OBJ_UTILS_HPP
#define OBJ_UTILS_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace obj
{
	namespace util
	{
		/****************************************************************************
		*                       IsEndOfBuffer
		*************************************************************************//**
		*  @fn        inline bool IsEndBuffer(Char_T it, Char_T end)
		*  @brief     Returns true, if the last entry of the buffer is reached
		*  @param[in] char_t it
		*  @param[in] char_t end
		*  @return Å@Å@bool
		*****************************************************************************/
		template<class Char_T>
		inline bool IsEndBuffer(Char_T it, Char_T end)
		{
			if (it == end) { return true; }
			--end;
			return (it == end);
		}

		template<class Char_T>
		inline Char_T GetNextWord(Char_T buffer, Char_T end)
		{
			return buffer;
		}
	}
}
#endif