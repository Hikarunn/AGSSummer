#pragma once
#include <DxLib.h>
#include <vector>
#include "BaseScene.h"
#include "../Common/Shared.h"
#include "StageID.h"

struct DepthParameter
{
	float start;
	float end;
	float scope;
};

struct WEIGHT_NUM
{
	float weight[8];
};
//
//// 敵との距離と角度を確保
//struct Radar
//{
//	float angle;
//	float range;
//	bool flag;
//};
class GameScene :
	public BaseScene
{
public:
	GameScene(StageID stageID);
	~GameScene();
	// ライトカメラの位置
	static constexpr Vector3 camPos{ 500.0f, 800.0f, -3000.0f };
	// ライトカメラの注視点
	static constexpr Vector3 camTar{ 0.0f, 0.0f, -1000.0f };
	// ライトカメラの正射影の表示範囲
	static constexpr float offsetOrtho = 12000.0f;
	// ライトカメラの手前の距離と奥の距離
	static constexpr float offsetNear = 0.001f;
	static constexpr float offsetFar = 13000.0f;
	// ガウシアンブラの重さ
	static constexpr int NUM_WEIGHTS = 8;

	// ぼかし無しとぼかしありの補間距離
	static constexpr float dofInterpSize = 7000;
	// ぼかし無しの範囲の中心位置
	static constexpr float dofFocus = 500.0;
	// ぼかし無しの範囲
	static constexpr float dofFocusSize = 17000;

	// ミニマップに敵を表示する範囲
	static constexpr float RADAR_RANGE = 90;


	void SetResult(Result result)
	{
		result_ = result;
	}

	/// <summary>
	/// ゲームに戻るときに挟む処理
	/// </summary>
	/// <param name=""></param>
	void SetUp(void);
	

private:
	const SceneID GetID(void)const
	{
		return SceneID::Game;
	}

	/// <summary>
	/// リザルトの生成
	/// </summary>
	/// <param name="own"></param>
	/// <returns></returns>
	SceneUptr MakeResultFunc(SceneUptr own);

	/// <summary>
	/// ポーズシーンの生成
	/// </summary>
	/// <param name="own"></param>
	/// <returns></returns>
	SceneUptr MakePauseFunc(SceneUptr own);

	/// <summary>
	/// セレクトシーンの生成
	/// </summary>
	/// <param name="own"></param>
	/// <returns></returns>
	SceneUptr MakeSelectFunc(SceneUptr own);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="deltaa"></param>
	/// <param name="controller"></param>
	void Update(float deltaa, Controller& controller)final;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name=""></param>
	void DrawScene(void)final;

	/// <summary>
	/// シャドウマップの作成
	/// </summary>
	/// <param name=""></param>
	void SetupShadowMap(void);
	
	/// <summary>
	/// 被写界深度用の深度テクスチャ
	/// </summary>
	/// <param name=""></param>
	//void SetUpDepth(void);

	/// <summary>
	/// ポストエフェクトで使うスクリーンの作成
	/// </summary>
	/// <param name=""></param>
	void SetOffsetScreen(void);

	/// <summary>
	/// メイン以外のスクリーン
	/// </summary>
	/// <param name=""></param>
	void SetSubScreen(void);

	/// <summary>
	/// ロードが完了しているか
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	bool IsLoaded(void)final;

	/// <summary>
	/// ロード完了時の処理
	/// </summary>
	/// <param name="controller"></param>
	void Loaded(Controller& controller);

	/// <summary>
	/// レーダーのセットアップ
	/// </summary>
	/// <param name=""></param>
	/*void RadarSetUp(void);

	/// <summary>
	/// レーダーの更新
	/// </summary>
	/// <param name="pPos"></param>
	void RadarUpdate(Vector2 pPos);

	/// <summary>
	/// レーダーの描画
	/// </summary>
	/// <param name=""></param>
	void RadarDraw(void);*/

	// シャドウマップ用の深度テクスチャ
	// シャドウマップ用シェーダ
	SharedShaderHandle shadowPs_;
	int shadowMap_;
	LIGHT_MAT* lightMat_;
	LIGHT_MAT lightMat;
	int shadowBuff_;

	// 被写界深度用の深度テクスチャ
	// 深度テクスチャ用シェーダ
	//SharedShaderHandle depthPS_;
	//int depth_;
	//DepthParameter* depthMat_;
	//DepthParameter depthMat;
	//int depthbuffer_;

	float dofTotalSize_;


	// オフスクリーン
	int offScreen_;
	// スカイドームのみのスクリーン
	int skyScreen_;

	// スカイドームとステージのみのスクリーン
	int subScreen_;

	// レーダー用のスクリーン
	int radarMap_;

	// プレイヤーとエネミーの距離と角度をエネミーの数だけ取得
	//std::map<int, Radar>radar_;

	// 作成したmapの数を格納
	int count_;
	// レーダーのサイズ
	int radarSize_;

	Result result_;

	SharedRenderTargetHandle resultCapture_;

	// 現在のステージID
	StageID stageID_;

	int radarGraph_;

	friend class PauseScene;
};

