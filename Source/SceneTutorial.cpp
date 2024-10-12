#include "Graphics/Graphics.h"
#include"Input/Input.h"
#include "SceneTutorial.h"
#include"Player.h"
#include"Camera.h"

#include"EffectManager.h"
#include"StageManager.h"
#include"StageMain.h"
#include"SceneManager.h"
#include"SceneLoading.h"
#include"SceneSelect.h"
//�O���[�o���ϐ�
bool tutoFlag = false;

// ������
void SceneTutorial::Initialize()
{
	StageManager& stageManager = StageManager::Instance();
	StageMain* stageMain = new StageMain();
	stageManager.Register(stageMain);
	
	player = new Player();

	//�J���������ݒ�
	Graphics& graphics = Graphics::Instance();
	Camera& camera = Camera::Instance();
	camera.SetLookAt(
		DirectX::XMFLOAT3(0, 10, -10),
		DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3(0, 1, 0)
	);
	camera.SetPerspectiveFov(
		DirectX::XMConvertToRadians(45),
		graphics.GetScreenWidth() / graphics.GetScreenHeight(),
		0.1f,
		1000.0f
	);
	//�J�����R���g���[���[������
	cameraController = new CameraController();
	sprite = new Sprite("Data/Sprite/tyuto/tyuto.png");
	tutoFlag = true;
	state = Tuto_State::Tuto_0;
	stateTimer = 0;
}

// �I����
void SceneTutorial::Finalize()
{
	StageManager::Instance().Clear();

	if (player != nullptr)
	{
		delete player;
		player = nullptr;
	}
	//�J�����R���g���[���[�I����
	if (cameraController != nullptr)
	{
		delete cameraController;
		cameraController = nullptr;
	}
}

// �X�V����
void SceneTutorial::Update(float elapsedTime)
{
	stateTimer+=elapsedTime;
	Mouse& mouse = Input::Instance().GetMouse();
	GamePad& gamePad = Input::Instance().GetGamePad();
	mousePositionX = mouse.GetPositionX();
	mousePositionY = mouse.GetPositionY();

	//�X�e�[�W�X�V����
	StageManager::Instance().Update(elapsedTime);

	//�G�t�F�N�g�X�V����
	EffectManager::Instance().Update(elapsedTime);
	//�J�����R���g���[���[�X�V����
	DirectX::XMFLOAT3 target = player->GetPosition();
	target.y += 0.5f;
	cameraController->SetTarget(target);
	cameraController->Update(elapsedTime);

	player->Update(elapsedTime);

	auto HandleMouseClick = [&]( Tuto_State nextState)
	{
		if ((mouse.GetButtonDown() & Mouse::BTN_LEFT))
		{
			state = nextState;
			return true;
		}
		return false;
	};

	auto IncrementState = [&]() {
		if (state < Tuto_State::Tuto_20) {  // ��Ԃ��ő�l�𒴂��Ȃ��悤�ɐ���
			state = static_cast<Tuto_State>(static_cast<int>(state) + 1);
		}
	};
		switch (state)
		{
		case Tuto_State::Tuto_0:
		case Tuto_State::Tuto_1:
			if (stateTimer>5)
			{
				stateTimer = 0;
				IncrementState();
			}
			break;
		case Tuto_State::Tuto_2://�܂���
			if (gamePad.GetButtonDown() & GamePad::BTN_W || gamePad.GetButtonDown() & GamePad::BTN_A || gamePad.GetButtonDown() & GamePad::BTN_S || gamePad.GetButtonDown() & GamePad::BTN_D)
			{
				stateTimer = 0;
				IncrementState();
			}
			break;
		case Tuto_State::Tuto_3:
			if (stateTimer > 5)
			{
				stateTimer = 0;
				IncrementState();
			}
			break;
		case Tuto_State::Tuto_4://����
			if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
			{
				stateTimer = 0;
				IncrementState();
			}
			break;
		case Tuto_State::Tuto_5:
		case Tuto_State::Tuto_6:
			if (stateTimer > 5)
			{
				stateTimer = 0;
				IncrementState();
			}
			break;
		case Tuto_State::Tuto_7://����E�������Ă݂悤
			if (gamePad.GetButtonDown() & GamePad::BTN_E)
			{
				stateTimer = 0;
				IncrementState();
			}
			break;
		case Tuto_State::Tuto_8://����
		case Tuto_State::Tuto_9://E��������
		case Tuto_State::Tuto_10://���̑���
			if (stateTimer > 5)
			{
				stateTimer = 0;
				IncrementState();
			}
			break;
		case Tuto_State::Tuto_11://�����͉E�N���b�N��
			if (mouse.GetButtonDown() & Mouse::BTN_RIGHT)
			{
				stateTimer = 0;
				IncrementState();
			}
			break;
		case Tuto_State::Tuto_12://�L��
		case Tuto_State::Tuto_13://�E�N���b�N�����
			if (stateTimer > 5)
			{
				stateTimer = 0;
				IncrementState();
			}
			break;
		case Tuto_State::Tuto_14://���ꂪ���X�g��
			if (gamePad.GetButtonDown() & GamePad::BTN_Q)
			{
				stateTimer = 0;
				IncrementState();
			}
			break;
		case Tuto_State::Tuto_15://�悭�����
		case Tuto_State::Tuto_16://�����
		case Tuto_State::Tuto_17://������
		case Tuto_State::Tuto_18://�C��t����
		case Tuto_State::Tuto_19://����ŊȒP
			if (stateTimer > 5)
			{
				stateTimer = 0;
				IncrementState();
			}
			break;
		case Tuto_State::Tuto_20://���N���b�N����
			if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
			{
				tutoFlag = false;
				SceneManager::Instance().ChangeScene(new SceneLoading(new SceneSelect));
			}
		}
}

void SceneTutorial::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

	// ��ʃN���A�������_�[�^�[�Q�b�g�ݒ�
	const FLOAT clearColor[] = { 0.0f, 0.0f, 0.5f, 1.0f }; // RGBA(0.0�`1.0)
	dc->ClearRenderTargetView(rtv, clearColor);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);
	// �`��R���e�L�X�g�̏���
	RenderContext rc;
	rc.lightDirection = { 0.0f, -1.0f, 0.0f, 0.0f }; // ���C�g����
	Camera& camera = Camera::Instance();
	rc.view = camera.GetView();
	rc.projection = camera.GetProjection();


	

	// 3D���f���`��
	{
		Shader* shader = graphics.GetShader();
		shader->Begin(dc, rc);
		StageManager::Instance().Render(dc, shader);

		player->Render(dc, shader);

		shader->End(dc);
	}


	// 2D�X�v���C�g�`��
	{
		//�����X�v���C�g�`��
		{
			switch (state)
			{//1�s�@160 140 2�s 280 150  
			case Tuto_State::Tuto_0:sprite->Render(dc, 50, 10, 1000, 75, 0, 0, 2700, 170, angle, r, g, b, a); break;//
			case Tuto_State::Tuto_1:sprite->Render(dc, 50, 10, 1000, 150, 0, 170, 2700, 290, angle, r, g, b, a); break;//
			case Tuto_State::Tuto_2:sprite->Render(dc, 50, 10, 1000, 75, 0, 470, 2700, 140, angle, r, g, b, a); break;//
			case Tuto_State::Tuto_3:sprite->Render(dc, 50, 10, 1000, 150, 0, 620, 2700, 280, angle, r, g, b, a); break;//
			case Tuto_State::Tuto_4:sprite->Render(dc, 50, 10, 1000, 75, 0, 910, 2700, 140, angle, r, g, b, a); break;//
			case Tuto_State::Tuto_5:sprite->Render(dc, 100, 10, 1000, 75, 0, 1060, 2700, 130, angle, r, g, b, a); break;//
			case Tuto_State::Tuto_6:sprite->Render(dc, 50, 10, 1000, 150, 0, 1200, 2700, 280, angle, r, g, b, a); break;//
			case Tuto_State::Tuto_7:sprite->Render(dc, 50, 10, 1000, 75, 0, 1490, 2700, 140, angle, r, g, b, a); break;//
			case Tuto_State::Tuto_8:sprite->Render(dc, 100, 10, 1000, 75, 0, 1640, 2700, 130, angle, r, g, b, a); break;//
			case Tuto_State::Tuto_9:sprite->Render(dc, 50, 10, 1000, 150, 0, 1780, 2700, 280, angle, r, g, b, a); break;//
			case Tuto_State::Tuto_10:sprite->Render(dc, 50, 10, 1000, 75, 0,2070, 2700, 130, angle, r, g, b, a); break;//
			case Tuto_State::Tuto_11:sprite->Render(dc, 50, 10, 1000, 75, 0, 2210, 2700, 150, angle, r, g, b, a); break;//
			case Tuto_State::Tuto_12:sprite->Render(dc, 50, 10, 1000, 75, 0, 2350, 2700, 140, angle, r, g, b, a); break;//
			case Tuto_State::Tuto_13:sprite->Render(dc, 50, 10, 1000, 150, 0, 2490, 2700, 290, angle, r, g, b, a); break;//
			case Tuto_State::Tuto_14:sprite->Render(dc, 100, 10, 1000, 75, 0, 2790, 2700, 150, angle, r, g, b, a); break;//
			case Tuto_State::Tuto_15:sprite->Render(dc, 50, 10, 1000, 75, 0, 2950, 2700, 130, angle, r, g, b, a); break;//
			case Tuto_State::Tuto_16:sprite->Render(dc, 50, 10, 1000, 150, 0, 3090, 2700, 280, angle, r, g, b, a); break;//
			case Tuto_State::Tuto_17:sprite->Render(dc, 50, 10, 1000, 150, 0, 3380, 2700, 280, angle, r, g, b, a); break;
			case Tuto_State::Tuto_18:sprite->Render(dc, 50, 10, 1000, 75, 0, 3660, 2700, 140, angle, r, g, b, a); break;
			case Tuto_State::Tuto_19:sprite->Render(dc, 50, 10, 1000, 150, 0, 3800, 2700, 290, angle, r, g, b, a); break;
			case Tuto_State::Tuto_20:sprite->Render(dc, 50, 10, 1000, 75, 0, 4090, 2700, 140, angle, r, g, b, a); break;
			}
		}
	}

	// 3D�f�o�b�O�`��
	{
		player->DrawDebugPrimitive();
		graphics.GetLineRenderer()->Render(dc, rc.view, rc.projection);
		graphics.GetDebugRenderer()->Render(dc, rc.view, rc.projection);
	}

	

	// 3D�G�t�F�N�g�`��
	EffectManager::Instance().Render(rc.view, rc.projection);

	

	// 2D�f�o�b�OGUI�`��
	player->DrawDebugGUI();
}

