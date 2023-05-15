#pragma once
#include "../BaseScene.h"


class Transition :
	public BaseScene
{
public:
	Transition(SceneUptr before, SceneUptr after);
	virtual ~Transition();
protected:
	virtual bool IsTransAfter(float delta) = 0;
	SceneUptr before_;
	SceneUptr after_;
private:
	const SceneID GetID(void) const final
	{
		return SceneID::Transition;
	}
	void Relese(void) override;
	void Update(float delta, Controller& controller) final;
	SceneUptr MakeNextFunc(SceneUptr own);
};

