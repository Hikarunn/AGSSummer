#include "../../Scene/BaseScene.h"
#include "PlayerBehavior.h"
#include "../../Common/Input/InputType/Keyboard.h"
#include "../../Common/Input/InputType/Pad.h"
#include "../../Common/Debug.h"
#include "../../Common/Math.h"
#include "../../Common/Camera.h"
#include "../Collider/CharactorCollider.h"
#include "../Info/ObjectInfo.h"
#include "EnemyBehavior.h"
#include "PlayerAttackBehavior.h"
#include "PlayerShotBehavior.h"
#include "CameraBehavior.h"

PlayerBehavior::PlayerBehavior(void)
{
	Init();
}

bool PlayerBehavior::Init(void)
{
	// 移動
	dodgeTimer_ = 0.0f;
	noDashTime_ = 0.0f;
	isStaminaLoss_ = false;
	// 回転
	angles_ = Vector3{ Deg2Rad(0.0f),Deg2Rad(dir_.up),Deg2Rad(0.0f) };
	// 攻撃
	atkLimit_ = false;
	isAttack_ = false;
	isDodge_ = false;
	combo_ = 0;
	canSkill_ = false;
	isSkill_ = false;
	// アニメーション
	anim_.push_back(std::pair(AnimIndex::Idle, animtime_.idle));
	anim_.push_back(std::pair(AnimIndex::ForwardMove, animtime_.move));
	anim_.push_back(std::pair(AnimIndex::Attack1SwordOn, animtime_.attack1));
	anim_.push_back(std::pair(AnimIndex::Attack2SwordOn, animtime_.attack2));
	anim_.push_back(std::pair(AnimIndex::Attack3SwordOn, animtime_.attack3));
	// ゲージ
	gauge_.emplace(UiID::Torion, std::pair(200.0f, std::pair(0.0f, 200.0f)));
	atkCnt_ = 0;
	// サウンド
	/*for (int i = 0; i < static_cast<int>(SOUNDNAME_SE::Max); i++)
	{
		sound_.push_back(std::pair(static_cast<SOUNDNAME_SE>(i), false));
	}*/


	return true;
}

void PlayerBehavior::Begin(ObjectManager& objectManager)
{
	// Y座標地面固定
	transform_ = objectManager.GetComponent<Transform>(ownerId_);
	// モデル情報
	auto modelrender = objectManager.GetComponent<ModelRender>(ownerId_);
	auto& handle = modelrender->GetHandle();
	// アニメーション
	animation_ = objectManager.GetComponent<Animator>(ownerId_);
	for (int i = 0; i <= 13; i++)
	{
		animation_->AddAnimation(i, false);
	}
	// 初期アニメーションセット
	animation_->SetNextAnimation(static_cast<int>(AnimIndex::Idle));
	// 当たり判定
	collider_ = objectManager.GetComponent<CharactorCollider>(ownerId_);
	collider_->SetHitFunc(std::bind(&PlayerBehavior::OnHit, this, std::placeholders::_1, std::placeholders::_2));
	// 音関係
	//lpSooundPross.PlayBackSound(SOUNDNAME_SE::playerMove, lpSooundPross.GetVolume(), true);

	
	camera_ = objectManager.GetComponent<Transform>(objectManager.GetCameraID());

	float_.groundPosY = transform_->Pos().y;
}

void PlayerBehavior::Update(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller)
{
	// 地面の高さ調整
	CorrectionGroundPos();

	if (controller.Pressed(InputID::Dash)) {}

	// 操作制御
	auto speed = ProcessDash(controller, delta);
	collider_->SetSpeed(speed);
	auto rot = ProcessMove(scene, objectManager, controller, speed, delta);
	ProcessRotate(objectManager, scene, controller, rot);
	ProcessJump(controller);

	if (float_.canFloat)
	{
		ProcessFloat();

	}
	ProcessAttack(controller, objectManager);
	ProcessStamina(controller);
	if (isStaminaLoss_)
	{
		// クールタイム
		CoolTimer(delta);
	}

	//ProcessSkill(controller, objectManager);
	// アニメーション制御
	Animation(controller, objectManager);
	// 音関係制御
	//Sound();
}

float PlayerBehavior::ProcessMove(BaseScene& scene, ObjectManager& objectManager, Controller& controller, float speed, float delta)
{
	auto rot = controller.GetLeftInput().GetAngle();
	// 必殺技発動中は移動自体をさせない


	// アニメーション移動量
	auto animMove = animation_->GetAnimState()->GetMovePosition();
	transform_->Pos().x += sinf(angles_.y) * animMove.z;
	transform_->Pos().z += cosf(angles_.y) * animMove.z;

	// 攻撃中は通常移動させない
	//if (atkLimit_)
	//{
	//	return rot;
	//}

	// 通常移動
	if (controller.GetLeftInput().SqMagnitude() > 0.0f)
	{
		auto camAngle = camera_->GetRotation().ToEuler();
		transform_->Pos().x -= sinf(camAngle.y + rot) * speed;
		transform_->Pos().z -= cosf(camAngle.y + rot) * speed;
		float_.canFloat = false;
	
	}
	else
	{
		float_.canFloat = true;
		

	}




	// リスナーの位置と向きを設定
	//auto pPos = VGet(transform_->GetPos().x, transform_->GetPos().y, transform_->GetPos().z);
	//auto pFpos = VGet(transform_->GetForward().x, transform_->GetForward().y, transform_->GetForward().z);

	//// 
	//Set3DSoundListenerPosAndFrontPos_UpVecY(pPos, pFpos);

	//// リスナーの速度を設定
	//Set3DSoundListenerVelocity(VScale(VSub(pPos, pFpos), 60.0f));

	return rot;
}
//
//void PlayerBehavior::ProcessMoveSkill(float interval, float delta)
//{
//	
//	// ビームの反動で後に下がる
//	transform_->Pos().x += sinf(angles_.y);
//	transform_->Pos().z += cosf(angles_.y);
//}

float PlayerBehavior::ProcessDash(Controller& controller, const float& delta)
{
	auto speed = movedis_.walk;
	if (isStaminaLoss_)
	{
		return speed;
	}

	// 回避制御
	if (controller.Pressed(InputID::Dash))
	{
		isDodge_ = true;
		isDash_ = true;
		}
	else if (dodgeTimer_ > DodgeLimit)
	{
		isDodge_ = false;
	}
	else if (controller.Released(InputID::Dash))
	{
		isDodge_ = false;
		isDash_ = false;
	}
	// ダッシュ制御
	if (controller.Press(InputID::Dash) && (controller.GetLeftInput().SqMagnitude() > 0.0f))
	{
		speed = isDodge_ ? movedis_.dodge : movedis_.dash;									// 回避の有無による移動量計算
		(dodgeTimer_ >= 0.0f && isDodge_) ? dodgeTimer_ += delta : dodgeTimer_ = 0.0f;		// 回避時間中及び、回避中ならタイマー経過
	}
	else if (controller.Released(InputID::Dash))
	{
	
	}

	return speed;
}

void PlayerBehavior::ProcessRotate(ObjectManager& objectManager, BaseScene& scene, Controller& controller, float rot)
{
	// モデルの角度を設定
	transform_->SetRotation({ angles_.x, angles_.y, angles_.z });
	if (!(controller.GetLeftInput().SqMagnitude() > 0.0f))
	{
		return;
	}

	// 移動方向にキャラクターの角度を徐々に変える
	float radUnitAnglesY = angles_.y;
	auto camAngle = camera_->GetRotation().ToEuler();

	float radMoveAnglesY = RadIn2PI(camAngle.y + rot);

	// 回転量が少ない方の回転方向を取得する（時計回り：１、反時計回り：ー１）
	float aroundDir = DirNearAroundRad(radUnitAnglesY, radMoveAnglesY);

	// true:しきい値以内に合わせる false:回転量を加える
	float diff = abs(radMoveAnglesY - radUnitAnglesY);
	//auto rotRad = (isSkill_ || atkLimit_) ? SpeedRotRad / 10 : SpeedRotRad;
	auto rotRad = (isSkill_) ? SpeedRotRad / 10 : SpeedRotRad;
	angles_.y = (diff < 0.1f && !isSkill_) ? radMoveAnglesY : angles_.y + (rotRad * aroundDir);

	// angles_.yも0~360度以内に収める
	angles_.y = RadIn2PI(angles_.y);
	collider_->SetMoveDir(-transform_->GetForward());
}


void PlayerBehavior::ProcessJump(Controller& controller)
{
	// ジャンプ制御
	auto hoverUD = (controller.Press(InputID::Jump) && !isStaminaLoss_) ? 1			// Up
		: !collider_->IsGround() ? -1												// Down
		: 0;
	if (hoverUD)
	{
		// 速度更新
		if ((transform_->Pos().y > 0.0f && hoverUD == -1) || (transform_->Pos().y < HoverPosYMax && hoverUD == 1))
		{
			jump_.vel += jump_.acc * hoverUD;
			collider_->SetGravityPow(jump_.vel);
		
		}
		collider_->SetGroundFlag(false);
		// 座標更新
		transform_->Pos().y = Clamp(transform_->Pos().y + transform_->GetUp().y * jump_.vel, 0.0f, HoverPosYMax);
		// ジャンプ中は浮遊処理させない
		float_.canFloat = false;
		
	}
	else
	{
		// ジャンプしていない時		
		jump_.vel = 0.0f;
		if (float_.canFloat)
		{
			float_.canFloat = true;
		}
	}

	
}

void PlayerBehavior::ProcessFloat(void)
{
	auto floatUD = (float_.isUp) ? 1 : -1;									// Up : Down
	float_.vel += float_.acc * floatUD;										// 浮遊速度計算
	auto disY = Clamp(transform_->GetUp().y * float_.vel, -0.14f, 0.14f);	// 浮遊ふれ幅計算
	transform_->Pos().y += disY;											// 座標更新
	// 浮遊方向切り替え
	float_.isUp = (disY >= 0.14) ? false									// 下降させる
		: (disY <= -0.14) ? true											// 上昇させる
		: float_.isUp;														// 現状維持

}

void PlayerBehavior::ProcessAttack(Controller& controller, ObjectManager& objectManager)
{
	if (controller.Pressed(InputID::MainTrigerAttack) && !isSkill_ && !isAttack_)
	{
		// コンボ攻撃
		auto animPlayTime = animation_->GetAnimState()->GetPlayTime();
		auto animTotalTime = animation_->GetAnimState()->GetTotalTime();
		if (animPlayTime >= animTotalTime)
		{
			auto id = objectManager.CreateFromFactory(FactoryID::PlayerAttack, ownerId_, objectManager.GetComponent<Transform>(ownerId_)->GetPos());
			objectManager.Begin(id);
			auto attack = objectManager.GetComponent<PlayerAttackBehavior>(id);
			if (!(atkCnt_ % AtkAnimMax))
			{
				// 最初の攻撃は当たり判定出現までに溜めの時間が必要
				attack->SetAttackStartTime(0.5f);
				// 攻撃再生時間
				attack->SetAttackTime((animTotalTime - 30.0f) / 30.0f);
			}
			else
			{
				// 攻撃再生時間
				attack->SetAttackTime(animTotalTime / 30.0f);
			}
			// アニメーションインデックス
			atkCnt_++;
			atkLimit_ = true;
			isAttack_ = true;
			// SE
		}
	}
}

void PlayerBehavior::ResetCombo(void)
{
	atkCnt_ = 0;
	combo_ = 0;
	atkLimit_ = false;
}
//
void PlayerBehavior::ProcessStamina(Controller& controller)
{
//	//auto& value = gauge_.at(UiID::Stamina);
//	//auto& [min, max] = value.second;
//	//auto dashFlag = (controller.Press(InputID::Dash) && controller.GetLeftInput().SqMagnitude() > 0.0f && !isStaminaLoss_);
//	//// ダッシュ
//	//value.first = (dashFlag && isDodge_) ? value.first - RiseValue				// 回避
//	//	: dashFlag ? value.first - RiseValue * 0.5								// ダッシュ
//	//	: value.first;															// 回復
//	//// ジャンプ
//	//auto jumpFlag = (controller.Press(InputID::Jump) && !isStaminaLoss_);
//	//value.first = (jumpFlag && jump_.vel >= 15.0f) ? value.first - RiseValue	// ジャンプ加速後
//	//	: (jumpFlag && jump_.vel < 15.0f) ? value.first - RiseValue * 0.5		// ジャンプ加速中
//	//	: value.first;															// 回復
//
//	//if ((!controller.Press(InputID::Jump) && !(controller.Press(InputID::Dash) && (controller.GetLeftInput().SqMagnitude() > 0.0f))) || isStaminaLoss_)
//	//{
//	//	value.first += RiseValue;
//	//}
//	//value.first = Clamp(value.first, min, max);
//	//// スタミナ切れ
//	//if (value.first <= min)
//	//{
//	//	isStaminaLoss_ = true;
//	//
//	//}
}

void PlayerBehavior::CoolTimer(float& delta)
{
	// クールタイム
	noDashTime_ += delta;
	if (noDashTime_ >= CoolTime)
	{
		isStaminaLoss_ = false;
		noDashTime_ = 0.0f;
	}
}
//
//void PlayerBehavior::ProcessSkill(Controller& controller, ObjectManager& objectManager)
//{
//	if (controller.Pressed(InputID::Skil) && canSkill_)
//	{
//		// 必殺スキル
//		auto id = objectManager.CreateFromFactory(FactoryID::PlayerSkill, ownerId_, transform_->GetPos());
//		objectManager.Begin(id);
//
//		auto skill = objectManager.GetComponent<PlayerSkillBehavior>(id);
//		skill->SetAttackStartTime(1.0f);
//		isSkill_ = true;
//	
//	}
//}
//
//void PlayerBehavior::RiseSkillValue(void)
//{
//	auto& value = gauge_.at(UiID::Skill);
//	auto& [min, max] = value.second;
//	// スキルゲージ上昇
//	value.first = Clamp(value.first + RiseValue * 30.0f, min, max);
//	// ゲージMAX
//	if (value.first >= max)
//	{
//		canSkill_ = true;
//	}
//}
//
//void PlayerBehavior::RiseSkillMax(void)
//{
//	auto& value = gauge_.at(UiID::Skill);
//	auto& [min, max] = value.second;
//	// ゲージMAX
//	value.first = max;
//	canSkill_ = true;
//}

void PlayerBehavior::Animation(Controller& controller, ObjectManager& objectManager)
{
	// コンボ攻撃
	auto index = AnimIndex::Idle;
	if (atkLimit_)
	{
		// 攻撃アニメーション
		index = (atkCnt_ % AtkAnimMax == 1) ? AnimIndex::Attack1SwordOn
			: (atkCnt_ % AtkAnimMax == 2) ? AnimIndex::Attack2SwordOn
			: (atkCnt_ % AtkAnimMax == 0) ? AnimIndex::Attack3SwordOn
			: index;
		// 入力受付時間内に入力が無ければコンボ終了
		auto animPlayTime = animation_->GetAnimState()->GetPlayTime();
		auto animTotalTime = animation_->GetAnimState()->GetTotalTime();
		if (animPlayTime >= animTotalTime && animTotalTime != 0.0f)
		{
			// 再生時間超過分の時間
			auto animPlayTimeOver = animation_->GetAnimState()->GetPlayTimeOver();
			if (animPlayTimeOver >= animTotalTime)
			{
				// コンボリセット
				ResetCombo();
			}
			isAttack_ = false;
		}
	}
	else
	{
		index = isSkill_ ? AnimIndex::Attack1SwordOn		// 必殺技
			: (controller.GetLeftInput().SqMagnitude() > 0.0f) ? AnimIndex::ForwardMove		// 移動
			: (controller.IsNeutral()) ? AnimIndex::Idle		// 立ち
			: index;// 何もしない
	}
	for (auto& tmp : anim_)
	{
		auto& [idx, time] = tmp;
		// indexに対応したアニメーションを入れる
		if (index == idx)
		{
			animation_->SetNextAnimation(static_cast<int>(idx), time);
		}
	}

}

void PlayerBehavior::Sound(void)
{
	//// 音制御
	//for (auto& s : sound_)
	//{
	//	if (s.second)
	//	{
	//		lpSooundPross.PlayBackSound(s.first, lpSooundPross.GetVolume(), false);
	//		s.second = false;
	//	}
	//}
}

void PlayerBehavior::OnHit(Collider& col, ObjectManager& objectManager)
{
	auto atr = objectManager.GetComponent<ObjectInfo>(col.GetOwnerID())->GetAttribute();
	if (atr == ObjectAttribute::EnemyAttack)
	{
		auto camera = objectManager.GetComponent<CameraBehavior>(objectManager.GetCameraID());
		if (camera.IsActive())
		{
			camera->StartShake(0.25f);
		}

		auto attackTransform = objectManager.GetComponent<Transform>(col.GetOwnerID());
	/*	auto effect = objectManager.CreateFromFactory(FactoryID::HitEffect, ownerId_, (transform_->GetPos() + attackTransform->GetPos()) / 2.0f);
		objectManager.GetComponent<Transform>(effect)->Scale() = Vector3{ 0.3f,0.3f ,0.3f };
		objectManager.Begin(effect);*/

		// エネルギーを減らす
		auto power = objectManager.GetComponent<EnemyBehavior>(objectManager.GetEnemyID())->GetEnemyPower();
		auto& value = gauge_.at(UiID::Torion);
		auto& [min, max] = value.second;
		// エネルギー消費
		value.first = Clamp(value.first - power, min, max);
		// エネルギー全損
		if (value.first <= 0.0f)
		{
			// プレイヤー消滅処理
			objectManager.GetComponent<ObjectInfo>(objectManager.GetPlayerID())->Destory();
		//	sound_.at(static_cast<int>(SOUNDNAME_SE::playerDestory)).second = true;
		}
		//else
		//{
		////	sound_.at(static_cast<int>(SOUNDNAME_SE::playerHit)).second = true;
		//}
	}
}

void PlayerBehavior::CorrectionGroundPos(void)
{
	if (float_.groundPosY > transform_->Pos().y)
	{
		if (collider_->IsGround())
		{
			float_.groundPosY = transform_->Pos().y;
		}
		else
		{
			transform_->Pos().y = float_.groundPosY;
		}
	}
}
