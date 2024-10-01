#include "SceneGame3.h"
#include"Graphics/Graphics.h"
#include"Input/Input.h"
#include"SceneManager.h"
#include"SceneTitle.h"

void SceneGame3::Initialize()
{
    spr2 = new Sprite("Data/Sprite/makee.png");
}

void SceneGame3::Finalize()
{
    if (spr2 != nullptr)
    {
        delete spr2;
        spr2 = nullptr;
    }
}

void SceneGame3::Update(float elapsedTime)
{
    Render();
    GamePad& gamePad = Input::Instance().GetGamePad();
    if (gamePad.GetButtonDown() & GamePad::BTN_Y)
    {
        SceneManager::Instance().ChangeScene(new SceneTitle);
    }
}

void SceneGame3::Render()
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
        float textureWidth = static_cast<float>(spr2->GetTextureWidth());
        float textureHeight = static_cast<float>(spr2->GetTextureHeight());
        //�^�C�g���X�v���C�g�`��
        spr2->Render(
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
