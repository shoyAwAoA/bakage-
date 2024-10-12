#pragma once
#include"Stage.h"
#include"Player.h"
#include"CameraController.h"
#include"Scene.h"
#include"Graphics/Sprite.h"


// �Q�[���V�[��
//class SceneGame
class SceneGame:public Scene
{
public:
	SceneGame() {}
	//~SceneGame() {}
	~SceneGame()override {}

	// ������
	//void Initialize();
	void Initialize()override;

	// �I����
	//void Finalize();
	void Finalize()override;

	// �X�V����
	//void Update(float elapsedTime);
	void Update(float elapsedTime)override;

	// �`�揈��
	//void Render();
	void Render()override;


	void RenderEnemyGauge(
		ID3D11DeviceContext* dc,
		const DirectX::XMFLOAT4X4& view,
		const DirectX::XMFLOAT4X4& projection
	);
private:
	//�G�l�~�[HP�Q�[�W�`��

public:

	//Sprite* guage = nullptr;
	//Sprite* speak = nullptr;//�ŏ���
	//Sprite* dieSpeak = nullptr;//���ʂƂ��̃Z���t
	Sprite* hissatu=nullptr;

private:
	Player* player = nullptr;
	CameraController* cameraController = nullptr;
	
	float spriteMoveX = 1280;
	float miaSpeed = 1.0f;
};
