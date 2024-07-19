//////////////////////////////////////////////////////////////////////////////////
///  @file   SceneDownSample.hpp
///  @brief  描画シーンを縦横半分に縮小するシェーダ. ぼかしを入れることで品質を向上しています @n
///          使用マクロ @n
///          - USE_COMPUTE_SHADER (Switch) : コンピュートシェーダーを使用します. 使用する場合はTHREADGROUP_SIZE_X, Yを定義してください@n
///          - USE_LOW_QUALITY    (Switch) : ダウンサンプリングの品質を下げます. 下げる場合, ぼかしを入れません.
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
	/* @brief 描画シーンを縦横半分に縮小するシェーダ. ぼかしを入れることで品質を向上しています @n
	*  @param[in] USE_COMPUTE_SHADER (Switch) : コンピュートシェーダーを使用します. 使用する場合はTHREADGROUP_SIZE_X, Yを定義してください@n
	*  @param[in] FORCE_LOW_QUALITY  (Switch) : ダウンサンプリングの品質を下げます. 下げる場合, ぼかしを入れません.
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
		*  @brief     画面のサイズを変更します.
		*  @param[in] const gu::uint32 新しい幅
		*  @param[in] const gu::uint32 新しい高さ
		*  @return    void
		*************************************************************************/
		virtual void OnResize(const gu::uint32 newWidth, const gu::uint32 newHeight) override;

		/*!**********************************************************************
		*  @brief     コマンドリストに描画内容を登録します
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		virtual void Draw() override;

		#pragma endregion 

		#pragma region Public Property
		/*!**********************************************************************
		*  @brief     ピクセルシェーダーを使用するかどうか
		*  @param[in] void
		*  @return    bool
		*************************************************************************/
		__forceinline bool UsePixelShader() const { return !_useComputeShader; }

		/*!**********************************************************************
		*  @brief     コンピュートシェーダーを使用するかどうか
		*  @param[in] void
		*  @return    bool
		*************************************************************************/
		__forceinline bool UseComputeShader() const { return _useComputeShader; }

		#pragma endregion 

		#pragma region Public Operator 

		#pragma endregion 

		#pragma region Public Constructor and Destructor
		/*! @brief デフォルトコンストラクタ*/
		SceneDownSample();

		/*! @brief デストラクタ*/
		~SceneDownSample();

		/*! @brief エンジンを使った初期化*/
		SceneDownSample(const LowLevelGraphicsEnginePtr& engine, const gu::tstring& useMacro, const gu::tstring& addName = SP(""));

		#pragma endregion 

	protected:
		#pragma region Protected Constructor and Destructor

		#pragma endregion 

		#pragma region Protected Function
		/*!**********************************************************************
		*  @brief     パイプラインステートを準備します.
		*  @param[in] const gu::tstring& デバッグ表示名
		*  @return    void
		*************************************************************************/
		virtual void PreparePipelineState(const gu::tstring& addName) override;

		/*!**********************************************************************
		*  @brief     Texture情報のバッファを準備します.
		*  @param[in] const gu::tstring& デバッグ表示名
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
		/*! @brief Compute Shaderを使用するか*/
		bool _useComputeShader = false;

		/*! @brief Texture情報*/
		SceneTextureInfo _sceneTextureInfo = {};

		/*! @brief Compute Shaderを使用するかのマクロ*/
		static constexpr gu::tchar* USE_COMPUTE_SHADER_MACRO = SP("USE_COMPUTE_SHADER_MACRO");

		/*! @brief ぼかしを使用しないダウンサンプリングを使用するか*/
		static constexpr gu::tchar* USE_LOW_QUALITY_MACRO    = SP("USE_LOW_QUALITY");

		/*! @brief ComputeShader使用時のスレッドグループ数*/
		static constexpr gu::uint32 THREAD_GROUP_SIZE = 8;
		#pragma endregion 

	};
}

#endif