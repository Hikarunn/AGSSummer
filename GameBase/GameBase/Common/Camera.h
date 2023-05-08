#pragma once
#include <DxLib.h>
#include "Vector3.h"
#include "ScreenID.h"
#include "../SceneManager.h"
#include "Quaternion.h"

class ObjectManager;
class Controller;

class Camera
{
public:

	/// <summary>
	/// カメラのセットアップ(描画用)
	/// </summary>
	/// <param name=""></param>
	void SetScreen(void)const;

	/// <summary>
	/// カメラの上方向を取得する
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	const Vector3 GetUp(void)const;

	/// <summary>
	/// カメラ前方向を取得する
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	const Vector3 GetForward(void)const;

	/// <summary>
	/// カメラの座標をセット
	/// </summary>
	/// <param name="pos"></param>
	void SetPos(const Vector3& pos)
	{
		pos_ = pos;
	}

	/// <summary>
	/// シャドウマップ描画用のカメラ
	/// </summary>
	/// <param name="size"></param>
	/// <param name="ner"></param>
	/// <param name="fur"></param>
	/// <param name="target"></param>
	void SetUpShadow(float size, float ner, float fur, const Vector3& target)const;

	/// <summary>
	/// 回転をクォータニオンでセットする
	/// </summary>
	/// <param name="q"></param>
	void SetRotation(const Quaternion& q);

private:

	// カメラ座標
	Vector3 pos_;

	// カメラの回転
	Quaternion quaRot_;

};

