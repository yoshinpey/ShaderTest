////───────────────────────────────────────
//// テクスチャ＆サンプラーデータのグローバル変数定義
////───────────────────────────────────────
//Texture2D g_texture; // テクスチャ
//SamplerState g_sampler : register(s0); // サンプラー

////───────────────────────────────────────
//// コンスタントバッファ
//// DirectX 側から送信されてくる、ポリゴン頂点以外の諸情報の定義
////───────────────────────────────────────
//cbuffer gmodel : register(b0)
//{
//	float4x4 matWVP; // ワールド・ビュー・プロジェクションの合成行列
//	float4x4 matW; // ワールド行列
//	float4x4 matNormal; // ワールド行列
//	float4 diffuseColor; // 拡散反射係数(マテリアルの色）
//	float4 ambientColor; // 環境光
//	float4 specularColor; // 鏡面反射(ハイライト)
//	float shininess; // 光沢
//	bool isTextured; // テクスチャ貼ってあるかどうか
//};

//cbuffer gmodel : register(b1)
//{
//	float4 lightPosition; // 光源の位置
//	float4 eyePosition; // 視点の位置
//};

////───────────────────────────────────────
//// 頂点シェーダー出力構造体
////───────────────────────────────────────
//struct VS_OUT
//{
//	float4 pos : SV_POSITION; // 位置
//	float2 uv : TEXCOORD; // UV座標
//	float4 color : COLOR; // 色（明るさ）
//	float4 eyev : POSITION1; // 視線ベクトル
//	float4 normal : POSITION2; // 法線ベクトル
//};

////───────────────────────────────────────
//// 頂点シェーダー
////───────────────────────────────────────
//VS_OUT VS(float4 pos : POSITION, float4 uv : TEXCOORD, float4 normal : NORMAL)
//{
//	// ピクセルシェーダーへ渡す情報
//	VS_OUT outData = (VS_OUT) 0;

//	// ローカル座標に、ワールド・ビュー・プロジェクション行列をかけて
//	// スクリーン座標に変換し、ピクセルシェーダーへ
//	outData.pos = mul(pos, matWVP);
//	outData.uv = uv;

//	// 法線ベクトルの変換と正規化
//	normal.w = 0;
//	normal = mul(normal, matNormal); // ローカル法線ベクトルをワールド法線ベクトルに変換
//	normal = normalize(normal); // 正規化
//	outData.normal = normal;

//	// ワールド座標系での位置
//	float4 posw = mul(pos, matW);

//	// 光源ベクトルと視線ベクトルの計算
//	float4 light = normalize(lightPosition);

//	// 光源方向と法線ベクトルのドット積を使用して拡散反射の明るさを計算
//	outData.color = saturate(dot(normal, light));

//	// 視線方向ベクトルを計算
//	outData.eyev = eyePosition - posw;

//	// まとめて出力
//	return outData;
//}

////───────────────────────────────────────
//// ピクセルシェーダ
////───────────────────────────────────────
//float4 PS(VS_OUT inData) : SV_Target
//{
//	float4 lightSource = float4(1.0, 1.0, 1.0, 1.0);
//	float4 diffuse;
//	float4 ambient;

//	// 反射ベクトルの計算
//	float4 reflection = reflect(normalize(-lightPosition), inData.normal);

//	// 鏡面反射成分の計算
//	float4 specular = pow(saturate(dot(reflection, normalize(inData.eyev))), shininess) * specularColor;

//	// テクスチャがない場合の計算
//	if (!isTextured)
//	{
//		diffuse = lightSource * diffuseColor * inData.color;
//		ambient = lightSource * ambientColor * inData.color;
//	}
//	else
//	{
//		// テクスチャがある場合の計算
//		diffuse = lightSource * g_texture.Sample(g_sampler, inData.uv) * inData.color;
//		ambient = lightSource * g_texture.Sample(g_sampler, inData.uv) * ambientColor;
//	}

//	// 最終的な色を返す
//	return diffuse + ambient + specular;
//}