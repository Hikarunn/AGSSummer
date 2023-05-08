#pragma once
#include <cstdint>

// �f�[�^�̎�ޕ���
enum class DataType
{
	Transform,		// ���W��]�g�k
	Collider,		// �����蔻��
	Model,			// ���f��
	Spawn,			// �X�|�[�����W
	Other			// ���̑�
};

// �f�[�^�t�@�C���̃w�b�_�[
struct ObjectDataHeader
{
	char sig[4];			// �V�O�l�`��
	unsigned int check;		// �`�F�b�N
	unsigned int version;	// �o�[�W�������
};

// �f�[�^�̃w�b�_�[
struct DataHeader
{
	DataType type;			// �^�C�v
	std::uint32_t size;		// �f�[�^�T�C�Y
};

// �����蔻��f�[�^���
enum class ColiderDataType
{
	Box,
	Capsule,
	Sphere,
	Mesh
};