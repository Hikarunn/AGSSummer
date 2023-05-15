#include "../../SceneManager.h"
#include "../../UI/UiManager.h"
#include "Loading.h"

#include "../../Common/Debug.h"

Loading::Loading(SceneUptr befor, SceneUptr after, float transTime) :
	BaseScene{ ScreenID::Loading, SceneID::Loading },
	after_{ std::move(after) }, befor_{ std::move(befor) },
	transTime_{ transTime }, stepTime_{ 0.0f }
{
	SetMakeSceneFunc(std::bind(&Loading::MakeNextFunc, this, std::placeholders::_1), after_->GetID());
	uiManager_ = std::make_unique<UiManager>("Resource/Other/UiData/load.ui", false, false, false);
	uiManager_->Begin();
}

Loading::~Loading()
{
}


void Loading::LoadingIndicator(float delta)
{
}

void Loading::Relese(void)
{
}


BaseScene::SceneUptr Loading::MakeNextFunc(SceneUptr own)
{
	return  std::move(after_);
}
