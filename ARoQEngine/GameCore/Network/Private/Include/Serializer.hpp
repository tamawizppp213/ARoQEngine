//////////////////////////////////////////////////////////////////////////////////
///             @file   Serializer.hpp
///             @brief  Serialize
///             @author Toide Yutaro
///             @date   2022_12_05
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef SERIALIZER_HPP
#define SERIALIZER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "MemoryStream.hpp"
#include "NetworkDefine.hpp"
#include <string>
#include <stdint.h>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace gc
{
	
	/****************************************************************************
	*				  			 Serializer
	*************************************************************************//**
	*  @class     Serializer
	*  @brief     serialize data 
	*****************************************************************************/
	class Serializer
	{

	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/* @brief : Serialize T -> byte length*/
		template<typename T> requires std::is_integral_v<T> || std::is_floating_point_v<T>
		void Serialize(const T element);

		/* @brief : Serialize byte vector -> write stream buffer*/
		void Serialize(const std::vector<std::uint8_t>& byte);

		/* @brief : Deserialize*/
		template<typename T> T Deserialize();

		/* @brief : Deserialize std::string -> byteLenght*/
		std::string Deserialize(const std::uint64_t byteLength);

		/* Clear memory stream*/
		void Clear();
		/****************************************************************************
		**                Public Property
		*****************************************************************************/
		Endian GetEndian() const { return _endian; }
		
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		Serializer();

		virtual ~Serializer();
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		
		/****************************************************************************
		**                Protected Property
		*****************************************************************************/
		
	private:
		/****************************************************************************
		**                Private Property
		*****************************************************************************/
		MemoryStream _stream;

		Endian _endian = Endian::BigEndian;
	};

	/****************************************************************************
	*                      Serialize
	*************************************************************************//**
	*  @fn        template <typename T> requires std::is_integral_v<T> || std::is_floating_point_v<T>
	              void Serializer::Serialize(const T element) 
	*
	*  @brief     Serialize (T type -> std::vector<std::uint8_t>)
	*
	*  @param[in] cosnt T element
	*
	*  @return    void
	*****************************************************************************/
	template <typename T> requires std::is_integral_v<T> || std::is_floating_point_v<T>
	void Serializer::Serialize(const T element) 
	{
		const std::vector<std::uint8_t> data = BitConverter::GetBytes(element);
		_stream.Append(buffer);
	}

	/****************************************************************************
	*                      Deserialize
	*************************************************************************//**
	*  @fn        template <typename T> T Serializer::Deserialize()
	*
	*  @brief     Deserialize (Read) value
	*
	*  @param[in] void
	*
	*  @return    void
	*****************************************************************************/
	template <typename T> T Serializer::Deserialize()
	{
		const auto byteSize = sizeof(T);
		const auto data     = _stream.Read(byteSize);
		
		return BitConverter::GetValue<T>(data, 0);
	}
}
#endif