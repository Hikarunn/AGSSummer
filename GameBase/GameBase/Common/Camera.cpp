#include "Math.h"
#include "Camera.h"

#include "Debug.h"
constexpr float TARGET_TO_CAMERA = 500.0f;

// �J�����̃Z�b�g�A�b�v

void Camera::SetScreen(void) const
{
	auto up = quaRot_ * upVector3<float>;
	auto forward = quaRot_ * forwardVector3<float>;

	auto target = VAdd(VGet(pos_.x, pos_.y, pos_.z), VScale(VGet(forward.x, forward.y, forward.z), TARGET_TO_CAMERA));
	SetCameraPositionAndTargetAndUpVec(VGet(pos_.x, pos_.y, pos_.z), target, VGet(up.x, up.y, up.z));

}

const Vector3 Camera::GetForward(void) const
{
	auto vec = quaRot_ * forwardVector3<float>;
	return { vec.x, vec.y, vec.z };
}


void Camera::SetUpShadow(float size, float ner, float fur, const Vector3& target) const
{
	auto lightDir = GetLightDirection();
	auto targetPos = VGet(target.x, target.y, target.z);
	auto lightPos = VAdd(targetPos, VScale(lightDir, -5000.0f));

	// �J�����̕`��͈͂ƌ�����ʒu��ݒ�
	SetupCamera_Ortho(size);
	SetCameraNearFar(ner, fur);

	// �J�����̌��������C�g�̌����ɍ��킹��
	SetCameraPositionAndTarget_UpVecY(lightPos, targetPos);
}

void Camera::SetRotation(const Quaternion& q)
{
	quaRot_ = q;
}

