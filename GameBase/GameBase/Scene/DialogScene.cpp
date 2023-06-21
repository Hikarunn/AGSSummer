#include <DxLib.h>
#include "../SceneManager.h"
#include "../Common/ResourceManager.h"
//#include "../UI/UiManager.h"
#include "DialogScene.h"


DialogScene::DialogScene(SceneUptr owner) :
	WindowScene{ std::move(owner),0.25f, ScreenID::DialogUi, ScreenID::Dialog, SceneID::Dialog }
{

	SetUseASyncLoadFlag(false);
	//uiManager_ = std::make_unique<UiManager>("Resource/Other/UiData/dialog.ui", false);
//	uiManager_->Begin();
	lpSceneManager.GetResourceManager().LoadTexture(frame_, L"Resource/resource/DialogFrame.png");
	SetUseASyncLoadFlag(true);
}

void DialogScene::Ok(void)
{
	lpSceneManager.End();
}

void DialogScene::Cancel(void)
{
	Close();
}

void DialogScene::UpdateOpend(float delta, Controller& controller)
{
	//uiManager_->Update(delta, *this, *objManager_, controller);
}

void DialogScene::DrawWindow(void)
{
	DrawRotaGraph(SceneManager::screenSize_<int>.x / 2, SceneManager::screenSize_<int>.y / 2, 1.0, 0.0f, *frame_, true);
//	uiManager_->Draw();
}

void DialogScene::Closed(void)
{
	WindowScene::Closed();
	auto* ownerPtr = dynamic_cast<WindowScene*>(&*owner_);
	if (ownerPtr != nullptr)
	{
		ownerPtr->Open();
	}

	owner_->ChangeSceneID(owner_->GetID());
	ChangeSceneID(owner_->GetID());
}


