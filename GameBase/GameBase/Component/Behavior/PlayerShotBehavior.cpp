#include "../../Object/ObjectManager.h"
#include "../Info/ObjectInfo.h"
#include "../../Factory/PlayerShotFactory.h"
#include "PlayerBehavior.h"
#include "EnemyBehavior.h"
#include "../../UI/UiID.h"
#include "PlayerShotBehavior.h"

#include "../../Common/Debug.h"


PlayerShotBehavior::PlayerShotBehavior() :
	attackTime_{ 0.0f }, attackStartTime_{ 0.0f }, animTime_{ 0.0f }, update_{ nullptr }
{
}

void PlayerShotBehavior::Update(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller)
{
	auto player = objectManager.GetComponent<Transform>(objectManager.GetPlayerID());

	if (player.IsActive())
	{
		// プレイヤーと座標を同期させる
		transform_->Pos() = player->GetPos();
		(this->*update_)(objectManager, delta);

		// プレイヤーと回転を同期させる
		auto plRot = player->GetRotation().ToEuler();
		transform_->SetRotation({ 0.0f,0.0f,-plRot.y });
	}
}

void PlayerShotBehavior::Begin(ObjectManager& objectManager)
{
	auto player = (objectManager.GetComponent<Transform>(objectManager.GetPlayerID()));
	transform_ = objectManager.GetComponent<Transform>(ownerId_);
	transform_->Pos() = player->GetPos();

	collider_ = objectManager.GetComponent<Collider>(ownerId_);
	collider_->SetHitFunc(std::bind(&PlayerShotBehavior::OnHit, this, std::placeholders::_1, std::placeholders::_2));
	collider_->SetActiveFlag(false);
	attackTime_ = 2.6f;

	update_ = &PlayerShotBehavior::UpdateNonAttack;
}

void PlayerShotBehavior::End(ObjectManager& objectManager)
{
	attackStartTime_ = 0.0f;
	auto effect = objectManager.GetComponent<ObjectInfo>(effectID_);
	effect->Destory();
}

void PlayerShotBehavior::OnHit(Collider& col, ObjectManager& objectManager)
{
}

void PlayerShotBehavior::Destory(ObjectManager& objManager)
{
	objManager.GetFactory(FactoryID::PlayerShot).Destroy(ownerId_);
}

void PlayerShotBehavior::UpdateAttack(ObjectManager& objectManager, float delta)
{
	attackTime_ -= delta;
	// エフェクト更新
	if (attackTime_ <= 0.0f)
	{
		objectManager.GetComponent<PlayerBehavior>(objectManager.GetPlayerID())->EndSkill();
		objectManager.GetComponent<ObjectInfo>(ownerId_)->Destory();
	}
}

void PlayerSkillBehavior::UpdateNonAttack(ObjectManager& objectManager, float delta)
{
	attackStartTime_ -= delta;
	if (attackStartTime_ <= 0.0f)
	{
		update_ = &PlayerSkillBehavior::UpdateAttack;
		collider_->SetActiveFlag(true);
		// エフェクト発動
		auto beem = objectManager.GetComponent<BeemEffectBehavior>(effectID_);
		if (beem.IsActive())
		{
			beem->Start();
		}
	}
}
