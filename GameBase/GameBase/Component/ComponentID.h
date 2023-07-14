#pragma once


enum class ComponentID :
	unsigned long long
{
	Base = 1ull,									// ���ׂĂ̊��
	Info = Base | (1ull << 1),						// �I�u�W�F�N�g�̏��
	Transform = Base | (1ull << 2),				// �g�����X�t�H�[��
	RenderBase = Base | (1ull << 3),				// �`��̊��
	Animator = Base | (1ull << 4),					// �A�j���[�V����
	Collider = Base | (1ull << 5),					// �����蔻��
	BehaviorBase = Base | (1ull << 6),				// �I�u�W�F�N�g�̓���
	ModelRender = RenderBase | (1ull << 7),		// model�̕`��
//	SoundSpeaker = Base | (1ull << 8),				// �T�E���h�̍Đ�
	CapsuleRender = RenderBase | (1ull << 8),		// �J�v�Z���̕`��
	PlayerBehavior = BehaviorBase | (1ull << 9),	// �v���C���[�̓���
	MeshCollider = Collider | (1ull << 10),		// ���b�V���ł̓����蔻��
	SphereCollider = Collider | (1ull << 11),		// ���̂ł̓����蔻��
	CapsuleCollider = Collider | (1ull << 12),		// �J�v�Z���ł̓����蔻��
	CharactorCollider = Collider | (1ull << 13),	// �L�����N�^�[�̓����蔻��
	TerrainCollider = Collider | (1ull << 14),		// �n�`�̓����蔻��
	EnemyBehavior = BehaviorBase | (1ull << 15),	// �G�̓���
	EnemyBulletBehavior = BehaviorBase | (1ull << 16),		// �G�̒e�̓���
	StageBehavior = BehaviorBase | (1ull << 17),			// �X�e�[�W�̐���
	PlayerShotBehavior = BehaviorBase | (1ull << 18),	// �v���C���[�̍U��
	PlayerAttackBehavior = BehaviorBase | (1ull << 19),	// �v���C���[�̍U��
	TutorialBehavior = BehaviorBase | (1ull << 20),		// �`���[�g���A������p
	BillBoardRender = RenderBase | (1ull << 21),			// �r���{�[�h�`��p
	CameraBehavior = BehaviorBase | (1ull << 22),			// �J����
	SkyBehavior = BehaviorBase | (1ull << 23),				// �X�J�C�h�[���̐���p
	LineRender = RenderBase | (1ull << 24),					// ���̕`��p
	EnemyLaserSightBehavior = BehaviorBase | (1ull << 25),		// �G�̍U�����ɏo�鎋��
	reserve1 = 1ull << 26,
	reserve2 = 1ull << 27,
	reserve3 = 1ull << 28,
	reserve4 = 1ull << 29,
	Max = ~Base
};
