#pragma once
#include "../Common/Vector2.h"
#include "../UI/UiBase.h"

class BaseScene;

// �J�[�\��
class Cursor :
	public UiBase
{
public:
	Cursor();
	void Check(std::list<std::unique_ptr<UiBase>>& uiList, BaseScene& scene, Controller& controller);
private:
	void Update(float delta, BaseScene& scene, ObjectManager& objMng, Controller& controller) final;
	void Draw(int mainScr) final;
	UiID GetUIID(void) { return UiID::Cursor; };

	// �J�[�\���摜
	SharedGraphicHandle handle_;
};

