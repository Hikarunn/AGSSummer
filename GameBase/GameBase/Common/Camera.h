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
	/// �J�����̃Z�b�g�A�b�v(�`��p)
	/// </summary>
	/// <param name=""></param>
	void SetScreen(void)const;

	/// <summary>
	/// �J�����̏�������擾����
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	const Vector3 GetUp(void)const;

	/// <summary>
	/// �J�����O�������擾����
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	const Vector3 GetForward(void)const;

	/// <summary>
	/// �J�����̍��W���Z�b�g
	/// </summary>
	/// <param name="pos"></param>
	void SetPos(const Vector3& pos)
	{
		pos_ = pos;
	}

	/// <summary>
	/// �V���h�E�}�b�v�`��p�̃J����
	/// </summary>
	/// <param name="size"></param>
	/// <param name="ner"></param>
	/// <param name="fur"></param>
	/// <param name="target"></param>
	void SetUpShadow(float size, float ner, float fur, const Vector3& target)const;

	/// <summary>
	/// ��]���N�H�[�^�j�I���ŃZ�b�g����
	/// </summary>
	/// <param name="q"></param>
	void SetRotation(const Quaternion& q);

private:

	// �J�������W
	Vector3 pos_;

	// �J�����̉�]
	Quaternion quaRot_;

};

