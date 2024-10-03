#include "EnemySlime.h"
#include"Graphics/Graphics.h"
#include"Mathf.h"
#include"Player.h"
#include"Collision.h"
#include"SceneGame.h"

extern bool speak_flag;

static EnemySlime* instance = nullptr;


EnemySlime& EnemySlime::Instance()
{
    // TODO: return ステートメントをここに挿入します
    return *instance;
}

//コンストラクタ
EnemySlime::EnemySlime()
{
    model = new Model("Data/Model/Slime/Slime.mdl");

    //モデルが大きいのでスケーリング
    scale.x = scale.y = scale.z = 0.01f;

    //幅、高さ設定
    radius = 0.5f;
    height = 1.0f;

    //徘徊ステートへ遷移
    TransitionWanderState();
}

//デストラクタ
EnemySlime::~EnemySlime()
{
    delete model;
}

//更新処理
void EnemySlime::Update(float elapsedTime)
{
    //ステート毎の更新処理
    switch (state)
    {
    case State::Wander:
        UpdateWanderState(elapsedTime);
        break;
    case State::Idle:
        UpdateIdleState(elapsedTime);
        break;
    case State::Pursuit:
        UpdatePursuitState(elapsedTime);
        break;
    case State::Attack:
        UpdateAttackState(elapsedTime);
        break;
    case State::IdleBattle:
        UpdateIdleBattleState(elapsedTime);
        break;
    case State::Damage:
        UpdateDamageState(elapsedTime);
        break;
    case State::Death:
        UpdateDeathState(elapsedTime);
        break;
    }


    //速力処理更新
    UpdateVelocity(elapsedTime);

    //無敵時間更新
    UpdateInvincibleTimer(elapsedTime);

    //オブジェクト行列を更新
    UpdateTransform();

    //モデルアニメーション更新
    model->UpdateAnimetion(elapsedTime);

    //モデル行列更新
    model->UpdateTransform(transform);


}
//描画処理
void EnemySlime::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    shader->Draw(dc, model);


}

//デバッグプリミティブ描画
void EnemySlime::DrawDebugPrimitive()
{
    //基底クラスのデバッグプリミティブ描画
    Enemy::DrawDebugPrimitive();

    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    //縄張り範囲をデバッグ円柱描画
    debugRenderer->DrawCylinder(territoryOrigin, territoryRange, 1.0f,
                                                             DirectX::XMFLOAT4(0, 1, 0, 1));
    //ターゲット位置をデバッグ球描画
    debugRenderer->DrawSphere(targetPosition, radius, DirectX::XMFLOAT4(1, 1, 0, 1));

    //索敵範囲をデバッグ円柱描画
    debugRenderer->DrawCylinder(position, searchRange, 1.0f, DirectX::XMFLOAT4(0, 0, 1, 1));

    //攻撃範囲をデバッグ円柱描画
    debugRenderer->DrawCylinder(position, attackRange, 1.0f, DirectX::XMFLOAT4(1, 0, 0, 1));

    if (ImGui::Begin("Enemy", nullptr, ImGuiTreeNodeFlags_None))
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
            ImGui::InputInt("Health", &health);
       //     ImGui::Text(u8"State %s", str.c_str());

        }

        if (ImGui::CollapsingHeader("Velocity", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::InputFloat3("Velocity", &velocity.x);
        }
    }
    ImGui::End();

}
//縄張り設定
void EnemySlime::SetTerritory(const DirectX::XMFLOAT3& origin, float range)
{
    territoryOrigin = origin;
    territoryRange = range;
}

//死亡した時に呼ばれる
void EnemySlime::OnDead()
{
    ////自身を破棄
    //Destroy();

    //死亡ステートへ遷移
    TransitionDeathState();

}

//ダメージを受けたときに呼ばれる
void EnemySlime::OnDamaged()
{
    //ダメージステートへ遷移
    TransitionDamageState();
}

//ターゲット位置をランダム設定
void EnemySlime::SetRandomTargetPosition()
{
    float angle = Mathf::RandomRange(0.0f, turnSpeed);

    float radius = Mathf::RandomRange(0.0f, territoryRange);

    targetPosition.x = territoryOrigin.x + radius * cos(angle);
    targetPosition.y = territoryOrigin.y;
    targetPosition.z = territoryOrigin.z + radius * sinf(angle);

}

//目標地点へ移動
void EnemySlime::MoveToTarget(float elapsedTime, float speedRate)
{
    //ターゲット方向への進行ベクトルを算出
    float vx = targetPosition.x - position.x;
    float vz = targetPosition.z - position.z;
    float dist = sqrtf(vx * vx + vz * vz);
    vx /= dist;
    vz /= dist;
    //移動処理
    Move(vx, vz, moveSpeed * speedRate);
    Turn(elapsedTime, vx, vz, turnSpeed);
}

//ノードとプレイヤーの衝突処理
void EnemySlime::CollisionNodeVsPlayer(const char* nodeName, float nodeRadius)
{
    //ノードの位置と当たり判定を行う
    Model::Node* node = model->FindNode(nodeName);
    if (node != nullptr)
    {
        //ノードのワールド座標
        DirectX::XMFLOAT3 nodePosition(
            node->worldTransform._41,
            node->worldTransform._42,
            node->worldTransform._43
        );

        //当たり判定表示
        Graphics::Instance().GetDebugRenderer()->DrawSphere(
            nodePosition, nodeRadius, DirectX::XMFLOAT4(0, 0, 1, 1)
        );

        
        //プレイヤーと当たり判定
        Player& player = Player::Instance();
        DirectX::XMFLOAT3 outPosition;
        if (!player.GetAvoidanceCollisionFlag())
        {
            if (Collision::IntersectSphereVsCylinder(
                nodePosition,
                nodeRadius,
                player.GetPosition(),
                player.GetRadius(),
                player.GetHeight(),
                outPosition))
            {
                //ダメージを与える
                if (player.ApplyDamage(1, 0.5f))
                {
                    //敵を吹っ飛ばすベクトルを算出
                    DirectX::XMFLOAT3 vec;
                    vec.x = outPosition.x - nodePosition.x;
                    vec.z = outPosition.z - nodePosition.z;
                    float length = sqrtf(vec.x * vec.x + vec.z * vec.z);
                    vec.x /= length;
                    vec.z /= length;

                    //XZ平面に吹っ飛ばす力をかける
                    float power = 10.0f;
                    vec.x *= power;
                    vec.z *= power;
                    //Y方向にも力をかける
                    vec.y = 5.0f;

                    //吹っ飛ばす
                    player.AddImpulse(vec);
                }
            }
        }
       
    }
}

//徘徊ステートへ遷移
void EnemySlime::TransitionWanderState()
{
    speak_flag = false;
    state = State::Wander;

    //目標地点設定
    SetRandomTargetPosition();

    //歩きアニメーション再生
    model->PlayAnimation(Anim_WalkFWD, true);
}
//徘徊ステート更新処理
void EnemySlime::UpdateWanderState(float elapsedTime)
{
    //目標地点までXZ平面での距離判定
    float vx = targetPosition.x - position.x;
    float vz = targetPosition.z - position.z;
    float distSq = vx * vx + vz * vz;
    if (distSq < radius * radius)
    {
        ////次の目標地点
        //SetRandomTargetPosition();

        //待機ステートへ遷移
        TransitionIdleState();
    }

    //目標地点へ移動
    MoveToTarget(elapsedTime, 0.5f);

    //プレイヤー索敵
    if (SearchPlayer())
    {
        //見つかったら追跡ステートへ遷移
        TransitionPursuitState();
    }

}
//待機ステートへ遷移
void EnemySlime::TransitionIdleState()
{
    state = State::Idle;

    speak_flag = false;

    //タイマーをランダム設定
    stateTimer = Mathf::RandomRange(3.0f, 5.0f);

    //待機アニメーション再生
    model->PlayAnimation(Anim_IdleNormal, true);
}

//待機ステート更新処理
void EnemySlime::UpdateIdleState(float elapsedTime)
{
    //タイマー処理
    stateTimer -= elapsedTime;
    if (stateTimer < 0.0f)
    {
        //徘徊ステートへ遷移
        TransitionWanderState();
    }

    //プレイヤー索敵
    if (SearchPlayer())
    {
        //見つかったら追跡ステートへ遷移
        TransitionPursuitState();
    }

}
//プレイヤー索敵
bool EnemySlime::SearchPlayer()
{
    //プレイヤーとの高低差を考慮して3Dでの距離判定をする
    const  DirectX::XMFLOAT3& playerPosition = Player::Instance().GetPosition();
    float vx = playerPosition.x - position.x;
    float vy = playerPosition.y - position.y;
    float vz = playerPosition.z - position.z;
    float dist = sqrtf(vx * vx + vy * vy + vz * vz);
    if (dist < searchRange)
    {
        float distXZ = sqrtf(vx * vx + vz * vz);
        //単位ベクトル化
        vx /= distXZ;
        vz /= distXZ;
        //前方ベクトル
        float frontX = sinf(angle.y);
        float frontZ = cosf(angle.y);
        //２つのベクトルの内積値で前後判定
        float dot = (frontX * vx) + (frontZ * vz);
        if (dot > 0.0f)
        {
            return true;
        }
    }

    return false;
}
//追跡ステートへ遷移
void EnemySlime::TransitionPursuitState()
{
    
    state = State::Pursuit;

    //数秒間追跡するタイマーをランダム設定
    stateTimer = Mathf::RandomRange(3.0f, 5.0f);

    //歩きアニメーション再生
    model->PlayAnimation(Anim_RunFWD, true);
}
//追跡ステート更新処理
void EnemySlime::UpdatePursuitState(float elapsedTime)
{
    //目標地点をプレイヤー位置に設定
    targetPosition = Player::Instance().GetPosition();

    //目標地点へ移動
    MoveToTarget(elapsedTime, 1.0f);

    speak_flag = true;

    //タイマー処理
    stateTimer -= elapsedTime;
    if (stateTimer < 0.0f)
    {
        speak_flag = false;
      
        //待機ステートへ遷移
        TransitionIdleState();
    }

    //プレイヤーの近くに近づくと攻撃ステートへ遷移
    float vx = targetPosition.x - position.x;
    float vy = targetPosition.y - position.y;
    float vz = targetPosition.z - position.z;
    float dist = sqrtf(vx * vx + vy * vy + vz * vz);
    if (dist < attackRange)
    {
        //攻撃ステートへ遷移
        TransitionAttackState();
    }
    
}

//攻撃ステートへ遷移
void EnemySlime::TransitionAttackState()
{
    state = State::Attack;

    //攻撃アニメーション再生
    model->PlayAnimation(Anim_Attack1, false);
}

//攻撃ステート更新処理
void EnemySlime::UpdateAttackState(float elapsedTime)
{
    //任意のアニメーション再生区間でのみ衝突処理をする
    float animationTime = model->GetCurrentAnimationSeconds();
    if (animationTime >= 0.1f && animationTime <= 0.35f)
    {
        //目玉ノードとプレイヤーの衝突処理
        CollisionNodeVsPlayer("EyeBall", 0.2f);
    }

    if (!model->IsPlayAnimation())
    {
        TransitionIdleBattleState();
    }
}

//戦闘待機ステートへ遷移
void EnemySlime::TransitionIdleBattleState()
{
    state = State::IdleBattle;

    //数秒間待機するタイマーをランダム設定
    stateTimer = Mathf::RandomRange(2.0f, 3.0f);

    //戦闘待機アニメーション再生
    model->PlayAnimation(Anim_IdleBattle, true);
}

//戦闘待機ステート更新処理
void EnemySlime::UpdateIdleBattleState(float elapsedTime)
{
    //目標地点をプレイヤー位置に設定
    targetPosition = Player::Instance().GetPosition();

    //タイマー処理
    stateTimer -= elapsedTime;
    if (stateTimer < 0.0f)
    {
        //プレイヤーが攻撃範囲にいた場合は攻撃ステートへ遷移
        float vx = targetPosition.x - position.x;
        float vy = targetPosition.y - position.y;
        float vz = targetPosition.z - position.z;
        float dist = sqrtf(vx * vx + vy * vy + vz * vz);
        if (dist < attackRange)
        {
            //攻撃ステートへ遷移
            TransitionAttackState();
        }
        else
        {
            //徘徊ステートへ遷移
            TransitionWanderState();
        }
    }

    MoveToTarget(elapsedTime, 0.0f);
}
//ダメージステートへ遷移
void EnemySlime::TransitionDamageState()
{
    state = State::Damage;

    //ダメージアニメーション再生
    model->PlayAnimation(Anim_GetHit, false);
}

//ダメージステート更新処理
void EnemySlime::UpdateDamageState(float elapsedTime)
{
    //ダメージアニメーションが終わったら戦闘待機ステートへ遷移
    if (!model->IsPlayAnimation())
    {
        TransitionIdleBattleState();
    }
}

//死亡ステートへ遷移
void EnemySlime::TransitionDeathState()
{
    state = State::Death;

    //ダメージアニメーション再生
    model->PlayAnimation(Anim_Die, false);
}

//死亡ステート更新処理
void EnemySlime::UpdateDeathState(float elapsedTime)
{
    //ダメージアニメーションが終わったら自分を破棄
    if (!model->IsPlayAnimation())
    {
        Destroy();
    }
}
