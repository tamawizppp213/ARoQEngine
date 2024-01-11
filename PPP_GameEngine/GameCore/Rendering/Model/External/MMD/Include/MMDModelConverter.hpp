//////////////////////////////////////////////////////////////////////////////////
///             @file   MMDModelConverter.hpp
///             @brief  Pmx amd pmd model <-> Game engine model
///             @author Toide Yutaro
///             @date   2023_02_01
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef MMD_MODEL_CONVERTER_HPP
#define MMD_MODEL_CONVERTER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../../../Include/GameModelConverter.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
namespace pmx
{
	class PMXFile;
}
//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace gc::core
{
	class GameModel;
	/****************************************************************************
	*				  			    PMXConverter
	*************************************************************************//**
	*  @class     PMXConverter
	*  @brief     PMX model <-> Game engine model
	*****************************************************************************/
	class PMXConverter : public IGameModelConverter, public Copyable
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		bool Load(const std::wstring& filePath, GameModelPtr model) override;

		bool Save([[maybe_unused]] const std::wstring& filePath, [[maybe_unused]] const GameModelPtr model) override { return false; };


		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		~PMXConverter() = default;
	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/
		void PrepareTotalMesh(const GameModelPtr model, pmx::PMXFile& file);

		void PrepareEachMaterialMesh(const GameModelPtr model, pmx::PMXFile& file);
	};

	/****************************************************************************
	*				  			    Class
	*************************************************************************//**
	*  @class     Class
	*  @brief     temp
	*****************************************************************************/
	class PMDConverter : public IGameModelConverter, public Copyable
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		bool Load([[maybe_unused]] const std::wstring& filePath, [[maybe_unused]] GameModelPtr model) override;

		bool Save([[maybe_unused]] const std::wstring& filePath, [[maybe_unused]] const GameModelPtr model) override { return false; };


		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		~PMDConverter() = default;
	};
}
#endif