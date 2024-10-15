#include"Graphics/Graphics.h"
#include"Input/Input.h"
#include "SceneSelect.h"
#include"SceneGame2.h"
#include"SceneManager.h"
#include"SceneLoading.h"
#include"SceneGame2_2D.h"
//初期化

//エクスターン
extern int make;
extern int kati;
extern int stage;
extern bool hipopoSkip;
static SceneGame2_2D* instance = nullptr;

SceneGame2_2D& SceneGame2_2D::Instance()
{
    // TODO: return ステートメントをここに挿入します
    return *instance;
}

void SceneGame2_2D::Initialize()
{

    //スプライトの初期化
    sprite = new Sprite("Data/Sprite/score.png");
    sprite2 = new Sprite("Data/Sprite/hipopo/matome.png");
    skip = new Sprite("Data/Sprite/skip.png");
    hidari = new Sprite("Data/Sprite/hidaku.png");
    state = Quote_State::Quote_0;
    minasuFlag = false;
    plusFlag = false;
    clickFlag = false;
    hidariTimer = 0;
}

//終了化
void SceneGame2_2D::Finalize()
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
    if (skip != nullptr)
    {
        delete skip;
        skip = nullptr;
    }
    if (hidari != nullptr)
    {
        delete hidari;
        hidari = nullptr;
    }
}

//更新処理
void SceneGame2_2D::Update(float elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    Mouse& mouse = Input::Instance().GetMouse();

    mousePositionX = mouse.GetPositionX();
    mousePositionY = mouse.GetPositionY();

    if (make == 23) state = Quote_State::Quote_23;
    if (kati == 24) state = Quote_State::Quote_24;

    // マウスクリックをX座標内で処理するヘルパー関数
    auto HandleMouseClick = [&](int minY, int maxY, Quote_State nextState) {
        if ((mouse.GetButtonDown() & Mouse::BTN_LEFT) &&
            mousePositionX >= 0 && mousePositionX < 1280 &&
            mousePositionY >= minY && mousePositionY < maxY)
        {
            state = nextState;
            return true;
        }
        return false;
    };

    // `state + 1` が `Quote_State` の範囲内であるかを確認しつつキャスト
    auto IncrementState = [&]() {
        if (state < Quote_State::Quote_29) {  // 状態が最大値を超えないように制限
            state = static_cast<Quote_State>(static_cast<int>(state) + 1);
        }
    };

    //マウスクリック
    auto HandleMouseClickSkip = [&](int minX, int maxX, int minY, int maxY, Quote_State nextState) {
        if ((mouse.GetButtonDown() & Mouse::BTN_LEFT) &&
            mousePositionX >= minX && mousePositionX < maxX &&
            mousePositionY >= minY && mousePositionY < maxY)
        {
            state = nextState;
            return true;
        }
        return false;
    };

    {
        if (hipopoSkip)
        {
            if(HandleMouseClickSkip(900, 1280, 0, 200, state))
            {
               state = Quote_State::Quote_22;
            }
        }
    }

    if (hidariTimer >= 500)
    {
        clickFlag = true;
    }
    else
    {
        clickFlag = false;
    }

    switch (state)
    {
    case Quote_State::Quote_0:
    case Quote_State::Quote_1:
    case Quote_State::Quote_2:
    case Quote_State::Quote_3:
    case Quote_State::Quote_4:
    case Quote_State::Quote_5:
    case Quote_State::Quote_6:
    case Quote_State::Quote_7:
        //  case Quote_State::Quote_8:
        if (HandleMouseClick(0, 720, state)) {
            IncrementState();  // 状態を次に進める
            hidariTimer=0;
        }
        break;

    case Quote_State::Quote_8: // 選択肢の状態
        if (HandleMouseClick(600, 720, Quote_State::Quote_12) ||
            HandleMouseClick(480, 600, Quote_State::Quote_9))
        {
            hidariTimer = 0;
            break;
        }
    case Quote_State::Quote_17:
        if (HandleMouseClick(600, 720, Quote_State::Quote_21) ||
            HandleMouseClick(480, 600, Quote_State::Quote_18))
        {
            hidariTimer = 0;
            break;
        }
        break;

    case Quote_State::Quote_9:
    case Quote_State::Quote_10:
    case Quote_State::Quote_12:
    case Quote_State::Quote_13:
    case Quote_State::Quote_14:
    case Quote_State::Quote_15:
    case Quote_State::Quote_16:
    case Quote_State::Quote_18:
    case Quote_State::Quote_19:
    case Quote_State::Quote_21:
    case Quote_State::Quote_25:
    case Quote_State::Quote_26:
    
        if (HandleMouseClick(0, 720, state)) {
            IncrementState();  // 状態を次に進める
            hidariTimer = 0;
        }
        break;
    case Quote_State::Quote_23: // 敗北
    {
        make = 0;
            hipopoSkip = true;
        if (HandleMouseClick(0, 720, state)) {
            state = Quote_State::Quote_11;
            hidariTimer = 0;
            // IncrementState();  // 状態を次に進める
        }
    }
    break;

    // Quote_State::Quote_12またはQuote_State::Quote_22になった場合、クリックでQuote_State::Quote_28に遷移
    case Quote_State::Quote_11: // ゲームオーバー
    case Quote_State::Quote_20: // ゲームオーバー
       // if (HandleMouseClick(0, 720, state)) {
            a2_flag = true;
        if (a2_flag)
        {
            a2 += elapsedTime * 0.35f;  // フェードアウト速度調整
            if (a2 < 0.0f) a2 = 0.0f;  // 最小透明度の制限
            {
                if (a2 >= 1)
                {
                    // SceneManager::Instance().ChangeScene(new SceneLoading(new SceneSelect));
                    state = Quote_State::Quote_27;
                }
            }
       }
        break;

    case Quote_State::Quote_27: // 最終状態
        if (HandleMouseClick(0, 720, state)) {
            a2 = 1;
            a2_flag = false;
            SceneManager::Instance().ChangeScene(new SceneLoading(new SceneSelect));
            hidariTimer = 0;
        }
        break;

    case Quote_State::Quote_22: // ゲーム再スタート
        if (HandleMouseClick(0, 720, state)) {
            stage = 2;
            SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame2));
            hidariTimer = 0;
        }
        break;

    case Quote_State::Quote_24: // 勝利
        if (HandleMouseClick(0, 720, Quote_State::Quote_25)) {
            kati = 0;
            stage = 0;
            hipopoSkip = false;
            hidariTimer = 0;
        }
        break;
    }

    if (clickFlag)
    {
        if (a3 >= 1.0f)
        {
            plusFlag = false;
            minasuFlag = true;
        }
        else if (a3 <= 0.0f)
        {
            minasuFlag = false;
            plusFlag = true;
        }
        if (minasuFlag)
        {
            a3 -= elapsedTime * 0.75f;
        }
        if (plusFlag)
        {
            a3 += elapsedTime * 0.75f;
        }
    }
    if (!clickFlag)
    {
        a3 = 1;
        plusFlag = false;
        minasuFlag = false;
    }
    hidariTimer +=1;
}
//描画処理
void SceneGame2_2D::Render()
{
    Graphics& graphics = Graphics::Instance();
    ID3D11DeviceContext* dc = graphics.GetDeviceContext();
    ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
    ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

    // 画面クリア＆レンダーターゲット設定
    FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };
    dc->ClearRenderTargetView(rtv, color);
    dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    dc->OMSetRenderTargets(1, &rtv, dsv);

    // 2Dスプライト描画
   /* float screenWidth = static_cast<float>(graphics.GetScreenWidth());
    float screenHeight = static_cast<float>(graphics.GetScreenHeight());
    float textureWidth = static_cast<float>(sprite->GetTextureWidth());
    float textureHeight = static_cast<float>(sprite->GetTextureHeight());*/

    //11,20、22以外
    if (hipopoSkip&&state >= Quote_State::Quote_0 && state <= Quote_State::Quote_21 && state != Quote_State::Quote_11 && state != Quote_State::Quote_20)
    {
        skip->Render(dc, 1000, 50, 150, 150, 0, 0, 400, 200, angle, r, g, b, a);
    }

    //クリック
    {
        if (clickFlag)
        {
            hidari->Render(dc, 1000, 600, 200, 130, 0, 0, 700, 250, angle, r, g, b, a3);
        }
    }

    // 状態に応じた描画処理
    switch (state)//1=2.5,2=5.0f.3=7.5f,4=9.0f 5,9,13,17
    {//1行　150 150,2行 300,300　3行　430,300    5行 700 400  
    case Quote_State::Quote_0: sprite2->Render(dc,100, 300, 1400,300,0,0,2100,300, angle, r, g, b, a); break; //
    case Quote_State::Quote_1: sprite2->Render(dc,100, 300, 800,150,0,300,2100,150, angle, r, g, b, a); break;  //
    case Quote_State::Quote_2: sprite2->Render(dc,100, 300, 800,150,0,450,2100,150, angle, r, g, b, a); break;// 
    case Quote_State::Quote_3: sprite2->Render(dc,100, 300, 800,300,0,600,2100,430, angle, r, g, b, a); break; // 
    case Quote_State::Quote_4: sprite2->Render(dc,100, 300, 800,300,0,1030,2100,430, angle, r, g, b, a); break;  //
    case Quote_State::Quote_5: sprite2->Render(dc,100, 300, 800,300,0,1460,2100,300, angle, r, g, b, a); break;  //
    case Quote_State::Quote_6: sprite2->Render(dc,100, 300, 800,300,0,1760,2100,280, angle, r, g, b, a); break;  //
    case Quote_State::Quote_7: sprite2->Render(dc,100, 300, 800,300,0,2040,2100,430, angle,r,g,b,a); break;  //
    case Quote_State::Quote_8: sprite2->Render(dc,100, 300, 1200,400,0,2470,2100,700, angle,r,g,b,a); break;  //
    case Quote_State::Quote_9: sprite2->Render(dc,100, 300, 1000,150,0,3350,2100,150, angle, r, g, b, a); break; // 
    case Quote_State::Quote_10:sprite2->Render(dc, 100, 300,1000,150,0,3500,2100,150, angle, r, g, b, a); break; // 
    case Quote_State::Quote_12:sprite2->Render(dc, 100, 300,800,300,0,3920,2100,430, angle, r, g, b, a); break;  //
    case Quote_State::Quote_13:sprite2->Render(dc,100, 300, 800,150,0,4340,2100,150,angle,r,g,b,a ); break;  //
    case Quote_State::Quote_14:sprite2->Render(dc,100, 300, 800,300,0,4490,2100,300,angle,r,g,b,a); break;  //
    case Quote_State::Quote_15:sprite2->Render(dc,100, 300, 800,150,0,4800,2100,150,angle,r,g,b,a); break;  //
    case Quote_State::Quote_16:sprite2->Render(dc,100, 300, 800,150,0,4950,2100,150,angle,r,g,b,a); break;  //
    case Quote_State::Quote_17:sprite2->Render(dc,100, 300, 800,400,0,5100,2100,430,angle,r,g,b,a); break;  //
    case Quote_State::Quote_18:sprite2->Render(dc,100, 300, 800,150,0,5670,2100,150,angle,r,g,b,a); break;  //
    case Quote_State::Quote_19:sprite2->Render(dc,100, 300, 800,150,0,5820,2100,150,angle,r,g,b,a); break;  //
    case Quote_State::Quote_11:sprite2->Render(dc,400, 250, 1600,300,0,3650,2100,150, angle, r, g, b, a2); break; //ゲームオーバー
    case Quote_State::Quote_20:sprite2->Render(dc,400, 250, 1600,300,0,5970,2100,150,angle,r,g,b,a2); break; //ゲームオーバー 
    case Quote_State::Quote_21:sprite2->Render(dc,100, 250, 800,500,0,6250,2100,600,angle,r,g,b,a); break;  //
    case Quote_State::Quote_22:sprite2->Render(dc,100, 300, 800,150,0,6830,2100,150,angle,r,g,b,a); break;  //
    case Quote_State::Quote_23:sprite2->Render(dc,100, 300, 800,300,0,7130,2100,280,angle,r,g,b,a ); break; //
    case Quote_State::Quote_24:sprite2->Render(dc,100, 250, 800,500,0,7410,2100,600,angle,r,g,b,a ); break; //
    case Quote_State::Quote_25:sprite2->Render(dc,100, 300, 800,150,0,8000,2100,140,angle,r,g,b,a ); break; //
    case Quote_State::Quote_26:sprite2->Render(dc,100, 300, 800,150,0,8140,2100,150,angle,r,g,b,a ); break;// 
    case Quote_State::Quote_27:sprite2->Render(dc,400, 300, 1600,150,0,8290,2100,150,angle,r,g,b,a); break;  //
   
   // case Quote_State::Quote_29: RenderSprite2(0, 380, 2.5f, 5.0f, 59); break;  // 指定位置
    }
}
