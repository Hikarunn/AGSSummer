#pragma once
#include "ButtonBase.h"
#include <functional>

// 音量などの調整に使われるUI
class Slider :
	public ButtonBase
{
	using ValueGetFunc = std::function<void(const float)>;
public:
	Slider(const ValueGetFunc& valueGetFunc, const std::filesystem::path& sliderBarPath, const std::filesystem::path& sliderTabPath, float defaultValue, const Vector2& pos);
	void Update(float delta, BaseScene& scene, ObjectManager& objMng, Controller& controller);
	void Draw(int mainScr);

private:
	void Push(BaseScene& scene, Controller& controller) final;
	void HitCursor(void) final;
	void NotHitCursor(void)final;
	bool IsDecision(Controller& controller) const final;

	UiID GetUIID(void) final
	{
		return UiID::Button;
	}
	void Loaded(void) final;

	// 外部から値を知るために使う
	ValueGetFunc valueGetFunc_;

	// スライダーのバーの画像
	SharedGraphicHandle sliderBar_;

	// スライダーの持ちての画像
	SharedGraphicHandle sliderTab_;

	// スライダーの持ち手のサイズ
	Vector2 sliderTabSize_;

	// スライダーの持ち手の座標
	Vector2 sliderTabPos_;

	// 値(0～1の範囲)
	float value_;
};

