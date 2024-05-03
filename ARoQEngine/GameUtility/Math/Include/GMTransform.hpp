//////////////////////////////////////////////////////////////////////////////////
///             @file   GMMatrix.hpp
///             @brief  Matrix
///             @author James Stanard  // Partially modified by Toide.
///             @date   2021_05_09 (modified)
//////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) Microsoft. All rights reserved.
// This code is licensed under the MIT License (MIT).
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//
// Developed by Minigraph
//
// Author:  James Stanard 
//
#pragma once
#ifndef GM_TRANSFORM_HPP
#define GM_TRANSFORM_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GMMatrix.hpp"
#include "GameUtility/Container/Include/GUDynamicArray.hpp"


//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////
//                              Class
//////////////////////////////////////////////////////////////////////////////////
namespace gm
{
	struct Transform;
	struct Transform
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		
		/****************************************************************************
		**                Public Property
		*****************************************************************************/
		// @brief : ���[�J����3�����ʒu
		Vector3f    LocalPosition;

		// @brief : ���[�J���̎p��
		QuaternionF LocalRotation;

		// @brief : ���[�J���̊g�嗦
		Vector3f    LocalScale;

		/*----------------------------------------------------------------------
		*  @brief : �e��ݒ肵�܂�
		/*----------------------------------------------------------------------*/
		__forceinline void SetParent(Transform* parent)
		{
			if (_parent != nullptr) { RemoveChild(this); }
			if (parent  != nullptr) { parent->SetChild(this); }
			_parent = parent;
		}

		/*----------------------------------------------------------------------
		*  @brief :�@Scale * Rotation * Transtion����ϊ��s����擾
		/*----------------------------------------------------------------------*/
		__forceinline Matrix4f GetMatrix() const
		{
			Matrix4f parent = _parent != nullptr ? _parent->GetMatrix() : Matrix4f();
			return (Scaling(LocalScale) * RotationQuaternion(LocalRotation) * Translation(LocalPosition) * parent);
		}

		/*----------------------------------------------------------------------
		*  @brief :�@Scale * Rotation * Transtion����ϊ��s����擾
		/*----------------------------------------------------------------------*/
		__forceinline Float4x4 GetFloat4x4()
		{
			Matrix4f parent = _parent != nullptr ? _parent->GetMatrix() : Matrix4f();
			return (Scaling(LocalScale) * RotationQuaternion(LocalRotation) * Translation(LocalPosition) * parent).ToFloat4x4();
		}

		/*----------------------------------------------------------------------
		*  @brief :�@�q���擾���܂�
		/*----------------------------------------------------------------------*/
		__forceinline gu::uint64  GetChildCount() const { return _children.Size(); }
		
		/*----------------------------------------------------------------------
		*  @brief :�@�q���擾���܂�
		/*----------------------------------------------------------------------*/
		__forceinline Transform* GetChild(const gu::uint64 index) const { return index < _children.Size() ? _children[index] : nullptr; }
		
		/*----------------------------------------------------------------------
		*  @brief : �����K�w�Ɏq��ݒu���܂�
		/*----------------------------------------------------------------------*/
		__forceinline void SetChild(Transform* child) { _children.Push(child); };
		
		/*----------------------------------------------------------------------
		*  @brief : �e��ݒ肵�܂�
		/*----------------------------------------------------------------------*/
		__forceinline void RemoveChild(Transform* child)
		{
			_children.Remove(child);
		}

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		// @brief : Default Constructor
		Transform() : LocalPosition(Vector3f()), LocalRotation(QuaternionF()), LocalScale(Vector3f(1,1,1)) {}
		
		// @brief : 3�����ʒu���g���ď�����
		Transform(const Vector3f& position) : LocalPosition(position), LocalRotation(QuaternionF()), LocalScale(Vector3f(1, 1, 1)) {};
		
		// @brief : 3�����ʒu�ƃN�H�[�^�j�I�����g���ď�����
		Transform(const Vector3f& position, const QuaternionF& rotation) : LocalPosition(position), LocalRotation(rotation), LocalScale(Vector3f(1, 1, 1)) {};

		// @brief : 3�����ʒu�ƃN�H�[�^�j�I���ƃX�P�[�����g���ď�����
		Transform(const Vector3f& position, const QuaternionF& rotation, const Vector3f& scale) : LocalPosition(position), LocalRotation(rotation), LocalScale(scale) {};
		
		~Transform() { SetParent(nullptr); }

	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/
		

		/****************************************************************************
		**                Private Property
		*****************************************************************************/
		Transform* _parent = nullptr;
		gu::DynamicArray<Transform*> _children = {};
	};

}
#endif