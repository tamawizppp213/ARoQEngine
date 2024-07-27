//////////////////////////////////////////////////////////////////////////////////
///  @file   SFMesh.hpp
///  @brief  temp
///  @author toide
///  @date   2024/07/21 23:18:28
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef SF_MESH_HPP
#define SF_MESH_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Math/Include/GMVector.hpp"
#include "GameUtility/Container/Include/GUDynamicArray.hpp"
#include "GameUtility/Container/Include/GUHashMap.hpp"
#include "GameUtility/Base/Include/GUString.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace engine::file::sf
{
	/****************************************************************************
	*				  			   SFMesh
	****************************************************************************/
	/* @brief  �O���t�@�C������ǂݍ��ލۂ̋��ʃ��b�V���f�[�^. GameEngine�ɒ��ڊi�[����̂ł͂Ȃ�, @n 
	*          �����܂Ń��f���f�[�^�̈ꎞ�I�ȕۑ���ړI�Ƃ���. 
	*****************************************************************************/
	struct SFMesh
	{
	public:
		#pragma region Public Function

		#pragma endregion 

		#pragma region Public Property
		/*! @brief 3�����ʒu [���b�V���ɂ͏�Ɋi�[�����]*/
		gu::DynamicArray<gm::Float3> Positions = {};

		/*! @brief 3�����̐��K�����ꂽ�@���x�N�g��. �_�Ɛ������̃��b�V���͖@���x�N�g���͎��ĂȂ�. */
		gu::DynamicArray<gm::Float3> Normals = {};

		/*! @brief �e�N�X�`����X���������w��. ���K�����ꂽ�ڐ��x�N�g��*/
		gu::DynamicArray<gm::Float3> Tangents = {};

		/*! @brief �e�N�X�`����Y���������w��. ���K�����ꂽ�ڐ��x�N�g��*/
		gu::DynamicArray<gm::Float3> Bitangents = {};

		/*! @brief ���_�J���[*/
		gu::DynamicArray<gm::Float4> Colors = {};

		/*! @brief �e�N�X�`����UVW�l*/
		gu::DynamicArray<gm::Float3> TextureCoords = {};

		/*! @brief ���̃��b�V���ɓK�p�����}�e���A��Index*/
		gu::uint32 MaterialIndex = 0;

		/*! @brief ���b�V���̖��O*/
		gu::tstring Name = SP("");

		/*!**********************************************************************
		*  @brief     3�����ʒu���擾���Ă��邩
		*  @param[in] void
		*  @return    bool 3�����ʒu���擾���Ă���ꍇ��true, �����łȂ��ꍇ��false
		*************************************************************************/
		__forceinline bool HasPositions() const noexcept { return !Positions.IsEmpty(); }

		/*!**********************************************************************
		*  @brief     �@���x�N�g�����擾���Ă��邩
		*  @param[in] void
		*  @return    bool �@���x�N�g�����擾���Ă���ꍇ��true, �����łȂ��ꍇ��false
		*************************************************************************/
		__forceinline bool HasNormals() const noexcept { return !Normals.IsEmpty(); }

		/*!**********************************************************************
		*  @brief     �ڐ��x�N�g�����擾���Ă��邩
		*  @param[in] void
		*  @return    bool �ڐ��x�N�g�����擾���Ă���ꍇ��true, �����łȂ��ꍇ��false
		*************************************************************************/
		__forceinline bool HasTangents() const noexcept { return !Tangents.IsEmpty(); }

		/*!**********************************************************************
		* @brief     �ڐ��x�N�g�����擾���Ă��邩
		* @param[in] void
		* @return    bool �ڐ��x�N�g�����擾���Ă���ꍇ��true, �����łȂ��ꍇ��false
		*************************************************************************/
		__forceinline bool HasBitangents() const noexcept { return !Bitangents.IsEmpty(); }

		#pragma endregion 

		#pragma region Public Operator 

		#pragma endregion 

		#pragma region Public Constructor and Destructor
		
		#pragma endregion 

	protected:
		#pragma region Protected Constructor and Destructor

		#pragma endregion 

		#pragma region Protected Function

		#pragma endregion 

		#pragma region Protected Property

		#pragma endregion

	private:
		#pragma region Private Constructor and Destructor

		#pragma endregion

		#pragma region Private Function

		#pragma endregion 

		#pragma region Private Property

		#pragma endregion 

	};
}

#endif