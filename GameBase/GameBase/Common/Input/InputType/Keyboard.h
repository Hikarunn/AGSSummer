#pragma once
#include "../Controller.h"

// �L�[�{�[�h�p
class Keyboard :
	public Controller
{
public:
	Keyboard();
	~Keyboard();
	bool Init(void)override;										// ������
	void Update(float delta) override;								// �X�V Update(deltaTime)
	ControllerType GetControllerType(void) override { return ControllerType::Key; }		// �R���g���[���[�^�C�v�̎擾private:
	const std::array<char, 256>& GetKeyData(void) const&
	{
		return keyData_;
	}
private:

	/// <summary>
	/// �J�[�\���̍��W���Z�b�g����
	/// </summary>
	/// <param name="pos"> ���W </param>
	void SetCursorPos(const Vector2& pos = lpSceneManager.screenSize_<float> / 2.0f);

	// �L�[�f�[�^�i�[�p
	std::array<char, 256> keyData_ = {};
};

