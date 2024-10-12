#include"Graphics/Graphics.h"
#include"Input/Input.h"
#include "SceneSelect.h"
#include"SceneGame.h"
#include"SceneManager.h"
#include"SceneLoading.h"
#include"SceneGame2D.h"

//#include <SpriteFont.h>
//#include <SpriteBatch.h>
#include <wrl/client.h> // Microsoft::WRL::ComPtr
#include <d3d11.h>
#include <string>
#include <locale>
#include <codecvt>  // UTF-8/UTF-16変換用
//初期化

//エクスターン
extern int make;
extern int kati;
extern int stage;
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
    sprite2 = new Sprite("Data/Sprite/kiriko/kiriko.png");
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

    if (make == 17) state = Quote_State::Quote_17;
    if (kati == 18) state = Quote_State::Quote_18;

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
        if (state < Quote_State::Quote_21) {  // 状態が最大値を超えないように制限
            state = static_cast<Quote_State>(static_cast<int>(state) + 1);
        }
    };

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

        if (HandleMouseClick(0, 720, state)) {
            IncrementState();  // 状態を次に進める
        }
        break;

    case Quote_State::Quote_8: // 選択肢の状態
        if (HandleMouseClick(600, 720, Quote_State::Quote_9) ||
            HandleMouseClick(480, 600, Quote_State::Quote_13))
        {
            break;
        }
        break;
    case Quote_State::Quote_9:
    case Quote_State::Quote_10:
    case Quote_State::Quote_11:
    {if (HandleMouseClick(0, 720, state))
    {
        state = Quote_State::Quote_12;
    }
    }
    break;
    case Quote_State::Quote_13:
    case Quote_State::Quote_14:
    case Quote_State::Quote_15:
    case Quote_State::Quote_19:
    case Quote_State::Quote_20:
        if (HandleMouseClick(0, 720, state)) {
            IncrementState();  // 状態を次に進める
        }
        break;

    case Quote_State::Quote_17: // 敗北
        if (HandleMouseClick(0, 720, state)) {
            make = 0;
            state = Quote_State::Quote_12;
        }
        break;
    case Quote_State::Quote_12://ゲームオーバー
    {
        if (HandleMouseClick(0, 720, state)) {
            a2_flag = true;


        }
        if (a2_flag)
        {
            a2 -= elapsedTime * 0.5f;  // フェードアウト速度調整
            if (a2 < 0.0f) a2 = 0.0f;  // 最小透明度の制限
            {
                if (a2 == 0)
                {
                    SceneManager::Instance().ChangeScene(new SceneLoading(new SceneSelect));

                }
            }

        }


    }
    break;
    case Quote_State::Quote_21: // 最終状態
        if (HandleMouseClick(0, 720, state)) {
            SceneManager::Instance().ChangeScene(new SceneLoading(new SceneSelect));
        }
        break;
    case Quote_State::Quote_16: // ゲーム再スタート
        if (HandleMouseClick(0, 720, state)) {
            stage = 1;
            SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
        }
        break;

    case Quote_State::Quote_18: // 勝利
        if (HandleMouseClick(0, 720, Quote_State::Quote_19)) {
            kati = 0;
            stage = 0;
        }
        break;
    }
};

//描画処理
void SceneGame2D::Render()
{
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

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
    float screenWidth = static_cast<float>(graphics.GetScreenWidth());
    float screenHeight = static_cast<float>(graphics.GetScreenHeight());
    float textureWidth = static_cast<float>(sprite->GetTextureWidth());
    float textureWidth2 = static_cast<float>(sprite2->GetTextureWidth());
    float textureHeight = static_cast<float>(sprite->GetTextureHeight());
    float textureHeight2 = static_cast<float>(sprite2->GetTextureHeight());

    // タイトルスプライト描画
  /*  sprite->Render(dc,
        0, 0, screenWidth, screenHeight,
        0, 0, textureWidth, textureHeight,
        0, 1, 1, 1, 1); */
 /*   sprite2->Render2(dc,
        0, 0, screenWidth, screenHeight,
        0, 0, textureWidth2, textureHeight2,
        0, 1, 1, 1, 1);*/
    
    // レンダリングの共通処理を関数化（X, Y 座標を追加）


    if (ImGui::Begin("Player", nullptr, ImGuiTreeNodeFlags_None))
    {
        //トランスフォーム
        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
        {
            //位置
            ImGui::SliderFloat("dx", &dx, 0, 1280);
            ImGui::SliderFloat("dy", &dy, 0, 720);
            ImGui::SliderFloat("dw", &dw, 0, 6120);
            ImGui::SliderFloat("sh", &sh, 0, 500);
           // ImGui::SliderFloat("sx", &sx, 0, 1280);
          //  ImGui::SliderFloat("sy", &sy, 0, 720);
          //  ImGui::SliderFloat("sw", &sw, 0, 1280);
          //  ImGui::SliderFloat("sh", &sh, 0, 720);
        }
        ImGui::End();

        // 状態に応じた描画処理
        switch (state)
        {//１行140.5f 100, 4行550 300 3行421.5f 200,2行270 200
        case Quote_State::Quote_0: sprite2->Render(dc, 100,300,800,300,0,0,3300,650,angle, r, g, b, a); break;  // 指定位置での描画
        case Quote_State::Quote_1: sprite2->Render(dc, 100,300,800,200,0,655,3300,421.5f,angle, r, g, b, a); break; // 指定位置での描画
        case Quote_State::Quote_2: sprite2->Render(dc, 100,300,800,100,0,1075,3300,140.5f,angle, r,g,b,a); break;  // 指定位置
        case Quote_State::Quote_3: sprite2->Render(dc, 100,300,800,100,0,1215.5f,3300,140.5f,angle,r,g,b,a);break;//
        case Quote_State::Quote_4: sprite2->Render(dc, 100,300,800,200,0,1361,3300,270,angle,r,g,b,a);break;//
        case Quote_State::Quote_5: sprite2->Render(dc, 100,400,800,100,0,1635,3300,140.5f,angle,r,g,b,a);break;//
        case Quote_State::Quote_6: sprite2->Render(dc, 100,400,800,200,0,1780.5f,3300,270,angle,r,g,b,a);break;//
        case Quote_State::Quote_7: sprite2->Render(dc, 100,400,800,200,0,2055.5f,3300,421.5f,angle,r,g,b,a);break;//
        case Quote_State::Quote_8: sprite2->Render(dc, 100,400,800,200,0,2482,3300,421.5f,angle,r,g,b,a);break;//
        case Quote_State::Quote_9: sprite2->Render(dc, 100,400,800,200,0,2908.5f,3300,270,angle,r,g,b,a);break;//
        case Quote_State::Quote_10:sprite2->Render(dc, 100,400,800,100,0,3178.5,3300,140.5f,angle,r,g,b,a);break;//ドゴ
        case Quote_State::Quote_11:sprite2->Render(dc, 100,400,800,100,0,3332,3300,140.5f,angle,r,g,b,a);break;//土
        case Quote_State::Quote_12:sprite2->Render(dc, 400,250,1200,200,0,3453.0f,3300,140.5f,angle,r,g,b,a2);break;//GAMEOVER
        case Quote_State::Quote_13:sprite2->Render(dc, 100,400,800,300,0,3597.5f,3300,550,angle,r,g,b,a);break;//
        case Quote_State::Quote_14:sprite2->Render(dc, 100,400,800,100,0,4157.5f,3300,138,angle,r,g,b,a);break;//
        case Quote_State::Quote_15:sprite2->Render(dc, 100,400,800,200,0,4300.5f,3300,270,angle,r,g,b,a);break;//
        case Quote_State::Quote_16:sprite2->Render(dc, 100,400,800,100,0,4575.5,3300,138,angle,r,g,b,a);break;//
        case Quote_State::Quote_17:sprite2->Render(dc, 100,400,800,200,0,4718.5f,3300,270,angle,r,g,b,a);break;//
        case Quote_State::Quote_18:sprite2->Render(dc, 100,400,800,300,0,4993.5f,3300,560,angle,r,g,b,a);break;//
        case Quote_State::Quote_19:sprite2->Render(dc, 100,400,800,100,0,5558.5f,3300,138,angle,r,g,b,a);break;//
        case Quote_State::Quote_20:sprite2->Render(dc, 100,400,800,100,0,5701.5f,3300,138,angle,r,g,b,a);break;
        case Quote_State::Quote_21:sprite2->Render(dc, 100,400,800,100,0,5844.5f,3300,138,angle,r,g,b,a); break;  // 指定位置
        }
    }
}