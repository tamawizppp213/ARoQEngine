//////////////////////////////////////////////////////////////////////////////////
//              Title:  PrimitiveMesh.cpp
//            Content   2D or 3D Primitive Geometry
//             Author:  Toide Yutaro
//             Create:  2022_11_16
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Rendering/Model/Include/PrimitiveMesh.hpp"
#include "GameUtility/Math/Include/GMMathConstants.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gm;
using namespace gc::core;

//////////////////////////////////////////////////////////////////////////////////
//                            Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor
PrimitiveMesh::~PrimitiveMesh()
{
	Vertices.clear(); Vertices.shrink_to_fit();
	Indices.clear(); Indices.shrink_to_fit();
}
#pragma endregion Constructor and Destructor
/****************************************************************************
*							     Rect
****************************************************************************/
/* @struct         void PrimitiveMeshGenerator::Rect(float width, float height)
*  @brief      Create rect vertex and index data (Center is origin)
*  @param[in]  float width
*  @param[in]  float height
*  @param[in]  float depth
*  @return 　　 PrimitiveMesh
*****************************************************************************/
PrimitiveMesh PrimitiveMeshGenerator::Rect(float width, float height, float depth, const gm::Float4& color)
{
	PrimitiveMesh meshData;

	float w2 = width / 2;
	float h2 = height / 2;

	meshData.Vertices.resize(4);
	meshData.Indices.resize(6);

	meshData.Vertices[0] = Vertex(Float3(-w2, -h2, depth), Float3(0.0f, 0.0f, -1.0f), color, Float2(0.0f, 1.0f));
	meshData.Vertices[1] = Vertex(Float3(-w2, +h2, depth), Float3(0.0f, 0.0f, -1.0f), color, Float2(0.0f, 0.0f));
	meshData.Vertices[2] = Vertex(Float3(+w2, +h2, depth), Float3(0.0f, 0.0f, -1.0f), color, Float2(1.0f, 0.0f));
	meshData.Vertices[3] = Vertex(Float3(+w2, -h2, depth), Float3(0.0f, 0.0f, -1.0f), color, Float2(1.0f, 1.0f));

	meshData.Indices[0] = 0;
	meshData.Indices[1] = 1;
	meshData.Indices[2] = 2;

	meshData.Indices[3] = 0;
	meshData.Indices[4] = 2;
	meshData.Indices[5] = 3;
	return meshData;
}

/****************************************************************************
*							Quadrangle
****************************************************************************/
/* @struct         void PrimitiveMeshGenerator::Quadrangle(float x, float y, float w, float h, float depth)
*  @brief      Create Quadrangle vertex and index data
*  @param[in]  float x
*  @param[in]  float y
*  @param[in]  float z
*  @param[in]  float h
*  @param[in]  float depth
*  @return 　　 PrimitiveMesh
*****************************************************************************/
PrimitiveMesh PrimitiveMeshGenerator::Quadrangle(float x, float y, float w, float h, float depth, const Float4& color)
{
	PrimitiveMesh meshData;

	meshData.Vertices.resize(4);
	meshData.Indices.resize(6);

	meshData.Vertices[0] = Vertex(Float3(x, y - h, depth), Float3(0.0f, 0.0f, -1.0f), color, Float2(0.0f, 1.0f));
	meshData.Vertices[1] = Vertex(Float3(x, y, depth), Float3(0.0f, 0.0f, -1.0f), color, Float2(0.0f, 0.0f));
	meshData.Vertices[2] = Vertex(Float3(x + w, y, depth), Float3(0.0f, 0.0f, -1.0f), color, Float2(1.0f, 0.0f));
	meshData.Vertices[3] = Vertex(Float3(x + w, y - h, depth), Float3(0.0f, 0.0f, -1.0f), color, Float2(1.0f, 1.0f));

	meshData.Indices[0] = 0;
	meshData.Indices[1] = 1;
	meshData.Indices[2] = 2;

	meshData.Indices[3] = 0;
	meshData.Indices[4] = 2;
	meshData.Indices[5] = 3;

	return meshData;
}

/****************************************************************************
*							Box
****************************************************************************/
/* @struct         void PrimitiveMeshGenerator::Box(float width, float height, float depth, UINT32 numSubdivisions, bool isInvertNormal)
*  @brief      Create Box vertex and index data (Box center is origin)
*  @param[in]  float width
*  @param[in]  float height
*  @param[in]  float depth
*  @param[in]  UINT32 numSubdivisions
*  @param[in]  bool isInvertNormal
*  @return 　　 PrimitiveMesh
*****************************************************************************/
PrimitiveMesh PrimitiveMeshGenerator::Box(float width, float height, float depth, std::uint32_t numSubdivisions, bool isInvertNormal, const Float4& color)
{
	// set half width, height and depth
	constexpr int faceCount = 6;
	float w2 = 0.5f * width;
	float h2 = 0.5f * height;
	float d2 = 0.5f * depth;

	PrimitiveMesh meshData;
	Vertex   v[faceCount * 4];
	/*-------------------------------------------------------------------
	-					  Fill Vertex data
	---------------------------------------------------------------------*/
	// Fill in the front face vertex data.
	v[0] = Vertex(Float3(-w2, -h2, -d2), Float3(0.0f, 0.0f, -1.0f), color, Float2(0.0f, 1.0f));
	v[1] = Vertex(Float3(-w2, +h2, -d2), Float3(0.0f, 0.0f, -1.0f), color, Float2(0.0f, 0.0f));
	v[2] = Vertex(Float3(+w2, +h2, -d2), Float3(0.0f, 0.0f, -1.0f), color, Float2(1.0f, 0.0f));
	v[3] = Vertex(Float3(+w2, -h2, -d2), Float3(0.0f, 0.0f, -1.0f), color, Float2(1.0f, 1.0f));

	// Fill in the back face vertex data.
	v[4] = Vertex(Float3(-w2, -h2, +d2), Float3(0.0f, 0.0f, +1.0f), color, Float2(1.0f, 1.0f));
	v[5] = Vertex(Float3(+w2, -h2, +d2), Float3(0.0f, 0.0f, +1.0f), color, Float2(0.0f, 1.0f));
	v[6] = Vertex(Float3(+w2, +h2, +d2), Float3(0.0f, 0.0f, +1.0f), color, Float2(0.0f, 0.0f));
	v[7] = Vertex(Float3(-w2, +h2, +d2), Float3(0.0f, 0.0f, +1.0f), color, Float2(1.0f, 0.0f));

	// Fill in the top face vertex data.
	v[8] = Vertex(Float3(-w2, +h2, -d2), Float3(0.0f, +1.0f, 0.0f), color, Float2(0.0f, 1.0f));
	v[9] = Vertex(Float3(-w2, +h2, +d2), Float3(0.0f, +1.0f, 0.0f), color, Float2(0.0f, 0.0f));
	v[10] = Vertex(Float3(+w2, +h2, +d2), Float3(0.0f, +1.0f, 0.0f), color, Float2(1.0f, 0.0f));
	v[11] = Vertex(Float3(+w2, +h2, -d2), Float3(0.0f, +1.0f, 0.0f), color, Float2(1.0f, 1.0f));

	// Fill in the bottom face vertex data.
	v[12] = Vertex(Float3(-w2, -h2, -d2), Float3(0.0f, -1.0f, 0.0f), color, Float2(1.0f, 1.0f));
	v[13] = Vertex(Float3(+w2, -h2, -d2), Float3(0.0f, -1.0f, 0.0f), color, Float2(0.0f, 1.0f));
	v[14] = Vertex(Float3(+w2, -h2, +d2), Float3(0.0f, -1.0f, 0.0f), color, Float2(0.0f, 0.0f));
	v[15] = Vertex(Float3(-w2, -h2, +d2), Float3(0.0f, -1.0f, 0.0f), color, Float2(1.0f, 1.0f));

	// Fill in the left face vertex data.
	v[16] = Vertex(Float3(-w2, -h2, +d2), Float3(-1.0f, 0.0f, 0.0f), color, Float2(0.0f, 1.0f));
	v[17] = Vertex(Float3(-w2, +h2, +d2), Float3(-1.0f, 0.0f, 0.0f), color, Float2(0.0f, 0.0f));
	v[18] = Vertex(Float3(-w2, +h2, -d2), Float3(-1.0f, 0.0f, 0.0f), color, Float2(1.0f, 0.0f));
	v[19] = Vertex(Float3(-w2, -h2, -d2), Float3(-1.0f, 0.0f, 0.0f), color, Float2(1.0f, 1.0f));

	// Fill in the right face vertex data.
	v[20] = Vertex(Float3(+w2, -h2, -d2), Float3(+1.0f, 0.0f, 0.0f), color, Float2(0.0f, 1.0f));
	v[21] = Vertex(Float3(+w2, +h2, -d2), Float3(+1.0f, 0.0f, 0.0f), color, Float2(0.0f, 0.0f));
	v[22] = Vertex(Float3(+w2, +h2, +d2), Float3(+1.0f, 0.0f, 0.0f), color, Float2(1.0f, 0.0f));
	v[23] = Vertex(Float3(+w2, -h2, +d2), Float3(+1.0f, 0.0f, 0.0f), color, Float2(1.0f, 1.0f));

	meshData.Vertices.assign(&v[0], &v[24]);
	/*-------------------------------------------------------------------
	-					     Set index data
	---------------------------------------------------------------------*/
	std::uint16_t index[faceCount * 6] = { 0 };
	// Fill front-> back -> top -> bottom -> left -> right
	for (std::uint16_t i = 0; i < faceCount; ++i)
	{
		index[6 * i + 0] = 4 * i + 0;
		index[6 * i + 1] = 4 * i + 1;
		index[6 * i + 2] = 4 * i + 2;
		index[6 * i + 3] = 4 * i + 0;
		index[6 * i + 4] = 4 * i + 2;
		index[6 * i + 5] = 4 * i + 3;
	}

	meshData.Indices.assign(&index[0], &index[faceCount * 6]);

	/*-------------------------------------------------------------------
	-					     SubDivide
	---------------------------------------------------------------------*/
	numSubdivisions = std::min<std::uint16_t>(static_cast<std::int16_t>(numSubdivisions), 6u);
	for (std::uint16_t i = 0; i < numSubdivisions; ++i)
	{
		SubDivide(meshData);
	}

	/*-------------------------------------------------------------------
	-					  Invert Normal Vector
	---------------------------------------------------------------------*/
	if (isInvertNormal)
	{
		IsInvertNormal(meshData);
	}

	return meshData;
}

/****************************************************************************
*							Sphere
****************************************************************************/
/* @struct         void PrimitiveMeshGenerator::Sphere(float radius, UINT32 sliceCount, UINT32 stackCount, bool isInvertNormal)
*  @brief      Create Sphere vertex and index data (Sphere center is origin)
*  @param[in]  float radius
*  @param[in]  UINT32 sliceCount
*  @param[in]  UINT32 stackCount
*  @param[in]  bool isInvertNormal
*  @return 　　 PrimitiveMesh
*****************************************************************************/
PrimitiveMesh PrimitiveMeshGenerator::Sphere(float radius, std::uint32_t sliceCount, std::uint32_t stackCount, bool isInvertNormal, const Float4& color)
{
	PrimitiveMesh meshData;

	Vertex topVertex = Vertex(Float3(0.0f, +radius, 0.0f), Float3(0.0f, +1.0f, 0.0f), color, Float2(0.0f, 0.0f));
	Vertex bottomVertex = Vertex(Float3(0.0f, -radius, 0.0f), Float3(0.0f, -1.0f, 0.0f), color, Float2(0.0f, 1.0f));

	/*-------------------------------------------------------------------
	-					  Fill Vertex data
	---------------------------------------------------------------------*/
	meshData.Vertices.push_back(topVertex);

	float phiStep = gm::GM_PI_FLOAT / stackCount;
	float thetaStep = 2.0f * gm::GM_PI_FLOAT / sliceCount;

	for (std::uint32_t i = 1; i <= stackCount - 1; ++i)
	{
		float phi = i * phiStep;

		// Verteies of ring
		for (std::uint16_t j = 0; j <= sliceCount; ++j)
		{
			float theta = j * thetaStep;

			Vertex vertex;

			/*-------------------------------------------------------------------
			-					Spherical to cartesian
			---------------------------------------------------------------------*/
			vertex.Position.x = radius * sinf(phi) * cosf(theta);
			vertex.Position.y = radius * cosf(phi);
			vertex.Position.z = radius * sinf(phi) * sinf(theta);

			Vector3f pos = Vector3f(vertex.Position);
			vertex.Normal = Normalize(pos).ToFloat3();

			vertex.UV.x = theta / gm::GM_2PI_FLOAT;
			vertex.UV.y = phi / gm::GM_PI_FLOAT;

			meshData.Vertices.push_back(vertex);
		}
	}

	meshData.Vertices.push_back(bottomVertex);

	/*-------------------------------------------------------------------
	-					    Set index data
	---------------------------------------------------------------------*/
	for (std::uint16_t i = 1; i <= sliceCount; ++i)
	{
		meshData.Indices.push_back(0);
		meshData.Indices.push_back(i + 1);
		meshData.Indices.push_back(i);
	}

	std::uint16_t baseIndex = 1;
	std::uint16_t ringVertexCount = static_cast<std::uint16_t>(sliceCount) + 1;
	for (std::uint16_t i = 0; i < stackCount - 2; ++i)
	{
		for (std::uint16_t j = 0; j < sliceCount; ++j)
		{
			meshData.Indices.push_back(baseIndex + i * ringVertexCount + j);
			meshData.Indices.push_back(baseIndex + i * ringVertexCount + j + 1);
			meshData.Indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);

			meshData.Indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);
			meshData.Indices.push_back(baseIndex + i * ringVertexCount + j + 1);
			meshData.Indices.push_back(baseIndex + (i + 1) * ringVertexCount + j + 1);
		}
	}

	// South pole vertex was added last.
	std::uint16_t southPoleIndex = (std::uint16_t)meshData.Vertices.size() - 1;

	// Offset the indices to the index of the first vertex in the last ring.
	baseIndex = southPoleIndex - ringVertexCount;

	for (std::uint16_t i = 0; i < sliceCount; ++i)
	{
		meshData.Indices.push_back(southPoleIndex);
		meshData.Indices.push_back(baseIndex + i);
		meshData.Indices.push_back(baseIndex + i + 1);
	}

	if (isInvertNormal)
	{
		IsInvertNormal(meshData);
	}
	return meshData;
}

/****************************************************************************
*							GeoSphere
****************************************************************************/
/* @struct     void PrimitiveMeshGenerator::GeoSphere(float radius, UINT32 numSubdivisions)
*  @brief      Create geoSphere vertex and index data (GeoSphere center is origin)
*  @param[in]  float radius
*  @param[in]  UINT32 numSubdivisions
*  @param[in]  bool isInvertNormal
*  @return 　　 PrimitiveMesh
*****************************************************************************/
PrimitiveMesh PrimitiveMeshGenerator::GeoSphere(float radius, std::uint32_t numSubdivisions, bool isInvertNormal, const Float4& color)
{
	PrimitiveMesh meshData;

	numSubdivisions = std::min<std::uint16_t>(static_cast<std::uint16_t>(numSubdivisions), 6u);;

	// Approximate a sphere by tessellating an icosahedron
	const float X = 0.525731f;
	const float Z = 0.850651f;

	Float3 pos[12] =
	{
		Float3(-X, 0.0f, Z),  Float3(X, 0.0f, Z),
		Float3(-X, 0.0f, -Z), Float3(X, 0.0f, -Z),
		Float3(0.0f, Z, X),   Float3(0.0f, Z, -X),
		Float3(0.0f, -Z, X),  Float3(0.0f, -Z, -X),
		Float3(Z, X, 0.0f),   Float3(-Z, X, 0.0f),
		Float3(Z, -X, 0.0f),  Float3(-Z, -X, 0.0f)
	};

	std::uint16_t k[60] =
	{
		1,4,0,  4,9,0,  4,5,9,  8,5,4,  1,8,4,
		1,10,8, 10,3,8, 8,3,5,  3,2,5,  3,7,2,
		3,10,7, 10,6,7, 6,11,7, 6,0,11, 6,1,0,
		10,1,6, 11,0,9, 2,11,9, 5,2,9,  11,2,7
	};

	meshData.Vertices.resize(12);
	meshData.Indices.assign(&k[0], &k[60]);

	for (std::uint16_t i = 0; i < 12; ++i)
	{
		meshData.Vertices[i].Position = pos[i];
	}

	for (std::uint16_t i = 0; i < numSubdivisions; ++i)
	{
		SubDivide(meshData);
	}

	for (std::uint16_t i = 0; i < meshData.Vertices.size(); ++i)
	{
		// Project onto unit sphere.
		Vector3f n = Normalize(Vector3f(meshData.Vertices[i].Position));
		Vector3f p = radius * n;

		meshData.Vertices[i].Position = p.ToFloat3();
		meshData.Vertices[i].Normal = n.ToFloat3();

		//Derive texture coordinates from spherical coordinates.
		float theta = atan2f(meshData.Vertices[i].Position.z, meshData.Vertices[i].Position.x);

		if (theta < 0.0f)
		{
			theta += gm::GM_2PI_FLOAT;
		}

		float phi = acosf(meshData.Vertices[i].Position.y / radius);

		meshData.Vertices[i].UV.x = theta / gm::GM_2PI_FLOAT;
		meshData.Vertices[i].UV.y = phi / gm::GM_PI_FLOAT;
		meshData.Vertices[i].Color = color;
	}

	if (isInvertNormal)
	{
		IsInvertNormal(meshData);
	}
	return meshData;
}

/****************************************************************************
*							Cylinder
****************************************************************************/
/* @struct         void PrimitiveMeshGenerator::Cylinder(float bottomRadius, float topRadius, float height, UINT32 sliceCount, UINT32 stackCount, bool isInvertNormal)
*  @brief      Create Cylinder vertex and index data (Cylinder center is origin)
*  @param[in]  float bottomRadius
*  @param[in]  float topRadius
*  @param[in]  float height
*  @param[in]  UINT32 sliceCount
*  @param[in]  UINT32 stackCount
*  @param[in]  bool isInvertNormal
*  @return 　　 PrimitiveMesh
*****************************************************************************/
PrimitiveMesh PrimitiveMeshGenerator::Cylinder(float bottomRadius, float topRadius, float height, std::uint32_t sliceCount, std::uint32_t stackCount, bool isInvertNormal, const Float4& color)
{
	PrimitiveMesh meshData;

	float stackHeight = height / stackCount;
	float radiusStep = (topRadius - bottomRadius) / stackCount;
	std::uint32_t ringCount = stackCount + 1;

	// Compute vertices for each stack ring starting at the bottom and moving up.
	for (std::uint32_t i = 0; i < ringCount; ++i)
	{
		float y = -0.5f * height + i * stackHeight;
		float r = bottomRadius + i * radiusStep;

		// vertices of ring
		float dTheta = 2.0f * gm::GM_PI_FLOAT / sliceCount;
		for (std::uint32_t j = 0; j <= sliceCount; ++j)
		{
			Vertex vertex;

			float c = cosf(j * dTheta);
			float s = sinf(j * dTheta);

			vertex.Position = Float3(r * c, y, r * s);
			vertex.UV.x = (float)j / sliceCount;
			vertex.UV.y = 1.0f - (float)i / stackCount;

			float dr = bottomRadius - topRadius;
			Float3 bitangent(dr * c, -height, dr * s);

			Float3  t = Float3(-s, 0.0f, c);
			Vector3f T = Vector3f(t);
			Vector3f B = Vector3f(bitangent);
			Vector3f N = Normalize(Cross(T, B));
			vertex.Normal = N.ToFloat3();

			meshData.Vertices.push_back(vertex);
		}
	}

	std::uint32_t ringVertexCount = static_cast<std::uint32_t>(sliceCount) + 1;

	// Compute indices for each stack.
	for (std::uint32_t i = 0; i < stackCount; ++i)
	{
		for (std::uint32_t j = 0; j < sliceCount; ++j)
		{
			meshData.Indices.push_back(i * ringVertexCount + j);
			meshData.Indices.push_back((i + 1) * ringVertexCount + j);
			meshData.Indices.push_back((i + 1) * ringVertexCount + j + 1);

			meshData.Indices.push_back(i * ringVertexCount + j);
			meshData.Indices.push_back((i + 1) * ringVertexCount + j + 1);
			meshData.Indices.push_back(i * ringVertexCount + j + 1);
		}
	}

	BuildCylinderTopCap(topRadius, height, sliceCount, meshData, color);
	BuildCylinderBottomCap(bottomRadius, height, sliceCount, meshData, color);

	if (isInvertNormal)
	{
		IsInvertNormal(meshData);
	}

	return meshData;
}

/****************************************************************************
*							Grid
****************************************************************************/
/* @struct         void PrimitiveMeshGenerator::Grid(float width, float depth, UINT32 rows, UINT32 columns, bool isInvertNormal)
*  @brief      Create grid vertex and index data (Grid center is origin)
*  @param[in]  float width
*  @param[in]  float depth
*  @param[in]  UINT32 rows
*  @param[in]  UINT32 columns
*  @param[in]  bool isInvertNormal
*  @return 　　 PrimitiveMesh
*****************************************************************************/
PrimitiveMesh PrimitiveMeshGenerator::Grid(float width, float depth, std::uint32_t rows, std::uint32_t columns, bool isInvertNormal, const Float4& color)
{
	PrimitiveMesh meshData;

	std::uint64_t vertexCount = (std::uint64_t)rows * columns;
	std::uint64_t faceCount = ((std::uint64_t)rows - 1) * (std::uint64_t)(columns - 1) * 2;

	/*-------------------------------------------------------------------
	-					  Fill Vertex data
	---------------------------------------------------------------------*/
	float halfWidth = 0.5f * width;
	float halfDepth = 0.5f * depth;

	float dx = width / (columns - 1);
	float dz = depth / (rows - 1);

	float du = 1.0f / (columns - 1);
	float dv = 1.0f / (rows - 1);

	meshData.Vertices.resize(vertexCount);
	for (std::uint16_t i = 0; i < rows; ++i)
	{
		float z = halfDepth - i * dz;
		for (std::uint16_t j = 0; j < columns; ++j)
		{
			float x = -halfWidth + j * dx;

			meshData.Vertices[(std::uint64_t)i * columns + j].Position = Float3(x, 0.0f, z);
			meshData.Vertices[(std::uint64_t)i * columns + j].Normal = Float3(0.0f, 1.0f, 0.0f);

			meshData.Vertices[(std::uint64_t)i * columns + j].UV.x = j * du;
			meshData.Vertices[(std::uint64_t)i * columns + j].UV.y = i * dv;

			meshData.Vertices[(std::uint64_t)i * columns + j].Color = color;
		}
	}

	meshData.Indices.resize((std::uint64_t)faceCount * 3);

	/*-------------------------------------------------------------------
	-					    Set Index data
	---------------------------------------------------------------------*/
	std::uint16_t k = 0;
	for (std::uint16_t i = 0; i < rows - 1; ++i)
	{
		for (std::uint16_t j = 0; j < columns - 1; ++j)
		{
			meshData.Indices[(std::uint64_t)k] = (i) * (std::uint16_t)columns + (j);
			meshData.Indices[(std::uint64_t)k + 1] = (i) * (std::uint16_t)columns + (j + 1);
			meshData.Indices[(std::uint64_t)k + 2] = (i + 1) * (std::uint16_t)columns + (j);

			meshData.Indices[(std::uint64_t)k + 3] = (i + 1) * (std::uint16_t)columns + (j);
			meshData.Indices[(std::uint64_t)k + 4] = (i) * (std::uint16_t)columns + (j + 1);
			meshData.Indices[(std::uint64_t)k + 5] = (i + 1) * (std::uint16_t)columns + (j + 1);

			k += 6;
		}
	}

	if (isInvertNormal)
	{
		IsInvertNormal(meshData);
	}

	return meshData;
}

PrimitiveMesh PrimitiveMeshGenerator::Torus(float majorRadius, float minorRadius, std::uint32_t numMajor, std::uint32_t numMinor, const Float4& color)
{
	if (majorRadius < minorRadius)
	{
		std::swap(majorRadius, minorRadius);
	}

	PrimitiveMesh meshData;

	const float majorStep = 2.0f * gm::GM_PI_FLOAT / numMajor;
	const float minorStep = 2.0f * gm::GM_PI_FLOAT / numMinor;
	float majorPosition = 0.0f;

	for (std::uint32_t i = 0; i < numMajor; ++i)
	{
		const float cosMajor = cosf(majorPosition);
        const float sinMajor = sinf(majorPosition);
        float minorPosition = 0.0f;

        for (std::uint32_t j = 0; j < numMinor; ++j) {
            float cosMinor = cosf(minorPosition);
            float sinMinor = sinf(minorPosition);

			const Float3 position = 
			{
				(majorRadius + minorRadius * cosMinor) * cosMajor,
				(majorRadius + minorRadius * cosMinor) * sinMajor,
				minorRadius * sinMinor
			};

			const Float3 normal = { cosMinor * cosMajor, cosMinor * sinMajor, sinMinor };
			const Float2 uv     = { (float)i / numMajor , (float)j / numMinor };
	
			meshData.Vertices.push_back(Vertex(position, normal, color, uv));

            minorPosition += minorStep;
        }

        majorPosition += majorStep; 
	}

	for (std::uint32_t i = 0; i < numMajor; ++i)
	{
		const auto i1 = i * numMinor;
		const auto i2 = (i + 1) % numMajor * numMinor;

		for (std::uint32_t j = 0; j < numMinor; ++j)
		{
			const auto j1 = j;
			const auto j2 = (j + 1) % numMinor;

			meshData.Indices.push_back(i1 + j1);
			meshData.Indices.push_back(i2 + j1);
			meshData.Indices.push_back(i1 + j2);

			meshData.Indices.push_back(i1 + j2);
			meshData.Indices.push_back(i2 + j1);
			meshData.Indices.push_back(i2 + j2);
		}
	}

	return meshData;
}

#pragma region Private Function
/****************************************************************************
*							IsInvertNormal
****************************************************************************/
/* @struct         void PrimitiveMeshGenerator::IsInvertNormal(PrimitiveMesh& meshData)
*  @brief      Invert meshData normal vector
*  @param[out] MeshData& meshData
*  @return 　　 void
*****************************************************************************/
void PrimitiveMeshGenerator::IsInvertNormal(PrimitiveMesh& meshData)
{
	for (auto it = meshData.Vertices.begin(); it != meshData.Vertices.end(); ++it)
	{
		it->Normal.x = -it->Normal.x;
		it->Normal.y = -it->Normal.y;
		it->Normal.z = -it->Normal.z;
	}
}

/****************************************************************************
*							SubDivide
****************************************************************************/
/* @struct         void PrimitiveMeshGenerator::SubDivide(MeshData& meshData)
*  @brief      Divide each mesh
*  @param[out] PrimitiveMesh& meshData
*  @return 　　 void
*****************************************************************************/
void PrimitiveMeshGenerator::SubDivide(PrimitiveMesh& meshData)
{
	//using Vertex = VertexPositionNormalTexture; 

	// Save a copy of the input geometry
	PrimitiveMesh inputCopy = meshData;

	meshData.Vertices.resize(0);
	meshData.Indices.resize(0);

	/*-------------------------------------------------------------------
	-					 SubDivide Figure
	---------------------------------------------------------------------*/
	//       v1
	//       *
	//      / \
	//     /   \
	//  m0*-----*m1
	//   / \   / \
	//  /   \ /   \
	// *-----*-----*
	// v0    m2     v2
	/*-------------------------------------------------------------------*/

	/*-------------------------------------------------------------------
	-						SubDivide
	---------------------------------------------------------------------*/
	std::uint16_t numTris = (std::uint16_t)inputCopy.Indices.size() / 3;
	for (std::uint16_t i = 0; i < numTris; ++i)
	{
		/*-------------------------------------------------------------------
		-						Set Vertex
		---------------------------------------------------------------------*/
		Vertex vertex0 = inputCopy.Vertices[inputCopy.Indices[(std::uint64_t)i * 3 + 0]];
		Vertex vertex1 = inputCopy.Vertices[inputCopy.Indices[(std::uint64_t)i * 3 + 1]];
		Vertex vertex2 = inputCopy.Vertices[inputCopy.Indices[(std::uint64_t)i * 3 + 2]];

		/*-------------------------------------------------------------------
		-						Set MidPoint
		---------------------------------------------------------------------*/
		Vertex mid0 = MidPoint(vertex0, vertex1);
		Vertex mid1 = MidPoint(vertex1, vertex2);
		Vertex mid2 = MidPoint(vertex0, vertex2);

		/*-------------------------------------------------------------------
		-						Add new geometry
		---------------------------------------------------------------------*/
		// vertex
		meshData.Vertices.push_back(vertex0); // index: 0
		meshData.Vertices.push_back(vertex1); // index: 1
		meshData.Vertices.push_back(vertex2); // index: 2
		meshData.Vertices.push_back(mid0);    // index: 3
		meshData.Vertices.push_back(mid1);    // index: 4
		meshData.Vertices.push_back(mid2);    // index: 5 

		// index
		meshData.Indices.push_back(i * 6 + 0);
		meshData.Indices.push_back(i * 6 + 3);
		meshData.Indices.push_back(i * 6 + 5);

		meshData.Indices.push_back(i * 6 + 3);
		meshData.Indices.push_back(i * 6 + 4);
		meshData.Indices.push_back(i * 6 + 5);

		meshData.Indices.push_back(i * 6 + 5);
		meshData.Indices.push_back(i * 6 + 4);
		meshData.Indices.push_back(i * 6 + 2);

		meshData.Indices.push_back(i * 6 + 3);
		meshData.Indices.push_back(i * 6 + 1);
		meshData.Indices.push_back(i * 6 + 4);

	}
}

gm::Vertex PrimitiveMeshGenerator::MidPoint(const Vertex& v0, const Vertex& v1)
{
	/*-------------------------------------------------------------------
	-						Load vertex data
	---------------------------------------------------------------------*/
	Vector3f p0 = Vector3f(v0.Position);
	Vector3f p1 = Vector3f(v1.Position);
	Vector3f n0 = Vector3f(v0.Normal);
	Vector3f n1 = Vector3f(v1.Normal);
	Vector2f t0 = Vector2f(v0.UV);
	Vector2f t1 = Vector2f(v1.UV);
	Vector4f c0 = Vector4f(v0.Color);
	Vector4f c1 = Vector4f(v1.Color);


	/*-------------------------------------------------------------------
	-						Calcurate mid point
	---------------------------------------------------------------------*/
	Vector3f pos = 0.5f * (p0 + p1);
	Vector3f normal = Normalize(0.5f * (n0 + n1));
	Vector2f uv = 0.5f * (t0 + t1);
	Vector4f color = 0.5f * (c0 + c1);

	/*-------------------------------------------------------------------
	-						Set vertex data
	---------------------------------------------------------------------*/
	Vertex vertex;
	vertex.Position = pos.ToFloat3();
	vertex.Normal = normal.ToFloat3();
	vertex.UV = uv.ToFloat2();
	vertex.Color = color.ToFloat4();
	return vertex;
}

/****************************************************************************
*							BuildCylinderTopCap
****************************************************************************/
/* @struct         void GeometryGenerator::BuildCylinderTopCap(float bottomRadius, float topRadius, float height, UINT32 sliceCount, UINT32 stackCount, MeshData& meshData)
*  @brief      Create Cylinder top cap vertex and index data (GeoSphere center is origin)
*  @param[in]  float bottomRadius,
*  @param[in]  float topRadius,
*  @param[in]  float height,
*  @param[in]  UINT32 sliceCount,
*  @param[in]  UINT32 stackCount,
*  @param[out] MeshData& meshData,
*  @return 　　 void
*****************************************************************************/
void PrimitiveMeshGenerator::BuildCylinderTopCap(float topRadius, float height, std::uint32_t sliceCount, PrimitiveMesh& meshData, const Float4& color)
{
	std::uint16_t baseIndex = (std::uint16_t)meshData.Vertices.size();

	float y = 0.5f * height;
	float dTheta = 2.0f * gm::GM_PI_FLOAT / sliceCount;

	// Duplicate cap ring vertices because the texture coordinates and normals differ.
	for (std::uint16_t i = 0; i <= sliceCount; ++i)
	{
		float x = topRadius * cosf(i * dTheta);
		float z = topRadius * sinf(i * dTheta);

		float u = x / height + 0.5f;
		float v = z / height + 0.5f;

		meshData.Vertices.push_back(Vertex(Float3(x, y, z), Float3(0.0f, 1.0f, 0.0f), color, Float2(u, v)));

	}
	meshData.Vertices.push_back(Vertex(Float3(0.0f, y, 0.0f), Float3(0.0f, 1.0f, 0.0f), color, Float2(0.5f, 0.5f)));

	std::uint16_t centerIndex = (std::uint16_t)meshData.Vertices.size() - 1;

	for (std::uint16_t i = 0; i < sliceCount; ++i)
	{
		meshData.Indices.push_back(centerIndex);
		meshData.Indices.push_back(baseIndex + i + 1);
		meshData.Indices.push_back(baseIndex + i);
	}
}

/****************************************************************************
*							BuildCylinderBottomCap
****************************************************************************/
/* @struct         void GeometryGenerator::BuildCylinderBottomCap(float bottomRadius, float topRadius, float height, UINT32 sliceCount, UINT32 stackCount, MeshData& meshData)
*  @brief      Create Cylinder bottom cap vertex and index data (GeoSphere center is origin)
*  @param[in]  float bottomRadius,
*  @param[in]  float topRadius,
*  @param[in]  float height,
*  @param[in]  UINT32 sliceCount,
*  @param[in]  UINT32 stackCount,
*  @param[out] MeshData& meshData,
*  @return 　　 void
*****************************************************************************/
void PrimitiveMeshGenerator::BuildCylinderBottomCap(float bottomRadius, float height, std::uint32_t sliceCount, PrimitiveMesh& meshData, const Float4& color)
{
	std::uint16_t baseIndex = (std::uint16_t)meshData.Vertices.size();

	float y = -0.5f * height;
	float dTheta = 2.0f * gm::GM_PI_FLOAT / sliceCount;

	// Duplicate cap ring vertices because the texture coordinates and normals differ.
	for (std::uint16_t i = 0; i <= sliceCount; ++i)
	{
		float x = bottomRadius * cosf(i * dTheta);
		float z = bottomRadius * sinf(i * dTheta);

		float u = x / height + 0.5f;
		float v = z / height + 0.5f;

		meshData.Vertices.push_back(Vertex(Float3(x, y, z), Float3(0.0f, -1.0f, 0.0f), color, Float2(u, v)));

	}
	meshData.Vertices.push_back(Vertex(Float3(0.0f, y, 0.0f), Float3(0.0f, -1.0f, 0.0f), color, Float2(0.5f, 0.5f)));

	std::uint16_t centerIndex = (std::uint16_t)meshData.Vertices.size() - 1;

	for (std::uint16_t i = 0; i < sliceCount; ++i)
	{
		meshData.Indices.push_back(centerIndex);
		meshData.Indices.push_back(baseIndex + i);
		meshData.Indices.push_back(baseIndex + i + 1);
	}
}

#pragma endregion Private Function

