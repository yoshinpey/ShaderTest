//������������������������������������������������������������������������������
// �e�N�X�`�����T���v���[�f�[�^�̃O���[�o���ϐ���`
//������������������������������������������������������������������������������
Texture2D g_texture : register(t0);       // �e�N�X�`��
SamplerState g_sampler : register(s0);    // �T���v���[
Texture2D g_toon_texture : register(t1);  // �g�D�[���V�F�[�_�[�p�̃e�N�X�`��

//������������������������������������������������������������������������������
// �R���X�^���g�o�b�t�@
// DirectX �����瑗�M����Ă���A�|���S�����_�ȊO�̏����̒�`
//������������������������������������������������������������������������������
cbuffer gmodel : register(b0)
{
    float4x4 matWVP;        // ���[���h�E�r���[�E�v���W�F�N�V�����̍����s��
    float4x4 matW;          // ���[���h�s��
    float4x4 matNormal;     // ���[���h�@���s��
    float4 diffuseColor;    // �}�e���A���̐F���g�U���ˌW��
    float4 ambientColor;    // ����
    float4 specularColor;   // ���ʔ��ˁ��n�C���C�g
    float shininess;
    bool isTextured;        // �e�N�X�`�����\���Ă��邩�ǂ���
};

cbuffer gmodel_light : register(b1)
{
    float4 lightPosition;
    float4 eyePosition;
};

//������������������������������������������������������������������������������
// ���_�V�F�[�_�[�o�́��s�N�Z���V�F�[�_�[���̓f�[�^�\����
//������������������������������������������������������������������������������
struct VS_OUT
{
    float4 pos : SV_POSITION;   // �ʒu
    float2 uv : TEXCOORD;       // UV���W
    float4 color : COLOR;       // �F�i���邳�j
    float4 eyev : POSITION;     // �����x�N�g��
    float4 normal : NORMAL;     // �@���x�N�g��
};

//������������������������������������������������������������������������������
// ���_�V�F�[�_
//������������������������������������������������������������������������������
VS_OUT VS(float4 pos : POSITION, float4 uv : TEXCOORD, float4 normal : NORMAL)
{
    // �s�N�Z���V�F�[�_�[�֓n�����
    VS_OUT outData = (VS_OUT)0;

    // ���[�J�����W�ɁA���[���h�E�r���[�E�v���W�F�N�V�����s���������
    // �X�N���[�����W�ɕϊ����A�s�N�Z���V�F�[�_�[��
    outData.pos = mul(pos, matWVP);
    outData.uv = uv;
    normal.w = 0;
    normal = mul(normal, matNormal);
    normal = normalize(normal);
    outData.normal = normal;

    float4 light = normalize(lightPosition);
    outData.color = saturate(dot(normal, light));
    float4 posw = mul(pos, matW);
    outData.eyev = eyePosition - posw;

    // �A�E�g���C���p�̃I�t�Z�b�g��������
    float outlineOffset = 0.02; // �A�E�g���C���̑�������
    float4 outlinePos = pos + normal * outlineOffset;

    // �A�E�g���C���p�̒��_�����o��
    VS_OUT outlineData = (VS_OUT)0;
    outlineData.pos = mul(outlinePos, matWVP);
    outlineData.uv = uv;
    outlineData.normal = normal;

    // �܂Ƃ߂ďo��
    return outData;
}

//������������������������������������������������������������������������������
// �s�N�Z���V�F�[�_
//������������������������������������������������������������������������������
float4 PS(VS_OUT inData) : SV_Target
{
    float4 lightSource = float4(1.0, 1.0, 1.0, 1.0);
    float4 diffuse;
    float4 ambient;

    float4 reflection = reflect(normalize(-lightPosition), inData.normal);
    float4 specular = pow(saturate(dot(reflection, normalize(inData.eyev))), shininess) * specularColor;

    float2 uv;

    uv.x = inData.color.x;
    uv.y = 0;

    float4 tI = g_toon_texture.Sample(g_sampler, uv);

    if (!isTextured)
    {
        diffuse = lightSource * diffuseColor * tI;
        ambient = lightSource * ambientColor * tI;
    }
    else
    {
        diffuse = lightSource * g_texture.Sample(g_sampler, inData.uv) * tI;
        ambient = lightSource * g_texture.Sample(g_sampler, inData.uv) * ambientColor;
    }

    // �A�E�g���C���̐F���w��
    float4 outlineColor = float4(1.0, 1.0, 1.0, 1.0); // �A�E�g���C���J���[

    // �A�E�g���C���p�̃s�N�Z���V�F�[�_�[
    float4 outlinePixel = outlineColor;

    // �I���W�i���̃s�N�Z���V�F�[�_�[����
    float4 originalPixel = diffuse + ambient + specular;

    // �A�E�g���C���̕`�������ݒ�
    bool isOutline = dot(normalize(inData.eyev), normalize(inData.normal)) < 0.3; // ��: �����Ɩ@������v����ꍇ�ɃA�E�g���C����`��

    // �A�E�g���C���̕`��
    if (isOutline)
    {
        return outlinePixel;
    }
    else
    {
        return originalPixel;
    }
}
