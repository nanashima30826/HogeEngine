#include "Example.h"
#include "main.h"
#include "renderer.h"
#include "modelRenderer.h"
#include "scene.h"
#include "manager.h"
void Example::Init() {
     m_Component = new ModelRenderer(this);
     ((ModelRenderer*)m_Component)->Load("asset\model\GMmodel01.obj");
      Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\unlitTextureVS.cso");
     Renderer::CreatePixelShader(&m_PixelShader, "shader\unlitTexturePS.cso");
}
void Example::Uninit() {
m_VertexLayout->Release();m_VertexShader->Release();m_PixelShader->Release(); }
void Example::Update() {
}
void Example::Draw() {
    m_Scale = XMFLOAT3(0.1f, 0.1f, 0.1f);
    Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

    Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
    Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

    XMMATRIX world, scale, rot, trans;
    scale = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
    rot = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
    trans = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
    world = scale * rot * trans;
    Renderer::SetWorldMatrix(world);

        m_Component->Draw();
}
