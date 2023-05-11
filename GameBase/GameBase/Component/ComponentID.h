#pragma once


enum class ComponentID :
	unsigned long long
{
	Base = 1ull,									// すべての基底
	Info = Base | (1ull << 1),						// オブジェクトの情報
	Transform = Base | (1ull << 2),				// トランスフォーム
	RenderBase = Base | (1ull << 3),				// 描画の基底
	Animator = Base | (1ull << 4),					// アニメーション
	Collider = Base | (1ull << 5),					// 当たり判定
	BehaviorBase = Base | (1ull << 6),				// オブジェクトの動き
	ModelRender = RenderBase | (1ull << 7),		// modelの描画
	SoundSpeaker = Base | (1ull << 8),				// サウンドの再生
	CapsuleRender = RenderBase | (1ull << 9),		// カプセルの描画
	PlayerBehavior = BehaviorBase | (1ull << 10),	// プレイヤーの動き
	MeshCollider = Collider | (1ull << 11),		// メッシュでの当たり判定
	SphereCollider = Collider | (1ull << 12),		// 球体での当たり判定
	CapsuleCollider = Collider | (1ull << 13),		// カプセルでの当たり判定
	CharactorCollider = Collider | (1ull << 14),	// キャラクターの当たり判定
	TerrainCollider = Collider | (1ull << 15),		// 地形の当たり判定
	EnemyBehavior = BehaviorBase | (1ull << 16),	// 敵の動き
	EnemyBulletBehavior = BehaviorBase | (1ull << 17),		// 敵の弾の動き
	StageBehavior = BehaviorBase | (1ull << 18),			// ステージの制御
	PlayerShotBehavior = BehaviorBase | (1ull << 19),	// プレイヤーの攻撃
	PlayerAttackBehavior = BehaviorBase | (1ull << 20),	// プレイヤーの攻撃
	TutorialBehavior = BehaviorBase | (1ull << 21),		// チュートリアル制御用
	BillBoardRender = RenderBase | (1ull << 22),			// ビルボード描画用
	CameraBehavior = BehaviorBase | (1ull << 23),			// カメラ
	SkyBehavior = BehaviorBase | (1ull << 24),				// スカイドームの制御用
	LineRender = RenderBase | (1ull << 25),					// 線の描画用
	EnemyLaserSightBehavior = BehaviorBase | (1ull << 26),		// 敵の攻撃時に出る視線
	reserve4 = 1ull << 27,
	Max = ~Base
};
