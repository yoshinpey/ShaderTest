#include "Stage.h"
#include "Engine/Model.h"
#include "Engine/Camera.h"
#include "Engine/Input.h"
#include "axisClass.h"
#include "Engine/Direct3D.h"

//コンストラクタ
Stage::Stage(GameObject* parent)
    :GameObject(parent, "Stage"), hModel_(-1), hGround_(-1), hLightBall_(-1), lightSourcePosition_(DEF_LIGHT_POSITION), pCBStageScene_(nullptr)
{
}

//デストラクタ
Stage::~Stage()
{
}

//初期化
void Stage::Initialize()
{
    ////モデルデータのロード
    //hModel_ = Model::Load("Assets/DO-NATU.fbx");
    //assert(hModel_ >= 0);

    hModel_ = Model::Load("Assets/DO-NATU.fbx");
    assert(hModel_ >= 0);

    hGround_ = Model::Load("Assets/Ground.fbx");
    assert(hGround_ >= 0);

    hLightBall_ = Model::Load("Assets/RedBall.fbx");
    assert(hLightBall_ >= 0);

    hTranslucent_ = Model::Load("Assets/magicalDice2.fbx");
    assert(hTranslucent_ >= 0);

    trDonuts.position_ = { 0, 0, 0 };
    trDonuts.rotate_ = { 0, 0, 0 };
    trDonuts.scale_ = { 0.2,0.2,0.2 };

    trGround.position_ = { 0, 0, 0 };
    trGround.rotate_ = { 0, 0, 0 };
    trGround.scale_ = { 10, 10, 10 };

    trLightBall.position_ = { 0, 0, 0 };
    trLightBall.rotate_ = { 0, 0, 0 };
    trLightBall.scale_ = { 0.4, 0.4, 0.4 };
    Instantiate<axisClass>(this);
    IntConstantBuffer();
}

//更新
void Stage::Update()
{
    Camera::SetPosition(XMVECTOR{ 0, 4, -10, 0 });
    Camera::SetTarget(XMVECTOR{ 0, 1, 0, 0 });
    //trDonuts.rotate_.y += 0.5f;

    if (Input::IsKeyUp(DIK_SPACE))
    {
        Model::ToggleRenderState();
    }
    if (Input::IsKey(DIK_RIGHT))
    {
        XMFLOAT4 p = GetLightPos();
        XMFLOAT4 margin{ p.x + 0.1f, p.y + 0.0f, p.z + 0.0f, p.w + 0.0f };
        SetLightPos(margin);
    }
    if (Input::IsKey(DIK_LEFT))
    {
        XMFLOAT4 p = GetLightPos();
        XMFLOAT4 margin{ p.x - 0.1f, p.y - 0.0f, p.z - 0.0f, p.w - 0.0f };
        SetLightPos(margin);
    }
    if (Input::IsKey(DIK_UP))
    {
        XMFLOAT4 p = GetLightPos();
        XMFLOAT4 margin{ p.x - 0.0f, p.y + 0.1f, p.z - 0.0f, p.w - 0.0f };
        SetLightPos(margin);
    }
    if (Input::IsKey(DIK_DOWN))
    {
        XMFLOAT4 p = GetLightPos();
        XMFLOAT4 margin{ p.x - 0.0f, p.y - 0.1f, p.z - 0.0f, p.w - 0.0f };
        SetLightPos(margin);
    }
    if (Input::IsKey(DIK_W))
    {
        XMFLOAT4 p = GetLightPos();
        XMFLOAT4 margin{ p.x - 0.0f, p.y - 0.0f, p.z + 0.1f, p.w + 0.0f };
        SetLightPos(margin);
    }
    if (Input::IsKey(DIK_S))
    {
        XMFLOAT4 p = GetLightPos();
        XMFLOAT4 margin{ p.x - 0.0f, p.y - 0.0f, p.z - 0.1f, p.w - 0.0f };
        SetLightPos(margin);
    }
    XMFLOAT4 tmp{ GetLightPos() };
    trLightBall.position_ = { tmp.x, tmp.y,tmp.z };

    CBUFF_STAGESCENE cb;
    cb.lightPosition = lightSourcePosition_;
    XMStoreFloat4(&cb.eyePos, Camera::GetEyePosition());

    Direct3D::pContext_->UpdateSubresource(pCBStageScene_, 0, NULL, &cb, 0, 0);
    Direct3D::pContext_->VSSetConstantBuffers(1, 1, &pCBStageScene_);	//頂点シェーダー用	
    Direct3D::pContext_->PSSetConstantBuffers(1, 1, &pCBStageScene_);	//ピクセルシェーダー用
}

//描画
void Stage::Draw()
{
    Direct3D::SetShader(SHADER_TYPE::SHADER_3D);
    Model::SetTransform(hModel_, trDonuts);
    Model::Draw(hModel_);
    //Model::SetTransform(hGround_, trGround);
    //Model::Draw(hGround_);
    Model::SetTransform(hLightBall_, trLightBall);
    Model::Draw(hLightBall_);

    //Direct3D::SetShader(SHADER_TYPE::SHADER_TRANSLUCENT);
    Model::SetTransform(hTranslucent_, trLightBall);
    Model::Draw(hTranslucent_);
}

//開放
void Stage::Release()
{
}

// コンスタントバッファ準備
void Stage::IntConstantBuffer()
{
    D3D11_BUFFER_DESC cb;
    cb.ByteWidth = sizeof(CBUFF_STAGESCENE);
    cb.Usage = D3D11_USAGE_DEFAULT;
    cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cb.CPUAccessFlags = 0;
    cb.MiscFlags = 0;
    cb.StructureByteStride = 0;

    // コンスタントバッファの作成
    HRESULT hr;
    hr = Direct3D::pDevice_->CreateBuffer(&cb, nullptr, &pCBStageScene_);
    if (FAILED(hr))
    {
        MessageBox(NULL, "コンスタントバッファの作成に失敗しました", "エラー", MB_OK);
    }

}