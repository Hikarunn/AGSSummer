#include "../Transform/Transform.h"
#include "../../SceneManager.h"
#include "../../Object/ObjectManager.h"
#include "../Info/ObjectInfo.h"
#include "../Behavior/StageBehavior.h"
#include "PlayerBehavior.h"
//#include "EnemyBulletBehavior.h"
#include "PlayerAttackBehavior.h"
//#include "../../Common/SoundPross.h"
#include "../ComponentPool.h"
#include "EnemyBehavior.h"

#include "../../Common/Debug.h"


// 回転が必要な範囲(角度)
constexpr float rotDeff{ Square(Deg2Rad(15.0f)) };

// ノックバック時の初速
constexpr float v0{ 9.0f };

// ノックバック用の重力加速度
constexpr float gravity{ 9.80f * 10.0f };

// コンストラクタ
EnemyBehavior::EnemyBehavior()
{
	// デフォルトのスピードとして120を入れとく
	speed_ = 120.0f;
	update_ = nullptr;
	stopDistance_ = 0.0f;
	searchDistance_ = 0.0f;
	// デフォルトの振り向き時間として1を入れとく
	turnTime_ = 1.0f;
	Init();
}

// 更新処理
void EnemyBehavior::Update(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller)
{
	stateTime_ += delta;

	// 重力処理
	Gravity(delta);

	(this->*update_)(objectManager, delta);

	//DebugLog("jumpTime_=", jumpTime_);
}

// カウンタ変数や時間用変数の初期化
void EnemyBehavior::Init(void)
{
	shotTime_ = 0.0f;
	burstCnt_ = 0;
	damageCnt_ = 0;
	isRot_ = false;
	stateTime_ = 0.0f;
	hitTime_ = 0.0f;
	rotSing_ = 0.0f;
	hitCombo_ = 0;
	jumpTime_ = 0.0f;
}

static int num = 1;

// 有効時の開始処理
void EnemyBehavior::Begin(ObjectManager& objectManager)
{
	// 変数を初期化
	Init();

	// トランスフォームを取得
	transform_ = objectManager.GetComponent<Transform>(ownerId_);

	// プレイヤーのトランスフォームを取得
	playerT_ = objectManager.GetComponent<Transform>(objectManager.GetPlayerID());

	// コライダーを取得しヒット時の関数をセット
	collider_ = objectManager.GetComponent<CharactorCollider>(ownerId_);
	collider_->SetHitFunc(std::bind(&EnemyBehavior::OnHit, this, std::placeholders::_1, std::placeholders::_2));
	collider_->SetGroundFlag(true);

	if (!objectManager.HaveComponent<Collider>(ownerId_))
	{
		assert(false);
	}

	// 更新用関数をセット
	update_ = &EnemyBehavior::UpdateMove;

	if (isTutorial_)
	{
		shot_ = &EnemyBehavior::NonShot;
	}
	else
	{
		shot_ = &EnemyBehavior::Shot;

	}

	// レーザーサイト取得
	//laserSight_ = objectManager.GetComponent<EnemyLaserSightBehavior>(laserSightID_);

	// アニメーター取得
	animetor_ = objectManager.GetComponent<Animator>(ownerId_);
	animetor_->AddAnimation(0, true);
	animetor_->AddAnimation(1, true);
	animetor_->AddAnimation(5, true);
	animetor_->SetNextAnimation(0);


	DebugLog("EnemyBehavior::Begin:id=", *ownerId_, ":num=", num++);
}

// 終了処理
void EnemyBehavior::End(ObjectManager& objectManager)
{
	// コンポーネントが向こうになった時に呼ばれる
	auto stage = objectManager.GetComponent<StageBehavior>(stageID_);
	if (stage.IsActive())
	{
		// ステージが生きている時ステージの敵のカウントを減らさせる
		stage->SubEnemy(ownerId_);
	}
}

// 追跡前の状態の更新
void EnemyBehavior::UpdateSearch(ObjectManager& objectManager, float delta)
{
	if ((transform_->GetPos() - playerT_->GetPos()).SqMagnitude() <= Square(searchDistance_))
	{
		// 一定の距離になったら移動開始する
		stateTime_ = 0.0f;
		update_ = &EnemyBehavior::UpdateMove;
		animetor_->SetNextAnimation(1, 0.5f);
	}
}

// 射撃処理
void EnemyBehavior::Shot(ObjectManager& objectManager, float delta)
{
	shotTime_ += delta;
	if (shotTime_ >= 0.0f)
	{
		if (shotTime_ >= shotData_.burstInterval)
		{
			shotTime_ = 0.0f;
			burstCnt_++;
			auto shotPos = transform_->GetPos() + (transform_->GetRotation() * Vector3 { 10.0f, 190.0f, 120.0f });

			// 右上方向にずらして作成
			/*auto id = objectManager.CreateFromFactory(FactoryID::EnemyBullet, ownerId_, shotPos);
			objectManager.Begin(id);*/
		

			DebugLog("発射");

			if (burstCnt_ >= shotData_.burstNum)
			{
				burstCnt_ = 0;
				shotTime_ = -shotData_.shotInterval;
			}
		}
	}
}

void EnemyBehavior::NonShot(ObjectManager& objectManager, float delta)
{
}

// 追跡移動状態の処理
void EnemyBehavior::UpdateMove(ObjectManager& objectManager, float delta)
{
	// 回転処理
	Rotation(delta);

	// 移動
	//DebugLog("x=", animetor_->GetAnimState()->GetMovePosition().x,"y=", animetor_->GetAnimState()->GetMovePosition().y,"z=", animetor_->GetAnimState()->GetMovePosition().z);
	transform_->Pos() += transform_->GetForward() * (delta * speed_);

	if (!animetor_->GetAnimState()->IsBlend())
	{
		if ((transform_->GetPos() - playerT_->GetPos()).SqMagnitude() <= Square(stopDistance_))
		{
			// 一定の距離になったら停止状態へ
			stateTime_ = 0.0f;
			update_ = &EnemyBehavior::UpdateStop;
			//laserSight_->On();
			animetor_->SetNextAnimation(5, 0.5f);
		}
		else if ((transform_->GetPos() - playerT_->GetPos()).SqMagnitude() >= Square(searchDistance_))
		{
			// 一定の距離以上になったら追跡前の状態へ
			stateTime_ = 0.0f;
			update_ = &EnemyBehavior::UpdateSearch;
			animetor_->SetNextAnimation(0, 0.5f);
		}
	}
}

// 停止状態の処理
void EnemyBehavior::UpdateStop(ObjectManager& objectManager, float delta)
{
	// 回転
	Rotation(delta);

	if (!animetor_->GetAnimState()->IsBlend())
	{
		// 射撃
		(this->*shot_)(objectManager, delta);
		//muzFlash_->Play();
		if ((transform_->GetPos() - playerT_->GetPos()).SqMagnitude() >= Square(stopDistance_))
		{
			// 一定以上の距離になったら移動状態へ
			stateTime_ = 0.0f;
			update_ = &EnemyBehavior::UpdateMove;
			animetor_->SetNextAnimation(1, 0.5f);
			//laserSight_->Off();
		}
	}
}

// 攻撃を受けた後の状態
void EnemyBehavior::UpdateHit(ObjectManager& objectManager, float delta)
{
	// 後ろに下がる
	transform_->Pos() += knockBackVec_ * (speed_)*delta;
	if (collider_->IsGround() && stateTime_ >= hitTime_)
	{
		// 硬直時間分経ったら
		stateTime_ = 0.0f;
		auto player = (objectManager.GetComponent<PlayerBehavior>(objectManager.GetPlayerID()));
		player->AttackEnd();
		//hitCombo_ = -1;
		update_ = &EnemyBehavior::UpdateMove;
		DebugLog("硬直終了");
		// 3回攻撃を受けたら消滅
		if (damageCnt_ >= HpMax)
		{
			DebugLog("倒された");
			objectManager.GetComponent<ObjectInfo>(ownerId_)->Destory();
			
			//lpSooundPross.PlayBackSound(SOUNDNAME_SE::enemyDestroy, lpSooundPross.GetVolume(), false);
		}
	}
}

// 回転処理
void EnemyBehavior::Rotation(float delta)
{
	if (isRot_)
	{
		// 回転中だった時の処理
		transform_->AddRotation({ 0.0f, Deg2Rad(60.0f) * rotSing_ * delta, 0.0f });
	}


	auto targetVec{ playerT_->GetPos() - transform_->GetPos() };
	auto forward{ transform_->GetForward() };
	auto rot = Square(std::atan2(targetVec.z, targetVec.x) - std::atan2(forward.z, forward.x));
	if (rot > rotDeff)
	{
		// 回転すべき時
		isRot_ = true;
		rotSing_ = Cross(Vector2{ forward.z, forward.x }, Vector2{ targetVec.z,targetVec.x }) > 0.0f ? 1.0f : -1.0f;
	}
	else
	{
		isRot_ = false;
	}
}

// 重力処理
void EnemyBehavior::Gravity(float delta)
{
	if (!collider_->IsGround())
	{
		// 空中にいる時
		jumpTime_ += delta;
		float val = (v0 * jumpTime_) - (0.5f * gravity * Square(jumpTime_));
		transform_->Pos() += transform_->GetUp() * (val);
		collider_->SetGravityPow(val);
	}
	else
	{
		jumpTime_ = 0.0f;
	}
}


// ヒット時の処理
void EnemyBehavior::OnHit(Collider& col, ObjectManager& objectManager)
{
	auto atr = objectManager.GetComponent<ObjectInfo>(col.GetOwnerID())->GetAttribute();
	if (atr == ObjectAttribute::PlayerAttack)
	{
		auto player = (objectManager.GetComponent<PlayerBehavior>(objectManager.GetPlayerID()));
		if (hitCombo_ == player->GetAtkCnt())
		{
			// 同じ攻撃なので処理しない
			return;
		}
		if (damageCnt_ >= HpMax)
		{
			// もうすでに死亡しているので処理しない
			return;
		}

		// 攻撃を受けたときの処理
		hitCombo_ = player->GetAtkCnt();
		damageCnt_++;
		//player->RiseSkillValue();
		player->AddCombo();

		auto hit = objectManager.GetComponent<Transform>(col.GetOwnerID());

		// ノックバックさせる
		KnockBack(objectManager);
	}
	else if (atr == ObjectAttribute::PlayerShot)
	{
		if (damageCnt_ >= HpMax)
		{
			// もうすでに死亡しているので処理しない
			return;
		}
		// 一撃撃破
		damageCnt_ += HpMax;

		KnockBack(objectManager);
	}
}

void EnemyBehavior::KnockBack(ObjectManager& objectManager)
{
	update_ = &EnemyBehavior::UpdateHit;
	stateTime_ = 0.0f;

	jumpTime_ = 0.0f;
	collider_->SetGroundFlag(false);
	transform_->Pos().y += 2.0f;
	objectManager.StartHitStop(0.1f);
	auto playerTransform = (objectManager.GetComponent<Transform>(objectManager.GetPlayerID()));
	if (playerTransform.IsActive())
	{
		knockBackVec_ = (transform_->GetPos() - playerTransform->GetPos());
		knockBackVec_.y = 0.0f;
		knockBackVec_.Normalize();
	}
	animetor_->SetNextAnimation(0, 0.25f);
	//lpSooundPross.PlayBackSound(SOUNDNAME_SE::playerAttackHit, lpSooundPross.GetVolume(), false);
}
