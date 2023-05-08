#include <DxLib.h>
#include <time.h>
#include "Controller.h"

#include "../Debug.h"

Controller::Controller()
{
    cursorSpeed_ = 120.0f;
    cursorPos_ = lpSceneManager.screenSize_<float> / 2.0f;
    isPress_ = false;
    isFinish_ = false;
    PressCount_ = 0;
    data_.fill(std::make_pair(false, false));
}

Controller::~Controller()
{
}

ControllerType Controller::GetControllerType(void)
{
   
}

bool Controller::MousePress(ControllerInputID id)
{
    if (ControllerInputID::Attack == id)
    {
        if (GetMouseInput() & MOUSE_INPUT_LEFT)
        {
            return true;
        }
    }
    return false;
}

bool Controller::Press(ControllerInputID id)
{
    return  data_[static_cast<size_t>(id)].first;
}

bool Controller::Pressed(ControllerInputID id)
{
    return data_[static_cast<size_t>(id)].first && !data_[static_cast<size_t>(id)].second;
}

bool Controller::Released(ControllerInputID id)
{
    return !data_[static_cast<size_t>(id)].first && data_[static_cast<size_t>(id)].second;
}

bool Controller::NotPress(ControllerInputID id)
{
    return !data_[static_cast<size_t>(id)].first;
}

bool Controller::IsNeutral()
{
    bool rtnflag = false;
    for (int n = 0; n < static_cast<int>(ControllerInputID::Max); n++)
    {
        rtnflag |= data_[static_cast<size_t>(static_cast<ControllerInputID>(n))].first;
    }
    return !rtnflag;
}

bool Controller::IsAnyPress()
{
    bool rtnflag = false;
    for (int n = 0; n < static_cast<int>(ControllerInputID::Max); n++)
    {
        rtnflag |= data_[static_cast<size_t>(static_cast<ControllerInputID>(n))].first && !data_[static_cast<size_t>(static_cast<ControllerInputID>(n))].second;
    }
    return !rtnflag;
}


std::string Controller::LongPress(ControllerInputID id, double limit, double delta)
{
    if (!isPress_)
    {
        if (Press(ControllerInputID::Attack))
        {
            isPress_ = true;
            PressCount_ = 0;
        }
    }
    else
    {
        PressCount_ += (float)delta;
        if (!Press(ControllerInputID::Attack))
        {
            isPress_ = false;
            isFinish_ = true;
        }

        if (isFinish_)
        {
            isFinish_ = false;
            if (PressCount_ < limit)
            {
                return "Attack";
            }
            else
            {
                return "LongAttack";
            }
        }
    }
    return "";
}

const InputData& Controller::Get(void)
{
    return data_;
}

bool Controller::PressDecision(void) const
{
    return  decisionData_.first;
}

bool Controller::PressedDecision(void) const
{
    return decisionData_.first && !decisionData_.second;
}

bool Controller::PressCancel(void) const
{
    return cancelData_.first;
}

bool Controller::PressdCancel(void) const
{
    return cancelData_.first && !cancelData_.second;
}
