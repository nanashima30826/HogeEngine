///////////////////////////////////
//2025/02/17 
//nanashima camera.cpp
//////////////////////////////////
#include"main.h"
#include"manager.h"
#include"renderer.h"
#include"camera.h"
#include"scene.h"
#include"player.h"
#include"input.h"
#include"game.h"
bool Camera::m_collision = false;
void Camera::Init()
{
	m_Position = XMFLOAT3(0.0f, 10.0f, -10.0f);
	m_Target = XMFLOAT3(0, 30, -100);
}

void Camera::Uninit()
{

}

void Camera::Update()
{
	Scene* scene=Manager::GetScene();
	
	//m_Target = player->GetPosition();
	if (Game* gameScene = dynamic_cast<Game*>(scene))
	{
		Cursor* cursor = scene->GetGameObject<Cursor>();
		XMFLOAT3 target = cursor->GetPosition();
		target.y += 2.0f;
		m_Target.x += (target.x - m_Target.x) * 0.1;
		m_Target.y += (target.y - m_Target.y) * 0.1;
		m_Target.z += (target.z - m_Target.z) * 0.1;
	}

	
	if (Input::GetKeyPress(VK_LEFT))
	{
		m_Rotation.y -= 0.1f;
	}
	if (Input::GetKeyPress(VK_RIGHT))
	{
		m_Rotation.y += 0.1;
	}
	if (Input::GetKeyPress(VK_UP))
	{
		m_Rotation.x -= 0.1f;
	}
	if (Input::GetKeyPress(VK_DOWN))
	{
		m_Rotation.x += 0.1;
	}
	m_Position.x = m_Target.x - 6 * sinf(m_Rotation.y) * cosf(m_Rotation.x);
	m_Position.y = m_Target.y + 6 * sinf(m_Rotation.x);
	m_Position.z = m_Target.z - 6 * cosf(m_Rotation.y) * cosf(m_Rotation.x);
	
}

void Camera::Draw()
{
	XMFLOAT3 up{ 0.0f,1.0f,0.0f };

	XMMATRIX viewMatrix = XMMatrixLookAtLH(XMLoadFloat3(&m_Position), XMLoadFloat3(&m_Target), XMLoadFloat3(&up));
	//XMFLOAT4X4 m_ViewMatrix;//
	Renderer::SetViewMatrix(viewMatrix);
	XMStoreFloat4x4(&m_ViewMatrix, viewMatrix);

	XMMATRIX projectionMatrix;
	projectionMatrix = XMMatrixPerspectiveFovLH(FovAngleY, (float)SCREEN_WIDTH / SCREEN_HEIGHT, NearZ, FarZ);

	Renderer::SetProjectionMatrix(projectionMatrix);
}

bool Camera::GetCollision()
{
	return m_collision;
}
