//////////////////////////////////////////////////////////////////////////////////
///             @file   GLTFModelConverter.hpp
///             @brief  GLTF model <-> Game engine model
///             @author Toide Yutaro
///             @date   2023_02_01
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GLTF_MODEL_CONVERTER_HPP
#define GLTF_MODEL_CONVERTER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../../../../Include/GameModelConverter.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
namespace gltf
{
	class GLTFFile;
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
	class GLTFConverter : public IGameModelConverter, public Copyable
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		bool Load([[maybe_unused]] const std::wstring& filePath, [[maybe_unused]] GameModelPtr model) override;

		bool Save([[maybe_unused]]const std::wstring& filePath, [[maybe_unused]] const GameModelPtr model) override { return false; };


		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		~GLTFConverter() = default;
	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/
		void PrepareTotalMesh(const GameModelPtr model, gltf::GLTFFile& file);

		void PrepareEachMaterialMesh(const GameModelPtr model, gltf::GLTFFile& file);
	};

}
#endif