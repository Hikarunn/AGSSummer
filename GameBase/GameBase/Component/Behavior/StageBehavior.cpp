#include "../../Object/ObjectManager.h"
#include "../../Component/Transform/Transform.h"
//#include "../../Common/SoundPross.h"
#include "../../Scene/GameScene.h"
#include "StageBehavior.h"

StageBehavior::StageBehavior(std::map<size_t, Vector2>&& posList) :
	enemyPosList_{ std::move(posList) }
{
}

void StageBehavior::SubEnemy(ObjectID& id)
{
	enemyPosList_.erase(*id);
}

const std::map<size_t, Vector2>& StageBehavior::GetEnemyPosList(void) const&
{
	return enemyPosList_;
}

void StageBehavior::Update(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller)
{
	for (auto& pair : enemyPosList_)
	{
		auto trans = objectManager.GetComponent<Transform>(ObjectID{ pair.first });
		if (trans.IsActive())
		{
			pair.second.x = trans->GetPos().x;
			pair.second.y = trans->GetPos().z;
		}
	}

	if (enemyPosList_.size() <= 0)
	{
		// 敵をすべて倒したのでシーンを変える
		scene.ChangeSceneID(SceneID::Result);
		static_cast<GameScene&>(scene).SetResult(Result::Clear);
	}

	auto player = (objectManager.GetComponent<Transform>(objectManager.GetPlayerID()));
	if (!player.IsActive())
	{
		// ゲームオーバー
		static_cast<GameScene&>(scene).SetResult(Result::GameOver);
		scene.ChangeSceneID(SceneID::Result);
		//lpSooundPross.Release();
	}
}