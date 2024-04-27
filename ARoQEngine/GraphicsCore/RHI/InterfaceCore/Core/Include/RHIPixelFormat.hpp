//////////////////////////////////////////////////////////////////////////////////
///  @file   RHIPixelFormat.hpp
///  @brief  �摜�f�[�^���������߂̃N���X�ł�. 
///  @author toide
///  @date   2024/04/24 22:50:59
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef RHI_PIXEL_FORMAT_HPP
#define RHI_PIXEL_FORMAT_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/GUType.hpp"
#include "GameUtility/Base/Include/GUEnumClassFlags.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::core
{
	/****************************************************************************
	*				  			PixelFormat
	*************************************************************************//**
	/*  @brief  �`��Ŏg�p����s�N�Z���t�H�[�}�b�g
	*****************************************************************************/
	enum class PixelFormat : gu::uint8
	{
		Unknown            = 0,  //!< �g�p�p�r���s���̃t�H�[�}�b�g�ł�.

		R32G32B32A32_FLOAT   = 1,  //!< RGBA�R���|�[�l���g��32bit��Float�l
		R32G32B32A32_UINT    = 2,  //!< RGBA�R���|�[�l���g��32bit��Unsigned int�l
		R32G32B32A32_SINT    = 3,  //!< RGBA�R���|�[�l���g��32bit��Signed int�l
		
		R16G16B16A16_FLOAT   = 4,  //!< RGBA�R���|�[�l���g��16bit��Float�l,
		R16G16B16A16_UINT    = 5,  //!< RGBA�R���|�[�l���g��16bit��Unsigned int�l
		R16G16B16A16_SINT    = 6,  //!< RGBA�R���|�[�l���g��16bit��Signed int�l
		R16G16B16A16_UNORM   = 7,  //!< RGBA�R���|�[�l���g��16bit�̕����Ȃ����K��. 0�`1�ŋ�؂�܂�
		R16G16B16A16_SNORM   = 8,  //!< RGBA�R���|�[�l���g��16bit�̕������萳�K��.  -1�`1�ŋ�؂�܂�
		
		R8G8B8A8_UINT        = 9,   //!< RGBA�R���|�[�l���g��8bit��Unsigned int�l. 
		R8G8B8A8_SINT        = 10,  //!< RGBA�R���|�[�l���g��8bit��Signed int�l . 
		R8G8B8A8_UNORM       = 11,  //!< RGBA�R���|�[�l���g��8bit�̕����Ȃ����K��.  0�`1�ŋ�؂�܂�
		R8G8B8A8_SNORM       = 12,  //!< RGBA�R���|�[�l���g��8bit�̕������萳�K�� .  -1�`1�ŋ�؂�܂�
		R8G8B8A8_UNORM_SRGB  = 13,  //!< RGBA�R���|�[�l���g��SRGB�e�N�X�`���Ɏg�p. 0�`1�ŋ�؂�܂�,

		B8G8R8A8_UNORM       = 14,  //!< BGRA�R���|�[�l���g��8bit�̕����Ȃ����K��.  0�`1�ŋ�؂�܂� 
		B8G8R8A8_UNORM_SRGB  = 15,  //!< BGRA�R���|�[�l���g��SRGB�e�N�X�`���Ɏg�p. 8bit�̕����Ȃ����K��.  0�`1�ŋ�؂�܂� 

		R32G32B32_FLOAT      = 16,  //!< RGB�R���|�[�l���g��32bit��Float�l
		R32G32B32_UINT       = 17,  //!< RGB�R���|�[�l���g��32bit��Unsigned int�l.
		R32G32B32_SINT       = 18,  //!< RGB�R���|�[�l���g��32bit��Signed int�l
		                               
		R32G32_FLOAT         = 19,  //!< RG�R���|�[�l���g��32bit��Float�l
		R32G32_UINT          = 20,  //!< RG�R���|�[�l���g��32bit��Unsigned int�l
		R32G32_SINT          = 21,  //!< RG�R���|�[�l���g��32bit��Signed int�l
		
		R16G16_FLOAT         = 22,  //!< RG�R���|�[�l���g��16bit��Float�l
		R16G16_UINT          = 23,  //!< RG�R���|�[�l���g��16bit��Unsigned int�l
		R16G16_SINT          = 24,  //!< RG�R���|�[�l���g��16bit��Signed int�l

		R8G8_UINT            = 25,  //!< RG�R���|�[�l���g��8bit��Unsigned int�l. 
		R8G8_SINT            = 26,  //!< RG�R���|�[�l���g��8bit��Signed int�l . 
		R8G8_UNORM           = 27,  //!< RG�R���|�[�l���g��8bit�̕����Ȃ����K��.  0�`1�ŋ�؂�܂�
		R8G8_SNORM           = 28,  //!< RG�R���|�[�l���g��8bit�̕������萳�K�� .  -1�`1�ŋ�؂�܂�

		D32_FLOAT            = 29,  //!< Depth�o�b�t�@�p��32bit��Float�l
		R32_FLOAT            = 30,  //!< R�R���|�[�l���g��32bit��Float�l
		R32_UINT             = 31,  //!< R�R���|�[�l���g��32bit��Unsigned int�l
		R32_SINT             = 32,  //!< R�R���|�[�l���g��32bit��Signed int�l
		
		D16_UNORM            = 33,  //!< Depth�o�b�t�@�p��16bit��Float�l
		R16_FLOAT            = 34,  //!< R�R���|�[�l���g��16bit��Float�l
		R16_UINT             = 35,  //!< R�R���|�[�l���g��16bit��Unsigned int�l
		R16_SINT             = 36,  //!< R�R���|�[�l���g��16bit��Signed int�l
		R16_UNORM            = 37,  //!< R�R���|�[�l���g��16bit�̕����Ȃ����K��.  0�`1�ŋ�؂�܂�
		R16_SNORM            = 38,  //!< R�R���|�[�l���g��16bit�̕������萳�K�� .  -1�`1�ŋ�؂�܂�

		R8_UINT              = 39,  //!< R�R���|�[�l���g��8bit��Unsigned int�l
		R8_SINT              = 40,  //!< R�R���|�[�l���g��8bit��Signed int�l
		R8_UNORM             = 41,  //!< R�R���|�[�l���g��8bit�̕����Ȃ����K��.  0�`1�ŋ�؂�܂�
		R8_SNORM             = 42,  //!< R�R���|�[�l���g��8bit�̕������萳�K�� .  -1�`1�ŋ�؂�܂�
		A8_UNORM             = 43,  //!< A�R���|�[�l���g��8bit�̕����Ȃ����K��.  0�`1�ŋ�؂�܂�

		R1_UNORM             = 44,  //!< R�R���|�[�l���g��1bit�̕����Ȃ����K��.  0�`1�ŋ�؂�܂�.

		D32_FLOAT_S8X24_UINT = 45, //!< 32bit�̐[�x�R���|�[�l���gFloat�l�Őݒ肵, Stencil�R���|�[�l���g��8bit, ���g�p�̃R���|�[�l���g24bit���g�p���܂�
		D24_UNORM_S8_UINT    = 46, //!< Depth�p��24bit��0�`1�̕����Ȃ����K�����s��, Stencil�p��Unsigned int�l��8bit�Ŏw�肵�܂�.
		R10G10B10A2_UNORM    = 47, //!< RGB�R���|�[�l���g��10bit����, A�R���|�[�l���g��2bit�̕����Ȃ����K��. 0�`1�ŋ�؂�܂�
		R10G10B10A2_UINT     = 48, //!< RGB�R���|�[�l���g��10bit����, A�R���|�[�l���g��2bit��Unsigned int�l

		// Block compression format (�摜��4x4�s�N�Z���P�ʂ̃u���b�N�ɕ�������, ���ꂼ��̃u���b�N���ƂɈ��k���s������)
		// https://www.webtech.co.jp/blog/optpix_labs/format/6993/
		BC1_UNORM            = 49, //!< 2 byte RGB(5,6,5bit) + 2 byte RGB(5,6,5bit) + 4 byte(2 bit * 4 pixel * 4 pixel��, 2bit�ɂ�3�F+������4�F�̑g�ݍ��킹���i�[)��0�`1��Linear�e�N�X�`�� 
		BC1_UNORM_SRGB       = 50, //!< 2 byte RGB(5,6,5bit) + 2 byte RGB(5,6,5bit) + 4 byte(2 bit * 4 pixel * 4 pixel��, 2bit�ɂ�3�F+������4�F�̑g�ݍ��킹���i�[)��0�`1��SRGB�e�N�X�`�� 
		BC2_UNORM            = 51, //!< 8 byte (4bit�ł̊e�s�N�Z�����̃A���t�@�l) + 2 byte RGB(5,6,5bit) + 2 byte RGB(5,6,5bit) + 4 byte(2 bit * 4 pixel * 4 pixel��, 2bit�ɂ�3�F+������4�F�̑g�ݍ��킹���i�[)��0�`1��Linear�e�N�X�`��
		BC2_UNORM_SRGB       = 52, //!< 8 byte (4bit�ł̊e�s�N�Z�����̃A���t�@�l) + 2 byte RGB(5,6,5bit) + 2 byte RGB(5,6,5bit) + 4 byte(2 bit * 4 pixel * 4 pixel��, 2bit�ɂ�3�F+������4�F�̑g�ݍ��킹���i�[)��0�`1��SRGB�e�N�X�`��
		BC3_UNORM            = 53, //!< 1 byte (�A���t�@�̑�\�l0) + 1 byte (�A���t�@�̑�\�l1) + 6 byte (�s�N�Z�����̃C���f�b�N�X�e3bit���g���Đ��`���) + 2 byte(RGB 5,6,5bit) + 2 byte(RGB 5,6,5bit) + 4byte(�s�N�Z�����̃C���f�b�N�X(�e2bit)) 
		BC3_UNORM_SRGB       = 54, //!< 1 byte (�A���t�@�̑�\�l0) + 1 byte (�A���t�@�̑�\�l1) + 6 byte (�s�N�Z�����̃C���f�b�N�X�e3bit���g���Đ��`���) + 2 byte(RGB 5,6,5bit) + 2 byte(RGB 5,6,5bit) + 4byte(�s�N�Z�����̃C���f�b�N�X(�e2bit)) 
		BC4_UNORM            = 55, //!< 1�`�����l���̏�񂾂����i�[����(�P�x�Ƃ��n�C�g�}�b�v�Ƃ�)1 byte(��\�l 0), 1 byte (��\�l1) + 6 byte(�e�s�N�Z�����̃C���f�b�N�X�e3bit) 0.0�`1.0
		BC4_SNORM            = 56, //!< 1�`�����l���̏�񂾂����i�[����(�P�x�Ƃ��n�C�g�}�b�v�Ƃ�)1 byte(��\�l 0), 1 byte (��\�l1) + 6 byte(�e�s�N�Z�����̃C���f�b�N�X�e3bit) -1.0�`1.0
		BC5_UNORM            = 57, //!< 2�`�����l���̏����i�[���܂�(�@���}�b�v��xy�������i�[����z�͌v�Z�ŏo����) 1 byte(����1�̑�\�l0) + 1 byte(����1�̑�\�l1) + 6 byte����1�̊e�s�N�Z�����̃C���f�b�N�X(3bit)������2�����������i�[ 0.0�`1.0
		BC5_SNORM            = 58, //!< 2�`�����l���̏����i�[���܂�(�@���}�b�v��xy�������i�[����z�͌v�Z�ŏo����) 1 byte(����1�̑�\�l0) + 1 byte(����1�̑�\�l1) + 6 byte����1�̊e�s�N�Z�����̃C���f�b�N�X(3bit)������2�����������i�[ -1.0�`1.0
		BC7_UNORM            = 59, //!< BC1�`BC3�������i���ȉ摜�f�[�^��RGB, RGBA�摜���i�[���܂�. �e�N�X�`����Linear. �R�����g�ɂ͏�������Ȃ�����, �ڍׂ�https://www.webtech.co.jp/blog/optpix_labs/format/7006/���Q��
		BC7_UNORM_SRGB       = 60, //!< BC1�`BC3�������i���ȉ摜�f�[�^��RGB, RGBA�摜���i�[���܂�. �e�N�X�`����SRGB. �R�����g�ɂ͏�������Ȃ�����, �ڍׂ�https://www.webtech.co.jp/blog/optpix_labs/format/7006/���Q��
		BC6H_UFLOAT16        = 61, //!< 4x4�s�N�Z����16byte�Ɉ��k:HDR��RGB�摜�ŃA���t�@�`�����l���̓T�|�[�g���Ă��Ȃ�. ���[�h�r�b�g�ŏ��������򂷂邽�߃R�����g�ɂ͏�������Ȃ�. �ڍׂ�https://www.webtech.co.jp/blog/optpix_labs/format/7006/���Q��. �����Ȃ�
		BC6H_SFLOAT16        = 62, //!< 4x4�s�N�Z����16byte�Ɉ��k:HDR��RGB�摜�ŃA���t�@�`�����l���̓T�|�[�g���Ă��Ȃ�. ���[�h�r�b�g�ŏ��������򂷂邽�߃R�����g�ɂ͏�������Ȃ�. �ڍׂ�https://www.webtech.co.jp/blog/optpix_labs/format/7006/���Q��. ��������


		CountOfPixelFormat
	};

	/****************************************************************************
	*				  		PixelChannelFlags
	*************************************************************************//**
	/*  @brief  �ǂ̃s�N�Z���̃`�����l�����g�p�ł��邩
	*****************************************************************************/
	enum class PixelChannelFlags : gu::uint8
	{
		None = 0,            //!< ���p�s��
		R    = 1 << 0,       //!< Red Channel
		G    = 1 << 1,       //!< Green Channel
		B    = 1 << 2,       //!< Blue Channel
		A    = 1 << 3,       //!< Alpha Channel

		RG   = R | G,        //!< Red, Green Channel
		RGB  = R | G | B,    //!< Red, Green, Blue Channel
		RGBA = R | G | B | A //!< Red, Green, Blue, Alpha Channel 
	};

	/****************************************************************************
	*				  		PixelFormatCapabilities
	*************************************************************************//**
	/*  @brief  �Ώۃs�N�Z���t�H�[�}�b�g�ɑ΂���, �����\��GPU���\�[�X�̎g�����ɂ��Ē�`���Ă��܂�. 
	*****************************************************************************/
	enum class PixelFormatCapabilities : gu::uint32
	{
		None = 0, //!< ���p�s��
		Texture1D        = 1ull << 1,  //!< 1�����̃e�N�X�`�������p�\
		Texture2D        = 1ull << 2,  //!< 2�����̃e�N�X�`�������p�\
		Texture3D        = 1ull << 3,  //!< 3�����̃e�N�X�`�������p�\
		TextureCube      = 1ull << 4,  //!< �L���[�u�}�b�v�e�N�X�`�������p�\
		RenderTarget     = 1ull << 5,  //!< �����_�[�^�[�Q�b�g�����p�\
		DepthStencil     = 1ull << 6,  //!< �f�v�X�X�e���V�������p�\
		TextureMipmap    = 1ull << 7,  //!< �~�b�v�}�b�v�̃e�N�X�`�������p�\
		TextureLoad      = 1ull << 8,  //!< ���[�h���삪�\
		TextureSample    = 1ull << 9,  //!< �V�F�[�_�[����e�N�X�`���̃s�N�Z���l�𒼐ڃT���v�����O���邱�Ƃ��\�ł�.
		TextureGather    = 1ull << 10, //!< �e�N�X�`���̃T���v�����O����Bilinear��ԂɎg�p�����4�̃T���v�����擾���܂�. 
		TextureAtomics   = 1ull << 11, //!< �e�N�X�`�����A�g�~�b�N���� (�����X���b�h�̏������݋�����Ԃ�����邽�߂̑���)���\�ł�
		TextureBlendable = 1ull << 12, //!< �e�N�X�`���̃u�����f�B���O���\�ł�.
		TextureStore     = 1ull << 13, //!< �e�N�X�`���̃s�N�Z���l�̏������݂��\�ł�. 

		Buffer           = 1ull << 14, //!< �o�b�t�@�Ƃ��ė��p�\
		VertexBuffer     = 1ull << 15, //!< ���_�o�b�t�@�Ƃ��Ă����p�\
		IndexBuffer      = 1ull << 16, //!< �C���f�b�N�X�o�b�t�@�Ƃ��Ă����p�\
		BufferLoad       = 1ull << 17, //!< �V�F�[�_�[����o�b�t�@�̓���̒l����x�ɓǂݍ��ނ��Ƃ��\�ł�
		BufferStore      = 1ull << 18, //!< �V�F�[�_�[����o�b�t�@�̓���̒l���������ނ��Ƃ��\�ł�. 
		BufferAtomics    = 1ull << 19, //!< �o�b�t�@���A�g�~�b�N���� (�����X���b�h�̏������݋�����Ԃ�����邽�߂̑���)���\�ł�

		UAV              = 1ull << 20, //!< Unordered access view�Ƃ��ăs�N�Z���̓ǂݏ������\
		UAVLoad          = 1ull << 21, //!< �V�F�[�_�[����UAV�̓���̒l����x�ɓǂݍ��ނ��Ƃ��\�ł�
		UAVStore         = 1ull << 22, //!< �V�F�[�_�[����UAV�̓���̒l���������ނ��Ƃ��\�ł�.

		/// �e�N�X�`���Ƃ��ė��p�\
		AnyTexture = Texture1D | Texture2D | Texture3D | TextureCube,

		/// �S�Ẵe�N�X�`�����\�[�X���g�p�\
		AllTexture = AnyTexture | RenderTarget | DepthStencil | TextureMipmap | TextureLoad | TextureSample | TextureGather | TextureAtomics | TextureBlendable | TextureStore,

		/// �S�Ẵo�b�t�@�Ƃ��ė��p�\
		AllBuffer  = Buffer | VertexBuffer | IndexBuffer | BufferLoad | BufferStore | BufferAtomics,

		/// �S�Ă�Unordered access view�Ƃ��ė��p�\
		AllUAV     = UAV | UAVLoad | UAVStore,

		/// �S�Ẵ��\�[�X�t���O�����p�\
		AllResource = AllTexture | AllBuffer | AllUAV
	};

	ENUM_CLASS_FLAGS(PixelFormatCapabilities);

	/****************************************************************************
	*				  			  PixelFormatInfo
	*************************************************************************//**
	/*  @brief  �Ώۃs�N�Z���t�H�[�}�b�g�ɉ����������i�[���Ă��܂�. @n
	*           ��{�I�ɒ��ڈ����Ƃ�������Static�֐�����Ώۂ̃s�N�Z���t�H�[�}�b�g�����擾���Ă�������.
	*****************************************************************************/
	struct PixelFormatInfo
	{
	public:
		#pragma region Static Function
		/*!**********************************************************************
		*  @brief    �s�N�Z���t�H�[�}�b�g�����擾 (�ɗ�Const���g���悤�ɂ��Ă�������. �͈͊O�̏ꍇ�͑����I�����܂�)
		*  @return   �s�N�Z���t�H�[�}�b�g : Global�ȃt�H�[�}�b�g����擾���邽��, �l��ς���ꍇ�͐T�d�ɍs���Ă�������. 
		*************************************************************************/
		static PixelFormatInfo& Get(const PixelFormat format);

		/*!**********************************************************************
		*  @brief    �s�N�Z���t�H�[�}�b�g��Const�̒l�Ŏ擾
		*  @return   �s�N�Z���t�H�[�}�b�g���
		*************************************************************************/
		static const PixelFormatInfo& GetConst(const PixelFormat format);
		#pragma endregion

		#pragma region Public Function

		#pragma region Format Check
		/*!**********************************************************************
		*  @brief    �����t�H�[�}�b�g��
		*  @return   �����t�H�[�}�b�gUnsigned int��Signed int�ł����true
		*************************************************************************/
		bool IsInteger() const;

		/*!**********************************************************************
		*  @brief    Float�t�H�[�}�b�g��
		*  @return   Float�t�H�[�}�b�g�ł���ꍇ��true
		*************************************************************************/
		bool IsFloat() const ;

		/*!**********************************************************************
		*  @brief    HDR�Ŏg�p�����t�H�[�}�b�g��
		*  @return   HDR�Ŏg�p�ł���t�H�[�}�b�g (1.0�ȏ�̒l���i�[�o����t�H�[�}�b�g)�ł����true
		*************************************************************************/
		bool IsHDR() const;

		/*!**********************************************************************
		*  @brief    Depth��Stencil�Ŏg�p����Ă���t�H�[�}�b�g��
		*  @return   DepthStencil�Ŏg�p�ł���t�H�[�}�b�g�̏ꍇtrue
		*************************************************************************/
		bool IsDepthStencil() const;

		/*!**********************************************************************
		*  @brief    Stencil�̂ݎg�p����Ă���t�H�[�}�b�g��
		*  @return   Stencil�Ŏg�p�ł��� (uint8�̂悤�Ȃ���)�ꍇtrue
		*************************************************************************/
		bool IsStencil() const;

		/*!**********************************************************************
		*  @brief    �u���b�N���k�̃t�H�[�}�b�g�ł��邩�ǂ���
		*  @return   �e�N�X�`���̃s�N�Z�����u���b�N���Ƃɕ������Ĉ��k����t�H�[�}�b�g�̏ꍇtrue
		*************************************************************************/
		bool IsBlockCompressed() const;
		#pragma endregion

		#pragma endregion 

		#pragma region Public Member Variables
		/*! @brief �u���b�N���k�f�[�^�Ɏ�Ɏg�p���܂�. ���k�̃s�N�Z�����ł�(������)*/
		gu::int32 BlockSizeX = 0;

		/*! @brief �u���b�N���k�f�[�^�Ɏ�Ɏg�p���܂�. ���k�̃s�N�Z�����ł�(��������)*/
		gu::int32 BlockSizeY = 0;

		/*! @brief �u���b�N���k�f�[�^�Ɏ�Ɏg�p���܂�. ���k�̃s�N�Z�����ł�(���s����)*/
		gu::int32 BlockSizeZ = 0;

		/*! @brief �u���b�N���k�f�[�^�ɂ��o�C�g�T�C�Y. ��������1�s�N�Z���ɕK�v�ȃo�C�g�T�C�Y*/
		gu::int32 BlockBytes = 0;

		/*! @brief �`�����l���̃J�E���g�ł�. (RGBA�̏ꍇ, 4�����邱�ƂɂȂ�܂�)*/
		gu::int32 ChannelCount = 0;

		/*! @brief �Ώۃs�N�Z���t�H�[�}�b�g�ɑ΂���, �����\��GPU���\�[�X�̎g�������`*/
		PixelFormatCapabilities Capabilities = PixelFormatCapabilities::None;

		/*! @brief �v���b�g�t�H�[���Őݒ肷��s�N�Z���t�H�[�}�b�g (DirectX12��Vulkan��Enum�Ǘ��Ɏg�p���܂�.)*/
		gu::uint32 PlatformFormat = 0;

		/*! @brief �摜�f�[�^�̐ݒ��� ()*/
		PixelFormat Format = PixelFormat::Unknown;

		/*! @brief ����̃v���b�g�t�H�[���ƃ����_�����O�̑g�ݍ��킹�ŃT�|�[�g����Ă���t�H�[�}�b�g��*/
		bool Supported = true;

		#pragma endregion 

		#pragma region Public Operator 

		#pragma endregion 

		#pragma region Public Constructor and Destructor
		/*! @brief �f�t�H���g�R���X�g���N�^*/
		PixelFormatInfo() = default;

		/*! @brief �������Ɏg�p���܂�*/
		PixelFormatInfo(const PixelFormat format,
			const gu::int32 blockSizeX, const gu::int32 blockSizeY, const gu::int32 blockSizeZ,
			const gu::int32 blockBytes, const gu::int32 channelCount, const bool supported) :
			Format(format), BlockSizeX(blockSizeX), BlockSizeY(blockSizeY), BlockSizeZ(blockSizeZ),
			BlockBytes(blockBytes), ChannelCount(channelCount), Supported(supported) {};
		#pragma endregion 

	protected:
		#pragma region Protected Constructor and Destructor

		#pragma endregion 

		#pragma region Protected Function

		#pragma endregion 

		#pragma region Protected Member Variables

		#pragma endregion

	private:
		#pragma region Private Constructor and Destructor

		#pragma endregion

		#pragma region Private Function

		#pragma endregion 

		#pragma region Private Member Variables

		#pragma endregion 

	};

}
#endif