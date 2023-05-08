using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ObjectDataPacker
{
	/// <summary>
	/// 3次元ベクトル
	/// </summary>
	public struct Vector3
	{
		public float x;
		public float y;
		public float z;
		public Vector3()
		{
			x = 0;
			y = 0;
			z = 0;
		}

		public Vector3 (float inX, float inY, float inZ)
		{
			x = inX;
			y = inY;
			z = inZ;
		}

		public Vector3 (BinaryReader br)
        {
			x = br.ReadSingle();
			y = br.ReadSingle();
			z = br.ReadSingle();
        }

		public void Write(BinaryWriter bw)
		{
			bw.Write(x);
			bw.Write(y);
			bw.Write(z);
		}

        public override string ToString()
        {
            return "x=" + x + "y=" + y + "z=" + z ;
        }

    }
}
