#include "../Common/Input/Controller.h"
#include "ButtonBase.h"

ButtonBase::ButtonBase(const Vector2& pos) :
	UiBase{ pos }, isHitCursor_{ false }
{
}

ButtonBase::~ButtonBase()
{
}

bool ButtonBase::IsDecision(Controller& controller) const
{
	return controller.PressedDecision();
}
