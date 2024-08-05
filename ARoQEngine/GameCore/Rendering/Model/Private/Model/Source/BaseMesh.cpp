//////////////////////////////////////////////////////////////////////////////////
///  @file   BaseMesh.cpp
///  @brief  ���b�V���̊��N���X�ł�. GPU�o�b�t�@�̍쐬, ���b�V������肷�邽�߂̋��ʏ�����񋟂��܂�.
///  @author toide
///  @date   2024/07/28 2:10:58
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Rendering/Model/Private/Model/Include/BaseMesh.hpp"
#include "GraphicsCore/Engine/Include/LowLevelGraphicsEngine.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUBuffer.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace engine;
using namespace gu;
using namespace rhi::core;

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor
IBaseMesh::IBaseMesh(const gu::SharedPointer<LowLevelGraphicsEngine>& engine, const BaseMeshSetting& setting, const gu::tstring& name)
	: _engine(engine)
	, _baseMeshSetting(setting), 
	_name(name)
{

}

#pragma endregion Constructor and Destructor

#pragma region Public Function

#pragma endregion Public Function

#pragma region Public Property
/*!**********************************************************************
*  @brief     ���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@���쐬���܂�.
*  @param[in] const gu::uint32 VertexBuffer��Stride (1���_�ɕK�v�ȃo�C�g�T�C�Y)
*  @param[in] const gu::uint32 VertexBuffer�̒��_��
*  @param[in] const gu::uint32 IndexBuffer�̌�
*  @param[in] const void* VertexBuffer�ɏ������ݎn�߂�擪�|�C���^
*  @param[in] const void* IndexBuffer�������ݎn�߂�擪�|�C���^
*  @return    void
*************************************************************************/
void IBaseMesh::SetupVertexBufferAndIndexBuffer(const gu::uint32 vbStride, const gu::uint32 vbCount, const gu::uint32 ibCount,
	const void* vbData, const void* ibData)
{
	Checkf(_engine, "Engine is nullptr");
	Checkf(_engine->GetDevice(), "Device is nullptr");
	
	/*-------------------------------------------------------------------
	-            �o�b�t�@�̃��T�C�Y�@
	---------------------------------------------------------------------*/
	if (_baseMeshSetting.IsCPUAccessibleVB)
	{
		_vertexBuffer.Resize(_engine->FRAME_BUFFER_COUNT);
	}
	else
	{
		_vertexBuffer.Resize(1);
	}

	if (_baseMeshSetting.IsCPUAccessibleIB)
	{
		_indexBuffer.Resize(_engine->FRAME_BUFFER_COUNT);
	}
	else
	{
		_indexBuffer.Resize(1);
	}

	/*-------------------------------------------------------------------
	-            ���_�o�b�t�@�̍쐬
	---------------------------------------------------------------------*/
	const auto device = _engine->GetDevice();
	for (uint32 i = 0; i < _vertexBuffer.Size(); ++i)
	{
		const auto metaData = GPUBufferMetaData::VertexBuffer(vbStride, vbCount, 
			_baseMeshSetting.IsCPUAccessibleVB ? MemoryHeap::Upload : MemoryHeap::Default);

		_vertexBuffer[i] = device->CreateBuffer(metaData);
		_vertexBuffer[i]->SetName(_name + SP("::VB"));
		_vertexBuffer[i]->UploadByte(vbData, metaData.GetTotalByte(), 0, _engine->GetCommandList(rhi::core::CommandListType::Graphics));
	}

	/*-------------------------------------------------------------------
	-            �C���f�b�N�X�o�b�t�@�̍쐬
	---------------------------------------------------------------------*/
	const uint32 indexByteSize = static_cast<uint32>(_baseMeshSetting.Is32BitIndex ? sizeof(uint32) : sizeof(uint16));
	for (uint32 i = 0; i < (uint32)_indexBuffer.Size(); ++i)
	{
		const auto metaData = GPUBufferMetaData::IndexBuffer(indexByteSize, ibCount,
			_baseMeshSetting.IsCPUAccessibleIB ? MemoryHeap::Upload : MemoryHeap::Default, ResourceState::Common, nullptr);

		_indexBuffer[i] = device->CreateBuffer(metaData);
		_indexBuffer[i]->SetName(_name + SP("::IB"));
		_indexBuffer[i]->UploadByte(ibData, metaData.GetTotalByte(), 0, _engine->GetCommandList(rhi::core::CommandListType::Graphics));
	}
}

/*!**********************************************************************
*  @brief     ���_�o�b�t�@���擾���܂�. CPU�A�N�Z�X�\�̏ꍇ�͌��������݃t���[���̃o�b�t�@��Ԃ��܂�.
*  @param[in] void
*  @return    GPUBufferPtr
*************************************************************************/
GPUBufferPtr IBaseMesh::GetVertexBuffer() const noexcept
{
	if (_baseMeshSetting.IsCPUAccessibleVB)
	{
		const auto frameIndex = _engine->GetCurrentFrameIndex();
		return _vertexBuffer[frameIndex];
	}
	else
	{
		return _vertexBuffer[0];
	}
}

/*!**********************************************************************
*  @brief     �C���f�b�N�X�o�b�t�@���擾���܂�. CPU�A�N�Z�X�\�̏ꍇ�͌��������݃t���[���̃o�b�t�@��Ԃ��܂�.
*  @param[in] void
*  @return    GPUBufferPtr
*************************************************************************/
GPUBufferPtr IBaseMesh::GetIndexBuffer() const noexcept
{
	if(_baseMeshSetting.IsCPUAccessibleIB)
	{
		const auto frameIndex = _engine->GetCurrentFrameIndex();
		return _indexBuffer[frameIndex];
	}
	else
	{
		return _indexBuffer[0];
	}
}
#pragma endregion Public Property