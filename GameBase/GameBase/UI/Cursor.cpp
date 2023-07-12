#include <DxLib.h>
#include "../Common/Input/Controller.h"
#include "../Common/ResourceManager.h"
#include "Button.h"
//#include "../Common/SoundPross.h"
#include "Cursor.h"

#include "../Common/Debug.h"

constexpr float cursorSize{ 20.0f };

Cursor::Cursor() :
	UiBase{ zeroVector2<float> }
{
	lpSceneManager.GetResourceManager().LoadTexture(handle_, "Resource/resource/Ui/cursor.png");
}

void Cursor::Update(float delta, BaseScene& scene, ObjectManager& objMng, Controller& controller)
{
	pos_ = controller.GetCursorPos();
}

void Cursor::Draw(int mainScr)
{
	DrawRotaGraphF(pos_.x, pos_.y, 1.0, 0.0, *handle_, true);
}

void Cursor::Check(std::list<std::unique_ptr<UiBase>>& uiList, BaseScene& scene, Controller& controller)
{
	DebugDrawString("x=", controller.GetCursorPos().x, "y=", controller.GetCursorPos().x);
	auto checkpos = pos_;
	for (auto& ui : uiList)
	{
		ButtonBase* button;
		if (!ui->IsButton())
		{
			// �{�^���ł͂Ȃ��Ƃ��֌W�Ȃ��̂ŃX�L�b�v
			continue;
		}

		button = static_cast<ButtonBase*>(ui.get());
		if (ui->GetPos().x <= checkpos.x && (ui->GetPos().x + ui->GetSize().x) >= checkpos.x &&
			ui->GetPos().y <= checkpos.y && (ui->GetPos().y + ui->GetSize().y) >= checkpos.y
			)
		{
			if (!button->IsHitCursor())
			{
				// �J�[�\������������Ƃ�`����
			//	lpSooundPross.PlayBackSound(SOUNDNAME_SE::cursorMove, lpSooundPross.GetVolume(), false);
				button->HitCursor();
			}

			if (button->IsDecision(controller))
			{
				// �N���b�N�������Ƃ�`����
				//lpSooundPross.PlayBackSound(SOUNDNAME_SE::click, lpSooundPross.GetVolume(), false);
				button->Push(scene, controller);
			}

		}
		else
		{
			if (button->IsHitCursor())
			{
				// �J�[�\�����痣�ꂽ���Ƃ�`����
				button->NotHitCursor();
			}
		}
	}
}
