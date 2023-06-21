#include <DxLib.h>
#include <type_traits>
#include "Transition/FadeLoading.h"
#include "GameScene.h"
#include "SelectScene.h"
#include "../SceneManager.h"
#include "../Common/Camera.h"
#include "../Common/ResourceManager.h"
//#include "../Common/SoundPross.h"
#include "../UI/UiManager.h"
//#include "../Shader/PEManager.h"
#include "DialogScene.h"
#include "ResultScene.h"

#include "../Common/Debug.h"



ResultScene::ResultScene(StageID stageID, Result atr, SharedRenderTargetHandle& capture) :
	BaseScene{ ScreenID::Result,SceneID::Result }, capture_{ capture }, stageID_{ stageID }
{
	SetMakeSceneFunc(std::bind(&ResultScene::MakeSelectFunc, this, std::placeholders::_1), SceneID::Select);
	SetMakeSceneFunc(std::bind(&ResultScene::MakeRetryFunc, this, std::placeholders::_1), SceneID::Game);
	SetMakeSceneFunc(std::bind(&ResultScene::MakeDialogFunc, this, std::placeholders::_1), SceneID::Dialog);
	/*peMng_ = std::make_unique<PEManager>();
	lpSooundPross.Init(SceneID::Result);
	lpSooundPross.PlayBackSound(SOUNDNAME_BGM::ResultSceneBGM, lpSooundPross.GetVolume(), true);*/

	if (atr == Result::Clear)
	{
		if (static_cast<StageID>(std::underlying_type <StageID>::type(stageID_) + 1) == StageID::Max)
		{
			//uiManager_ = std::make_unique<UiManager>("Resource/Other/UiData/Clear.ui");
		}
		else
		{
		//	uiManager_ = std::make_unique<UiManager>("Resource/Other/UiData/ClearNext.ui");
		}
		//peMng_->SetFlag(PEID::Default);
	}
	else
	{
	//	uiManager_ = std::make_unique<UiManager>("Resource/Other/UiData/GameOver.ui");
		//peManager_->SetFlag(PEID::CtoM);
	}

	lpSceneManager.GetResourceManager().LoadTexture(frame_, "./Resource/resource/ResultFrame.png");

	AddLoadedFunc([this](auto& contr) {
		//uiManager_->Begin();
		});
}

void ResultScene::NextStage(void)
{
	stageID_ = static_cast<StageID>(std::underlying_type <StageID>::type(stageID_) + 1);
	ChangeSceneID(SceneID::Game);
}

BaseScene::SceneUptr ResultScene::MakeSelectFunc(SceneUptr own)
{
	//lpSooundPross.Release();
	return std::make_unique<FadeLoading>(std::move(own), std::make_unique<SelectScene>(), 2.0f);
}

BaseScene::SceneUptr ResultScene::MakeRetryFunc(SceneUptr own)
{
	//lpSooundPross.Release();
	return std::make_unique<FadeLoading>(std::move(own), std::make_unique<GameScene>(stageID_), 2.0f);
}

BaseScene::SceneUptr ResultScene::MakeDialogFunc(SceneUptr own)
{
	return std::make_unique<DialogScene>(std::move(own));
}

void ResultScene::Update(float delta, Controller& controller)
{
	//uiManager_->Update(delta, *this, *objManager_, controller);
	//peManager_->Update(delta);
}

void ResultScene::DrawScene(void)
{
	SetDrawScreen(*screenHandle_);
	ClsDrawScreen();
	//peMng_->Draw(*capture_, *screenHandle_, -1, -1, -1);
	DrawGraph(0, 0, *frame_, true);
	///uiManager_->Draw();
}

bool ResultScene::IsLoaded(void)
{
	return BaseScene::IsLoaded();//;&& //uiManager_->IsLoaded();
}
