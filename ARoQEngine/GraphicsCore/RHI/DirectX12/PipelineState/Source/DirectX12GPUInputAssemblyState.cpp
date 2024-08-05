//////////////////////////////////////////////////////////////////////////////////
//              @file   DirectX12GPUInputAssemblyState.cpp
///             @brief  Input assembly
///             @author Toide Yutaro
///             @date   2022_07_01
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/DirectX12/PipelineState/Include/DirectX12GPUInputAssemblyState.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12EnumConverter.hpp"
#include "GameUtility/Container/Include/GUPair.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi;
using namespace rhi::directX12;
using namespace gu;
//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
rhi::directX12::GPUInputAssemblyState::GPUInputAssemblyState(
	const gu::SharedPointer<core::RHIDevice>& device, 
	const gu::DynamicArray<core::InputLayoutElement>& elements,
	const core::PrimitiveTopology primitiveTopology
) : core::GPUInputAssemblyState(device, elements, primitiveTopology)
{
	/*-------------------------------------------------------------------
	-         ���̓��C�A�E�g�̎��O����
	---------------------------------------------------------------------*/
	_inputLayoutElements.Resize(_elements.Size());

	// Slot����, �e���̓f�[�^��A�����Ĕz�u�����邽�߂�offset�o�C�g���w�肵�܂�. 
	auto offsetInBytes = gu::DynamicArray<uint32>(_slotCount); // slot���Ƃ�Offset

	// ���̓��C�A�E�g�̕��� (���_���Ƃ��C���X�^���X���Ƃ�)�̔z��ł�. 
	// gu::Pair��Key�����ɏ������ς݂̓��e���ǂ���, Value�����ނ���v���邩���m�F���邽�߂Ɏg�p���܂�. 
	gu::DynamicArray<gu::Pair<bool, core::InputClassification>> inputClassifications
	(
		_slotCount,
		gu::Pair<bool, core::InputClassification>(false, core::InputClassification::PerVertex)
	);

	/*-------------------------------------------------------------------
	-         ���̓��C�A�E�g�ݒ�
	---------------------------------------------------------------------*/
	for (uint32 i = 0; i < _elements.Size(); ++i)
	{
		const auto& element = _elements[i];

		_inputLayoutElements[i] = D3D12_INPUT_ELEMENT_DESC
			{
				.SemanticName         = element.SemanticName.CString(),
				.SemanticIndex        = element.SemanticIndex,
				.Format               = (DXGI_FORMAT)core::PixelFormatInfo::GetConst(element.Format).PlatformFormat,
				.InputSlot            = static_cast<uint32>(element.Slot),
				.AlignedByteOffset    = offsetInBytes[element.Slot],
				.InputSlotClass       = EnumConverter::Convert(element.Classification),
				.InstanceDataStepRate = static_cast<uint32>(element.Classification == core::InputClassification::PerInstance)
			}
		;

		/*-------------------------------------------------------------------
		-           �X���b�g���ƂɓK�؂ɓ��̓��C�A�E�g�̕��ނ��Ȃ���Ă��邩���m�F����. 
		---------------------------------------------------------------------*/
		if (inputClassifications[element.Slot].Key) // �������ς�
		{
			if (inputClassifications[element.Slot].Value != element.Classification)
			{
				throw ("You set the different classification type. InputClassification should be unified for each slot.");
			}
		}
		else // ���������s���Ă��Ȃ��ꍇ
		{
			inputClassifications[element.Slot].Key   = true; // �������ς�
			inputClassifications[element.Slot].Value = element.Classification;
		}

		// �e�X���b�g�ɑ΂��ă������̋󂫂��o�Ȃ��悤�ɔz�u����.
		offsetInBytes[element.Slot] += static_cast<uint32>(core::PixelFormatInfo::GetConst(element.Format).BlockBytes);
	}

	_inputLayout.NumElements        = static_cast<uint32>(_inputLayoutElements.Size());
	_inputLayout.pInputElementDescs = _inputLayoutElements.Data();
}