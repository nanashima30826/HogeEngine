///////////////////////////////////
//2025/02/17 
//nanashima Bullet.cpp
//////////////////////////////////
#include "bullet.h"
#include"renderer.h"
#include"main.h"
#include"modelRenderer.h"
#include"input.h"
#include"explosion.h"
#include"scene.h"
#include"manager.h"

#include"camera.h"

void Bullet::Init()
{
	m_Component = new ModelRenderer(this);
	((ModelRenderer*)m_Component)->Load("asset\\model\\bullet.obj");

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader, "shader\\unlitTexturePS.cso");

}

void Bullet::Uninit()
{
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}
void Bullet::Update()
{
	oldpos = m_Position;
	Scene* scene = Manager::GetScene();
	Camera* camera = scene->GetGameObject<Camera>();


	XMFLOAT3 forward = camera->GetForward();
	m_Position.x += forward.x  ;
	m_Position.y += forward.y   ;
	m_Position.z += forward.z  ;
	
	m_timer++;
		if (m_timer>100)
	{
		Explosion* explosion=scene->AddGameObject<Explosion>(e_Object);
		explosion->SetPosition(m_Position);

		SetDestroy();//ƒŠƒXƒg\‘¢‚Ífor‚Ì’†‚ÅÁ‚¹‚È‚¢‚Ì‚Å‚±‚±‚ÅÁ‚·—\–ñ‚¾‚¯‚µ‚Æ‚­
	}
	
}
	



void Bullet::Draw()
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

	m_Component->Draw();
}
