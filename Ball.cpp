#include "Engine/Model.h"
#include "Ball.h"

//�R���X�g���N�^
Ball::Ball(GameObject* parent)
    :GameObject(parent, "Ball"), hModel_(-1)
{
}

//�f�X�g���N�^
Ball::~Ball()
{
}

//������
void Ball::Initialize()
{
    //���f���f�[�^�̃��[�h
    //hModel_ = Model::Load("Assets/numberSphere.fbx");
    //assert(hModel_ >= 0);
}

//�X�V
void Ball::Update()
{
}

//�`��
void Ball::Draw()
{
    //Model::SetTransform(hModel_, transform_);
    //Model::Draw(hModel_);
}

//�J��
void Ball::Release()
{
}