#pragma once

#include"Graphics/Shader.h"
#include"Graphics/Model.h"
#include"Character.h"
#include"ProjectileManager.h"
#include"Effect.h"
#include"Graphics/Sprite.h"
#include"Audio/AudioSource.h"

class Player :public Character
{
public:
    Player();
    ~Player()override;

    //インスタンス取得
    static Player& Instance();

    //更新処理
    void Update(float elapsedTime);

    //描画処理
    void Render(ID3D11DeviceContext* dc, Shader* sharder);

    //デバッグ用GUI描画
    void DrawDebugGUI();

    //デバッグプリミティブ描画
    void DrawDebugPrimitive();

    const bool& GetSpecialAttack()const { return specialAttack; }

    void SetSpecialAttack(const bool& specialAttack) { this->specialAttack = specialAttack; }

    const bool& GetKirikoUp()const { return kirikoUp; }

    bool SetKirikoUp(const bool& kirikoUp) { this->kirikoUp; }

    const bool& GetspecialAttack()const { return specialAttack; }

    bool SetspecialAttack(const bool& specialAttack)const { this->specialAttack; }

    //回避入力処理
    bool Inputavoidance();

    const bool& GetAvoidanceCollisionFlag()const { return avoidanceCollisionFlag; }

    bool SetAvoidanceCollisionFlag(const bool& avoidanceCollisionFlag) { this->avoidanceCollisionFlag; }

    //蹴り入力処理
    bool InputKick();

private:
    //スティック入力値から移動ベクトルを取得
    DirectX::XMFLOAT3 GetMoveVec()const;

    //必殺技入力処理
    bool InputSpecialAttack();

    //移動入力処理
    //void InputMove(float elapsedTime);
    bool InputMove(float elapsedTime);

    //攻撃入力処理
    bool InputAttack();

    //プレイヤーとエネミーとの衝突処理
    void CollisionPlayerVsEnemies();

    //ノードとエネミーの衝突処理
    void CollisionNodeVsEnemies(const char* nodeName, float nodeRadius);

    //弾丸と敵の衝突判定
   // void CollisionProjectilesVsEnemies();


    //待機ステートへ遷移
    void TransitionIdleState();

    //待機ステート更新処理
    void UpdateIdleState(float elapsedTime);

    //移動ステートへ遷移
    void TransitionMoveState();

    //移動ステート更新処理
    void UpdateMoveState(float elapsedTime);
   
    //攻撃ステートへ遷移
    void TransitionAttackState();

    //攻撃ステート更新処理
    void UpdateAttackState(float elapsedTime);  

    //死亡ステートへ遷移
    void TransitionDeathState();

    //死亡ステート更新処理
    void UpdateDeathState(float elapsedTime);

    //回避ステートへ遷移
    void TransitionAvoidanceState();

    //回避ステート更新処理
    void UpdateAvoidanceState(float elapsedTime);

    //蹴りステート
    void TransitionKickState();

    //蹴りの更新処理
    void UpdateKickState(float elapsedTime);

    //必殺技ステート
    void TransitionSpecialAttackState();

    void UpdateSpecialAttackState(float elapsedTime);


protected:
   
    //ダメージを受けたときに呼ばれる
  
    //死亡した時に呼ばれる
    void OnDead()override;


private:
    enum Animation
    {
        Anim_Idle,
        Anim_Move,
        Anim_Kick,
        Anim_Attack,
        Anim_Avoidance,
        Anim_SpecialAttack,
        Anim_Death,
    };

    //ステート
    enum class State
    {
        Idle,
        Move,
        Kick,
        Attack,
        Avoidance,
        SpecialAttack,
        Death,
    };

private:
    Model* model = nullptr;
    ProjectileManager projectileManager;
    float moveSpeed = 5.0f;
    float turnSpeed = DirectX::XMConvertToRadians(720);
    Effect* hitEffect = nullptr;
    State              state = State::Idle;
    float  leftHandRadius = 0.5f;
    bool attackCollisionFlag = false;
    bool specialAttack = false; //必殺技
    float           specialTime = 200.0f;
    float           specialTimeMax = 200.0f;
    bool            kirikoUp = false;
    bool avoidanceCollisionFlag = false;
    float avoidanceRadius = 0.7f;
    bool kickCollisionFlag = false;
    float kickRadius = 3.0f;

    bool specialAttackCollisionFlag = false;
    bool specialAttack2CollisionFlag = false;
    bool specialAttack3CollisionFlag = false;
    bool specialAttack4CollisionFlag = false;
    

    float specialAttackRadius = 0.5f;
    float specialAttackRadius2 = 0.5f;

    float HealthPreservation = 0;
   
    int         avoidanceTime = 60;
    int         avoidanceFlag = true;
    DirectX::XMFLOAT3 AvoidanceMove;
    DirectX::XMFLOAT3 moveVec;
    //音
    std::unique_ptr<AudioSource> punch_Sound;//パンチ
};