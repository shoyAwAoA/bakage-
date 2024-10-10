#pragma once

//#include "Graphics/Shader.h"
#include"Graphics/Model.h"
#include"Stage.h"
//#include"Collision.h"

////ステージ
//class Stage
//{
//public:
//    Stage();
//    ~Stage();
//
//    //更新処理
//    void Update(float elapsedTime);
//
//    //描画処理
//    void Render(ID3D11DeviceContext* dc, Shader* shader);
//
//    //レイキャスト
//    bool RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit);
//
//    //インスタンス取得
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

    //更新処理
    void Update(float elapsedTime)override;

    //描画処理
    void Render(ID3D11DeviceContext* dc, Shader* shader)override;

    //レイキャスト
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
