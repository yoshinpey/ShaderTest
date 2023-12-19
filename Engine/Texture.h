#pragma once
#include <d3d11.h>
#include <string>
#include <wrl.h>
#include <DirectXMath.h>

using namespace DirectX;
using namespace Microsoft::WRL;
using std::string;

class Texture
{
	ID3D11SamplerState* pSampler_;		//サンプラー作成
	ID3D11ShaderResourceView* pSRV_;	//シェーダーリソースビュー作成
	XMFLOAT2 imgSize_;
public:
	Texture();
	~Texture();

	XMFLOAT2 GetTextureSize() { return imgSize_; }

	// テクスチャの読み込み
	HRESULT Load(string fileName);

	// テクスチャの解放
	void Release();

	// サンプラーステートの取得
	ID3D11SamplerState* GetSampler() { return pSampler_; }

	// シェーダーリソースビューの取得
	ID3D11ShaderResourceView* GetSRV() { return pSRV_; }

};