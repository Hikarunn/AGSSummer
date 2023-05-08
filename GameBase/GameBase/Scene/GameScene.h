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

// 敵との距離と角度を確保
struct Rader
{
	float angle;
	float range;
	bool flag;
};
class GameScene :
	public BaseScene
{
public:
	GameScene(StageID stageID);
	~GameScene();

	// ライトカメラの位置

	// ライトカメラ注視点

	// レーダーに敵を表示する範囲

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
	void RaderSetUp(void);

	/// <summary>
	/// レーダーの更新
	/// </summary>
	/// <param name="pPos"></param>
	void RaderUpdate(Vector2 pPos);

	/// <summary>
	/// レーダーの描画
	/// </summary>
	/// <param name=""></param>
	void RaderDrow(void);

	// オフスクリーン
	int offScreen_;
	// スカイドームのみのスクリーン
	int skyScreen_;

	// スカイドームとステージのみのスクリーン
	int subScreen_;

	// レーダー用のスクリーン
	int raderMap_;

	// プレイヤーとエネミーの距離と角度をエネミーの数だけ取得
	std::map<int, Rader>rader_;

	// レーダーのサイズ
	int raderSize_;

	Result result_;

	// 現在のステージID
	StageID stageID_;

	int radarGraph_;

	friend class PauseScene;
};

