#include <DxLib.h>
#include "../../Object/ObjectManager.h"
#include "../../Common/Math.h"
#include "../Behavior/PlayerBehavior.h"
#include "Animator.h"

#include "../../Common/Debug.h"

void Animator::SetNextAnimation(int idx, float blendTime)
{
	if (nowState_ == nullptr)
	{
		// ���݂̃X�e�[�g���Ȃ��Ƃ��Ƃ肠����idx���Z�b�g����
		nowState_ = animStates_[idx].get();
		nowState_->Attach();
		return;
	}
	if (idx == nowState_->GetAnimIdx() || nowState_->IsBlend())
	{
		// �������u�����h���̎��͏������Ȃ�
		return;
	}
	// �u�����h����������
	blendState_->SetBlendTime(blendTime);
	static_cast<AnimationState&>(*blendState_).Init(nowState_, animStates_[idx].get());
	nowState_ = blendState_.get();

}

void Animator::Update(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller)
{
	nowState_ = nowState_->Update(nowState_, objectManager, delta);
}

void Animator::Begin(ObjectManager& objectManager)
{
	render_ = objectManager.GetComponent<ModelRender>(ownerId_);
	blendState_ = std::make_unique<BlendState>();
}

void Animator::AddAnimation(int animIdx, bool isLoop)
{
	animStates_.emplace(animIdx, std::make_unique<NormalState>());
	animStates_[animIdx]->SetHandle(*render_->GetHandle());
	animStates_[animIdx]->SetUpAnim(animIdx);
	animStates_[animIdx]->SetLoopFlag(isLoop);
}


void NormalState::Init(AnimationState* start, AnimationState* end)
{
	// �������Ȃ�
	playTime_ = 0.0f;
	playTimeOver_ = 0.0f;
	start->Detach();
}

AnimationState* NormalState::Update(AnimationState* state, ObjectManager& objManager, float delta)
{
	if (animIdx_ == static_cast<int>(AnimIndex::Attack3))
	{
		MV1ResetFrameUserLocalMatrix(handle_, MV1SearchFrame(handle_, L"Root"));
		// �U��3�i��
		moveAnimFrameIndex_ = MV1SearchFrame(handle_, L"Center");
		MV1SetFrameUserLocalMatrix(handle_, moveAnimFrameIndex_, MGetIdent());
	}
	else
	{
		MV1ResetFrameUserLocalMatrix(handle_, MV1SearchFrame(handle_, L"Center"));
		moveAnimFrameIndex_ = MV1SearchFrame(handle_, L"Root");
		MV1SetFrameUserLocalMatrix(handle_, moveAnimFrameIndex_, MGetTranslate(VGet(0.0f, -5.0f, 0.0f)));
	}
	prePos_ = MV1GetAttachAnimFrameLocalPosition(handle_, attachIdx_, moveAnimFrameIndex_);

	// https://qiita.com/aa_debdeb/items/abe90a9bd0b4809813da#%E5%9B%9E%E8%BB%A2%E8%A1%8C%E5%88%97%E3%81%8B%E3%82%89%E3%82%AA%E3%82%A4%E3%83%A9%E3%83%BC%E8%A7%92
	//auto m = MV1GetFrameLocalMatrix(handle_, moveAnimFrameIndex_);
	//auto euler = ToQuaternion(m).ToEuler();
	//euler.y += 1.0f;
	//auto qua = ToQuaternion(euler);
	//MV1SetFrameUserLocalMatrix(handle_, moveAnimFrameIndex_, ToMatrix(qua));
	//DebugLog("X:", euler.x, "Y:", euler.y, "Z:", euler.z);

	playTime_ += delta * 60.0f;
	DebugDrawString(playTime_);

	if (playTime_ >= totalTime_)
	{
		MV1SetAttachAnimTime(handle_, attachIdx_, totalTime_);
		nowPos_ = MV1GetAttachAnimFrameLocalPosition(handle_, attachIdx_, moveAnimFrameIndex_);
		pos_ = nowPos_ - prePos_;
		// ���Đ����Ԃ܂ōĐ������Ƃ�
		if (isLoop_)
		{
			// ���[�v����Ƃ��Đ����Ԃ�0�ɂ���
			playTime_ = 0.0f;
			MV1SetAttachAnimTime(handle_, attachIdx_, 0.0f);
			prePos_ = MV1GetAttachAnimFrameLocalPosition(handle_, attachIdx_, moveAnimFrameIndex_);
			MV1SetAttachAnimTime(handle_, attachIdx_, playTime_);
			nowPos_ = MV1GetAttachAnimFrameLocalPosition(handle_, attachIdx_, moveAnimFrameIndex_);
			pos_ = nowPos_ - prePos_;
		}
		else
		{
			// ���[�v�Đ����Ȃ��Ƃ��͑��Đ����Ԃ����Ƃ�
			playTime_ = totalTime_;
		}
		playTimeOver_ += delta * 60.0f;
	}
	else
	{
		MV1SetAttachAnimTime(handle_, attachIdx_, playTime_);
		nowPos_ = MV1GetAttachAnimFrameLocalPosition(handle_, attachIdx_, moveAnimFrameIndex_);
		pos_ += nowPos_ - prePos_;
	}
	return state;
}



void BlendState::Init(AnimationState* start, AnimationState* end)
{
	pos_ = VGet(0.0f, 0.0f, 0.0f);
	start_ = start;
	end_ = end;
	//blendTime_ = 1.0f;
	playTime_ = 0.0f;
	playTimeOver_ = 0.0f;
	end_->Attach();
	end_->ReStart();
	MV1SetAttachAnimBlendRate(start_->GetHandle(), start_->GetAttachIdx());
	MV1SetAttachAnimBlendRate(end_->GetHandle(), end_->GetAttachIdx(), 0.0f);
}

AnimationState* BlendState::Update(AnimationState* state, ObjectManager& objManager, float delta)
{
	// �u�����h����A�j���[�V�������Đ���������
	start_->Update(nullptr, objManager, delta);
	end_->Update(nullptr, objManager, delta);

	// �b���ɉ����ău�����h����ς���
	playTime_ += delta;
	float d = (playTime_ / blendTime_);
	MV1SetAttachAnimBlendRate(end_->GetHandle(), end_->GetAttachIdx(), d);
	MV1SetAttachAnimBlendRate(start_->GetHandle(), start_->GetAttachIdx(), 1.0f - d);
	DebugDrawString(d);
	if (d >= 1.0f)
	{
		// 1�ȏ�̎��u�����h�I���Ȃ̂�
		start_->Detach();
		return end_;
	}

	return state;
}

void AnimationState::ReStart(void)
{
	playTime_ = 0.0f;
	playTimeOver_ = 0.0f;
	MV1SetAttachAnimTime(handle_, attachIdx_, 0.0f);
}

void AnimationState::Attach(void)
{
	attachIdx_ = MV1AttachAnim(handle_, animIdx_);
}

int AnimationState::GetAttachIdx(void)
{
	return attachIdx_;
}

void AnimationState::Detach()
{
	MV1DetachAnim(handle_, attachIdx_);
}

void AnimationState::SetHandle(int handle)
{
	handle_ = handle;
}

const int AnimationState::GetHandle(void) const
{
	return handle_;
}

void AnimationState::SetUpAnim(int idx)
{
	animIdx_ = idx;
	totalTime_ = MV1GetAnimTotalTime(handle_, animIdx_);
}

int AnimationState::GetAnimIdx(void)
{
	return animIdx_;
}
