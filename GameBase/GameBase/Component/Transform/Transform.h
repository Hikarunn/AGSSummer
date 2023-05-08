#pragma once
#include <list>
#include <DxLib.h>
#include <fstream>
#include "../../Common/Math.h"
#include "../../Common/Quaternion.h"
#include "../ComponentBase.h"



// トランスフォームのクラス
class Transform :
	public ComponentBase
{
public:
	Transform();

	void Update(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller) final;


	void End(ObjectManager& objectManager) final;

	void Begin(ObjectManager& objectManager) final
	{
		if (!std::isfinite(rot_.w))
		{
			return;
		}
	}

	/// <summary>
	/// 前方方向のベクトルを取得
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	const Vector3 GetForward(void) const;

	/// <summary>
	/// 上方向のベクトルを取得
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	const Vector3 GetUp(void) const;


	/// <summary>
	/// 右方向のベクトルを取得
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	const Vector3 GetRight(void) const;


	/// <summary>
	/// トランスフォームを指定ベクトルへ傾ける
	/// </summary>
	/// <param name="vec"> 傾ける方向のベクトル </param>
	void LookAt(const Vector3& vec);

	/// <summary>
	/// 回転を追加する(クォータニオン)
	/// </summary>
	/// <param name="addRotation"> 追加する回転(クォータニオン) </param>
	/// <returns></returns>
	void AddRotation(const Quaternion& addRotation)&
	{
		rot_ *= addRotation;
	}

	/// <summary>
	/// 回転をセットする(クォータニオン)
	/// </summary>
	/// <param name="rotation"></param>
	void SetRotation(const Quaternion& rotation)&
	{
		rot_ = rotation;
	}

	/// <summary>
	/// 回転の取得
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	const Quaternion GetRotation(void) const;

	void SetLocalRotation(const Quaternion& rotation)&
	{
		localRot_ = rotation;
	}

	void AddLocalRotation(const Quaternion& rotation)&
	{
		localRot_ *= rotation;
	}

	/// <summary>
	/// 座標の取得
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	const Vector3& GetPos(void) const&
	{
		return pos_;
	}

	/// <summary>
	/// 座標の参照の取得
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	Vector3& Pos(void)&
	{
		return pos_;
	}

	/// <summary>
	/// スケーリングの取得
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	const Vector3& GetScale(void) const&
	{
		return scale_;
	}

	/// <summary>
	/// スケーリングの参照の取得
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	Vector3& Scale(void)&
	{
		return scale_;
	}

	Transform& operator=(const Transform& transform);



	/// <summary>
	/// ファイルからデータを読み込み適応する
	/// </summary>
	/// <param name="file"></param>
	void Read(std::ifstream& file);

	/// <summary>
	/// ファイルから座標、回転、拡縮のデータを取得する
	/// </summary>
	/// <param name="file"></param>
	/// <returns></returns>
	static std::tuple<Vector3, Vector3, Vector3> Load(std::ifstream& file);

	SetID(ComponentID::Transform, ComponentID::Transform)
		ComponentID GetID(void) const override
	{
		return id_;
	}
private:

	Quaternion rot_;

	Quaternion localRot_;


	// 座標
	Vector3 pos_;

	// 拡大縮小
	Vector3 scale_{ 1.0f,1.0f,1.0f };

	// 親からのオフセット位置
	Vector3 offset_{ zeroVector3<float> };
};

