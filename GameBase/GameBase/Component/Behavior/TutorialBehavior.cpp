#include "TutorialBehavior.h"
#include "../../Object/ObjectManager.h"
#include "../Transform/Transform.h"
#include "../Behavior/PlayerBehavior.h"
#include "../Info/ObjectInfo.h"
#include "../Behavior/EnemyBehavior.h"
#include "../Render/BillboardRender.h"
#include "../../Common/Debug.h"

#include "../../Common/ResourceManager.h"
#include "../ComponentPool.h"

#include "../../Scene/BaseScene.h"
// 矢印表示をするときの敵からの高さオフセット
constexpr float arrowOffsetY{ 400.0f };

TutorialBehavior::TutorialBehavior() :
	step_{ 0.0f }, nextUpdate_{ nullptr }, update_{ nullptr }, waitTime_{ 1.0f }
{
	nextType_ = nowType_ = TutorialType::Move;
	lpSceneManager.GetResourceManager().LoadTexture(handle_, "Resource/resource/TutorialArrow.png");
}

void TutorialBehavior::Update(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller)
{
	step_ += delta;
	(this->*update_)(scene, objectManager, delta, controller);
}

void TutorialBehavior::Begin(ObjectManager& objectManager)
{
	auto player = objectManager.GetComponent<Transform>(objectManager.GetPlayerID());

	if (player.IsActive())
	{
		playerStartPos_ = player->GetPos();
	}
	nextType_ = nowType_ = TutorialType::Move;
	nextUpdate_ = &TutorialBehavior::UpdateMove;
	update_ = &TutorialBehavior::UpdateMove;
	step_ = 0.0f;
}

void TutorialBehavior::UpdateWait(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller)
{
	if (step_ >= waitTime_)
	{
		// 一定時間経ったらnextUpdateを移す
		update_ = nextUpdate_;
		step_ = 0.0f;
		nowType_ = nextType_;
	}
}

void TutorialBehavior::UpdateMove(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller)
{
	auto player = objectManager.GetComponent<Transform>(objectManager.GetPlayerID());
	if (controller.GetLeftInput().SqMagnitude() >= 1.0f)
	{
		// 次へ
		nextUpdate_ = &TutorialBehavior::UpdateView;
		update_ = &TutorialBehavior::UpdateWait;
		step_ = 0.0f;
		waitTime_ = 2.0f;
		nextType_ = TutorialType::View;
	}
}

void TutorialBehavior::UpdateView(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller)
{
	if (step_ >= 2.0f)
	{
		// 次へ
		nextUpdate_ = &TutorialBehavior::UpdateJump;
		update_ = &TutorialBehavior::UpdateWait;
		step_ = 0.0f;
		waitTime_ = 2.0f;
		nextType_ = TutorialType::Jump;
	}
}

void TutorialBehavior::UpdateJump(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller)
{
	if (controller.Press(InputID::Jump))
	{
		// 次へ
		nextUpdate_ = &TutorialBehavior::UpdateAttack;
		update_ = &TutorialBehavior::UpdateWait;
		step_ = 0.0f;
		nextType_ = TutorialType::Attack;
	}
}

void TutorialBehavior::UpdateAttack(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller)
{
	if (controller.Pressed(InputID::MainTrigerAttack))
	{
		// 次へ
		nextUpdate_ = &TutorialBehavior::UpdateEnemySpawn;
		update_ = &TutorialBehavior::UpdateWait;
		step_ = 0.0f;
		nextType_ = TutorialType::Attack;

	}
}

void TutorialBehavior::UpdateEnemySpawn(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller)
{
	nextUpdate_ = &TutorialBehavior::UpdateEnemy;
	update_ = &TutorialBehavior::UpdateEnemy;
	nextType_ = TutorialType::Enemy;
	nowType_ = TutorialType::Enemy;
	step_ = 0.0f;

	// 敵を生成
	enemyID_ = objectManager.CreateFromFactory(FactoryID::Enemy, ownerId_, zeroVector3<float>);
	objectManager.GetComponent<EnemyBehavior>(enemyID_)->Tutorial();
	objectManager.Begin(enemyID_);

	// 矢印を生成
	arrow_ = objectManager.MakeObjectID();
	objectManager.AddComponent(objectManager.GetPool().Pop<BillboardRender>(), arrow_);
	objectManager.AddComponent(objectManager.GetPool().Pop<Transform>(), arrow_);
	objectManager.AddComponent(objectManager.GetPool().Pop<ObjectInfo>(), arrow_);
	objectManager.GetComponent<Transform>(arrow_)->Pos() = objectManager.GetComponent<Transform>(enemyID_)->GetPos() + (upVector3<float> *arrowOffsetY);

	objectManager.GetComponent<Render>(arrow_)->Load("Resource/resource/TutorialArrow.png");
	objectManager.Begin(arrow_);
}

void TutorialBehavior::UpdateEnemy(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller)
{
	if (!objectManager.IsActive(enemyID_))
	{
		// 敵が倒された時
		objectManager.GetComponent<ObjectInfo>(arrow_)->Destory();
		nextUpdate_ = &TutorialBehavior::UpdateSkillMess;
		update_ = &TutorialBehavior::UpdateSkillMess;
		nextType_ = TutorialType::SkillMess;
		nowType_ = TutorialType::SkillMess;
		step_ = 0.0f;
	}
	else
	{
		objectManager.GetComponent<Transform>(arrow_)->Pos() = objectManager.GetComponent<Transform>(enemyID_)->GetPos() + (upVector3<float> *arrowOffsetY);
	}
}

void TutorialBehavior::UpdateEnemySpawnGage(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller)
{
	// 敵をスポーンさせる
	auto id = objectManager.CreateFromFactory(FactoryID::Enemy, ownerId_, zeroVector3<float>);
	objectManager.GetComponent<EnemyBehavior>(id)->Tutorial();
	objectManager.Begin(id);

	id = objectManager.CreateFromFactory(FactoryID::Enemy, ownerId_, { 0.0f,0.0f,-100.0f });
	objectManager.GetComponent<EnemyBehavior>(id)->Tutorial();
	objectManager.Begin(id);

	id = objectManager.CreateFromFactory(FactoryID::Enemy, ownerId_, { 0.0f,0.0f,100.0f });
	objectManager.GetComponent<EnemyBehavior>(id)->Tutorial();
	objectManager.Begin(id);

	id = objectManager.CreateFromFactory(FactoryID::Enemy, ownerId_, { 0.0f,0.0f,200.0f });
	objectManager.GetComponent<EnemyBehavior>(id)->Tutorial();
	objectManager.Begin(id);

	id = objectManager.CreateFromFactory(FactoryID::Enemy, ownerId_, { 0.0f,0.0f,300.0f });
	objectManager.GetComponent<EnemyBehavior>(id)->Tutorial();
	objectManager.Begin(id);

	id = objectManager.CreateFromFactory(FactoryID::Enemy, ownerId_, { 0.0f,0.0f,-100.0f });
	objectManager.GetComponent<EnemyBehavior>(id)->Tutorial();
	objectManager.Begin(id);

	id = objectManager.CreateFromFactory(FactoryID::Enemy, ownerId_, { 0.0f,0.0f,-200.0f });
	objectManager.GetComponent<EnemyBehavior>(id)->Tutorial();
	objectManager.Begin(id);

	id = objectManager.CreateFromFactory(FactoryID::Enemy, ownerId_, { 0.0f,0.0f,-300.0f });
	objectManager.GetComponent<EnemyBehavior>(id)->Tutorial();
	objectManager.Begin(id);

	nextUpdate_ = &TutorialBehavior::UpdateGageSkill;
	update_ = &TutorialBehavior::UpdateGageSkill;
}

void TutorialBehavior::UpdateGageSkill(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller)
{
	// プレイヤーが
	auto player = objectManager.GetComponent<PlayerBehavior>(objectManager.GetPlayerID());
	if (player.IsActive())
	{
		// プレイヤーのスキルをマックスまでためる
		const auto& [val, range] = player->GetGaugeValue(UiID::Torion);
		if (val <= range.second)
		{
			// スキルがたまったらスキルの使用の説明へ以降
			nextUpdate_ = &TutorialBehavior::UpdateSkill;
			update_ = &TutorialBehavior::UpdateSkill;
			nextType_ = TutorialType::Shot;
			nowType_ = TutorialType::Shot;
		}
	}
}

void TutorialBehavior::UpdateSkillMess(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller)
{
	if (step_ >= 5.0f)
	{
		nextUpdate_ = &TutorialBehavior::UpdateEnemySpawnGage;
		update_ = &TutorialBehavior::UpdateEnemySpawnGage;
	
		step_ = 0.0f;
		auto player = objectManager.GetComponent<PlayerBehavior>(objectManager.GetPlayerID());
		//if (player.IsActive())
		//{
		//	// プレイヤーのスキルをマックスまでためる
		//	player->RiseSkillMax();
		//}
	}
}

void TutorialBehavior::UpdateSkill(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller)
{
	if (controller.Pressed(InputID::SubTrigerAttack))
	{
		// スキル使用時
		nextUpdate_ = &TutorialBehavior::UpdateEnd;
		update_ = &TutorialBehavior::UpdateWait;
		waitTime_ = 3.0f;
		nextType_ = TutorialType::End;
		step_ = 0.0f;
	}
}

void TutorialBehavior::UpdateEnd(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller)
{
	if (step_ >= 5.0f)
	{
		// 5秒経ったらセレクトへ戻る
		scene.ChangeSceneID(SceneID::Select);
	}
}
