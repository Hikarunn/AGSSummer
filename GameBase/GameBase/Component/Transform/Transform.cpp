#include "../../Scene/BaseScene.h"
#include "../../Common/Math.h"
#include "Transform.h"
#include "../../Object/ObjectManager.h"
#include "../../Common/Math.h"

#include "../../Common/Debug.h"

Transform::Transform()
{
}


void Transform::Update(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller)
{
	DebugDrawLine(pos_, pos_ + (GetForward() * 50.0f), 0x0000ff);
	DebugDrawLine(pos_, pos_ + (GetUp() * 50.0f), 0x00ff00);
	DebugDrawLine(pos_, pos_ + (GetRight() * 50.0f), 0xff0000);

}

void Transform::End(ObjectManager& objectManager)
{
	rot_ = {};
	localRot_ = {};
	offset_ = zeroVector3<float>;
	scale_ = Vector3{ 1.0f,1.0f,1.0f };

}

const Vector3 Transform::GetForward(void) const
{
	auto v{ GetRotation() * forwardVector3<float> };
	return { v.x,v.y,v.z };
}

const Vector3 Transform::GetUp(void) const
{
	auto v{ GetRotation() * upVector3<float> };
	return { v.x,v.y,v.z };
}

const Vector3 Transform::GetRight(void) const
{
	auto v{ GetRotation() * rightVector3<float> };
	return { v.x,v.y,v.z };
}

void Transform::LookAt(const Vector3& vec)
{
	/*auto fVec = VTransform(VGet(forwardVector3<float>.x, forwardVector3<float>.y, forwardVector3<float>.z), GetRotationMatrix());
	rot_ = MMult(rot_,MGetRotVec2(VGet(fVec.x, fVec.y, fVec.z), VGet(vec.x, vec.y, vec.z)));*/
	auto up = GetForward();
	up.Normalize();
	auto target = vec.Normalized();
	auto dot = Dot(up, target);
	auto rad = std::acos(GetNormalizedAngle(dot, -1.0f, 1.0f));
	if (!std::isfinite(dot))
	{
		return;
	}
	auto cross = Cross(up, target);
	cross.Normalize();
	if (!std::isfinite(cross.x))
	{
		return;
	}
	rot_ = { GetNormalizedAngle(rad),cross };
}

const Quaternion Transform::GetRotation(void) const
{
	return (localRot_ * rot_);
}


Transform& Transform::operator=(const Transform& transform)
{
	return *this;
}

void Transform::Read(std::ifstream& file)
{
	const auto& [pos, rot, scale] = Load(file);
	pos_ = pos;
	scale_ = scale;
	rot_ = { rot.x, rot.y, rot.z };
}

std::tuple<Vector3, Vector3, Vector3> Transform::Load(std::ifstream& file)
{
	Vector3 pos, rotation, scale;
	file.read(reinterpret_cast<char*>(&pos), sizeof(pos));
	file.read(reinterpret_cast<char*>(&rotation), sizeof(rotation));
	file.read(reinterpret_cast<char*>(&scale), sizeof(scale));
	return std::tie(pos, rotation, scale);
}

