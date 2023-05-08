#pragma once

// 吸っクリーンの状態用ID
enum class ScreenID
{
	Title,			// タイトル 
	Select,			// 選択シーン
	SelectUI,		// 選択シーンのUI表示
	Game,			// プレイ中
	Result,			// リザルト
	Pause,			// ポーズ画面
	Option,			// オプション
	OptionUI,		// オプションのUI表示
	Transition,		// 遷移
	Loading,		// ロードシーン
	InputSet,		// キーの設定シーン
	Dialog,			// 選択事項の可否確認
	DialogUI,
	Tutorial,		// チュートリアル
};