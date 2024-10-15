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
extern bool tutoFlag;
template<typename T>
T clamp(T value, T min, T max) {
    return (value < min) ? min : (value > max) ? max : value;
}

//グローバル許可
bool Special = false;
int make = 0;//敗北
int kati = 0;//勝ち
bool kirikoSkip = false;
bool hipopoSkip = false;
bool playerAvoidanceFlag = false;
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
    playerAvoidanceFlag = avoidanceFlag;
    avoidanceTime =180;
    AvoidanceMove = {};
    specialAttack = false;
    specialAttack2CollisionFlag = false;
    specialAttack3CollisionFlag = false;
    specialAttack4CollisionFlag = false;

    Audio& audioManager = Audio::Instance();
    
    //punch_Sound=audioManager.LoadAudioSource("Data/Audio/idou.wav");
    
    model = new Model("Data/Model/jiki/jiki2.mdl");
    scale.x = scale.y = scale.z = 0.2f;

    //ヒットエフェクト読み込み
    hitEffect = new Effect("Data/Effect/Hit.efk");
    health = 1;

    height = 3.0f;

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
    playerAvoidanceFlag = avoidanceFlag;
    {
        if (!avoidanceFlag)
        {
            avoidanceTime--;
            if (avoidanceTime < 0)
            {
                avoidanceFlag = true;
                avoidanceTime = 130;
            }
        }
    }
    // プレイヤーの状態を更新
    switch (state) {
    case State::Idle:          UpdateIdleState(elapsedTime); break;
    case State::Move:          UpdateMoveState(elapsedTime); break;
    case State::Attack:        UpdateAttackState(elapsedTime); break;
    case State::Kick:          UpdateKickState(elapsedTime); break;
    case State::Avoidance:     UpdateAvoidanceState(elapsedTime); break;
    case State::SpecialAttack: UpdateSpecialAttackState(elapsedTime); break;
    case State::Death:         UpdateDeathState(elapsedTime); break;

    }

    // 変換処理の更新
    UpdateTransform();

    //// 特殊攻撃の衝突処理
    //if (specialAttackCollisionFlag) {
    //    CollisionNodeVsEnemies("mixamorig:LeftHand", leftHandRadius);
    //}
    //InputSpecial();

//    HitCheck();
    //速力処理更新
    UpdateVelocity(elapsedTime);
    UpdateInvincibleTimer(elapsedTime);
    projectileManager.Update(elapsedTime);

    // 衝突処理
    if (!avoidanceCollisionFlag) {
        CollisionPlayerVsEnemies();
    }
    //CollisionProjectilesVsEnemies();
    //プレイヤーと敵との衝突処理

    //弾丸と敵の衝突処理
   
    //モデルアニメーション更新処理
    model->UpdateAnimetion(elapsedTime);
    model->UpdateTransform(transform);

    // 特殊攻撃のタイミング管理
    if (specialAttack) {
        //specialTime--;
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
    case Player::State::Attack:
        str = "Attack";
        break;
    case Player::State::Kick:
        str = "Kick";
        break;
    case Player::State::Avoidance:
        str = "Avoidance";
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
            ImGui::Checkbox("playerAvoidance", &playerAvoidanceFlag);
        }
        if (ImGui::CollapsingHeader("Velocity", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::InputFloat3("Velocity", &velocity.x);
        }
    }
    ImGui::End();

    //ImGui::SetNextWindowPos(ImVec2(300, 300), ImGuiCond_FirstUseEver);
    //ImGui::SetNextWindowSize(ImVec2(50, 50), ImGuiCond_FirstUseEver);
    //static char name[256] = { 0 };
    //if (ImGui::Begin("NameWindow", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground))
    //{
    //    if (ImGui::InputText("", name, sizeof(name)))
    //    {
    //        
    //        OutputDebugStringA(name);
    //    }
    //}
    //ImGui::End();

    //ImGui::SetNextWindowPos(ImVec2(600, 300), ImGuiCond_FirstUseEver);
    //ImGui::SetNextWindowSize(ImVec2(0, 0), ImGuiCond_FirstUseEver);

    //ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(1, 1, 1,0));
    //if (ImGui::Begin("TextWindow", nullptr, ImGuiWindowFlags_NoDecoration))
    //{
    //    ImGui::CalcTextSize(name, name, true, 10.0f);
    //    ImGui::Text(u8"% s\n", name);
    // }
    //ImGui::PopStyleColor();
    //ImGui::End();
 
    // 2番目のフォントを使用（[0]はデフォルトフォント）// "TextWindow"の開始 

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
    if (kickCollisionFlag)
    {
        Model::Node* kickBone = model->FindNode("joint25");
        debugRenderer->DrawSphere(DirectX::XMFLOAT3(
            kickBone->worldTransform._41,
            kickBone->worldTransform._42,
            kickBone->worldTransform._43),
            kickRadius,
            DirectX::XMFLOAT4(1, 0, 0, 1)
        );
    }

    if (attackCollisionFlag)
    {
        Model::Node* panchiBone = model->FindNode("joint21");
        debugRenderer->DrawSphere(DirectX::XMFLOAT3(
            panchiBone->worldTransform._41,
            panchiBone->worldTransform._42,
            panchiBone->worldTransform._43),
            leftHandRadius,
            DirectX::XMFLOAT4(1, 0, 0, 1)
        );
    }
        if (specialAttackCollisionFlag)
        {
            Model::Node* specialAttackBone = model->FindNode("joint11");
            debugRenderer->DrawSphere(DirectX::XMFLOAT3(
                specialAttackBone->worldTransform._41,
                specialAttackBone->worldTransform._42,
                specialAttackBone->worldTransform._43),
                specialAttackRadius,
                DirectX::XMFLOAT4(1, 0, 0, 1)
            );
        }
        if (specialAttack2CollisionFlag)
        {
            Model::Node* specialAttack2Bone = model->FindNode("joint20");
            debugRenderer->DrawSphere(DirectX::XMFLOAT3(
                specialAttack2Bone->worldTransform._41,
                specialAttack2Bone->worldTransform._42,
                specialAttack2Bone->worldTransform._43),
                specialAttackRadius2,
                DirectX::XMFLOAT4(1, 0, 0, 1)
            );
        }
}






//弾丸と敵の衝突処理
//void Player::CollisionProjectilesVsEnemies()
//{
//    EnemyManager& enemyManager = EnemyManager::Instance();
//
//    //全ての弾丸と全ての敵を総当たりで衝突処理
//    int projectileCount = projectileManager.GetProjectileCount();
//    int enemyCount = enemyManager.GetEnemyCount();
//    for (int i = 0; i < projectileCount; ++i)
//    {
//        Projectile* projectile = projectileManager.GetProjectile(i);
//
//        for (int j = 0; j < enemyCount; ++j)
//        {
//            Enemy* enemy = enemyManager.GetEnemy(j);
//
//            //衝突処理
//            DirectX::XMFLOAT3 outPosition;
//            if (Collision::IntersectSphereVsCylinder(
//                projectile->GetPosition(),
//                projectile->GetRadius(),
//                enemy->GetPosition(),
//                enemy->GetRadius(),
//                enemy->GetHeight(),
//                outPosition))
//            {
//                //ダメージを与える
//                if (enemy->ApplyDamage(2, 0.5f))
//                {
// 
//                    Special = true;//大事
// 
//                    //吹き飛ばす
//                    DirectX::XMFLOAT3 impulse;
//                    float tuyo = 10.0f;
//                    DirectX::XMFLOAT3 enemy_pos = enemy->GetPosition();
//                    DirectX::XMFLOAT3 projectile_pos = projectile->GetPosition();
//                    float vx = enemy_pos.x - projectile_pos.x;
//                    float vz = enemy_pos.z - projectile_pos.z;
//                    impulse.x = vx * tuyo;
//                    impulse.y = tuyo * 0.5f;
//                    impulse.z = vz * tuyo;
//                    enemy->AddImpulse(impulse);
//                }
//
//                //ヒットエフェクト再生
//                {
//                    DirectX::XMFLOAT3 e = enemy->GetPosition();
//                    e.y += enemy->GetHeight() * 0.5f;
//                    hitEffect->Play(e);
//                }
//                    //弾丸破棄
//                    projectile->Destroy(); 
//            }
//        }
//    }
//}


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

    if (InputSpecialAttack())
    {
        TransitionSpecialAttackState();
    }

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

    /*if (specialTime <= 0 && specialAttack)
    {
        TransitionSpecialAttackState();
    }*/

    //弾丸入力処理
    if (InputSpecialAttack())
    {
        TransitionSpecialAttackState();
    };
}

bool Player::InputAttack()
{
    Mouse& mouse = Input::Instance().GetMouse();
    GamePad& gamePad = Input::Instance().GetGamePad();
    if (mouse.GetButtonDown() & Mouse::BTN_LEFT && state != State::Kick && state != State::Avoidance && state != State::SpecialAttack)
    {
        return true;
    }
    return false;
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
    attackCollisionFlag=animationTime >= 0.5f && animationTime <= 0.8f;
    if (attackCollisionFlag)
    {
        //左手ノードとエネミーの衝突処理
        CollisionNodeVsEnemies("joint21", leftHandRadius);
    }
}

//死亡ステートへ遷移
void Player::TransitionDeathState()
{
    state = State::Death;

    //死亡アニメーション再生
    model->PlayAnimation(Anim_Kick, false);
}

//死亡ステート更新処理
void Player::UpdateDeathState(float elapsedTime)
{
       
    if (!model->IsPlayAnimation())
    {
        if (make != 17 && stage == 1) { 
            make = 17;
            SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame2D));
        }
        if (make != 23 && stage == 2) { 
            make = 23;
            SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame2_2D));
        }
    }
}




bool Player::Inputavoidance()
{
    Mouse& mouse = Input::Instance().GetMouse();
    if (mouse.GetButtonDown() & Mouse::BTN_RIGHT && state!=State::SpecialAttack&& avoidanceFlag&&state!=State::Attack&&state!=State::Kick)
    {
        AvoidanceMove = GetMoveVec();
        return true;
    }
    return false;
}

//回避ステートへ遷移
void Player::TransitionAvoidanceState()
{
    avoidanceFlag = false;
    state = State::Avoidance;
    velocity.x *= 3.0f;
    velocity.y *= 3.0f;
    model->PlayAnimation(Anim_Avoidance, false);
}

//回避ステート更新処理
void Player::UpdateAvoidanceState(float elapsedTime)
{
    InputMove(elapsedTime);
    if (!model->IsPlayAnimation())
    {
        avoidanceCollisionFlag = false;
        TransitionMoveState();
        AvoidanceMove = {};
    }
    avoidanceCollisionFlag = true;
}

//蹴り入力処理
bool Player::InputKick()
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    Mouse& mouse = Input::Instance().GetMouse();//mouse.GetButtonDown() & Mouse::BTN_LEFT
    if (gamePad.GetButtonDown() &GamePad::BTN_E &&state != State::Attack && state != State::Avoidance && state != State::SpecialAttack)
    {
        return true;
    }
    return false;
}

//蹴りステートへ遷移
void Player::TransitionKickState()
{
    state = State::Kick;

    model->PlayAnimation(Anim_Kick, false);
}

//蹴りステート更新処理
void Player::UpdateKickState(float elapsedTime)
{
    if (!model->IsPlayAnimation())
    {
        TransitionIdleState();
    }

    float animationTime = model->GetCurrentAnimationSeconds();
    kickCollisionFlag = animationTime >= 0.3f && animationTime <= 0.5f;
    if (kickCollisionFlag)
    {
        //蹴りノードとエネミーの衝突処理
        CollisionNodeVsEnemies("joint25", kickRadius);
        //CollisionNodeVsEnemies("mixamorig:LeftHand", kickRadius);
    }
}

bool Player::InputSpecialAttack()
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    if (gamePad.GetButtonDown() & GamePad::BTN_Q && state != State::Attack && state != State::Avoidance && state != State::Kick)
    {
        if (tutoFlag)
        {
            return true;
        }
        else if (!tutoFlag&&!specialAttack)
        {
            
            return true;
        }
    }
    return false;
}

//必殺技ステートへ遷移
void Player::TransitionSpecialAttackState()
{
    specialAttack = true;
    state = State::SpecialAttack;
    model->PlayAnimation(Anim_SpecialAttack, false);
}

//必殺技ステートの更新処理
void Player::UpdateSpecialAttackState(float elapsedTime)
{
    if (!model->IsPlayAnimation())
    {
        if (!tutoFlag)
        {
            if (!Special)
            {
                health--;
                TransitionDeathState();
            }
            else if(Special)
            {
                TransitionIdleState();
            }

        }
        else if (tutoFlag)
        {
            TransitionIdleState();
        }
    }
    float animationTime = model->GetCurrentAnimationSeconds();
    specialAttackCollisionFlag = animationTime >= 0.3f && animationTime <= 0.7f;
    specialAttack2CollisionFlag=animationTime>=0.3f && animationTime <= 0.7f;
    if (specialAttackCollisionFlag&&specialAttack2CollisionFlag)
    {
        //ノードの名前を打つ
        CollisionNodeVsEnemies("joint11", specialAttackRadius);
        CollisionNodeVsEnemies("joint120", specialAttackRadius2);


    }
    

}


void Player::CollisionNodeVsEnemies(const char* nodeName, float nodeRadius)
{
    if (!tutoFlag)
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
  
}


//死亡した時に呼ばれる
void Player::OnDead()
{
    //死亡ステートへ遷移
    TransitionDeathState();
}
    