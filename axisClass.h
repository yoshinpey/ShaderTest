#pragma once
#include "Engine/GameObject.h"
#include "Engine/Fbx.h"

// 座標軸を管理するクラス
class axisClass : public GameObject
{
    int hModel_;            // モデル番号
    Transform xt, yt, zt;   // 軸方向
public:
    //コンストラクタ
    axisClass(GameObject* parent);

    //デストラクタ
    ~axisClass();

    //初期化
    void Initialize() override;

    //更新
    void Update() override;

    //描画
    void Draw() override;

    //開放
    void Release() override;
};

