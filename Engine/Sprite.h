#pragma once
#include <DirectXMath.h>
#include <vector>
#include "Transform.h"
#include "Direct3D.h"
#include "Texture.h"
#include "Camera.h"

using namespace DirectX;

#define SAFE_DELETE_ARRAY(p) if(p != nullptr){ delete p; p = nullptr;}

//�l�p�`�|���S���i�O�p�`���Q���j��`�悷��N���X
class Sprite
{
private:
	//�R���X�^���g�o�b�t�@�[
	struct CONSTANT_BUFFER
	{
		XMMATRIX	matNormal;		//���[���h�s��
		XMMATRIX	uvTrans;	// �e�N�X�`�����W�ϊ��s��
		XMFLOAT4	color;		// �e�N�X�`���Ƃ̍����F
	};

	//���_���
	struct VERTEX
	{
		XMVECTOR position;			//�ʒu
		XMVECTOR uv;				//UV
	};

	//--------------Initilaize����Ă΂��֐�
	virtual void InitVertexData();		//���_��񏀔�
	HRESULT CreateVertexBuffer();		//���_�o�b�t�@�쐬
	virtual void InitIndexData();		//�C���f�b�N�X��������
	HRESULT CreateIndexBuffer();		//�C���f�b�N�X�o�b�t�@�쐬
	HRESULT CreateConstantBuffer();		//�R���X�^���g�o�b�t�@�쐬
	HRESULT LoadTexture();				//�e�N�X�`�������[�h

	HRESULT LoadTexture(std::string fileName);

	//--------------Draw�֐�����Ă΂��֐�
	void PassDataToCB(Transform transformmatNormal);		//�R���X�^���g�o�b�t�@�Ɋe�����n��
	void SetBufferToPipeline();								//�e�o�b�t�@���p�C�v���C���ɃZ�b�g

protected:
	uint64_t vertexNum_;				//���_��
	std::vector<VERTEX> vertices_;		//���_���
	ID3D11Buffer* pVertexBuffer_;		//���_�o�b�t�@

	uint64_t indexNum_;					//�C���f�b�N�X��
	std::vector<int> index_;			//�C���f�b�N�X���
	ID3D11Buffer* pIndexBuffer_;		//�C���f�b�N�X�o�b�t�@

	ID3D11Buffer* pConstantBuffer_;		//�R���X�^���g�o�b�t�@

	Texture* pTexture_;					//�e�N�X�`��

public:
	Sprite();
	~Sprite();

	//�e��񏉊����A�߂�l�F���s/����
	HRESULT Initialize();				

	//�����Ftransform	�g�����X�t�H�[���N���X�I�u�W�F�N�g
	void Draw(Transform& transform);	
	void Draw(Transform& transform, RECT rect, float alpha);

	//�摜�T�C�Y�̎擾
	//�ߒl�F�摜�T�C�Y
	XMFLOAT2 GetTextureSize() { return pTexture_->GetTextureSize(); }

	//���[�h
	//�����FfileName	�摜�t�@�C����
	//�ߒl�F����/���s
	HRESULT Load(std::string fileName);

	//���
	void Release();						
};

