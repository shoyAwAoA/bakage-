//#include"Stage.h"
#include"StageMain.h"

//static Stage* instance = nullptr;

////インスタンス取得
//Stage& Stage::Instance()
//{
//
//    return *instance;
//}
//
////コンストラクタ
//Stage::Stage()
//{
//
//    instance = this;
//
//    //ステージモデルを読み込み
//    model = new Model("Data/Model/ExampleStage/ExampleStage.mdl");
//}
//
//Stage::~Stage()
//{
//    //ステージモデルの破棄
//    delete model;
//}
//
//void Stage::Update(float elapsedTime)
//{
//    //今は特にやることない
//}
//
//void Stage::Render(ID3D11DeviceContext* dc, Shader* shader)
//{
//    //シェーダーにモデルを描画してもらう
//    shader->Draw(dc, model);
//}
//
////レイキャスト
//bool Stage::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
//{
//    return Collision::IntersectRayVsModel(start,end,model,hit);
//}

//コンストラクタ
StageMain::StageMain()
{
    //ステージモデルを読み込み
    model = new Model("Data/Model/stage/stage.mdl");
    scale.x = scale.y = scale.z = 0.03f;
}

//デストラクタ
StageMain::~StageMain()
{
    //ステージモデルを破棄
    delete model;
}

//更新処理
void StageMain::Update(float elapsedTime)
{
    //今は特にやることはない
    UpdateTransform();
}

//描画処理
void StageMain::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    //シェーダーにモデルを描画してもらう
    model->UpdateTransform(transform);
    shader->Draw(dc, model);
}

//レイキャスト
bool StageMain::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
    return Collision::IntersectRayVsModel(start, end, model, hit);;
}

void StageMain::UpdateTransform()
{
    //スケール行列を作成
    DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);

    ////回転行列を作成
    //DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.z,angle.y,angle.x);

    //回転行列を作成
    DirectX::XMMATRIX X = DirectX::XMMatrixRotationX(angle.x);

    DirectX::XMMATRIX Y = DirectX::XMMatrixRotationY(angle.y);

    DirectX::XMMATRIX Z = DirectX::XMMatrixRotationZ(angle.z);

    DirectX::XMMATRIX R = Y * X * Z;

    //位置行列を作成
    DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);

    //3つの行列を組み合わせ、ワールド行列を作成
    DirectX::XMMATRIX W = S * R * T;

    //計算したワールド行列を取り出す
    DirectX::XMStoreFloat4x4(&transform, W);

}
