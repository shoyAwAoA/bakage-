#pragma once
#include"Graphics/Sprite.h"
#include"Scene.h"
#include<thread>
#include"Input/Mouse.h"


//シーンゲーム2D
class SceneGame2D :public Scene
{
public:
    SceneGame2D() {}
    ~SceneGame2D()override {}

    static SceneGame2D& Instance();

    //初期化
    void Initialize()override;

    //終了化
    void Finalize()override;

    //更新処理
    void Update(float elapsedTime)override;

    //描画処理
    void Render()override;

    enum Quote
    {
        Quote_0,
        Quote_1,
        Quote_2,
        Quote_3,
        Quote_4,
        Quote_5,
        Quote_6,
        Quote_7,
        Quote_8,
        Quote_9,
        Quote_10,
        Quote_11,
        Quote_12,
        Quote_13,//ゲームオーバー
        Quote_14,
        Quote_15,
        Quote_16,
        Quote_17,
        Quote_18,//敗北
        Quote_19,//勝ち
        Quote_20,
        Quote_21,
    };

private:

    enum class Quote_State
    {
        Quote_0,
        Quote_1,
        Quote_2,
        Quote_3,
        Quote_4,
        Quote_5,
        Quote_6,
        Quote_7,
        Quote_8,
        Quote_9,
        Quote_10,
        Quote_11,
        Quote_12,
        Quote_13,//ゲームオーバー
        Quote_14,
        Quote_15,
        Quote_16,
        Quote_17,
        Quote_18,//敗北
        Quote_19,//勝ち
        Quote_20,
        Quote_21,
    };
public:
    int stateGet;
private:
    float mousePositionX = 0.0f;
    float mousePositionY = 0.0f;
    float dx = 0, dy = 0;
    float dw = 2700, dh = 300;
    float r=1, g=1, b=1, a=1;
    float a2 = 0;
    float a3 = 1;
    bool a2_flag = false;
    bool clickFlag = false;
    int hidariTimer = 0;
    float angle = 0;
    float sh=215.35f;
    Sprite* sprite2 = nullptr;
    Sprite* sprite = nullptr;
    Sprite* skip = nullptr;
    Sprite* hidari=nullptr;
    Quote_State state;
    bool plusFlag=false;
    bool minasuFlag=false;
};