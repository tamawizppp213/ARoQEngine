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
	-         入力レイアウトの事前準備
	---------------------------------------------------------------------*/
	_inputLayoutElements.Resize(_elements.Size());

	// Slot毎に, 各入力データを連続して配置させるためのoffsetバイトを指定します. 
	auto offsetInBytes = gu::DynamicArray<uint32>(_slotCount); // slotごとのOffset

	// 入力レイアウトの分類 (頂点ごとかインスタンスごとか)の配列です. 
	// gu::PairはKeyが既に初期化済みの内容かどうか, Valueが分類が一致するかを確認するために使用します. 
	gu::DynamicArray<gu::Pair<bool, core::InputClassification>> inputClassifications
	(
		_slotCount,
		gu::Pair<bool, core::InputClassification>(false, core::InputClassification::PerVertex)
	);

	/*-------------------------------------------------------------------
	-         入力レイアウト設定
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
		-           スロットごとに適切に入力レイアウトの分類がなされているかを確認する. 
		---------------------------------------------------------------------*/
		if (inputClassifications[element.Slot].Key) // 初期化済み
		{
			if (inputClassifications[element.Slot].Value != element.Classification)
			{
				throw ("You set the different classification type. InputClassification should be unified for each slot.");
			}
		}
		else // 初期化が行われていない場合
		{
			inputClassifications[element.Slot].Key   = true; // 初期化済み
			inputClassifications[element.Slot].Value = element.Classification;
		}

		// 各スロットに対してメモリの空きが出ないように配置する.
		offsetInBytes[element.Slot] += static_cast<uint32>(core::PixelFormatInfo::GetConst(element.Format).BlockBytes);
	}

	_inputLayout.NumElements        = static_cast<uint32>(_inputLayoutElements.Size());
	_inputLayout.pInputElementDescs = _inputLayoutElements.Data();
}