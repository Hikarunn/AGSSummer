#include "../Object/ObjectManager.h"
#include "../Component/ComponentPool.h"
#include "../Component/Behavior/PlayerShotBehavior.h"
#include "../Component/Info/ObjectInfo.h"
#include "../Component/Collider/CapsuleCollider.h"
#include "PlayerShotFactory.h"

PlayerShotFactory::PlayerShotFactory(ObjectManager& objectManager) :
	Factory{ objectManager }
{
}

ObjectID PlayerShotFactory::Create(ObjectID ownerID, const Vector3& pos, const Vector3& rot)
{
	auto id = objectManager_.MakeObjectID();

	// ����
	auto behavior = objectManager_.GetPool().Pop<PlayerShotBehavior>();
	behavior->SetAttackTime(3.0f);
	objectManager_.AddComponent(std::move(behavior), id);

	// �I�u�W�F�N�g�̏��
	auto info = objectManager_.GetPool().Pop<ObjectInfo>();
	info->SetUseFactory(true);
	info->SetAttribute(ObjectAttribute::PlayerShot);
	objectManager_.AddComponent(std::move(info), id);

	// �g�����X�t�H�[��
	auto transform = objectManager_.GetPool().Pop<Transform>();
	transform->Pos() = zeroVector3<float>;
	transform->SetLocalRotation({ Deg2Rad(90.0f), 0.0f, 0.0f });
	transform->SetRotation({ 0.0f,0.0f,-rot.y });
	objectManager_.AddComponent(std::move(transform), id);

	// �����蔻��
	auto col = objectManager_.GetPool().Pop<CapsuleCollider>();
	col->SetOffset({ 0.0,-2100.0f,-50.0f });
	col->SetRadius(200.0f);
	col->SetHeight(2000.0f);

	objectManager_.AddComponent(std::move(col), id);
	return id;
}

void PlayerShotFactory::Destroy(ObjectID id)
{
	auto& pool = objectManager_.GetPool();
	pool.Push(objectManager_.RemoveComponent<PlayerShotBehavior>(id));
	pool.Push(objectManager_.RemoveComponent<ObjectInfo>(id));
	pool.Push(objectManager_.RemoveComponent<Transform>(id));
	pool.Push(objectManager_.RemoveComponent<CapsuleCollider>(id));
}
