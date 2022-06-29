//////////////////////////////////////////////////////////////////////////////////
//              @file   a.cpp
///             @brief  a
///             @author Toide Yutaro
///             @date   -
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Rendering/Sprite/Include/Image.hpp"
#include "GameUtility/Base/Include/Screen.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace ui;
using namespace gm;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
static const UINT16 g_indices[] = { 0,1,2,2,1,3 };
Image::~Image()
{

}
void ui::Image::CreateInScreenSpace(const Float3& position, const Float2& rectSize, const Float2& u, const Float2& v, const Float4& color, float radian)
{
	CreateRect(Float3(position.x / Screen::GetScreenWidth(), position.y / Screen::GetScreenHeight(), position.z), Float2(rectSize.x / Screen::GetScreenWidth(), rectSize.y / Screen::GetScreenHeight()),
		color, u, v, radian);
}
void ui::Image::CreateInNDCSpace(const Float3& position, const Float2& rectSize, const Float2& u, const Float2& v, const Float4& color, float radian)
{
	CreateRect(position, rectSize,color, u, v, radian);
}
/****************************************************************************
*                       CreateRect
*************************************************************************//**
*  @fn        void Sprite::CreateRect(const Float3& position, const Float2& rectSize, const Float4& color)
*  @brief     Create Rectangle
*  @param[in] const Float3& position (rect center position)
*  @param[in] const Float2& rectSize ([0]: width, [1] height)
*  @param[in] const Float4& color    ( R, G, B, Alpha)
*  @return Å@Å@void
*****************************************************************************/
void Image::CreateRect(const Float3& position, const Float2& rectSize, const Float4& color, const Float2& u, const Float2& v, float radian)
{
	using Vertex = VertexPositionNormalColorTexture;

	/*-------------------------------------------------------------------
	-              Get rect info
	-------------------------t-------------------------------------------*/
	_size = rectSize;
	float w2 = rectSize.x / 2;
	float h2 = rectSize.y / 2;

	/*-------------------------------------------------------------------
	-              Get rotation info
	-------------------------t-------------------------------------------*/
	float fSin, fCos;
	DirectX::XMScalarSinCos(&fSin, &fCos, radian);

	/*-------------------------------------------------------------------
	-              Create RectPosition
	-------------------------t-------------------------------------------*/
	Float3 positions[4];
	positions[0] = Float3(position.x - w2 * fCos - h2 * fSin, position.y + w2 * fSin - h2 * fCos, position.z);
	positions[1] = Float3(position.x - w2 * fCos + h2 * fSin, position.y + w2 * fSin + h2 * fCos, position.z);
	positions[2] = Float3(position.x + w2 * fCos + h2 * fSin, position.y - w2 * fSin + h2 * fCos, position.z);
	positions[3] = Float3(position.x + w2 * fCos - h2 * fSin, position.y - w2 * fSin - h2 * fCos, position.z);

	/*-------------------------------------------------------------------
	-              Create Normal Vector
	---------------------------------------------------------------------*/
	Float3 faceEdge_0, faceEdge_1, faceNormal;
	faceEdge_0 = Vector3(positions[3] - positions[0]).ToFloat3();
	faceEdge_1 = Vector3(positions[1] - positions[0]).ToFloat3();
	faceNormal = Normalize(Cross(Vector3(faceEdge_0), Vector3(faceEdge_1))).ToFloat3();

	/*-------------------------------------------------------------------
	-              Create UV Vector
	---------------------------------------------------------------------*/
	Float2 uvs[] = { Float2(u.x, v.y), Float2(u.x, v.x), Float2(u.y, v.x), Float2(u.y,v.y) };

	/*-------------------------------------------------------------------
	-              Set Verteices
	---------------------------------------------------------------------*/
	for (int i = 0; i < 4; ++i)
	{
		_vertices[i] = Vertex(positions[i], faceNormal, color, uvs[i]);
	}
}