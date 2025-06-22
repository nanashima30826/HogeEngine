///////////////////////////////////
//2025/02/17 
//nanashima TitleObject.cpp
//////////////////////////////////
#include "TitleObject.h"
#include"renderer.h"
#include"main.h"
#include"modelRenderer.h"
#include"bullet.h"
#include"scene.h"
#include"manager.h"
#include"bullet.h"
void TitleObject::Init()
{

	m_Component = new ModelRenderer(this);
	((ModelRenderer*)m_Component)->Load("asset\\model\\title.obj");

	TexMetadata metadata;
	ScratchImage image;
	LoadFromWICFile(L"asset\\texture\\earthenvmap.png", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &m_EnvTexture);
	assert(m_EnvTexture);
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\EnvmappingVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader, "shader\\EnvmappingPS.cso");

}

void TitleObject::Uninit()
{
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
	m_EnvTexture->Release();
}
void TitleObject::Update()
{
	
}
void TitleObject::Draw()
{
	m_Scale = XMFLOAT3(1, 1, 1);
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	XMMATRIX world, scale, rot, trans;

	scale = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
	rot = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
	trans = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(world);

	Renderer::GetDeviceContext()->PSSetShaderResources(1, 1, &m_EnvTexture);

	m_Component->Draw();
}
