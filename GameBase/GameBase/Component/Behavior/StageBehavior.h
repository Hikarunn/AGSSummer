#pragma once
#include "Behavior.h"
#include <map>
#include "../../Common/Vector2.h"

class StageBehavior :
	public Behavior
{
public:
	StageBehavior(std::map<size_t, Vector2>&& posList);

	/// <summary>
	/// “G‚Ì”‚ÌƒJƒEƒ“ƒg‚ğŒ¸‚ç‚·
	/// </summary>
	/// <param name=""></param>
	void SubEnemy(ObjectID& id);


	const std::map<size_t, Vector2>& GetEnemyPosList(void) const&;
private:
	void Update(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller) final;

	std::map<size_t, Vector2> enemyPosList_;
};
