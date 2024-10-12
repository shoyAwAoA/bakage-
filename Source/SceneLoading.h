#pragma once

#include"Graphics/Sprite.h"
#include"Scene.h"
#include<thread>

class SceneLoading :public Scene
{
public:
    SceneLoading(Scene* nextScene):nextScene(nextScene){}
    ~SceneLoading()override{}

    //初期化
    void Initialize()override;

    //終了化
    void Finalize()override;

    //更新処理
    void Update(float elapsedTime)override;

    //描画処理
    void Render()override;

private:
    //ローディングスレッド
    static void LoadingThread(SceneLoading* scene);

private:
    Sprite* sprite = nullptr;
    float angle = 0.0f;
    Scene* nextScene = nullptr;
    std::thread* thread = nullptr;
    float angle0 = 0, angle1 = 0, angle2 = 0, angle3 = 0, angle4 = 0, angle5 = 0, angle6 = 0, angle7 = 0, angle8 = 0;
    float r = 1, g = 1, b = 1, a = 1;
};
