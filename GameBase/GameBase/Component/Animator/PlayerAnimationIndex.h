#pragma once

// アニメーション名
enum class AnimIndex {
	Idle,							// 待ち
	ForwardMove,					// 前身
	Shot,							// 片手のショット
	FullShot,						// ショットによるフルアッタック
	Jamp,							// ジャンプ
	Fall,							// 落下
	TouchDown,						// 着地
	GrassHopper,					// ハイジャンプ用スキル
	Bagworm,						// 隠密マント着脱モーション
	IdleSwordOn,					// 待ち(抜刀)
	ForwardMoveSwordOn,				// 全身(抜刀)
	ShotSwordOn,					// 片手のショット(抜刀)
	JampSwordOn,					// ジャンプ(抜刀)
	FallSwordOn,					// 落下
	TouchDownSwordOn,				// 着地(抜刀)
	GrassHopperSwordOn,				// ハイジャンプ用スキル(抜刀)
	bagwormSwordOn,					// 隠密マント着脱モーション(抜刀)
	Attack1SwordOn,					// 刀攻撃１コンボ目(抜刀)
	Attack2SwordOn,					// 刀攻撃２コンボ目(抜刀)
	Attack3SwordOn,					// 刀攻撃３コンボ目(抜刀)
	IaiSword,						// 居合(旋空)
	Max
};
