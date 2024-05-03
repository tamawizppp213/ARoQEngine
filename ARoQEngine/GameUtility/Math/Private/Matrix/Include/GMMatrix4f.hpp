//////////////////////////////////////////////////////////////////////////////////
///             @file   GMMatrix3f.hpp
///             @brief  temp
///             @author toide
///             @date   2024/03/20 15:10:37
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GM_MATRIX4F_HPP
#define GM_MATRIX4F_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Math/Private/Simd/Include/GMSimdMacros.hpp"
#include "GameUtility/Math/Private/Vector/Include/GMVector2f.hpp"
#include "GameUtility/Math/Private/Vector/Include/GMVector3f.hpp"
#include "GameUtility/Math/Private/Vector/Include/GMVector4f.hpp"
#include "GameUtility/Math/Private/Quaternion/Include/GMQuaternionF.hpp"
#include SIMD_COMPILED_HEADER(GameUtility/Math/Private/Simd/Include, GMSimdMatrix128)

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
#if defined(PLATFORM_CPU_INSTRUCTION_NOT_USE)
	#define MATRIX128 gm::simd::non::Matrix128
#elif PLATFORM_CPU_INSTRUCTION_NEON
	#define MATRIX128 gm::simd::neon::Matrix128 
#else
	#define MATRIX128 gm::simd::sse::Matrix128
#endif

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace gm
{
	/****************************************************************************
	*				  			   GMVector4
	*************************************************************************//**
	*  @class     GMVector4
	*  @brief     SIMD演算用のVector4クラスです. アラインメントを行っているため, データを保持する場合はFloat4x4を使用してください
	*****************************************************************************/
	struct Float3x4
	{
	public:
		/****************************************************************************
		**                Public Property
		*****************************************************************************/
		union U
		{
			struct S
			{
				float _00, _01, _02, _03;
				float _10, _11, _12, _13;
				float _20, _21, _22, _23;
			} s;
			float m[3][4];
			float a[12];
		} u;

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		// @brief : Default constructor (単位行列)
		Float3x4()
		{
			u.s._00 = 1.0f; u.s._01 = 0.0f; u.s._02 = 0.0f; u.s._03 = 0.0f;
			u.s._10 = 0.0f; u.s._11 = 1.0f; u.s._12 = 0.0f; u.s._13 = 0.0f;
			u.s._20 = 0.0f; u.s._21 = 0.0f; u.s._22 = 1.0f; u.s._23 = 0.0f;
		}

		// @brief : 全ての要素で初期化
		constexpr Float3x4(const float m00, const float m01, const float m02, const float m03,
			const float m10, const float m11, const float m12, const float m13,
			const float m20, const float m21, const float m22, const float m23)
		{
			u.s._00 = m00; u.s._01 = m01; u.s._02 = m02; u.s._03 = m03;
			u.s._10 = m10; u.s._11 = m11; u.s._12 = m12; u.s._13 = m13;
			u.s._20 = m20; u.s._21 = m21; u.s._22 = m22; u.s._23 = m23;
		}

		// @brief : 配列を使って初期化
		explicit Float3x4(_In_reads_(12) const float* pArray)
		{
			Check(pArray);
			u.m[0][0] = pArray[0];
			u.m[0][1] = pArray[1];
			u.m[0][2] = pArray[2];
			u.m[0][3] = pArray[3];

			u.m[1][0] = pArray[4];
			u.m[1][1] = pArray[5];
			u.m[1][2] = pArray[6];
			u.m[1][3] = pArray[7];

			u.m[2][0] = pArray[8];
			u.m[2][1] = pArray[9];
			u.m[2][2] = pArray[10];
			u.m[2][3] = pArray[11];
		}

		// @brief : copy constructor
		Float3x4(const Float3x4&) = default;

		Float3x4& operator=(const Float3x4&) = default;

		// @brief : move constructor
		Float3x4(Float3x4&&) = default;

		Float3x4& operator=(Float3x4&&) = default;

		operator MATRIX128() const noexcept
		{
			return SIMD_NAME_SPACE::Matrix128Utility::LoadFloat4x4((float*)this);
		}
	};

	struct Float4x4
	{
	public:
		/****************************************************************************
		**                Public Property
		*****************************************************************************/
		union U
		{
			struct S
			{
				float _00, _01, _02, _03;
				float _10, _11, _12, _13;
				float _20, _21, _22, _23;
				float _30, _31, _32, _33;
			} s;
			float m[4][4];
			float a[16];
		} u;

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		// @brief : Default constructor (単位行列)
		Float4x4()
		{
			u.s._00 = 1.0f; u.s._01 = 0.0f; u.s._02 = 0.0f; u.s._03 = 0.0f;
			u.s._10 = 0.0f; u.s._11 = 1.0f; u.s._12 = 0.0f; u.s._13 = 0.0f;
			u.s._20 = 0.0f; u.s._21 = 0.0f; u.s._22 = 1.0f; u.s._23 = 0.0f;
			u.s._30 = 0.0f; u.s._31 = 0.0f; u.s._32 = 0.0f; u.s._33 = 1.0f;
		};

		// @brief : 全ての要素で初期化
		constexpr Float4x4(const float m00, const float m01, const float m02, const float m03,
			const float m10, const float m11, const float m12, const float m13,
			const float m20, const float m21, const float m22, const float m23,
			const float m30, const float m31, const float m32, const float m33)
		{
			u.s._00 = m00; u.s._01 = m01; u.s._02 = m02; u.s._03 = m03;
			u.s._10 = m10; u.s._11 = m11; u.s._12 = m12; u.s._13 = m13;
			u.s._20 = m20; u.s._21 = m21; u.s._22 = m22; u.s._23 = m23;
			u.s._30 = m30; u.s._31 = m31; u.s._32 = m32; u.s._33 = m33;
		}

		// @brief : 配列を使って初期化
		explicit Float4x4(_In_reads_(16) const float* pArray)
		{
			Check(pArray);
			u.m[0][0] = pArray[0];
			u.m[0][1] = pArray[1];
			u.m[0][2] = pArray[2];
			u.m[0][3] = pArray[3];

			u.m[1][0] = pArray[4];
			u.m[1][1] = pArray[5];
			u.m[1][2] = pArray[6];
			u.m[1][3] = pArray[7];

			u.m[2][0] = pArray[8];
			u.m[2][1] = pArray[9];
			u.m[2][2] = pArray[10];
			u.m[2][3] = pArray[11];

			u.m[3][0] = pArray[12];
			u.m[3][1] = pArray[13];
			u.m[3][2] = pArray[14];
			u.m[3][3] = pArray[15];
		}

		// @brief : copy constructor
		Float4x4(const Float4x4&) = default;

		Float4x4& operator=(const Float4x4&) = default;

		// @brief : move constructor
		Float4x4(Float4x4&&) = default;

		Float4x4& operator=(Float4x4&&) = default;

		operator MATRIX128() const noexcept
		{
			return SIMD_NAME_SPACE::Matrix128Utility::LoadFloat4x4((float*)this);
		}

		// @brief : 要素を( , )を使って二次元配列の要素に直接アクセス (参照渡しのため直接要素を書き換えられます)
		__forceinline float& operator ()(const gu::uint32 row, const gu::uint32 column)
		{
			return u.m[row][column];
		}

		// @brief : 要素を( , )を使って二次元配列の要素に直接アクセス(参照渡しのため直接要素を書き換えられます)
		__forceinline const float& operator()(const gu::uint32 row, const gu::uint32 column) const
		{
			return u.m[row][column];
		}
	};

	/****************************************************************************
	*				  			   Matrix4f
	*************************************************************************//**
	*  @class     Matrixf4f
	*  @brief     temp
	*****************************************************************************/
	struct Matrix4f
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/

		/****************************************************************************
		**                Public Property
		*****************************************************************************/
		#pragma region Getter
		/*----------------------------------------------------------------------
		*  @brief : 各要素を取得します (ただし、これは参照ではなくコピーなので注意)
		/*----------------------------------------------------------------------*/
		__forceinline Vector4f GetX() const { return _matrix.Row[0]; }
		__forceinline Vector4f GetY() const { return _matrix.Row[1]; }
		__forceinline Vector4f GetZ() const { return _matrix.Row[2]; }
		__forceinline Vector4f GetW() const { return _matrix.Row[3]; }
		__forceinline Vector4f GetRow(const gu::uint32 index) const { Check(index < 4); return _matrix.Row[index]; }

		/*----------------------------------------------------------------------
		*  @brief : 4x4行列を取得
		/*----------------------------------------------------------------------*/
		__forceinline Float4x4 ToFloat4x4() const
		{
			Float4x4 result = {};
			SIMD_NAME_SPACE::Matrix128Utility::StoreFloat4x4(result.u.a, _matrix);
			return result;
		}
		#pragma endregion Getter

		#pragma region Setter
		/*----------------------------------------------------------------------
		*  @brief : 各行を設定します
		/*----------------------------------------------------------------------*/
		__forceinline void SetX(const Vector4f& x) { _matrix.Row[0] = x; }
		__forceinline void SetY(const Vector4f& y) { _matrix.Row[1] = y; }
		__forceinline void SetZ(const Vector4f& z) { _matrix.Row[2] = z; }
		__forceinline void SetW(const Vector4f& w) { _matrix.Row[3] = w; }
		__forceinline void SetRow(const gu::uint32 index, const Vector4f& vector) { Check(index < 4); _matrix.Row[index] = vector; }
		#pragma endregion Setter

		#pragma region Operator Function
		// @brief : 加算
		__forceinline Matrix4f operator+ (const Matrix4f& right) const { return Matrix4f(SIMD_NAME_SPACE::Matrix128Utility::Add(_matrix, right._matrix)); }
		
		// @brief : 減算
		__forceinline Matrix4f operator- (const Matrix4f& right) const { return Matrix4f(SIMD_NAME_SPACE::Matrix128Utility::Subtract(_matrix, right._matrix)); }

		// @brief : 乗算
		__forceinline Matrix4f operator* (const Matrix4f& right) const { return Matrix4f(SIMD_NAME_SPACE::Matrix128Utility::Multiply(_matrix, right._matrix)); }

		// 除算は定義しない

		// @brief : 加算
		__forceinline Matrix4f& operator +=(const Matrix4f& right) noexcept 
		{
			_matrix = SIMD_NAME_SPACE::Matrix128Utility::Add(_matrix, right._matrix);
			return *this;
		}

		// @brief : 減算
		__forceinline Matrix4f& operator -=(const Matrix4f& right) noexcept
		{
			_matrix = SIMD_NAME_SPACE::Matrix128Utility::Subtract(_matrix, right._matrix);
			return *this;
		}

		// @brief : 乗算 (元々の行列は左から掛けます)
		__forceinline Matrix4f& operator *=(const Matrix4f& right) noexcept
		{
			_matrix = SIMD_NAME_SPACE::Matrix128Utility::Multiply(_matrix, right._matrix);
			return *this;
		}

		// @brief : スカラー倍
		__forceinline Matrix4f operator* (const float right) noexcept
		{
			_matrix.Row[0] = SIMD_NAME_SPACE::Vector128Utility::Scale(_matrix.Row[0], right);
			_matrix.Row[1] = SIMD_NAME_SPACE::Vector128Utility::Scale(_matrix.Row[1], right);
			_matrix.Row[2] = SIMD_NAME_SPACE::Vector128Utility::Scale(_matrix.Row[2], right);
			_matrix.Row[3] = SIMD_NAME_SPACE::Vector128Utility::Scale(_matrix.Row[3], right);
			return Matrix4f(_matrix);
		}

		__forceinline friend Matrix4f operator* (const float left, const Matrix4f& right)
		{
			Matrix4f result(right._matrix);
			
			result._matrix.Row[0] = SIMD_NAME_SPACE::Vector128Utility::Scale(right._matrix.Row[0], left);
			result._matrix.Row[1] = SIMD_NAME_SPACE::Vector128Utility::Scale(right._matrix.Row[1], left);
			result._matrix.Row[2] = SIMD_NAME_SPACE::Vector128Utility::Scale(right._matrix.Row[2], left);
			result._matrix.Row[3] = SIMD_NAME_SPACE::Vector128Utility::Scale(right._matrix.Row[3], left);
			return result;
		}

		__forceinline bool operator ==(const Matrix4f& right) const noexcept 
		{
			return  SIMD_NAME_SPACE::Vector128Utility::EqualVector4(_matrix.Row[0], right._matrix.Row[0])
				 && SIMD_NAME_SPACE::Vector128Utility::EqualVector4(_matrix.Row[1], right._matrix.Row[1])
				 && SIMD_NAME_SPACE::Vector128Utility::EqualVector4(_matrix.Row[2], right._matrix.Row[2])
				 && SIMD_NAME_SPACE::Vector128Utility::EqualVector4(_matrix.Row[3], right._matrix.Row[3]);
		}

		__forceinline bool operator !=(const Matrix4f& right) const noexcept
		{
			return  SIMD_NAME_SPACE::Vector128Utility::NotEqualVector4(_matrix.Row[0], right._matrix.Row[0])
				 || SIMD_NAME_SPACE::Vector128Utility::NotEqualVector4(_matrix.Row[1], right._matrix.Row[1])
				 || SIMD_NAME_SPACE::Vector128Utility::NotEqualVector4(_matrix.Row[2], right._matrix.Row[2])
				 || SIMD_NAME_SPACE::Vector128Utility::NotEqualVector4(_matrix.Row[3], right._matrix.Row[3]);
		}

		// @brief : 要素を( , )を使って二次元配列の要素に直接アクセス (参照渡しのため直接要素を書き換えられます)
		__forceinline float& operator ()(const gu::uint32 row, const gu::uint32 column)
		{
			return _matrix.Row[row].m128_f32[column];
		}

		// @brief : 要素を( , )を使って二次元配列の要素に直接アクセス(参照渡しのため直接要素を書き換えられます)
		__forceinline const float& operator()(const gu::uint32 row, const gu::uint32 column) const
		{
			return _matrix.Row[row].m128_f32[column];
		}

		#pragma endregion Operator Function

		#pragma region Math
		/*----------------------------------------------------------------------
		*  @brief : 転置
		/*----------------------------------------------------------------------*/
		__forceinline Matrix4f SIMD_CALL_CONVENTION Transpose() const
		{
			return SIMD_NAME_SPACE::Matrix128Utility::Transpose(_matrix);
		}

		/*----------------------------------------------------------------------
		*  @brief : 逆行列 
		            float* 行列式
		/*----------------------------------------------------------------------*/
		__forceinline Matrix4f SIMD_CALL_CONVENTION Inverse(float* determine) const
		{
			return SIMD_NAME_SPACE::Matrix128Utility::Inverse(_matrix, determine);
		}

		/*----------------------------------------------------------------------
		*  @brief : 逆行列
		/*----------------------------------------------------------------------*/
		__forceinline float SIMD_CALL_CONVENTION Determinant() const
		{
			return SIMD_NAME_SPACE::Matrix128Utility::Determinant(_matrix);
		}

		/*----------------------------------------------------------------------
		*  @brief : 対象の行列をscale, rotationのquoternion, translation(並進)に分割します.
		/*----------------------------------------------------------------------*/
		/*__forceinline bool SIMD_CALL_CONVENTION DecomposeSRT(Vector3f* scale, QuaternionF* quaternion, Vector3f* translation) noexcept
		{
			return SIMD_NAME_SPACE::Matrix128Utility::DecomposeSRT(_matrix, &scale->operator gm::simd::sse::Vector128(), quaternion, &);
		}*/

		__forceinline operator MATRIX128() const { return _matrix; }
		#pragma endregion Math

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		// @brief : Default constructor (デフォルトは単位行列)
		Matrix4f() { _matrix = SIMD_NAME_SPACE::Matrix128Utility::Identity(); }

		// @brief : Vector4f型を使って生成するコンストラクタ
		Matrix4f(const Vector4f& r0, const Vector4f& r1, const Vector4f& r2, const Vector4f& r3)
		{
			_matrix.Row[0] = r0;  _matrix.Row[1] = r1;
			_matrix.Row[2] = r2;  _matrix.Row[3] = r3;
		}

		// @brief : float値を直接使って行列を設定する
		Matrix4f
		(
			const float m00, const float m01, const float m02, const float m03,
			const float m10, const float m11, const float m12, const float m13,
			const float m20, const float m21, const float m22, const float m23,
			const float m30, const float m31, const float m32, const float m33
		) noexcept
		{
			_matrix = SIMD_NAME_SPACE::Matrix128Utility::Set
			(
				m00, m01, m02, m03,
				m10, m11, m12, m13,
				m20, m21, m22, m23,
				m30, m31, m32, m33
			);
		}

		// @brief : MATRIX128を直接代入するコンストラクタ
		Matrix4f(const MATRIX128& matrix) : _matrix(matrix) {};

		// @brief : Copy constructor
		Matrix4f(const Matrix4f&)            = default;
		Matrix4f& operator=(const Matrix4f&) = default;

		// @brief : Move constructor
		Matrix4f(Matrix4f&&)            = default;
		Matrix4f& operator=(Matrix4f&&) = default;

	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/

		/****************************************************************************
		**                Private Property
		*****************************************************************************/
		MATRIX128 _matrix;
	};
	
	/*----------------------------------------------------------------------
	*  @brief : 転置
	/*----------------------------------------------------------------------*/
	__forceinline MATRIX128 Transpose(const Matrix4f& matrix) noexcept
	{
		return SIMD_NAME_SPACE::Matrix128Utility::Transpose(matrix);
	}

	/*----------------------------------------------------------------------
	*  @brief : 逆行列
	*           const Matrix4f& matrix
	*           float* 行列式
	/*----------------------------------------------------------------------*/
	__forceinline MATRIX128 Inverse(const Matrix4f& matrix, float* determinant = nullptr) noexcept
	{
		return SIMD_NAME_SPACE::Matrix128Utility::Inverse(matrix, determinant);
	}

	/*----------------------------------------------------------------------
	*  @brief : 逆行列
	/*----------------------------------------------------------------------*/
	__forceinline float Determinant(const Matrix4f& matrix) noexcept
	{
		return SIMD_NAME_SPACE::Matrix128Utility::Determinant(matrix);
	}

	/*----------------------------------------------------------------------
	*  @brief : 並進移動用の行列を作成します.
	/*----------------------------------------------------------------------*/
	__forceinline Matrix4f Translation(const float offsetX, const float offsetY, const float offsetZ) noexcept
	{
		return SIMD_NAME_SPACE::Matrix128Utility::Translation(offsetX, offsetY, offsetZ);
	}

	__forceinline Matrix4f Translation(const Vector3f& offset) noexcept
	{
		return SIMD_NAME_SPACE::Matrix128Utility::Translation(offset);
	}

	__forceinline Matrix4f Translation(const Float3& offset) noexcept
	{
		return SIMD_NAME_SPACE::Matrix128Utility::Translation(offset);
	}

	/*----------------------------------------------------------------------
	*  @brief : スケーリング用の行列を作成します.
	/*----------------------------------------------------------------------*/
	__forceinline Matrix4f Scaling(const float scaleX, const float scaleY, const float scaleZ) noexcept
	{
		return SIMD_NAME_SPACE::Matrix128Utility::Scaling(scaleX, scaleY, scaleZ);
	}

	__forceinline Matrix4f Scaling(const Vector3f& scale) noexcept
	{
		return SIMD_NAME_SPACE::Matrix128Utility::Scaling(scale);
	}

	__forceinline Matrix4f Scaling(const Float3& scale) noexcept
	{
		return SIMD_NAME_SPACE::Matrix128Utility::Scaling(scale);
	}

	/*----------------------------------------------------------------------
	*  @brief : X, (Y, Z)軸それぞれに対してrad単位で角度を回転します
	/*----------------------------------------------------------------------*/
	__forceinline Matrix4f RotationX(const float radian) noexcept
	{
		return SIMD_NAME_SPACE::Matrix128Utility::RotationX(radian);
	}

	__forceinline Matrix4f RotationY(const float radian) noexcept
	{
		return SIMD_NAME_SPACE::Matrix128Utility::RotationY(radian);
	}

	__forceinline Matrix4f RotationZ(const float radian) noexcept
	{
		return SIMD_NAME_SPACE::Matrix128Utility::RotationZ(radian);
	}

	/*----------------------------------------------------------------------
	*  @brief : ロー(y axis)、ピッチ(x-axis)、ヨー(z-axis)を使ってrad単位で角度を回転します
	/*----------------------------------------------------------------------*/
	__forceinline Matrix4f RotationRollPitchYawMatrix(const float roll, const float pitch, const float yaw) noexcept
	{
		return SIMD_NAME_SPACE::Matrix128Utility::RotationRollPitchYaw(roll, pitch, yaw);
	}

	__forceinline Matrix4f RotationRollPitchYawMatrix(const Vector3f& rollPitchYaw) noexcept
	{
		return SIMD_NAME_SPACE::Matrix128Utility::RotationRollPitchYaw(rollPitchYaw);
	}

	__forceinline Matrix4f RotationRollPitchYawMatrix(const Float3& rollPitchYaw) noexcept
	{
		return SIMD_NAME_SPACE::Matrix128Utility::RotationRollPitchYaw(rollPitchYaw);
	}

	/*----------------------------------------------------------------------
	*  @brief : 法線ベクトルを中心にrad単位で角度を回転します
	/*----------------------------------------------------------------------*/
	__forceinline Matrix4f RotationNormalMatrix(const Vector3f& normal, const float radian) noexcept
	{
		return SIMD_NAME_SPACE::Matrix128Utility::RotationNormal(normal, radian);
	}

	__forceinline Matrix4f RotationNormalMatrix(const Float3& normal, const float radian) noexcept
	{
		return SIMD_NAME_SPACE::Matrix128Utility::RotationNormal(normal, radian);
	}

	/*----------------------------------------------------------------------
	*  @brief : ある軸を中心にrad単位で角度を回転します
	/*----------------------------------------------------------------------*/
	__forceinline Matrix4f RotationAxisMatrix(const Vector3f& axis, const float radian) noexcept
	{
		return SIMD_NAME_SPACE::Matrix128Utility::RotationAxis(axis, radian);
	}

	__forceinline Matrix4f RotationAxisMatrix(const Float3& axis, const float radian) noexcept
	{
		return SIMD_NAME_SPACE::Matrix128Utility::RotationAxis(axis, radian);
	}

	/*----------------------------------------------------------------------
	*  @brief : クォータニオンを使って回転
	/*----------------------------------------------------------------------*/
	__forceinline Matrix4f RotationQuaternion(const QuaternionF& q) noexcept
	{
		return SIMD_NAME_SPACE::Matrix128Utility::RotationQuaternion(q);
	}

	/*----------------------------------------------------------------------
		*  @brief : FocusPositionに視点を向けるための変換行列を作成します(左手系, 右手系)
		/*----------------------------------------------------------------------*/
	__forceinline Matrix4f LookAtLH(const Vector3f& eyePosition, const Vector3f& focusPosition, const Vector3f& worldUp) noexcept
	{
		return SIMD_NAME_SPACE::Matrix128Utility::LookAtLH(eyePosition, focusPosition, worldUp);
	}
	__forceinline Matrix4f LookAtRH(const Vector3f& eyePosition, const Vector3f& focusPosition, const Vector3f& worldUp) noexcept
	{
		return SIMD_NAME_SPACE::Matrix128Utility::LookAtRH(eyePosition, focusPosition, worldUp);
	}

	/*----------------------------------------------------------------------
	*  @brief : 視点を対象方向に向けるための変換行列を作成します(左手系, 右手系)
	/*----------------------------------------------------------------------*/
	__forceinline Matrix4f LookToLH(const Vector3f& eyePosition, const Vector3f& eyeDirection, const Vector3f& worldUp) noexcept
	{
		return SIMD_NAME_SPACE::Matrix128Utility::LookToLH(eyePosition, eyeDirection, worldUp);
	}

	__forceinline Matrix4f LookToRH(const Vector3f& eyePosition, const Vector3f& eyeDirection, const Vector3f& worldUp) noexcept
	{
		return SIMD_NAME_SPACE::Matrix128Utility::LookToRH(eyePosition, eyeDirection, worldUp);
	}

	/*----------------------------------------------------------------------
	*  @brief : 透視投影を行うための変換行列を作成します (左手系, 右手系)
	/*----------------------------------------------------------------------*/
	__forceinline Matrix4f PerspectiveLH(const float viewWidth, const float viewHeight, const float nearZ, const float farZ) noexcept
	{
		return SIMD_NAME_SPACE::Matrix128Utility::PerspectiveLH(viewWidth, viewHeight, nearZ, farZ);
	}

	__forceinline Matrix4f PerspectiveRH(const float viewWidth, const float viewHeight, const float nearZ, const float farZ) noexcept
	{
		return SIMD_NAME_SPACE::Matrix128Utility::PerspectiveRH(viewWidth, viewHeight, nearZ, farZ);
	}

	/*----------------------------------------------------------------------
	*  @brief : 透視投影を視野角を使って変換する行列を作成します (左手系, 右手系)
	/*----------------------------------------------------------------------*/
	__forceinline Matrix4f PerspectiveFovLH(const float fovAngleY, const float aspectRatio, const float nearZ, const float farZ) noexcept
	{
		return SIMD_NAME_SPACE::Matrix128Utility::PerspectiveFovLH(fovAngleY, aspectRatio, nearZ, farZ);
	}

	__forceinline Matrix4f PerspectiveFovRH(const float fovAngleY, const float aspectRatio, const float nearZ, const float farZ) noexcept
	{
		return SIMD_NAME_SPACE::Matrix128Utility::PerspectiveFovRH(fovAngleY, aspectRatio, nearZ, farZ);
	}

	/*----------------------------------------------------------------------
	*  @brief : 平行投影を行うための変換行列を作成します (左手系, 右手系)
	/*----------------------------------------------------------------------*/
	__forceinline Matrix4f OrthographicLH(const float viewWidth, const float viewHeight, const float nearZ, const float farZ) noexcept
	{
		return SIMD_NAME_SPACE::Matrix128Utility::OrthographicLH(viewWidth, viewHeight, nearZ, farZ);
	}

	__forceinline Matrix4f OrthographicRH(const float viewWidth, const float viewHeight, const float nearZ, const float farZ) noexcept
	{
		return SIMD_NAME_SPACE::Matrix128Utility::OrthographicRH(viewWidth, viewHeight, nearZ, farZ);
	}

	/*----------------------------------------------------------------------
	*  @brief : 平行投影を行うための変換行列を作成します (左手系, 右手系)
	/*----------------------------------------------------------------------*/
	__forceinline Vector3f TransformNormalVector3(const Matrix4f& matrix, const Vector3f vector) noexcept
	{
		return SIMD_NAME_SPACE::Matrix128Utility::TransformNormalVector3(matrix, vector);
	}
}

#endif