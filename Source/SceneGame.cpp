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

//�O���[�o���ϐ�
bool speak_flag = false;



// ������
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

	//�Q�[�W�X�v���C�g
	guage = new Sprite();
	hissatu = new Sprite("Data/Sprite/riza.png");
	speak = new Sprite("Data/Sprite/riza.png");

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

	//�G�l�~�[������
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

// �I����
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

	//�Q�[�W�X�v���C�g�I����
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


	//�G�l�~�[�I����
	EnemyManager::Instance().Clear();

	//�J�����R���g���[���[�I����
	if (cameraController != nullptr)
	{
		delete cameraController;
		cameraController = nullptr;
	}
}

// �X�V����
void SceneGame::Update(float elapsedTime)
{
		EnemyManager::Instance().Update(elapsedTime);
		//�X�e�[�W�X�V����
		StageManager::Instance().Update(elapsedTime);
	
		//�G�t�F�N�g�X�V����
		EffectManager::Instance().Update(elapsedTime);

	if (player->GetSpecialAttack())
	{
		spriteMoveX -= 7.0f;
	}
	//�J�����R���g���[���[�X�V����
	DirectX::XMFLOAT3 target = player->GetPosition();
	target.y += 0.5f;
	cameraController->SetTarget(target);
	cameraController->Update(elapsedTime);
	
	if (!player->GetKirikoUp()|| cameraController->GetKirikoCameraCompreat())
	{
		player->Update(elapsedTime);
	}


}

// �`�揈��
void SceneGame::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

	// ��ʃN���A�������_�[�^�[�Q�b�g�ݒ�
	FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };	// RGBA(0.0�`1.0)
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);

	// �`�揈��
	RenderContext rc;
	rc.lightDirection = { 0.0f, -1.0f, 0.0f, 0.0f };	// ���C�g�����i�������j

	//�J�����p�����[�^�ݒ�
	Camera& camera = Camera::Instance();
	rc.view = camera.GetView();
	rc.projection = camera.GetProjection();

	//// �r���[�s��
	//{
	//	DirectX::XMFLOAT3 eye = { 0, 10, -10 };	// �J�����̎��_�i�ʒu�j
	//	DirectX::XMFLOAT3 focus = { 0, 0, 0 };	// �J�����̒����_�i�^�[�Q�b�g�j
	//	DirectX::XMFLOAT3 up = { 0, 1, 0 };		// �J�����̏����

	//	DirectX::XMVECTOR Eye = DirectX::XMLoadFloat3(&eye);
	//	DirectX::XMVECTOR Focus = DirectX::XMLoadFloat3(&focus);
	//	DirectX::XMVECTOR Up = DirectX::XMLoadFloat3(&up);
	//	DirectX::XMMATRIX View = DirectX::XMMatrixLookAtLH(Eye, Focus, Up);LH=left hand
	//	DirectX::XMStoreFloat4x4(&rc.view, View);
	//}
	//// �v���W�F�N�V�����s��
	//{
	//	float fovY = DirectX::XMConvertToRadians(45);	// ����p
	//	float aspectRatio = graphics.GetScreenWidth() / graphics.GetScreenHeight();	// ��ʏc���䗦
	//	float nearZ = 0.1f;	// �J�������f���o���̍ŋߋ���
	//	float farZ = 1000.0f;	// �J�������f���o���̍ŉ�����
	//	DirectX::XMMATRIX Projection = DirectX::XMMatrixPerspectiveFovLH(fovY, aspectRatio, nearZ, farZ);
	//	DirectX::XMStoreFloat4x4(&rc.projection, Projection);
	//}

	// 3D���f���`��
	{
		Shader* shader = graphics.GetShader();
		shader->Begin(dc, rc);
		//�X�e�[�W�`��
		//stage->Render(dc, shader);
		StageManager::Instance().Render(dc, shader);
		
		if (!player->GetKirikoUp() || cameraController->GetKirikoCameraCompreat())
		{
			player->Render(dc, shader);
		}

		//�G�l�~�[�`��
		EnemyManager::Instance().Render(dc, shader);
		shader->End(dc);
	}

	// 3D�f�o�b�O�`��
	{
		//�v���C���[�f�o�b�O�v���~�e�B�u�`��
		player->DrawDebugPrimitive();

		//�G�l�~�[�f�o�b�O�v���~�e�B�u�`��

		EnemyManager::Instance().DrawDebugPrimitive();

		// ���C�������_���`����s
		graphics.GetLineRenderer()->Render(dc, rc.view, rc.projection);

		// �f�o�b�O�����_���`����s
		graphics.GetDebugRenderer()->Render(dc, rc.view, rc.projection);
	}

	//3D�G�t�F�N�g�`��
	{
		EffectManager::Instance().Render(rc.view, rc.projection);
	}

	// 2D�X�v���C�g�`��
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

	// 2D�f�o�b�OGUI�`��
	{
		//�v���C���[�f�o�b�O�`��
		player->DrawDebugGUI();
		
	}
}

//�G�l�~�[HP�Q�[�W�`��
void SceneGame::RenderEnemyGauge(
	ID3D11DeviceContext* dc,
	const DirectX::XMFLOAT4X4& view,
	const DirectX::XMFLOAT4X4& projection)
{
	//�r���[�|�[�g
	D3D11_VIEWPORT viewport;
	UINT numViewports = 1;
	dc->RSGetViewports(&numViewports, &viewport);

	//�ϊ��s��
	DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&view);
	DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&projection);
	DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();

	//�S�Ă̓G�̓����HP�Q�[�W��\��
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

		//�X�N���[�����W
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

	
	

	////�G�l�~�[�z�u����
	//Mouse& mouse = Input::Instance().GetMouse();
	//if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
	//{
	//	//�}�E�X�J�[�\�����W���擾
	//	DirectX::XMFLOAT3 screenPosition;
	//	screenPosition.x = static_cast<float>(mouse.GetPositionX());
	//	screenPosition.y = static_cast<float>(mouse.GetPositionY());

	//

	//}

}
