#include"Graphics/Graphics.h"
#include"Input/Input.h"
#include "SceneTitle.h"
#include"SceneGame.h"
#include"SceneGame2.h"
#include"SceneGame3.h"
#include"SceneManager.h"
#include"SceneLoading.h"
#include"SceneSelect.h"
using namespace std;


//初期化
void SceneTitle::Initialize()
{
    //Direct3Dの初期化


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

    //何かボタンを押したらローディングシーンをはさんでセレクへ切り替え
    if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
    {
        if (0 <= mousePositionX && 1280 > mousePositionX)
        {
            SceneManager::Instance().ChangeScene(new SceneLoading(new SceneSelect));
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

//void DisplayJapaneseText()
//{
//    string japaneseText = "日本語のテキスト";
//    int xPosition = 10;
//    int yPosition = 30;
//
//    using (Font font = new Font("Arial Unicode MS", 12))
//        using (Brush brush = new SolidBrush(Color.Black))
//    {
//        e.Graphics.DrawString(japaneseText, font, brush, new PointF(xPosition, yPosition));
//    };
//}