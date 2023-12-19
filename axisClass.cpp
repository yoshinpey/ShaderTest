#include "axisClass.h"
#include "Engine/Model.h"
#include "Engine/Fbx.h"

axisClass::axisClass(GameObject* parent)
    :GameObject(parent, "AXIS"), hModel_(-1)
{
}

axisClass::~axisClass()
{
}

void axisClass::Initialize()
{
    hModel_ = Model::Load("Assets/Arrow.fbx");
    assert(hModel_ >= 0);

    xt.rotate_.y = 0;
    xt.scale_ = { 0.5,0.5,0.5 };
    yt.rotate_.z = 90;
    yt.scale_ = { 0.5,0.5,0.5 };
    zt.rotate_.y = -90;
    zt.scale_ = { 0.5,0.5,0.5 };
}

void axisClass::Update()
{
}

void axisClass::Draw()
{
    Model::SetTransform(hModel_, xt);
    Model::Draw(hModel_);
    Model::SetTransform(hModel_, yt);
    Model::Draw(hModel_);
    Model::SetTransform(hModel_, zt);
    Model::Draw(hModel_);
}

void axisClass::Release()
{
}
