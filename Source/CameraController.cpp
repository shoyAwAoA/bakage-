#include"CameraController.h"
#include"Camera.h"
#include"Input/Input.h"
#include"Player.h"

static CameraController* instance = nullptr;


CameraController& CameraController::Instance()
{
    
    return *instance;
}

void CameraController::Update(float elapsedTime)
{
    instance = this;

    Mouse& mouse = Input::Instance().GetMouse();

    Player& player = Player::Instance();

    if (!player.GetKirikoUp())
    {
        float ax = mouse.GetPositionX();
        float ay = mouse.GetPositionY();

        float oldAx = mouse.GetOldPositionX();
        float oldAy = mouse.GetOldPositionY();

        float dx = ax - oldAx;
        float dy = ay - oldAy;

    //マウスの入力値に合わせてX軸とY軸を回転
        angle.y += dx*sensitivity;//これがX
        angle.x += dy*sensitivity;//これがY
    }

    /*else
    {


    }*/
    //カメラの回転速度
    float speed = rollSpeed * elapsedTime;

    //X軸のカメラ回転を制限
    if (angle.y > DirectX::XM_PI)
    {
        angle.y -= DirectX::XM_2PI;
    }


    if (angle.x > DirectX::XMConvertToRadians(45))
    {
        angle.x = DirectX::XMConvertToRadians(45);
    }
    if (angle.x < DirectX::XMConvertToRadians(-45))
    {
        angle.x = DirectX::XMConvertToRadians(-45);
    }

    //カメラ回転値を回転行列に変換
    DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

    //回転行列から前方向ベクトルを取り出す
    DirectX::XMVECTOR Front = Transform.r[2];
    DirectX::XMFLOAT3 front;
    DirectX::XMStoreFloat3(&front, Front);

    //注意点から後ろベクトル方向に一定距離離れたカメラ視点を求める
    DirectX::XMFLOAT3 eye;
    //if (player.GetKirikoUp()&&!kirikoCameraCompreat)
    //{
    //    target.x = player.GetPosition().x + front.x * range;
    //    target.y = player.GetPosition().y + front.y * range;
    //    target.z = player.GetPosition().z + front.z * range;

    //    eye.x = target.x - front.x * range;
    //    eye.y = target.y - front.y * range;
    //    eye.z = target.z - front.z * range;

    //    target.x = eye.x + front.x * range;
    //    target.y = eye.y + front.y * range;
    //    target.z = eye.z + front.z * range;

    //    if (!cameraReset&&!kirikoCameraCompreat)
    //    {
    //        angle.y = 0;
    //        angle.x = 0;
    //        cameraReset = true;
    //    }
    //    if (cameraReset)
    //    {
    //        angle.x -= sensitivity2;
    //        if (angle.x < DirectX::XMConvertToRadians(-60))
    //        {
    //            kirikoCameraCompreat = true;
    //            cameraReset = true;
    //        }
    //    }
    //    eye.y = player.GetPosition().y + (player.GetHeight() * 0.7f);
    //    eye.z = player.GetPosition().z;
    //    
    //    //カメラの視点と注視点を設定
    //    Camera::Instance().SetLookAt(eye, target, DirectX::XMFLOAT3(0, 1, 0));
    //}
    //else 
    {
        if (!cameraReset2&&kirikoCameraCompreat)
        {
            angle.y = 0;
            angle.x = 0;
            cameraReset2 = true;
        }
        eye.x = target.x - front.x * range;
        eye.y = target.y - front.y * range;
        eye.z = target.z - front.z * range;

        // カメラに視点を注視点を設定
        Camera::Instance().SetLookAt(eye, target, DirectX::XMFLOAT3(0, 1, 0));

    }
}