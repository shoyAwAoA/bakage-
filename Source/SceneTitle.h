#pragma once

#include"Graphics/Sprite.h"
#include"Scene.h"
//#include""

//�^�C�g���V�[��
class SceneTitle :public Scene
{
public:
    SceneTitle() {}
    ~SceneTitle()override {}

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
    Sprite* sprite = nullptr;
    float mousePositionX = 0.0f;
    float mousePositionY = 0.0f;
};
