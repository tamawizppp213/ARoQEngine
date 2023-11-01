//////////////////////////////////////////////////////////////////////////////////
///             @file   GUUUID.hpp
///             @brief  Universally Unique Identifier
///                     全てのResource中辛一いに識別するために使用されるラベル
///             @author toide
///             @date   2023/10/28 23:21:04
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GUUUID_HPP
#define GUUUID_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../../Math/Include/GMTypeDef.hpp"
#include <string>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace gu
{
	enum class UUIDFormat
	{
		Default // 00000000-00000000-00000000-00000000
	};

	/****************************************************************************
	*				  			   UUID
	*************************************************************************//**
	*  @struct    UUID
	*  @brief     オブジェクトを一意に特定するUniversally Unique Identifier (UUID)
	*****************************************************************************/
	struct UUID
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		static UUID Create();

		// @brief : GUIDを無効化します. (ゼロに初期化する)
		inline void Invalidate() { A = B = C = D = 0; }

		// @brief : GUIDが有効かどうかを調べます. 
		inline bool IsValid() const { return ((A | B | C | D) != 0); }

		// @brief : 文字列を後に追加します.
		void AppendString(std::string& string, const UUIDFormat format);

		// @brief : 文字列を返します
		std::string ToString(const UUIDFormat format);
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		gm::uint32 A = 0; // First component
		gm::uint32 B = 0; // Second component
		gm::uint32 C = 0; // Third component
		gm::uint32 D = 0; // Fourth component

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		UUID() = default;

		UUID(const gm::uint32 a, const gm::uint32 b, const gm::uint32 c, const gm::uint32 d)
			: A(a), B(b), C(c), D(d){ }

		bool operator ==(const UUID& rhs) const noexcept
		{
			return ((A ^ rhs.A) | (B ^ rhs.B) | (C ^ rhs.C) | (D ^ rhs.D)) == 0;
		}
		bool operator !=(const UUID& rhs) const noexcept
		{
			return ((A ^ rhs.A) | (B ^ rhs.B) | (C ^ rhs.C) | (D ^ rhs.D)) != 0;
		}

		// GUIDのコンポーネントにIndexから直接アクセスできるようにする.
		gm::uint32& operator[](const gm::uint32 index);
		const gm::uint32& operator[](const gm::uint32 index) const;
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
	};

}
#endif