#include"Graphics/Graphics.h"
#include"Input/Input.h"
#include "SceneSelect.h"
#include"SceneGame2.h"
#include"SceneManager.h"
#include"SceneLoading.h"
#include"SceneGame2_2D.h"
//������

//�G�N�X�^�[��
extern int make;
extern int kati;
extern int stage;
static SceneGame2_2D* instance = nullptr;

SceneGame2_2D& SceneGame2_2D::Instance()
{
    // TODO: return �X�e�[�g�����g�������ɑ}�����܂�
    return *instance;
}

void SceneGame2_2D::Initialize()
{

    //�X�v���C�g�̏�����
    sprite = new Sprite("Data/Sprite/score.png");
    sprite2 = new Sprite("Data/Sprite/hipopo/matome.png");
    state = Quote_State::Quote_0;
}

//�I����
void SceneGame2_2D::Finalize()
{
    //�X�v���C�g�I����
    if (sprite != nullptr)
    {
        delete sprite;
        sprite = nullptr;
    }//�X�v���C�g�I����
    if (sprite2 != nullptr)
    {
        delete sprite2;
        sprite2 = nullptr;
    }
}

//�X�V����
void SceneGame2_2D::Update(float elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    Mouse& mouse = Input::Instance().GetMouse();

    mousePositionX = mouse.GetPositionX();
    mousePositionY = mouse.GetPositionY();

    if (make == 25) state = Quote_State::Quote_25;
    if (kati == 26) state = Quote_State::Quote_26;

    // �}�E�X�N���b�N��X���W���ŏ�������w���p�[�֐�
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

    // `state + 1` �� `Quote_State` �͈͓̔��ł��邩���m�F���L���X�g
    auto IncrementState = [&]() {
        if (state < Quote_State::Quote_29) {  // ��Ԃ��ő�l�𒴂��Ȃ��悤�ɐ���
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
            IncrementState();  // ��Ԃ����ɐi�߂�
        }
        break;

    case Quote_State::Quote_8: // �I�����̏��
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
            IncrementState();  // ��Ԃ����ɐi�߂�
        }
        break;
    case Quote_State::Quote_25: // �s�k
    {
        make = 0;
        if (HandleMouseClick(0, 720, state)) {
            state = Quote_State::Quote_12;
            // IncrementState();  // ��Ԃ����ɐi�߂�
        }
    }
    break;

    // Quote_State::Quote_12�܂���Quote_State::Quote_22�ɂȂ����ꍇ�A�N���b�N��Quote_State::Quote_28�ɑJ��
    case Quote_State::Quote_12: // �Q�[���I�[�o�[
    case Quote_State::Quote_22: // �Q�[���I�[�o�[
        if (HandleMouseClick(0, 720, Quote_State::Quote_28)) {
            state = Quote_State::Quote_28;
        }
        break;

    case Quote_State::Quote_28: // �ŏI���
        if (HandleMouseClick(0, 720, state)) {
            SceneManager::Instance().ChangeScene(new SceneLoading(new SceneSelect));
        }
        break;

    case Quote_State::Quote_24: // �Q�[���ăX�^�[�g
        if (HandleMouseClick(0, 720, state)) {
            stage = 2;
            SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame2));
        }
        break;

    case Quote_State::Quote_26: // ����
        if (HandleMouseClick(0, 720, Quote_State::Quote_27)) {
            kati = 0;
            stage = 0;
        }
        break;
    }
}
//�`�揈��
void SceneGame2_2D::Render()
{
    Graphics& graphics = Graphics::Instance();
    ID3D11DeviceContext* dc = graphics.GetDeviceContext();
    ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
    ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

    // ��ʃN���A�������_�[�^�[�Q�b�g�ݒ�
    FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };
    dc->ClearRenderTargetView(rtv, color);
    dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    dc->OMSetRenderTargets(1, &rtv, dsv);

    // 2D�X�v���C�g�`��
    float screenWidth = static_cast<float>(graphics.GetScreenWidth());
    float screenHeight = static_cast<float>(graphics.GetScreenHeight());
    float textureWidth = static_cast<float>(sprite->GetTextureWidth());
    float textureHeight = static_cast<float>(sprite->GetTextureHeight());

    // �^�C�g���X�v���C�g�`��
    sprite->Render(dc,
        0, 0, screenWidth, screenHeight,
        0, 0, textureWidth, textureHeight,
        0, 1, 1, 1, 1);

    // �����_�����O�̋��ʏ������֐����iX, Y ���W��ǉ��j
    auto RenderSprite2 = [&](float x, float y, float fontSizeX, float scale, float posYMultiplier, float offsetY = 0.0f)
    {
        sprite2->Render(dc,
            x, y,  // X���W��Y���W���w��\��
            screenWidth * fontSizeX, screenHeight * 10.0f,  // �����Əc���̃X�P�[�����O
            0, 144.92928571f * posYMultiplier + offsetY,  // Y���W�X�P�[�����O�ƃI�t�Z�b�g
            textureWidth * scale, textureHeight * scale,  // �e�N�X�`���T�C�Y
            0, 1, 1, 1, 1);  // �F
    };

    // ��Ԃɉ������`�揈��
    switch (state)//1=2.5,2=5.0f.3=7.5f,4=9.0f 5,9,13,17
    {
    case Quote_State::Quote_0: RenderSprite2(0, 380, 5.0f, 9.0f, 0); break;  // �w��ʒu�ł̕`��
    case Quote_State::Quote_1: RenderSprite2(0, 380, 2.5f, 5.0f, 2); break; // �w��ʒu�ł̕`��
    case Quote_State::Quote_2: RenderSprite2(0, 380, 2.5f, 5.0f, 3); break;  // �w��ʒu
    case Quote_State::Quote_3: RenderSprite2(0, 380, 7.5f, 13.0f, 4); break;  // �w��ʒu
    case Quote_State::Quote_4: RenderSprite2(0, 380, 7.5f, 13.0f, 7); break;  // �w��ʒu
    case Quote_State::Quote_5: RenderSprite2(0, 380, 5.0f, 9.0f, 10); break;  // �w��ʒu
    case Quote_State::Quote_6: RenderSprite2(0, 380, 5.0f, 9.0f, 12); break;  // �w��ʒu
    case Quote_State::Quote_7: RenderSprite2(0, 380, 7.5f, 13.0f, 14); break;  // �w��ʒu
    case Quote_State::Quote_8: RenderSprite2(0, 380, 11.5f, 21.0f, 17); break;  // �w��ʒu
    case Quote_State::Quote_9: RenderSprite2(0, 380, 2.5f, 5.0f, 22); break;  // �w��ʒu
    case Quote_State::Quote_10: RenderSprite2(0, 380, 2.5f, 5.0f, 23); break;  // �w��ʒu
    case Quote_State::Quote_11: RenderSprite2(0, 380, 2.5f, 5.0f, 24); break;  // �w��ʒu
    case Quote_State::Quote_12: RenderSprite2(0, 380, 2.5f, 5.0f, 25); break;  // �w��ʒu
    case Quote_State::Quote_13: RenderSprite2(0, 380, 9.0f, 17.0f, 26); break;  // �w��ʒu
    case Quote_State::Quote_14: RenderSprite2(0, 380, 2.5f, 5.0f, 30); break;  // �w��ʒu
    case Quote_State::Quote_15: RenderSprite2(0, 380, 5.0f, 9.0f, 31); break;  // �w��ʒu
    case Quote_State::Quote_16: RenderSprite2(0, 380, 2.5f, 5.0f, 33); break;  // �w��ʒu
    case Quote_State::Quote_17: RenderSprite2(0, 380, 2.5f, 5.0f, 34); break;  // �w��ʒu
    case Quote_State::Quote_18: RenderSprite2(0, 380, 7.5f, 13.0f, 35); break;  // �w��ʒu
    case Quote_State::Quote_19: RenderSprite2(0, 380, 2.5f, 5.0f, 38); break;  // �w��ʒu
    case Quote_State::Quote_20: RenderSprite2(0, 380, 2.5f, 5.0f, 39); break;  // �w��ʒu
    case Quote_State::Quote_21: RenderSprite2(0, 380, 2.5f, 5.0f, 40); break;  // �w��ʒu
    case Quote_State::Quote_22: RenderSprite2(0, 380, 2.5f, 5.0f, 41); break;  // �w��ʒu
    case Quote_State::Quote_23: RenderSprite2(0, 380, 11.5f, 21.0f, 42); break;  // �w��ʒu
    case Quote_State::Quote_24: RenderSprite2(0, 380, 2.5, 5.0f, 47); break;  // �w��ʒu
    case Quote_State::Quote_25: RenderSprite2(0, 380, 5.0f, 9.0f, 49); break;  // �w��ʒu
    case Quote_State::Quote_26: RenderSprite2(0, 380, 9.0f, 17.0f, 51); break;  // �w��ʒu
    case Quote_State::Quote_27: RenderSprite2(0, 380, 2.5f, 5.0f, 55); break;  // �w��ʒu
    case Quote_State::Quote_28: RenderSprite2(0, 380, 2.5f, 5.0f, 57); break;  // �w��ʒu
   // case Quote_State::Quote_29: RenderSprite2(0, 380, 2.5f, 5.0f, 59); break;  // �w��ʒu
    }
}
