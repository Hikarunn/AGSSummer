#include "Transition.h"

Transition::Transition(SceneUptr before, SceneUptr after) :
	BaseScene{ ScreenID::Transition, SceneID::Transition }
{
	after_ = std::move(after);
	before_ = std::move(before);
	SetMakeSceneFunc(std::bind(&Transition::MakeNextFunc, this, std::placeholders::_1), after_->GetID());
}


Transition::~Transition()
{
}


void Transition::Relese(void)
{
}

void Transition::Update(float delta, Controller& controller)
{
	if (IsTransAfter(delta))
	{
		before_->Relese();
		ChangeSceneID(after_->GetID());
	}
}

BaseScene::SceneUptr Transition::MakeNextFunc(SceneUptr own)
{
	return std::move(after_);
}
