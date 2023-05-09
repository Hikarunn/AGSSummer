using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;
using System.IO;

public class MenuExport
{
	[MenuItem("データ出力/Transform出力", priority = 1)]
	static public void TransformExport()
	{
		// パスを取得
		var path = EditorUtility.SaveFilePanel("Transform出力", ".", "transform.tdat", "tdat");

		if (path == "")
		{
			// 選択されてなかったらダイアログ出す
			EditorUtility.DisplayDialog("Transform出力", "キャンセルされました", "Ok");
			return;
		}

		// ファイルを開ける
		var stream = File.Create(path);
		var bw = new BinaryWriter(stream);

		var sig = new char[4];
		sig[0] = 't';
		sig[1] = 'd';
		sig[2] = 'a';
		sig[3] = 't';

		// シグネチャ入れる
		bw.Write(sig);

		// 選択中のオブジェクトをセット
		var gameObject = Selection.activeGameObject;
		if (gameObject == null)
		{
			EditorUtility.DisplayDialog("Transform出力", "オブジェクトが選択されてません", "Ok");
			return;
		}

		VectorExport(bw, gameObject.transform.position * 100.0f);
		VectorExport(bw, gameObject.transform.rotation.eulerAngles);
		VectorExport(bw, gameObject.transform.localScale);

		bw.Close();
		stream.Close();
		EditorUtility.DisplayDialog("Transform出力", "出力されました", "Ok");
	}

	enum ColType
	{
		Box,
		Capsule,
		Sphere,
		Mesh
	};

	[MenuItem("データ出力/Collider出力", priority = 2)]
	static public void ColliderExport()
	{
		var path = EditorUtility.SaveFilePanel("Collider出力", ".", "collider.cdat", "cdat");

		if (path == "")
		{
			EditorUtility.DisplayDialog("Collider出力", "キャンセルされました", "Ok");
			return;
		}

		var gameObject = Selection.activeGameObject;
		if (gameObject == null)
		{
			EditorUtility.DisplayDialog("Collider出力", "ゲームオブジェクトが選択されていません", "Ok");
			return;
		}

		if (gameObject.GetComponent<Collider>() == null)
		{
			EditorUtility.DisplayDialog("Collider出力", "Colliderコンポーネントがありません", "Ok");
			return;
		}

		var stream = File.Create(path);
		var bw = new BinaryWriter(stream);

		var sig = new char[4];
		sig[0] = 'c';
		sig[1] = 'd';
		sig[2] = 'a';
		sig[3] = 't';

		// シグネチャ入れる
		bw.Write(sig);

		ColType type;

		// 各コライダーに合わせて出力する
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

		EditorUtility.DisplayDialog("Collider出力", "出力されました", "Ok");
	}

	static private void VectorExport(BinaryWriter bw, in Vector3 vector)
	{
		bw.Write(vector.x);
		bw.Write(vector.y);
		bw.Write(vector.z);
	}
}