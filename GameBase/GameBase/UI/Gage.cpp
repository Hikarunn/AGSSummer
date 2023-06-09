#include <DxLib.h>
#include "../Common/ResourceManager.h"
#include "../SceneManager.h"
#include "../Object/ObjectManager.h"
#include "../Component/Transform/Transform.h"
#include "../Component/Behavior/EnemyBehavior.h"
#include "Gage.h"

#include "../Common/Debug.h"

Gage::Gage(const int& id, const DataBindFunc& dataBindFunc, const std::filesystem::path& gagePath, const std::filesystem::path& framePath, float defalutValue, const Vector2& pos) :
	UiBase{ pos }, id_{ static_cast<GageID>(id) }, dataBindFunc_{ dataBindFunc }, defValue_{ defalutValue }, value_{ defalutValue }, subGageValue_{ defalutValue }
{
	lpSceneManager.GetResourceManager().LoadTexture(gageHandle_, gagePath);
	lpSceneManager.GetResourceManager().LoadTexture(frameHandle_, framePath);
	power_ = 0.0f;
}

void Gage::Update(float delta, BaseScene& scene, ObjectManager& objMng, Controller& controller)
{
	// 各ゲージ更新
	switch (id_)
	{
	case GageID::Trion:
		TrionUpdate(objMng);
		break;

	default:
		break;
	}
}

void Gage::TrionUpdate(ObjectManager& objManager)
{
	// ゲージ変化なしの状態は入らない
	if (Clamp(dataBindFunc_(objManager) / size_.x) == subGageValue_)
	{
		return;
	}
	// 減少値
	power_ = objManager.GetComponent<EnemyBehavior>(objManager.GetEnemyID())->GetEnemyPower();
	// 規定値に到達したら
	if (value_ <= Clamp(dataBindFunc_(objManager) / size_.x))
	{
		// 規定値代入
		AssignGageValue(objManager);
		// サブゲージ減少処理
		DecSubGageValue(objManager, 100.0f);
		return;
	}
	// 値増減処理
	FlucGageValue(objManager, 20.0f, false);
}

//void Gage::SkillUpdate(ObjectManager& objMng)
//{
//	// ゲージリセット
//	if (!dataBindFunc_(objMng))
//	{
//		value_ = 0.0f;
//	}
//	// 規定値に到達したら
//	if (value_ >= Clamp(dataBindFunc_(objMng) / size_.x))
//	{
//		// 規定値代入
//		AssignGageValue(objMng);
//		return;
//	}
//	// 値増減処理
//	FlucGageValue(objMng, 50.0f, true);
//}

void Gage::FlucGageValue(ObjectManager& objMng, float cnt, bool isUp)
{
	// 増減判定(増:1、減:-1)
	auto fluc = isUp ? 1 : -1;
	// 加算処理
	value_ += Clamp((dataBindFunc_(objMng) / size_.x) / cnt) * fluc;
	DebugLog(dataBindFunc_(objMng));
}

void Gage::DecSubGageValue(ObjectManager& objMng, float cnt)
{
	// 減少処理
	subGageValue_ -= Clamp((dataBindFunc_(objMng) / size_.x) / cnt);
	// 規定値に到達したら
	if (subGageValue_ <= Clamp(dataBindFunc_(objMng) / size_.x))
	{
		// 規定値代入
		subGageValue_ = value_;
	}
}

void Gage::AssignGageValue(ObjectManager& objMng)
{
	// 規定値代入
	value_ = Clamp(dataBindFunc_(objMng) / size_.x);
}

void Gage::Draw(int mainScr)
{
	// HPゲージのみサブゲージ表示
	if (id_ == GageID::Trion)
	{
		DrawBoxAA(pos_.x, pos_.y, pos_.x + size_.x * subGageValue_ + 2.0f, pos_.y + size_.y + 2.0f, 0xff0000, true);
	}
	// メインゲージ
	DrawRectGraphF(pos_.x, pos_.y, 2, 2, static_cast<int>(size_.x * value_ + 2.0f), static_cast<int>(size_.y + 2.0f), *gageHandle_, true);
	// ゲージフレーム
	DrawGraphF(pos_.x, pos_.y, *frameHandle_, true);
}

void Gage::Loaded(void)
{
	GetGraphSizeF(*gageHandle_, &size_.x, &size_.y);
}
