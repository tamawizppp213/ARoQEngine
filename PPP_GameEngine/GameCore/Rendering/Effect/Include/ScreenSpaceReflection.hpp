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
		int   MaxRayMarchIteraction; // ���C�}�[�`���O���s���ő�̎��s��
		int   MaxBinarySearchIteration; // �񕪒T�����s�����s��
		float MaxRayDistance;           // ���C�̍ő勗��
		float RayThicknessInViewSpace;  // ���C�̌���
		float MaxFadeDistance;          // �ő�̋����t�F�[�h���s������
	};

	/****************************************************************************
	*				  			ScreenSpaceReflection
	*************************************************************************//**
	*  @class     ScreenSpaceReflection
	*  @brief     �|�X�g�G�t�F�N�g�I�ɔ��˂�\������
	*****************************************************************************/
	class ScreenSpaceReflection : public IFullScreenEffector
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/* @brief : Resize frame buffer (Not implement)*/
		void OnResize(int newWidth, int newHeight) override;

		/*@brief : Render to back buffer*/
		void Draw() override;

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		int GetMaxRayMarchIteration() const { return _settings.MaxRayMarchIteraction; }
		
		int GetMaxBinarySearchIteration() const { return _settings.MaxBinarySearchIteration; }
		
		float GetMaxRayDistance() const { return _settings.MaxRayDistance; }
		
		float GetMaxFadeDistance() const { return _settings.MaxFadeDistance; }
		
		float GetRayThicknessInViewSpace() const { return _settings.RayThicknessInViewSpace; }

		void SetMaxRayMarchIteration(const int iteration) { _isSettingChanged = true; _settings.MaxRayMarchIteraction; }

		void SetMaxBinarySearchIteration(const int iteration) { _isSettingChanged = true; _settings.MaxBinarySearchIteration; }

		void SetMaxRayDistance(const float distance) { _isSettingChanged = true; _settings.MaxRayDistance; }

		void SetMaxFadeDistance(const float distance) { _isSettingChanged = true; _settings.MaxFadeDistance; }

		void SetRayThicknessInViewSpace(const float thickness) { _isSettingChanged = true; _settings.RayThicknessInViewSpace; }

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		ScreenSpaceReflection();

		~ScreenSpaceReflection();

		ScreenSpaceReflection(const LowLevelGraphicsEnginePtr& engine, const SSRSettings& settings, const std::wstring& addName = L"");

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		void PrepareBuffer(const SSRSettings& setting, const std::wstring& addName);

		void PreparePipelineState(const std::wstring& addName) override;

		void PrepareResourceView() override;

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		SSRSettings _settings = {};

		bool _isSettingChanged = false;
	};
}
#endif