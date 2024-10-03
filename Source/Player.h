#pragma once

#include"Graphics/Shader.h"
#include"Graphics/Model.h"
#include"Character.h"
#include"ProjectileManager.h"
#include"Effect.h"
#include"Graphics/Sprite.h"
class Player :public Character
{
public:
    Player();
    ~Player()override;

    //�C���X�^���X�擾
    static Player& Instance();

    //�X�V����
    void Update(float elapsedTime);

    //�`�揈��
    void Render(ID3D11DeviceContext* dc, Shader* sharder);

    //�f�o�b�O�pGUI�`��
    void DrawDebugGUI();

    //�f�o�b�O�v���~�e�B�u�`��
    void DrawDebugPrimitive();

    const bool& GetSpecialAttack()const { return specialAttack; }

    void SetSpecialAttack(const bool& specialAttack) { this->specialAttack = specialAttack; }

    const bool& GetKirikoUp()const { return kirikoUp; }

    bool SetKirikoUp(const bool& kirikoUp) { this->kirikoUp; }

    //void InputJump();
    bool InputJump();

    //�����͏���
    bool Inputavoidance();

    const bool& GetAvoidanceCollisionFlag()const { return avoidanceCollisionFlag; }

    bool SetAvoidanceCollisionFlag(const bool& avoidanceCollisionFlag) { this->avoidanceCollisionFlag; }

    //�R����͏���
    bool InputKick();



private:
    //�X�e�B�b�N���͒l����ړ��x�N�g�����擾
    DirectX::XMFLOAT3 GetMoveVec()const;

    //�K�E�Z���͏���
    bool InputSpecialAttack();

    //�ړ����͏���
    //void InputMove(float elapsedTime);
    bool InputMove(float elapsedTime);

    //�U�����͏���
    bool InputAttack();

    //�v���C���[�ƃG�l�~�[�Ƃ̏Փˏ���
    void CollisionPlayerVsEnemies();

    //�m�[�h�ƃG�l�~�[�̏Փˏ���
    void CollisionNodeVsEnemies(const char* nodeName, float nodeRadius);

    //�e�ۂƓG�̏Փ˔���
    void CollisionProjectilesVsEnemies();

    //�e�ۓ��͏���
    void InputSpecial();

    //�ҋ@�X�e�[�g�֑J��
    void TransitionIdleState();

    //�ҋ@�X�e�[�g�X�V����
    void UpdateIdleState(float elapsedTime);

    //�ړ��X�e�[�g�֑J��
    void TransitionMoveState();

    //�ړ��X�e�[�g�X�V����
    void UpdateMoveState(float elapsedTime);

    //�W�����v�X�e�[�g�֑J��
    void TransitionJumpState();

    //�W�����v�X�e�[�g�X�V����
    void UpdateJumpState(float elapsedTime);

    //���n�X�e�[�g�֑J��
    void TransitionLandState();

    //���n�X�e�[�g�X�V����
    void UpdateLandState(float elapsedTime);

    //�U���X�e�[�g�֑J��
    void TransitionAttackState();

    //�U���X�e�[�g�X�V����
    void UpdateAttackState(float elapsedTime);

    //�_���[�W�X�e�[�g�֑J��
    void TransitionDamageState();

    //�_���[�W�X�e�[�g�X�V����
    void UpdateDamageState(float elapsedTime);

    //���S�X�e�[�g�֑J��
    void TransitionDeathState();

    //���S�X�e�[�g�X�V����
    void UpdateDeathState(float elapsedTime);

    //�����X�e�[�g�֑J��
    void TransitionReviveState();

    //�����X�e�[�g�X�V����
    void UpdateReviveState(float elapsedTime);

    //����X�e�[�g�֑J��
    void TransitionAvoidanceState();

    //����X�e�[�g�X�V����
    void UpdateAvoidanceState(float elapsedTime);

    //�R��X�e�[�g
    void TransitionKickState();

    //�R��̍X�V����
    void UpdateKickState(float elapsedTime);

    //�K�E�Z�X�e�[�g
    void TransitionSpecialAttackState();

    void UpdateSpecialAttackState(float elapsedTime);


protected:
    //���n�����Ƃ��ɌĂ΂��
    void OnLanding()override;

    //�_���[�W���󂯂��Ƃ��ɌĂ΂��
    void OnDamaged()override;

    //���S�������ɌĂ΂��
    void OnDead()override;


private:
    enum Animation
    {
        Anim_Attack,
        Anim_Death,
        Anim_Falling,
        Anim_GetHit1,
        Anim_GetHit2,
        Anim_Idle,
        Anim_Jump,
        Anim_Jump_Flip,
        Anim_Landing,
        Anim_Revive,
        Anim_Running,
        Anim_Walking,
        Anim_Avoidance,
        Anim_SpecialAttack,
    };

    //�X�e�[�g
    enum class State
    {
        Idle,
        Move,
        Jump,
        Land,
        Attack,
        Damage,
        Death,
        Revive,
        Avoidance,
        Kick,
        SpecialAttack,
    };

private:
    Model* model = nullptr;
    ProjectileManager projectileManager;
    float moveSpeed = 5.0f;
    float turnSpeed = DirectX::XMConvertToRadians(720);
    float jumpSpeed = 20.0f;
    int jumpCount = 0;
    int jumpLimit = 2;
    Effect* hitEffect = nullptr;
    State              state = State::Idle;
    float  leftHandRadius = 0.7f;
    bool attackCollisionFlag = false;
    bool specialAttack = false; //�K�E�Z
    float           specialTime = 200.0f;
    float           specialTimeMax = 200.0f;
    bool            kirikoUp = false;
    bool avoidanceCollisionFlag = false;
    float avoidanceRadius = 0.7f;
    bool kickCollisionFlag = false;
    float kickRadius = 1.0f;
    bool specialAttackCollisionFlag = false;
    float specialAttackRadius = 1.0f;

    float HealthPreservation = 0;
    bool Special_Hit_Check = false;
};