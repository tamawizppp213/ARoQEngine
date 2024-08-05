//////////////////////////////////////////////////////////////////////////////////
///             @file   RHIInstance.hpp
///             @brief  Select device api (���̃G���W�����g�p���ŏ��ɌĂяo��.) 
///             @author Toide Yutaro
///             @date   2022_09_05
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Instance.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Debug.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Adapter.hpp"
#include "Platform/Core/Include/CorePlatformMacros.hpp"
#include "Platform/Core/Include/CoreOS.hpp"
#include "GameUtility/Base/Include/GUParse.hpp"
#include "GameUtility/Base/Include/GUCommandLine.hpp"
#include <d3d12.h>
#include <stdexcept>

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi;
using namespace rhi::directX12;
using namespace gu;

#if PLATFORM_CPU_X86_FAMILY && PLATFORM_DESKTOP && _WIN64
extern "C" { _declspec(dllexport) extern const UINT  D3D12SDKVersion = 613; }
extern "C" { _declspec(dllexport) extern const char* D3D12SDKPath = ".\\D3D12\\"; }
#endif

bool RHIInstance::SHOULD_CREATE_WITH_WARP  = false;
bool RHIInstance::ALLOW_SOFTWARE_RENDERING = false;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor
RHIInstance::RHIInstance(const core::RHIDebugCreateInfo& debugCreateInfo):
	core::RHIInstance(debugCreateInfo)
{
	/*-------------------------------------------------------------------
	-                   Enable CPU and GPU Debugger
	---------------------------------------------------------------------*/
#ifdef _DEBUG
	if (debugCreateInfo.EnableCPUDebugger) { EnabledDebugLayer(); }

	if (debugCreateInfo.EnableGPUDebugger) { EnabledShaderBasedValidation();}

	/*-------------------------------------------------------------------
	-                   Create Factory
	---------------------------------------------------------------------*/
	ThrowIfFailed(CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&_factory)));
#else
	ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&_factory)));
#endif
	
	/*-------------------------------------------------------------------
	-                   Create DRED
	---------------------------------------------------------------------*/
	EnabledGPUClashDebuggingModes();

	/*-------------------------------------------------------------------
	-           CommandLine����K�v�ȃR�}���h���擾����
	---------------------------------------------------------------------*/
	SHOULD_CREATE_WITH_WARP  = Parse::Contains(CommandLine::Get(), SP("warp"));
	ALLOW_SOFTWARE_RENDERING = Parse::Contains(CommandLine::Get(), SP("allowSoftwareRendering"));

	printf("StartInstance\n");
}

RHIInstance::~RHIInstance()
{
	if (_factory) { _factory.Reset(); }

	printf("DestroyInstance\n");
}

#pragma endregion Constructor and Destructor

#pragma region Public Function
/*!**********************************************************************
*  @brief     �ł����\�����������f�o�C�X�������őI�肵�܂�. ��������xGPU(�O��GPU), dGPU(discrete GPU), iGPU (integrated GPU)�̏��ɗD�悳��܂�
*  @note      DirectX12�ł͊O��GPU, �f�B�X�N���[�gGPU, ����GPU�̏��ɑI������܂�.
*  @param[in] void
*  @return gu::SharedPointer<RHIDisplayAdapter> DisplayAdapter�̃|�C���^
*************************************************************************/
gu::SharedPointer<core::RHIDisplayAdapter> RHIInstance::SearchHighPerformanceAdapter()
{
	return SearchAdapter(DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE);
}

/*!**********************************************************************
*  @brief     �ł����\���Ⴂ(����d�͂��Ⴂ)�����f�o�C�X�������őI�肵�܂�. ��������iGPU (integrated GPU), dGPU(discrete GPU), xGPU(�O��GPU)�̏��ɗD�悳��܂�
*  @note      DirectX12�ł͓���GPU, �f�B�X�N���[�gGPU, �O��GPU�̏��ɑI������܂�.�@@n
*  @param[in] void
*  @return gu::SharedPointer<RHIDisplayAdapter> DisplayAdapter�̃|�C���^
*************************************************************************/
gu::SharedPointer<core::RHIDisplayAdapter> RHIInstance::SearchMinimumPowerAdapter()
{
	return SearchAdapter(DXGI_GPU_PREFERENCE_MINIMUM_POWER);
}

/*!**********************************************************************
*  @brief     �����\�܂��͍ŏ��d�͂�����Adapter��I���i�i���jxGPU�AdGPU iGPU�i��j
*  @param[in] const DXGI_GPU_PREFERENCE �I������GPU�̎��
*  @return    gu::SharedPointer<RHIDisplayAdapter> : �����f�o�C�X�̃|�C���^
*************************************************************************/
gu::SharedPointer<core::RHIDisplayAdapter> RHIInstance::SearchAdapter(const DXGI_GPU_PREFERENCE gpuPreference) const
{
	/*-------------------------------------------------------------------
	-      ���̕����f�o�C�X���ǂ̏��ԂŒ��ׂ邩�����肷��֐�
	---------------------------------------------------------------------*/
	auto ProceedNextAdapter = [&](const DXGI_GPU_PREFERENCE preference, const uint32 adapterIndex, AdapterComPtr& adapter)
	{
		#if DXGI_MAX_FACTORY_INTERFACE >= 6
			return _factory->EnumAdapterByGpuPreference(adapterIndex, preference, IID_PPV_ARGS(&adapter));
		#else
			return _factory->EnumAdapters(adapterIndex, (IDXGIAdapter**)adapter.GetAddressOf());
		#endif
	};

	// ProceedNextAdapter�ňꎞ�I�Ɉ����Ƃ��Ďg�p���镨���f�o�C�X
	AdapterComPtr tempAdapter = nullptr;
	AdapterComPtr firstSelectedAdapter   = nullptr; // �ŏ��Ɏ擾�o���������f�o�C�X
	AdapterComPtr bestIntegratedAdapter = nullptr; // �ő�e�ʂ�VideoMemory������������GPU
	AdapterComPtr bestDiscreteAdapter   = nullptr; // �ő�e�ʂ�VideoMemory��������DiscreteGPU

	/*-------------------------------------------------------------------
	-          �����f�o�C�X�̈ꗗ���擾���� 
	-   (��������|�C���^��Ԃ��ł��ǂ���, �������Ȃ��_�Ƃ��ڍׂɃJ�X�^�}�C�Y���o����_����)
	-   ProceedNextAdapter�ɂ����, DXGI_MAX_FACTIRY_INTERFACE >= 6�ł������̓\�[�g����Ă��܂�.
	---------------------------------------------------------------------*/
	for (uint32 i = 0; ProceedNextAdapter(gpuPreference, i, tempAdapter) != DXGI_ERROR_NOT_FOUND; ++i)
	{
		if (tempAdapter == nullptr) { continue; }

		// �����f�o�C�X�̏����擾���܂�. 
		DXGI_ADAPTER_DESC adapterDesc = {};
		tempAdapter->GetDesc(&adapterDesc);

		// �����D�݂̃x���_�[�^�C�v������Ύ擾���Ă���
		const auto preferredVendor = GetPreferredAdapterVendor();

		/* WARP (Windows Advanced Rasterization Platform)�A�_�v�^���ǂ����𒲂ׂ܂�. 
		*  WARP�Ƃ�, D�RD�n�[�h�E�F�A���g�p�ł��Ȃ��ꍇ��, �����ȃ����_�����O��L�������邱�Ƃ��o���܂�. */
		const bool isWARP = adapterDesc.VendorId == (uint32)core::DisplayAdapterVendorType::Microsoft;

		// WARP���v�����ꂽ�ꍇ, ���̂��ׂẴA�_�v�^�����ۂ���. 
		// WARP���v������Ă��Ȃ��ꍇ�A�\�t�g�E�F�A�����_�����O�T�|�[�g��������Ă��Ȃ���΁AWARP�f�o�C�X�����ۂ���B
		const bool skipWARPProcess = (SHOULD_CREATE_WITH_WARP && !isWARP) || 
			                         (!SHOULD_CREATE_WITH_WARP && isWARP && !ALLOW_SOFTWARE_RENDERING);

		// �㑱�̏��������s���邩�����߂�
		const bool skipAdapterProcess = skipWARPProcess;

		/*-------------------------------------------------------------------
		-      �Ώۂ̕����f�o�C�X��������
		---------------------------------------------------------------------*/
		if (skipAdapterProcess) { continue; }

		if (firstSelectedAdapter == nullptr)
		{
			firstSelectedAdapter = tempAdapter;
		}

		const bool isDiscreteGPU = adapterDesc.DedicatedVideoMemory != 0;

		/*-------------------------------------------------------------------
		-     Discrete GPU�̏ꍇ
		---------------------------------------------------------------------*/
		if (isDiscreteGPU)
		{
			const bool isFirstSelected   = bestDiscreteAdapter == nullptr;
			const bool isPreferredVender = preferredVendor != core::DisplayAdapterVendorType::Unknown && (uint32)preferredVendor == adapterDesc.VendorId;
			
			// �܂�DicreteGPU���Ȃ��ꍇ�͋����I�Ɍ��������Ƃɂ���. 
			if (isFirstSelected) 
			{ 
				bestDiscreteAdapter = tempAdapter; 
				
				if (isPreferredVender) { break;    }
				else                   { continue; }
			}

			DXGI_ADAPTER_DESC bestDiscreteDesc = {};
			bestDiscreteAdapter->GetDesc(&bestDiscreteDesc);

			/*-------------------------------------------------------------------
			-     �����D�݂̃x���_�[������ꍇ�͂�����g�p���ċ����I��
			---------------------------------------------------------------------*/
			if (isPreferredVender)
			{
				bestDiscreteAdapter = tempAdapter;
				break;
			}
			/*-------------------------------------------------------------------
			-     �����D�݂̃x���_�[���Ȃ��ꍇ�͍ő�e�ʂ̃��������������f�o�C�X���g�p����
			---------------------------------------------------------------------*/
			else
			{
				if (adapterDesc.DedicatedVideoMemory > bestDiscreteDesc.DedicatedVideoMemory)
				{
					bestDiscreteAdapter = tempAdapter;
				}
			}
		}
		/*-------------------------------------------------------------------
		-     ����GPU�̏ꍇ: ���̂܂܍ŏ��Ɍ��������̂�������
		---------------------------------------------------------------------*/
		else
		{
			bestIntegratedAdapter = tempAdapter;
		}
	}

	/*-------------------------------------------------------------------
	-        GPU Preference�ɂ����������f�o�C�X���擾����. 
	        ������Ȃ��ꍇ�͋����I�ɍŏ��ɑI�񂾂��̂��g�p����. 
	---------------------------------------------------------------------*/
	if (gpuPreference == DXGI_GPU_PREFERENCE::DXGI_GPU_PREFERENCE_MINIMUM_POWER)
	{
		tempAdapter = bestIntegratedAdapter;
	}
	else if (gpuPreference == DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE)
	{
		tempAdapter = bestDiscreteAdapter;
	}
	else
	{
		tempAdapter = firstSelectedAdapter;
	}

	if (tempAdapter == nullptr) 
	{
		tempAdapter = firstSelectedAdapter;
	}

	// Adapter��������Ȃ������ꍇ��WARP�f�o�C�X���g�p����
	return firstSelectedAdapter ? gu::MakeShared<RHIDisplayAdapter>(SharedFromThis(), tempAdapter) : nullptr;
}

/*!**********************************************************************
*  @brief     �S�Ă̗��p�\�ȕ����f�o�C�X��z��̌`�ŕԂ��܂�.
*  @attention ���̊֐���, �z��̏��Ԃ����Ɏw�肪����܂���.
*  @param[in] void
*  @return    gu::DynamicArray<gu::SharedPointer<RHIDisplayAdapter>> : �����f�o�C�X�̔z��
*************************************************************************/
gu::DynamicArray<gu::SharedPointer<core::RHIDisplayAdapter>> RHIInstance::EnumrateAdapters() const
{
	gu::DynamicArray<gu::SharedPointer<core::RHIDisplayAdapter>> adapterLists = {};

	/*-------------------------------------------------------------------
	-                  Define Proceed next adapter function
	---------------------------------------------------------------------*/
	auto ProceedNextAdapter = [&](uint32 adapterIndex, AdapterComPtr& adapter)
	{
		#if DXGI_MAX_FACTORY_INTERFACE >= 6
			return _factory->EnumAdapterByGpuPreference(adapterIndex, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&adapter));
		#else
			return _factory->EnumAdapters(adapterIndex, (IDXGIAdapter**)adapter.GetAddressOf());
		#endif
	};

	/*-------------------------------------------------------------------
	-                  EmplaceBack Adapter List
	---------------------------------------------------------------------*/
	AdapterComPtr adapter = nullptr;
	for (int i = 0; ProceedNextAdapter(i, adapter) != DXGI_ERROR_NOT_FOUND; ++i)
	{
		const auto thisInstance = SharedFromThis();
		const auto rhiAdapter = gu::MakeShared<RHIDisplayAdapter>(thisInstance, adapter);
		adapterLists.Push(rhiAdapter);
		adapter.Reset(); // memory leak�ɑΏ�
	}

	return adapterLists;
}

/*!**********************************************************************
*  @brief     �o�͗��ɑS�Ă̕����f�o�C�X���L�����܂�
*  @param[in] void
*  @return    void
*************************************************************************/
void RHIInstance::LogAdapters() const
{
	auto adapterList = EnumrateAdapters();
	
	for (auto& adapter : adapterList)
	{
		adapter->PrintInfo();
	}

}

#if D3D12_CORE_ENABLED
/*!**********************************************************************
*  @brief     DirectXAgilitySDK�����[�h�ς݂���Ԃ��܂�
*************************************************************************/
bool RHIInstance::HasLoadedDirectXAgilitySDK() const
{
	return platform::core::OS::GetDLLHandle(SP("D3D12Core.dll")) != nullptr;
}
#endif
#pragma endregion Public Function


#pragma region Debugger
/*!**********************************************************************
*  @brief    DirectX12�ɂ�����CPU�̃f�o�b�O�@�\��On�ɂ���
*  @note     �f�o�b�O���[�h�ɂ����g�p�ł��܂���
*************************************************************************/
void RHIInstance::EnabledDebugLayer()
{
	DebugComPtr debugController;
	ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)));
	
	/* Switch on Debug Layer*/
	debugController->EnableDebugLayer();

	/* Release Debugger pointer*/
	debugController.Reset();
}


/*!**********************************************************************
*  @brief     GPU�x�[�X�̃o���f�[�V����
*  @note      FPS�ɋ����e����^���邱�Ƃ���, Release���ɂ͎g�p���Ȃ��悤�ɂ��Ă�������
*  @details   �ȉ��̂��Ƃ��s���܂�. @n
*             1. ����������Ă��Ȃ��A�܂��͌݊����̂Ȃ��f�B�X�N���v�^���V�F�[�_�Ŏg�p���� @n
*             2. �폜���ꂽResource���Q�Ƃ���f�B�X�N���v�^���V�F�[�_�Ŏg�p���� @n
*             3. ���i�������\�[�X�̏�Ԃƃ��\�[�X�̏�Ԃ̌����̌��� @n
*             4. �V�F�[�_�[�ɂ�����f�B�X�N���v�^�E�q�[�v�̏I�[�𒴂����C���f�b�N�X�t�� @n
*             5. �݊����̂Ȃ���ԂŃV�F�[�_�[�����\�[�X�ɃA�N�Z�X���� @n
*             6. ����������Ă��Ȃ��A�܂��͌݊����̂Ȃ��T���v���[���V�F�[�_�[�Ŏg�p���� @n
*************************************************************************/
void RHIInstance::EnabledShaderBasedValidation()
{
#if PLATFORM_OS_WINDOWS && D3D12_MAX_DEBUG_INTERFACE >= 3
	DebugComPtr debugController;

	// Get Debugger
	ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)));

	/* Switch on GPU Debugger*/
	debugController->SetEnableGPUBasedValidation(true);

	/* Release Debug Pointer*/
	debugController.Reset();
#endif
}

/*!**********************************************************************
*  @brief     Device���폜���ꂽ�Ƃ��̏����L�q����(DRED)�Ȃǂ̗L����
*************************************************************************/
void RHIInstance::EnabledGPUClashDebuggingModes()
{
	_useDRED            = false;
	_useLightWeightDRED = false;
	_useDREDContext     = false;

	/*-------------------------------------------------------------------
	-                Dred setting default
	---------------------------------------------------------------------*/
#ifdef  __ID3D12DeviceRemovedExtendedDataSettings_INTERFACE_DEFINED__
	ComPtr<ID3D12DeviceRemovedExtendedDataSettings> dredSettings = nullptr;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(dredSettings.GetAddressOf()))))
	{
		// Turn on AutoBreadCrumbs and Page Fault reporting.
		// GPU�N���b�V�����ɂǂ̖��߂����������N������������肷��
		dredSettings->SetAutoBreadcrumbsEnablement(D3D12_DRED_ENABLEMENT_FORCED_ON);
		
		// �������A�N�Z�X�ᔽ�����������ꏊ����肷��
		dredSettings->SetPageFaultEnablement(D3D12_DRED_ENABLEMENT_FORCED_ON);

		_useDRED = true;
		_useLightWeightDRED = false;

		_RPT0(_CRT_WARN, "DRED enabled\n");
	}
	else
	{
		_RPT0(_CRT_WARN, "DRED requested but interface was not found, hresult: %x. DRED only works on Windows 10 1903+.");
	}
#endif

	/*-------------------------------------------------------------------
	-                Dred setting 1
	---------------------------------------------------------------------*/
#ifdef  __ID3D12DeviceRemovedExtendedDataSettings1_INTERFACE_DEFINED__
	ComPtr<ID3D12DeviceRemovedExtendedDataSettings1> dredSettings1 = nullptr;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(dredSettings1.GetAddressOf()))))
	{
		dredSettings1->SetBreadcrumbContextEnablement(D3D12_DRED_ENABLEMENT_FORCED_ON);
		_useDREDContext = true;
		_RPT0(_CRT_WARN, "dred breadcrumb context enabled\n");
	}
#endif

	/*-------------------------------------------------------------------
	-                Dred setting 2
	---------------------------------------------------------------------*/
#ifdef __ID3D12DeviceRemovedExtendedDataSettings2_INTERFACE_DEFINED__
	ComPtr<ID3D12DeviceRemovedExtendedDataSettings2> dredSettings2 = nullptr;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(dredSettings2.GetAddressOf()))))
	{
		dredSettings2->SetBreadcrumbContextEnablement(D3D12_DRED_ENABLEMENT_FORCED_ON);
		dredSettings2->SetAutoBreadcrumbsEnablement(D3D12_DRED_ENABLEMENT_FORCED_ON);
		dredSettings2->SetPageFaultEnablement(D3D12_DRED_ENABLEMENT_FORCED_ON);
		dredSettings2->UseMarkersOnlyAutoBreadcrumbs(true);
		_useDREDContext = true;
		_useLightWeightDRED = true;
		_RPT0(_CRT_WARN, "dred breadcrumb context enabled\n");
	}
#endif
}
#pragma endregion Debugger
#pragma region Private Function