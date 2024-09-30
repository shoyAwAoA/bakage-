#include<imgui.h>
#include"Player.h"
#include"Input/Input.h"
#include"Camera.h"
#include"Graphics/Graphics.h"
#include"EnemyManager.h"
#include"Collision.h"
#include"ProjectileStraight.h"
#include"ProjectileHoming.h"


static Player* instance = nullptr;

Player& Player::Instance()
{
    // TODO: return ステートメントをここに挿入します
    return *instance;
}


//コンストラクタ
Player::Player()
{

    //インスタンスポインタ設定
    instance = this;

    //model = new Model("Data/Model/Mr.Incredible/Mr.Incredible.mdl");
    model = new Model("Data/Model/Jammo/Jammo.mdl");
    //model->PlayAnimation(0);
    scale.x = scale.y = scale.z = 0.01f;

    //ヒットエフェクト読み込み
    hitEffect = new Effect("Data/Effect/Hit.efk");

    //待機ステートへ遷移
    TransitionIdleState();
}
Player::~Player()
{
    if (model)
    {
        delete model;
        model = nullptr;

    }
    
    delete hitEffect;
}
//更新処理
void Player::Update(float elapsedTime)
{
    //Bボタン押下でワンショットアニメーション再生
    //GamePad& gamePad = Input::Instance().GetGamePad();
    //if (gamePad.GetButtonDown() & GamePad::BTN_B)
    //{
    //    model->PlayAnimation(Anim_Running, true);
    //}
    //ワンショットアニメーション再生が終わったらループアニメーション再生
    //if (!model->IsPlayAnimation())
    //{
    //    model->PlayAnimation(1, true);
    //}



    ////進行ベクトル取得
    //DirectX::XMFLOAT3 moveVec = GetMoveVec();

    ////移動処理
    //float moveSpeed = this->moveSpeed * elapsedTime;
    //position.x += moveVec.x * moveSpeed;
    //position.z += moveVec.z * moveSpeed;

    //InputMove(elapsedTime);

    ////入力情報を取得
    //GamePad& gamePad = Input::Instance().GetGamePad();
    //float ax = gamePad.GetAxisLX();
     //float ay = gamePad.GetAxisLY();

    ////移動操作
    //float moveSpeed = 5.0f * elapsedTime;
    //{
    //    //左スティックの入力情報をもとにXZへの移動処理
    //    position.x += ax*moveSpeed;
    //    position.z += ay*moveSpeed;

    //}

    ////回転操作
    //float rotateSpeed = DirectX::XMConvertToRadians(360) * elapsedTime;
    //if (gamePad.GetButton() & GamePad::BTN_A)
    //{
    //    //X軸回転操作
    //    angle.z += rotateSpeed;
    //}
    //if (gamePad.GetButton() & GamePad::BTN_B)
    //{
    //    //Y軸回転操作
    //    angle.y += rotateSpeed;
    //}
    //if (gamePad.GetButton() & GamePad::BTN_X)
    //{
    //    //Z軸回転操作
    //    angle.x += rotateSpeed;
    //}
     

    //ジャンプ入力処理
    //InputJump();

    //弾丸入力処理
    //InputProjectile();
    switch (state)
    {
    case State::Idle:
        UpdateIdleState(elapsedTime);
        break;
    case State::Move:
        UpdateMoveState(elapsedTime);
        break;
    case State::Jump:
        UpdateJumpState(elapsedTime);
        break;
    case State::Land:
        UpdateLandState(elapsedTime);
        break;
    case State::Attack:
        UpdateAttackState(elapsedTime);
        break;
    case State::Damage:
        UpdateDamageState(elapsedTime);
        break;
    case State::Death:
        UpdateDeathState(elapsedTime);
        break;
    case State::Revive:
        UpdateReviveState(elapsedTime);
        break;
    }


    //速力処理更新
    UpdateVelocity(elapsedTime);

    //無敵時間更新
    UpdateInvincibleTimer(elapsedTime);

    //弾丸更新処理
    projectileManager.Update(elapsedTime);

    //プレイヤーと敵との衝突処理
    CollisionPlayerVsEnemies();

    //弾丸と敵の衝突処理
    CollisionProjectilesVsEnemies();

    //オブジェクト行列を更新
    UpdateTransform();

    //モデルアニメーション更新処理
    model->UpdateAnimetion(elapsedTime);

    //モデル行列更新
    model->UpdateTransform(transform);
}
//描画処理
void Player::Render(ID3D11DeviceContext* dc, Shader* sharder)
{
    sharder->Draw(dc, model);

    //弾丸描画処理
    projectileManager.Render(dc,sharder);
}

//デバッグ用GUI描画
void Player::DrawDebugGUI()
{
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("Player", nullptr, ImGuiTreeNodeFlags_None))
    {
        //トランスフォーム
        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
        {
            //位置
            ImGui::InputFloat3("Position", &position.x);
            //回転
            DirectX::XMFLOAT3 a;
            a.x = DirectX::XMConvertToDegrees(angle.x);
            a.y = DirectX::XMConvertToDegrees(angle.y);
            a.z = DirectX::XMConvertToDegrees(angle.z);
            ImGui::InputFloat3("Angle", &a.x);
            angle.x = DirectX::XMConvertToRadians(a.x);
            angle.y = DirectX::XMConvertToRadians(a.y);
            angle.z = DirectX::XMConvertToRadians(a.z);
            //スケール
            ImGui::InputFloat3("Scale", &scale.x);


            
        }

        if (ImGui::CollapsingHeader("Velocity", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::InputFloat3("Velocity", &velocity.x);
        }
    }
    ImGui::End();
}

//デバッグプリミティブ描画
void Player::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    ////衝突判定用のデバッグ球を描画
    //debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));

    //衝突判定用のデバッグ円柱を描画
    debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));

    //弾丸デバッグプリミティブ描画
    projectileManager.DrawDebugPrimitive();

    //攻撃衝突用の左手ノードのデバッグ球を描画
    //Model::Node* leftHandBone = model->FindNode("mixamoring:LeftHand");
    //debugRenderer->DrawSphere(DirectX::XMFLOAT3(
    //    leftHandBone->worldTransform._41,
    //    leftHandBone->worldTransform._42,
    //    leftHandBone->worldTransform._43),
    //    leftHandRadius,
    //    DirectX::XMFLOAT4(1, 0, 0, 1));
    if (attackCollisionFlag)
    {
        Model::Node* leftHandBone = model->FindNode("mixamorig:LeftHand");
        debugRenderer->DrawSphere(DirectX::XMFLOAT3(
            leftHandBone->worldTransform._41,
            leftHandBone->worldTransform._42,
            leftHandBone->worldTransform._43),
            leftHandRadius,
            DirectX::XMFLOAT4(1, 0, 0, 1));
    }

}



bool Player::InputJump()
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    if (gamePad.GetButtonDown() & GamePad::BTN_A)
    {
            //ジャンプ回数制限
            if (jumpCount < jumpLimit)
            {
                ++jumpCount;
                //ジャンプ
                Jump(jumpSpeed);

                //ジャンプ入力した
                return true;
            }
    }

    return false;
}

////ジャンプ入力処理
//void Player::InputJump()
//{
//    GamePad& gamePad = Input::Instance().GetGamePad();
//    if (gamePad.GetButtonDown() & GamePad::BTN_A)
//    {
//        if (jumpCount > 0)
//        {
//            Jump(jumpSpeed);
//            jumpCount--;
//
//        }
//    }
//}

//弾丸と敵の衝突処理
void Player::CollisionProjectilesVsEnemies()
{
    EnemyManager& enemyManager = EnemyManager::Instance();

    //全ての弾丸と全ての敵を総当たりで衝突処理
    int projectileCount = projectileManager.GetProjectileCount();
    int enemyCount = enemyManager.GetEnemyCount();
    for (int i = 0; i < projectileCount; ++i)
    {
        Projectile* projectile = projectileManager.GetProjectile(i);

        for (int j = 0; j < enemyCount; ++j)
        {
            Enemy* enemy = enemyManager.GetEnemy(j);

            //衝突処理
            DirectX::XMFLOAT3 outPosition;
            if (Collision::IntersectSphereVsCylinder(
                projectile->GetPosition(),
                projectile->GetRadius(),
                enemy->GetPosition(),
                enemy->GetRadius(),
                enemy->GetHeight(),
                outPosition))
            {
                //ダメージを与える
                if (enemy->ApplyDamage(1, 0.5f))
                {
                    //吹き飛ばす
                    DirectX::XMFLOAT3 impulse;
                    float tuyo = 10.0f;
                    DirectX::XMFLOAT3 enemy_pos = enemy->GetPosition();
                    DirectX::XMFLOAT3 projectile_pos = projectile->GetPosition();
                    float vx = enemy_pos.x - projectile_pos.x;
                    float vz = enemy_pos.z - projectile_pos.z;
                    impulse.x = vx * tuyo;
                    impulse.y = tuyo * 0.5f;
                    impulse.z = vz * tuyo;
                    //DirectX::XMVECTOR impulseVec = DirectX::XMLoadFloat3(&impulse);
                    //DirectX::XMVECTOR outPositionVec = DirectX::XMLoadFloat3(&outPosition);
                    //DirectX::XMVECTOR projectileVec = DirectX::XMLoadFloat3(&projectile->GetPosition());

                    //DirectX::XMVECTOR direction = DirectX::XMVectorSubtract(outPositionVec, projectileVec);

                    //direction = DirectX::XMVector3Normalize(direction);
                    //DirectX::XMFLOAT3 directionflo;
                    //DirectX::XMStoreFloat3(&directionflo, direction);

                    //impulse.x = directionflo.x = 10.0f;
                    //impulse.y = directionflo.y = 0;
                    //impulse.z = directionflo.z = 10.0f;


                    enemy->AddImpulse(impulse);
                }

                //ヒットエフェクト再生
                {
                    DirectX::XMFLOAT3 e = enemy->GetPosition();
                    e.y += enemy->GetHeight() * 0.5f;
                    hitEffect->Play(e);


                }


                
                    //弾丸破棄
                    projectile->Destroy();
                
            }
        }
    }
}



DirectX::XMFLOAT3 Player::GetMoveVec() const
{
    //入力情報を取得
    GamePad& gamePad = Input::Instance().GetGamePad();
    float ax = gamePad.GetAxisLX();
    float ay = gamePad.GetAxisLY();

    //カメラ方向とスティックの入力値によって進行方向を計算する
    Camera& camera = Camera::Instance();
    const DirectX::XMFLOAT3& cameraRight = camera.GetRight();
    const DirectX::XMFLOAT3& cameraFront = camera.GetFront();

    //移動ベクトルはXZ平面に水平なベクトルになるようにする

    //カメラ右方向ベクトルをXZ単位ベクトルに変換
    float cameraRightX = cameraRight.x;
    float cameraRightZ = cameraRight.z;
    float cameraRightLength = sqrtf(cameraRightX * cameraRightX + cameraRightZ * cameraRightZ);
        if (cameraRightLength > 0.0f)
        {
            //単位ベクトル化
            cameraRightX /= cameraRightLength;
            cameraRightZ /= cameraRightLength;

        }

    //カメラ前方向ベクトルをXZ単位ベクトルに変換
        float cameraFrontX = cameraFront.x;
        float cameraFrontZ = cameraFront.z;
        float cameraFrontLength = sqrtf(cameraFrontX * cameraFrontX + cameraFrontZ * cameraFrontZ);
        if (cameraFrontLength > 0.0f)
        {
            //単位ベクトル化
            cameraFrontX /= cameraFrontLength;
            cameraFrontZ /= cameraFrontLength;
            
        }

    //スティックの水平入力値をカメラ右方向に反映し、
    //スティックの垂直入力値をカメラ前方向に反映し、
    //進行ベクトルを計算する
    DirectX::XMFLOAT3 vec;
    vec.x = ax * cameraRightX + ay * cameraFrontX;
    vec.z = ax * cameraRightZ + ay * cameraFrontZ;
    //Y軸方向には移動しない
    vec.y = 0.0f;

    return vec;


}


////移動入力処理
//void Player::InputMove(float elapsedTime)
//{
//    //進行ベクトル取得
//    DirectX::XMFLOAT3 moveVec = GetMoveVec();
//
//    //移動処理
//    //Move(elapsedTime, moveVec.x, moveVec.z, moveSpeed);
//
//    Move(moveVec.x, moveVec.z, moveSpeed);
//
//    //旋回処理
//    Turn(elapsedTime, moveVec.x, moveVec.z, turnSpeed);
//}





bool Player::InputMove(float elapsedTime)
{
    //進行ベクトル所得
    DirectX::XMFLOAT3 moveVec = GetMoveVec();

    //移動処理
    //Move(elapsedTime, moveVec.x, moveVec.z, moveSpeed);
    Move(moveVec.x, moveVec.z, moveSpeed);

    //旋回処理
    Turn(elapsedTime, moveVec.x, moveVec.z, turnSpeed);

    //進行ベクトルがゼロベクトルではない場合は入力された
    return moveVec.x != 0 || moveVec.y != 0 || moveVec.z != 0;
}

bool Player::InputAttack()
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    if (gamePad.GetButtonDown() & GamePad::BTN_B)
    {
        return true;
    }
    return false;
}

//プレイヤーとエネミーとの衝突処理
void Player::CollisionPlayerVsEnemies()
{
    EnemyManager& enemyManager = EnemyManager::Instance();

    //全ての敵と総当たりで衝突処理
    int enemyCount = enemyManager.GetEnemyCount();
    for (int i = 0; i < enemyCount; ++i)
    {
        Enemy* enemy = enemyManager.GetEnemy(i);

        //衝突処理
        DirectX::XMFLOAT3 outPosition;
        //if (Collision::IntersectSphereVsSphere(
        //    this->position,
        //    this->radius,
        //    enemy->GetPosition(),
        //    enemy->GetRadius(),outPosition))
        //{
        //    //押し出し後の位置設定
        //    enemy->SetPosition(outPosition);


        //}
;
    float position_x = position.x - enemy->GetPosition().x;
    float position_z = position.z - enemy->GetPosition().z;
    float range = (position_x * position_x) + (position_z * position_z);

    if (enemy->GetPosition().x - 1 <= GetPosition().x && enemy->GetPosition().x + 1 >= GetPosition().x)
    {
        if (enemy->GetPosition().y + 1 == GetPosition().y)
        {
            if (enemy->GetPosition().z - 1 <= GetPosition().z && enemy->GetPosition().z + 1 >= GetPosition().z)
            {
                Jump(jumpSpeed);
                enemy->ApplyDamage(1, 0.5f);
            }
        }
    }


    //if (!position.y > enemy->GetPosition().y && position.y <= enemy->GetPosition().y + enemy->GetHeight() && range < (radius + enemy->GetRadius()) * (radius + enemy->GetRadius()))
    //{
        if (Collision::IntersectCylinderVsCylinder(
            position,
            radius,
            height,
            enemy->GetPosition(),
            enemy->GetRadius(),
            enemy->GetHeight(),
            outPosition))
        {
            enemy->SetPosition(outPosition);
        }
    //}



    //DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&position);
    //DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&enemy->GetPosition());
    //DirectX::XMVECTOR V = DirectX::XMVectorSubtract(P, E);
    //DirectX::XMVECTOR N = DirectX::XMVector3Normalize(V);
    //DirectX::XMFLOAT3 normal;
    //DirectX::XMStoreFloat3(&normal, N);

        //////敵を踏みつけた時に小ジャンプする
        //if (position.y>enemy->GetPosition().y&&position.y <= enemy->GetPosition().y + enemy->GetHeight() && range < (radius + enemy->GetRadius()) * (radius + enemy->GetRadius()))
        //{
        //    Jump(jumpSpeed * 0.5f);
        //    enemy->ApplyDamage(1, 0.5f);
        //}


    }
}

//弾丸入力処理
void Player::InputProjectile()
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    //直進弾丸発射
    if (gamePad.GetButtonDown() & GamePad::BTN_X)
    {
        //前方向
        DirectX::XMFLOAT3 dir;
        //dir.x = sinf(angle.y) * cosf(angle.x);
        dir.x = sinf(angle.y);
        dir.y = sinf(angle.x);
        //dir.z = cosf(angle.y) * cosf(angle.x);
        dir.z = cosf(angle.y);// * cosf(angle.x);

        //発射位置(プレイヤーの腰あたり)
        DirectX::XMFLOAT3 pos;
        pos.x = this->position.x;
        pos.y = this->position.y + this->height * 0.5f;
        pos.z = this->position.z;
        //発射
        ProjectileStraight * projectile = new ProjectileStraight(&projectileManager);
        projectile->Launch(dir, pos);
        //projectileManager.Register(projectile);
    }

    if (gamePad.GetButtonDown() & GamePad::BTN_Y)
    {
        //前方向
        DirectX::XMFLOAT3 dir;
        dir.x = sinf(angle.y);
        dir.y = 0;
        dir.z = cosf(angle.y);
        //dir.x = sinf(angle.y) * cosf(angle.x);
        //dir.y = sinf(angle.x);
        //dir.z = cosf(angle.y) * cosf(angle.x);

        //発射位置(プレイヤーの腰あたり)
        DirectX::XMFLOAT3 pos;
        pos.x = position.x;
        pos.y = position.y+height*0.5f;
        pos.z = position.z;

        //ターゲット(デフォルトではプレイヤーの前方)
        DirectX::XMFLOAT3 target;
        target.x = pos.x + dir.x * 100.0f;
        target.y = pos.y + dir.y * 100.0f;
        target.z = pos.z + dir.z * 100.0f;

        //一番近くの敵をターゲットにする
        float dist = FLT_MAX;
        EnemyManager& enemyManager = EnemyManager::Instance();
        int enemyCount = enemyManager.GetEnemyCount();
        for (int i = 0; i < enemyCount; ++i)
        {
            //敵との距離判定
            Enemy* enemy = EnemyManager::Instance().GetEnemy(i);
            DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&position);
            DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&enemy->GetPosition());
            DirectX::XMVECTOR V = DirectX::XMVectorSubtract(E, P);
            DirectX::XMVECTOR D = DirectX::XMVector3LengthSq(V);
            float d;
            DirectX::XMStoreFloat(&d, D);
            if (d < dist)
            {
                dist = d;
                target = enemy->GetPosition();
                target.y += enemy->GetHeight() * 0.5f;
            }
        }
        //発射
        ProjectileHoming* projectilehoming = new ProjectileHoming(&projectileManager);
        projectilehoming->Launch(dir, pos,target);
        
    }
}

//待機ステートへ遷移
void Player::TransitionIdleState()
{
    state = State::Idle;

    //待機アニメーション再生
    model->PlayAnimation(Anim_Idle, true);
}

//待機ステート更新処理
void Player::UpdateIdleState(float elapsedTime)
{
    //移動入力処理
    if (InputMove(elapsedTime))
    {
        TransitionMoveState();
    };

    //ジャンプ入力処理
    if (InputJump())
    {
        TransitionJumpState();
    }

    //攻撃入力処理
    if (InputAttack())
    {
        TransitionAttackState();
    }
    //ジャンプ入力処理
//    InputJump();

    //弾丸入力処理
    InputProjectile();
}

//待機ステート更新処理
void Player::TransitionMoveState()
{

    state = State::Move;

    //走りアニメーション再生
    model->PlayAnimation(Anim_Running, true);
}

//移動ステート更新処理
void Player::UpdateMoveState(float elapsedTime)
{
    //移動入力処理
    if (!InputMove(elapsedTime))
    {
        TransitionIdleState();
    }
    //ジャンプ入力処理
    if (InputJump())
    {
        TransitionJumpState();
    }
    //攻撃入力処理
    if (InputAttack())
    {
        TransitionAttackState();
    }
    ////ジャンプ入力処理
    //InputJump();

    //弾丸入力処理
    InputProjectile();


}

//ジャンプステートへ遷移
void Player::TransitionJumpState()
{
    state = State::Jump;

    //ジャンプアニメーション再生
    model->PlayAnimation(Anim_Jump, false);
}

void Player::UpdateJumpState(float elapsedTime)
{
    //移動入力処理
    InputMove(elapsedTime);

    if (InputJump())
    {
        model->PlayAnimation(Anim_Jump_Flip, false);
    };
    //アニメーション終了
    if (!model->IsPlayAnimation())
    {
        model->PlayAnimation(Anim_Falling, false);
    }
    InputProjectile();
}

//着地ステートへ遷移
void Player::TransitionLandState()
{
    state = State::Land;

    //着地アニメーション再生
    model->PlayAnimation(Anim_Landing, false);
}

//着地ステート更新処理
void Player::UpdateLandState(float elapsedTime)
{
    if (!model->IsPlayAnimation())
    {
        TransitionIdleState();
    }
}

//攻撃ステートへ遷移
void Player::TransitionAttackState()
{
    state = State::Attack;
    model->PlayAnimation(Anim_Attack, false);
}

//攻撃ステート更新処理
void Player::UpdateAttackState(float elapsedTime)
{
    if (!model->IsPlayAnimation())
    {
        TransitionIdleState();
    }

    ////左手ノードとエネミーの衝突処理
    //CollisionNodeVsEnemies("mixamorig:LeftHand", leftHandRadius);

    //任意のアニメーション再生区間でのみ衝突判定処理をする
    float animationTime = model->GetCurrentAnimationSeconds();
    attackCollisionFlag=animationTime >= 0.3f && animationTime <= 0.4f;
    if (attackCollisionFlag)
    {
        //左手ノードとエネミーの衝突処理
        CollisionNodeVsEnemies("mixamorig:LeftHand", leftHandRadius);
    }
}
//ダメージステートへ遷移
void Player::TransitionDamageState()
{
    state = State::Damage;

    //ダメージアニメーション再生
    model->PlayAnimation(Anim_GetHit1, false);
}

//ダメージステート更新処理
void Player::UpdateDamageState(float elapsedTime)
{
    //ダメージアニメーションが終わったら待機ステートへ遷移
    if (!model->IsPlayAnimation())
    {
        TransitionIdleState();
    }
}

//死亡ステートへ遷移
void Player::TransitionDeathState()
{
    state = State::Death;

    //死亡アニメーション再生
    model->PlayAnimation(Anim_Death, false);
}

//死亡ステート更新処理
void Player::UpdateDeathState(float elapsedTime)
{
    if (!model->IsPlayAnimation())
    {
        //ボタンを押したら復活ステートへ遷移
        GamePad& gamePad = Input::Instance().GetGamePad();
        if (gamePad.GetButtonDown() & GamePad::BTN_A)
        {
            TransitionReviveState();
        }
    }
}
//復活ステートへ遷移
void Player::TransitionReviveState()
{
    state = State::Revive;

    //体力回復
    health = maxHealth;

    //復活アニメーション再生
    model->PlayAnimation(Anim_Revive, false);
}

//復活ステート更新処理
void Player::UpdateReviveState(float elapsedTime)
{
    //復活アニメーション終了後に待機ステートへ遷移
    if (!model->IsPlayAnimation())
    {
        TransitionIdleState();
    }
}

void Player::CollisionNodeVsEnemies(const char* nodeName, float nodeRadius)
{
    //ノード取得
    Model::Node* node = model->FindNode(nodeName);

    //ノード位置取得
    DirectX::XMFLOAT3 nodePosition;
    nodePosition.x = node->worldTransform._41;
    nodePosition.y = node->worldTransform._42;
    nodePosition.z = node->worldTransform._43;

    //指定のノードと全ての敵を総当たりで衝突処理
    EnemyManager& enemyManager = EnemyManager::Instance();
    int enemyCount = enemyManager.GetEnemyCount();

    for (int i = 0; i < enemyCount; ++i)
    {
        Enemy* enemy = enemyManager.GetEnemy(i);

        //衝突処理
        DirectX::XMFLOAT3 outPosition;
        if (Collision::IntersectSphereVsCylinder(
            position,
            radius,
            enemy->GetPosition(),
            enemy->GetRadius(),
            enemy->GetHeight(),
            outPosition))
        {

            //ダメージを与える
            if (enemy->ApplyDamage(1, 0.5f))
            {
                //吹っ飛ばす
                const float power = 5.0f;
                DirectX::XMFLOAT3 e = enemy->GetPosition();
                float vx = e.x - nodePosition.x;
                float vz = e.z - nodePosition.z;

                float lengthXZ = sqrtf((vx * vx) + (vz * vz));

                vx /= lengthXZ;
                vz /= lengthXZ;

                DirectX::XMFLOAT3 impulse;

                impulse.x = vx * power;
                impulse.y = power * 0.5f;
                impulse.z = vz * power;

                enemy->AddImpulse(impulse);
            }
            //ヒットエフェクト再生
            {
                DirectX::XMFLOAT3 e = enemy->GetPosition();
                e.y += enemy->GetHeight() * 0.5f;
                hitEffect->Play(e);
            }
        }
    }
}


void Player::OnLanding()
{
    jumpCount = 0;

    ////下方向の速力が一定以上なら着地ステートへ
    //if (velocity.y < gravity * 5.0f)
    //{
    //    //state = State::Land;
    //    TransitionLandState();
    //}
    //ダメージ、死亡ステート時は着地した時にステート遷移しないようにする
    if (state != State::Damage && state != State::Death)
    {
        //着地ステートへ遷移
        TransitionLandState();
    }
}

//ダメージを受けたときに呼ばれる
void Player::OnDamaged()
{
        //ダメージステートへ遷移
        TransitionDamageState();
}

//死亡した時に呼ばれる
void Player::OnDead()
{
    //死亡ステートへ遷移
    TransitionDeathState();
}
