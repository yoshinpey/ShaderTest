#include "Quad.h"

Quad::Quad()
    : pVertexBuffer_(nullptr), pIndexBuffer_(nullptr), pConstantBuffer_(nullptr), pTexture_(nullptr), indexNum_(0), vertexNum_(0)
{
}

Quad::~Quad()
{
    Release();
}

HRESULT Quad::Initialize()
{
    HRESULT hr = S_OK;

    hr = CreateVertexBuffer();
    if (FAILED(hr))
    {
        MessageBox(NULL, "���_�o�b�t�@�̍쐬�Ɏ��s���܂���", "�G���[", MB_OK);
        return hr;
    }

    hr = CreateIndexBuffer();
    if (FAILED(hr))
    {
        MessageBox(NULL, "�C���f�b�N�X�o�b�t�@�̍쐬�Ɏ��s���܂���", "�G���[", MB_OK);
        return hr;
    }

    hr = CreateConstantBuffer();
    if (FAILED(hr))
    {
        MessageBox(NULL, "�R���X�^���g�o�b�t�@�̍쐬�Ɏ��s���܂���", "�G���[", MB_OK);
        return hr;
    }

    hr = LoadTexture();
    if (FAILED(hr))
    {
        MessageBox(NULL, "�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���", "�G���[", MB_OK);
        return hr;
    }

    return hr;
}


void Quad::Draw(Transform& transform)
{
    Direct3D::SetShader(SHADER_3D);
    transform.Calclation();                     //�g�����X�t�H�[�����v�Z
    PassDataToCB(transform);	                //�R���X�^���g�o�b�t�@�ɏ���n��
    SetBufferToPipeline();                      //���_�o�b�t�@�A�C���f�b�N�X�o�b�t�@�A�R���X�^���g�o�b�t�@���p�C�v���C���ɃZ�b�g

    //�`��
    Direct3D::pContext_->DrawIndexed(indexNum_, 0, 0);
}

//�J������
void Quad::Release()
{
    SAFE_DELETE(pTexture_);
    SAFE_RELEASE(pVertexBuffer_);
    SAFE_RELEASE(pIndexBuffer_);
    SAFE_RELEASE(pConstantBuffer_);
}

void Quad::InitVertexData()
{
    // ���_���
    vertices_ =
    {
        {XMVectorSet(-1.0f,  1.0f, 0.0f, 0.0f), XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f), XMVectorSet(0.0f,0.0f,-1.0, 0.0f)},  // �l�p�`�̒��_�i����j
        {XMVectorSet(1.0f,  1.0f, 0.0f, 0.0f),  XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f), XMVectorSet(0.0f,0.0f,-1.0, 0.0f)},  // �l�p�`�̒��_�i�E��j
        {XMVectorSet(1.0f, -1.0f, 0.0f, 0.0f),  XMVectorSet(1.0f, 1.0f, 0.0f, 0.0f), XMVectorSet(0.0f,0.0f,-1.0, 0.0f)},  // �l�p�`�̒��_�i�E���j
        {XMVectorSet(-1.0f, -1.0f, 0.0f, 0.0f), XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), XMVectorSet(0.0f,0.0f,-1.0, 0.0f)}   // �l�p�`�̒��_�i�����j
    };
    // ���_��
    vertexNum_ = vertices_.size();
}

// ���_�o�b�t�@�̍쐬
HRESULT Quad::CreateVertexBuffer()
{
    InitVertexData();
    // ���_�o�b�t�@�̍쐬�Ə�����
    D3D11_BUFFER_DESC bd_vertex{};
    bd_vertex.ByteWidth = sizeof(VERTEX) * vertexNum_;
    bd_vertex.Usage = D3D11_USAGE_IMMUTABLE;
    bd_vertex.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd_vertex.CPUAccessFlags = 0;
    bd_vertex.MiscFlags = 0;
    bd_vertex.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA data_vertex{};
    data_vertex.pSysMem = vertices_.data();

    HRESULT hr;
    hr = Direct3D::pDevice_->CreateBuffer(&bd_vertex, &data_vertex, &pVertexBuffer_);
    if (FAILED(hr))
    {
        MessageBox(NULL, "���_�o�b�t�@�̍쐬�Ɏ��s���܂���", "�G���[", MB_OK);
        return hr;
    }
    return S_OK;
}

//�C���f�b�N�X��������
void Quad::InitIndexData()
{
    // �C���f�b�N�X���
    index_ = 
    {
        0, 1, 2, 
        2, 3, 0 
    };
    indexNum_ = index_.size();
}

// �C���f�b�N�X�o�b�t�@�쐬
HRESULT Quad::CreateIndexBuffer()
{
    // �C���f�b�N�X���ǂݍ���
    InitIndexData();
    // �C���f�b�N�X�o�b�t�@�̍쐬�Ə�����
    D3D11_BUFFER_DESC bd_index{};
    bd_index.Usage = D3D11_USAGE_IMMUTABLE;
    bd_index.ByteWidth = sizeof(UINT) * indexNum_;
    bd_index.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd_index.CPUAccessFlags = 0;
    bd_index.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA data_index{};
    data_index.pSysMem = index_.data();
    data_index.SysMemPitch = 0;
    data_index.SysMemSlicePitch = 0;

    HRESULT hr;
    hr= Direct3D::pDevice_->CreateBuffer(&bd_index, &data_index, &pIndexBuffer_);
    if (FAILED(hr))
    {
        MessageBox(NULL, "�C���f�b�N�X�o�b�t�@�̍쐬�Ɏ��s���܂���", "�G���[", MB_OK);
        return hr;
    }
    return S_OK;
}

// �R���X�^���g�o�b�t�@�̍쐬
HRESULT Quad::CreateConstantBuffer()
{
    // �R���X�^���g�o�b�t�@�̍쐬
    D3D11_BUFFER_DESC bd_constant{};
    bd_constant.ByteWidth = sizeof(CONSTANT_BUFFER);
    bd_constant.Usage = D3D11_USAGE_DYNAMIC;
    bd_constant.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd_constant.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    bd_constant.MiscFlags = 0;
    bd_constant.StructureByteStride = 0;

    HRESULT hr;
    hr = Direct3D::pDevice_->CreateBuffer(&bd_constant, nullptr, &pConstantBuffer_);
    if (FAILED(hr))
    {
        MessageBox(NULL, "�R���X�^���g�o�b�t�@�̍쐬�Ɏ��s���܂���", "�G���[", MB_OK);
        return hr;
    }
    return S_OK;
}

//�e�N�X�`�������[�h
HRESULT Quad::LoadTexture()
{
    // �e�N�X�`���̓ǂݍ���
    HRESULT hr;
    pTexture_ = new Texture;
    hr = pTexture_->Load("Assets\\ball.png");
    if (FAILED(hr))
    {
        MessageBox(NULL, "�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���", "�G���[", MB_OK);
        return hr;
    }
    return S_OK;
}

//�R���X�^���g�o�b�t�@�Ɋe�����n��
void Quad::PassDataToCB(Transform transform)
{
    // �R���X�^���g�o�b�t�@�ɓn�����
    CONSTANT_BUFFER cb{};
    cb.matWVP = XMMatrixTranspose(transform.GetWorldMatrix() * Camera::GetViewMatrix() * Camera::GetProjectionMatrix());
    cb.matNormal = XMMatrixTranspose(transform.GetNormalMatrix());
    cb.wSize = pTexture_->GetTextureSize();

    // �R���X�^���g�o�b�t�@�̍X�V
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    Direct3D::pContext_->Map(pConstantBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    memcpy_s(mappedResource.pData, mappedResource.RowPitch, (void*)(&cb), sizeof(cb));

    ID3D11SamplerState* pSampler = pTexture_->GetSampler();
    Direct3D::pContext_->PSSetSamplers(0, 1, &pSampler);

    ID3D11ShaderResourceView* pSRV = pTexture_->GetSRV();
    Direct3D::pContext_->PSSetShaderResources(0, 1, &pSRV);

    Direct3D::pContext_->Unmap(pConstantBuffer_, 0);
}

//�e�o�b�t�@���p�C�v���C���ɃZ�b�g
void Quad::SetBufferToPipeline()
{
    using Direct3D::pContext_;

    // ���_�o�b�t�@�̐ݒ�
    UINT stride = sizeof(VERTEX);
    UINT offset = 0;
    pContext_->IASetVertexBuffers(0, 1, &pVertexBuffer_, &stride, &offset);

    // �C���f�b�N�X�o�b�t�@�̐ݒ�
    stride = sizeof(int);
    offset = 0;
    pContext_->IASetIndexBuffer(pIndexBuffer_, DXGI_FORMAT_R32_UINT, 0);

    // �R���X�^���g�o�b�t�@�̐ݒ�
    pContext_->VSSetConstantBuffers(0, 1, &pConstantBuffer_);
    pContext_->PSSetConstantBuffers(0, 1, &pConstantBuffer_);

    pContext_->DrawIndexed(indexNum_, 0, 0);
}
