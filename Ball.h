#pragma once
#include "Engine/GameObject.h"

//�X�e�[�W���Ǘ�����N���X
class Ball : public GameObject
{
    int hModel_;
public:
    //�R���X�g���N�^
    Ball(GameObject* parent);

    //�f�X�g���N�^
    ~Ball();

    //������
    void Initialize() override;

    //�X�V
    void Update() override;

    //�`��
    void Draw() override;

    //�J��
    void Release() override;
};