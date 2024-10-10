//#include"Stage.h"
#include"StageMain.h"

//static Stage* instance = nullptr;

////�C���X�^���X�擾
//Stage& Stage::Instance()
//{
//
//    return *instance;
//}
//
////�R���X�g���N�^
//Stage::Stage()
//{
//
//    instance = this;
//
//    //�X�e�[�W���f����ǂݍ���
//    model = new Model("Data/Model/ExampleStage/ExampleStage.mdl");
//}
//
//Stage::~Stage()
//{
//    //�X�e�[�W���f���̔j��
//    delete model;
//}
//
//void Stage::Update(float elapsedTime)
//{
//    //���͓��ɂ�邱�ƂȂ�
//}
//
//void Stage::Render(ID3D11DeviceContext* dc, Shader* shader)
//{
//    //�V�F�[�_�[�Ƀ��f����`�悵�Ă��炤
//    shader->Draw(dc, model);
//}
//
////���C�L���X�g
//bool Stage::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
//{
//    return Collision::IntersectRayVsModel(start,end,model,hit);
//}

//�R���X�g���N�^
StageMain::StageMain()
{
    //�X�e�[�W���f����ǂݍ���
    model = new Model("Data/Model/stage/stage.mdl");
    scale.x = scale.y = scale.z = 0.03f;
}

//�f�X�g���N�^
StageMain::~StageMain()
{
    //�X�e�[�W���f����j��
    delete model;
}

//�X�V����
void StageMain::Update(float elapsedTime)
{
    //���͓��ɂ�邱�Ƃ͂Ȃ�
    UpdateTransform();
}

//�`�揈��
void StageMain::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    //�V�F�[�_�[�Ƀ��f����`�悵�Ă��炤
    model->UpdateTransform(transform);
    shader->Draw(dc, model);
}

//���C�L���X�g
bool StageMain::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
    return Collision::IntersectRayVsModel(start, end, model, hit);;
}

void StageMain::UpdateTransform()
{
    //�X�P�[���s����쐬
    DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);

    ////��]�s����쐬
    //DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.z,angle.y,angle.x);

    //��]�s����쐬
    DirectX::XMMATRIX X = DirectX::XMMatrixRotationX(angle.x);

    DirectX::XMMATRIX Y = DirectX::XMMatrixRotationY(angle.y);

    DirectX::XMMATRIX Z = DirectX::XMMatrixRotationZ(angle.z);

    DirectX::XMMATRIX R = Y * X * Z;

    //�ʒu�s����쐬
    DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);

    //3�̍s���g�ݍ��킹�A���[���h�s����쐬
    DirectX::XMMATRIX W = S * R * T;

    //�v�Z�������[���h�s������o��
    DirectX::XMStoreFloat4x4(&transform, W);

}
