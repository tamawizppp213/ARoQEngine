//////////////////////////////////////////////////////////////////////////////////
///  @file   WindowsCommonState.hpp
///  @brief  temp
///  @author toide
///  @date   2024/06/30 11:35:03
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef WINDOWS_COMMON_STATE_HPP
#define WINDOWS_COMMON_STATE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/GUString.hpp"
#include "GameUtility/Container/Include/GUDynamicArray.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace platform::windows::details
{
	/****************************************************************************
	*				  	DragDropOperationType
	****************************************************************************/
	/* @brief  �h���b�O�h���b�v�̎��
	*****************************************************************************/
	enum class DragDropOperationType
	{
		DragEnter, //!< �h���b�O�J�n
		DragOver,  //!< �h���b�O��
		DragLeave, //!< �h���b�O�I��
		Drop,      //!< �h���b�v����
	};

	/****************************************************************************
	*				  	DragDropOLEData   
	****************************************************************************/
	/* @brief  �h���b�O�h���b�v�������Ƃ��Ɋm�ۂ���f�[�^
	*****************************************************************************/
	struct DragDropOLEData
	{
		enum WindowsOLEDataType
		{
			None  = 0,
			Text  = 1<<0,
			Files = 1<<1
		};

		/*! @brief ���쎞�̃e�L�X�g*/
		gu::tstring TextString = SP("");

		/*! @brief ���삵�Ă���t�@�C����*/
		gu::DynamicArray<gu::tstring> FileNameList = {};

		/*! @brief ���*/
		gu::uint8 TypeFlag = WindowsOLEDataType::None;
	};
}


#endif