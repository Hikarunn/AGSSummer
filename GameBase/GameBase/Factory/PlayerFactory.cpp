#include "../Object/ObjectManager.h"
#include "../Component/Info/ObjectInfo.h"
#include "../Component/Behavior/PlayerBehavior.h"
#include "../Common/Input/Controller.h"
#include "../Component/Render/ModelRender.h"
#include "../Component/Collider/CharactorCollider.h"
#include "../Factory/PlayerAttackFactory.h"
#include "PlayerShotFactory.h"

#include "PlayerFactory.h"

#include "../Common/Debug.h"

PlayerFactory::PlayerFactory(const std::string& filePath, ObjectManager& objectManager) :
	Factory{ objectManager }
{
	Factory::Load(filePath);
	objectManager_.AddFactory(std::make_unique<PlayerAttackFactory>(objectManager_));
	objectManager_.AddFactory(std::make_unique<PlayerShotFactory>(objectManager_));

}

ObjectID PlayerFactory::Create(ObjectID ownerID, const Vector3& pos, const Vector3& rot)
{
	auto id = objectManager_.MakeObjectID();

	objectManager_.AddComponent(std::make_unique<ObjectInfo>(), id);

	// capsuleを描画するコンポーネントを追加
	objectManager_.AddComponent(std::make_unique<ModelRender>(), id);

	// 座標とかに関する

	objectManager_.AddComponent(std::make_unique<Transform>(), id);

	objectManager_.AddComponent(std::make_unique<Animator>(), id);


	objectManager_.AddComponent(std::make_unique<PlayerBehavior>(), id);

	objectManager_.AddComponent(std::make_unique<CharactorCollider>(), id);

	auto transform = objectManager_.GetComponent<Transform>(id);
	transform->Pos() = pos;
	transform->Pos().y += 15;
	transform.Get()->Scale() = Vector3{ 15,15,15 };
	//transform->SetLocalRotFromEulerRot({ 0.0f, Deg2Rad(180.0f), 0.0f });
	//transform->scale_ = scale_;
	//transform-> = rot_;
	//auto player = objectManager_.GetComponent<PlayerBehavior>(id);

	objectManager_.GetComponent<Render>(id)->Load(modelPath_);
	auto col = objectManager_.GetComponent<CharactorCollider>(id);
	col->SetHeight(70.0f);
	col->SetRadius(50.0f);
	col->SetOffset({ 0.0f, 40.0f, 0.0f });

	objectManager_.SetPlayerID(id);
	return id;
}

void PlayerFactory::LoadTrans(std::ifstream& file, std::uint32_t size)
{
	const auto& [pos, rot, scale] = Transform::Load(file);
	scale_ = scale;
	rot_ = rot;
}

void PlayerFactory::LoadModel(std::ifstream& file, std::uint32_t size)
{
	modelPath_ = ModelRender::Load(file);
}


