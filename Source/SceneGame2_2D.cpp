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
    state = Quote_State::Quote_0;
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
}

//更新処理
void SceneGame2_2D::Update(float elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    Mouse& mouse = Input::Instance().GetMouse();

    mousePositionX = mouse.GetPositionX();
    mousePositionY = mouse.GetPositionY();

    if (make == 25) state = Quote_State::Quote_25;
    if (kati == 26) state = Quote_State::Quote_26;

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
        }
        break;

    case Quote_State::Quote_8: // 選択肢の状態
        if (HandleMouseClick(600, 720, Quote_State::Quote_13) ||
            HandleMouseClick(480, 600, Quote_State::Quote_9))
        {
            break;
        }
    case Quote_State::Quote_18:
        if (HandleMouseClick(600, 720, Quote_State::Quote_23) ||
            HandleMouseClick(480, 600, Quote_State::Quote_19))
        {
            break;
        }
        break;

    case Quote_State::Quote_9:
    case Quote_State::Quote_10:
    case Quote_State::Quote_11:
    case Quote_State::Quote_13:
    case Quote_State::Quote_14:
    case Quote_State::Quote_15:
    case Quote_State::Quote_16:
    case Quote_State::Quote_17:
    case Quote_State::Quote_19:
    case Quote_State::Quote_20:
    case Quote_State::Quote_21:
    case Quote_State::Quote_23:
    case Quote_State::Quote_27:
        if (HandleMouseClick(0, 720, state)) {
            IncrementState();  // 状態を次に進める
        }
        break;
    case Quote_State::Quote_25: // 敗北
    {
        make = 0;
        if (HandleMouseClick(0, 720, state)) {
            state = Quote_State::Quote_12;
            // IncrementState();  // 状態を次に進める
        }
    }
    break;

    // Quote_State::Quote_12またはQuote_State::Quote_22になった場合、クリックでQuote_State::Quote_28に遷移
    case Quote_State::Quote_12: // ゲームオーバー
    case Quote_State::Quote_22: // ゲームオーバー
        if (HandleMouseClick(0, 720, Quote_State::Quote_28)) {
            state = Quote_State::Quote_28;
        }
        break;

    case Quote_State::Quote_28: // 最終状態
        if (HandleMouseClick(0, 720, state)) {
            SceneManager::Instance().ChangeScene(new SceneLoading(new SceneSelect));
        }
        break;

    case Quote_State::Quote_24: // ゲーム再スタート
        if (HandleMouseClick(0, 720, state)) {
            stage = 2;
            SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame2));
        }
        break;

    case Quote_State::Quote_26: // 勝利
        if (HandleMouseClick(0, 720, Quote_State::Quote_27)) {
            kati = 0;
            stage = 0;
        }
        break;
    }
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
    float screenWidth = static_cast<float>(graphics.GetScreenWidth());
    float screenHeight = static_cast<float>(graphics.GetScreenHeight());
    float textureWidth = static_cast<float>(sprite->GetTextureWidth());
    float textureHeight = static_cast<float>(sprite->GetTextureHeight());

    // タイトルスプライト描画
    sprite->Render(dc,
        0, 0, screenWidth, screenHeight,
        0, 0, textureWidth, textureHeight,
        0, 1, 1, 1, 1);

    // レンダリングの共通処理を関数化（X, Y 座標を追加）
    auto RenderSprite2 = [&](float x, float y, float fontSizeX, float scale, float posYMultiplier, float offsetY = 0.0f)
    {
        sprite2->Render(dc,
            x, y,  // X座標とY座標を指定可能に
            screenWidth * fontSizeX, screenHeight * 10.0f,  // 横幅と縦幅のスケーリング
            0, 144.92928571f * posYMultiplier + offsetY,  // Y座標スケーリングとオフセット
            textureWidth * scale, textureHeight * scale,  // テクスチャサイズ
            0, 1, 1, 1, 1);  // 色
    };

    // 状態に応じた描画処理
    switch (state)//1=2.5,2=5.0f.3=7.5f,4=9.0f 5,9,13,17
    {
    case Quote_State::Quote_0: RenderSprite2(0, 380, 5.0f, 9.0f, 0); break;  // 指定位置での描画
    case Quote_State::Quote_1: RenderSprite2(0, 380, 2.5f, 5.0f, 2); break; // 指定位置での描画
    case Quote_State::Quote_2: RenderSprite2(0, 380, 2.5f, 5.0f, 3); break;  // 指定位置
    case Quote_State::Quote_3: RenderSprite2(0, 380, 7.5f, 13.0f, 4); break;  // 指定位置
    case Quote_State::Quote_4: RenderSprite2(0, 380, 7.5f, 13.0f, 7); break;  // 指定位置
    case Quote_State::Quote_5: RenderSprite2(0, 380, 5.0f, 9.0f, 10); break;  // 指定位置
    case Quote_State::Quote_6: RenderSprite2(0, 380, 5.0f, 9.0f, 12); break;  // 指定位置
    case Quote_State::Quote_7: RenderSprite2(0, 380, 7.5f, 13.0f, 14); break;  // 指定位置
    case Quote_State::Quote_8: RenderSprite2(0, 380, 11.5f, 21.0f, 17); break;  // 指定位置
    case Quote_State::Quote_9: RenderSprite2(0, 380, 2.5f, 5.0f, 22); break;  // 指定位置
    case Quote_State::Quote_10: RenderSprite2(0, 380, 2.5f, 5.0f, 23); break;  // 指定位置
    case Quote_State::Quote_11: RenderSprite2(0, 380, 2.5f, 5.0f, 24); break;  // 指定位置
    case Quote_State::Quote_12: RenderSprite2(0, 380, 2.5f, 5.0f, 25); break;  // 指定位置
    case Quote_State::Quote_13: RenderSprite2(0, 380, 9.0f, 17.0f, 26); break;  // 指定位置
    case Quote_State::Quote_14: RenderSprite2(0, 380, 2.5f, 5.0f, 30); break;  // 指定位置
    case Quote_State::Quote_15: RenderSprite2(0, 380, 5.0f, 9.0f, 31); break;  // 指定位置
    case Quote_State::Quote_16: RenderSprite2(0, 380, 2.5f, 5.0f, 33); break;  // 指定位置
    case Quote_State::Quote_17: RenderSprite2(0, 380, 2.5f, 5.0f, 34); break;  // 指定位置
    case Quote_State::Quote_18: RenderSprite2(0, 380, 7.5f, 13.0f, 35); break;  // 指定位置
    case Quote_State::Quote_19: RenderSprite2(0, 380, 2.5f, 5.0f, 38); break;  // 指定位置
    case Quote_State::Quote_20: RenderSprite2(0, 380, 2.5f, 5.0f, 39); break;  // 指定位置
    case Quote_State::Quote_21: RenderSprite2(0, 380, 2.5f, 5.0f, 40); break;  // 指定位置
    case Quote_State::Quote_22: RenderSprite2(0, 380, 2.5f, 5.0f, 41); break;  // 指定位置
    case Quote_State::Quote_23: RenderSprite2(0, 380, 11.5f, 21.0f, 42); break;  // 指定位置
    case Quote_State::Quote_24: RenderSprite2(0, 380, 2.5, 5.0f, 47); break;  // 指定位置
    case Quote_State::Quote_25: RenderSprite2(0, 380, 5.0f, 9.0f, 49); break;  // 指定位置
    case Quote_State::Quote_26: RenderSprite2(0, 380, 9.0f, 17.0f, 51); break;  // 指定位置
    case Quote_State::Quote_27: RenderSprite2(0, 380, 2.5f, 5.0f, 55); break;  // 指定位置
    case Quote_State::Quote_28: RenderSprite2(0, 380, 2.5f, 5.0f, 57); break;  // 指定位置
   // case Quote_State::Quote_29: RenderSprite2(0, 380, 2.5f, 5.0f, 59); break;  // 指定位置
    }
}
