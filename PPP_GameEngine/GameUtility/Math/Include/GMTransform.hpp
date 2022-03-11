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
#include <vector>

#pragma warning(disable: 26812 26495)

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////
//                              Class
//////////////////////////////////////////////////////////////////////////////////
namespace gm
{
	Quaternion GetAffineTransformRotation(const Matrix3& matrix);
	struct OrthogonalTransform;
	struct AffineTransform;
	struct ScaleAndTranslation;
	struct UniformScaleTransform;
	struct Transform;

	struct Transform
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		Vector3    LocalPosition;
		Quaternion LocalRotation;
		Vector3    LocalScale;

		INLINE void SetParent(Transform* parent)
		{
			if (_parent != nullptr) { RemoveChild(this); }
			if (parent  != nullptr) { parent->SetChild(this); }
			_parent = parent;
		}

		INLINE Matrix4 GetMatrix() const
		{
			Matrix4 model;
			if (_parent != nullptr) { model = _parent->GetMatrix(); }
			else { model = Matrix4(EIdentityTag::kIdentity); }

			return Scaling(LocalScale) * RotationQuaternion(LocalRotation) * Translation(LocalPosition) * model;
		}

		INLINE Float4x4 GetFloat4x4()
		{
			Matrix4 model;
			if (_parent != nullptr) { model = _parent->GetMatrix(); }
			else { model = Matrix4(EIdentityTag::kIdentity); }
			return (Scaling(LocalScale) * RotationQuaternion(LocalRotation) * Translation(LocalPosition) * model).ToFloat4x4();
		}

		INLINE int  GetChildCount()           const { return static_cast<int>(_child.size()); }
		INLINE Transform* GetChild(int index) const { return (_child.size() <= index) ? nullptr : _child[index]; }
		INLINE void SetChild(Transform* child) { _child.push_back(child); };
		INLINE bool RemoveChild(Transform* child)
		{
			for (auto it = _child.begin(); it != _child.end(); ++it)
			{
				if (*it == child)
				{
					_child.erase(it); return true;
				}
			}
			return false;
		}

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		INLINE Transform()                  : LocalPosition(kZero)   , LocalRotation(kIdentity), LocalScale(kIdentity) {}
		INLINE Transform(Quaternion rotate) : LocalPosition(kZero)   , LocalRotation(rotate)   , LocalScale(kIdentity) {}
		INLINE Transform(Vector3 position)  : LocalPosition(position), LocalRotation(kIdentity), LocalScale(kIdentity) {}
		INLINE Transform(float x, float y, float z)                          : LocalPosition(x,y,z), LocalRotation(kIdentity), LocalScale(kIdentity) {}
		INLINE Transform(Quaternion rotate, Vector3 position)                : LocalRotation(rotate), LocalPosition(position), LocalScale(kIdentity) {}
		INLINE Transform(Vector3 scale, Quaternion rotate, Vector3 position) : LocalRotation(rotate), LocalPosition(position), LocalScale(scale) {}
		INLINE ~Transform() { SetParent(nullptr); }

	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/
		

		/****************************************************************************
		**                Private Member Variables
		*****************************************************************************/
		Transform* _parent = nullptr;
		std::vector<Transform*> _child;
	};


	struct OrthogonalTransform
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		INLINE void SetRotation(const Quaternion& quaternion) { _rotation = quaternion; }
		INLINE void SetTranslation(const Vector3& vector)     { _translation = vector; }
		INLINE Quaternion GetRotation() const { return _rotation; }
		INLINE Vector3 GetTranslation() const { return _translation; }
		static INLINE OrthogonalTransform MakeXRotation(float angle) { return OrthogonalTransform(Quaternion(Vector3(kXUnitVector), angle)); }
		static INLINE OrthogonalTransform MakeYRotation(float angle) { return OrthogonalTransform(Quaternion(Vector3(kYUnitVector), angle)); }
		static INLINE OrthogonalTransform MakeZRotation(float angle) { return OrthogonalTransform(Quaternion(Vector3(kZUnitVector), angle)); }
		static INLINE OrthogonalTransform MakeTranslation(Vector3 translate) { return OrthogonalTransform(translate); }

		INLINE Vector3 operator* (Vector3 vector) const { return _rotation * vector + _translation; }
		INLINE Vector4 operator* (Vector4 vector) const 
		{ 
			return Vector4(vector::SetWToZero(_rotation      * Vector3((DirectX::XMVECTOR)vector))) +
				   Vector4(vector::SetWToOne (_translation)) * vector.GetW();
		}
		INLINE OrthogonalTransform operator* (const OrthogonalTransform& xform) const
		{
			return OrthogonalTransform(_rotation * xform._rotation, _rotation * xform._translation + _translation);
		}
		INLINE OrthogonalTransform operator~ () const
		{
			Quaternion invertedRotation = ~_rotation;
			return OrthogonalTransform(invertedRotation, invertedRotation * - _translation);
		}

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		INLINE OrthogonalTransform()                                       : _rotation(kIdentity), _translation(kZero)     {}
		INLINE OrthogonalTransform(Quaternion rotate)                      : _rotation(rotate)   , _translation(kZero)     {}
		INLINE OrthogonalTransform(Vector3 translate)                      : _rotation(kIdentity), _translation(translate) {}
		INLINE OrthogonalTransform(Quaternion rotate, Vector3 translate)   : _rotation(rotate)   , _translation(translate) {}
		INLINE OrthogonalTransform(const Matrix3& mat)                     : _rotation(mat)      , _translation(kZero)     {}
		INLINE OrthogonalTransform(const Matrix3& mat, Vector3 translate)  : _rotation(mat)      , _translation(translate) {}
		INLINE OrthogonalTransform(EIdentityTag)                           : _rotation(kIdentity), _translation(kZero)     {}
		INLINE explicit OrthogonalTransform(const DirectX::XMMATRIX& mat) { *this = OrthogonalTransform(Matrix3(mat), Vector3(mat.r[3])); }
	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/

		/****************************************************************************
		**                Private Member Variables
		*****************************************************************************/
		Quaternion _rotation;
		Vector3 _translation;
		
	};

	struct ScaleAndTranslation
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		INLINE void SetScale(Scalar s)        { _vector.SetW(s); }
		INLINE void SetTranslation(Vector3 t) { _vector.SetXYZ(t); }
		INLINE Scalar  GetScale()       const { return   _vector.GetW(); }
		INLINE Vector3 GetTranslation() const { (Vector3)_vector; }
		

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		INLINE ScaleAndTranslation() {}
		INLINE ScaleAndTranslation(EIdentityTag) : _vector(vector::CreateWUnitVector()) {};
		INLINE ScaleAndTranslation(float tx, float ty, float tz, float scale) : _vector(tx, ty, tz, scale) {};
		INLINE ScaleAndTranslation(Vector3 translate, Scalar scale)
		{
			_vector = Vector4(translate);
			_vector.SetW(scale);
		}
		INLINE explicit ScaleAndTranslation(const DirectX::XMVECTOR& v) : _vector(v){}

	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/

		/****************************************************************************
		**                Private Member Variables
		*****************************************************************************/
		Vector4 _vector;
	};

	struct UniformScaleTransform
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		INLINE void SetRotation(Quaternion rotation)    { _rotation = rotation; }
		INLINE void SetScale(Scalar scale)              { _translationScale.SetScale(scale); }
		INLINE void SetTranslation(Vector3 translation) { _translationScale.SetTranslation(translation); }


		INLINE Quaternion GetRotation() const { return _rotation; }
		INLINE Scalar GetScale()        const { return _translationScale.GetScale(); }
		INLINE Vector3 GetTranslation() const { return _translationScale.GetTranslation(); }

		INLINE Vector3 operator*(Vector3 vector) const
		{
			return _rotation * (vector * _translationScale.GetScale()) + _translationScale.GetTranslation();
		}
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		INLINE UniformScaleTransform(){}
		INLINE UniformScaleTransform(EIdentityTag) : _rotation(kIdentity), _translationScale(kIdentity){}
		INLINE UniformScaleTransform(Quaternion rotation, Scalar scale, Vector3 translation) : _rotation(rotation), _translationScale(translation, scale) {}


	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/

		/****************************************************************************
		**                Private Member Variables
		*****************************************************************************/
		Quaternion _rotation;
		ScaleAndTranslation _translationScale;
	};

	// A AffineTransform is a 3x4 matrix with an implicit 4th row = [0,0,0,1].  This is used to perform a change of
	// basis on 3D points.  An affine transformation does not have to have orthonormal basis vectors.
	struct AffineTransform
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		INLINE void SetX(Vector3 x) { _basis.SetX(x); }
		INLINE void SetY(Vector3 y) { _basis.SetY(y); }
		INLINE void SetZ(Vector3 z) { _basis.SetZ(z); }
		INLINE void SetTranslation(Vector3 w) { _translation = w; }
		INLINE void SetBasis(const Matrix3& m) { _basis = m; }
		INLINE void SetRotation(const Quaternion& q) { _basis = Matrix3(q); }
		INLINE Vector3 GetX() const { return _basis.GetX(); }
		INLINE Vector3 GetY() const { return _basis.GetY(); }
		INLINE Vector3 GetZ() const { return _basis.GetZ(); }
		INLINE Vector3 GetTranslation() const  { return _translation; }
		INLINE const Matrix3& GetBasis() const { return (const Matrix3&)*this; }
		INLINE Quaternion   GetRotation() const { return GetAffineTransformRotation(this->GetBasis()); }
		INLINE Vector3 operator* (Vector3 vector) const { return _basis * vector + _translation; }
		INLINE AffineTransform operator* (const AffineTransform& matrix) const{return AffineTransform(_basis * matrix._basis, *this * matrix.GetTranslation());}
		bool operator == (const AffineTransform& transform) const noexcept;
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		INLINE AffineTransform(){}
		INLINE AffineTransform(Vector3 x, Vector3 y, Vector3 z, Vector3 w)                : _basis(x, y, z)  , _translation(w){}
		INLINE AffineTransform(Vector3 translate)                                         : _basis(kIdentity), _translation(translate){}
		INLINE AffineTransform(const Matrix3& matrix, Vector3 translate = Vector3(kZero)) : _basis(matrix)   , _translation(translate){}
		INLINE AffineTransform(Quaternion rotation,   Vector3 translate = Vector3(kZero)) : _basis(rotation) , _translation(translate){}
		INLINE AffineTransform(const OrthogonalTransform& xform)                          : _basis(xform.GetRotation()), _translation(xform.GetTranslation()){}
		INLINE AffineTransform(const UniformScaleTransform& xform)
		{
			_basis = Matrix3(xform.GetRotation()) * xform.GetScale();
			_translation = xform.GetTranslation();
		}
		INLINE AffineTransform(EIdentityTag) : _basis(kIdentity), _translation(kZero){}
		INLINE explicit AffineTransform(const DirectX::XMMATRIX& mat): _basis(mat), _translation(mat.r[3]){}
		INLINE operator DirectX::XMMATRIX() const { return (DirectX::XMMATRIX&)*this; }

	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/

		/****************************************************************************
		**                Private Member Variables
		*****************************************************************************/
		Matrix3 _basis;
		Vector3 _translation;
	};
	INLINE bool AffineTransform::operator == (const AffineTransform& transform) const noexcept
	{
		return (_basis == transform.GetBasis() &&
			_translation == transform.GetTranslation());
	}

	INLINE Vector3 InverseXForm(AffineTransform& transform, const Vector3& inVector)
	{
		Vector3 v = inVector - transform.GetTranslation();
		return (Transpose(transform.GetBasis()) * v);
	}
	INLINE Vector3 InverseXForm(const AffineTransform& transform, const Vector3& inVector)
	{
		Vector3 v = inVector - transform.GetTranslation();
		return (Transpose(transform.GetBasis()) * v);
	}
	INLINE AffineTransform InverseAffineTransform(const AffineTransform& transform)
	{
		Matrix3 inverse = Transpose(transform.GetBasis());
		return AffineTransform(inverse, inverse * (-transform.GetTranslation()));
	}
}
#endif