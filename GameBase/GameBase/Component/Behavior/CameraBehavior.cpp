#include "../../Object/ObjectManager.h"
#include "../../Scene/BaseScene.h"
#include "PlayerBehavior.h"
#include "../../Common/Camera.h"
#include "CameraBehavior.h"

#include "../../Common/Debug.h"

constexpr float offsetHeight{ 150.0f };
constexpr float offsetLength{ 500.0f };

// カメラをシェイクする方向
constexpr struct Shake {
	Vector3 dir{ 1.0f,1.0f,0.0f };
	float width{ 4.0f };
	float speed{ 60.0f };
} shake;

// プレイヤー移動時のカメラオフセット
constexpr struct CameraOffset {
	float dodge = 200.0f;
	float dash = 50.0f;
	float walk = 0.0f;
} offset;


void CameraBehavior::StartShake(float shakeTime)
{
	shakeTime_ = shakeTime;
	isShake_ = true;
	defPos_ = transform_->GetPos();
}

void CameraBehavior::Update(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller)
{
	stepTime_ += delta;
	(this->*update_)(scene, objectManager, delta, controller);

	if (isShake_)
	{
		// シェイク時の処理
		Shake(delta);
	}

	auto player = objectManager.GetComponent<PlayerBehavior>(objectManager.GetPlayerID());
	if (player.IsActive())
	{
		offset_ = (player->IsDodge()) ? offset.dodge
			: (player->IsDash()) ? offset.dash
			: offset.walk;
		if (offset_ != cnt_)
		{
			CameraOffset(delta);
		}
	}
	// カメラに同期させる
	scene.GetCamera().SetPos(transform_->GetPos() + (-transform_->GetForward() * cnt_));
	scene.GetCamera().SetRotation(transform_->GetRotation());
}

void CameraBehavior::Begin(ObjectManager& objectManager)
{
	transform_ = objectManager.GetComponent<Transform>(ownerId_);
	transform_->Pos() += upVector3<float> *offsetHeight;
	update_ = &CameraBehavior::UpdateSync;
	speed_ = 0.5f;
	stepTime_ = 0.0f;
	offset_ = 0.0f;
	cnt_ = 0.0f;
	offsetMax_ = 0.0f;

	auto syncObj = objectManager.GetComponent<Transform>(syncID_);
	if (syncObj.IsActive())
	{
		transform_->Pos() = syncObj->GetPos() + (-transform_->GetForward() * offsetLength);
		transform_->Pos().y = syncObj->GetPos().y + offsetHeight;
		defPos_ = transform_->GetPos();
		DebugDrawLine(transform_->GetPos(), syncObj->GetPos(), 0xffffff);
	}
}

void CameraBehavior::UpdateSync(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller)
{
	constexpr auto half = SceneManager::screenSize_<float> / 2.0f;
	auto mousePos = controller.GetRightInput();
	auto move = mousePos.x - half.x;
	float rotY = 0.0f;
	if (half.x < move)
	{
		rotY = (move * speed_ * delta);
	}
	else if (half.x > move)
	{
		rotY = (move * speed_ * delta);
	}
	else
	{
		rotY = 0.0f;
	}

	transform_->AddRotation({ 0.0f,rotY,0.0f });
	controller.SetQursorPos();
	auto syncObj = objectManager.GetComponent<Transform>(syncID_);
	if (syncObj.IsActive())
	{
		transform_->Pos() = syncObj->GetPos() + (-transform_->GetForward() * offsetLength);
		transform_->Pos().y = syncObj->GetPos().y + offsetHeight;
		defPos_ = transform_->GetPos();
		DebugDrawLine(transform_->GetPos(), syncObj->GetPos(), 0xffffff);
	}
}

void CameraBehavior::UpdateNon(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller)
{

}

void CameraBehavior::Shake(float delta)
{
	if (stepTime_ >= shakeTime_)
	{
		isShake_ = false;
		transform_->Pos() = defPos_;
		stepTime_ = 0.0f;
		return;
	}
	transform_->Pos() += shake.dir * (shake.width * std::sin(stepTime_ * shake.speed));
}

void CameraBehavior::CameraOffset(float delta)
{
	if (offset_ > cnt_)
	{
		cnt_ += delta * 1000.0f;
		offsetMax_ = offset_;
	}
	else if (offset_ < cnt_)
	{
		cnt_ -= delta * 1000.0f;
		if (offset_ >= cnt_)
		{
			offsetMax_ = offset_;
		}
	}
	cnt_ = Clamp(cnt_, 0.0f, offsetMax_);
}
