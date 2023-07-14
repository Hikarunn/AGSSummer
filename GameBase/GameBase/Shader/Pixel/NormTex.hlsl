struct PSInput {
    float4 pos:POSITION0;
    float4 lpos:POSITION1;
    float3 norm:NORMAL0;
    float3 cnorm:NORMAL1;
    float2 uv:TECOORD;
    float4 diff:COLOR0;
    float4 spec:COLOR1;
    float3 tan:TANGENT;
    float3 bin:BINORMAL;
    float4 svpos : SV_POSITION;
};

// ピクセルシェーダーの出力
struct PSOutput
{
    float4 color0 : SV_TARGET0; // 色
};

// マテリアルパラメータ
struct CONST_MATERIAL
{
    float4 diffuse; // ディフューズカラー
    float4 specular; // スペキュラカラー
    float4 ambient_Emissive; // マテリアルエミッシブカラー + マテリアルアンビエントカラー * グローバルアンビエントカラー

    float power; // スペキュラの強さ
    float typeParam0; // マテリアルタイプパラメータ0
    float typeParam1; // マテリアルタイプパラメータ1
    float typeParam2; // マテリアルタイプパラメータ2
};

// フォグパラメータ
struct CONST_FOG
{
    float linearAdd; // フォグ用パラメータ end / ( end - start )
    float linearDiv; // フォグ用パラメータ -1  / ( end - start )
    float density; // フォグ用パラメータ density
    float e; // フォグ用パラメータ 自然対数の低

    float4 color; // カラー
};

// ライトパラメータ
struct CONST_LIGHT
{
    int type; // ライトタイプ( DX_LIGHTTYPE_POINT など )
    int3 padding1; // パディング１

    float3 position; // 座標( ビュー空間 )
    float rangePow2; // 有効距離の２乗

    float3 direction; // 方向( ビュー空間 )
    float fallOff; // スポットライト用FallOff

    float3 diffuse; // ディフューズカラー
    float spotParam0; // スポットライト用パラメータ０( cos( Phi / 2.0f ) )

    float3 specular; // スペキュラカラー
    float spotParam1; // スポットライト用パラメータ１( 1.0f / ( cos( Theta / 2.0f ) - cos( Phi / 2.0f ) ) )

    float4 ambient; // アンビエントカラーとマテリアルのアンビエントカラーを乗算したもの

    float attenuation0; // 距離による減衰処理用パラメータ０
    float attenuation1; // 距離による減衰処理用パラメータ１
    float attenuation2; // 距離による減衰処理用パラメータ２
    float padding2; // パディング２
};

// ピクセルシェーダー・頂点シェーダー共通パラメータ
struct CONST_BUFFER_COMMON
{
    CONST_LIGHT light[6]; // ライトパラメータ
    CONST_MATERIAL material; // マテリアルパラメータ
    CONST_FOG fog; // フォグパラメータ
};

// 定数バッファピクセルシェーダー基本パラメータ
struct CONST_BUFFER_BASE
{
    float4 factorColor; // アルファ値等

    float mulAlphaColor; // カラーにアルファ値を乗算するかどうか( 0.0f:乗算しない  1.0f:乗算する )
    float alphaTestRef; // アルファテストで使用する比較値
    float2 padding1;

    int alphaTestCmpMode; // アルファテスト比較モード( DX_CMP_NEVER など )
    int3 padding2;

    float4 ignoreTextureColor; // テクスチャカラー無視処理用カラー
};

// 頂点シェーダー・ピクセルシェーダー共通パラメータ
cbuffer cbD3D11_CONST_BUFFER_COMMON : register(b0)
{
    CONST_BUFFER_COMMON g_Common;
};

// 基本パラメータ
cbuffer cbD3D11_CONST_BUFFER_PS_BASE : register(b1)
{
    CONST_BUFFER_BASE g_Base;
};

SamplerState texsam : register(s0);
SamplerState normsam : register(s1);
SamplerState specsam : register(s2);
SamplerState depthsam : register(s3);
SamplerComparisonState depthsmp : register(s3);
//SamplerComparisonState depthsmp  		// 深度バッファテクスチャ
//{
//	// sampler state
//    Filter = COMPARISON_MIN_MAG_MIP_LINEAR;
//    MaxAnisotropy = 1;
//    AddressU = MIRROR;
//    AddressV = MIRROR;

//	// sampler conmparison state
//    ComparisonFunc = GREATER;
//};

Texture2D<float4> tex : register(t0);
Texture2D<float4> norm : register(t1);
Texture2D<float4> spec : register(t2);
Texture2D<float4> depthtex : register(t3);


PSOutput main(PSInput input) : SV_TARGET
{
    PSOutput output;
// 接線・従法線・法線を正規化
float3 VNrm = normalize(input.norm);
float3 VTan = normalize(input.tan);
float3 VBin = normalize(input.bin);

// 頂点座標から視点へのベクトルを接底空間に投影した後正規化して保存
float3 TempF3;
TempF3.x = dot(VTan, -input.pos.xyz);
TempF3.y = dot(VBin, -input.pos.xyz);
TempF3.z = dot(VNrm, -input.pos.xyz);
float3 V_to_Eye = normalize(TempF3);
// 法線の0～1の値を-1.0f～1.0fに変換する
float3 Normal = (norm.Sample(normsam, input.uv.xy).xyz - float3(0.5f, 0.5f, 0.5f)) * 2.0f;
// ディフューズカラーとスペキュラカラーの蓄積値を初期化
float4 TotalDiffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
float4 TotalSpecular = float4(0.0f, 0.0f, 0.0f, 0.0f);

// ライト方向ベクトルの計算
TempF3 = g_Common.light[0].direction;
// ライトのベクトルを接地空間に変換
float3 lLightDir;
lLightDir.x = dot(VTan, TempF3);
lLightDir.y = dot(VBin, TempF3);
lLightDir.z = dot(VNrm, TempF3);
lLightDir = normalize(lLightDir);
// ディフューズ角度減衰率計算
float DiffuseAngleGen = saturate(dot(Normal, -lLightDir));
// ディフューズカラーに蓄積する
TotalDiffuse.xyz += (g_Common.light[0].diffuse * 2) * g_Common.material.diffuse.xyz * DiffuseAngleGen + g_Common.light[0].ambient.xyz;
// スペキュラカラー計算とハーフベクトルの計算
TempF3 = normalize(V_to_Eye - lLightDir);
float4 Temp = pow(max(0.0f, dot(Normal, TempF3)), g_Common.material.power);
float3 TextureSpecular = spec.Sample(specsam, input.uv.xy);
TotalSpecular.xyz += Temp.xyz * g_Common.light[0].specular * TextureSpecular;

TotalDiffuse += g_Common.material.ambient_Emissive;
float3 SpecularColor = TotalSpecular.xyz * g_Common.material.specular.xyz;

float3 dirLig = TotalDiffuse.xyz + SpecularColor;

// リムライトの計算
// 法線と光の入射方向に依存するリムの強さを求める
float power1 = 1.0f - max(0.0f, dot(g_Common.light[0].direction, input.norm));
// 法線と視線の方向に依存するリムの強さを求める
float power2 = 1.0f - max(0.0f, input.cnorm.z * -1.0f);
// 最終的なリムの強さを求める
float limPower = power1 * power2;
// pow()を使用して、強さの変化を指数関数的にする
limPower = pow(limPower, 0.8f);
// dirLigにリムライトの反射光を合算する
// まず、リムライトのカラーを計算する
float3 limColor = limPower * g_Common.light[0].ambient;
// 最終的な反射光にリムの反射光を合算する
dirLig += limColor;


float4 TextureDiffuseColor = tex.Sample(texsam, input.uv);
// アルファ値以外のカラーを出力
output.color0.rgb = (TextureDiffuseColor.xyz * g_Common.material.diffuse.xyz) * dirLig;
// アルファ値の出力
output.color0.a = TextureDiffuseColor.a * g_Common.material.diffuse.a * g_Base.factorColor.a;

float2 shadowUV;
// 深度テクスチャの座標を算出
shadowUV.x = (input.lpos.x + 1.0f) * 0.5f;
// yは上下反転しないといけない
shadowUV.y = 1.0f - (input.lpos.y + 1.0f) * 0.5f;
// マッハバンドを起こさないようにするため
input.lpos.z -= 0.005f;

// 周囲のデータと深度テクスチャの深度を取得
float comp = 0;
float U = 1.0f / 2048;
float V = 1.0f / 2048;
comp += saturate(max(input.lpos.z - depthtex.Sample(depthsam, shadowUV + float2(0, 0)).r, 0.0f) * 1500 - 0.5f);
comp += saturate(max(input.lpos.z - depthtex.Sample(depthsam, shadowUV + float2(U, 0)).r, 0.0f) * 1500 - 0.5f);
comp += saturate(max(input.lpos.z - depthtex.Sample(depthsam, shadowUV + float2(-U, 0)).r, 0.0f) * 1500 - 0.5f);
comp += saturate(max(input.lpos.z - depthtex.Sample(depthsam, shadowUV + float2(0, V)).r, 0.0f) * 1500 - 0.5f);
comp += saturate(max(input.lpos.z - depthtex.Sample(depthsam, shadowUV + float2(0, -V)).r, 0.0f) * 1500 - 0.5f);
comp += saturate(max(input.lpos.z - depthtex.Sample(depthsam, shadowUV + float2(U, V)).r, 0.0f) * 1500 - 0.5f);
comp += saturate(max(input.lpos.z - depthtex.Sample(depthsam, shadowUV + float2(-U, V)).r, 0.0f) * 1500 - 0.5f);
comp += saturate(max(input.lpos.z - depthtex.Sample(depthsam, shadowUV + float2(U, -V)).r, 0.0f) * 1500 - 0.5f);
comp += saturate(max(input.lpos.z - depthtex.Sample(depthsam, shadowUV + float2(-U, -V)).r, 0.0f) * 1500 - 0.5f);

// 出したものの平均を取得
comp = 1 - saturate(comp / 9);

// そのまま入れると黒が強いので、少しだけ薄める
output.color0.xyz *= comp / 2.0f + 0.2;

return output;
}