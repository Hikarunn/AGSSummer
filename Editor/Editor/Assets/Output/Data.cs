using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;
using System.IO;

public class MenuExport
{
	[MenuItem("�f�[�^�o��/Transform�o��", priority = 1)]
	static public void TransformExport()
	{
		// �p�X���擾
		var path = EditorUtility.SaveFilePanel("Transform�o��", ".", "transform.tdat", "tdat");

		if (path == "")
		{
			// �I������ĂȂ�������_�C�A���O�o��
			EditorUtility.DisplayDialog("Transform�o��", "�L�����Z������܂���", "Ok");
			return;
		}

		// �t�@�C�����J����
		var stream = File.Create(path);
		var bw = new BinaryWriter(stream);

		var sig = new char[4];
		sig[0] = 't';
		sig[1] = 'd';
		sig[2] = 'a';
		sig[3] = 't';

		// �V�O�l�`�������
		bw.Write(sig);

		// �I�𒆂̃I�u�W�F�N�g���Z�b�g
		var gameObject = Selection.activeGameObject;
		if (gameObject == null)
		{
			EditorUtility.DisplayDialog("Transform�o��", "�I�u�W�F�N�g���I������Ă܂���", "Ok");
			return;
		}

		VectorExport(bw, gameObject.transform.position * 100.0f);
		VectorExport(bw, gameObject.transform.rotation.eulerAngles);
		VectorExport(bw, gameObject.transform.localScale);

		bw.Close();
		stream.Close();
		EditorUtility.DisplayDialog("Transform�o��", "�o�͂���܂���", "Ok");
	}

	enum ColType
	{
		Box,
		Capsule,
		Sphere,
		Mesh
	};

	[MenuItem("�f�[�^�o��/Collider�o��", priority = 2)]
	static public void ColliderExport()
	{
		var path = EditorUtility.SaveFilePanel("Collider�o��", ".", "collider.cdat", "cdat");

		if (path == "")
		{
			EditorUtility.DisplayDialog("Collider�o��", "�L�����Z������܂���", "Ok");
			return;
		}

		var gameObject = Selection.activeGameObject;
		if (gameObject == null)
		{
			EditorUtility.DisplayDialog("Collider�o��", "�Q�[���I�u�W�F�N�g���I������Ă��܂���", "Ok");
			return;
		}

		if (gameObject.GetComponent<Collider>() == null)
		{
			EditorUtility.DisplayDialog("Collider�o��", "Collider�R���|�[�l���g������܂���", "Ok");
			return;
		}

		var stream = File.Create(path);
		var bw = new BinaryWriter(stream);

		var sig = new char[4];
		sig[0] = 'c';
		sig[1] = 'd';
		sig[2] = 'a';
		sig[3] = 't';

		// �V�O�l�`�������
		bw.Write(sig);

		ColType type;

		// �e�R���C�_�[�ɍ��킹�ďo�͂���
		if (gameObject.GetComponent<BoxCollider>() != null)
		{
			var collider = gameObject.GetComponent<BoxCollider>();
			type = ColType.Box;
			bw.Write((int)type);
			VectorExport(bw, collider.center * 50.0f);
			VectorExport(bw, collider.size * 50.0f);
		}
		else if (gameObject.GetComponent<CapsuleCollider>() != null)
		{
			var collider = gameObject.GetComponent<CapsuleCollider>();
			type = ColType.Capsule;
			bw.Write((int)type);
			VectorExport(bw, collider.center * 50.0f);
			float r = collider.radius;
			float h = collider.height;
			h -= r * 2.0f;
			h *= 50.0f;
			r *= 50.0f;
			bw.Write(h);
			bw.Write(r);
		}
		else if (gameObject.GetComponent<SphereCollider>() != null)
		{
			var collider = gameObject.GetComponent<SphereCollider>();
			type = ColType.Sphere;
			bw.Write((int)type);
			VectorExport(bw, collider.center * 50.0f);
			bw.Write(collider.radius * 50.0f);
		}

		bw.Close();

		EditorUtility.DisplayDialog("Collider�o��", "�o�͂���܂���", "Ok");
	}

	static private void VectorExport(BinaryWriter bw, in Vector3 vector)
	{
		bw.Write(vector.x);
		bw.Write(vector.y);
		bw.Write(vector.z);
	}
}