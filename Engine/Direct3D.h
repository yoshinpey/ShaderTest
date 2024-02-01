#pragma once

//�C���N���[�h
#include <d3d11.h>

//�����J
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

//�}�N����`
#define SAFE_DELETE(p) if(p != nullptr){ delete p; p = nullptr;}
#define SAFE_RELEASE(p) if(p != nullptr){ p->Release(); p = nullptr;}

//�V�F�[�_�[�̎��
enum SHADER_TYPE
{
	SHADER_2D,		//2D�V�F�[�_�[
	SHADER_3D,		//3D�V�F�[�_�[
	SHADER_POINT,	//�|�C���g�V�F�[�_�[
	SHADER_TOON,	//�g�D�[���V�F�[�_�[
	SHADER_MAX		//�V�F�[�_�[�̍ő吔
};

namespace Direct3D
{
	//�f�o�C�X�ƃf�o�C�X�R���e�L�X�g
	extern ID3D11Device* pDevice_;
	extern ID3D11DeviceContext* pContext_;

	//������
	HRESULT Initialize(int winW, int winH, HWND hWnd);

	//�V�F�[�_�[����
	HRESULT InitShader();
	HRESULT InitShader3D();
	HRESULT InitShader2D();
	HRESULT InitShaderNormalMap();
	HRESULT InitShaderToon();
	HRESULT InitShaderPointLight();

	//�g�p����V�F�[�_�[��ݒ�
	void SetShader(SHADER_TYPE type);

	//�`��J�n
	void BeginDraw();

	//�`��I��
	void EndDraw();

	//���
	void Release();
};

