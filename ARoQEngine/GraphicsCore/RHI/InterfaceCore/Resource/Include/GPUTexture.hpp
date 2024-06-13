//////////////////////////////////////////////////////////////////////////////////
///  @file   GPUTexture.hpp
///  @brief  �e�N�X�`���Ƃ��Ďg�p����ꍇ��GPU���\�[�X�ł�.
///  @author Toide Yutaro
///  @date   2024_04_21
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GPU_TEXTURE_HPP
#define GPU_TEXTURE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GPUResource.hpp"
#include "GameUtility/Base/Include/GUSmartPointer.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace gm
{
	struct RGBA;
}

namespace rhi::core
{
	class RHIDevice;
	class RHICommandList;
	class RHICommandQueue;
	class GPUBuffer;

	/****************************************************************************
	*				  			GPUTexture
	****************************************************************************/
	/*  @brief  �e�N�X�`���Ƃ��Ďg�p����ꍇ��GPU���\�[�X�ł�.
	*****************************************************************************/
	class GPUTexture : public GPUResource, public gu::EnableSharedFromThis<GPUTexture>
	{
	public:
		#pragma region Public Function
		virtual void Load(const gu::tstring& filePath, const gu::SharedPointer<RHICommandList>& commandList) = 0;

		virtual void Save(const gu::tstring& filePath, const gu::SharedPointer<RHICommandList>& commandList, const gu::SharedPointer<RHICommandQueue>& commandQueue) = 0;

		virtual void Write(const gu::SharedPointer<RHICommandList>& commandList, const gm::RGBA* pixel) = 0;
		#pragma endregion
		#pragma region Public Property
		/*!**********************************************************************
		*  @brief     �e�N�X�`���̕���pixel�P�ʂŎw�肵�܂�. ���̍�, �~�b�v�}�b�v���x�����w�肷��΂��̒l�����T�C�Y�𔼕��ɂ��܂�.
		*  @param[in] const gu::uint8 �e�N�X�`���̃~�b�v�}�b�v���x��
		*  @return    gu::uint32 �e�N�X�`���̕�(pixel)
		*************************************************************************/
		__forceinline gu::uint32 GetWidth(const gu::uint8 mipLevel = 0) const noexcept
		{
			return _metaData.Width > 1u << mipLevel ? 1u : _metaData.Width >> mipLevel;
		}

		/*!**********************************************************************
		*  @brief     �e�N�X�`���̍�����pixel�P�ʂŎw�肵�܂�. ���̍�, �~�b�v�}�b�v���x�����w�肷��΂��̒l�����T�C�Y�𔼕��ɂ��܂�.
		*  @param[in] const gu::uint8 �e�N�X�`���̃~�b�v�}�b�v���x��
		*  @return    gu::uint32 �e�N�X�`���̍���(pixel)
		*************************************************************************/
		__forceinline gu::uint32 GetHeight(const gu::uint8 mipLevel = 0) const noexcept
		{
			return _metaData.Height > 1u << mipLevel ? 1u : _metaData.Height >> mipLevel;
		}

		/*!**********************************************************************
		*  @brief     �e�N�X�`����Depth��pixel�P�ʂŎw�肵�܂�. ���̍�, �~�b�v�}�b�v���x�����w�肷��΂��̒l�����T�C�Y�𔼕��ɂ��܂�.
		*  @param[in] const gu::uint8 �e�N�X�`���̃~�b�v�}�b�v���x��
		*  @return    gu::uint16 �e�N�X�`����Depth(pixel)
		*************************************************************************/
		gu::uint16 GetDepth (const gu::uint8 mipLevel = 0) const noexcept;

		/*!**********************************************************************
		*  @brief     �e�N�X�`����Width, Height, Depth��pixel�P�ʂŎw�肵�܂�
		*  @param[in] const gu::uint8 �e�N�X�`���̃~�b�v�}�b�v���x��
		*  @return    gm::Vector3i<uint32>
		*************************************************************************/
		__forceinline gm::Vector3i<gu::uint32> GetTexelSize(const gu::uint8 mipLevel = 0) const noexcept
		{
			return gm::Vector3i<gu::uint32>(GetWidth(mipLevel), GetHeight(0), static_cast<gu::uint32>(GetDepth()));
		}

		/*!**********************************************************************
		*  @brief     �e�N�X�`���S�̂̃o�C�g�T�C�Y���v�Z���܂�. 
		*  @param[in] const gu::uint8 �e�N�X�`���̃~�b�v�}�b�v���x��
		*  @return    gu::uint64 �e�N�X�`���̃o�C�g�T�C�Y
		*************************************************************************/
		gu::uint64 GetByteSize(const gu::uint8 mipLevel = 0) const noexcept;

		/*!**********************************************************************
		*  @brief     1�s�N�Z���̌^��`��Ԃ��܂�
		*  @return    PixelFormat 1�s�N�Z���̌^��`
		*************************************************************************/
		__forceinline PixelFormat GetPixelFormat() const noexcept { return _metaData.PixelFormat; }

		/*!**********************************************************************
		*  @brief     �T���v�����O�̐����w�肵�܂�. ��{�I�ɂ�Count1���g�p��, MSAA�Ȃǂ��g�p����Ƃ���Count4����Ɏg�p���܂�.
		*  @return    MultiSample : �T���v�����O�̐�
		*************************************************************************/
		__forceinline MultiSample GetMultiSample() const noexcept { return _metaData.Sample; }

		/*!**********************************************************************
		*  @brief     �e�N�X�`���̎�������ݒ肵�܂�.
		*  @return    ResourceDimension : �e�N�X�`���̎�����
		*************************************************************************/
		__forceinline ResourceDimension GetDimension() const noexcept { return _metaData.Dimension; }

		/*!**********************************************************************
		*  @brief     �e�N�X�`���̎g�p���@���擾���܂� @n
		*             ResourceType��Texture2D�ȂǃL���[�u�}�b�v�Ƃ��Ă̎g�p�Ȃǂ��`������̂ł�. @n
		* �@�@�@�@�@�@�@ TextureCreateFlags�̓����_�[�^�[�Q�b�g�Ƃ���GPU���\�[�X���g�p�\�Ȃǂ��`������̂ł�.
		*  @return    ResourceType �e�N�X�`���̎g�p���@
		*************************************************************************/
		__forceinline ResourceType GetResourceType() const noexcept { return _metaData.ResourceType; }

		/*!**********************************************************************
		*  @brief     �e�N�X�`����GPU���\�[�X�Ƃ��Ă̎g�p���@���擾���܂�. @n
		*             ResourceType��Texture2D�ȂǃL���[�u�}�b�v�Ƃ��Ă̎g�p�Ȃǂ��`��, @n
		* �@�@�@�@�@�@�@ ����̓����_�[�^�[�Q�b�g�Ƃ���GPU���\�[�X���g�p�\�Ȃǂ��`������̂ł�.
		*  @return    TextureCreateFlags �e�N�X�`���쐬���@
		*************************************************************************/
		__forceinline TextureCreateFlags GetUsage() const noexcept { return _metaData.Usage; }

		/*!**********************************************************************
		*  @brief     �e�N�X�`�����������ɏ����������F���w�肵�܂�. @n
		*             �����_�[�^�[�Q�b�g��f�v�X�X�e���V�����g�p����Ƃ��Ɏg�p��, �f�t�H���g�͔��F�ł�
		*  @return    ClearValue : �N���A�J���[
		*************************************************************************/
		__forceinline ClearValue GetClearValue () const noexcept { return _metaData.ClearColor; }

		/*!**********************************************************************
		*  @brief     GPU���\�[�X�̃o���A��Ԃ��`���܂�.
		*  @return    core::ResourceState GPU���\�[�X�o���A���
		*************************************************************************/
		__forceinline core::ResourceState GetResourceState() const noexcept override { return _metaData.State; }

		/*!**********************************************************************
		*  @brief     �~�b�v�}�b�v�̒i�K�� (2���w�肷���, �~�b�v�}�b�v�̒l��0��1�̂��̂��g�p����Ƃ����Ӗ��ł�)
		*  @return    gu::uint8 �~�b�v�}�b�v�̒i�K��
		*************************************************************************/
		__forceinline gu::uint8 GetMipMapCount() const noexcept { return _metaData.MipMapLevels; }

		/*!**********************************************************************
		*  @brief     �~�b�v�}�b�v�̐ݒ�\�ȍő僌�x�� (2���w�肷���, �~�b�v�}�b�v�̒l��0��1�̂��̂��g�p����Ƃ����Ӗ��ł�)
		*  @return    gu::uint8 �~�b�v�}�b�v�̍ő僌�x��
		*************************************************************************/
		__forceinline gu::uint8 GetMaxMipMapLevel() const noexcept { return _metaData.MipMapLevels >= 1? _metaData.MipMapLevels - 1 : 0; }

		/*!**********************************************************************
		*  @brief     �~�b�v�}�b�v�̐ݒ�\�ȍŏ����x�� (���݂�0���O��ł�)
		*  @return    gu::uint8 �~�b�v�}�b�v�̍ŏ����x��
		*************************************************************************/
		__forceinline gu::uint8 GetMinMipMapLevel() const noexcept { return 0; }

		/*!**********************************************************************
		*  @brief     �e�N�X�`���̔z�񒷂�Ԃ��܂�
		*  @return    gu::uint16 �z��
		*************************************************************************/
		__forceinline gu::uint16 GetArrayLength() const noexcept { return _metaData.Dimension == ResourceDimension::Texture3D ?  1 : _metaData.DepthOrArraySize; }

		/*!**********************************************************************
		*  @brief     �e�N�X�`���z��Ƃ��Ďg�p����Ă��邩
		*  @return    bool �e�N�X�`���z��Ƃ��Ďg�p���Ă���ꍇ��true��Ԃ��܂�. (3�����e�N�X�`����false���w�肵�܂�)
		*************************************************************************/
		__forceinline bool IsArray() const noexcept {  return GetArrayLength() != 1; }

		/*!**********************************************************************
		*  @brief     �T�u���\�[�XID���擾���܂� 
		*  @note      https://learn.microsoft.com/ja-jp/windows/win32/direct3d12/subresources
		*  @param[in] const gu::uint32 mipSlice   : �擾�������~�b�v�}�b�v�̃��x�����w�肵�܂�.  
		*  @param[in] const gu::uint32 arraySlice : �e�N�X�`���z��ɂ�����, �e�N�X�`���̎�ނ��w�肷�鑀��ɑΉ����܂�
		*  @param[in] const gu::uint32 planeSlice : RGBA�ȊO�̃e�N�X�`���t�H�[�}�b�g����, DepthStencil�e�N�X�`���̂悤�ȕ����v���[�������������̂Ɏg�p���܂�.@n
		*                                           ��{��0�ɐݒ肳��܂�. DepthStencil�̏ꍇDepth��0��Stencil��1��ݒ肷�邱�ƂɂȂ�܂�.
		*  @return    gu::uint32 �T�u���\�[�XID   
		*************************************************************************/
		__forceinline gu::uint32 GetSubresourceIndex(const gu::uint32 mipSlice, const gu::uint32 arraySlice, const gu::uint32 planeSlice = 0) const noexcept 
		{
			const gu::uint32 mipmapLevel = GetMaxMipMapLevel();
			return mipSlice + (arraySlice * mipmapLevel) + (planeSlice * mipmapLevel * static_cast<gu::uint32>(GetArrayLength()));
		}

		/*!**********************************************************************
		*  @brief     �����I�Ɋm�ۂ��Ă���e�N�X�`���T�C�Y��Ԃ��܂�. 
		*************************************************************************/
		__forceinline gu::uint64 GetPhysicalSize() const noexcept { return _physicalSize; }

		/*!**********************************************************************
		*  @brief     �e�N�X�`���̃A���C�������g���w�肵�܂�
		*************************************************************************/
		__forceinline gu::uint64 GetAlignment() const noexcept { return _alignment; }

		/*!**********************************************************************
		*  @brief     Texture�̍쐬������ʂ�Ǘ����Ă���\���̂�ϊ�
		*  @return    GPUTextureMetaData
		*************************************************************************/
		__forceinline GPUTextureMetaData& GetMetaData() { return _metaData; }

		/*!**********************************************************************
		*  @brief     Texture�̍쐬������ʂ�Ǘ����Ă���\���̂�ϊ�
		*  @return    GPUTextureMetaData
		*************************************************************************/
		__forceinline const GPUTextureMetaData& GetMetaData() const noexcept { return _metaData; }

		#pragma endregion 

		#pragma region Public Constructor and Destuctor
		#pragma endregion

	protected:
		#pragma region Protected Constructor and Destructor
		/*! @brief �f�t�H���g�R���X�g���N�^*/
		GPUTexture() = default;

		/*! @brief �f�X�g���N�^*/
		~GPUTexture() = default;

		/*! @brief �_���f�o�C�X���g���ď��������܂�. */
		explicit GPUTexture(const gu::SharedPointer<RHIDevice>& device, [[maybe_unused]]const gu::tstring& name = SP("Texture")) : core::GPUResource(device) {};
		
		/*! @brief �e�N�X�`���쐬���Ƙ_���f�o�C�X���g���ď�����*/
		explicit GPUTexture(const gu::SharedPointer<RHIDevice>& device, const GPUTextureMetaData& metaData, [[maybe_unused]] const gu::tstring& name): core::GPUResource(device), _metaData(metaData) {};
		#pragma endregion 
		
		#pragma region Protected Function

		#pragma endregion Protected Function

		#pragma region Protected Property
		/*! @brief �e�N�X�`���̍쐬������ʂ�Ǘ����Ă���\����*/
		GPUTextureMetaData _metaData;

		/*! @brief �����I�Ɋm�ۂ��Ă���e�N�X�`���T�C�Y*/
		gu::uint64 _physicalSize = 0;

		/*! @brief �A���C�������g*/
		gu::uint64 _alignment = 0;
		#pragma endregion 

	};
}
#endif