#include <DxLib.h>
#include "../../SceneManager.h"
#include "Fade.h"

#include "../../Common/Debug.h"

Fade::Fade(SceneUptr before, SceneUptr after, float fadeTime) :
	Transition{ std::move(before),std::move(after) }, fadeTime_{ fadeTime }
{
	time_ = fadeTime_;
	draw_ = &Fade::DrawFadeOut;
	update_ = &Fade::UpdateFadeOut;

	after_->DrawScene();
}

bool Fade::UpdateFadeOut(void)
{
	if (time_ <= fadeTime_ / 2.0f)
	{
		draw_ = &Fade::DrawFadeIn;
		update_ = &Fade::UpdateFadeIn;
		after_->DrawScene();
	}
	return false;
}

bool Fade::UpdateFadeIn(void)
{
	return time_ <= 0;
}

bool Fade::IsTransAfter(float delta)
{
	time_ -= delta;
	return (this->*update_)();
}

void Fade::DrawScene(void)
{
	SetDrawScreen(*screenHandle_);
	ClsDrawScreen();
	(this->*draw_)();
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void Fade::DrawFadeOut(void)
{
	float alpha = time_ - (fadeTime_ / 2.0f);
	alpha /= (fadeTime_ / 2.0f);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255.0f * alpha));
	before_->Draw();
}

void Fade::DrawFadeIn(void)
{
	float alpha = time_ / (fadeTime_ / 2.0f);
	alpha = 1 - alpha;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255.0f * alpha));
	after_->Draw();
}
