#include <DxLib.h>
#include "Pad.h"
#include "../InputConfig/InputConfig.h"

#include "../../Debug.h"

Pad::Pad(int padType) :
	nowPadType_{ padType }, update_{ nullptr }
{
	if (nowPadType_ == DX_PADTYPE_DUAL_SHOCK_4 || nowPadType_ == DX_PADTYPE_DUAL_SENSE)
	{
		update_ = &Pad::UpdatePsPad;
	}
	else if (nowPadType_ == DX_PADTYPE_XBOX_360 || nowPadType_ == DX_PADTYPE_XBOX_ONE)
	{
		update_ = &Pad::UpdateXboxPad;
	}
	else
	{
		// とりあえずxbox系と同じの入れとく
		update_ = &Pad::UpdateXboxPad;
	}

	std::fill(std::begin(state_.Buttons), std::end(state_.Buttons), 0);
	Init();

}

Pad::~Pad()
{
}

bool Pad::Init(void)
{
	return true;
}

void Pad::Update(float delta)
{
	GetJoypadDirectInputState(DX_INPUT_PAD1, &state_);

	for (auto& code : lpConfigManager.GetInputCode())
	{
		data_[static_cast<size_t>(code.first)].second = data_[static_cast<size_t>(code.first)].first;
		data_[static_cast<size_t>(code.first)].first = state_.Buttons[code.second];
	}

	leftInput_ = Vector2{ static_cast<float>(state_.Y), static_cast<float>(-state_.X) };
	if (leftInput_.SqMagnitude() <= Square(500.0f))
	{
		leftInput_ = zeroVector2<float>;
	}
	else
	{
		leftInput_.Normalize();
		cursorPos_ += Vector2{ static_cast<float>(-leftInput_.y),static_cast<float>(leftInput_.x), } *cursorSpeed_ * delta;
	}

	// スティックとかの更新
	(this->*update_)(delta);
}

const int Pad::GetPadType(void) const
{
	return nowPadType_;
}

void Pad::SetCursorPos(const Vector2& pos)
{
	cursorPos_ = pos;
	rightInput_ = pos;
}

void Pad::UpdatePsPad(float delta)
{
	decisionData_.second = decisionData_.first;
	decisionData_.first = state_.Buttons[1];
	cancelData_.second = cancelData_.first;
	cancelData_.first = state_.Buttons[9];
	Vector2 move{ static_cast<float>(state_.Z),static_cast<float>(state_.Rz) };
	rightInput_ = zeroVector2<float>;
	if (move.SqMagnitude() <= Square(500.0f))
	{
		return;
	}
	move.Normalize();
	rightInput_ += move * cursorSpeed_ * delta;
}

void Pad::UpdateXboxPad(float delta)
{
	decisionData_.second = decisionData_.first;
	decisionData_.first = state_.Buttons[0];
	cancelData_.second = cancelData_.first;
	cancelData_.first = state_.Buttons[7];
	Vector2 move{ static_cast<float>(state_.Rx),static_cast<float>(state_.Ry) };
	if (move.SqMagnitude() <= Square(500.0f))
	{
		return;
	}
	move.Normalize();
	rightInput_ += move * cursorSpeed_ * delta;
}
