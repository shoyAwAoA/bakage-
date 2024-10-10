#pragma once

//#include "Graphics/Shader.h"
#include"Graphics/Model.h"
#include"Stage.h"
//#include"Collision.h"

////�X�e�[�W
//class Stage
//{
//public:
//    Stage();
//    ~Stage();
//
//    //�X�V����
//    void Update(float elapsedTime);
//
//    //�`�揈��
//    void Render(ID3D11DeviceContext* dc, Shader* shader);
//
//    //���C�L���X�g
//    bool RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit);
//
//    //�C���X�^���X�擾
//    static Stage& Instance();
//
//private:
//    Model* model = nullptr;
//};

class StageMain :public Stage
{
public:
    StageMain();

    ~StageMain()override;

    //�X�V����
    void Update(float elapsedTime)override;

    //�`�揈��
    void Render(ID3D11DeviceContext* dc, Shader* shader)override;

    //���C�L���X�g
    bool RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)override;
    
    void UpdateTransform();

private:
    Model* model = nullptr;
    DirectX::XMFLOAT3       position = { 24,0,0 };
    DirectX::XMFLOAT3       angle = { 0,0,0 };
    DirectX::XMFLOAT3       scale = { 1,1,1 };
    DirectX::XMFLOAT3 velocity = { 0,0,0 };
    DirectX::XMFLOAT4X4     transform = {
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1
    };
};
