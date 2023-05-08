#pragma once
#include <map>
#include <array>
#include <string>
#include "InputID.h"
#include "../Vector2.h"
#include "../../SceneManager.h"

// �g���K�[���
enum class Triger
{
	Now,			// ����
	Old,			// �ЂƂO
	Mac			
};

// ���͑��u�̎��
enum class ControllerType
{
	Key,			// �L�[�{�[�h
	Pad,			// �p�b�h
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
	/// ������
	/// </summary>
	/// <param name=""></param>
	/// <returns>��������true�A���s����false</returns>
	virtual bool Init(void) = 0;

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="delta">�f���^�^�C��</param>
	virtual void Update(float delta) = 0;

	/// <summary>
	/// �R���g���[���[�̎�ގ擾
	/// </summary>
	/// <param name=""></param>
	/// <returns>�R���g���[���[�̎��</returns>
	virtual ControllerType GetControllerType(void) = 0;

	/// <summary>
	/// ���N���b�N���Ă��邩
	/// </summary>
	/// <param name="id">�L�[�̎��</param>
	/// <returns>���ہ��@true,false</returns>
	bool MousePress(ControllerInputID id);

	/// <summary>
	/// �����Ă��邩
	/// </summary>
	/// <param name="id">�L�[�̎��</param>
	/// <returns></returns>
	bool Press(ControllerInputID id);

	/// <summary>
	/// �������u��
	/// </summary>
	/// <param name="id">�L�[�̎��</param>
	/// <returns></returns>
	bool Pressed(ControllerInputID id);

	/// <summary>
	/// �������u��
	/// </summary>
	/// <param name="id">�L�[�̎��</param>
	/// <returns></returns>
	bool Released(ControllerInputID id);

	/// <summary>
	/// ������Ă��Ȃ��Ƃ�
	/// </summary>
	/// <param name="id"></param>
	/// <returns></returns>
	bool NotPress(ControllerInputID id);

	/// <summary>
	/// ����������Ă��Ȃ����ǂ���
	/// </summary>
	/// <returns></returns>
	bool IsNeutral();

	/// <summary>
	/// ���������ꂽ��
	/// </summary>
	/// <returns></returns>
	bool IsAnyPress();

	/// <summary>
	/// �J�[�\���̈ʒu�Z�b�g
	/// </summary>
	/// <param name="pos"></param>
	virtual void SetQursorPos(const Vector2& pos = lpSceneManager.screenSize_<float> / 2.0f) = 0;

	/// <summary>
	/// �J�[�\���ʒu�̎擾
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	const Vector2& GetCursorPos(void)const&
	{
		return cursorPos_;
	}

	/// <summary>
	/// �J�[�\���̃X�s�[�h�Z�b�g
	/// </summary>
	/// <param name="speed"></param>
	void SetCursorSpeed(float speed)
	{
		cursorSpeed_ = speed;
	}

	/// <summary>
	/// �J�[�\���̃X�s�[�h���擾
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	const float GetCursorSpeed(void) const
	{
		return cursorSpeed_;
	}

	/// <summary>
	/// �ړ��p
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	const Vector2& GetLeftInput(void) const&
	{
		return leftInput_;
	}

	/// <summary>
	/// ���_����p
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	const Vector2& GetRightInput(void) const&
	{
		return rightInput_;
	}

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="id">�L�[�̎��</param>
	/// <param name="limit">�ő厞��</param>
	/// <param name="delta">�f���^�^�C��</param>
	/// <returns></returns>
	std::string LongPress(ControllerInputID id, double limit, double delta);

	/// <summary>
	/// �C���v�b�g�f�[�^���擾
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	const InputData& Get(void);

	/// <summary>
	/// ����{�^����������Ă��邩�ǂ���
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	bool PressDecision(void) const;

	/// <summary>
	/// ����{�^���������ꂽ�u�Ԃ��ǂ���
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	bool PressedDecision(void) const;

	/// <summary>
	/// �L�����Z���{�^����������Ă��邩�ǂ���
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	bool PressCancel(void) const;

	/// <summary>
	/// �L�����Z���{�^���������ꂽ�u�Ԃ��ǂ���
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	bool PressdCancel(void) const;

protected:
	//CntData cntData_;
	InputData data_;

	// ���艺���̏��̏��
	std::pair<bool, bool> decisionData_;

	// �L�����Z�����������̃f�[�^
	std::pair<bool, bool> cancelData_;

	// �J�[�\���̍��W
	Vector2 cursorPos_;

	// �J�[�\���̃X�s�[�h(ui���Ɏg�p)
	float cursorSpeed_;

	// �ړ��p�̓��͂��ꂽ����
	Vector2 leftInput_;

	// �U������p���͍��W
	Vector2 rightInput_;

	/// <summary> LongPress�֌W </summary>
	bool isPress_;
	bool isFinish_;
	float PressCount_;
};

