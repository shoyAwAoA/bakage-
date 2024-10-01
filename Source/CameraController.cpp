#include"CameraController.h"
#include"Camera.h"
#include"Input/Input.h"
#include"Player.h"

void CameraController::Update(float elapsedTime)
{
    Mouse& mouse = Input::Instance().GetMouse();

    float ax = mouse.GetPositionX();
    float ay = mouse.GetPositionY();

    float oldAx = mouse.GetOldPositionX();
    float oldAy = mouse.GetOldPositionY();

    float dx = ax - oldAx;
    float dy = ay - oldAy;
    //カメラの回転速度
    float speed = rollSpeed * elapsedTime;

    //スティックの入力値に合わせてX軸とY軸を回転
    
    angle.y += dx*sensitivity;//これがX
    angle.x += dy*sensitivity;//これがY

    //X軸のカメラ回転を制限
    if (angle.y > DirectX::XM_PI)
    {
        angle.y -= DirectX::XM_2PI;
    }

    //カメラ回転値を回転行列に変換
    DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

    //回転行列から前方向ベクトルを取り出す
    DirectX::XMVECTOR Front = Transform.r[2];
    DirectX::XMFLOAT3 front;
    DirectX::XMStoreFloat3(&front, Front);

    //注意点から後ろベクトル方向に一定距離離れたカメラ視点を求める
    DirectX::XMFLOAT3 eye;
    Player& player = Player::Instance();

    target.x = player.GetPosition().x+front.x*range;
    target.y = player.GetPosition().y+front.y*range;
    target.z = player.GetPosition().z+front.z*range;


    eye.x = target.x - front.x * range;
    eye.y = target.y - front.y * range;
    eye.z = target.z - front.z * range;

    target.x = eye.x+front.x*range;
    target.y = eye.y+front.y*range;
    target.z = eye.z+front.z*range;

    eye.y = player.GetPosition().y + player.GetHeight();

        //カメラの視点と注視点を設定
    Camera::Instance().SetLookAt(eye, target, DirectX::XMFLOAT3(0, 1, 0));

}