#pragma once
#include "Engine/GameObject.h"
#include "Engine/Fbx.h"

// ���W�����Ǘ�����N���X
class axisClass : public GameObject
{
    int hModel_;            // ���f���ԍ�
    Transform xt, yt, zt;   // ������
public:
    //�R���X�g���N�^
    axisClass(GameObject* parent);

    //�f�X�g���N�^
    ~axisClass();

    //������
    void Initialize() override;

    //�X�V
    void Update() override;

    //�`��
    void Draw() override;

    //�J��
    void Release() override;
};

