#pragma once

#include<DirectXMath.h>

//カメラコントローラー
class CameraController
{
public:
    CameraController()  {}
    ~CameraController() {}

    //インスタンス取得
    static CameraController& Instance();

    //更新処理
    void Update(float elapsedTime);

    //ターゲット位置設定
    void SetTarget(const DirectX::XMFLOAT3& target) { this->target = target; }

    const bool& GetKirikoCameraCompreat()const { return kirikoCameraCompreat; }

    bool SetKirikoCameraCompreat(const bool& kirikoCameraCompreat) { this->kirikoCameraCompreat; }

private:
    DirectX::XMFLOAT3    target = { 0,0,0 };
    DirectX::XMFLOAT3    angle = { 0,0,0 };
    float                rollSpeed = DirectX::XMConvertToRadians(90);
    float                range = 10.0f;
    float                maxAngleX = DirectX::XMConvertToRadians(45);
    float                minAngleX = DirectX::XMConvertToRadians(-45);

    float               sensitivity=0.007f;
    float               sensitivity2 = 0.0015f;
    float               maxPitch=1;
    float               maxYaw = 1;
    DirectX::XMFLOAT2 oldMouse{ 0,0 };
    bool                cameraReset = false;
    bool                cameraReset2 = false;
    bool                kirikoCameraCompreat = false;
   

};
