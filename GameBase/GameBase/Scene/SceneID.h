#pragma once

// スクリーンのID 
enum class SceneID
{
	Title,			// タイトル 
	Select,			// 選択シーン
	Game,			// プレイ中
	Result,			// リザルト
	Pause,			// ポーズ画面
	Option,			// オプション
	Transition,		// 遷移
	Loading,		// ロードシーン
	InputSet,		// キーの設定シーン
	Dialog,			// 選択事項の可否確認
	Tutorial,		// チュートリアル
}; 
enum class Result
{
	Clear,
	GameOver,
	Max,
};