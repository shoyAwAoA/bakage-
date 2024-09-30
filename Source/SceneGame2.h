#pragma once
#include"Stage.h"
#include"Player.h"
#include"CameraController.h"
#include"Scene.h"
#include"Graphics/Sprite.h"

class SceneGame2 :public Scene
{
public:
	SceneGame2() {}
	//~SceneGame() {}
	~SceneGame2()override {}

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


private:
	//Stage* stage = nullptr;
	Player* player = nullptr;
	CameraController* cameraController = nullptr;
	Sprite* spr = nullptr;

};