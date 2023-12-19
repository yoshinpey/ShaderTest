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
	ID3D11SamplerState* pSampler_;		//�T���v���[�쐬
	ID3D11ShaderResourceView* pSRV_;	//�V�F�[�_�[���\�[�X�r���[�쐬
	XMFLOAT2 imgSize_;
public:
	Texture();
	~Texture();

	XMFLOAT2 GetTextureSize() { return imgSize_; }

	// �e�N�X�`���̓ǂݍ���
	HRESULT Load(string fileName);

	// �e�N�X�`���̉��
	void Release();

	// �T���v���[�X�e�[�g�̎擾
	ID3D11SamplerState* GetSampler() { return pSampler_; }

	// �V�F�[�_�[���\�[�X�r���[�̎擾
	ID3D11ShaderResourceView* GetSRV() { return pSRV_; }

};