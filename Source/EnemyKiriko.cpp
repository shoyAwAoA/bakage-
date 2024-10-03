#include "EnemyKiriko.h"
#include"Graphics/Graphics.h"
#include"Mathf.h"
#include"Player.h"
#include"Collision.h"
#include"SceneGame.h"

//�O���[�o��
extern bool speak_flag;
extern bool Special;

static EnemyKiriko* instance = nullptr;


EnemyKiriko& EnemyKiriko::Instance()
{
    // TODO: return �X�e�[�g�����g�������ɑ}�����܂�
    return *instance;
}

//�R���X�g���N�^
EnemyKiriko::EnemyKiriko()
{
    model = new Model("Data/Model/Slime/Slime.mdl");

    //���f�����傫���̂ŃX�P�[�����O
    scale.x = scale.y = scale.z = 0.01f;

    //���A�����ݒ�
    radius = 0.5f;
    height = 1.0f;

    //�p�j�X�e�[�g�֑J��
    TransitionWanderState();
}

//�f�X�g���N�^
EnemyKiriko::~EnemyKiriko()
{
    delete model;
}

//�X�V����
void EnemyKiriko::Update(float elapsedTime)
{
    //�X�e�[�g���̍X�V����
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
    Player& player = Player::Instance();


    if (!player.GetSpecialAttack())
    {
        //���͏����X�V
        UpdateVelocity(elapsedTime);
    }


    //���G���ԍX�V
    UpdateInvincibleTimer(elapsedTime);

    //�I�u�W�F�N�g�s����X�V
    UpdateTransform();

    //���f���A�j���[�V�����X�V
    model->UpdateAnimetion(elapsedTime);

    //���f���s��X�V
    model->UpdateTransform(transform);


}
//�`�揈��
void EnemyKiriko::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    shader->Draw(dc, model);


}

//�f�o�b�O�v���~�e�B�u�`��
void EnemyKiriko::DrawDebugPrimitive()
{
    //���N���X�̃f�o�b�O�v���~�e�B�u�`��
    Enemy::DrawDebugPrimitive();

    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    //�꒣��͈͂��f�o�b�O�~���`��
    debugRenderer->DrawCylinder(territoryOrigin, territoryRange, 1.0f,
                                                             DirectX::XMFLOAT4(0, 1, 0, 1));
    //�^�[�Q�b�g�ʒu���f�o�b�O���`��
    debugRenderer->DrawSphere(targetPosition, radius, DirectX::XMFLOAT4(1, 1, 0, 1));

    //���G�͈͂��f�o�b�O�~���`��
    debugRenderer->DrawCylinder(position, searchRange, 1.0f, DirectX::XMFLOAT4(0, 0, 1, 1));

    //�U���͈͂��f�o�b�O�~���`��
    debugRenderer->DrawCylinder(position, attackRange, 1.0f, DirectX::XMFLOAT4(1, 0, 0, 1));

    if (ImGui::Begin("Enemy", nullptr, ImGuiTreeNodeFlags_None))
    {
        //�g�����X�t�H�[��
        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
        {
            //�ʒu
            ImGui::InputFloat3("Position", &position.x);
            //��]
            DirectX::XMFLOAT3 a;
            a.x = DirectX::XMConvertToDegrees(angle.x);
            a.y = DirectX::XMConvertToDegrees(angle.y);
            a.z = DirectX::XMConvertToDegrees(angle.z);
            ImGui::InputFloat3("Angle", &a.x);
            angle.x = DirectX::XMConvertToRadians(a.x);
            angle.y = DirectX::XMConvertToRadians(a.y);
            angle.z = DirectX::XMConvertToRadians(a.z);
            //�X�P�[��
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
//�꒣��ݒ�
void EnemyKiriko::SetTerritory(const DirectX::XMFLOAT3& origin, float range)
{
    territoryOrigin = origin;
    territoryRange = range;
}

//���S�������ɌĂ΂��
void EnemyKiriko::OnDead()
{
    ////���g��j��
    //Destroy();
    Player& player = Player::Instance();
    if (player.GetspecialAttack())
    {
        Special = true;
    }

    //���S�X�e�[�g�֑J��
    TransitionDeathState();

}

//�_���[�W���󂯂��Ƃ��ɌĂ΂��
void EnemyKiriko::OnDamaged()
{
    //�_���[�W�X�e�[�g�֑J��
    Player& player = Player::Instance();
    if (player.GetspecialAttack())
    {
        Special = true;
    }
    TransitionDamageState();
}

//�^�[�Q�b�g�ʒu�������_���ݒ�
void EnemyKiriko::SetRandomTargetPosition()
{
    float angle = Mathf::RandomRange(0.0f, turnSpeed);

    float radius = Mathf::RandomRange(0.0f, territoryRange);

    targetPosition.x = territoryOrigin.x + radius * cos(angle);
    targetPosition.y = territoryOrigin.y;
    targetPosition.z = territoryOrigin.z + radius * sinf(angle);

}

//�ڕW�n�_�ֈړ�
void EnemyKiriko::MoveToTarget(float elapsedTime, float speedRate)
{
    //�^�[�Q�b�g�����ւ̐i�s�x�N�g�����Z�o
    float vx = targetPosition.x - position.x;
    float vz = targetPosition.z - position.z;
    float dist = sqrtf(vx * vx + vz * vz);
    vx /= dist;
    vz /= dist;
    //�ړ�����
    Move(vx, vz, moveSpeed * speedRate);
    Turn(elapsedTime, vx, vz, turnSpeed);
}

//�m�[�h�ƃv���C���[�̏Փˏ���
void EnemyKiriko::CollisionNodeVsPlayer(const char* nodeName, float nodeRadius)
{
    //�m�[�h�̈ʒu�Ɠ����蔻����s��
    Model::Node* node = model->FindNode(nodeName);
    if (node != nullptr)
    {
        //�m�[�h�̃��[���h���W
        DirectX::XMFLOAT3 nodePosition(
            node->worldTransform._41,
            node->worldTransform._42,
            node->worldTransform._43
        );

        //�����蔻��\��
        Graphics::Instance().GetDebugRenderer()->DrawSphere(
            nodePosition, nodeRadius, DirectX::XMFLOAT4(0, 0, 1, 1)
        );

        
        //�v���C���[�Ɠ����蔻��
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
                //�_���[�W��^����
                if (player.ApplyDamage(1, 0.5f))
                {
                    //�G�𐁂���΂��x�N�g�����Z�o
                    DirectX::XMFLOAT3 vec;
                    vec.x = outPosition.x - nodePosition.x;
                    vec.z = outPosition.z - nodePosition.z;
                    float length = sqrtf(vec.x * vec.x + vec.z * vec.z);
                    vec.x /= length;
                    vec.z /= length;

                    //XZ���ʂɐ�����΂��͂�������
                    float power = 10.0f;
                    vec.x *= power;
                    vec.z *= power;
                    //Y�����ɂ��͂�������
                    vec.y = 5.0f;

                    //������΂�
                    player.AddImpulse(vec);
                }
            }
        }
       
    }
}

//�p�j�X�e�[�g�֑J��
void EnemyKiriko::TransitionWanderState()
{
    speak_flag = false;
    state = State::Wander;

    //�ڕW�n�_�ݒ�
    SetRandomTargetPosition();

    //�����A�j���[�V�����Đ�
    model->PlayAnimation(Anim_WalkFWD, true);
}
//�p�j�X�e�[�g�X�V����
void EnemyKiriko::UpdateWanderState(float elapsedTime)
{
    //�ڕW�n�_�܂�XZ���ʂł̋�������
    float vx = targetPosition.x - position.x;
    float vz = targetPosition.z - position.z;
    float distSq = vx * vx + vz * vz;
    if (distSq < radius * radius)
    {
        ////���̖ڕW�n�_
        //SetRandomTargetPosition();

        //�ҋ@�X�e�[�g�֑J��
        TransitionIdleState();
    }

    //�ڕW�n�_�ֈړ�
    MoveToTarget(elapsedTime, 0.5f);

    //�v���C���[���G
    if (SearchPlayer())
    {
        //����������ǐՃX�e�[�g�֑J��
        TransitionPursuitState();
    }

}
//�ҋ@�X�e�[�g�֑J��
void EnemyKiriko::TransitionIdleState()
{
    state = State::Idle;

    speak_flag = false;

    //�^�C�}�[�������_���ݒ�
    stateTimer = Mathf::RandomRange(3.0f, 5.0f);

    //�ҋ@�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_IdleNormal, true);
}

//�ҋ@�X�e�[�g�X�V����
void EnemyKiriko::UpdateIdleState(float elapsedTime)
{
    //�^�C�}�[����
    stateTimer -= elapsedTime;
    if (stateTimer < 0.0f)
    {
        //�p�j�X�e�[�g�֑J��
        TransitionWanderState();
    }

    //�v���C���[���G
    if (SearchPlayer())
    {
        //����������ǐՃX�e�[�g�֑J��
        TransitionPursuitState();
    }

}
//�v���C���[���G
bool EnemyKiriko::SearchPlayer()
{
    //�v���C���[�Ƃ̍��፷���l������3D�ł̋������������
    const  DirectX::XMFLOAT3& playerPosition = Player::Instance().GetPosition();
    float vx = playerPosition.x - position.x;
    float vy = playerPosition.y - position.y;
    float vz = playerPosition.z - position.z;
    float dist = sqrtf(vx * vx + vy * vy + vz * vz);
    if (dist < searchRange)
    {
        float distXZ = sqrtf(vx * vx + vz * vz);
        //�P�ʃx�N�g����
        vx /= distXZ;
        vz /= distXZ;
        //�O���x�N�g��
        float frontX = sinf(angle.y);
        float frontZ = cosf(angle.y);
        //�Q�̃x�N�g���̓��ϒl�őO�㔻��
        float dot = (frontX * vx) + (frontZ * vz);
        if (dot > 0.0f)
        {
            return true;
        }
    }

    return false;
}
//�ǐՃX�e�[�g�֑J��
void EnemyKiriko::TransitionPursuitState()
{
    
    state = State::Pursuit;

    //���b�ԒǐՂ���^�C�}�[�������_���ݒ�
    stateTimer = Mathf::RandomRange(3.0f, 5.0f);

    //�����A�j���[�V�����Đ�
    model->PlayAnimation(Anim_RunFWD, true);
}
//�ǐՃX�e�[�g�X�V����
void EnemyKiriko::UpdatePursuitState(float elapsedTime)
{
    //�ڕW�n�_���v���C���[�ʒu�ɐݒ�
    targetPosition = Player::Instance().GetPosition();

    //�ڕW�n�_�ֈړ�
    MoveToTarget(elapsedTime, 1.0f);

    speak_flag = true;

    //�^�C�}�[����
    stateTimer -= elapsedTime;
    if (stateTimer < 0.0f)
    {
        speak_flag = false;
      
        //�ҋ@�X�e�[�g�֑J��
        TransitionIdleState();
    }

    //�v���C���[�̋߂��ɋ߂Â��ƍU���X�e�[�g�֑J��
    float vx = targetPosition.x - position.x;
    float vy = targetPosition.y - position.y;
    float vz = targetPosition.z - position.z;
    float dist = sqrtf(vx * vx + vy * vy + vz * vz);
    if (dist < attackRange)
    {
        //�U���X�e�[�g�֑J��
        TransitionAttackState();
    }
    
}

//�U���X�e�[�g�֑J��
void EnemyKiriko::TransitionAttackState()
{
    state = State::Attack;

    //�U���A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Attack1, false);
}

//�U���X�e�[�g�X�V����
void EnemyKiriko::UpdateAttackState(float elapsedTime)
{
    //�C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓˏ���������
    float animationTime = model->GetCurrentAnimationSeconds();
    if (animationTime >= 0.1f && animationTime <= 0.35f)
    {
        //�ڋʃm�[�h�ƃv���C���[�̏Փˏ���
        CollisionNodeVsPlayer("EyeBall", 0.2f);
    }

    if (!model->IsPlayAnimation())
    {
        TransitionIdleBattleState();
    }
}

//�퓬�ҋ@�X�e�[�g�֑J��
void EnemyKiriko::TransitionIdleBattleState()
{
    state = State::IdleBattle;

    //���b�ԑҋ@����^�C�}�[�������_���ݒ�
    stateTimer = Mathf::RandomRange(2.0f, 3.0f);

    //�퓬�ҋ@�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_IdleBattle, true);
}

//�퓬�ҋ@�X�e�[�g�X�V����
void EnemyKiriko::UpdateIdleBattleState(float elapsedTime)
{
    //�ڕW�n�_���v���C���[�ʒu�ɐݒ�
    targetPosition = Player::Instance().GetPosition();

    //�^�C�}�[����
    stateTimer -= elapsedTime;
    if (stateTimer < 0.0f)
    {
        //�v���C���[���U���͈͂ɂ����ꍇ�͍U���X�e�[�g�֑J��
        float vx = targetPosition.x - position.x;
        float vy = targetPosition.y - position.y;
        float vz = targetPosition.z - position.z;
        float dist = sqrtf(vx * vx + vy * vy + vz * vz);
        if (dist < attackRange)
        {
            //�U���X�e�[�g�֑J��
            TransitionAttackState();
        }
        else
        {
            //�p�j�X�e�[�g�֑J��
            TransitionWanderState();
        }
    }

    MoveToTarget(elapsedTime, 0.0f);
}
//�_���[�W�X�e�[�g�֑J��
void EnemyKiriko::TransitionDamageState()
{
    state = State::Damage;

    //�_���[�W�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_GetHit, false);
}

//�_���[�W�X�e�[�g�X�V����
void EnemyKiriko::UpdateDamageState(float elapsedTime)
{
    //�_���[�W�A�j���[�V�������I�������퓬�ҋ@�X�e�[�g�֑J��
    if (!model->IsPlayAnimation())
    {
        TransitionIdleBattleState();
    }
}

//���S�X�e�[�g�֑J��
void EnemyKiriko::TransitionDeathState()
{
    state = State::Death;

    //�_���[�W�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Die, false);
}

//���S�X�e�[�g�X�V����
void EnemyKiriko::UpdateDeathState(float elapsedTime)
{
    //�_���[�W�A�j���[�V�������I������玩����j��
    if (!model->IsPlayAnimation())
    {
        Destroy();
    }
}
