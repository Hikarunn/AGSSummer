#pragma once
#include "../Common/Vector2.h"
#include "../UI/UiBase.h"

class BaseScene;

// カーソル
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

	// カーソル画像
	SharedGraphicHandle handle_;
};

