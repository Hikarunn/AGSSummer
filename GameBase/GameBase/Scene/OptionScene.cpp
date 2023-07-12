#include <DxLib.h>
#include "../UI/UiManager.h"
#include "../SceneManager.h"
#include "../Common/ResourceManager.h"
#include "InputSetScene.h"
#include "OptionScene.h"


OptionScene::OptionScene(SceneUptr owner) :
	WindowScene{ std::move(owner), 0.25f, ScreenID::OptionUi,ScreenID::Option, SceneID::Option }
{
	SetUseASyncLoadFlag(false);
	uiManager_ = std::make_unique<UiManager>("Resource/Other/UiData/Option.ui", false);
	uiManager_->Begin();
	lpSceneManager.GetResourceManager().LoadTexture(frame_, "Resource/resource/Ui/OptionWindow.png");
	SetUseASyncLoadFlag(true);

	SetMakeSceneFunc(std::bind(&OptionScene::MakeInputSetFunc, this, std::placeholders::_1), SceneID::InputSet);
	setResultcode_ = lpConfigManager.GetInputCode();

//	onPeMono_ = lpConfigMng.GetPeConfig().at(PEID::Mono);
	//onPeFog_ = lpConfigMng.GetPeConfig().at(PEID::VolFog);
}

void OptionScene::SetInputCode(InputID id, int code)
{
	auto itr = std::find_if(setResultcode_.begin(), setResultcode_.end(), [code](auto& itr) { return itr.second == code; });
	if (itr != setResultcode_.end())
	{
		itr->second = setResultcode_[id];
	}
	setResultcode_[id] = code;
}

InputCode& OptionScene::GetInputCode(void)&
{
	return setResultcode_;
}

void OptionScene::Default(void)
{
	lpConfigManager.SetDefalutCode();
	setResultcode_ = lpConfigManager.GetInputCode();

	onPeMono_ = false;
	onPeFog_ = true;
	//lpConfigManager.SetPeConfig(PEID::Mono, onPeMono_);
	//lpConfigManager.SetPeConfig(PEID::VolFog, onPeFog_);
}

void OptionScene::Save(void)
{
	// 設定した結果をコンフィグに反映させる
	lpConfigManager.SetInputCode(setResultcode_);

	//lpConfigManager.SetPeConfig(PEID::Mono, onPeMono_);
	//lpConfigManager.SetPeConfig(PEID::VolFog, onPeFog_);
}


void OptionScene::UpdateOpend(float delta, Controller& controller)
{
	uiManager_->Update(delta, *this, *objManager_, controller);
}

void OptionScene::DrawWindow(void)
{
	DrawRotaGraph(SceneManager::screenSize_<int>.x / 2, SceneManager::screenSize_<int>.y / 2, 1.0, 0.0f, *frame_, true);
	uiManager_->Draw(*screenHandle_);
}

void OptionScene::Closed(void)
{
	WindowScene::Closed();
	owner_->ChangeSceneID(owner_->GetID());
	ChangeSceneID(owner_->GetID());
}


BaseScene::SceneUptr OptionScene::MakeInputSetFunc(SceneUptr own)
{
	return  std::make_unique<InputSetScene>(std::move(own), setID_);
}
