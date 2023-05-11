#include <DxLib.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "Keyboard.h"
#include "../InputConfig/InputConfig.h"
#include "../../../SceneManager.h"
#include "../../Debug.h"

Keyboard::Keyboard()
{
	keyData_.fill(0);
	Init();
}

Keyboard::~Keyboard()
{
}

bool Keyboard::Init(void)
{
	return true;
}

void Keyboard::Update(float delta)
{
	GetHitKeyStateAll(keyData_.data());		// キーデータをまとめて取得
	auto mouseState = GetMouseInput();
	for (auto& code : lpConfigManager.GetInputCode())
	{
		data_[static_cast<size_t>(code.first)].second = data_[static_cast<size_t>(code.first)].first;
		if (code.second < 0)
		{
			data_[static_cast<size_t>(code.first)].first = std::abs(code.second) & mouseState;
		}
		else
		{
			data_[static_cast<size_t>(code.first)].first = keyData_[code.second];
		}
	}

	leftInput_ = zeroVector2<float>;
	leftInput_.x += (keyData_[KEY_INPUT_W] ? -1.0f : 0.0f);
	leftInput_.x += (keyData_[KEY_INPUT_S] ? 1.0f : 0.0f);

	leftInput_.y += (keyData_[KEY_INPUT_D] ? -1.0f : 0.0f);
	leftInput_.y += (keyData_[KEY_INPUT_A] ? 1.0f : 0.0f);
	leftInput_.Normalize();
	//DebugLog("x=", inputVec_.x, "y=", inputVec_.y, "sq=", inputVec_.SqMagnitude());
	int x = 0, y = 0;
	GetMousePoint(&x, &y);
	rightInput_ = cursorPos_ = Vector2{ static_cast<float>(x), static_cast<float>(y) };

	decisionData_.second = decisionData_.first;
	decisionData_.first = mouseState & MOUSE_INPUT_LEFT;

	cancelData_.second = cancelData_.first;
	cancelData_.first = keyData_[KEY_INPUT_ESCAPE];
}

void Keyboard::SetCursorPos(const Vector2& pos)
{
	SetMousePoint(static_cast<int>(pos.x), static_cast<int>(pos.y));
	cursorPos_ = pos;
}
