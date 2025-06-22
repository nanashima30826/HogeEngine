#include "player.h"
#include"renderer.h"
#include"main.h"
#include"manager.h"
#include"bullet.h"
#include"scene.h"
#include"modelRenderer.h"
#include"animationModel.h"
#include"input.h"
#include"title.h"
#include"audio.h"
#include"meshField.h"
#include"game.h"
///////////////////////////////////
//2025/02/17 
//nanashima Player.cpp
//////////////////////////////////
void Player::Init()
{
	m_AnimationName = "Idle";
	m_AnimationName2 = "Idle";
	m_Component = new AnimationModel(this);
	((AnimationModel*)m_Component)->Load("asset\\model\\Akai.fbx");
	((AnimationModel*)m_Component)->LoadAnimation("asset\\model\\Akai_Idle.fbx","Idle");
	((AnimationModel*)m_Component)->LoadAnimation("asset\\model\\Akai_Run.fbx", "Run");

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader, "shader\\unlitTexturePS.cso");

	m_se = new Audio(this);
	m_se->Load("asset\\audio\\wan.wav");

	m_Quaternion.x = 0;
	m_Quaternion.y = 0;
	m_Quaternion.z = 0;
	m_Quaternion.w = 1;//wは1

	m_Scale={0.01,0.01,0.01 };

	m_ChildModel = new ModelRenderer(this);
	((ModelRenderer*)m_ChildModel)->Load("asset\\model\\box.obj");

}

void Player::Uninit()
{
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
	m_se->Uninit();
	m_Component->Uninit();
	delete m_Component;
	delete m_se;
}
void Player::Update()
{
	Scene* scene = Manager::GetScene();
	oldpos = m_Position;
	float dt = 1.0 / 60;
	Camera* camera = scene->GetGameObject<Camera>();

	
	XMFLOAT3 forward = camera->GetForward();

	

	
	if (Input::GetKeyPress('W'))
	
	{m_Rotation = camera->GetRotation();
		m_Velocity.x += forward.x * 100 * dt;
		m_Velocity.y += forward.y * 100 * dt;
		m_Velocity.z += forward.z * 100 * dt;

		//SIMD
		//XMVECTOR position, forward;
		//position += forward * 0.1;

		if (m_AnimationName2!="Run")
		{
			m_AnimationName = m_AnimationName2;
			m_AnimationName2 = "Run";
			m_ratio = 0.0;
		}
		
		
	
		
	}

	else
	{
		if (m_AnimationName2!="Idle")
		{
			m_AnimationName = m_AnimationName2;
			m_AnimationName2 = "Idle";
			m_ratio = 0;
		}
}
		m_ratio += 0.1;
		if (m_ratio>1)
		{
			m_ratio = 1;
		}
	
	
	if (Input::GetKeyPress('S'))
	{
		m_Velocity.x -= forward.x * 100 * dt;
		m_Velocity.y -= forward.y * 100 * dt;
		m_Velocity.z -= forward.z * 100 * dt;
	
		XMVECTOR quat = XMQuaternionRotationRollPitchYaw(0, camera->GetRotation().y, 0);
	quat = XMQuaternionMultiply(XMLoadFloat4(&m_Quaternion), quat);
	XMStoreFloat4(&m_Quaternion, quat);
	}
	
	if (Input::GetKeyPress('A'))
	{
		m_Velocity.x -= 100 * dt;
	
	}
	if (Input::GetKeyPress('D'))
	{
		m_Velocity.x += 100 * dt;
		
	}
	m_Velocity.x -= m_Velocity.x * 1*dt;
	m_Velocity.z -= m_Velocity.z * 1 * dt;

	if (Input::GetKeyPress(VK_SPACE))//ジャンプ
	{
		m_Velocity.y = 20;
	}
	if (Game* gameScene = dynamic_cast<Game*>(scene))
	{
		m_Velocity.y += -20 * dt;//重力加速度
	}

	m_Position.y += m_Velocity.y * dt;
	m_Position.x += m_Velocity.x * dt;
	m_Position.z += m_Velocity.z * dt;
	meshField* meshfield = Manager::GetScene()->GetGameObject<meshField>();
	float groundHeight =meshfield->GetHeight(m_Position);
	
	if (m_Position.y<groundHeight)//地面との衝突判定
	{
		m_Velocity.y = 0.0f;
		m_Position.y = groundHeight;
	}
	Scene* pscene = Manager::GetScene();
	if (Input::GetKeyTrigger('K'))
	{
		Bullet* bullet=pscene->AddGameObject<Bullet>(e_Object);
		bullet->SetPosition(GetPosition());
		m_se->Play();
	}
	
		if (Input::GetKeyTrigger(VK_RETURN))
		{
			Manager::SetScene<Title>();
		}

}
void Player::Draw()
{
	
	((AnimationModel*)m_Component)->Update(m_AnimationName.c_str(), m_animationtime, m_AnimationName2.c_str(), m_animationtime, m_ratio);
	m_animationtime++;
	m_Scale = XMFLOAT3(0.01, 0.01, 0.01);
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

	XMMATRIX childTrans,childScale,childWorld,childRot;
	childRot = XMMatrixRotationRollPitchYaw(0, 1, 0);
	childScale = XMMatrixScaling(1 / m_Scale.x, 1 / m_Scale.y, 1 / m_Scale.z);
	childTrans = XMMatrixTranslation(2, 0, 0);
	childWorld = childRot*childTrans *childScale* world;
	
	//子のワールド=子のlocal*親のlocal
	

	Renderer::SetWorldMatrix(childWorld);

	//m_ChildModel->Draw();
}
