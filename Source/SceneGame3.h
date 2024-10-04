#pragma once
#include"Stage.h"
#include"Player.h"
#include"CameraController.h"
#include"Scene.h"
#include"Graphics/Sprite.h"

class SceneGame3 :public Scene
{
public:
	SceneGame3() {}
	//~SceneGame() {}
	~SceneGame3()override {}

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
	//Stage* stage = nullptr;
	Player* player = nullptr;
	CameraController* cameraController = nullptr;
	Sprite* spr = nullptr;

	Sprite* guage = nullptr;
	Sprite* silverbackSpeak = nullptr;//�ŏ���
	Sprite* silverbackDieSpeak = nullptr;//���ʂƂ��̃Z���t
	Sprite* hissatu = nullptr;

	float spriteMoveX = 1280;
	float miaSpeed = 1.0f;
};