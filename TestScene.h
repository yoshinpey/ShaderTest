#pragma once
#include "Engine/GameObject.h"
#include "Engine/RootJob.h"
#include "Engine/Sprite.h"

class TestScene : public GameObject
{
    Sprite* sImage;
public:
    TestScene(GameObject* parent);
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
};
