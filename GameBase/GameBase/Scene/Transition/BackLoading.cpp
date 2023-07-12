#include "../../UI/UiManager.h"
#include "BackLoading.h"

#include "../../Common/Debug.h"

BackLoading::BackLoading(SceneUptr befor, SceneUptr after, float transTime) :
	Loading{ std::move(befor),std::move(after), transTime }
{
}

void BackLoading::Update(float delta, Controller& controller)
{
	uiManager_->Update(delta, *this, *objManager_, controller);
	if (after_->IsLoaded())
	{
		after_->Loaded(controller);
		after_->DrawScene();
		ChangeSceneID(after_->GetID());
		DebugLog("ƒ[ƒh’†");
	}
}

void BackLoading::DrawScene(void)
{
	SetDrawScreen(*screenHandle_);
	ClsDrawScreen();
	befor_->Draw();
	uiManager_->Draw(*screenHandle_);

}
