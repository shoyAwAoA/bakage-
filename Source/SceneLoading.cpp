#include"Graphics/Graphics.h"
#include"Input/Input.h"
#include "SceneLoading.h"
#include"SceneManager.h"
#include "SceneTyping.h"


//������
void SceneLoading::Initialize()
{
    //�X�v���C�g������
   // sprite = new Sprite("Data/Sprite/LoadingIcon.png");
    sprite = new Sprite("Data/Sprite/loading/taitle.png");

    //�X���b�h�J�n
    thread = new std::thread(LoadingThread, this);
}

//�I����
void SceneLoading::Finalize()
{
    //�X���b�h�I����
    if (thread != nullptr)
    {
        thread->join();
        delete thread;
        thread = nullptr;
    }

    //�X�v���C�g������
    if (sprite != nullptr)
    {
        delete sprite;
        sprite = nullptr;
    }

}

//�X�V����
void SceneLoading::Update(float elapsedTime)
{
   
    constexpr float speed = 100;
    angle0 += speed * elapsedTime;
    angle1 -= speed * elapsedTime;
    angle2 += speed * elapsedTime;
    angle3 -= speed * elapsedTime;
    angle4 += speed * elapsedTime;
    angle5 -= speed * elapsedTime;
    angle6 += speed * elapsedTime;
    angle7 -= speed * elapsedTime;
    angle8 += speed * elapsedTime;
    

    

    //���̃V�[���̏���������������V�[����؂�ւ���
    if (nextScene->IsReady())
    {
        SceneManager::Instance().ChangeScene(nextScene);
        nextScene = nullptr;
    }

}

//�`�揈��
void SceneLoading::Render()
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
        //��ʉE���Ƀ��[�f�B���O�A�C�R����`��
        float screenWidth = static_cast<float>(graphics.GetScreenWidth());
        float screenHeight = static_cast<float>(graphics.GetScreenHeight());
        float textureWidth = static_cast<float>(sprite->GetTextureWidth());
        float textureHeight = static_cast<float>(sprite->GetTextureHeight());
        float positionX = screenWidth - textureWidth;
        float positionY = screenHeight - textureHeight;

        sprite->Render(dc, 0,300, 142.2222f, 200, 0, 0, 122.222222f, 200, angle0, r, g, b, a);
        sprite->Render(dc, 142.2222f*1,300, 142.2222f, 200, 122.222222f, 0, 122.222222f, 200, angle1, r, g, b, a);
        sprite->Render(dc, 142.2222f*2,300, 142.2222f, 200, 122.222222f*2, 0, 122.222222f, 200, angle2, r, g, b, a);
        sprite->Render(dc, 142.2222f*3,300, 142.2222f, 200, 122.222222f*3, 0, 122.222222f, 200, angle3, r, g, b, a);
        sprite->Render(dc, 142.2222f*4,300, 142.2222f, 200, 122.222222f*4, 0, 122.222222f, 200, angle4, r, g, b, a);
        sprite->Render(dc, 142.2222f*5,300, 142.2222f, 200, 122.222222f*5, 0, 122.222222f, 200, angle5, r, g, b, a);
        sprite->Render(dc, 142.2222f*6,300, 142.2222f, 200, 122.222222f*6, 0, 122.222222f, 200, angle6, r, g, b, a);
        sprite->Render(dc, 142.2222f*7,300, 142.2222f, 200, 122.222222f*7, 0, 122.222222f, 200, angle7, r, g, b, a);
        sprite->Render(dc, 142.2222f*8,300, 142.2222f, 200, 122.222222f*8, 0, 122.222222f, 200, angle8, r, g, b, a);
    }
}

//���[�f�B���O�X���b�h
void SceneLoading::LoadingThread(SceneLoading* scene)
{
    //COM�֘A�̏������ŃX���b�h���ɌĂԕK�v������
    CoInitialize(nullptr);

    //���̃V�[���̏��������s��
    scene->nextScene->Initialize();
    


    //�X���b�h���I���O��COM�֘A�̏I����
    CoUninitialize();

    //���̃V�[���̏��������ݒ�
    scene->nextScene->SetReady();


}
