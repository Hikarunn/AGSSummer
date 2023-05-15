#pragma once
#include "../Controller.h"

// キーボード用
class Keyboard :
	public Controller
{
public:
	Keyboard();
	~Keyboard();
	bool Init(void)override;										// 初期化
	void Update(float delta) override;								// 更新 Update(deltaTime)
	ControllerType GetControllerType(void) override { return ControllerType::Key; }		// コントローラータイプの取得private:
	const std::array<char, 256>& GetKeyData(void) const&
	{
		return keyData_;
	}
private:

	/// <summary>
	/// カーソルの座標をセットする
	/// </summary>
	/// <param name="pos"> 座標 </param>
	void SetCursorPos(const Vector2& pos = lpSceneManager.screenSize_<float> / 2.0f);

	// キーデータ格納用
	std::array<char, 256> keyData_ = {};
};

