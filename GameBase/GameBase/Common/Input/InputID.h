#pragma once
#include <type_traits>

// ƒL[î•ñ
enum class InputID
{
	Jump,
	Dash,
	MainTrigerAttack,
	MainTrigerChange,
	SubTrigerAttack,
	SubTrigerChange,
	LockOn,
	CameraFlont,
	Max
};

static InputID begin(InputID) { return InputID::Jump; }
static InputID end(InputID) { return InputID::Max; }
static InputID operator++(InputID& state) { return (state = static_cast<InputID>(std::underlying_type<InputID>::type(state) + 1)); }
static InputID operator*(const InputID& state) { return state; }
