#pragma once

// �X�N���[����ID 
enum class SceneID
{
	Title,			// �^�C�g�� 
	Select,			// �I���V�[��
	Game,			// �v���C��
	Result,			// ���U���g
	Pause,			// �|�[�Y���
	Option,			// �I�v�V����
	Transition,		// �J��
	Loading,		// ���[�h�V�[��
	InputSet,		// �L�[�̐ݒ�V�[��
	Dialog,			// �I�������̉ۊm�F
	Tutorial,		// �`���[�g���A��
}; 
enum class Result
{
	Clear,
	GameOver,
	Max,
};