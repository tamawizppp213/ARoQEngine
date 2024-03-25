//////////////////////////////////////////////////////////////////////////////////
///             @file   GMColor.hpp
///             @brief  Game Math ()
///             @author Toide Yutaro
///             @date   2021_05_16
//////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef GM_COLOR_HPP
#define GM_COLOR_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GMVector.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace gm
{
	/****************************************************************************
	*				  			Float4
	*************************************************************************//**
	*  @class     Float4
	*  @brief     Float4 class for holding the value
	*****************************************************************************/
    namespace color
    {
        enum class ColorTag : gu::uint8
        {
            RGBA,
            SRGB,
            HSL,
            HSV,
            XYZ,
            YUV,
            YUV_HD,
            RGBA_HD,
            REC_709,

        };
    }
    
    // Simple rgba
    struct RGBA
    {
        float R;
        float G;
        float B;
        float A;

        RGBA() = default;
        RGBA(const float r, const float g, const float b, const float a)
            : R(r), G(g), B(b), A(a)
        {
            
        }
    };

    struct BGRA
    {
        float B;
        float G;
        float R;
        float A;

        BGRA() = default;
        BGRA(const float b, const float g, const float r, const float a)
            : R(r), G(g), B(b), A(a)
        {

        }
    };

//	struct Color
//	{
//	public:
//		/****************************************************************************
//		**                Public Function
//		*****************************************************************************/
//        uint32_t R10G10B10A2(void) const;
//        uint32_t R8G8B8A8(void) const;
//		/****************************************************************************
//		**                Public Member Variables
//		*****************************************************************************/
//		INLINE float R() const  { return DirectX::XMVectorGetX(_value); }
//		INLINE float G() const  { return DirectX::XMVectorGetY(_value); }
//		INLINE float B() const  { return DirectX::XMVectorGetZ(_value); }
//		INLINE float A() const  { return DirectX::XMVectorGetW(_value); }
//		INLINE void  R(float r) { _value.f[0] = r; }
//		INLINE void  G(float g) { _value.f[1] = g; }
//		INLINE void  B(float b) { _value.f[2] = b; }
//		INLINE void  A(float a) { _value.f[3] = a; }
//        INLINE uint8_t GetTag() { return (uint8_t)_tag; };
//        INLINE Float4 ToFloat4() { return Float4(_value); }
//        INLINE Float3 ToFloat3() { return Float3(_value); }
//		INLINE void  SetRGB(float r, float g, float b)           { _value.v = DirectX::XMVectorSelect(_value, DirectX::XMVectorSet(r, g, b, b), DirectX::g_XMMask3); }
//		INLINE void  SetRGBA(float r, float g, float b, float a) { _value.v = DirectX::XMVectorSet(r, g, b, a); }
//		INLINE float* Ptr(void) { return reinterpret_cast<float*>(this); }
//		INLINE float& operator[](int idx) { return Ptr()[idx]; }
//		INLINE bool operator == (const Color& rhs) const noexcept { DirectX::XMVector4Equal(*this, rhs); };
//		INLINE bool operator != (const Color& rhs) const noexcept { DirectX::XMVector4NotEqual(*this, rhs); };
//        INLINE Color& operator+= (const Color& color) noexcept { *this = DirectX::XMVectorSaturate(DirectX::XMVectorAdd(_value, color)); return *this; }
//        INLINE Color& operator-= (const Color& color) noexcept { *this = DirectX::XMVectorSaturate(DirectX::XMVectorSubtract(_value, color)); return *this; }
//        INLINE Color& operator*= (const Color& color) noexcept { *this = DirectX::XMVectorSaturate(DirectX::XMVectorMultiply(_value, color)); return *this; }
//        INLINE Color& operator/= (const Color& color) noexcept { *this = DirectX::XMVectorSaturate(DirectX::XMVectorDivide(_value, color)); return *this; }
//        INLINE Color& operator*= (const float  S) noexcept { *this = DirectX::XMVectorSaturate(DirectX::XMVectorScale(_value, S)); return *this; }
//
//		/****************************************************************************
//		**                Constructor and Destructor
//		*****************************************************************************/
//		INLINE Color(                                 color::ColorTag tag = color::ColorTag::RGBA) : _value(DirectX::g_XMOne), _tag(tag) {};
//        INLINE Color(DirectX::FXMVECTOR vec,          color::ColorTag tag = color::ColorTag::RGBA) { _value.v = vec; _tag = tag; }
//		INLINE Color(const DirectX::XMVECTORF32& vec, color::ColorTag tag = color::ColorTag::RGBA) { _value   = vec; _tag = tag;}
//        INLINE Color(float r, float g, float b, float a = 1.0f, color::ColorTag tag = color::ColorTag::RGBA) { _value.v = DirectX::XMVectorSet(r, g, b, a); _tag = tag; }
//        INLINE Color(uint16_t r, uint16_t g, uint16_t b, uint16_t a = 255, uint16_t bitDepth = 8, color::ColorTag tag = color::ColorTag::RGBA) { _value.v = DirectX::XMVectorScale(DirectX::XMVectorSet(r, g, b, a), 1.0f / ((1 << bitDepth) - 1)); _tag = tag; }
//		INLINE explicit Color(uint32_t rgbaLittleEndian)
//		{
//			float r = (float)((rgbaLittleEndian >> 0) & 0xFF);
//			float g = (float)((rgbaLittleEndian >> 8) & 0xFF);
//			float b = (float)((rgbaLittleEndian >> 16) & 0xFF);
//			float a = (float)((rgbaLittleEndian >> 24) & 0xFF);
//			_value.v = DirectX::XMVectorScale(DirectX::XMVectorSet(r, g, b, a), 1.0f / 255.0f);
//		}
//
//
//		Color(const Color&)            = default;
//		Color& operator=(const Color&) = default;
//		Color(Color&&)                 = default;
//		Color& operator=(Color&&)      = default;
//
//		INLINE operator DirectX::XMVECTOR() const { return _value; }
//	private:
//		/****************************************************************************
//		**                Private Function
//		*****************************************************************************/
//
//		/****************************************************************************
//		**                Private Member Variables
//		*****************************************************************************/
//		DirectX::XMVECTORF32 _value;
//        color::ColorTag _tag = color::ColorTag::RGBA;
//	};
//    INLINE Color operator+ (const Color& C1, const Color& C2) noexcept { return Color(DirectX::XMVectorSaturate(DirectX::XMVectorAdd(C1, C2))); }
//    INLINE Color operator- (const Color& C1, const Color& C2) noexcept { return Color(DirectX::XMVectorSaturate(DirectX::XMVectorSubtract(C1, C2))); }
//    INLINE Color operator* (const Color& C1, const Color& C2) noexcept { return Color(DirectX::XMVectorSaturate(DirectX::XMVectorMultiply(C1, C2))); }
//    INLINE Color operator/ (const Color& C1, const Color& C2) noexcept { return Color(DirectX::XMVectorSaturate(DirectX::XMVectorDivide(C1, C2))); }
//    INLINE Color operator* (const Color& C1, float S) noexcept { return Color(DirectX::XMVectorSaturate(DirectX::XMVectorScale(C1, S))); }
//
//    INLINE Color Max             (Color a, Color b)              { return Color(DirectX::XMVectorMax(a, b)); }
//    INLINE Color Min             (Color a, Color b)              { return Color(DirectX::XMVectorMin(a, b)); }
//    INLINE Color Clamp           (Color x, Color a, Color b)     { return Color(DirectX::XMVectorClamp(x, a, b)); }
//    INLINE Color Negate          (Color color)                   { return Color(DirectX::XMColorNegative(color)); }
//    INLINE Color Saturate        (Color color)                   { return Color(DirectX::XMVectorSaturate(color)); }
//    INLINE Color AdjustSaturation(Color color, float sat)        { return Color(DirectX::XMColorAdjustSaturation(color, sat)); }
//    INLINE Color AdjustContrast  (Color color, float contrast)   { return Color(DirectX::XMColorAdjustContrast(color, contrast)); }
//    INLINE Color Modulate  (const Color& c1, const Color& c2)    { return Color(DirectX::XMColorModulate(c1, c2)); }
//    INLINE Color Lerp(const Color& c1, const Color& c2, float t) { return Color(DirectX::XMVectorLerp(c1, c2, t)); }
//    INLINE bool ColorIsNAN(const Color& color) { return DirectX::XMColorIsNaN(color); }
//    INLINE bool ColorIsInfinite(const Color& color) { return DirectX::XMColorIsInfinite(color); }
//    INLINE Color HSLToRGB(Color hsl)
//    { 
//        assert((uint8_t)color::ColorTag::HSL == hsl.GetTag());
//        return Color(DirectX::XMColorHSLToRGB(hsl), color::ColorTag::RGBA);
//    }
//    INLINE Color HSVToRGB(Color hsv)
//    {
//        assert((uint8_t)color::ColorTag::HSV == hsv.GetTag());
//        return Color(DirectX::XMColorHSVToRGB(hsv), color::ColorTag::RGBA);
//    }
//    INLINE Color RGBToHSV(Color rgb)
//    {
//        assert((uint8_t)color::ColorTag::RGBA == rgb.GetTag());
//        return Color(DirectX::XMColorRGBToHSV(rgb), color::ColorTag::HSV);
//    }
//    INLINE Color RGBToHSL(Color rgb)
//    {
//        assert((uint8_t)color::ColorTag::RGBA == rgb.GetTag());
//        return Color(DirectX::XMColorRGBToHSL(rgb), color::ColorTag::HSL);
//    }
//    INLINE Color RGBToSRGB(Color rgb)
//    {
//        assert((uint8_t)color::ColorTag::RGBA == rgb.GetTag());
//        return Color(DirectX::XMColorRGBToSRGB(rgb), color::ColorTag::SRGB);
//    }
//    INLINE Color RGBToXYZ(Color rgb)
//    {
//        assert((uint8_t)color::ColorTag::RGBA == rgb.GetTag());
//        return Color(DirectX::XMColorRGBToXYZ(rgb), color::ColorTag::XYZ);
//    }
//    INLINE Color RGBToYUV(Color rgb)
//    {
//        assert((uint8_t)color::ColorTag::RGBA == rgb.GetTag());
//        return Color(DirectX::XMColorRGBToYUV(rgb), color::ColorTag::YUV);
//    }
//    INLINE Color RGBToYUV_HD(Color rgb)
//    {
//        assert((uint8_t)color::ColorTag::RGBA == rgb.GetTag());
//        return Color(DirectX::XMColorRGBToYUV_HD(rgb), color::ColorTag::YUV_HD);
//    }
//    INLINE Color SRGBToRGB(Color srgb)
//    {
//        assert((uint8_t)color::ColorTag::SRGB == srgb.GetTag());
//        return Color(DirectX::XMColorSRGBToRGB(srgb), color::ColorTag::RGBA);
//    }
//    INLINE Color SRGBToXYZ(Color srgb)
//    {
//        assert((uint8_t)color::ColorTag::SRGB == srgb.GetTag());
//        return Color(DirectX::XMColorSRGBToXYZ(srgb), color::ColorTag::XYZ);
//    }
//    INLINE Color XYZToRGB(Color xyz)
//    {
//        assert((uint8_t)color::ColorTag::XYZ == xyz.GetTag());
//        return Color(DirectX::XMColorXYZToRGB(xyz), color::ColorTag::RGBA);
//    }
//    INLINE Color XYZToSRGB(Color xyz)
//    {
//        assert((uint8_t)color::ColorTag::XYZ == xyz.GetTag());
//        return Color(DirectX::XMColorXYZToSRGB(xyz), color::ColorTag::SRGB);
//    }
//    INLINE Color YUVToRGB(Color yuv)
//    {
//        assert((uint8_t)color::ColorTag::YUV == yuv.GetTag());
//        return Color(DirectX::XMColorYUVToRGB(yuv), color::ColorTag::RGBA);
//    }
//    INLINE Color YUVToRGB_HD(Color yuv)
//    {
//        assert((uint8_t)color::ColorTag::YUV == yuv.GetTag());
//        return Color(DirectX::XMColorYUVToRGB_HD(yuv), color::ColorTag::RGBA_HD);
//    }
//    INLINE Color RGBAToREC709(Color rgba)
//    {
//        using namespace DirectX;
//        assert((uint8_t)color::ColorTag::RGBA == rgba.GetTag());
//        XMVECTOR T      = XMVectorSaturate(rgba);
//        XMVECTOR result = XMVectorSubtract(XMVectorScale(XMVectorPow(T, XMVectorReplicate(0.45f)), 1.099f), XMVectorReplicate(0.099f));
//        result          = XMVectorSelect(result, XMVectorScale(T, 4.5f), XMVectorLess(T, XMVectorReplicate(0.0018f)));
//        return Color(XMVectorSelect(T, result, g_XMSelect1110), color::ColorTag::REC_709);
//    }
//
//    INLINE Color REC709ToRGBA(Color rec709) 
//    {
//        using namespace DirectX;
//        assert((uint8_t)color::ColorTag::REC_709 == rec709.GetTag());
//        XMVECTOR T      = XMVectorSaturate(rec709);
//        XMVECTOR result = XMVectorPow(XMVectorScale(XMVectorAdd(T, XMVectorReplicate(0.099f)), 1.0f / 1.099f), XMVectorReplicate(1.0f / 0.45f));
//        result          = XMVectorSelect(result, XMVectorScale(T, 1.0f / 4.5f), XMVectorLess(T, XMVectorReplicate(0.0081f)));
//        return Color(XMVectorSelect(T, result, g_XMSelect1110), color::ColorTag::RGBA);
//    }
//
//    INLINE uint32_t Color::R10G10B10A2(void) const
//    {
//        using namespace DirectX;
//        XMVECTOR result = XMVectorRound(XMVectorMultiply(XMVectorSaturate(_value), XMVectorSet(1023.0f, 1023.0f, 1023.0f, 3.0f)));
//        result     = _mm_castsi128_ps(_mm_cvttps_epi32(result));
//        uint32_t r = XMVectorGetIntX(result);
//        uint32_t g = XMVectorGetIntY(result);
//        uint32_t b = XMVectorGetIntZ(result);
//        uint32_t a = XMVectorGetIntW(result) >> 8;
//        return a << 30 | b << 20 | g << 10 | r;
//    }
//
//    INLINE uint32_t Color::R8G8B8A8(void) const
//    {
//        using namespace DirectX;
//        XMVECTOR result = XMVectorRound(XMVectorMultiply(XMVectorSaturate(_value), XMVectorReplicate(255.0f)));
//        result = _mm_castsi128_ps(_mm_cvttps_epi32(result));
//        uint32_t r = XMVectorGetIntX(result);
//        uint32_t g = XMVectorGetIntY(result);
//        uint32_t b = XMVectorGetIntZ(result);
//        uint32_t a = XMVectorGetIntW(result);
//        return a << 24 | b << 16 | g << 8 | r;
//    }
//	namespace color
//	{
//        using namespace DirectX;
//		// Standard colors (Red/Green/Blue/Alpha)
//        XMGLOBALCONST XMVECTORF32 AliceBlue            = { { { 0.941176534f, 0.972549081f, 1.000000000f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 AntiqueWhite         = { { { 0.980392218f, 0.921568692f, 0.843137324f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 Aqua                 = { { { 0.000000000f, 1.000000000f, 1.000000000f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 Aquamarine           = { { { 0.498039246f, 1.000000000f, 0.831372619f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 Azure                = { { { 0.941176534f, 1.000000000f, 1.000000000f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 Beige                = { { { 0.960784376f, 0.960784376f, 0.862745166f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 Bisque               = { { { 1.000000000f, 0.894117713f, 0.768627524f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 Black                = { { { 0.000000000f, 0.000000000f, 0.000000000f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 BlanchedAlmond       = { { { 1.000000000f, 0.921568692f, 0.803921640f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 Blue                 = { { { 0.000000000f, 0.000000000f, 1.000000000f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 BlueViolet           = { { { 0.541176498f, 0.168627456f, 0.886274576f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 Brown                = { { { 0.647058845f, 0.164705887f, 0.164705887f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 BurlyWood            = { { { 0.870588303f, 0.721568644f, 0.529411793f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 CadetBlue            = { { { 0.372549027f, 0.619607866f, 0.627451003f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 Chartreuse           = { { { 0.498039246f, 1.000000000f, 0.000000000f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 Chocolate            = { { { 0.823529482f, 0.411764741f, 0.117647067f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 Coral                = { { { 1.000000000f, 0.498039246f, 0.313725501f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 CornflowerBlue       = { { { 0.392156899f, 0.584313750f, 0.929411829f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 Cornsilk             = { { { 1.000000000f, 0.972549081f, 0.862745166f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 Crimson              = { { { 0.862745166f, 0.078431375f, 0.235294133f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 Cyan                 = { { { 0.000000000f, 1.000000000f, 1.000000000f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 DarkBlue             = { { { 0.000000000f, 0.000000000f, 0.545098066f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 DarkCyan             = { { { 0.000000000f, 0.545098066f, 0.545098066f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 DarkGoldenrod        = { { { 0.721568644f, 0.525490224f, 0.043137256f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 DarkGray             = { { { 0.662745118f, 0.662745118f, 0.662745118f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 DarkGreen            = { { { 0.000000000f, 0.392156899f, 0.000000000f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 DarkKhaki            = { { { 0.741176486f, 0.717647076f, 0.419607878f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 DarkMagenta          = { { { 0.545098066f, 0.000000000f, 0.545098066f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 DarkOliveGreen       = { { { 0.333333343f, 0.419607878f, 0.184313729f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 DarkOrange           = { { { 1.000000000f, 0.549019635f, 0.000000000f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 DarkOrchid           = { { { 0.600000024f, 0.196078449f, 0.800000072f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 DarkRed              = { { { 0.545098066f, 0.000000000f, 0.000000000f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 DarkSalmon           = { { { 0.913725555f, 0.588235319f, 0.478431404f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 DarkSeaGreen         = { { { 0.560784340f, 0.737254918f, 0.545098066f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 DarkSlateBlue        = { { { 0.282352954f, 0.239215702f, 0.545098066f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 DarkSlateGray        = { { { 0.184313729f, 0.309803933f, 0.309803933f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 DarkTurquoise        = { { { 0.000000000f, 0.807843208f, 0.819607913f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 DarkViolet           = { { { 0.580392182f, 0.000000000f, 0.827451050f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 DeepPink             = { { { 1.000000000f, 0.078431375f, 0.576470613f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 DeepSkyBlue          = { { { 0.000000000f, 0.749019623f, 1.000000000f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 DimGray              = { { { 0.411764741f, 0.411764741f, 0.411764741f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 DodgerBlue           = { { { 0.117647067f, 0.564705908f, 1.000000000f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 Firebrick            = { { { 0.698039234f, 0.133333340f, 0.133333340f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 FloralWhite          = { { { 1.000000000f, 0.980392218f, 0.941176534f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 ForestGreen          = { { { 0.133333340f, 0.545098066f, 0.133333340f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 Fuchsia              = { { { 1.000000000f, 0.000000000f, 1.000000000f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 Gainsboro            = { { { 0.862745166f, 0.862745166f, 0.862745166f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 GhostWhite           = { { { 0.972549081f, 0.972549081f, 1.000000000f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 Gold                 = { { { 1.000000000f, 0.843137324f, 0.000000000f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 Goldenrod            = { { { 0.854902029f, 0.647058845f, 0.125490203f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 Gray                 = { { { 0.501960814f, 0.501960814f, 0.501960814f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 Green                = { { { 0.000000000f, 0.501960814f, 0.000000000f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 GreenYellow          = { { { 0.678431392f, 1.000000000f, 0.184313729f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 Honeydew             = { { { 0.941176534f, 1.000000000f, 0.941176534f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 HotPink              = { { { 1.000000000f, 0.411764741f, 0.705882370f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 IndianRed            = { { { 0.803921640f, 0.360784322f, 0.360784322f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 Indigo               = { { { 0.294117659f, 0.000000000f, 0.509803951f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 Ivory                = { { { 1.000000000f, 1.000000000f, 0.941176534f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 Khaki                = { { { 0.941176534f, 0.901960850f, 0.549019635f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 Lavender             = { { { 0.901960850f, 0.901960850f, 0.980392218f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 LavenderBlush        = { { { 1.000000000f, 0.941176534f, 0.960784376f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 LawnGreen            = { { { 0.486274540f, 0.988235354f, 0.000000000f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 LemonChiffon         = { { { 1.000000000f, 0.980392218f, 0.803921640f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 LightBlue            = { { { 0.678431392f, 0.847058892f, 0.901960850f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 LightCoral           = { { { 0.941176534f, 0.501960814f, 0.501960814f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 LightCyan            = { { { 0.878431439f, 1.000000000f, 1.000000000f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 LightGoldenrodYellow = { { { 0.980392218f, 0.980392218f, 0.823529482f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 LightGreen           = { { { 0.564705908f, 0.933333397f, 0.564705908f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 LightGray            = { { { 0.827451050f, 0.827451050f, 0.827451050f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 LightPink            = { { { 1.000000000f, 0.713725507f, 0.756862819f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 LightSalmon          = { { { 1.000000000f, 0.627451003f, 0.478431404f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 LightSeaGreen        = { { { 0.125490203f, 0.698039234f, 0.666666687f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 LightSkyBlue         = { { { 0.529411793f, 0.807843208f, 0.980392218f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 LightSlateGray       = { { { 0.466666698f, 0.533333361f, 0.600000024f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 LightSteelBlue       = { { { 0.690196097f, 0.768627524f, 0.870588303f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 LightYellow          = { { { 1.000000000f, 1.000000000f, 0.878431439f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 Lime                 = { { { 0.000000000f, 1.000000000f, 0.000000000f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 LimeGreen            = { { { 0.196078449f, 0.803921640f, 0.196078449f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 Linen                = { { { 0.980392218f, 0.941176534f, 0.901960850f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 Magenta              = { { { 1.000000000f, 0.000000000f, 1.000000000f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 Maroon               = { { { 0.501960814f, 0.000000000f, 0.000000000f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 MediumAquamarine     = { { { 0.400000036f, 0.803921640f, 0.666666687f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 MediumBlue           = { { { 0.000000000f, 0.000000000f, 0.803921640f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 MediumOrchid         = { { { 0.729411781f, 0.333333343f, 0.827451050f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 MediumPurple         = { { { 0.576470613f, 0.439215720f, 0.858823597f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 MediumSeaGreen       = { { { 0.235294133f, 0.701960802f, 0.443137288f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 MediumSlateBlue      = { { { 0.482352972f, 0.407843173f, 0.933333397f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 MediumSpringGreen    = { { { 0.000000000f, 0.980392218f, 0.603921592f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 MediumTurquoise      = { { { 0.282352954f, 0.819607913f, 0.800000072f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 MediumVioletRed      = { { { 0.780392230f, 0.082352944f, 0.521568656f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 MidnightBlue         = { { { 0.098039225f, 0.098039225f, 0.439215720f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 MintCream            = { { { 0.960784376f, 1.000000000f, 0.980392218f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 MistyRose            = { { { 1.000000000f, 0.894117713f, 0.882353008f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 Moccasin             = { { { 1.000000000f, 0.894117713f, 0.709803939f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 NavajoWhite          = { { { 1.000000000f, 0.870588303f, 0.678431392f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 Navy                 = { { { 0.000000000f, 0.000000000f, 0.501960814f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 OldLace              = { { { 0.992156923f, 0.960784376f, 0.901960850f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 Olive                = { { { 0.501960814f, 0.501960814f, 0.000000000f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 OliveDrab            = { { { 0.419607878f, 0.556862772f, 0.137254909f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 Orange               = { { { 1.000000000f, 0.647058845f, 0.000000000f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 OrangeRed            = { { { 1.000000000f, 0.270588249f, 0.000000000f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 Orchid               = { { { 0.854902029f, 0.439215720f, 0.839215755f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 PaleGoldenrod        = { { { 0.933333397f, 0.909803987f, 0.666666687f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 PaleGreen            = { { { 0.596078455f, 0.984313786f, 0.596078455f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 PaleTurquoise        = { { { 0.686274529f, 0.933333397f, 0.933333397f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 PaleVioletRed        = { { { 0.858823597f, 0.439215720f, 0.576470613f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 PapayaWhip           = { { { 1.000000000f, 0.937254965f, 0.835294187f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 PeachPuff            = { { { 1.000000000f, 0.854902029f, 0.725490212f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 Peru                 = { { { 0.803921640f, 0.521568656f, 0.247058839f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 Pink                 = { { { 1.000000000f, 0.752941251f, 0.796078503f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 Plum                 = { { { 0.866666734f, 0.627451003f, 0.866666734f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 PowderBlue           = { { { 0.690196097f, 0.878431439f, 0.901960850f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 Purple               = { { { 0.501960814f, 0.000000000f, 0.501960814f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 Red                  = { { { 1.000000000f, 0.000000000f, 0.000000000f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 RosyBrown            = { { { 0.737254918f, 0.560784340f, 0.560784340f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 RoyalBlue            = { { { 0.254901975f, 0.411764741f, 0.882353008f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 SaddleBrown          = { { { 0.545098066f, 0.270588249f, 0.074509807f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 Salmon               = { { { 0.980392218f, 0.501960814f, 0.447058856f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 SandyBrown           = { { { 0.956862807f, 0.643137276f, 0.376470625f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 SeaGreen             = { { { 0.180392161f, 0.545098066f, 0.341176480f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 SeaShell             = { { { 1.000000000f, 0.960784376f, 0.933333397f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 Sienna               = { { { 0.627451003f, 0.321568638f, 0.176470593f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 Silver               = { { { 0.752941251f, 0.752941251f, 0.752941251f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 SkyBlue              = { { { 0.529411793f, 0.807843208f, 0.921568692f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 SlateBlue            = { { { 0.415686309f, 0.352941185f, 0.803921640f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 SlateGray            = { { { 0.439215720f, 0.501960814f, 0.564705908f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 Snow                 = { { { 1.000000000f, 0.980392218f, 0.980392218f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 SpringGreen          = { { { 0.000000000f, 1.000000000f, 0.498039246f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 SteelBlue            = { { { 0.274509817f, 0.509803951f, 0.705882370f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 Tan                  = { { { 0.823529482f, 0.705882370f, 0.549019635f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 Teal                 = { { { 0.000000000f, 0.501960814f, 0.501960814f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 Thistle              = { { { 0.847058892f, 0.749019623f, 0.847058892f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 Tomato               = { { { 1.000000000f, 0.388235331f, 0.278431386f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 Transparent          = { { { 0.000000000f, 0.000000000f, 0.000000000f, 0.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 Turquoise            = { { { 0.250980407f, 0.878431439f, 0.815686345f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 Violet               = { { { 0.933333397f, 0.509803951f, 0.933333397f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 Wheat                = { { { 0.960784376f, 0.870588303f, 0.701960802f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 White                = { { { 1.000000000f, 1.000000000f, 1.000000000f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 WhiteSmoke           = { { { 0.960784376f, 0.960784376f, 0.960784376f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 Yellow               = { { { 1.000000000f, 1.000000000f, 0.000000000f, 1.000000000f } } };
//        XMGLOBALCONST XMVECTORF32 YellowGreen          = { { { 0.603921592f, 0.803921640f, 0.196078449f, 1.000000000f } } };
//	}
//
}

#endif