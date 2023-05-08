#include <fstream>
#include <DxLib.h>
#include <vector>
#include "InputConfigManager.h"
#include "InputConfig.h"
#include "../../Debug.h"

constexpr char path[] = "Resource/Other/Input/";
InputConfig* InputConfig::sInstance_ = nullptr;

// PadType���L�[�ɍ��킹�ăR���t�B�O�t�@�C���̖��O�ƃf�t�H���g�ɖ߂��֐������e�[�u��
InputConfig::TypeDataTbl InputConfig::typeDataTbl_{
	{DX_PADTYPE_DUAL_SHOCK_4,std::make_pair("padConfigPS.data",&InputConfig::SetPsDefalutCode)},
	{DX_PADTYPE_DUAL_SENSE,std::make_pair("padConfigPS.data",&InputConfig::SetPsDefalutCode)},
	{DX_PADTYPE_XBOX_360, std::make_pair("padConfigXbox.data", &InputConfig::SetXboxDefalutCode)},
	{DX_PADTYPE_XBOX_ONE, std::make_pair("padConfigXbox.data", &InputConfig::SetXboxDefalutCode)},
	{-1, std::make_pair("KeyboardConfig.data", &InputConfig::SetKeyDefalutCode)}
};

// �T�E���h�ݒ�̃f�[�^�̃p�X
constexpr char soundFile[] = "Resource/Other/Input/Sound.data";

void InputConfig::Create(void)
{
	if (sInstance_ == nullptr)
	{
		sInstance_ = new InputConfig();
	}
}

void InputConfig::Destroy(void)
{
	if (sInstance_ != nullptr)
	{
		delete sInstance_;
	}
	sInstance_ = nullptr;
}

InputConfig& InputConfig::GetInstance(void)
{
	return *sInstance_;
}



void InputConfig::ChangeInputCode(ControllerInputID id, int code)
{
	// �ύX�O�̒l�������Ƃ�
	auto tmp = inputControllerCode_[id];
	inputControllerCode_[id] = code;
	auto itr = std::find_if(inputControllerCode_.begin(), inputControllerCode_.end(), [code](auto& data) { return data.second == code; });
	if (itr != inputControllerCode_.end())
	{
		// �ύX�O�̒l������
		itr->second = tmp;
	}
}

void InputConfig::SetDefalutCode(void)
{
	if (typeDataTbl_.contains(nowType_))
	{
		// ���݂̃^�C�v���e�[�u���ɑ��݂��鎞
		(this->*typeDataTbl_[nowType_].second)();
	}
	else
	{
		// ���Ȃ��Ƃ��f�t�H���g���Z�b�g����
		SetKeyDefalutCode();
	}
}

void InputConfig::SetPsDefalutCode(void)
{
	inputControllerCode_.emplace(ControllerInputID::Attack, 1);
	inputControllerCode_.emplace(ControllerInputID::Jump, 2);
	inputControllerCode_.emplace(ControllerInputID::MainTriger1, 0);
	inputControllerCode_.emplace(ControllerInputID::MainTriger2, 3);
	inputControllerCode_.emplace(ControllerInputID::MainTriger3, 0);
	inputControllerCode_.emplace(ControllerInputID::MainTriger4, 0);
	inputControllerCode_.emplace(ControllerInputID::SubTriger1, 1);
	inputControllerCode_.emplace(ControllerInputID::SubTriger2, 3);
	inputControllerCode_.emplace(ControllerInputID::SubTriger3, 2);
	inputControllerCode_.emplace(ControllerInputID::SubTriger4, 0);
	inputControllerCode_.emplace(ControllerInputID::LockOn, 1);
	inputControllerCode_.emplace(ControllerInputID::CameraFlont, 3);
	

}

void InputConfig::SetXboxDefalutCode(void)
{
	inputControllerCode_.emplace(ControllerInputID::Attack, 1);
	inputControllerCode_.emplace(ControllerInputID::Jump, 2);
	inputControllerCode_.emplace(ControllerInputID::MainTriger1, 0);
	inputControllerCode_.emplace(ControllerInputID::MainTriger2, 3);
	inputControllerCode_.emplace(ControllerInputID::MainTriger3, 0);
	inputControllerCode_.emplace(ControllerInputID::MainTriger4, 0);
	inputControllerCode_.emplace(ControllerInputID::SubTriger1, 1);
	inputControllerCode_.emplace(ControllerInputID::SubTriger2, 3);
	inputControllerCode_.emplace(ControllerInputID::SubTriger3, 2);
	inputControllerCode_.emplace(ControllerInputID::SubTriger4, 0);
	inputControllerCode_.emplace(ControllerInputID::LockOn, 1);
	inputControllerCode_.emplace(ControllerInputID::CameraFlont, 3);
	
}

void InputConfig::SetKeyDefalutCode(void)
{
	inputKeyCode_.emplace(KeyInputID::Attack, MOUSE_INPUT_LEFT);
	inputKeyCode_.emplace(KeyInputID::Jump, KEY_INPUT_SPACE);
	inputKeyCode_.emplace(KeyInputID::MainTrigerChange, MOUSE_INPUT_MIDDLE);
	inputKeyCode_.emplace(KeyInputID::SubTrigerAttack, KEY_INPUT_Q);
	inputKeyCode_.emplace(KeyInputID::SubTrigerChange1, KEY_INPUT_1);
	inputKeyCode_.emplace(KeyInputID::SubTrigerChange2, KEY_INPUT_2);
	inputKeyCode_.emplace(KeyInputID::SubTrigerChange3, KEY_INPUT_3);
	inputKeyCode_.emplace(KeyInputID::SubTrigerChange4, KEY_INPUT_4);
	inputKeyCode_.emplace(KeyInputID::LockOn, KEY_INPUT_LSHIFT);
	inputKeyCode_.emplace(KeyInputID::CameraFlont, MOUSE_INPUT_RIGHT);

}
//
//const InputConfig::PeConfig& InputConfig::GetPeConfig(void) const&
//{
//	return peConfig_;
//}
//
//void InputConfig::SetPeConfig(PEID id, bool flag)
//{
//	peConfig_[id] = flag;
//}

InputConfig::InputConfig()
{
	// Input�̏�����
	InitInput();

	// �T�E���h�̏�����
	InitSound();

	// �|�X�g�v���Z�X�̏�����
	/*peConfig_.reserve(2);
	peConfig_[PEID::Mono] = false;
	peConfig_[PEID::VolFog] = true;*/

	// �J�����X�s�[�h��������
	camSpeed_ = 0.5f;
}

void InputConfig::InitSound()
{
	// �T�E���h�ݒ�t�@�C����ǂݍ���
	std::ifstream file{ soundFile, std::ios::binary };

	// �Ƃ肠����1.0f�ŏ�����
	auto se = 1.0f;
	auto bg = 1.0f;
	if (file)
	{
		// �t�@�C����ǂݍ��߂���
		std::uint64_t ck;
		file.read(reinterpret_cast<char*>(&se), sizeof(se));
		file.read(reinterpret_cast<char*>(&bg), sizeof(bg));
		file.read(reinterpret_cast<char*>(&ck), sizeof(ck));

		if (std::hash<float>()(se + bg) != ck)
		{
			// �`�F�b�N�Œl����v���Ă��Ȃ��Ƃ��f�t�H���g�ɂ���
		/*	se = lpSooundPross.GetDefaultVolume();
			bg = lpSooundPross.GetDefaultVolume();*/
		}
	}

	// �Z�b�g����
	/*lpSooundPross.SetSEVolumeEntire(se);
	lpSooundPross.SetBGMVolumeEntire(bg);*/
}

void InputConfig::InitInput()
{
	nowType_ = GetJoypadType(DX_INPUT_PAD1);

	if (typeDataTbl_.contains(nowType_))
	{
		// ���݂̃^�C�v���e�[�u���ɑ��݂��鎞�ݒ�t�@�C�������[�h����
		if (!Load(typeDataTbl_[nowType_].first))
		{
			// ���[�h���s�����݂̃^�C�v�̃f�t�H���g�ɂ���֐������s����
			(this->*typeDataTbl_[nowType_].second)();
		}
	}
	else
	{
		// �Ȃ��Ƃ��L�[�{�[�h�̕��Ńf�t�H���g�ɂ���
		nowType_ = -1;
		SetKeyDefalutCode();
	}
}

InputConfig::~InputConfig()
{
	if (typeDataTbl_.contains(nowType_))
	{
		Save(typeDataTbl_[nowType_].first);
	}

	std::ofstream file{ soundFile, std::ios::binary };
	if (file)
	{
		/*auto ck = std::hash<float>()(lpSooundPross.GetSEVolumeEntire() + lpSooundPross.GetBGMVolumeEntire());
		auto se = lpSooundPross.GetSEVolumeEntire();
		auto bg = lpSooundPross.GetBGMVolumeEntire();
		file.write(reinterpret_cast<char*>(&se), sizeof(se));
		file.write(reinterpret_cast<char*>(&bg), sizeof(bg));
		file.write(reinterpret_cast<char*>(&ck), sizeof(ck));*/
	}
}

bool InputConfig::Load(const std::string& fname)
{
	std::ifstream ifs(path + fname);
	if (!ifs)
	{
		return false;
	}
	Header h{};
	std::vector<Data> vec;
	// �w�b�_�[����ǂݍ���
	ifs.read(reinterpret_cast<char*>(&h), sizeof(h));
	// �ݒ�f�[�^��ǂݍ���
	vec.resize(h.size);
	ifs.read(reinterpret_cast<char*>(vec.data()), sizeof(vec[0]) * h.size);
	int sum = 0;
	// �ȉ�sum�l�`�F�b�N
	for (auto& v : vec)
	{
		sum += static_cast<int>(v.id) * v.code;
	}
	if (sum != h.sum)
	{
		return false;
	}
	// �}�b�v�ɓ����
	for (auto& v : vec)
	{
		inputControllerCode_.emplace(v.id, v.code);
	}
	return true;
}

bool InputConfig::Save(const std::string& fname)
{
	std::ofstream ofs(path + fname);
	if (!ofs)
	{
		return false;
	}
	Header h{};
	std::vector<Data> vec;
	//std::vector<Data> vec(inputCode_.size());
	vec.reserve(inputControllerCode_.size());
	for (auto& code : inputControllerCode_)
	{
		vec.emplace_back(Data{ code.first, code.second });
	}

	// �w�b�_�������
	h.size = vec.size();
	h.ver = 1.0f;
	for (auto& v : vec)
	{
		h.sum += static_cast<int>(v.id) * v.code;
	}
	// ��������
	ofs.write(reinterpret_cast<char*>(&h), sizeof(h));
	ofs.write(reinterpret_cast<char*>(vec.data()), sizeof(vec[0]) * vec.size());
	return true;
}
