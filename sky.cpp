///////////////////////////////////
//2025/02/17 
//nanashima sky.cpp
//////////////////////////////////
#include "sky.h"
#include"renderer.h"
#include"main.h"
#include"manager.h"
#include"bullet.h"
#include"scene.h"
#include"modelRenderer.h"
#include"input.h"

void Sky::Init()
{
	m_Component = new ModelRenderer(this);
	//LoadFromWICFile(L"asset\\texture\\tree.png", WIC_FLAGS_NONE, &metadata, image);
	((ModelRenderer*)m_Component)->Load("asset\\model\\sky.obj");

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader, "shader\\unlitTexturePS.cso");
	m_Position.x = 0;
	m_Position.y = 0;
	m_Position.z = 0;
}

void Sky::Uninit()
{
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}
void Sky::Update()
{
	

}
void Sky::Draw()
{
	m_Scale = XMFLOAT3(500,500 , 500);
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
