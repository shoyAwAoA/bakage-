#include "SceneTyping.h"
#include "imgui.h"
#include "SceneManager.h"
#include "Graphics/Graphics.h"
#include "Input/Input.h"
#include "SceneTutorial.h"
#include "SceneLoading.h"
void SceneTyping::Initialize()
{
}

void SceneTyping::Finalize()
{
  
}

void SceneTyping::Update(float elapsedTime)
{
    Graphics& graphics = Graphics::Instance();
    ID3D11DeviceContext* dc = graphics.GetDeviceContext();
    ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
    ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

    // 画面クリア＆レンダーターゲット設定
    FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };	// RGBA(0.0～1.0)
    dc->ClearRenderTargetView(rtv, color);
    dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    dc->OMSetRenderTargets(1, &rtv, dsv);

    // 描画処理
    RenderContext rc;
    rc.lightDirection = { 0.0f, -1.0f, 0.0f, 0.0f };	// ライト方向（下方向）
  
    SceneManager::Instance().ChangeScene(new SceneLoading(new SceneTutorial));
}

void SceneTyping::Render()
{
    ImGui::SetNextWindowPos(ImVec2(300, 300), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(50, 50), ImGuiCond_FirstUseEver);
    static char name[256] = { 0 };
    if (ImGui::Begin("NameWindow", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground))
    {
        if (ImGui::InputText("", name, sizeof(name)))
        {

            OutputDebugStringA(name);
        }
    }
    ImGui::End();

    ImGui::SetNextWindowPos(ImVec2(600, 300), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(0, 0), ImGuiCond_FirstUseEver);

    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(1, 1, 1, 0));
    if (ImGui::Begin("TextWindow", nullptr, ImGuiWindowFlags_NoDecoration))
    {
        
        ImGui::Text(u8"名前:% s\n", name);
    }
    ImGui::PopStyleColor();
    ImGui::End();
}
