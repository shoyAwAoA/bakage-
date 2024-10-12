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
//グローバル変数
bool tutoFlag = false;

// 初期化
void SceneTutorial::Initialize()
{
	StageManager& stageManager = StageManager::Instance();
	StageMain* stageMain = new StageMain();
	stageManager.Register(stageMain);
	
	player = new Player();

	//カメラ初期設定
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
	//カメラコントローラー初期化
	cameraController = new CameraController();
	sprite = new Sprite("Data/Sprite/tyuto/tyuto.png");
	tutoFlag = true;
	state = Tuto_State::Tuto_0;
	stateTimer = 0;
}

// 終了化
void SceneTutorial::Finalize()
{
	StageManager::Instance().Clear();

	if (player != nullptr)
	{
		delete player;
		player = nullptr;
	}
	//カメラコントローラー終了化
	if (cameraController != nullptr)
	{
		delete cameraController;
		cameraController = nullptr;
	}
}

// 更新処理
void SceneTutorial::Update(float elapsedTime)
{
	stateTimer+=elapsedTime;
	Mouse& mouse = Input::Instance().GetMouse();
	GamePad& gamePad = Input::Instance().GetGamePad();
	mousePositionX = mouse.GetPositionX();
	mousePositionY = mouse.GetPositionY();

	//ステージ更新処理
	StageManager::Instance().Update(elapsedTime);

	//エフェクト更新処理
	EffectManager::Instance().Update(elapsedTime);
	//カメラコントローラー更新処理
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
		if (state < Tuto_State::Tuto_20) {  // 状態が最大値を超えないように制限
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
		case Tuto_State::Tuto_2://まずは
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
		case Tuto_State::Tuto_4://次は
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
		case Tuto_State::Tuto_7://次はEを押してみよう
			if (gamePad.GetButtonDown() & GamePad::BTN_E)
			{
				stateTimer = 0;
				IncrementState();
			}
			break;
		case Tuto_State::Tuto_8://うむ
		case Tuto_State::Tuto_9://Eを押すと
		case Tuto_State::Tuto_10://その代わり
			if (stateTimer > 5)
			{
				stateTimer = 0;
				IncrementState();
			}
			break;
		case Tuto_State::Tuto_11://お次は右クリックだ
			if (mouse.GetButtonDown() & Mouse::BTN_RIGHT)
			{
				stateTimer = 0;
				IncrementState();
			}
			break;
		case Tuto_State::Tuto_12://有無
		case Tuto_State::Tuto_13://右クリックすると
			if (stateTimer > 5)
			{
				stateTimer = 0;
				IncrementState();
			}
			break;
		case Tuto_State::Tuto_14://これがラストだ
			if (gamePad.GetButtonDown() & GamePad::BTN_Q)
			{
				stateTimer = 0;
				IncrementState();
			}
			break;
		case Tuto_State::Tuto_15://よくやった
		case Tuto_State::Tuto_16://これは
		case Tuto_State::Tuto_17://しかし
		case Tuto_State::Tuto_18://気を付けて
		case Tuto_State::Tuto_19://これで簡単
			if (stateTimer > 5)
			{
				stateTimer = 0;
				IncrementState();
			}
			break;
		case Tuto_State::Tuto_20://左クリックして
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

	// 画面クリア＆レンダーターゲット設定
	const FLOAT clearColor[] = { 0.0f, 0.0f, 0.5f, 1.0f }; // RGBA(0.0～1.0)
	dc->ClearRenderTargetView(rtv, clearColor);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);
	// 描画コンテキストの準備
	RenderContext rc;
	rc.lightDirection = { 0.0f, -1.0f, 0.0f, 0.0f }; // ライト方向
	Camera& camera = Camera::Instance();
	rc.view = camera.GetView();
	rc.projection = camera.GetProjection();


	

	// 3Dモデル描画
	{
		Shader* shader = graphics.GetShader();
		shader->Begin(dc, rc);
		StageManager::Instance().Render(dc, shader);

		player->Render(dc, shader);

		shader->End(dc);
	}


	// 2Dスプライト描画
	{
		//説明スプライト描画
		{
			switch (state)
			{//1行　160 140 2行 280 150  
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

	// 3Dデバッグ描画
	{
		player->DrawDebugPrimitive();
		graphics.GetLineRenderer()->Render(dc, rc.view, rc.projection);
		graphics.GetDebugRenderer()->Render(dc, rc.view, rc.projection);
	}

	

	// 3Dエフェクト描画
	EffectManager::Instance().Render(rc.view, rc.projection);

	

	// 2DデバッグGUI描画
	player->DrawDebugGUI();
}

