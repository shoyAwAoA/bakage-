#pragma once

#include"Graphics/Sprite.h"
#include"Scene.h"
//#include""

//�^�C�g���V�[��
class SceneTyping :public Scene
{
public:
    SceneTyping() {}
    ~SceneTyping()override {}

    //������
    void Initialize()override;

    //�I����
    void Finalize()override;

    //�X�V����
    void Update(float elapsedTime)override;

    //�`�揈��
    void Render()override;

public:
    /* std::unique_ptr<sprite> title_sprite;*/
    /* Microsoft::WRL::ComPtr<Sprite> */


private:
    
};
