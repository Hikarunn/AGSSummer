#pragma once

// オブジェクトの属性
enum class ObjectAttribute
{
	Player,				// プレイヤー
	PlayerAttack,		// プレイヤーの攻撃範囲
	PlayerShot,			// プレイヤーのショット
	Enemy,				// 敵
	EnemyAttack,		// 敵の攻撃範囲
	Stage,				// ステージ
	Sky,				// 空
	Other				// それ以外
};