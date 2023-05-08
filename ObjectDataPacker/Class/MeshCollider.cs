using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ObjectDataPacker
{
	internal class MeshCollider : ColliderBase
	{
		private string path_;

		public MeshCollider(string path) : base(ColliderBase.ColType.Mesh, path)
		{
			path_ = path;
		}

		public MeshCollider(ColType type, BinaryReader br) : base(type,br)
		{
		}

		public override void Export(BinaryWriter bw)
		{
			// タイプを出力
			bw.Write((int)IExport.Type.Collider);

			// サイズを出力する
			bw.Write(8 + path_.Length);

			// タイプを出力
			bw.Write((int)ColType.Mesh);

			// 文字列の長さを出力
			bw.Write(path_.Length);

			bw.Write(path_.ToArray());

		}

		public override string GetInfo()
		{
			return "[MeshCollider]" + path_;
		}

		protected override void Load(string path)
		{
			// 何もしない
		}

		protected override void Load(BinaryReader br)
		{
			var length = br.ReadInt32();
			var str = new String(br.ReadChars(length));
			if (str == null)
			{
				path_ = "";
			}
			else
			{
				path_ = str;
			}
		}
	}
}
