//////////////////////////////////////////////////////////////////////////////////
///             @file   DirectX12VertexTypes.hpp
///             @brief  DirectX12 Vertex Types
///             @author Toide Yutaro (Reference DirectXTK)
///             @date   2020_11_19
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef DIRECTX12_VERTEX_TYPES_HPP
#define DIRECTX12_VERTEX_TYPES_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Math/Include/GMVector.hpp"
#include <d3d12.h>

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                              Struct
//////////////////////////////////////////////////////////////////////////////////
/****************************************************************************
*							 VertexPosition
*************************************************************************//**
*  @struct    VertexPosition
*  @brief     Vertex struct holding position information.
*****************************************************************************/
struct VertexPosition
{
public:
	gm::Float3 Position; /// Position

	static const D3D12_INPUT_LAYOUT_DESC InputLayout; /// Input Layout Descriptor

	VertexPosition() = default;
	VertexPosition(const VertexPosition&) = default;
	VertexPosition& operator=(const VertexPosition&) = default;
	VertexPosition(VertexPosition&&) = default;
	VertexPosition& operator=(VertexPosition&&) = default;
	VertexPosition(DirectX::XMFLOAT3 const& position) : Position(position) {};
	VertexPosition(DirectX::FXMVECTOR& position) { DirectX::XMStoreFloat3(&this->Position, position); }

private:
	static constexpr unsigned int InputElementCount = 1;
	static const D3D12_INPUT_ELEMENT_DESC InputElements[InputElementCount];
};

/****************************************************************************
*							 VertexPositionColor
*************************************************************************//**
*  @struct    VertexPositionColor
*  @brief     Vertex struct holding position and color information.
*****************************************************************************/
struct VertexPositionColor
{
public:
	gm::Float3 Position; /// Position
	gm::Float4 Color;    /// Color

	static const D3D12_INPUT_LAYOUT_DESC InputLayout;

	VertexPositionColor() = default;
	VertexPositionColor(const VertexPositionColor&) = default;
	VertexPositionColor& operator=(const VertexPositionColor&) = default;
	VertexPositionColor(VertexPositionColor&&) = default;
	VertexPositionColor& operator=(VertexPositionColor&&) = default;
	VertexPositionColor(DirectX::XMFLOAT3 const& position, DirectX::XMFLOAT4 const& color)
		: Position(position), Color(color)
	{
	};
	VertexPositionColor(DirectX::FXMVECTOR const& position, DirectX::FXMVECTOR const& color)
	{
		DirectX::XMStoreFloat3(&this->Position, position);
		DirectX::XMStoreFloat4(&this->Color, color);
	}

private:
	static constexpr unsigned int InputElementCount = 2;
	static const D3D12_INPUT_ELEMENT_DESC InputElements[InputElementCount];
};

/****************************************************************************
*							 VertexPositionTexture
*************************************************************************//**
*  @struct    VertexPositionTexture
*  @brief     Vertex struct holding position and uv information.
*****************************************************************************/
struct VertexPositionTexture
{
public:
	gm::Float3 Position; /// Position
	gm::Float2 UV;       /// UV

	static const D3D12_INPUT_LAYOUT_DESC InputLayout;

	VertexPositionTexture() = default;
	VertexPositionTexture(const VertexPositionTexture&) = default;
	VertexPositionTexture& operator=(const VertexPositionTexture&) = default;
	VertexPositionTexture(VertexPositionTexture&&) = default;
	VertexPositionTexture& operator=(VertexPositionTexture&&) = default;
	VertexPositionTexture(DirectX::XMFLOAT3 const& position, DirectX::XMFLOAT2 const& uv)
		: Position(position), UV(uv)
	{
	};
	VertexPositionTexture(DirectX::FXMVECTOR position, DirectX::FXMVECTOR uv)
	{
		DirectX::XMStoreFloat3(&this->Position, position);
		DirectX::XMStoreFloat2(&this->UV, uv);
	}

private:
	static constexpr unsigned int InputElementCount = 2;
	static const D3D12_INPUT_ELEMENT_DESC InputElements[InputElementCount];
};

/****************************************************************************
*					    VertexPositionDualTexture
*************************************************************************//**
*  @struct    VertexPositionDualTexture
*  @brief     Vertex struct holding position and UV (Dual) information.
*****************************************************************************/
struct VertexPositionDualTexture
{
public:
	gm::Float3 Position; /// Position
	gm::Float2 UV1;      /// UV1
	gm::Float2 UV2;      /// UV2

	static const D3D12_INPUT_LAYOUT_DESC InputLayout;

	VertexPositionDualTexture() = default;
	VertexPositionDualTexture(const VertexPositionDualTexture&) = default;
	VertexPositionDualTexture& operator=(const VertexPositionDualTexture&) = default;
	VertexPositionDualTexture(VertexPositionDualTexture&&) = default;
	VertexPositionDualTexture& operator=(VertexPositionDualTexture&&) = default;
	VertexPositionDualTexture(DirectX::XMFLOAT3 const& position, DirectX::XMFLOAT2 const& uv1, DirectX::XMFLOAT2 const& uv2)
		: Position(position), UV1(uv1), UV2(uv2)
	{
	};
	VertexPositionDualTexture(DirectX::FXMVECTOR position, DirectX::FXMVECTOR uv1, DirectX::FXMVECTOR uv2)
	{
		DirectX::XMStoreFloat3(&this->Position, position);
		DirectX::XMStoreFloat2(&this->UV1, uv1);
		DirectX::XMStoreFloat2(&this->UV2, uv2);
	}

private:
	static constexpr unsigned int InputElementCount = 3;
	static const D3D12_INPUT_ELEMENT_DESC InputElements[InputElementCount];
};

/****************************************************************************
*							 VertexPositionNormal
*************************************************************************//**
*  @struct    VertexPositionNormal
*  @brief     Vertex struct holding position and normal information.
*****************************************************************************/
struct VertexPositionNormal
{
public:
	gm::Float3 Position; /// Position
	gm::Float3 Normal;   /// Normal

	static const D3D12_INPUT_LAYOUT_DESC InputLayout;

	VertexPositionNormal() = default;
	VertexPositionNormal(const VertexPositionNormal&) = default;
	VertexPositionNormal& operator=(const VertexPositionNormal&) = default;
	VertexPositionNormal(VertexPositionNormal&&) = default;
	VertexPositionNormal& operator=(VertexPositionNormal&&) = default;
	VertexPositionNormal(DirectX::XMFLOAT3 const& position, DirectX::XMFLOAT3 const& normal)
		: Position(position), Normal(normal)
	{
	};
	VertexPositionNormal(DirectX::FXMVECTOR position, DirectX::FXMVECTOR normal)
	{
		DirectX::XMStoreFloat3(&this->Position, position);
		DirectX::XMStoreFloat3(&this->Normal, normal);
	}

private:
	static constexpr unsigned int InputElementCount = 2;
	static const D3D12_INPUT_ELEMENT_DESC InputElements[InputElementCount];
};

/****************************************************************************
*							 VertexPositionColorTexture
*************************************************************************//**
*  @struct    VertexPositionColorTexture
*  @brief     Vertex struct holding position, color and UV information.
*****************************************************************************/
struct VertexPositionColorTexture
{
public:
	gm::Float3 Position; /// Position
	gm::Float4 Color;    /// Color
	gm::Float2 UV;       /// UV

	static const D3D12_INPUT_LAYOUT_DESC InputLayout;

	VertexPositionColorTexture() = default;
	VertexPositionColorTexture(const VertexPositionColorTexture&) = default;
	VertexPositionColorTexture& operator=(const VertexPositionColorTexture&) = default;
	VertexPositionColorTexture(VertexPositionColorTexture&&) = default;
	VertexPositionColorTexture& operator=(VertexPositionColorTexture&&) = default;
	VertexPositionColorTexture(DirectX::XMFLOAT3 const& position, DirectX::XMFLOAT4 const& color, DirectX::XMFLOAT2 const& uv)
		: Position(position), Color(color), UV(uv)
	{
	};
	VertexPositionColorTexture(DirectX::FXMVECTOR position, DirectX::FXMVECTOR color, DirectX::FXMVECTOR uv)
	{
		DirectX::XMStoreFloat3(&this->Position, position);
		DirectX::XMStoreFloat4(&this->Color, color);
		DirectX::XMStoreFloat2(&this->UV, uv);
	}

private:
	static constexpr unsigned int InputElementCount = 3;
	static const D3D12_INPUT_ELEMENT_DESC InputElements[InputElementCount];
};

/****************************************************************************
*							 VertexPositionNormalColor
*************************************************************************//**
*  @struct    VertexPositionNormalColor
*  @brief     Vertex struct holding position, normal and color information.
*****************************************************************************/
struct VertexPositionNormalColor
{
public:
	gm::Float3 Position;
	gm::Float3 Normal;
	gm::Float4 Color;

	static const D3D12_INPUT_LAYOUT_DESC InputLayout;

	VertexPositionNormalColor() = default;
	VertexPositionNormalColor(const VertexPositionNormalColor&) = default;
	VertexPositionNormalColor& operator=(const VertexPositionNormalColor&) = default;
	VertexPositionNormalColor(VertexPositionNormalColor&&) = default;
	VertexPositionNormalColor& operator=(VertexPositionNormalColor&&) = default;
	VertexPositionNormalColor(DirectX::XMFLOAT3 const& position, DirectX::XMFLOAT3 const& normal, DirectX::XMFLOAT4 const& color)
		: Position(position), Normal(normal), Color(color)
	{
	};
	VertexPositionNormalColor(DirectX::FXMVECTOR position, DirectX::FXMVECTOR normal, DirectX::FXMVECTOR color)
	{
		DirectX::XMStoreFloat3(&this->Position, position);
		DirectX::XMStoreFloat3(&this->Normal, normal);
		DirectX::XMStoreFloat4(&this->Color, color);
	}

private:
	static constexpr unsigned int InputElementCount = 3;
	static const D3D12_INPUT_ELEMENT_DESC InputElements[InputElementCount];
};

/****************************************************************************
*							VertexPositionNormalColor
*************************************************************************//**
*  @struct    VertexPositionNormalTexture
*  @brief     Vertex struct holding position, normal and UV information.
*****************************************************************************/
struct VertexPositionNormalTexture
{
public:
	gm::Float3 Position; /// Position
	gm::Float3 Normal;   /// Normal
	gm::Float2 UV;       /// UV

	static const D3D12_INPUT_LAYOUT_DESC InputLayout;

	VertexPositionNormalTexture() = default;
	VertexPositionNormalTexture(const VertexPositionNormalTexture&) = default;
	VertexPositionNormalTexture& operator=(const VertexPositionNormalTexture&) = default;
	VertexPositionNormalTexture(VertexPositionNormalTexture&&) = default;
	VertexPositionNormalTexture& operator=(VertexPositionNormalTexture&&) = default;
	VertexPositionNormalTexture(DirectX::XMFLOAT3 const& position, DirectX::XMFLOAT3 const& normal, DirectX::XMFLOAT2 const& uv)
		: Position(position), Normal(normal), UV(uv)
	{
	};
	VertexPositionNormalTexture(DirectX::FXMVECTOR position, DirectX::FXMVECTOR normal, DirectX::FXMVECTOR uv)
	{
		DirectX::XMStoreFloat3(&this->Position, position);
		DirectX::XMStoreFloat3(&this->Normal, normal);
		DirectX::XMStoreFloat2(&this->UV, uv);

	}

private:
	static constexpr unsigned int InputElementCount = 3;
	static const D3D12_INPUT_ELEMENT_DESC InputElements[InputElementCount];
};

/****************************************************************************
*					  VertexPositionNormalColorTexture
*************************************************************************//**
*  @struct    VertexPositionNormalColorTexture
*  @brief     Vertex struct holding position, normal, color and uv information.
*****************************************************************************/
struct VertexPositionNormalColorTexture
{
public:
	gm::Float3 Position; /// Position
	gm::Float3 Normal;   /// Normal
	gm::Float4 Color;    /// Color
	gm::Float2 UV;       /// UV

	static const D3D12_INPUT_LAYOUT_DESC InputLayout;

	VertexPositionNormalColorTexture() = default;
	VertexPositionNormalColorTexture(const VertexPositionNormalColorTexture&) = default;
	VertexPositionNormalColorTexture& operator=(const VertexPositionNormalColorTexture&) = default;
	VertexPositionNormalColorTexture(VertexPositionNormalColorTexture&&) = default;
	VertexPositionNormalColorTexture& operator=(VertexPositionNormalColorTexture&&) = default;
	VertexPositionNormalColorTexture(DirectX::XMFLOAT3 const& position, DirectX::XMFLOAT3 const& normal, DirectX::XMFLOAT4 const& color, DirectX::XMFLOAT2 const& uv)
		: Position(position), Normal(normal), Color(color), UV(uv)
	{
	};
	VertexPositionNormalColorTexture(DirectX::FXMVECTOR position, DirectX::FXMVECTOR normal, DirectX::FXMVECTOR color, DirectX::FXMVECTOR uv)
	{
		DirectX::XMStoreFloat3(&this->Position, position);
		DirectX::XMStoreFloat3(&this->Normal, normal);
		DirectX::XMStoreFloat4(&this->Color, color);
		DirectX::XMStoreFloat2(&this->UV, uv);
	}

private:
	static constexpr unsigned int InputElementCount = 4;
	static const D3D12_INPUT_ELEMENT_DESC InputElements[InputElementCount];

};

/****************************************************************************
*					  VertexPositionNormalTextureTangent
*************************************************************************//**
*  @struct    VertexPositionNormalTextureTangent
*  @brief     Vertex struct holding position, normal, uv and tangent information.
*****************************************************************************/
struct VertexPositionNormalTextureTangent
{
public:
	gm::Float3 Position; /// Position
	gm::Float3 Normal;   /// Normal
	gm::Float2 UV;       /// Texture
	gm::Float3 Tangent;  /// Tangent
	gm::Float3 Binormal; // Binormal

	static const D3D12_INPUT_LAYOUT_DESC InputLayout;

	VertexPositionNormalTextureTangent() = default;
	VertexPositionNormalTextureTangent(const VertexPositionNormalTextureTangent&) = default;
	VertexPositionNormalTextureTangent& operator=(const VertexPositionNormalTextureTangent&) = default;
	VertexPositionNormalTextureTangent(VertexPositionNormalTextureTangent&&) = default;
	VertexPositionNormalTextureTangent& operator=(VertexPositionNormalTextureTangent&&) = default;
	VertexPositionNormalTextureTangent(DirectX::XMFLOAT3 const& position, DirectX::XMFLOAT3 const& normal, DirectX::XMFLOAT2 const& uv, DirectX::XMFLOAT3 const& tangent)
		: Position(position), Normal(normal), UV(uv), Tangent(tangent)
	{
	};
	VertexPositionNormalTextureTangent(DirectX::FXMVECTOR position, DirectX::FXMVECTOR normal, DirectX::FXMVECTOR uv, DirectX::FXMVECTOR tangent, DirectX::FXMVECTOR binormal)
	{
		DirectX::XMStoreFloat3(&this->Position, position);
		DirectX::XMStoreFloat3(&this->Normal, normal);
		DirectX::XMStoreFloat2(&this->UV, uv);
		DirectX::XMStoreFloat3(&this->Tangent, tangent);
		DirectX::XMStoreFloat3(&this->Binormal, binormal);
	}

private:
	static constexpr unsigned int InputElementCount = 5;
	static const D3D12_INPUT_ELEMENT_DESC InputElements[InputElementCount];
};
#endif