//////////////////////////////////////////////////////////////////////////////////
///             @file   ScreenSpaceReflection.hpp
///             @brief  �|�X�g�G�t�F�N�g�I�ɔ��˂�\������
///             @author Toide Yutaro
///             @date   2023_10_17
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef SCREEN_SPACE_REFLECTION_HPP
#define SCREEN_SPACE_REFLECTION_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "FullScreenEffector.hpp"
#include "GameUtility/Math/Include/GMVector.hpp"
#include "GameUtility/Base/Include/GUMemory.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace gc
{
	struct SSRSettings
	{
		int   MaxRayMarchIteraction    = 16; // ���C�}�[�`���O���s���ő�̎��s��
		int   MaxBinarySearchIteration = 16; // �񕪒T�����s�����s��
		float MaxRayDistance = 100;           // ���C�̍ő勗��
		float RayThicknessInViewSpace = 10;  // ���C�̌���
		float MaxFadeDistance         = 50;  // �ő�̋����t�F�[�h���s������
	};

	/****************************************************************************
	*				  			ScreenSpaceReflection
	*************************************************************************//**
	*  @class     ScreenSpaceReflection
	*  @brief     �|�X�g�G�t�F�N�g�I�ɔ��˂�\������
	*****************************************************************************/
	class ScreenSpaceReflection : public NonCopyable
	{
	protected:
		using LowLevelGraphicsEnginePtr = gu::SharedPointer<LowLevelGraphicsEngine>;
		using ResourceViewPtr           = gu::SharedPointer<rhi::core::GPUResourceView>;
		using PipelineStatePtr          = gu::SharedPointer<rhi::core::GPUGraphicsPipelineState>;
		using VertexBufferPtr           = gu::SharedPointer<rhi::core::GPUBuffer>;
		using IndexBufferPtr            = gu::SharedPointer<rhi::core::GPUBuffer>;
		using ResourceLayoutPtr         = gu::SharedPointer<rhi::core::RHIResourceLayout>;
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/* @brief : Resize frame buffer (Not implement)*/
		void OnResize(int newWidth, int newHeight);

		/*@brief : Render to back buffer*/
		void Draw(const ResourceViewPtr& scene);

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		int GetMaxRayMarchIteration() const { return _settings.MaxRayMarchIteraction; }
		
		int GetMaxBinarySearchIteration() const { return _settings.MaxBinarySearchIteration; }
		
		float GetMaxRayDistance() const { return _settings.MaxRayDistance; }
		
		float GetMaxFadeDistance() const { return _settings.MaxFadeDistance; }
		
		float GetRayThicknessInViewSpace() const { return _settings.RayThicknessInViewSpace; }

		void SetMaxRayMarchIteration(const int iteration) { _isSettingChanged = true; _settings.MaxRayMarchIteraction = iteration; }

		void SetMaxBinarySearchIteration(const int iteration) { _isSettingChanged = true; _settings.MaxBinarySearchIteration = iteration; }

		void SetMaxRayDistance(const float distance) { _isSettingChanged = true; _settings.MaxRayDistance = distance; }

		void SetMaxFadeDistance(const float distance) { _isSettingChanged = true; _settings.MaxFadeDistance = distance; }

		void SetRayThicknessInViewSpace(const float thickness) { _isSettingChanged = true; _settings.RayThicknessInViewSpace = thickness; }

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		ScreenSpaceReflection();

		~ScreenSpaceReflection();

		ScreenSpaceReflection(const LowLevelGraphicsEnginePtr& engine, const ResourceViewPtr& normalMap, const ResourceViewPtr& depthMap, const SSRSettings& settings, const std::wstring& addName = L"");

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		void PrepareVertexAndIndexBuffer(const std::wstring& name);
		
		void PrepareBuffer(const SSRSettings& setting, const std::wstring& addName);

		void PreparePipelineState(const std::wstring& addName);

		void PrepareResourceView();

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		SSRSettings _settings = {};

		ResourceViewPtr _settingsView = nullptr;

		bool _isSettingChanged = false;

		/* @brief : Normal texture*/
		ResourceViewPtr _normalMap = nullptr;

		/* @brief : Linear depth texture*/
		ResourceViewPtr _depthMap = nullptr;

		ResourceLayoutPtr _resourceLayout = nullptr;
		PipelineStatePtr _pipeline = nullptr;

		std::vector<VertexBufferPtr> _vertexBuffers = {};

		std::vector<IndexBufferPtr>  _indexBuffers = {};

		LowLevelGraphicsEnginePtr _engine = nullptr;

	};
}
#endif