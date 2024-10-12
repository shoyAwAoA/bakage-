#pragma once
#include"Graphics/Sprite.h"
#include"Scene.h"
#include<thread>
#include"Input/Mouse.h"
#include"Player.h"
#include"CameraController.h"


//シーンゲーム2D
class SceneTutorial :public Scene
{
public:
    SceneTutorial() {}
    ~SceneTutorial()override {}

    static SceneTutorial& Instance();

    //初期化
    void Initialize()override;

    //終了化
    void Finalize()override;

    //更新処理
    void Update(float elapsedTime)override;

    //描画処理
    void Render()override;

    enum Tuto
    {
        Tuto_0,
        Tuto_1,
        Tuto_2,
        Tuto_3,
        Tuto_4,
        Tuto_5,
        Tuto_6,
        Tuto_7,
        Tuto_8,
        Tuto_9,
        Tuto_10,
        Tuto_11,
        Tuto_12,
        Tuto_13,//ゲームオーバー
        Tuto_14,
        Tuto_15,
        Tuto_16,
        Tuto_17,
        Tuto_18,//敗北
        Tuto_19,//勝ち
        Tuto_20,
    };

    enum class Tuto_State
    {
        Tuto_0,
        Tuto_1,
        Tuto_2,
        Tuto_3,
        Tuto_4,
        Tuto_5,
        Tuto_6,
        Tuto_7,
        Tuto_8,
        Tuto_9,
        Tuto_10,
        Tuto_11,
        Tuto_12,
        Tuto_13,
        Tuto_14,
        Tuto_15,
        Tuto_16,
        Tuto_17,
        Tuto_18,
        Tuto_19,
        Tuto_20,
     
    };

private:
    Player* player = nullptr;
    CameraController* cameraController = nullptr;
    Tuto_State state;
    Sprite* sprite;
    float mousePositionX = 0.0f;
    float mousePositionY = 0.0f;
    float angle = 0;
    float stateTimer = 0.0f;
    float r = 1, g = 1, b = 1, a = 1;
};