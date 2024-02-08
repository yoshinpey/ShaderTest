#pragma once
#include <d3d11.h>
#include <string>
#include <wrl.h>
#include <vector>
#include <DirectXMath.h>

using std::vector;
using std::string;
using namespace DirectX;

class GifTexture
{
	ID3D11SamplerState* pSampler_;
	ID3D11ShaderResourceView* pSRV_;
	XMFLOAT2 imgSize_;
	int currSamplerState;
	int currSRV;
	int imageNum;
public:
	GifTexture();
	~GifTexture();
	XMFLOAT2 GetTextureSize() { return imgSize_; }
	HRESULT Load(string filename);
	void Release();
	int GetTextureNum() { return imageNum; }
	ID3D11SamplerState* GetSampler() { return pSampler_; }
	ID3D11ShaderResourceView* GetSRV() { return pSRV_; }
};