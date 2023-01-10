//////////////////////////////////////////////////////////////////////////////////
///             @file   GMMatrix.hpp
///             @brief  Matrix
///             @author Toide Yutaro
///             @date   2021_05_15
//////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef GM_MATRIX_HPP
#define GM_MATRIX_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GMQuaternion.hpp"


#pragma warning(disable: 26812 26495)

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
namespace gm
{
	struct Matrix3;
	struct Matrix4;
	struct Float3x3;
	struct Float3x4;
	struct Float4x4;

	/****************************************************************************
	*				  			Float4x4
	*************************************************************************//**
	*  @class     Float3x4
	*  @brief     Float3x4 class
	*****************************************************************************/
	struct Float3x3 : public DirectX::XMFLOAT3X3
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		Float3x3()         : XMFLOAT3X3() {};
		Float3x3(float ix) : XMFLOAT3X3(ix, ix, ix, ix, ix, ix, ix, ix, ix) {};
		Float3x3(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22) 
			               : XMFLOAT3X3(m00,m01,m02, m10, m11, m12, m20, m21, m22) {};
		Float3x3(const XMFLOAT3X3& M)
		{
			this->_11 = M._11; this->_12 = M._12; this->_13 = M._13;
			this->_21 = M._21; this->_22 = M._22; this->_23 = M._23;
			this->_31 = M._31; this->_32 = M._32; this->_33 = M._33;
		};

		Float3x3(const Float3x3&)            = default;
		Float3x3& operator=(const Float3x3&) = default;
		Float3x3(Float3x3&&)                 = default;
		Float3x3& operator=(Float3x3&&)      = default;

		operator DirectX::XMMATRIX() const noexcept { return DirectX::XMLoadFloat3x3(this); }
		
	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/

		/****************************************************************************
		**                Private Member Variables
		*****************************************************************************/
		
	};

	/****************************************************************************
	*				  			Float4x4
	*************************************************************************//**
	*  @class     Float4x4
	*  @brief     Float4x4 class 
	*****************************************************************************/
	struct Float3x4 : public DirectX::XMFLOAT3X4
	{
		/****************************************************************************
		**                Public Function
		*****************************************************************************/


		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		Float3x4()         : XMFLOAT3X4() {};
		Float3x4(float ix) : XMFLOAT3X4(ix,ix,ix, ix, ix, ix, ix, ix, ix, ix, ix, ix) {};
		Float3x4(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23) 
			               : XMFLOAT3X4(m00,m01,m02,m03, m10, m11, m12, m13, m20, m21, m22, m23) {};
		Float3x4(const XMFLOAT3X4& M)
		{
			this->_11 = M._11; this->_12 = M._12; this->_13 = M._13; this->_14 = M._14;
			this->_21 = M._21; this->_22 = M._22; this->_23 = M._23; this->_24 = M._24;
			this->_31 = M._31; this->_32 = M._32; this->_33 = M._33; this->_34 = M._34;
		};

		Float3x4(const Float3x4&)            = default;
		Float3x4& operator=(const Float3x4&) = default;
		Float3x4(Float3x4&&)                 = default;
		Float3x4& operator=(Float3x4&&)      = default;

		operator DirectX::XMMATRIX() const noexcept { return DirectX::XMLoadFloat3x4(this); }

	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/
		

		/****************************************************************************
		**                Private Member Variables
		*****************************************************************************/
	};

	/****************************************************************************
	*				  			Float4x4
	*************************************************************************//**
	*  @class     Float4x4
	*  @brief     Float4x4 class 
	*****************************************************************************/
	struct Float4x4 : public DirectX::XMFLOAT4X4
	{
		/****************************************************************************
		**                Public Function
		*****************************************************************************/


		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		INLINE Float4x4()         : XMFLOAT4X4() {};
		INLINE Float4x4(float ix) : XMFLOAT4X4(ix,ix,ix,ix, ix, ix, ix, ix, ix, ix, ix, ix, ix, ix, ix, ix) {};
		INLINE Float4x4(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33) 
			               : XMFLOAT4X4(m00,m01,m02,m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33) {};
		INLINE Float4x4(const XMFLOAT4X4& M)
		{
			this->_11 = M._11; this->_12 = M._12; this->_13 = M._13; this->_14 = M._14;
			this->_21 = M._21; this->_22 = M._22; this->_23 = M._23; this->_24 = M._24;
			this->_31 = M._31; this->_32 = M._32; this->_33 = M._33; this->_34 = M._34;
			this->_41 = M._41; this->_42 = M._42; this->_43 = M._43; this->_44 = M._44;
		};
		INLINE Float4x4(const Float4x4&)            = default;
		INLINE Float4x4& operator=(const Float4x4&) = default;
		INLINE Float4x4(Float4x4&&)                 = default;
		INLINE Float4x4& operator=(Float4x4&&)      = default;

		operator DirectX::XMMATRIX() const noexcept { return DirectX::XMLoadFloat4x4(this); }

	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/
		

		/****************************************************************************
		**                Private Member Variables
		*****************************************************************************/
	};



	/****************************************************************************
	*				  			Matrix3
	*************************************************************************//**
	*  @class     Matrix3
	*  @brief     Matrix3X3 class (note! : 16 bytes (for 16 alignment))
	*****************************************************************************/
	__declspec(align(16))struct Matrix3
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		static INLINE Matrix3 MakeXRotation(float angle)                { return Matrix3(DirectX::XMMatrixRotationX(angle)); }
		static INLINE Matrix3 MakeYRotation(float angle)                { return Matrix3(DirectX::XMMatrixRotationY(angle)); }
		static INLINE Matrix3 MakeZRotation(float angle)                { return Matrix3(DirectX::XMMatrixRotationZ(angle)); }
		static INLINE Matrix3 MakeScale(float scale)                    { return Matrix3(DirectX::XMMatrixScaling(scale, scale, scale)); }
		static INLINE Matrix3 MakeScale(float sx, float sy, float sz)   { return Matrix3(DirectX::XMMatrixScaling(sx, sy, sz)); }
		static INLINE Matrix3 MakeScale(const DirectX::XMFLOAT3& scale) { return Matrix3(DirectX::XMMatrixScaling(scale.x, scale.y, scale.z)); }
		static INLINE Matrix3 MakeScale(Vector3 scale)                  { return Matrix3(DirectX::XMMatrixScalingFromVector(scale)); }

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		INLINE void SetX(Vector3 x) { _matrix[0] = x; }
		INLINE void SetY(Vector3 y) { _matrix[1] = y; }
		INLINE void SetZ(Vector3 z) { _matrix[2] = z; }
		INLINE void SetRow(int i, Vector3 value){ _matrix[i] = value; }
		INLINE void SetElement(int row, int column, Scalar value) { _matrix[row].SetElement(column, value); }
		INLINE Vector3 GetX() const { return _matrix[0]; }
		INLINE Vector3 GetY() const { return _matrix[1]; }
		INLINE Vector3 GetZ() const { return _matrix[2]; }
		INLINE Vector3 GetRow(int i) const { return _matrix[i]; }
		INLINE Vector3 GetColumn(int i) const { return Vector3(_matrix[0].GetElement(i), _matrix[1].GetElement(i), _matrix[2].GetElement(i)); }
		INLINE Scalar  GetElement(int row, int column) const { return _matrix[row].GetElement(column); }
		INLINE operator DirectX::XMMATRIX() const { return DirectX::XMMATRIX(_matrix[0], _matrix[1], _matrix[2], DirectX::XMVectorZero()); }

		INLINE Matrix3 operator* (Scalar scl)         const { return Matrix3(scl * GetX(), scl * GetY(), scl * GetZ()); }
		INLINE Vector3 operator* (Vector3 vec)        const { return Vector3(DirectX::XMVector3TransformNormal(vec, *this)); }
		INLINE Matrix3 operator* (const Matrix3& mat) const { return Matrix3(*this * mat.GetX(), *this * mat.GetY(), *this * mat.GetZ()); }

		bool operator == (const Matrix3& M) const noexcept;
		bool operator != (const Matrix3& M) const noexcept;
		Matrix3& operator+= (const Matrix3& M) noexcept;
		Matrix3& operator-= (const Matrix3& M) noexcept;
		Matrix3& operator*= (const Matrix3& M) noexcept;
		Matrix3& operator*= (float S)          noexcept;
		Matrix3& operator/= (float S)          noexcept;
		Matrix3& operator/= (const Matrix3& M) noexcept;
		Matrix3 operator+ () const noexcept { return *this; }


		INLINE Scalar TDotX(const Vector3& v) const { return _matrix[0].GetX() * v.GetX() + _matrix[1].GetX() * v.GetY() + _matrix[2].GetX() * v.GetZ(); }
		INLINE Scalar TDotY(const Vector3& v) const { return _matrix[0].GetY() * v.GetX() + _matrix[1].GetY() * v.GetY() + _matrix[2].GetY() * v.GetZ(); }
		INLINE Scalar TDotZ(const Vector3& v) const { return _matrix[0].GetZ() * v.GetX() + _matrix[1].GetZ() * v.GetY() + _matrix[2].GetZ() * v.GetZ(); }
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		INLINE Matrix3() {}
		INLINE Matrix3(Vector3 x, Vector3 y, Vector3 z)     { _matrix[0] = x; _matrix[1] = y; _matrix[2] = z; }
		INLINE Matrix3(const Matrix3& m)                    { _matrix[0] = m._matrix[0]; _matrix[1] = m._matrix[1]; _matrix[2] = m._matrix[2]; }
		INLINE Matrix3(Quaternion q)                        { *this = Matrix3(DirectX::XMMatrixRotationQuaternion(q)); }
		INLINE explicit Matrix3(const DirectX::XMMATRIX& m) { _matrix[0] = Vector3(m.r[0]); _matrix[1] = Vector3(m.r[1]); _matrix[2] = Vector3(m.r[2]); }
		INLINE explicit Matrix3(EIdentityTag)               { _matrix[0] = Vector3(kXUnitVector); _matrix[1] = Vector3(kYUnitVector); _matrix[2] = Vector3(kZUnitVector); }
		INLINE explicit Matrix3(EZeroTag)                   { _matrix[0] = _matrix[1] = _matrix[2] = Vector3(kZero); }
	
		INLINE Matrix3& operator=(const Matrix3&) = default;
	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/

		/****************************************************************************
		**                Private Member Variables
		*****************************************************************************/
		Vector3 _matrix[3];
		
	};

	/****************************************************************************
	*				  			Matrix4
	*************************************************************************//**
	*  @class     Matrix4
	*  @brief     Matrix4X4 class (note! : 16 bytes (for 16 alignment))
	*****************************************************************************/
	__declspec(align(16))struct Matrix4
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		INLINE const Matrix3& Get3x3() const { return (const Matrix3&)*this; }
		INLINE void Set3x3(const Matrix3& xyz)
		{
			_matrix.r[0] = vector::SetWToZero(xyz.GetX());
			_matrix.r[1] = vector::SetWToZero(xyz.GetY());
			_matrix.r[2] = vector::SetWToZero(xyz.GetZ());
		}

		INLINE Vector4 GetX() const { return Vector4(_matrix.r[0]); }
		INLINE Vector4 GetY() const { return Vector4(_matrix.r[1]); }
		INLINE Vector4 GetZ() const { return Vector4(_matrix.r[2]); }
		INLINE Vector4 GetW() const { return Vector4(_matrix.r[3]); }

		INLINE void SetX(Vector4 x) { _matrix.r[0] = x; }
		INLINE void SetY(Vector4 y) { _matrix.r[1] = y; }
		INLINE void SetZ(Vector4 z) { _matrix.r[2] = z; }
		INLINE void SetW(Vector4 w) { _matrix.r[3] = w; }


		INLINE Float4x4 ToFloat4x4()
		{
			Float4x4 value;
			DirectX::XMStoreFloat4x4(&value, _matrix);
			return value;
		}


		INLINE Vector4 operator* (Vector3 vec) const        { return Vector4(DirectX::XMVector3Transform(vec, _matrix)); }
		INLINE Vector4 operator* (Vector4 vec) const        { return Vector4(DirectX::XMVector4Transform(vec, _matrix)); }
		INLINE Matrix4 operator* (const Matrix4& mat) const { return Matrix4(DirectX::XMMatrixMultiply(_matrix, mat)); }

		bool operator == (const Matrix4& M) const noexcept;
		bool operator != (const Matrix4& M) const noexcept;
		Matrix4& operator+= (const Matrix4& M) noexcept;
		Matrix4& operator-= (const Matrix4& M) noexcept;
		Matrix4& operator*= (const Matrix4& M) noexcept;
		Matrix4& operator*= (float S)          noexcept;
		Matrix4& operator/= (float S)          noexcept;
		Matrix4& operator/= (const Matrix4& M) noexcept;
		Matrix4 operator+ () const noexcept { return *this; }

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		INLINE Matrix4() {}
		INLINE Matrix4(Vector3 x, Vector3 y, Vector3 z, Vector3 w)
		{
			_matrix.r[0] = vector::SetWToZero(x); _matrix.r[1] = vector::SetWToZero(y);
			_matrix.r[2] = vector::SetWToZero(z); _matrix.r[3] = vector::SetWToOne(w);
		}
		INLINE Matrix4(const Float4x4& mat) { _matrix = DirectX::XMLoadFloat4x4(&mat); }
		INLINE Matrix4(const float* data){ _matrix = DirectX::XMLoadFloat4x4((DirectX::XMFLOAT4X4*)data);}
		INLINE Matrix4(Vector4 x, Vector4 y, Vector4 z, Vector4 w) { _matrix.r[0] = x; _matrix.r[1] = y; _matrix.r[2] = z; _matrix.r[3] = w; }
		INLINE Matrix4(const Matrix3& mat)
		{
			_matrix.r[0] = vector::SetWToZero(mat.GetX());
			_matrix.r[1] = vector::SetWToZero(mat.GetY());
			_matrix.r[2] = vector::SetWToZero(mat.GetZ());
			_matrix.r[3] = vector::CreateWUnitVector();    // 0,0,0,1
		}
		INLINE Matrix4(const Matrix3& xyz, Vector3 w)
		{
			_matrix.r[0] = vector::SetWToZero(xyz.GetX());
			_matrix.r[1] = vector::SetWToZero(xyz.GetY());
			_matrix.r[2] = vector::SetWToZero(xyz.GetZ());
			_matrix.r[3] = vector::SetWToOne(w);
		}

		INLINE explicit Matrix4(const DirectX::XMMATRIX& mat) { _matrix = mat; }
		INLINE explicit Matrix4(EIdentityTag)                 { _matrix = DirectX::XMMatrixIdentity(); }
		INLINE explicit Matrix4(EZeroTag)                     { _matrix.r[0] = _matrix.r[1] = _matrix.r[2] = _matrix.r[3] = vector::SplatZero(); }
		INLINE operator DirectX::XMMATRIX() const             { return _matrix; }


		INLINE Matrix4(const Matrix4&)            = default;
		INLINE Matrix4& operator=(const Matrix4&) = default;
		INLINE Matrix4(Matrix4&&)                 = default;
		INLINE Matrix4& operator=(Matrix4&&)      = default;
		
	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/

		/****************************************************************************
		**                Private Member Variables
		*****************************************************************************/
		DirectX::XMMATRIX _matrix;
	};

	INLINE Matrix3 HadamaldProduct(const Matrix3& a, const Matrix3& b)
	{
		Matrix3 result;
		result.SetX(a.GetX() * b.GetX());
		result.SetY(a.GetY() * b.GetY());
		result.SetZ(a.GetZ() * b.GetZ());
		return result;
	}
	INLINE Matrix3 Absolute(const Matrix3& mat) { return Matrix3(Abs(mat.GetX()), Abs(mat.GetY()), Abs(mat.GetZ())); }
	INLINE Matrix3 Transpose(const Matrix3& mat) { return Matrix3(DirectX::XMMatrixTranspose(mat)); }
	INLINE Matrix3 InverseMatrix3(const Matrix3& mat) { Matrix4 m(mat);  return Matrix4(DirectX::XMMatrixInverse(nullptr, m)).Get3x3(); }
	INLINE Matrix3 InverseTranspose(const Matrix3& mat)
	{
		const Vector3 x = mat.GetX();
		const Vector3 y = mat.GetY();
		const Vector3 z = mat.GetZ();

		const Vector3 inv0 = Cross(y, z);
		const Vector3 inv1 = Cross(z, x);
		const Vector3 inv2 = Cross(x, y);
		const Scalar  rDet = Recip(Dot(z, inv2));

		// Return the adjoint / determinant
		return Matrix3(inv0, inv1, inv2) * rDet;
	}
	INLINE Matrix3 Matrix3Identity() { return Matrix3(DirectX::XMMatrixIdentity()); }
	INLINE Matrix3 ScalingMatrix3(const Vector3& scale) { return Matrix3(DirectX::XMMatrixScalingFromVector(scale)); }
	INLINE Matrix3 RotationQuaternionMatrix3(const Quaternion& rotation) { return Matrix3(DirectX::XMMatrixRotationQuaternion(rotation)); }
	INLINE Vector3 Solve33(const Vector3& b, const Matrix3& matrix) 
	{
		Vector3 column1 = matrix.GetColumn(0);
		Vector3 column2 = matrix.GetColumn(1);
		Vector3 column3 = matrix.GetColumn(2);

		Scalar det = Dot(column1, Cross(column2, column3));
		if (Abs(det) > FLT_EPSILON) { det = 1.0f / det; }
		Vector3 x;
		x.SetX(det * Dot(b, Cross(column2, column3)));
		x.SetY(det * Dot(column1, Cross(b, column3)));
		x.SetZ(det * Dot(column1, Cross(column2, b)));
		return x;
	}
	INLINE void GetSkewSymmetricMatrix3(const Vector3& input, Matrix3& matrix)
	{
		matrix.SetX(Vector3(0.0f, -input.GetZ(), input.GetY()));
		matrix.SetY(Vector3(input.GetZ(), 0.0f, -input.GetX()));
		matrix.SetZ(Vector3(-input.GetY(), input.GetX(), 0.0f));
	}
	INLINE void GetSkewSymmetricMatrix3(const Vector3& input, Vector3* v0, Vector3* v1, Vector3* v2) { *v0 = Vector3(0.0f, -input.GetZ(), input.GetY()); *v1 = Vector3(input.GetZ(), 0.0f, -input.GetX()); *v2 = Vector3(-input.GetY(), input.GetX(), 0.0f); }

	INLINE Scalar  Determinant(const Matrix4& matrix) { return Scalar(DirectX::XMMatrixDeterminant(matrix)); }
	INLINE Matrix4 Transpose  (const Matrix4& matrix) { return Matrix4(DirectX::XMMatrixTranspose(matrix)); }
	INLINE Matrix4 Inverse    (const Matrix4& matrix) { return Matrix4(DirectX::XMMatrixInverse(nullptr, matrix)); }
	INLINE Matrix4 Inverse    (Vector4& determinant, const Matrix4& matrix) { return Matrix4(DirectX::XMMatrixInverse(determinant.VectorPtr(), matrix)); }
	INLINE Matrix4 OrthoInvert(const Matrix4& xform)
	{
		Matrix3 basis     = Transpose(xform.Get3x3());
		Vector3 translate = basis * -Vector3(xform.GetW());
		return Matrix4(basis, translate);
	}
	INLINE bool DecomposeSRT(const Matrix4& matrix, Vector3& scale, Quaternion& rotation, Vector3& translation)
	{
		DirectX::XMVECTOR s, r, t;
		if (!DirectX::XMMatrixDecompose(&s, &r, &t, matrix)) { return false; }
		scale       = Vector3(s);
		rotation    = Quaternion(r);
		translation = Vector3(t);
	}
	
	INLINE Matrix4  MatrixIdentity()   { return Matrix4(DirectX::XMMatrixIdentity()); }
	INLINE Float4x4 MatrixIdentityF()  { return Matrix4(DirectX::XMMatrixIdentity()).ToFloat4x4(); }
	INLINE Vector3 TransformVector3(Vector3& vector, Matrix4& matrix) { return Vector3(DirectX::XMVector3Transform(vector, matrix)); }
	INLINE Vector3 TransformCoordinateVector3(Vector3& vector, Matrix4& matrix) { return Vector3(DirectX::XMVector3TransformCoord(vector, matrix)); }
	INLINE Vector2 TransformNormal(const Vector2& v, const Matrix3& M) { return Vector2(DirectX::XMVector2TransformNormal(v, M)); }
	INLINE Vector3 TransformNormal(const Vector3& v, const Matrix4& M) { return Vector3(DirectX::XMVector3TransformNormal(v, M)); }
	INLINE Matrix4 Transform2D  (const Vector2& scaling, float rotation, const Vector2& translation, const Vector2& rotationOrigin = Vector2(0.0f, 0.0f))                 { return Matrix4(DirectX::XMMatrixAffineTransformation2D(scaling, rotationOrigin, rotation, translation)); }
	INLINE Matrix4 Transform3D  (const Vector3& scaling, const Quaternion& rotation, const Vector3& translation, const Vector3& rotationOrigin = Vector3(0.0f, 0.0f, 0.0f)) { return Matrix4(DirectX::XMMatrixAffineTransformation(scaling, rotationOrigin, rotation, translation)); }
	INLINE Matrix4 Transform3D  (const Matrix4& matrix, const Quaternion& rotation) { return matrix * Matrix4(DirectX::XMMatrixRotationQuaternion(rotation)); }
	INLINE Matrix4 Translation            (const Float3& position)              { return Matrix4(DirectX::XMMatrixTranslationFromVector(position)); }
	INLINE Matrix4 Translation            (const Vector3& position)             { return Matrix4(DirectX::XMMatrixTranslationFromVector(position)); }
	INLINE Matrix4 Translation            (float x, float y, float z)           { return Matrix4(DirectX::XMMatrixTranslation(x, y, z)); };
	INLINE Matrix4 Scaling                (float scale)                         { return Matrix4(DirectX::XMMatrixScaling(scale, scale, scale)); }
	INLINE Matrix4 Scaling                (const Float3& scale)                 { return Matrix4(DirectX::XMMatrixScalingFromVector(scale)); }
	INLINE Matrix4 Scaling                (const Vector3& scale)                { return Matrix4(DirectX::XMMatrixScalingFromVector(scale)); }
	INLINE Matrix4 Scaling                (float sx, float sy, float sz)        { return Matrix4(DirectX::XMMatrixScaling(sx, sy, sz)); }
	INLINE Matrix4 RotationX              (float radian)                        { return Matrix4(DirectX::XMMatrixRotationX(radian)); }
	INLINE Matrix4 RotationY              (float radian)                        { return Matrix4(DirectX::XMMatrixRotationY(radian)); }
	INLINE Matrix4 RotationZ              (float radian)                        { return Matrix4(DirectX::XMMatrixRotationZ(radian)); }
	INLINE Matrix4 RotationQuaternion     (const Quaternion& rotation)          { return Matrix4(DirectX::XMMatrixRotationQuaternion(rotation)); }
	INLINE Matrix4 RotationNormal         (const Vector3& normal, float radian) { return Matrix4(DirectX::XMMatrixRotationNormal(normal, radian)); }
	INLINE Matrix4 RotationNormal         (const Float3& normal, float radian)  { return Matrix4(DirectX::XMMatrixRotationNormal(normal, radian)); }
	INLINE Matrix4 RotationRollPitchYaw   (float roll, float pitch, float yaw)  { return Matrix4(DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll)); }
	INLINE Matrix4 RotationRollPitchYaw   (const Vector3& rollPitchYaw)         { return Matrix4(DirectX::XMMatrixRotationRollPitchYaw(rollPitchYaw.GetY(), rollPitchYaw.GetZ(), rollPitchYaw.GetX())); }
	INLINE Matrix4 RotationRollPitchYaw   (const Float3 & rollPitchYaw)         { Vector3 vector = Vector3(rollPitchYaw);  return Matrix4(DirectX::XMMatrixRotationRollPitchYaw(vector.GetY(), vector.GetZ(), vector.GetX())); }
	INLINE Matrix4 RotationAxis           (const Float3 & axis, float radian)                                                                   { return Matrix4(DirectX::XMMatrixRotationAxis(axis, radian)); }
	INLINE Matrix4 RotationAxis           (const Vector3& axis, float radian)                                                                   { return Matrix4(DirectX::XMMatrixRotationAxis(axis, radian)); }
	INLINE Matrix4 LookAtLH               (const Vector3& eye, const Vector3& target,    const Vector3& up)                                     { return Matrix4(DirectX::XMMatrixLookAtLH(eye, target, up)); }
	INLINE Matrix4 LookAtRH               (const Vector3& eye, const Vector3& target,    const Vector3& up)                                     { return Matrix4(DirectX::XMMatrixLookAtRH(eye, target, up)); }
	INLINE Matrix4 LookAtLH               (const Float3 & eye, const Float3 & target,    const Float3 & up)                                     { return Matrix4(DirectX::XMMatrixLookAtLH(eye, target, up)); }
	INLINE Matrix4 LookAtRH               (const Float3 & eye, const Float3 & target,    const Float3 & up)                                     { return Matrix4(DirectX::XMMatrixLookAtRH(eye, target, up)); }
	INLINE Matrix4 LookToLH               (const Vector3& eye, const Vector3& direction, const Vector3& up)                                     { return Matrix4(DirectX::XMMatrixLookToLH(eye, direction, up)); }
	INLINE Matrix4 LookToRH               (const Vector3& eye, const Vector3& direction, const Vector3& up)                                     { return Matrix4(DirectX::XMMatrixLookToRH(eye, direction, up)); }
	INLINE Matrix4 LookToLH               (const Float3 & eye, const Float3 & direction, const Float3 & up)                                     { return Matrix4(DirectX::XMMatrixLookToLH(eye, direction, up)); }
	INLINE Matrix4 LookToRH               (const Float3 & eye, const Float3 & direction, const Float3 & up)                                     { return Matrix4(DirectX::XMMatrixLookToRH(eye, direction, up)); }
	INLINE Matrix4 PerspectiveLH          (float width, float height, float nearPlane, float farPlane)                                          { return Matrix4(DirectX::XMMatrixPerspectiveLH(width, height, nearPlane, farPlane)); }
	INLINE Matrix4 PerspectiveRH          (float width, float height, float nearPlane, float farPlane)                                          { return Matrix4(DirectX::XMMatrixPerspectiveRH(width, height, nearPlane, farPlane)); }
	INLINE Matrix4 PerspectiveFovLH       (float fov, float aspectRatio, float nearPlane, float farPlane)                                       { return Matrix4(DirectX::XMMatrixPerspectiveFovLH(fov, aspectRatio, nearPlane, farPlane)); }
	INLINE Matrix4 PerspectiveFovRH       (float fov, float aspectRatio, float nearPlane, float farPlane)                                       { return Matrix4(DirectX::XMMatrixPerspectiveFovRH(fov, aspectRatio, nearPlane, farPlane)); }
	INLINE Matrix4 PerspectiveOffCenterLH (float left, float right, float bottom, float top, float nearPlane, float farPlane)                   { return Matrix4(DirectX::XMMatrixPerspectiveOffCenterLH(left, right, bottom, top, nearPlane, farPlane)); }
	INLINE Matrix4 PerspectiveOffCenterRH (float left, float right, float bottom, float top, float nearPlane, float farPlane)                   { return Matrix4(DirectX::XMMatrixPerspectiveOffCenterRH(left, right, bottom, top, nearPlane, farPlane)); }
	INLINE Matrix4 OrthographicLH         (float viewWidth, float viewHeight, float zNearPlane, float zFarPlane)                                { return Matrix4(DirectX::XMMatrixOrthographicLH(viewWidth, viewHeight, zNearPlane, zFarPlane)); }
	INLINE Matrix4 OrthographicRH         (float viewWidth, float viewHeight, float zNearPlane, float zFarPlane)                                { return Matrix4(DirectX::XMMatrixOrthographicRH(viewWidth, viewHeight, zNearPlane, zFarPlane)); }
	INLINE Matrix4 OrthographicOffCenterLH(float viewLeft, float viewRight, float viewBottom, float viewTop, float zNearPlane, float zFarPlane) { return Matrix4(DirectX::XMMatrixOrthographicOffCenterLH(viewLeft, viewRight, viewBottom, viewTop, zNearPlane, zFarPlane)); }
	INLINE Matrix4 OrthographicOffCenterRH(float viewLeft, float viewRight, float viewBottom, float viewTop, float zNearPlane, float zFarPlane) { return Matrix4(DirectX::XMMatrixOrthographicOffCenterRH(viewLeft, viewRight, viewBottom, viewTop, zNearPlane, zFarPlane)); }
	INLINE Matrix4 Shadow                 (const Vector3& lightDirection, const Vector4& shadowPlane)                                           { return Matrix4(DirectX::XMMatrixShadow(shadowPlane, lightDirection)); }
	INLINE Matrix4 Reflection             (const Vector4& reflectionPlane)                                                                      { return Matrix4(DirectX::XMMatrixReflect(reflectionPlane)); }
	INLINE bool Matrix4::operator == (const Matrix4& M) const noexcept
	{
		return (Vector4(_matrix.r[0]) == Vector4(M._matrix.r[0])
			&& Vector4(_matrix.r[1]) == Vector4(M._matrix.r[1])
			&& Vector4(_matrix.r[2]) == Vector4(M._matrix.r[2])
			&& Vector4(_matrix.r[3]) == Vector4(M._matrix.r[3]));
	}
	INLINE bool Matrix4::operator != (const Matrix4& M) const noexcept
	{
		return (Vector4(_matrix.r[0]) != Vector4(M._matrix.r[0])
			|| Vector4(_matrix.r[1]) != Vector4(M._matrix.r[1])
			|| Vector4(_matrix.r[2]) != Vector4(M._matrix.r[2])
			|| Vector4(_matrix.r[3]) != Vector4(M._matrix.r[3])) != 0;
	}
	INLINE Matrix4& Matrix4::operator+= (const Matrix4& M) noexcept
	{
		_matrix.r[0] = DirectX::XMVectorAdd(_matrix.r[0], M._matrix.r[0]);
		_matrix.r[1] = DirectX::XMVectorAdd(_matrix.r[1], M._matrix.r[1]);
		_matrix.r[2] = DirectX::XMVectorAdd(_matrix.r[2], M._matrix.r[2]);
		_matrix.r[3] = DirectX::XMVectorAdd(_matrix.r[3], M._matrix.r[3]);
		return *this;
	}
	INLINE Matrix4& Matrix4::operator-= (const Matrix4& M) noexcept
	{
		_matrix.r[0] = DirectX::XMVectorSubtract(_matrix.r[0], M._matrix.r[0]);
		_matrix.r[1] = DirectX::XMVectorSubtract(_matrix.r[1], M._matrix.r[1]);
		_matrix.r[2] = DirectX::XMVectorSubtract(_matrix.r[2], M._matrix.r[2]);
		_matrix.r[3] = DirectX::XMVectorSubtract(_matrix.r[3], M._matrix.r[3]);
		return *this;
	}
	INLINE Matrix4& Matrix4::operator*= (const Matrix4& M) noexcept
	{
		_matrix = DirectX::XMMatrixMultiply(_matrix, M);
		return *this;
	}
	INLINE Matrix4& Matrix4::operator*= (float S) noexcept
	{
		_matrix.r[0] = DirectX::XMVectorScale(_matrix.r[0], S);
		_matrix.r[1] = DirectX::XMVectorScale(_matrix.r[1], S);
		_matrix.r[2] = DirectX::XMVectorScale(_matrix.r[2], S);
		_matrix.r[3] = DirectX::XMVectorScale(_matrix.r[3], S);
		return *this;
	}
	INLINE Matrix4& Matrix4::operator/= (float S) noexcept
	{
		assert(S != 0.f);

		float rs = 1.0f / S;
		_matrix.r[0] = DirectX::XMVectorScale(_matrix.r[0], rs);
		_matrix.r[1] = DirectX::XMVectorScale(_matrix.r[1], rs);
		_matrix.r[2] = DirectX::XMVectorScale(_matrix.r[2], rs);
		_matrix.r[3] = DirectX::XMVectorScale(_matrix.r[3], rs);

		return *this;
	}
	INLINE Matrix4& Matrix4::operator/= (const Matrix4& M) noexcept
	{
		_matrix.r[0] = DirectX::XMVectorDivide(_matrix.r[0], M._matrix.r[0]);
		_matrix.r[1] = DirectX::XMVectorDivide(_matrix.r[1], M._matrix.r[1]);
		_matrix.r[2] = DirectX::XMVectorDivide(_matrix.r[2], M._matrix.r[2]);
		_matrix.r[3] = DirectX::XMVectorDivide(_matrix.r[3], M._matrix.r[3]);
		return *this;
	}
	INLINE Matrix4 operator+(const Matrix4& M1, const Matrix4& M2) noexcept
	{
		Matrix4 result;
		result.SetX(M1.GetX() + M2.GetX());
		result.SetY(M1.GetY() + M2.GetY());
		result.SetZ(M1.GetZ() + M2.GetZ());
		result.SetW(M1.GetW() + M2.GetW());
		return result;
	}
	INLINE Matrix4 operator-(const Matrix4& M1, const Matrix4& M2) noexcept
	{
		Matrix4 result;
		result.SetX(M1.GetX() - M2.GetX());
		result.SetY(M1.GetY() - M2.GetY());
		result.SetZ(M1.GetZ() - M2.GetZ());
		result.SetW(M1.GetW() - M2.GetW());
		return result;
	}
	
	INLINE Matrix4 operator* (const Matrix4& M, float S) noexcept
	{
		Matrix4 result;
		result.SetX(M.GetX() * S);
		result.SetY(M.GetY() * S);
		result.SetZ(M.GetZ() * S);
		result.SetW(M.GetW() * S);
		return result;
	}
	INLINE Matrix4 operator/ (const Matrix4& M, float S) noexcept
	{
		assert(S != 0.0f);
		float rs = 1.0f / S;

		Matrix4 result;
		result.SetX(M.GetX() * rs);
		result.SetY(M.GetY() * rs);
		result.SetZ(M.GetZ() * rs);
		result.SetW(M.GetW() * rs);
		return result;
	}
	INLINE Matrix4 operator/ (const Matrix4& M1, const Matrix4& M2) noexcept
	{
		Matrix4 result;
		result.SetX(M1.GetX() / M2.GetX());
		result.SetY(M1.GetY() / M2.GetY());
		result.SetZ(M1.GetZ() / M2.GetZ());
		result.SetW(M1.GetW() / M2.GetW());
		return result;
	}
	INLINE Matrix4 operator* (float S, const Matrix4& M) noexcept
	{
		Matrix4 result;
		result.SetX(M.GetX() * S);
		result.SetY(M.GetY() * S);
		result.SetZ(M.GetZ() * S);
		result.SetW(M.GetW() * S);
		return result;
	}

	INLINE bool Matrix3::operator == (const Matrix3& M) const noexcept
	{
		return ((Vector3)
			    _matrix[0] == M._matrix[0]
			&&  _matrix[1] == M._matrix[1]
			&&  _matrix[2] == M._matrix[2]);
	}
	INLINE bool Matrix3::operator != (const Matrix3& M) const noexcept
	{
		return ((Vector3)_matrix[0] != M._matrix[0]
			|| _matrix[1] != M._matrix[1]
			|| _matrix[2] != M._matrix[2]) != 0;
	}
	INLINE Matrix3& Matrix3::operator+= (const Matrix3& M) noexcept
	{
		_matrix[0] += M._matrix[0];
		_matrix[1] += M._matrix[1];
		_matrix[2] += M._matrix[2];
		return *this;
	}
	INLINE Matrix3& Matrix3::operator-= (const Matrix3& M) noexcept
	{
		_matrix[0] = _matrix[0] - M._matrix[0];
		_matrix[1] = _matrix[1] - M._matrix[1];
		_matrix[2] = _matrix[2] - M._matrix[2];
		return *this;
	}
	INLINE Matrix3& Matrix3::operator*= (const Matrix3& M) noexcept
	{
		Matrix4 mLeft(*this);
		Matrix4 mRight(M);
		Matrix4 result = mLeft * mRight;
		*this = result.Get3x3();
		return *this;
	}
	INLINE Matrix3& Matrix3::operator*= (float S) noexcept
	{
		_matrix[0] = _matrix[0] * S;
		_matrix[1] = _matrix[1] * S;
		_matrix[2] = _matrix[2] * S;
		return *this;
	}
	INLINE Matrix3& Matrix3::operator/= (float S) noexcept
	{
		assert(S != 0.f);

		float rs = 1.0f / S;
		_matrix[0] = _matrix[0] * rs;
		_matrix[1] = _matrix[1] * rs;
		_matrix[2] = _matrix[2] * rs;
		return *this;
	}
	INLINE Matrix3& Matrix3::operator/= (const Matrix3& M) noexcept
	{
		_matrix[0] = (_matrix[0] / M._matrix[0]);
		_matrix[1] = (_matrix[1] / M._matrix[1]);
		_matrix[2] = (_matrix[2] / M._matrix[2]);
		return *this;
	}
	INLINE Matrix3 operator+(const Matrix3& M1, const Matrix3& M2) noexcept
	{
		Matrix3 result;
		result.SetX(M1.GetX() + M2.GetX());
		result.SetY(M1.GetY() + M2.GetY());
		result.SetZ(M1.GetZ() + M2.GetZ());
		return result;
	}
	INLINE Matrix3 operator-(const Matrix3& M1, const Matrix3& M2) noexcept
	{
		Matrix3 result;
		result.SetX(M1.GetX() - M2.GetX());
		result.SetY(M1.GetY() - M2.GetY());
		result.SetZ(M1.GetZ() - M2.GetZ());
		return result;
	}

	INLINE Matrix3 operator* (const Matrix3& M, float S) noexcept
	{
		Matrix3 result;
		result.SetX(M.GetX() * S);
		result.SetY(M.GetY() * S);
		result.SetZ(M.GetZ() * S);
		return result;
	}
	INLINE Matrix3 operator/ (const Matrix3& M, float S) noexcept
	{
		assert(S != 0.0f);
		float rs = 1.0f / S;

		Matrix3 result;
		result.SetX(M.GetX() * rs);
		result.SetY(M.GetY() * rs);
		result.SetZ(M.GetZ() * rs);
		return result;
	}
	INLINE Matrix3 operator/ (const Matrix3& M1, const Matrix3& M2) noexcept
	{
		Matrix3 result;
		result.SetX(M1.GetX() / M2.GetX());
		result.SetY(M1.GetY() / M2.GetY());
		result.SetZ(M1.GetZ() / M2.GetZ());
		return result;
	}
	INLINE Matrix3 operator* (float S, const Matrix3& M) noexcept
	{
		Matrix3 result;
		result.SetX(M.GetX() * S);
		result.SetY(M.GetY() * S);
		result.SetZ(M.GetZ() * S);
		return result;
	}

	INLINE Vector3 operator* (const Vector3& v, const Matrix3& m)
	{
		return Vector3(m.TDotX(v), m.TDotY(v), m.TDotZ(v));
	}
	INLINE void Diagonalize(Matrix3& matrix, Matrix3& rotation, float threshold, int maxSteps)
	{
		rotation = Matrix3Identity();
		for (int step = maxSteps; step > 0; step--)
		{
			// find off-diagonal element [p][q] with largest magnitude
			int p = 0;
			int q = 1;
			int r = 2;
			float max = Abs(matrix.GetElement(0,1));
			float v   = Abs(matrix.GetElement(0,2));
			if (v > max)
			{
				q = 2;
				r = 1;
				max = v;
			}
			v = Abs(matrix.GetElement(1,2));
			if (v > max)
			{
				p = 1;
				q = 2;
				r = 0;
				max = v;
			}

			float t = threshold * (Abs(matrix.GetElement(0,0)) + Abs(matrix.GetElement(1,1)) + Abs(matrix.GetElement(2,2)));
			if (max <= t)
			{
				if (max <= FLT_MAX * t)
				{
					return;
				}
				step = 1;
			}

			// compute Jacobi rotation J which leads to a zero for element [p][q]
			float mpq    = matrix.GetElement(p,q);
			float theta  = (matrix.GetElement(q,q) - matrix.GetElement(p,p) / (2 * mpq));
			float theta2 = theta * theta;
			float cos;
			float sin;
			if (theta2 * theta2 < float(10 / FLT_EPSILON))
			{
				t = (theta >= 0) ? (float)1 / (theta + Sqrt(1 + theta2))
					: 1.0f / (theta - Sqrt(1 + theta2));
				cos = 1.0f / Sqrt(1 + t * t);
				sin = cos * t;
			}
			else
			{
				// approximation for large theta-value, i.e., a nearly diagonal matrix
				t = 1 / (theta * (2 + 0.5f / theta2));
				cos = 1 - 0.5f * t * t;
				sin = cos * t;
			}

			// apply rotation to matrix (this = J^T * this * J)
			matrix.SetElement(p, q, 0);
			matrix.SetElement(q,p ,0);
			matrix.SetElement(p,p, matrix.GetElement(p,p) -t * mpq);
			matrix.SetElement(q,q, matrix.GetElement(q,q) + t * mpq);
			float mrp = matrix.GetElement(r,p);
			float mrq = matrix.GetElement(r,q);
			matrix.SetElement(r, p, cos * mrp - sin * mrq);
			matrix.SetElement(p, r, cos * mrp - sin * mrq);
			matrix.SetElement(r, q, cos * mrq + sin * mrp);
			matrix.SetElement(q, r, cos * mrq + sin * mrp);

			// apply rotation to rot (rot = rot * J)
			for (int i = 0; i < 3; i++)
			{
				Vector3 row = rotation.GetRow(i);
				mrp = row.GetElement(p);
				mrq = row.GetElement(q);
				rotation.SetElement(i,p, cos * mrp - sin * mrq);
				rotation.SetElement(i,q, cos * mrq + sin * mrp);
			}
		}
	}

}
#endif
