#include "../Transform/Transform.h"
#include "../../SceneManager.h"
#include "../../Object/ObjectManager.h"
#include "../Info/ObjectInfo.h"
#include "../Behavior/StageBehavior.h"
#include "PlayerBehavior.h"
//#include "EnemyBulletBehavior.h"
#include "PlayerAttackBehavior.h"
//#include "../../Common/SoundPross.h"
#include "../ComponentPool.h"
#include "EnemyBehavior.h"

#include "../../Common/Debug.h"


// ��]���K�v�Ȕ͈�(�p�x)
constexpr float rotDeff{ Square(Deg2Rad(15.0f)) };

// �m�b�N�o�b�N���̏���
constexpr float v0{ 9.0f };

// �m�b�N�o�b�N�p�̏d�͉����x
constexpr float gravity{ 9.80f * 10.0f };

// �R���X�g���N�^
EnemyBehavior::EnemyBehavior()
{
	// �f�t�H���g�̃X�s�[�h�Ƃ���120�����Ƃ�
	speed_ = 120.0f;
	update_ = nullptr;
	stopDistance_ = 0.0f;
	searchDistance_ = 0.0f;
	// �f�t�H���g�̐U��������ԂƂ���1�����Ƃ�
	turnTime_ = 1.0f;
	Init();
}

// �X�V����
void EnemyBehavior::Update(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller)
{
	stateTime_ += delta;

	// �d�͏���
	Gravity(delta);

	(this->*update_)(objectManager, delta);

	//DebugLog("jumpTime_=", jumpTime_);
}

// �J�E���^�ϐ��⎞�ԗp�ϐ��̏�����
void EnemyBehavior::Init(void)
{
	shotTime_ = 0.0f;
	burstCnt_ = 0;
	damageCnt_ = 0;
	isRot_ = false;
	stateTime_ = 0.0f;
	hitTime_ = 0.0f;
	rotSing_ = 0.0f;
	hitCombo_ = 0;
	jumpTime_ = 0.0f;
}

static int num = 1;

// �L�����̊J�n����
void EnemyBehavior::Begin(ObjectManager& objectManager)
{
	// �ϐ���������
	Init();

	// �g�����X�t�H�[�����擾
	transform_ = objectManager.GetComponent<Transform>(ownerId_);

	// �v���C���[�̃g�����X�t�H�[�����擾
	playerT_ = objectManager.GetComponent<Transform>(objectManager.GetPlayerID());

	// �R���C�_�[���擾���q�b�g���̊֐����Z�b�g
	collider_ = objectManager.GetComponent<CharactorCollider>(ownerId_);
	collider_->SetHitFunc(std::bind(&EnemyBehavior::OnHit, this, std::placeholders::_1, std::placeholders::_2));
	collider_->SetGroundFlag(true);

	if (!objectManager.HaveComponent<Collider>(ownerId_))
	{
		assert(false);
	}

	// �X�V�p�֐����Z�b�g
	update_ = &EnemyBehavior::UpdateMove;

	if (isTutorial_)
	{
		shot_ = &EnemyBehavior::NonShot;
	}
	else
	{
		shot_ = &EnemyBehavior::Shot;

	}

	// ���[�U�[�T�C�g�擾
	//laserSight_ = objectManager.GetComponent<EnemyLaserSightBehavior>(laserSightID_);

	// �A�j���[�^�[�擾
	animetor_ = objectManager.GetComponent<Animator>(ownerId_);
	animetor_->AddAnimation(0, true);
	animetor_->AddAnimation(1, true);
	animetor_->AddAnimation(5, true);
	animetor_->SetNextAnimation(0);


	DebugLog("EnemyBehavior::Begin:id=", *ownerId_, ":num=", num++);
}

// �I������
void EnemyBehavior::End(ObjectManager& objectManager)
{
	// �R���|�[�l���g���������ɂȂ������ɌĂ΂��
	auto stage = objectManager.GetComponent<StageBehavior>(stageID_);
	if (stage.IsActive())
	{
		// �X�e�[�W�������Ă��鎞�X�e�[�W�̓G�̃J�E���g�����炳����
		stage->SubEnemy(ownerId_);
	}
}

// �ǐՑO�̏�Ԃ̍X�V
void EnemyBehavior::UpdateSearch(ObjectManager& objectManager, float delta)
{
	if ((transform_->GetPos() - playerT_->GetPos()).SqMagnitude() <= Square(searchDistance_))
	{
		// ���̋����ɂȂ�����ړ��J�n����
		stateTime_ = 0.0f;
		update_ = &EnemyBehavior::UpdateMove;
		animetor_->SetNextAnimation(1, 0.5f);
	}
}

// �ˌ�����
void EnemyBehavior::Shot(ObjectManager& objectManager, float delta)
{
	shotTime_ += delta;
	if (shotTime_ >= 0.0f)
	{
		if (shotTime_ >= shotData_.burstInterval)
		{
			shotTime_ = 0.0f;
			burstCnt_++;
			auto shotPos = transform_->GetPos() + (transform_->GetRotation() * Vector3 { 10.0f, 190.0f, 120.0f });

			// �E������ɂ��炵�č쐬
			/*auto id = objectManager.CreateFromFactory(FactoryID::EnemyBullet, ownerId_, shotPos);
			objectManager.Begin(id);*/
		

			DebugLog("����");

			if (burstCnt_ >= shotData_.burstNum)
			{
				burstCnt_ = 0;
				shotTime_ = -shotData_.shotInterval;
			}
		}
	}
}

void EnemyBehavior::NonShot(ObjectManager& objectManager, float delta)
{
}

// �ǐՈړ���Ԃ̏���
void EnemyBehavior::UpdateMove(ObjectManager& objectManager, float delta)
{
	// ��]����
	Rotation(delta);

	// �ړ�
	//DebugLog("x=", animetor_->GetAnimState()->GetMovePosition().x,"y=", animetor_->GetAnimState()->GetMovePosition().y,"z=", animetor_->GetAnimState()->GetMovePosition().z);
	transform_->Pos() += transform_->GetForward() * (delta * speed_);

	if (!animetor_->GetAnimState()->IsBlend())
	{
		if ((transform_->GetPos() - playerT_->GetPos()).SqMagnitude() <= Square(stopDistance_))
		{
			// ���̋����ɂȂ������~��Ԃ�
			stateTime_ = 0.0f;
			update_ = &EnemyBehavior::UpdateStop;
			//laserSight_->On();
			animetor_->SetNextAnimation(5, 0.5f);
		}
		else if ((transform_->GetPos() - playerT_->GetPos()).SqMagnitude() >= Square(searchDistance_))
		{
			// ���̋����ȏ�ɂȂ�����ǐՑO�̏�Ԃ�
			stateTime_ = 0.0f;
			update_ = &EnemyBehavior::UpdateSearch;
			animetor_->SetNextAnimation(0, 0.5f);
		}
	}
}

// ��~��Ԃ̏���
void EnemyBehavior::UpdateStop(ObjectManager& objectManager, float delta)
{
	// ��]
	Rotation(delta);

	if (!animetor_->GetAnimState()->IsBlend())
	{
		// �ˌ�
		(this->*shot_)(objectManager, delta);
		//muzFlash_->Play();
		if ((transform_->GetPos() - playerT_->GetPos()).SqMagnitude() >= Square(stopDistance_))
		{
			// ���ȏ�̋����ɂȂ�����ړ���Ԃ�
			stateTime_ = 0.0f;
			update_ = &EnemyBehavior::UpdateMove;
			animetor_->SetNextAnimation(1, 0.5f);
			//laserSight_->Off();
		}
	}
}

// �U�����󂯂���̏��
void EnemyBehavior::UpdateHit(ObjectManager& objectManager, float delta)
{
	// ���ɉ�����
	transform_->Pos() += knockBackVec_ * (speed_)*delta;
	if (collider_->IsGround() && stateTime_ >= hitTime_)
	{
		// �d�����ԕ��o������
		stateTime_ = 0.0f;
		auto player = (objectManager.GetComponent<PlayerBehavior>(objectManager.GetPlayerID()));
		player->AttackEnd();
		//hitCombo_ = -1;
		update_ = &EnemyBehavior::UpdateMove;
		DebugLog("�d���I��");
		// 3��U�����󂯂������
		if (damageCnt_ >= HpMax)
		{
			DebugLog("�|���ꂽ");
			objectManager.GetComponent<ObjectInfo>(ownerId_)->Destory();
			
			//lpSooundPross.PlayBackSound(SOUNDNAME_SE::enemyDestroy, lpSooundPross.GetVolume(), false);
		}
	}
}

// ��]����
void EnemyBehavior::Rotation(float delta)
{
	if (isRot_)
	{
		// ��]�����������̏���
		transform_->AddRotation({ 0.0f, Deg2Rad(60.0f) * rotSing_ * delta, 0.0f });
	}


	auto targetVec{ playerT_->GetPos() - transform_->GetPos() };
	auto forward{ transform_->GetForward() };
	auto rot = Square(std::atan2(targetVec.z, targetVec.x) - std::atan2(forward.z, forward.x));
	if (rot > rotDeff)
	{
		// ��]���ׂ���
		isRot_ = true;
		rotSing_ = Cross(Vector2{ forward.z, forward.x }, Vector2{ targetVec.z,targetVec.x }) > 0.0f ? 1.0f : -1.0f;
	}
	else
	{
		isRot_ = false;
	}
}

// �d�͏���
void EnemyBehavior::Gravity(float delta)
{
	if (!collider_->IsGround())
	{
		// �󒆂ɂ��鎞
		jumpTime_ += delta;
		float val = (v0 * jumpTime_) - (0.5f * gravity * Square(jumpTime_));
		transform_->Pos() += transform_->GetUp() * (val);
		collider_->SetGravityPow(val);
	}
	else
	{
		jumpTime_ = 0.0f;
	}
}


// �q�b�g���̏���
void EnemyBehavior::OnHit(Collider& col, ObjectManager& objectManager)
{
	auto atr = objectManager.GetComponent<ObjectInfo>(col.GetOwnerID())->GetAttribute();
	if (atr == ObjectAttribute::PlayerAttack)
	{
		auto player = (objectManager.GetComponent<PlayerBehavior>(objectManager.GetPlayerID()));
		if (hitCombo_ == player->GetAtkCnt())
		{
			// �����U���Ȃ̂ŏ������Ȃ�
			return;
		}
		if (damageCnt_ >= HpMax)
		{
			// �������łɎ��S���Ă���̂ŏ������Ȃ�
			return;
		}

		// �U�����󂯂��Ƃ��̏���
		hitCombo_ = player->GetAtkCnt();
		damageCnt_++;
		//player->RiseSkillValue();
		player->AddCombo();

		auto hit = objectManager.GetComponent<Transform>(col.GetOwnerID());

		// �m�b�N�o�b�N������
		KnockBack(objectManager);
	}
	else if (atr == ObjectAttribute::PlayerShot)
	{
		if (damageCnt_ >= HpMax)
		{
			// �������łɎ��S���Ă���̂ŏ������Ȃ�
			return;
		}
		// �ꌂ���j
		damageCnt_ += HpMax;

		KnockBack(objectManager);
	}
}

void EnemyBehavior::KnockBack(ObjectManager& objectManager)
{
	update_ = &EnemyBehavior::UpdateHit;
	stateTime_ = 0.0f;

	jumpTime_ = 0.0f;
	collider_->SetGroundFlag(false);
	transform_->Pos().y += 2.0f;
	objectManager.StartHitStop(0.1f);
	auto playerTransform = (objectManager.GetComponent<Transform>(objectManager.GetPlayerID()));
	if (playerTransform.IsActive())
	{
		knockBackVec_ = (transform_->GetPos() - playerTransform->GetPos());
		knockBackVec_.y = 0.0f;
		knockBackVec_.Normalize();
	}
	animetor_->SetNextAnimation(0, 0.25f);
	//lpSooundPross.PlayBackSound(SOUNDNAME_SE::playerAttackHit, lpSooundPross.GetVolume(), false);
}
