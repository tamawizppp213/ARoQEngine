//////////////////////////////////////////////////////////////////////////////////
///  @file   SceneDownSample.hpp
///  @brief  �`��V�[�����c�������ɏk������V�F�[�_. �ڂ��������邱�Ƃŕi�������サ�Ă��܂� @n
///          �g�p�}�N�� @n
///          - USE_COMPUTE_SHADER (Switch) : �R���s���[�g�V�F�[�_�[���g�p���܂�. �g�p����ꍇ��THREADGROUP_SIZE_X, Y���`���Ă�������@n
///          - USE_LOW_QUALITY    (Switch) : �_�E���T���v�����O�̕i���������܂�. ������ꍇ, �ڂ��������܂���.
///  @author toide
///  @date   2024/07/18 0:43:02
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef SCENE_DOWN_SAMPLE_HPP
#define SCENE_DOWN_SAMPLE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "FullScreenEffector.hpp"
#include "GameUtility/Math/Include/GMVector.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace engine
{
	/****************************************************************************
	*				  			   SceneDownSample
	****************************************************************************/
	/* @brief �`��V�[�����c�������ɏk������V�F�[�_. �ڂ��������邱�Ƃŕi�������サ�Ă��܂� @n
	*  @param[in] USE_COMPUTE_SHADER (Switch) : �R���s���[�g�V�F�[�_�[���g�p���܂�. �g�p����ꍇ��THREADGROUP_SIZE_X, Y���`���Ă�������@n
	*  @param[in] FORCE_LOW_QUALITY  (Switch) : �_�E���T���v�����O�̕i���������܂�. ������ꍇ, �ڂ��������܂���.
	*****************************************************************************/
	class SceneDownSample : public IFullScreenEffector
	{
	private:
		struct SceneTextureInfo
		{
			gm::Float2 SceneTextureInvertSize;
			gu::uint32 ViewportMax[2];
			gu::uint32 ViewportMin[2];
		};

	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief     ��ʂ̃T�C�Y��ύX���܂�.
		*  @param[in] const gu::uint32 �V������
		*  @param[in] const gu::uint32 �V��������
		*  @return    void
		*************************************************************************/
		virtual void OnResize(const gu::uint32 newWidth, const gu::uint32 newHeight) override;

		/*!**********************************************************************
		*  @brief     �R�}���h���X�g�ɕ`����e��o�^���܂�
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		virtual void Draw() override;

		#pragma endregion 

		#pragma region Public Property
		/*!**********************************************************************
		*  @brief     �s�N�Z���V�F�[�_�[���g�p���邩�ǂ���
		*  @param[in] void
		*  @return    bool
		*************************************************************************/
		__forceinline bool UsePixelShader() const { return !_useComputeShader; }

		/*!**********************************************************************
		*  @brief     �R���s���[�g�V�F�[�_�[���g�p���邩�ǂ���
		*  @param[in] void
		*  @return    bool
		*************************************************************************/
		__forceinline bool UseComputeShader() const { return _useComputeShader; }

		#pragma endregion 

		#pragma region Public Operator 

		#pragma endregion 

		#pragma region Public Constructor and Destructor
		/*! @brief �f�t�H���g�R���X�g���N�^*/
		SceneDownSample();

		/*! @brief �f�X�g���N�^*/
		~SceneDownSample();

		/*! @brief �G���W�����g����������*/
		SceneDownSample(const LowLevelGraphicsEnginePtr& engine, const gu::tstring& useMacro, const gu::tstring& addName = SP(""));

		#pragma endregion 

	protected:
		#pragma region Protected Constructor and Destructor

		#pragma endregion 

		#pragma region Protected Function
		/*!**********************************************************************
		*  @brief     �p�C�v���C���X�e�[�g���������܂�.
		*  @param[in] const gu::tstring& �f�o�b�O�\����
		*  @return    void
		*************************************************************************/
		virtual void PreparePipelineState(const gu::tstring& addName) override;

		/*!**********************************************************************
		*  @brief     Texture���̃o�b�t�@���������܂�.
		*  @param[in] const gu::tstring& �f�o�b�O�\����
		*  @return    void
		*************************************************************************/
		void PrepareTextureInfoBuffer(const gu::tstring& addName);

		#pragma endregion 

		#pragma region Protected Property

		#pragma endregion

	private:
		#pragma region Private Constructor and Destructor

		#pragma endregion

		#pragma region Private Function

		#pragma endregion 

		#pragma region Private Property
		/*! @brief Compute Shader���g�p���邩*/
		bool _useComputeShader = false;

		/*! @brief Texture���*/
		SceneTextureInfo _sceneTextureInfo = {};

		/*! @brief Compute Shader���g�p���邩�̃}�N��*/
		static constexpr gu::tchar* USE_COMPUTE_SHADER_MACRO = SP("USE_COMPUTE_SHADER_MACRO");

		/*! @brief �ڂ������g�p���Ȃ��_�E���T���v�����O���g�p���邩*/
		static constexpr gu::tchar* USE_LOW_QUALITY_MACRO    = SP("USE_LOW_QUALITY");

		/*! @brief ComputeShader�g�p���̃X���b�h�O���[�v��*/
		static constexpr gu::uint32 THREAD_GROUP_SIZE = 8;
		#pragma endregion 

	};
}

#endif