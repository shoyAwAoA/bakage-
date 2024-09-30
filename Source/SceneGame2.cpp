#include "SceneGame2.h"
#include"Graphics/Graphics.h"
#include"Input/Input.h"
#include"SceneManager.h"
#include"SceneTitle.h"

void SceneGame2::Initialize()
{
    spr = new Sprite("Data/Sprite/kati.png");
}

void SceneGame2::Finalize()
{
    if (spr != nullptr)
    {
        delete spr;
        spr = nullptr;
    }
}

void SceneGame2::Update(float elapsedTime)
{
    Render();
    GamePad& gamePad = Input::Instance().GetGamePad();
    if (gamePad.GetButtonDown() & GamePad::BTN_X)
    {
        SceneManager::Instance().ChangeScene(new SceneTitle);
    }
}

void SceneGame2::Render()
{
    Graphics& graphics = Graphics::Instance();
    ID3D11DeviceContext* dc = graphics.GetDeviceContext();
    ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
    ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

    //��ʃN���A�������_�[�^�[�Q�b�g�ݒ�
    FLOAT color[] = { 0.0f,0.0f,0.5f,1.0f };
    dc->ClearRenderTargetView(rtv, color);
    dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    dc->OMSetRenderTargets(1, &rtv, dsv);
    //2D�X�v���C�g�`��
    {
        float screenWidth = static_cast<float>(graphics.GetScreenWidth());
        float screenHeight = static_cast<float>(graphics.GetScreenHeight());
        float textureWidth = static_cast<float>(spr->GetTextureWidth());
        float textureHeight = static_cast<float>(spr->GetTextureHeight());
        //�^�C�g���X�v���C�g�`��
        spr->Render(
            dc,
            0, 0,
            screenWidth,
            screenHeight,
            0, 0,
            textureWidth,
            textureHeight,
            0,
            1, 1, 1, 1
        );
    }
}


