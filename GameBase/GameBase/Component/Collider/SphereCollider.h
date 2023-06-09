#pragma once
#include "Collider.h"
class SphereCollider :
	public Collider
{
public:
	SphereCollider();

	/// <summary>
	/// ¼aÌæ¾
	/// </summary>
	/// <param name=""></param>
	/// <returns> ¼a </returns>
	const float GetRadius(void) const
	{
		return radius_;
	}

	/// <summary>
	/// ¼aÌZbg
	/// </summary>
	/// <param name="radius"> Zbg·é¼a </param>
	void SetRadius(const float radius)
	{
		radius_ = radius;
	}

	/// <summary>
	/// ÀWðæ¾(offsetl¶ÏÝÌ)
	/// </summary>
	/// <param name=""></param>
	/// <returns> ÀW </returns>
	const Vector3 GetPos(void) const;

	static constexpr ComponentID id_{ ComponentID::SphereCollider };
	ComponentID GetID(void) const override
	{
		return id_;
	}
#ifdef _DEBUG
	void DrawDebug(void) final;
#endif
private:

	void Check(Collider& col, ObjectManager& objectManager) final;
	bool Check(CapsuleCollider& col, ObjectManager& objectManager) final;
	bool Check(SphereCollider& col, ObjectManager& objectManager) final;
	bool Check(MeshCollider& col, ObjectManager& objectManager) final;
	bool Check(CharactorCollider& col, ObjectManager& objectManager) final;
	bool Check(TerrainCollider& col, ObjectManager& objectManager) final;


	// ¼a
	float radius_;
};

