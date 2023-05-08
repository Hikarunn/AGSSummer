#pragma once
#include <map>
#include <array>
#include <string>
#include "InputID.h"
#include "../Vector2.h"
#include "../../SceneManager.h"

// トリガー情報
enum class Triger
{
	Now,			// 現在
	Old,			// ひとつ前
	Mac			
};

// 入力装置の種類
enum class ControllerType
{
	Key,			// キーボード
	Pad,			// パッド
	Max
};

using InputData = std::array<std::pair<bool, bool>, static_cast<size_t>(ControllerInputID::Max) + 1>;

class Controller
{
public:
	bool operator()(float delta)
	{
		if (this != nullptr)
		{
			Update(delta);
		}
		return this != nullptr;
	}
	Controller();
	virtual ~Controller();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name=""></param>
	/// <returns>成功時→true、失敗時→false</returns>
	virtual bool Init(void) = 0;

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="delta">デルタタイム</param>
	virtual void Update(float delta) = 0;

	/// <summary>
	/// コントローラーの種類取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>コントローラーの種類</returns>
	virtual ControllerType GetControllerType(void) = 0;

	/// <summary>
	/// 左クリックしているか
	/// </summary>
	/// <param name="id">キーの種類</param>
	/// <returns>成否→　true,false</returns>
	bool MousePress(ControllerInputID id);

	/// <summary>
	/// 押しているか
	/// </summary>
	/// <param name="id">キーの種類</param>
	/// <returns></returns>
	bool Press(ControllerInputID id);

	/// <summary>
	/// 押した瞬間
	/// </summary>
	/// <param name="id">キーの種類</param>
	/// <returns></returns>
	bool Pressed(ControllerInputID id);

	/// <summary>
	/// 離した瞬間
	/// </summary>
	/// <param name="id">キーの種類</param>
	/// <returns></returns>
	bool Released(ControllerInputID id);

	/// <summary>
	/// 押されていないとき
	/// </summary>
	/// <param name="id"></param>
	/// <returns></returns>
	bool NotPress(ControllerInputID id);

	/// <summary>
	/// 何も押されていないかどうか
	/// </summary>
	/// <returns></returns>
	bool IsNeutral();

	/// <summary>
	/// 何か押されたか
	/// </summary>
	/// <returns></returns>
	bool IsAnyPress();

	/// <summary>
	/// カーソルの位置セット
	/// </summary>
	/// <param name="pos"></param>
	virtual void SetQursorPos(const Vector2& pos = lpSceneManager.screenSize_<float> / 2.0f) = 0;

	/// <summary>
	/// カーソル位置の取得
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	const Vector2& GetCursorPos(void)const&
	{
		return cursorPos_;
	}

	/// <summary>
	/// カーソルのスピードセット
	/// </summary>
	/// <param name="speed"></param>
	void SetCursorSpeed(float speed)
	{
		cursorSpeed_ = speed;
	}

	/// <summary>
	/// カーソルのスピードを取得
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	const float GetCursorSpeed(void) const
	{
		return cursorSpeed_;
	}

	/// <summary>
	/// 移動用
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	const Vector2& GetLeftInput(void) const&
	{
		return leftInput_;
	}

	/// <summary>
	/// 視点操作用
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	const Vector2& GetRightInput(void) const&
	{
		return rightInput_;
	}

	/// <summary>
	/// 長押し
	/// </summary>
	/// <param name="id">キーの種類</param>
	/// <param name="limit">最大時間</param>
	/// <param name="delta">デルタタイム</param>
	/// <returns></returns>
	std::string LongPress(ControllerInputID id, double limit, double delta);

	/// <summary>
	/// インプットデータを取得
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	const InputData& Get(void);

	/// <summary>
	/// 決定ボタンが押されているかどうか
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	bool PressDecision(void) const;

	/// <summary>
	/// 決定ボタンが押された瞬間かどうか
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	bool PressedDecision(void) const;

	/// <summary>
	/// キャンセルボタンが押されているかどうか
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	bool PressCancel(void) const;

	/// <summary>
	/// キャンセルボタンが押された瞬間かどうか
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	bool PressdCancel(void) const;

protected:
	//CntData cntData_;
	InputData data_;

	// 決定下かの情報の情報
	std::pair<bool, bool> decisionData_;

	// キャンセル押したかのデータ
	std::pair<bool, bool> cancelData_;

	// カーソルの座標
	Vector2 cursorPos_;

	// カーソルのスピード(ui時に使用)
	float cursorSpeed_;

	// 移動用の入力された方向
	Vector2 leftInput_;

	// 振り向き用入力座標
	Vector2 rightInput_;

	/// <summary> LongPress関係 </summary>
	bool isPress_;
	bool isFinish_;
	float PressCount_;
};

