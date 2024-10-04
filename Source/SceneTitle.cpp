﻿#include"Graphics/Graphics.h"
#include"Input/Input.h"
#include "SceneTitle.h"
#include"SceneGame.h"
#include"SceneGame2.h"
#include"SceneGame3.h"
#include"SceneManager.h"
#include"SceneLoading.h"

//初期化
void SceneTitle::Initialize()
{
    
    //スプライト初期化
    sprite = new Sprite("Data/Sprite/Title.png");
}

//終了化
void SceneTitle::Finalize()
{
    //スプライト終了化
    if (sprite != nullptr)
    {
        delete sprite;
        sprite = nullptr;
    }
}

//更新処理
void SceneTitle::Update(float elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    Mouse& mouse = Input::Instance().GetMouse();

    mousePositionX = mouse.GetPositionX();
    mousePositionY = mouse.GetPositionY();

    //何かボタンを押したらローディングシーンをはさんでゲームシーンへ切り替え
    if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
    {
        //キリコ
        if (0 <= mousePositionX && 426 > mousePositionX)
        {
            SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
        }
        //ヒポポタマス
        else if (426 <= mousePositionX && 852 > mousePositionX)
        {
            SceneManager::Instance().ChangeScene(new SceneGame2);        
        }
        //シルバーバック先輩
        else
        {
            SceneManager::Instance().ChangeScene(new SceneGame3);
        }
    }

}

//描画処理
void SceneTitle::Render()
{
    Graphics& graphics = Graphics::Instance();
    ID3D11DeviceContext* dc = graphics.GetDeviceContext();
    ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
    ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

    //画面クリア＆レンダーターゲット設定
    FLOAT color[] = { 0.0f,0.0f,0.5f,1.0f };//RGBA(0.0〜1.0
    dc->ClearRenderTargetView(rtv, color);
    dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    dc->OMSetRenderTargets(1, &rtv, dsv);

    //2Dスプライト描画
    {
        float screenWidth = static_cast<float>(graphics.GetScreenWidth());
        float screenHeight = static_cast<float>(graphics.GetScreenHeight());
        float textureWidth = static_cast<float>(sprite->GetTextureWidth());
        float textureHeight = static_cast<float>(sprite->GetTextureHeight());
        //タイトルスプライト描画
        sprite->Render(dc,
            0, 0, screenWidth, screenHeight,
            0, 0, textureWidth, textureHeight,
            0,
            1, 1, 1, 1);
    }
}
