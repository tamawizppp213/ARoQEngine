//////////////////////////////////////////////////////////////////////////////////
//              @file   GLTFProperty.cpp
///             @brief  GLTF Property
///             @author Copyright (c) Microsoft Corporation. All rights reserved.
///                     Licensed under the MIT License. 
///             @date   2022_05_30
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Rendering/Model/External/GLTF/Private/Include/GLTFColor.hpp"
#include "GameUtility/Math/Include/GMMath.hpp"
#include <limits>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gltf;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
namespace
{


    inline float ByteToFloat(uint8_t value)
    {
        return value / 255.0f;
    }

    inline uint8_t FloatToByte(float value)
    {
        return static_cast<uint8_t>(value * 255.0f + 0.5f);
    }
    inline float ToGamma(float value)
    {
        if (value <= 0.0031308f)
        {
            return value * 12.92f;
        }

        return 1.055f * pow(value, 1.0f / 2.4f) - 0.055f;
    }
    // https://en.wikipedia.org/wiki/SRGB#The_reverse_transformation
    inline float ToLinear(float value)
    {
        if (value < 0.04045f)
        {
            return value / 12.92f;
        }

        return pow((value + 0.055f) / 1.055f, 2.4f);
    }
}


Color3::Color3(float r, float g, float b) :
    r(r),
    g(g),
    b(b)
{
}

Color3::Color3(uint8_t r, uint8_t g, uint8_t b) :
    r(ByteToFloat(r)),
    g(ByteToFloat(g)),
    b(ByteToFloat(b))
{
}

Color3 Color3::FromScalar(float value)
{
    return { value, value, value };
}

Color3& Color3::operator*=(const Color3& rhs)
{
    r *= rhs.r;
    g *= rhs.g;
    b *= rhs.b;

    return *this;
}

Color3& Color3::operator*=(float rhs)
{
    r *= rhs;
    g *= rhs;
    b *= rhs;

    return *this;
}

Color3& Color3::operator/=(const Color3& rhs)
{
    r /= rhs.r;
    g /= rhs.g;
    b /= rhs.b;

    return *this;
}

Color3& Color3::operator/=(float rhs)
{
    r /= rhs;
    g /= rhs;
    b /= rhs;

    return *this;
}

Color3& Color3::operator+=(const Color3& rhs)
{
    r += rhs.r;
    g += rhs.g;
    b += rhs.b;

    return *this;
}

Color3& Color3::operator+=(float rhs)
{
    r += rhs;
    g += rhs;
    b += rhs;

    return *this;
}

Color3& Color3::operator-=(const Color3& rhs)
{
    r -= rhs.r;
    g -= rhs.g;
    b -= rhs.b;

    return *this;
}

Color3& Color3::operator-=(float rhs)
{
    r -= rhs;
    g -= rhs;
    b -= rhs;

    return *this;
}

Color3 Color3::ToGamma() const
{
    return Color3(::ToGamma(r), ::ToGamma(g), ::ToGamma(b));
}

Color3 Color3::ToLinear() const
{
    return Color3(::ToLinear(r), ::ToLinear(g), ::ToLinear(b));
}

Color4 Color3::AsColor4(float a) const
{
    return { r, g, b, a };
}

uint32_t Color3::AsUint32RGBA() const
{
    const uint8_t rByte = FloatToByte(r);
    const uint8_t gByte = FloatToByte(g);
    const uint8_t bByte = FloatToByte(b);
    const uint8_t aByte = std::numeric_limits<uint8_t>::max();

    const uint32_t rgba = aByte << 24 | bByte << 16 | gByte << 8 | rByte;
    return rgba;
}

uint32_t Color3::AsUint32BGRA() const
{
    const uint8_t bByte = FloatToByte(b);
    const uint8_t gByte = FloatToByte(g);
    const uint8_t rByte = FloatToByte(r);
    const uint8_t aByte = std::numeric_limits<uint8_t>::max();

    const uint32_t bgra = aByte << 24 | rByte << 16 | gByte << 8 | bByte;
    return bgra;
}

Color3 Color3::FromUint32RGBA(uint32_t color)
{
    const uint8_t r = static_cast<uint8_t>(color & 0x000000ff);
    const uint8_t g = static_cast<uint8_t>((color & 0x0000ff00) >> 8);
    const uint8_t b = static_cast<uint8_t>((color & 0x00ff0000) >> 16);

    return Color3(r, g, b);
}

Color3 Color3::FromUint32BGRA(uint32_t color)
{
    const uint8_t b = static_cast<uint8_t>(color & 0x000000ff);
    const uint8_t g = static_cast<uint8_t>((color & 0x0000ff00) >> 8);
    const uint8_t r = static_cast<uint8_t>((color & 0x00ff0000) >> 16);

    return Color3(r, g, b);
}

Color3 Color3::Clamp(const Color3& color, float lo, float hi)
{
    return 
    {
        gm::Clamp(color.r, lo, hi),
        gm::Clamp(color.g, lo, hi),
        gm::Clamp(color.b, lo, hi)
    };
}

bool gltf::operator==(const Color3& lhs, const Color3& rhs)
{
    return lhs.r == rhs.r
        && lhs.g == rhs.g
        && lhs.b == rhs.b;
}

bool gltf::operator!=(const Color3& lhs, const Color3& rhs)
{
    return !(lhs == rhs);
}

Color4::Color4(float r, float g, float b, float a) :
    r(r),
    g(g),
    b(b),
    a(a)
{
}

Color4::Color4(uint8_t r, uint8_t g, uint8_t b, uint8_t a) :
    r(ByteToFloat(r)),
    g(ByteToFloat(g)),
    b(ByteToFloat(b)),
    a(ByteToFloat(a))
{
}

Color4 Color4::FromScalar(float value)
{
    return { value, value, value, value };
}

Color4& Color4::operator*=(const Color4& rhs)
{
    r *= rhs.r;
    g *= rhs.g;
    b *= rhs.b;
    a *= rhs.a;

    return *this;
}

Color4& Color4::operator*=(float rhs)
{
    r *= rhs;
    g *= rhs;
    b *= rhs;
    a *= rhs;

    return *this;
}

Color4& Color4::operator/=(const Color4& rhs)
{
    r /= rhs.r;
    g /= rhs.g;
    b /= rhs.b;
    a /= rhs.a;

    return *this;
}

Color4& Color4::operator/=(float rhs)
{
    r /= rhs;
    g /= rhs;
    b /= rhs;
    a /= rhs;

    return *this;
}

Color4& Color4::operator+=(const Color4& rhs)
{
    r += rhs.r;
    g += rhs.g;
    b += rhs.b;
    a += rhs.a;

    return *this;
}

Color4& Color4::operator+=(float rhs)
{
    r += rhs;
    g += rhs;
    b += rhs;
    a += rhs;

    return *this;
}

Color4& Color4::operator-=(const Color4& rhs)
{
    r -= rhs.r;
    g -= rhs.g;
    b -= rhs.b;
    a -= rhs.a;

    return *this;
}

Color4& Color4::operator-=(float rhs)
{
    r -= rhs;
    g -= rhs;
    b -= rhs;
    a -= rhs;

    return *this;
}

Color3 Color4::AsColor3() const
{
    return { r, g, b };
}

uint32_t Color4::AsUint32RGBA() const
{
    const uint8_t rByte = FloatToByte(r);
    const uint8_t gByte = FloatToByte(g);
    const uint8_t bByte = FloatToByte(b);
    const uint8_t aByte = FloatToByte(a);

    const uint32_t rgba = aByte << 24 | bByte << 16 | gByte << 8 | rByte;
    return rgba;
}

uint32_t Color4::AsUint32BGRA() const
{
    const uint8_t bByte = FloatToByte(b);
    const uint8_t gByte = FloatToByte(g);
    const uint8_t rByte = FloatToByte(r);
    const uint8_t aByte = FloatToByte(a);

    const uint32_t bgra = aByte << 24 | rByte << 16 | gByte << 8 | bByte;
    return bgra;
}

Color4 Color4::FromUint32RGBA(uint32_t color)
{
    const uint8_t r = static_cast<uint8_t>(color & 0x000000ff);
    const uint8_t g = static_cast<uint8_t>((color & 0x0000ff00) >> 8);
    const uint8_t b = static_cast<uint8_t>((color & 0x00ff0000) >> 16);
    const uint8_t a = static_cast<uint8_t>((color & 0xff000000) >> 24);

    return Color4(r, g, b, a);
}

Color4 Color4::FromUint32BGRA(uint32_t color)
{
    const uint8_t b = static_cast<uint8_t>(color & 0x000000ff);
    const uint8_t g = static_cast<uint8_t>((color & 0x0000ff00) >> 8);
    const uint8_t r = static_cast<uint8_t>((color & 0x00ff0000) >> 16);
    const uint8_t a = static_cast<uint8_t>((color & 0xff000000) >> 24);

    return Color4(r, g, b, a);
}

Color4 Color4::Clamp(const Color4& color, float lo, float hi)
{
    return {
        gm::Clamp(color.r, lo, hi),
        gm::Clamp(color.g, lo, hi),
        gm::Clamp(color.b, lo, hi),
        gm::Clamp(color.a, lo, hi)
    };
}

bool gltf::operator==(const Color4& lhs, const Color4& rhs)
{
    return lhs.r == rhs.r
        && lhs.g == rhs.g
        && lhs.b == rhs.b
        && lhs.a == rhs.a;
}

bool gltf::operator!=(const Color4& lhs, const Color4& rhs)
{
    return !(lhs == rhs);
}