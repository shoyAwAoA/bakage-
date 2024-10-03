#include "Graphics/Graphics.h"
#include"Input/Input.h"
#include "SceneGame.h"
#include"Player.h"
#include"Camera.h"
#include"EnemyManager.h"
#include"EnemyKiriko.h"
#include"EffectManager.h"
#include"StageManager.h"
#include"StageMain.h"
#include"StageMoveFloor.h"

//グローバル変数
bool speak_flag = false;



// 初期化
void SceneGame::Initialize()
{
	//stage = new Stage();
	StageManager& stageManager = StageManager::Instance();
	StageMain* stageMain = new StageMain();
	stageManager.Register(stageMain);
	StageMoveFloor* stageMoveFloor = new StageMoveFloor();
	stageMoveFloor->SetStartPoint(DirectX::XMFLOAT3(0, 1, 3));
	stageMoveFloor->SetGoalPoint(DirectX::XMFLOAT3(10, 2, 3));
	stageMoveFloor->SetTorque(DirectX::XMFLOAT3(0, 1.0f, 0));
	stageManager.Register(stageMoveFloor);
	spriteMoveX = 1280;

	player = new Player();

	//ゲージスプライト
	guage = new Sprite();
	hissatu = new Sprite("Data/Sprite/riza.png");
	speak = new Sprite("Data/Sprite/riza.png");

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

	//エネミー初期化
	EnemyManager& enemyManager = EnemyManager::Instance();
#if 0
	EnemyKiriko* Kiriko = new EnemyKiriko();
	Kiriko->SetPosition(DirectX::XMFLOAT3(0, 0, 5));
	enemyManager.Register(Kiriko);
#else

	//for (int i = 0; i < 50; ++i)
	//{
	//	EnemyKiriko* Kiriko = new EnemyKiriko();
	//	Kiriko->SetPosition(DirectX::XMFLOAT3(i * 2.0f, 0, 5));
	//	enemyManager.Register(Kiriko);
	//}

	for (int i = 0; i < 1; i++)
	{
		EnemyKiriko* Kiriko = new EnemyKiriko();
		Kiriko->SetPosition(DirectX::XMFLOAT3(i * 2.0f, 0, 5));
		Kiriko->SetTerritory(Kiriko->GetPosition(), 10.0f);
		enemyManager.Register(Kiriko);
	}
#endif
}

// 終了化
void SceneGame::Finalize()
{
	//if (stage != nullptr)
	//{
	//	delete stage;
	//	stage = nullptr;
	//}
	StageManager::Instance().Clear();

	if (player != nullptr)
	{
		delete player;
		player = nullptr;
	}

	//ゲージスプライト終了化
	if (guage != nullptr)
	{
		delete guage;
		guage = nullptr;
	}

	if (hissatu != nullptr)
	{
		delete hissatu;
		hissatu = nullptr;
	}
	if (speak != nullptr)
	{
		delete speak;
		speak = nullptr;
	}


	//エネミー終了化
	EnemyManager::Instance().Clear();

	//カメラコントローラー終了化
	if (cameraController != nullptr)
	{
		delete cameraController;
		cameraController = nullptr;
	}
}

// 更新処理
void SceneGame::Update(float elapsedTime)
{
		EnemyManager::Instance().Update(elapsedTime);
		//ステージ更新処理
		StageManager::Instance().Update(elapsedTime);
	
		//エフェクト更新処理
		EffectManager::Instance().Update(elapsedTime);

	if (player->GetSpecialAttack())
	{
		spriteMoveX -= 7.0f;
	}
	//カメラコントローラー更新処理
	DirectX::XMFLOAT3 target = player->GetPosition();
	target.y += 0.5f;
	cameraController->SetTarget(target);
	cameraController->Update(elapsedTime);
	
	if (!player->GetKirikoUp()|| cameraController->GetKirikoCameraCompreat())
	{
		player->Update(elapsedTime);
	}


}

// 描画処理
void SceneGame::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

	// 画面クリア＆レンダーターゲット設定
	FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };	// RGBA(0.0〜1.0)
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);

	// 描画処理
	RenderContext rc;
	rc.lightDirection = { 0.0f, -1.0f, 0.0f, 0.0f };	// ライト方向（下方向）

	//カメラパラメータ設定
	Camera& camera = Camera::Instance();
	rc.view = camera.GetView();
	rc.projection = camera.GetProjection();

	//// ビュー行列
	//{
	//	DirectX::XMFLOAT3 eye = { 0, 10, -10 };	// カメラの視点（位置）
	//	DirectX::XMFLOAT3 focus = { 0, 0, 0 };	// カメラの注視点（ターゲット）
	//	DirectX::XMFLOAT3 up = { 0, 1, 0 };		// カメラの上方向

	//	DirectX::XMVECTOR Eye = DirectX::XMLoadFloat3(&eye);
	//	DirectX::XMVECTOR Focus = DirectX::XMLoadFloat3(&focus);
	//	DirectX::XMVECTOR Up = DirectX::XMLoadFloat3(&up);
	//	DirectX::XMMATRIX View = DirectX::XMMatrixLookAtLH(Eye, Focus, Up);LH=left hand
	//	DirectX::XMStoreFloat4x4(&rc.view, View);
	//}
	//// プロジェクション行列
	//{
	//	float fovY = DirectX::XMConvertToRadians(45);	// 視野角
	//	float aspectRatio = graphics.GetScreenWidth() / graphics.GetScreenHeight();	// 画面縦横比率
	//	float nearZ = 0.1f;	// カメラが映し出すの最近距離
	//	float farZ = 1000.0f;	// カメラが映し出すの最遠距離
	//	DirectX::XMMATRIX Projection = DirectX::XMMatrixPerspectiveFovLH(fovY, aspectRatio, nearZ, farZ);
	//	DirectX::XMStoreFloat4x4(&rc.projection, Projection);
	//}

	// 3Dモデル描画
	{
		Shader* shader = graphics.GetShader();
		shader->Begin(dc, rc);
		//ステージ描画
		//stage->Render(dc, shader);
		StageManager::Instance().Render(dc, shader);
		
		if (!player->GetKirikoUp() || cameraController->GetKirikoCameraCompreat())
		{
			player->Render(dc, shader);
		}

		//エネミー描画
		EnemyManager::Instance().Render(dc, shader);
		shader->End(dc);
	}

	// 3Dデバッグ描画
	{
		//プレイヤーデバッグプリミティブ描画
		player->DrawDebugPrimitive();

		//エネミーデバッグプリミティブ描画

		EnemyManager::Instance().DrawDebugPrimitive();

		// ラインレンダラ描画実行
		graphics.GetLineRenderer()->Render(dc, rc.view, rc.projection);

		// デバッグレンダラ描画実行
		graphics.GetDebugRenderer()->Render(dc, rc.view, rc.projection);
	}

	//3Dエフェクト描画
	{
		EffectManager::Instance().Render(rc.view, rc.projection);
	}

	// 2Dスプライト描画
	{
		float screenWidth = static_cast<float>(graphics.GetScreenWidth());
		float screenHeight = static_cast<float>(graphics.GetScreenHeight());
		float textureWidth = static_cast<float>(hissatu->GetTextureWidth());
		float textureHeight = static_cast<float>(hissatu->GetTextureHeight());
		RenderEnemyGauge(dc, rc.view, rc.projection);

		if (player->GetSpecialAttack())
		{
			hissatu->Render(dc, spriteMoveX, 0, 200, 100, 0, 0, textureWidth, textureHeight, 0, 1, 1, 1, 1);
		}
		else
		{

		}
	}

	// 2DデバッグGUI描画
	{
		//プレイヤーデバッグ描画
		player->DrawDebugGUI();
		
	}
}

//エネミーHPゲージ描画
void SceneGame::RenderEnemyGauge(
	ID3D11DeviceContext* dc,
	const DirectX::XMFLOAT4X4& view,
	const DirectX::XMFLOAT4X4& projection)
{
	//ビューポート
	D3D11_VIEWPORT viewport;
	UINT numViewports = 1;
	dc->RSGetViewports(&numViewports, &viewport);

	//変換行列
	DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&view);
	DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&projection);
	DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();

	//全ての敵の頭上にHPゲージを表示
	EnemyManager& enemyManager = EnemyManager::Instance();
	int enemyCount = enemyManager.GetEnemyCount();

	for (int i = 0; i < enemyCount; ++i)
	{
		Enemy* enemy = enemyManager.GetEnemy(i);

		

		DirectX::XMFLOAT3 worldPosition = enemy->GetPosition();
		worldPosition.y += enemy->GetHeight();

		DirectX::XMVECTOR WorldPosition = DirectX::XMLoadFloat3(&worldPosition);
		
		DirectX::XMVECTOR ScreenPosition = DirectX::XMVector3Project(
			WorldPosition,
			viewport.TopLeftX,
			viewport.TopLeftY,
			viewport.Width,
			viewport.Height,
			viewport.MinDepth,
			viewport.MaxDepth,
			Projection,
			View,
			World
		);

		//スクリーン座標
		DirectX::XMFLOAT2 screenPosition;
		DirectX::XMStoreFloat2(&screenPosition, ScreenPosition);

		const float speakWidth = 50.0f;
		const float speakHeight = 20.0f;

		if (speak_flag)
		{
			speak->Render(dc,
				screenPosition.x,
				screenPosition.y + 20,
				speakWidth,
				speakHeight,
				0, 0,
				static_cast<float>(speak->GetTextureWidth()),
				static_cast<float>(speak->GetTextureHeight()),
				0.0f,
				1.0f, 1.0f, 1.0f, 1.0f
			);
		}
		else
		{

		}
	
	}

	
	

	////エネミー配置処理
	//Mouse& mouse = Input::Instance().GetMouse();
	//if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
	//{
	//	//マウスカーソル座標を取得
	//	DirectX::XMFLOAT3 screenPosition;
	//	screenPosition.x = static_cast<float>(mouse.GetPositionX());
	//	screenPosition.y = static_cast<float>(mouse.GetPositionY());

	//

	//}

}
