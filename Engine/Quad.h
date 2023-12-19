#pragma once
#include <DirectXMath.h>
#include <vector>
#include "Transform.h"
#include "Direct3D.h"
#include "Texture.h"
#include "Camera.h"

using namespace DirectX;

//�R���X�^���g�o�b�t�@�[
struct CONSTANT_BUFFER
{
	XMMATRIX	matWVP;			//���[���h�s��
	XMMATRIX	matNormal;		
	XMFLOAT2	wSize;
};

//���_���
struct VERTEX
{
	XMVECTOR position;			//�ʒu
	XMVECTOR uv;				//UV
	XMVECTOR normal;			
};

class Quad
{
private:
	//--------------Initilaize����Ă΂��֐�

	virtual void InitVertexData();		//���_��񏀔�
	virtual void InitIndexData();		//�C���f�b�N�X��������

	HRESULT CreateVertexBuffer();		//���_�o�b�t�@�쐬
	HRESULT CreateIndexBuffer();		//�C���f�b�N�X�o�b�t�@�쐬
	HRESULT CreateConstantBuffer();		//�R���X�^���g�o�b�t�@�쐬

	HRESULT LoadTexture();				//�e�N�X�`�������[�h

	//---------Draw�֐�����Ă΂��֐�---------

	void PassDataToCB(Transform transform);	//�R���X�^���g�o�b�t�@�Ɋe�����n��
	void SetBufferToPipeline();

	//------------------------------------------

protected:
	UINT64 vertexNum_;					//���_��
	std::vector<VERTEX> vertices_;		//���_���
	
	UINT64 indexNum_;					//�C���f�b�N�X��
	std::vector<int> index_;			//�C���f�b�N�X���

	ID3D11Buffer* pVertexBuffer_;		//���_�o�b�t�@
	ID3D11Buffer* pIndexBuffer_;		//�C���f�b�N�X�o�b�t�@
	ID3D11Buffer* pConstantBuffer_;		//�R���X�^���g�o�b�t�@

	Texture* pTexture_;					//�e�N�X�`��

public:
	Quad();
	~Quad();
	HRESULT Initialize();				//�e��񏉊����A�߂�l�F���s/����
	void Draw(Transform& transform);	//�`��
	void Release();						//���
};

