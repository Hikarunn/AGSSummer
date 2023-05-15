#include "../Common/ResourceManager.h"
#include "WindowScene.h"

WindowScene::WindowScene(SceneUptr owner, float actTime, ScreenID wndScreenID, ScreenID screenID, SceneID sceneID) :
	step_{ 0.0f }, actTime_{ actTime }, owner_{ std::move(owner) },
	update_{ &WindowScene::UpdateOpen }, draw_{ &WindowScene::DrawOpen },
	BaseScene{ screenID, sceneID }
{
	SetUseASyncLoadFlag(false);
	lpSceneManager.GetResourceManager().MakeRenderTarget(window_, wndScreenID, SceneManager::screenSize_<float>, true);
	SetUseASyncLoadFlag(true);

	if (owner_)
	{
		// ŒÄ‚Ño‚µŽå‚ª‘¶Ý‚·‚é‚Æ‚«ŒÄ‚Ño‚µŽå‚É–ß‚é‚½‚ß‚Ìˆ—‚ð’Ç‰Á‚µ‚Æ‚­
		SetMakeSceneFunc([this](auto own) {
			owner_->ChangeSceneID(owner_->GetID()); return std::move(owner_);
			}, owner_->GetID());
		owner_->DrawScene();
	}
}


void WindowScene::Open(void)
{
	update_ = &WindowScene::UpdateOpen;
	draw_ = &WindowScene::DrawOpen;
	step_ = 0.0f;
}

void WindowScene::Close(void)
{
	update_ = &WindowScene::UpdateClose;
	draw_ = &WindowScene::DrawClose;
	step_ = 0.0f;
}

void WindowScene::Update(float delta, Controller& controller)
{
	(this->*update_)(delta, controller);
}

void WindowScene::Closed(void)
{
	step_ = actTime_;
}

void WindowScene::DrawBackGround(void)
{
	if (owner_)
	{
		// ŒÄ‚Ño‚µŽå‚ª‘¶Ý‚·‚éê‡‚Í•`‰æ‚·‚é
		owner_->Draw();
	}
}

void WindowScene::DrawScene(void)
{
	SetDrawScreen(*window_);
	ClsDrawScreen();
	DrawWindow();


	SetDrawScreen(*screenHandle_);
	ClsDrawScreen();
	DrawBackGround();
	(this->*draw_)();
}

void WindowScene::UpdateOpen(float delta, Controller& controller)
{
	step_ += delta;
	if (step_ >= actTime_)
	{
		update_ = &WindowScene::UpdateOpend;
		draw_ = &WindowScene::DrawOpend;
		step_ = 0.0f;
	}
}

void WindowScene::UpdateClose(float delta, Controller& controller)
{
	step_ += delta;
	if (step_ >= actTime_)
	{
		Closed();
	}
}

void WindowScene::DrawOpen(void)
{
	constexpr float halfY = (SceneManager::screenSize_<float>.y / 2.0f);
	float yup = halfY * (1.0f - step_ / actTime_);
	float ybtm = halfY + halfY * step_ / actTime_;
	DrawModiGraphF(
		0.0f, yup,
		SceneManager::screenSize_<float>.x, yup,
		SceneManager::screenSize_ <float>.x, ybtm,
		0.0f, ybtm,
		*window_, true
	);
}

void WindowScene::DrawClose(void)
{
	constexpr float halfY = (SceneManager::screenSize_<float>.y / 2.0f);
	float yup = halfY * step_ / actTime_;
	float ybtm = halfY + halfY * (1.0f - step_ / actTime_);
	DrawModiGraphF(
		0.0f, yup,
		SceneManager::screenSize_<float>.x, yup,
		SceneManager::screenSize_ <float>.x, ybtm,
		0.0f, ybtm,
		*window_, true
	);
}

void WindowScene::DrawOpend(void)
{
	DrawGraph(0, 0, *window_, true);
}
