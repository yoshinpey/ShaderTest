#pragma once
#include "Engine/GameObject.h"
#include "Engine/Quad.h"

// �X�e�[�W��̃��C�g���
struct CBUFF_STAGESCENE
{
    XMFLOAT4	lightPosition;
    XMFLOAT4	eyePos;

};

// �f�t�H���g���C�g�ʒu
namespace
{
    const XMFLOAT4 DEF_LIGHT_POSITION{ 0, 1, -1, 0 };
}

// �X�e�[�W���Ǘ�����N���X
class Stage : public GameObject
{
    int hModel_;                        // �g�[���X
    int hGround_;                       // �n�ʃ��f��
    int hLightBall_;                    // ���C�g���f��

    Transform trDonuts;                 // �g�[���X�̍��W
    Transform trGround;                 // �n�ʂ̍��W
    Transform trLightBall;              // ���C�g�̍��W

    ID3D11Buffer* pCBStageScene_;       // �X�e�[�W�p�̃R���X�^���g�o�b�t�@
    XMFLOAT4	lightSourcePosition_;   // �����̈ʒu

    void IntConstantBuffer();           // �R���X�^���g�o�b�t�@������
public:
    //�R���X�g���N�^
    Stage(GameObject* parent);

    //�f�X�g���N�^
    ~Stage();

    //������
    void Initialize() override;

    //�X�V
    void Update() override;

    //�`��
    void Draw() override;

    //�J��
    void Release() override;

    // �����̈ʒu��ݒ�
    void SetLightPos(XMFLOAT4& _pos) { lightSourcePosition_ = _pos; }

    // �����̈ʒu���擾
    XMFLOAT4 GetLightPos() { return lightSourcePosition_; }
};