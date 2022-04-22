//////////////////////////////////////////////////////////////////////////////////
//              @file   Skybox.cpp
///             @brief  Skybox
///             @author Toide Yutaro
///             @date   2022/04/16
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Rendering/EnvironmentMap/Include/Skybox.hpp"
#include "GraphicsCore/Engine/Include/GraphicsCoreEngine.hpp"
#include "GraphicsCore/DirectX12/Core/Include/DirectX12PrimitiveGeometry.hpp"
#include "GraphicsCore/DirectX12/Core/Include/DirectX12Buffer.hpp"
#include "GraphicsCore/DirectX12/SimpleInclude/IncludeGraphicsPSO.hpp"
#include "GameCore/Core/Include/ResourceManager.hpp"
#include "GameUtility/Math/Include/GMMatrix.hpp"
#include <d3dcompiler.h>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gm;
namespace
{
	constexpr float SKY_SCALE = 5000.0f;
	RootSignature         s_RootSignature;
	GraphicsPipelineState s_PipelineState(L"Skybox::PipelineState");
}

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
void Skybox::Initialze(const std::wstring& texturePath, const std::wstring& addName)
{
	/*-------------------------------------------------------------------
	-            Set debug name
	---------------------------------------------------------------------*/
	std::wstring name = L""; if (addName != L"") { name += addName; name += L"::"; }
	name += L"Skybox::";
	/*-------------------------------------------------------------------
	-           Load Texture
	---------------------------------------------------------------------*/
	_texture = ResourceManager::Instance().LoadTexture(texturePath, TextureType::TextureCube);
	/*-------------------------------------------------------------------
	-           Prepare Pipeline
	---------------------------------------------------------------------*/
	PrepareVertexAndIndexBuffer(name);
	PrepareSkyObject(addName);
	PrepareRootSignature(addName);
	PreparePipelineState(addName);

}
void Skybox::Draw(Skybox::SceneGPUAddress scene) 
{
	auto& engine  = GraphicsCoreEngine::Instance();
	auto  context = engine.GetCommandContext();
	int currentFrame = engine.GetGraphicsDevice()->GetCurrentFrameIndex();
	/*-------------------------------------------------------------------
	-               Execute commandlist
	---------------------------------------------------------------------*/
	context->SetRootSignature(s_RootSignature.GetSignature());
	context->SetPipelineState(s_PipelineState.GetPipelineState());
	context->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->SetVertexBuffer(&_meshBuffer[currentFrame].VertexBufferView());
	context->SetIndexBuffer (&_meshBuffer[currentFrame].IndexBufferView());
	context->SetGraphicsConstantBufferView(0, scene);
	context->SetGraphicsConstantBufferView(1, _skyObject->GetGPUVirtualAddress());
	context->SetGraphicsShaderResourceView(2, _texture.Resource->GetGPUVirtualAddress());
	context->DrawIndexedInstanced(_meshBuffer[currentFrame].IndexBuffer->GetElementCount(),1);

}

void Skybox::Finalize()
{
	_meshBuffer.reset();
	_skyObject.reset();
}

#pragma region Private Function
/****************************************************************************
*							PrepareVertexBuffer
*************************************************************************//**
*  @fn        void Skybox::PrepareVertexAndIndexBuffer(const std::wstring& addName)
*  @brief     Prepare Sphere Vertex Buffer
*  @param[in] void
*  @return 　　bool
*****************************************************************************/
void Skybox::PrepareVertexAndIndexBuffer(const std::wstring& addName)
{
	/*-------------------------------------------------------------------
	-            Create Sphere Mesh
	---------------------------------------------------------------------*/
	MeshData sphereMesh = GeometryGenerator::Sphere(0.5f, 20, 20, false);
	/*-------------------------------------------------------------------
	-            Create Mesh Buffer
	---------------------------------------------------------------------*/
	auto& engine       = GraphicsCoreEngine::Instance();
	int totalFrameSize = engine.GetFrameBufferCount();
	_meshBuffer = std::make_unique<MeshBuffer[]>(totalFrameSize);
	for (int i = 0; i < totalFrameSize; ++i)
	{
		auto& meshBuffer = _meshBuffer[i];
		/*-------------------------------------------------------------------
		-            Calcurate Buffer Size
		---------------------------------------------------------------------*/
		auto vertexByteSize = sizeof(VertexPositionNormalTexture);
		auto indexByteSize  = sizeof(UINT16);
		auto vertexCount    = sphereMesh.Vertices.size();
		auto indexCount     = sphereMesh.Indices.size();
		/*-------------------------------------------------------------------
		-            Set Vertex Buffer and Index Buffer
		---------------------------------------------------------------------*/
		meshBuffer.VertexBuffer = std::make_unique<UploadBuffer>(engine.GetDevice(), vertexByteSize, indexByteSize, false, addName + L"VertexBuffer");
		meshBuffer.VertexBuffer->CopyStart();
		meshBuffer.VertexBuffer->CopyTotalData(sphereMesh.Vertices.data(), vertexCount);
		meshBuffer.VertexBuffer->CopyEnd();

		meshBuffer.IndexBuffer  = std::make_unique<UploadBuffer>(engine.GetDevice(), indexByteSize, indexCount, false, addName + L"IndexBuffer");
		meshBuffer.IndexBuffer->CopyStart();
		meshBuffer.IndexBuffer->CopyTotalData(sphereMesh.Indices.data(), indexCount);
		meshBuffer.IndexBuffer->CopyEnd();

	}
}
/****************************************************************************
*							PrepareSkyObject
*************************************************************************//**
*  @fn        void Skybox::PrepareSkyObject(const std::wstring& addName)
*  @brief     Build World Matrix Infomation
*  @param[in] std::wstring& addName
*  @return 　　void
*****************************************************************************/
void Skybox::PrepareSkyObject(const std::wstring& addName)
{
	auto& engine = GraphicsCoreEngine::Instance();
	auto skyObject = std::make_unique<UploadBuffer>(engine.GetDevice(), sizeof(gm::Matrix4), 1, true, addName + L"ObjectConstant");
	/*-------------------------------------------------------------------
	-			Set Skydata
	---------------------------------------------------------------------*/
	Matrix4 skyData; // sphere
	skyData = gm::Scaling(SKY_SCALE, SKY_SCALE, SKY_SCALE).ToFloat4x4();
	/*-------------------------------------------------------------------
	-			Copy Sky object data
	---------------------------------------------------------------------*/
	skyObject->CopyStart();
	skyObject->CopyData(0, &skyData);
	skyObject->CopyEnd();
	/*-------------------------------------------------------------------
	-			Move sky object data
	---------------------------------------------------------------------*/
	_skyObject = std::move(skyObject);

}
/****************************************************************************
*							PrepareRootSignature
*************************************************************************//**
*  @fn        void Skybox::PrepareRootSignature(const std::wstring& addName)
*  @brief     Build World Matrix Infomation
*  @param[in] std::wstring& addName
*  @return 　　void
*****************************************************************************/
void Skybox::PrepareRootSignature(const std::wstring& addName)
{
	s_RootSignature.Reset(3, 1);
	s_RootSignature.SetStaticSampler(GetStaticSampler(SamplerType::SamplerLinearWrap, 0));
	s_RootSignature[0].InitAsCBV(0); // Scene Constants
	s_RootSignature[1].InitAsCBV(1); // World Matrix
	s_RootSignature[2].InitAsSRV(0); // TextureCube Cubemap
}
/****************************************************************************
*							PreparePipelineState
*************************************************************************//**
*  @fn        void Skybox::PreparePipelineState(const std::wstring& addName)
*  @brief     Prepare pipelineState
*  @param[in] std::wstring& addName
*  @return 　　void
*****************************************************************************/
void Skybox::PreparePipelineState(const std::wstring& addName)
{
	RASTERIZER_DESC rasterizerState = RASTERIZER_DESC(D3D12_DEFAULT);
	rasterizerState.CullMode        = D3D12_CULL_MODE_NONE;

	s_PipelineState.SetRootSignature  (s_RootSignature);
	s_PipelineState.SetRasterizerState(rasterizerState);
	s_PipelineState.SetBlendState(GetBlendState(BlendStateType::OverWrite));
	s_PipelineState.SetInputLayouts(VertexPositionNormalTexture::InputLayout.NumElements, VertexPositionNormalTexture::InputLayout.pInputElementDescs);
	s_PipelineState.SetVertexShader(CompileShader(L"Shader\\EnvironmentMap\\ShaderSkybox.hlsl", L"VSMain", L"vs_6_3"));
	s_PipelineState.SetPixelShader (CompileShader(L"Shader\\EnvironmentMap\\ShaderSkybox.hlsl", L"PSMain", L"vs_6_3"));
	s_PipelineState.CompleteSetting(GraphicsCoreEngine::Instance().GetDevice());
}
#pragma endregion Private Function