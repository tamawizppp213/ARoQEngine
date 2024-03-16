//////////////////////////////////////////////////////////////////////////////////
///             @file   IESProfiler.hpp
///             @brief  IES Profile parser
///                     https://ieslibrary.com/en/home (IES light library)
///                     https://vdocuments.mx/ies-specification-lm-63-2002.html?page=3
///             @author Toide Yutaro
///             @date   2022_05_10
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef IES_PROFILER_HPP
#define IES_PROFILER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "IESProperty.hpp"
#include "GameUtility/Base/Include/GUClassUtility.hpp"
#include <string>

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace gc::rendering
{

	class IESProfiler : public gu::NonCopyable
	{
	protected:
		
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/* @brief : Load lamp data + GPUTexture resources.*/
		void Load(const std::wstring& filePath);

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		/* @brief : Return lamp structure*/
		const Lamp& GetLamp() const { return _lamp; }

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		IESProfiler() = default;

		~IESProfiler();

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		void LoadLamp(const std::wstring& name);

		void SetUpTexture();

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		Lamp _lamp = {};

		IESVersion _version = IESVersion::IES_1986;
	private:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		/* @brief: Determine if the IES format identifier is correct. 
		           Please add it when a new version is released. */
		bool IsProperIESFormat(const std::string& formatName);
	};
}
#endif