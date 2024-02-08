#include "Sprite.h"

Sprite::Sprite()
    : pVertexBuffer_(nullptr), pIndexBuffer_(nullptr), pConstantBuffer_(nullptr), pTexture_(nullptr),
    vertexNum_(0), indexNum_(0)
{
}

Sprite::Sprite(string _fileName)
    : pVertexBuffer_(nullptr), pIndexBuffer_(nullptr), pConstantBuffer_(nullptr), pTexture_(nullptr),
    vertexNum_(0), indexNum_(0), spriteFileName_(_fileName)
{
}

Sprite::~Sprite()
{
    Release();
}

//������
HRESULT Sprite::Initialize()
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

//�`��
void Sprite::Draw(Transform& transform)
{
    Direct3D::SetShader(SHADER_2D);
    transform.Calclation();
    PassDataToCB(transform);
    SetBufferToPipeline();
    Direct3D::pContext_->DrawIndexed(indexNum_, 0, 0);
}

void Sprite::Draw(Transform& transform, RECT rect, float alpha)
{
}

HRESULT Sprite::Load(std::string fileName)
{
    return E_NOTIMPL;
}

//�J������
void Sprite::Release()
{
    SAFE_DELETE(pTexture_);
    SAFE_RELEASE(pVertexBuffer_);
    SAFE_RELEASE(pIndexBuffer_);
    SAFE_RELEASE(pConstantBuffer_);
}


// ���_��񏀔�
void Sprite::InitVertexData()
{
    // ���_���
    vertices_ =
    {
        {XMVectorSet(-1.0f,  1.0f, 0.0f, 0.0f), XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f)},  // �l�p�`�̒��_�i����j
        {XMVectorSet(1.0f,  1.0f, 0.0f, 0.0f),  XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f)},  // �l�p�`�̒��_�i�E��j
        {XMVectorSet(1.0f, -1.0f, 0.0f, 0.0f),  XMVectorSet(1.0f, 1.0f, 0.0f, 0.0f)},  // �l�p�`�̒��_�i�E���j
        {XMVectorSet(-1.0f, -1.0f, 0.0f, 0.0f), XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)}   // �l�p�`�̒��_�i�����j
    };
    // ���_��
    vertexNum_ = vertices_.size();
}

// ���_�o�b�t�@�쐬
HRESULT Sprite::CreateVertexBuffer()
{    
    // ���_���ǂݍ���
    InitVertexData();
    // ���_�o�b�t�@�̍쐬�Ə�����
    D3D11_BUFFER_DESC bd_vertex{};
    bd_vertex.ByteWidth = sizeof(VERTEX)* vertexNum_;
    bd_vertex.Usage = D3D11_USAGE_DEFAULT;
    bd_vertex.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd_vertex.CPUAccessFlags = 0;
    bd_vertex.MiscFlags = 0;
    bd_vertex.StructureByteStride = 0;

    // ���_��񓝍�
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

// �C���f�b�N�X��������
void Sprite::InitIndexData()
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
HRESULT Sprite::CreateIndexBuffer()
{
    // �C���f�b�N�X���ǂݍ���
    InitIndexData();
    // �C���f�b�N�X�o�b�t�@�̍쐬�Ə�����
    D3D11_BUFFER_DESC bd_index {};
    bd_index.Usage = D3D11_USAGE_DEFAULT;
    bd_index.ByteWidth = sizeof(int) * indexNum_;
    bd_index.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd_index.CPUAccessFlags = 0;
    bd_index.MiscFlags = 0;

    // �C���f�b�N�X��񓝍�
    D3D11_SUBRESOURCE_DATA data_index{};
    data_index.pSysMem = index_.data();
    data_index.SysMemPitch = 0;
    data_index.SysMemSlicePitch = 0;

    HRESULT hr;
    hr = Direct3D::pDevice_->CreateBuffer(&bd_index, &data_index, &pIndexBuffer_);
    if (FAILED(hr))
    {
        MessageBox(NULL, "�C���f�b�N�X�o�b�t�@�̍쐬�Ɏ��s���܂���", "�G���[", MB_OK);
        return hr;
    }
    return S_OK;
}

// �R���X�^���g�o�b�t�@�쐬
HRESULT Sprite::CreateConstantBuffer()
{
    // �R���X�^���g�o�b�t�@�̍쐬
    D3D11_BUFFER_DESC bd_constant{};
    bd_constant.ByteWidth = sizeof(CONSTANT_BUFFER);
    //bd_constant.Usage = D3D11_USAGE_DYNAMIC;
    bd_constant.Usage = D3D11_USAGE_DEFAULT;// �����摜�̎��ύX����
    bd_constant.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    //bd_constant.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    bd_constant.CPUAccessFlags = 0;// �����摜�̎��ύX����
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
HRESULT Sprite::LoadTexture()
{
    HRESULT hr;
    // �e�N�X�`���̓ǂݍ���
    pTexture_ = new Texture;
    hr = pTexture_->Load("Assets\\ball.png");
    if (FAILED(hr))
    {
        MessageBox(NULL, "�e�N�X�`���ǂݍ��݂Ɏ��s���܂���", "�G���[", MB_OK);
        return hr;
    }
    return S_OK;
}

//�e�N�X�`�������[�h
HRESULT Sprite::LoadTexture(std::string fileName)
{
    pTexture_ = new Texture;

    HRESULT hr;
    string fname = "Assets\\" + fileName;
    hr = pTexture_->Load(fname);
    if (FAILED(hr))
    {
        MessageBox(NULL, "�e�N�X�`���̍쐬�Ɏ��s���܂���", "�G���[", MB_OK);
        return hr;
    }
    return S_OK;
}

//�R���X�^���g�o�b�t�@�Ɋe����n��
void Sprite::PassDataToCB(Transform transform)
{
    // �R���X�^���g�o�b�t�@�ɓn�����
    CONSTANT_BUFFER cb{};
    cb.matNormal = XMMatrixTranspose(transform.GetNormalMatrix());

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
void Sprite::SetBufferToPipeline()
{
    // ���_�o�b�t�@�̐ݒ�
    UINT stride = sizeof(VERTEX);
    UINT offset = 0;
    Direct3D::pContext_->IASetVertexBuffers(0, 1, &pVertexBuffer_, &stride, &offset);

    // �C���f�b�N�X�o�b�t�@�̐ݒ�
    stride = sizeof(int);
    offset = 0;
    Direct3D::pContext_->IASetIndexBuffer(pIndexBuffer_, DXGI_FORMAT_R32_UINT, 0);

    // �R���X�^���g�o�b�t�@�̐ݒ�
    Direct3D::pContext_->VSSetConstantBuffers(0, 1, &pConstantBuffer_);
    Direct3D::pContext_->PSSetConstantBuffers(0, 1, &pConstantBuffer_);
}


