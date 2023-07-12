#pragma once
#include <memory>
#include <vector>
#include <string>
#include <functional>
#include "../Common/Vector2.h"
#include "../Common/Shared.h"
#include "UiID.h"
#include <filesystem>

class ObjectManager;
class Controller;
class BaseScene;

class UiBase
{
public:
	UiBase(const Vector2& pos);
	virtual ~UiBase() = default;

	virtual void Update(float delta, BaseScene& scene, ObjectManager& objMng, Controller& controller) = 0;
	virtual void Draw(int mainScr) = 0;
	virtual UiID GetUIID(void) = 0;
	virtual void Loaded(void);

	/// <summary>
	/// ボタンとして機能するか(ButtonBaseを継承しているか？)
	/// </summary>
	/// <param name=""></param>
	/// <returns> ボタンの時true、そうでないときfalse </returns>
	virtual bool IsButton(void) const
	{
		return false;
	}

	/// <summary>
	/// 座標の取得
	/// </summary>
	/// <param name=""></param>
	/// <returns> 座標 </returns>
	const Vector2& GetPos(void) const&
	{
		return pos_;
	}

	/// <summary>
	/// サイズの取得(縦横)
	/// </summary>
	/// <param name=""></param>
	/// <returns> サイズ(縦横) </returns>
	const Vector2& GetSize(void) const&
	{
		return size_;
	}

	/// <summary>
	/// 分割数の取得(縦横)
	/// </summary>
	/// <param name=""></param>
	/// <returns> 分割数(縦横) </returns>
	const Vector2& GetDiv(void) const&
	{
		return div_;
	}
protected:
	// 座標
	Vector2 pos_;

	// サイズ
	Vector2 size_;

	// 画像分割数
	Vector2I div_;
};

