#include<imgui.h>
#include"Player.h"
#include"Input/Input.h"
#include"Camera.h"
#include"Graphics/Graphics.h"
#include"EnemyManager.h"
#include"Collision.h"
#include"ProjectileStraight.h"
#include"ProjectileHoming.h"
#include"CameraController.h"
#include"Audio/Audio.h"
#include"SceneLoading.h"
#include"SceneSelect.h"
#include"SceneManager.h"
#include"SceneGame2D.h"
#include"SceneGame2_2D.h"


extern int stage;

template<typename T>
T clamp(T value, T min, T max) {
    return (value < min) ? min : (value > max) ? max : value;
}

//グローバル許可
bool Special = false;
int make = 0;//敗北
int kati = 0;//勝ち

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
    make = 0;
    kati = 0;
    avoidanceFlag = true;
    avoidanceTime =180;
    AvoidanceMove = {};
    Audio& audioManager = Audio::Instance();

    //punch_Sound=audioManager.LoadAudioSource("Data/Audio/idou.wav");
    
    model = new Model("Data/Model/panda/panda.mdl");
    
    scale.x = scale.y = scale.z = 0.2f;

    //ヒットエフェクト読み込み
    hitEffect = new Effect("Data/Effect/Hit.efk");
    health = 1;

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
void Player::Update(float elapsedTime) {
    // 速力を制限
    velocity.x = clamp(velocity.x, -15.0f, 15.0f);
    velocity.z = clamp(velocity.z, -15.0f, 15.0f);


    {
        if (!avoidanceFlag)
        {
            avoidanceTime--;
            if (avoidanceTime < 0)
            {
                avoidanceFlag = true;
                avoidanceTime = 180;
            }
        }
    }
    // プレイヤーの状態を更新
    switch (state) {
    case State::Idle:          UpdateIdleState(elapsedTime); break;
    case State::Move:          UpdateMoveState(elapsedTime); break;
    case State::Land:          UpdateLandState(elapsedTime); break;
    case State::Attack:        UpdateAttackState(elapsedTime); break;
    case State::Damage:        UpdateDamageState(elapsedTime); break;
    case State::Death:         UpdateDeathState(elapsedTime); break;
    case State::Revive:        UpdateReviveState(elapsedTime); break;
    case State::Avoidance:     UpdateAvoidanceState(elapsedTime); break;
    case State::Kick:          UpdateKickState(elapsedTime); break;
    case State::SpecialAttack: UpdateSpecialAttackState(elapsedTime); break;
    }

    // 変換処理の更新
    UpdateTransform();

    // 特殊攻撃の衝突処理
    if (specialAttackCollisionFlag) {
        CollisionNodeVsEnemies("mixamorig:LeftHand", leftHandRadius);
    }
    InputSpecial();

//    HitCheck();
    //速力処理更新
    UpdateVelocity(elapsedTime);
    UpdateInvincibleTimer(elapsedTime);
    projectileManager.Update(elapsedTime);

    // 衝突処理
    if (!avoidanceCollisionFlag) {
        CollisionPlayerVsEnemies();
    }
    CollisionProjectilesVsEnemies();
    //プレイヤーと敵との衝突処理

    //弾丸と敵の衝突処理
   
    //モデルアニメーション更新処理
    model->UpdateAnimetion(elapsedTime);
    model->UpdateTransform(transform);

    // 特殊攻撃のタイミング管理
    if (specialAttack) {
        specialTime--;
    }

    // カメラとのインタラクション管理
    CameraController& cameraController = CameraController::Instance();
    if (!cameraController.GetKirikoCameraCompreat()) {
        kirikoUp = (position.x >= 5 && position.x <= 10 && position.z >= 5 && position.z <= 10);
    }
    else {
        kirikoUp = false;
    }
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

    std::string str = "";

    switch (state)
    {
    case Player::State::Idle:
        str = "Idle";
        break;
    case Player::State::Move:
        str = "Move";
        break;
    case Player::State::Land:
        str = "Land";
        break;
    case Player::State::Attack:
        str = "Attack";
        break;
    case Player::State::Damage:
        str = "Damage";
        break;
    case Player::State::Death:
        str = "Death";
        break;
    case Player::State::Revive:
        str = "Revive";
        break;
    case Player::State::Avoidance:
        str = "Avoidance";
        break;
    case Player::State::Kick:
        str = "Kick";
        break;
    case Player::State::SpecialAttack:
        str = "SpecialAttack";
        break;
    default:
        break;
    }

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

            ImGui::Text(u8"State %s", str.c_str());

            ImGui::InputInt("体力", &health);

            ImGui::Checkbox("Special", &Special);
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
            DirectX::XMFLOAT4(1, 0, 0, 1)
        );
    }
    if (kickCollisionFlag)
    {
        Model::Node* leftHandBone = model->FindNode("mixamorig:LeftToe_End");
        debugRenderer->DrawSphere(DirectX::XMFLOAT3(
            leftHandBone->worldTransform._41,
            leftHandBone->worldTransform._42,
            leftHandBone->worldTransform._43),
            leftHandRadius,
            DirectX::XMFLOAT4(1, 0, 0, 1)
        );
    }
    if (specialAttackCollisionFlag)
    {
        Model::Node* leftHandBone = model->FindNode("mixamorig:LeftHand");
        debugRenderer->DrawSphere(DirectX::XMFLOAT3(
            leftHandBone->worldTransform._41,
            leftHandBone->worldTransform._42,
            leftHandBone->worldTransform._43),
            leftHandRadius,
            DirectX::XMFLOAT4(1, 0, 0, 1)
        );
    }

}




bool Player::InputSpecialAttack()
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    //直進弾丸発射
    if (gamePad.GetButtonDown() & GamePad::BTN_Q&&state!=State::Attack&&state!=State::Avoidance&&state!=State::Kick)
    {
        if (specialTime >= specialTimeMax)
        {
           // Special = true;
            specialAttack = true;
        }
    }
    return false;
}

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
                    Special = true;
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


//蹴り入力処理
bool Player::InputKick()
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    if (gamePad.GetButtonDown() & GamePad::BTN_E&&!specialAttack)
    {
        return true;
    }
    return false;
}

bool Player::HitCheck()
{
    if (Special)
    {
        return true;
    }

    return false;
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

bool Player::InputMove(float elapsedTime)
{
    //進行ベクトル所得
    if (state == State::Avoidance)
    {
        Move(AvoidanceMove.x,AvoidanceMove.z,moveSpeed);
    }
    else
    {
        moveVec = GetMoveVec();
        Turn(elapsedTime,moveVec.x,moveVec.z,turnSpeed);
        Move(moveVec.x, moveVec.z, moveSpeed);
    }

    //移動処理
    //Move(elapsedTime, moveVec.x, moveVec.z, moveSpeed);

    //進行ベクトルがゼロベクトルではない場合は入力された
    return moveVec.x != 0 || moveVec.y != 0 || moveVec.z != 0;
}

bool Player::InputAttack()
{
     Mouse& mouse = Input::Instance().GetMouse();
        if (mouse.GetButtonDown() & Mouse::BTN_LEFT&&!specialAttack )
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
                    enemy->ApplyDamage(1, 0.5f);
                }
            }
        }
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
    }
}

//弾丸入力処理
void Player::InputSpecial()
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    ////直進弾丸発射
 
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
        
            if (specialTime<=0&&specialAttack)
            {
                ProjectileStraight * projectile = new ProjectileStraight(&projectileManager);
                projectile->Launch(dir, pos);
                specialAttack = false;
            }

    if (gamePad.GetButtonDown() & GamePad::BTN_Y)
    {
        //前方向
        DirectX::XMFLOAT3 dir;
        dir.x = sinf(angle.y);
        dir.y = 0;
        dir.z = cosf(angle.y);

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
    avoidanceCollisionFlag = false;
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

    //蹴り入力処理
    if (InputKick())
    {
        TransitionKickState();
    }
    //攻撃入力処理
    if (InputAttack())
    {
        TransitionAttackState();
    }

    //回避入力処理
    if (Inputavoidance())
    {
        TransitionAvoidanceState();
    }

    //必殺技入力処理
    if (specialTime <= 0 && specialAttack)
    {
        TransitionSpecialAttackState();
    }

    //弾丸入力処理
//    InputSpecialAttack();
}

//待機ステート更新処理
void Player::TransitionMoveState()
{
    state = State::Move;

    //走りアニメーション再生
    model->PlayAnimation(Anim_Move, true);
}

//移動ステート更新処理
void Player::UpdateMoveState(float elapsedTime)
{
    moveVec = GetMoveVec();
    Move(moveVec.x, moveVec.z, moveSpeed);
    avoidanceCollisionFlag = false;
    //移動入力処理
    if (!InputMove(elapsedTime))
    {
        TransitionIdleState();
    }
    
    //攻撃入力処理
    if (InputAttack())
    {
        TransitionAttackState();
    }
    //回避入力処理
    if (Inputavoidance())
    {
        TransitionAvoidanceState();
    }
    //蹴り入力処理
    if (InputKick())
    {
        TransitionKickState();
    }

    if (specialTime <= 0 && specialAttack)
    {
        TransitionSpecialAttackState();
    }
    //弾丸入力処理
    InputSpecialAttack();
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
    //着地アニメーション終了
    if (!model->IsPlayAnimation())
    {
        //待機ステートへ
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
        if (make != 18 && stage == 1) { 
            make = 18;
            SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame2D));
        }
        if (make != 25 && stage == 2) { 
            make = 25;
            SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame2_2D));
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

bool Player::Inputavoidance()
{
    Mouse& mouse = Input::Instance().GetMouse();
    if (mouse.GetButtonDown() & Mouse::BTN_RIGHT && !specialAttack && avoidanceFlag)
    {
        AvoidanceMove = GetMoveVec();
        return true;
    }
    return false;
}

//回避ステートへ遷移
void Player::TransitionAvoidanceState()
{
    state = State::Avoidance;
    velocity.x *= 3.0f;
    velocity.y *= 3.0f;
    model->PlayAnimation(Anim_Move, false);
}

//回避ステート更新処理
void Player::UpdateAvoidanceState(float elapsedTime)
{
    
    avoidanceFlag = false;
    InputMove(elapsedTime);
    if (!model->IsPlayAnimation())
    {
        avoidanceCollisionFlag = false;
        TransitionMoveState();
        AvoidanceMove = {};
    }
    avoidanceCollisionFlag = true;
}

//蹴りステートへ遷移
void Player::TransitionKickState()
{
    state = State::Kick;

    model->PlayAnimation(Anim_Revive, false);
}

//蹴りステート更新処理
void Player::UpdateKickState(float elapsedTime)
{
    if (!model->IsPlayAnimation())
    {
        TransitionIdleState();
    }

    float animationTime = model->GetCurrentAnimationSeconds();
    kickCollisionFlag = animationTime >= 0.2f && animationTime <= 0.4f;
    if (kickCollisionFlag)
    {
        //蹴りノードとエネミーの衝突処理
        CollisionNodeVsEnemies("mixamorig:LeftToe_End", kickRadius);
        //CollisionNodeVsEnemies("mixamorig:LeftHand", kickRadius);
    }
}

//必殺技ステートへ遷移
void Player::TransitionSpecialAttackState()
{
    state = State::SpecialAttack;

    model->PlayAnimation(Anim_Attack, false);
}

//必殺技ステートの更新処理
void Player::UpdateSpecialAttackState(float elapsedTime)
{
    float animationTime = model->GetCurrentAnimationSeconds();
    specialAttackCollisionFlag = animationTime >= 0.2f && animationTime <= 0.4f;
    if (specialAttackCollisionFlag)
    {
        //ノードの名前を打つ
        CollisionNodeVsEnemies("mixamorig:LeftHand", leftHandRadius);
    }

    if (!model->IsPlayAnimation())
    {
        if (Special)
        {
            TransitionIdleState();
        }
        else if(!Special)
        {
            health--;
            TransitionDeathState();
        }
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
            nodeRadius,
            enemy->GetPosition(),
            enemy->GetRadius(),
            enemy->GetHeight(),
            outPosition))
        {
            //ダメージを与える
            if (enemy->ApplyDamage(1, 1.5f))
            {
                if (state == State::SpecialAttack)
                {
                    Special = true;
                }
                
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
    
    
    //ダメージ、死亡ステート時は着地した時にステート遷移しないようにする
    if (state != State::Damage && state != State::Death)
    {
    //下方向の速力が一定以上なら着地ステートへ
        if (velocity.y < gravity * 5.0f)
        {
           TransitionLandState();
        }
        //着地ステートへ遷移
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
    