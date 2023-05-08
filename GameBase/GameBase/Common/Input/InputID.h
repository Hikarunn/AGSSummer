#pragma once
#include <type_traits>

// ÉLÅ[èÓïÒ
enum class ControllerInputID
{
	Jump,
	Attack,
	MainTriger1,
	MainTriger2,
	MainTriger3,
	MainTriger4,
	SubTriger1,
	SubTriger2,
	SubTriger3,
	SubTriger4,
	LockOn,
	CameraFlont,
	Max
};

enum class KeyInputID
{
	Jump,
	Attack,
	MainTrigerChange,
	SubTrigerAttack,
	SubTrigerChange1,
	SubTrigerChange2,
	SubTrigerChange3,
	SubTrigerChange4,
	LockOn,
	CameraFlont,
	Max
};

static ControllerInputID begin(ControllerInputID) { return ControllerInputID::Jump; }
static ControllerInputID end(ControllerInputID) { return ControllerInputID::Max; }
static ControllerInputID operator++(ControllerInputID& state) { return (state = static_cast<ControllerInputID>(std::underlying_type<ControllerInputID>::type(state) + 1)); }
static ControllerInputID operator*(const ControllerInputID& state) { return state; }

static KeyInputID begin(KeyInputID) { return KeyInputID::Jump; }
static KeyInputID end(KeyInputID) { return KeyInputID::Max; }
static KeyInputID operator++(KeyInputID& state) { return (state = static_cast<KeyInputID>(std::underlying_type<ControllerInputID>::type(state) + 1)); }
static KeyInputID operator*(const KeyInputID& state) { return state; }
