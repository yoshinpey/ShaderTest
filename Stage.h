#pragma once
#include "Engine/GameObject.h"
#include "Engine/Quad.h"

// ステージ上のライト情報
struct CBUFF_STAGESCENE
{
    XMFLOAT4	lightPosition;
    XMFLOAT4	eyePos;

};

// デフォルトライト位置
namespace
{
    const XMFLOAT4 DEF_LIGHT_POSITION{ 0, 1, -1, 0 };
}

// ステージを管理するクラス
class Stage : public GameObject
{
    int hModel_;                        // トーラス
    int hGround_;                       // 地面モデル
    int hLightBall_;                    // ライトモデル

    Transform trDonuts;                 // トーラスの座標
    Transform trGround;                 // 地面の座標
    Transform trLightBall;              // ライトの座標

    ID3D11Buffer* pCBStageScene_;       // ステージ用のコンスタントバッファ
    XMFLOAT4	lightSourcePosition_;   // 光源の位置

    void IntConstantBuffer();           // コンスタントバッファ初期化
public:
    //コンストラクタ
    Stage(GameObject* parent);

    //デストラクタ
    ~Stage();

    //初期化
    void Initialize() override;

    //更新
    void Update() override;

    //描画
    void Draw() override;

    //開放
    void Release() override;

    // 光源の位置を設定
    void SetLightPos(XMFLOAT4& _pos) { lightSourcePosition_ = _pos; }

    // 光源の位置を取得
    XMFLOAT4 GetLightPos() { return lightSourcePosition_; }
};