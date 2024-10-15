#include "Graphics/Graphics.h"
#include "Input/Input.h"
#include "SceneTitle.h"
#include "SceneGame.h"
#include "SceneGame2.h"
#include "SceneGame3.h"
#include "SceneManager.h"
#include "SceneLoading.h"
#include "SceneSelect.h"
#include "SceneTutorial.h"
#include "SceneTyping.h"
using namespace std;

float alpha = 1.0f;  // アルファ値の初期化（透明度）
bool isFadingOut = false;  // フェードアウト状態の管理

// 初期化
void SceneTitle::Initialize()
{
    // スプライト初期化
    sprite = new Sprite("Data/Sprite/Title.png");
    nextScene = 0;
}

// 終了化
void SceneTitle::Finalize()
{
    // スプライト終了化
    if (sprite != nullptr)
    {
        delete sprite;
        sprite = nullptr;
    }
}

// 更新処理
void SceneTitle::Update(float elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    Mouse& mouse = Input::Instance().GetMouse();

    mousePositionX = mouse.GetPositionX();
    mousePositionY = mouse.GetPositionY();

    // フェードイン処理
    if (!isFadingOut)
    {
        if (alpha < 1.0f) {
            alpha += elapsedTime * 0.25f;  // フェードイン速度調整
            if (alpha > 1.0f) alpha = 1.0f;  // 最大透明度の制限
        }
    }

    // クリックでフェードアウト開始
    if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
    {
        if (0 <= mousePositionX && 1280 > mousePositionX && alpha == 1.0f)
        {
            nextScene = 2;
            isFadingOut = true;
        }
        if (0 <= mousePositionX && 1280 > mousePositionX &&300>mousePositionY&& alpha == 1.0f)
        {
            nextScene = 1;
            isFadingOut = true;
        }
    }

    // フェードアウト処理
    if (isFadingOut)
    {
        alpha -= elapsedTime * 0.5f;  // フェードアウト速度調整
        if (alpha < 0.0f) alpha = 0.0f;  // 最小透明度の制限

        // フェードアウト完了後、次のシーンへ遷移
        if (alpha == 0.0f&&nextScene==1)
        {
            
            //SceneManager::Instance().ChangeScene(new SceneLoading(new SceneTyping));
            SceneManager::Instance().ChangeScene(new SceneLoading(new SceneTutorial));
        }
        if (alpha == 0.0f&&nextScene==2)
        {
            SceneManager::Instance().ChangeScene(new SceneLoading(new SceneSelect));
        }
    }
}

// 描画処理
void SceneTitle::Render()
{
    Graphics& graphics = Graphics::Instance();
    ID3D11DeviceContext* dc = graphics.GetDeviceContext();
    ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
    ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

    // 画面クリア＆レンダーターゲット設定
    FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f }; // RGBA (0.0〜1.0)
    dc->ClearRenderTargetView(rtv, color);
    dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    dc->OMSetRenderTargets(1, &rtv, dsv);

    // 2Dスプライト描画
    {
        float screenWidth = static_cast<float>(graphics.GetScreenWidth());
        float screenHeight = static_cast<float>(graphics.GetScreenHeight());
        float textureWidth = static_cast<float>(sprite->GetTextureWidth());
        float textureHeight = static_cast<float>(sprite->GetTextureHeight());

        // タイトルスプライト描画（アルファ値を使用）
        sprite->Render(dc,
            0, 0, screenWidth, screenHeight,
            0, 0, textureWidth, textureHeight,
            0,    // 回転角度
            1, 1, 1, alpha);  // RGBAのアルファ値で透明度を制御
    }
}
