//───────────────────────────────────────
// テクスチャ＆サンプラーデータのグローバル変数定義
//───────────────────────────────────────
Texture2D g_texture : register(t0);       // テクスチャ
SamplerState g_sampler : register(s0);    // サンプラー
Texture2D g_toon_texture : register(t1);  // トゥーンシェーダー用のテクスチャ

//───────────────────────────────────────
// コンスタントバッファ
// DirectX 側から送信されてくる、ポリゴン頂点以外の諸情報の定義
//───────────────────────────────────────
cbuffer gmodel : register(b0)
{
    float4x4 matWVP;        // ワールド・ビュー・プロジェクションの合成行列
    float4x4 matW;          // ワールド行列
    float4x4 matNormal;     // ワールド法線行列
    float4 diffuseColor;    // マテリアルの色＝拡散反射係数
    float4 ambientColor;    // 環境光
    float4 specularColor;   // 鏡面反射＝ハイライト
    float shininess;
    bool isTextured;        // テクスチャが貼られているかどうか
};

cbuffer gmodel_light : register(b1)
{
    float4 lightPosition;
    float4 eyePosition;
};

//───────────────────────────────────────
// 頂点シェーダー出力＆ピクセルシェーダー入力データ構造体
//───────────────────────────────────────
struct VS_OUT
{
    float4 pos : SV_POSITION;   // 位置
    float2 uv : TEXCOORD;       // UV座標
    float4 color : COLOR;       // 色（明るさ）
    float4 eyev : POSITION;     // 視線ベクトル
    float4 normal : NORMAL;     // 法線ベクトル
};

//───────────────────────────────────────
// 頂点シェーダ
//───────────────────────────────────────
VS_OUT VS(float4 pos : POSITION, float4 uv : TEXCOORD, float4 normal : NORMAL)
{
    // ピクセルシェーダーへ渡す情報
    VS_OUT outData = (VS_OUT)0;

    // ローカル座標に、ワールド・ビュー・プロジェクション行列をかけて
    // スクリーン座標に変換し、ピクセルシェーダーへ
    outData.pos = mul(pos, matWVP);
    outData.uv = uv;
    normal.w = 0;
    normal = mul(normal, matNormal);
    normal = normalize(normal);
    outData.normal = normal;

    float4 light = normalize(lightPosition);
    outData.color = saturate(dot(normal, light));
    float4 posw = mul(pos, matW);
    outData.eyev = eyePosition - posw;

    // アウトライン用のオフセットを加える
    float outlineOffset = 0.02; // アウトラインの太さ調整
    float4 outlinePos = pos + normal * outlineOffset;

    // アウトライン用の頂点情報を出力
    VS_OUT outlineData = (VS_OUT)0;
    outlineData.pos = mul(outlinePos, matWVP);
    outlineData.uv = uv;
    outlineData.normal = normal;

    // まとめて出力
    return outData;
}

//───────────────────────────────────────
// ピクセルシェーダ
//───────────────────────────────────────
float4 PS(VS_OUT inData) : SV_Target
{
    float4 lightSource = float4(1.0, 1.0, 1.0, 1.0);
    float4 diffuse;
    float4 ambient;

    float4 reflection = reflect(normalize(-lightPosition), inData.normal);
    float4 specular = pow(saturate(dot(reflection, normalize(inData.eyev))), shininess) * specularColor;

    float2 uv;

    uv.x = inData.color.x;
    uv.y = 0;

    float4 tI = g_toon_texture.Sample(g_sampler, uv);

    if (!isTextured)
    {
        diffuse = lightSource * diffuseColor * tI;
        ambient = lightSource * ambientColor * tI;
    }
    else
    {
        diffuse = lightSource * g_texture.Sample(g_sampler, inData.uv) * tI;
        ambient = lightSource * g_texture.Sample(g_sampler, inData.uv) * ambientColor;
    }

    // アウトラインの色を指定
    float4 outlineColor = float4(1.0, 1.0, 1.0, 1.0); // アウトラインカラー

    // アウトライン用のピクセルシェーダー
    float4 outlinePixel = outlineColor;

    // オリジナルのピクセルシェーダー処理
    float4 originalPixel = diffuse + ambient + specular;

    // アウトラインの描画条件を設定
    bool isOutline = dot(normalize(inData.eyev), normalize(inData.normal)) < 0.3; // 例: 視線と法線が一致する場合にアウトラインを描画

    // アウトラインの描画
    if (isOutline)
    {
        return outlinePixel;
    }
    else
    {
        return originalPixel;
    }
}
