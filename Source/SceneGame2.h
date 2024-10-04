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

	// 初期化
	//void Initialize();
	void Initialize()override;

	// 終了化
	//void Finalize();
	void Finalize()override;

	// 更新処理
	//void Update(float elapsedTime);
	void Update(float elapsedTime)override;

	// 描画処理
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

	Sprite* guage=nullptr;
	Sprite* speak=nullptr;//最初の
	Sprite* dieSpeak=nullptr;//死ぬときのセリフ
	Sprite* hissatu=nullptr;

	float spriteMoveX = 1280;
	float miaSpeed = 1.0f;
};