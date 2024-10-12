#pragma once

#include"Graphics/Sprite.h"
#include"Scene.h"
#include<thread>

class SceneLoading :public Scene
{
public:
    SceneLoading(Scene* nextScene):nextScene(nextScene){}
    ~SceneLoading()override{}

    //������
    void Initialize()override;

    //�I����
    void Finalize()override;

    //�X�V����
    void Update(float elapsedTime)override;

    //�`�揈��
    void Render()override;

private:
    //���[�f�B���O�X���b�h
    static void LoadingThread(SceneLoading* scene);

private:
    Sprite* sprite = nullptr;
    float angle = 0.0f;
    Scene* nextScene = nullptr;
    std::thread* thread = nullptr;
    float angle0 = 0, angle1 = 0, angle2 = 0, angle3 = 0, angle4 = 0, angle5 = 0, angle6 = 0, angle7 = 0, angle8 = 0;
    float r = 1, g = 1, b = 1, a = 1;
};
