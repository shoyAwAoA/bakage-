#include"Graphics/Graphics.h"
#include"Input/Input.h"
#include "SceneSelect.h"
#include"SceneGame.h"
#include"SceneManager.h"
#include"SceneLoading.h"
#include"SceneGame2D.h"
//初期化

//エクスターン
extern int make;
extern int kati;

static SceneGame2D* instance = nullptr;

SceneGame2D& SceneGame2D::Instance()
{
    // TODO: return ステートメントをここに挿入します
    return *instance;
}

void SceneGame2D::Initialize()
{
    
    //スプライトの初期化
    sprite = new Sprite("Data/Sprite/score.png");
    sprite2 = new Sprite("Data/Sprite/kiriko/matome.png");
    state = Quote_State::Quote_0;
}

//終了化
void SceneGame2D::Finalize()
{
    //スプライト終了化
    if (sprite != nullptr)
    {
        delete sprite;
        sprite = nullptr;
    }//スプライト終了化
    if (sprite2 != nullptr)
    {
        delete sprite2;
        sprite2 = nullptr;
    }
}

//更新処理
void SceneGame2D::Update(float elapsedTime)
{

    GamePad& gamePad = Input::Instance().GetGamePad();

    Mouse& mouse = Input::Instance().GetMouse();

    mousePositionX = mouse.GetPositionX();
    mousePositionY = mouse.GetPositionY();
    if (make == 18)
    {
        state = Quote_State::Quote_18;
    }
    if (kati == 19)
    {
        state = Quote_State::Quote_19;
    }
    switch (state)
    {
    case Quote_State::Quote_0:
        if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
        {
            //
            if (0 <= mousePositionX && 1280 > mousePositionX)
            {
                //SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
                state = Quote_State::Quote_1;
            }
        }
        break;
    case Quote_State::Quote_1:
        if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
        {
            //
            if (0 <= mousePositionX && 1280 > mousePositionX)
            {
                //SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
                state = Quote_State::Quote_2;
            }
        }
        break;
    case Quote_State::Quote_2:
        if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
        {
            //
            if (0 <= mousePositionX && 1280 > mousePositionX)
            {
                //SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
                state = Quote_State::Quote_3;
            }
        }
        break;
    case Quote_State::Quote_3:
        if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
        {
            //
            if (0 <= mousePositionX && 1280 > mousePositionX)
            {
                //SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
                state = Quote_State::Quote_4;
            }
        }
        break;
    case Quote_State::Quote_4:
        if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
        {
            //
            if (0 <= mousePositionX && 1280 > mousePositionX)
            {
                //SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
                state = Quote_State::Quote_5;
            }
        }
        break;
    case Quote_State::Quote_5:
        if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
        {
            //
            if (0 <= mousePositionX && 1280 > mousePositionX)
            {
                //SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
                state = Quote_State::Quote_6;
            }

        }    break;
    case Quote_State::Quote_6:
        if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
        {
            //
            if (0 <= mousePositionX && 1280 > mousePositionX)
            {
                //SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
                state = Quote_State::Quote_7;
            }
        }    break;
    case Quote_State::Quote_7:
        if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
        {
            //
            if (0 <= mousePositionX && 1280 > mousePositionX)
            {
                //SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
                state = Quote_State::Quote_8;
            }
        }    break;
    case Quote_State::Quote_8:
        if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
        {
            //
            if (0 <= mousePositionX && 1280 > mousePositionX)
            {
                //SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
                state = Quote_State::Quote_9;
            }
        }    break;
    case Quote_State::Quote_9://選択肢
        if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
        {
            //
            if (0 <= mousePositionX && 1280 > mousePositionX && 600 < mousePositionY && 720 > mousePositionY)
            {
                //SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
                state = Quote_State::Quote_10;
            }
            else if (0 <= mousePositionX && 1280 > mousePositionX && 480 < mousePositionY && 600 > mousePositionY)
            {
                state = Quote_State::Quote_14;
            }
        }    break;
    case Quote_State::Quote_10:
        if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
        {
            //
            if (0 <= mousePositionX && 1280 > mousePositionX)
            {
                //SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
                state = Quote_State::Quote_11;
            }
        }    break;
    case Quote_State::Quote_11:
        if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
        {
            //
            if (0 <= mousePositionX && 1280 > mousePositionX)
            {
                //SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
                state = Quote_State::Quote_12;
            }
        }    break;
    case Quote_State::Quote_12:
        if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
        {
            //
            //if (0 <= mousePositionX && 1280 > mousePositionX)
            //{
            //    SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
            state = Quote_State::Quote_13;
            //}
        }    break;
    case Quote_State::Quote_13://ゲームオーバー
        if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
        {

            if (0 <= mousePositionX && 1280 > mousePositionX)
            {
                SceneManager::Instance().ChangeScene(new SceneLoading(new SceneSelect));
            }
        }    break;
    case Quote_State::Quote_14://正解
        if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
        {
            if (0 <= mousePositionX && 1280 > mousePositionX)
            {
                state = Quote_State::Quote_15;
            }
        }    break;
    case Quote_State::Quote_15:
        if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
        {
            //
            if (0 <= mousePositionX && 1280 > mousePositionX)
            {
                state = Quote_State::Quote_16;
            }
        }    break;
    case Quote_State::Quote_16:
        if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
        {
            state = Quote_State::Quote_17;
        }        break;
    case Quote_State::Quote_17:
    {
        if (0 <= mousePositionX && 1280 > mousePositionX && (mouse.GetButtonDown() & Mouse::BTN_LEFT))
        {
            SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
        }
    }
    break;
    case Quote_State::Quote_18://敗北
    {
        make = 0;
        if (0 <= mousePositionX && 1280 > mousePositionX && (mouse.GetButtonDown() & Mouse::BTN_LEFT))
        {
            SceneManager::Instance().ChangeScene(new SceneLoading(new SceneSelect));
        }
    }
    break;
    case Quote_State::Quote_19://勝ち
    {
        kati = 0;
        if (0 <= mousePositionX && 1280 > mousePositionX && (mouse.GetButtonDown() & Mouse::BTN_LEFT))
        {
            state = Quote_State::Quote_20;
        }
    }
    break;
    case Quote_State::Quote_20:
    {
      
        if (0 <= mousePositionX && 1280 > mousePositionX && (mouse.GetButtonDown() & Mouse::BTN_LEFT))
        {
            state = Quote_State::Quote_21;
        }
    }
    break;
    case Quote_State::Quote_21:
    {
       
        if (0 <= mousePositionX && 1280 > mousePositionX && (mouse.GetButtonDown() & Mouse::BTN_LEFT))
        {
            SceneManager::Instance().ChangeScene(new SceneLoading(new SceneSelect));
        }
    }
    break;
    }
}

//描画処理
void SceneGame2D::Render()
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

        float quote_startX = 30;
        float quote_startY = 30;
        float textureWidth2 = 270;
        float textureHeight2 = 270;


        float posY = 144.92928571f;
        float posY2 = 289.85857142f;
        float gyouScale = 10.0f;
        float gyouScale2 = 40.0f;
        //1行
        float fontSizeX = 2.5f;
        float scale = 5.0f;
        //2行
        float fontSize2X = 5.0f;
        float scale2 = 9.0f;
        //3行
        float fontSize3X = 7.5f;
        float scale3 = 13.0f;
        //4行
        float fontSize4X = 9.0f;
        float scale4 = 17.0f;
        //5行
        float fontSize5X = 11.5f;
        float scale5 = 21.0f;

        //タイトルスプライト描画
        sprite->Render(dc,
            0, 0, screenWidth, screenHeight,
            0, 0, textureWidth, textureHeight,
            0,
            1, 1, 1, 1);


        switch (state)
        {
        case Quote_State::Quote_0://西孔雀羽織学園     1
            sprite2->Render(dc,
                200, 380,//ゲーム画面での描画１
                screenWidth * fontSizeX, screenHeight * gyouScale,
                0, 0,
                textureWidth * scale, textureHeight * scale,//画像で表す範囲？
                0,
                1, 1, 1, 1);
            break;
        case Quote_State::Quote_1://「おれは○○、４行    5
            sprite2->Render(dc,
                0, 380,
                screenWidth * fontSize4X, screenHeight * gyouScale,
                0, posY,
                textureWidth * scale4, textureHeight * scale4,
                0,
                1, 1, 1, 1);
            break;
        case Quote_State::Quote_2://(ここは元女性限定　３行　8
            sprite2->Render(dc,
                0, 380,
                screenWidth * fontSize3X, screenHeight * gyouScale,
                0, posY * 5,
                textureWidth * scale3, textureHeight * scale3,
                0,
                1, 1, 1, 1);
            break;
        case Quote_State::Quote_3://ドン！　1行 9
            sprite2->Render(dc,
                0, 380,
                screenWidth * fontSizeX, screenHeight * gyouScale,
                0, posY * 8,
                textureWidth * scale, textureHeight * scale,
                0,
                1, 1, 1, 1);
            break;
        case Quote_State::Quote_4://「んお！？」 1行 10
            sprite2->Render(dc,
                0, 380,
                screenWidth * fontSizeX, screenHeight * gyouScale,
                0, posY * 9,
                textureWidth * scale, textureHeight * scale,
                0,
                1, 1, 1, 1);
            break;
        case Quote_State::Quote_5://???ちょおっとお  2行　12
            sprite2->Render(dc,
                0, 380,
                screenWidth * fontSize2X, screenHeight * gyouScale,
                0, posY * 10,
                textureWidth * scale2, textureHeight * scale2,
                0,
                1, 1, 1, 1);
            break;
        case Quote_State::Quote_6://ごめんなさ 1行 13
            sprite2->Render(dc,
                0, 380,
                screenWidth * fontSizeX, screenHeight * gyouScale,
                0, posY * 12,
                textureWidth * scale, textureHeight * scale,
                0,
                1, 1, 1, 1);
        break; case Quote_State::Quote_7:// そこにはキリンが  2行 15
            sprite2->Render(dc,
                0, 380,
                screenWidth * fontSize2X, screenHeight * gyouScale,
                0, posY * 13,
                textureWidth * scale2, textureHeight * scale2,
                0,
                1, 1, 1, 1);
        break; case Quote_State::Quote_8://???「なにあんた　3行 18
            sprite2->Render(dc,
                0, 380,
                screenWidth * fontSize3X, screenHeight * gyouScale,
                0, posY * 15,
                textureWidth * scale3, textureHeight * scale3,
                0,
                1, 1, 1, 1);
        break; case Quote_State::Quote_9://選択肢　3 行　21
            sprite2->Render(dc,
                0, 380,
                screenWidth * fontSize3X, screenHeight * gyouScale,
                0, posY * 18,
                textureWidth * scale3, textureHeight * scale3,
                0,
                1, 1, 1, 1);
        break; case Quote_State::Quote_10://2番を選んだ場合 3行　24
            sprite2->Render(dc,
                0, 380,
                screenWidth * fontSize3X, screenHeight * gyouScale,
                0, posY * 21,
                textureWidth * scale3, textureHeight * scale3,
                0,
                1, 1, 1, 1);
        break; case Quote_State::Quote_11://ドゴッ！！ 1行　25
            sprite2->Render(dc,
                0, 380,
                screenWidth * fontSizeX, screenHeight * gyouScale,
                0, posY * 24,
                textureWidth * scale, textureHeight * scale,
                0,
                1, 1, 1, 1);
        break; case Quote_State::Quote_12://俺は頭部に 1行　26
            sprite2->Render(dc,
                0, 380,
                screenWidth * fontSizeX, screenHeight * gyouScale,
                0, posY * 25,
                textureWidth * scale, textureHeight * scale,
                0,
                1, 1, 1, 1);
            break;
        break; case Quote_State::Quote_13://gameover 1行　27
            sprite2->Render(dc,
                0, 380,
                screenWidth * fontSizeX, screenHeight * gyouScale,
                0, posY * 26,
                textureWidth * scale, textureHeight * scale,
                0,
                1, 1, 1, 1);
            break;
        break; case Quote_State::Quote_14://1,??? 5行　32
            sprite2->Render(dc,
                0, 380,
                screenWidth * fontSize5X, screenHeight * gyouScale,
                0, posY * 27+15,
                textureWidth * scale5, textureHeight * scale5,
                0,
                1, 1, 1, 1);
            break;
        break; case Quote_State::Quote_15://俺の名前は 1行　33
            sprite2->Render(dc,
                0, 380,
                screenWidth * fontSizeX, screenHeight * gyouScale,
                0, posY * 32,
                textureWidth * scale, textureHeight * scale,
                0,
                1, 1, 1, 1);
            break;
        break; case Quote_State::Quote_16://キリコ「ふうん、　1行　34
            sprite2->Render(dc,
                0, 380,
                screenWidth * fontSizeX, screenHeight * gyouScale,
                0, posY * 33+15,
                textureWidth * scale, textureHeight * scale,
                0,
                1, 1, 1, 1);
            break;
        break; case Quote_State::Quote_17://「ふんおッ！？！？　1行　35
            sprite2->Render(dc,
                0, 380,
                screenWidth * fontSizeX, screenHeight * gyouScale,
                0, posY * 34,
                textureWidth * scale, textureHeight * scale,
                0,
                1, 1, 1, 1);
            break;
        case Quote_State::Quote_18://敗北　やっぱり首の 1行 37
            sprite2->Render(dc,
                0, 380,
                screenWidth * fontSizeX, screenHeight * gyouScale,
                0, posY * 36,
                textureWidth * scale, textureHeight * scale,
                0,
                1, 1, 1, 1);
            break;
        case Quote_State::Quote_19://勝ち　や、やるじゃない 3行 40
            sprite2->Render(dc,
                0, 380,
                screenWidth * fontSize3X, screenHeight * gyouScale,
                0, posY * 37,
                textureWidth * scale3, textureHeight * scale3,
                0,
                1, 1, 1, 1);
            break;
        case Quote_State::Quote_20://パタッ　1行 41
            sprite2->Render(dc,
                0, 380,
                screenWidth * fontSizeX, screenHeight * gyouScale,
                0, posY * 40,
                textureWidth * scale, textureHeight * scale,
                0,
                1, 1, 1, 1);
            break;
        case Quote_State::Quote_21://こうして　1行 42
            sprite2->Render(dc,
                0, 380,
                screenWidth * fontSizeX, screenHeight * gyouScale,
                0, posY * 41,
                textureWidth * scale, textureHeight * scale,
                0,
                1, 1, 1, 1);
            break;

        }

    }
}