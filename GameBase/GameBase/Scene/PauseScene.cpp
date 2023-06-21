#include <DxLib.h>
#include "TitleScene.h"
#include "../SceneManager.h"
#include "../Common/ResourceManager.h"
#include "OptionScene.h"
#include "../UI/UiManager.h"
#include "DialogScene.h"
//#include "../Common/SoundPross.h"
#include "GameScene.h"
//#include "../Shader/PEManager.h"
#include "PauseScene.h"

// ÇøÇÂÇ§Ç«Ç¢Ç¢ä¥Ç∂Ç…í≤êÆÇ∑ÇÈÇ±Ç∆
constexpr Vector2I pauseWndSize{ 1280,720 };

PauseScene::PauseScene(SceneUptr owner) :
	WindowScene{ std::move(owner),0.25f,ScreenID::PauseUi, ScreenID::Pause,SceneID::Pause }
{

	choiceNextID_ = owner_->GetID();

	SetMakeSceneFunc(std::bind(&PauseScene::MakeDialogFunc, this, std::placeholders::_1), SceneID::Dialog);
	SetMakeSceneFunc(std::bind(&PauseScene::MakeOptionFunc, this, std::placeholders::_1), SceneID::Option);
	SetUseASyncLoadFlag(false);
	//uiManager_ = std::make_unique<UiManager>("Resource/Other/UiData/pause.ui", false);
	//uiManager_->Begin();
	lpSceneManager.GetResourceManager().LoadTexture(frame_, "Resource/resource/PauseFrame.png");
	SetUseASyncLoadFlag(true);
}

void PauseScene::Back(void)
{
	choiceNextID_ = owner_->GetID();
	Close();
}

void PauseScene::Dialog(void)
{
	choiceNextID_ = SceneID::Dialog;
	Close();
}

void PauseScene::Option(void)
{
	choiceNextID_ = SceneID::Option;
	Close();
}

BaseScene::SceneUptr PauseScene::MakeGameFunc(SceneUptr own)
{
	return std::make_unique<TitleScene>();
}

BaseScene::SceneUptr PauseScene::MakeDialogFunc(SceneUptr own)
{
	return std::make_unique<DialogScene>(std::move(own));
}

BaseScene::SceneUptr PauseScene::MakeOptionFunc(SceneUptr own)
{
	return std::make_unique<OptionScene>(std::move(own));
}




void PauseScene::UpdateOpend(float delta, Controller& controller)
{
	//uiManager_->Update(delta, *this, *objManager_, controller);
}

void PauseScene::DrawWindow(void)
{
	DrawRotaGraph(SceneManager::screenSize_<int>.x / 2, SceneManager::screenSize_<int>.y / 2, 1.0, 0.0f, *frame_, true);
	//uiManager_->Draw();
}

void PauseScene::Closed(void)
{
	WindowScene::Closed();
	Open();
	ChangeSceneID(choiceNextID_);

	// å„Ç≈PEMngÇ…çáÇÌÇπÇƒèëÇ´ä∑Ç¶ÇÈ
	if (choiceNextID_ == SceneID::Game)
	{
		auto& gameScene = static_cast<GameScene&>(*owner_);
		gameScene.SetUp();
	}
}


