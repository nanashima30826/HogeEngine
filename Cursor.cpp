///////////////////////////////////
//2025/02/17 
//nanashima Cursor.cpp
//////////////////////////////////
#include "cursor.h"
#include"renderer.h"
#include"main.h"
#include"manager.h"
#include"bullet.h"
#include"scene.h"
#include"modelRenderer.h"

#include"input.h"
#include"title.h"
#include"audio.h"
#include"meshField.h"

std::string Cursor::m_classtexture = "";
std::string Cursor::m_classmodel = "asset\\model\\sankaku.obj";
std::string Cursor::m_classVS = "shader\\unlitTextureVS.cso";
std::string Cursor::m_classPS = "shader\\unlitTexturePS.cso";
bool Cursor::m_collision = false;

void Cursor::Init()
{
	
	m_Component = new ModelRenderer(this);
	
	((ModelRenderer*)m_Component)->Load(GetModel());
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, GetVS());

	Renderer::CreatePixelShader(&m_PixelShader, GetPS());

	

	m_Quaternion.x = 0;
	m_Quaternion.y = 0;
	m_Quaternion.z = 0;
	m_Quaternion.w = 1;//wは1

	m_Scale={1,1,1 };
	
	

}

void Cursor::Uninit()
{
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
	m_Component->Uninit();
	delete m_Component;
	
}
void Cursor::Update()
{
	Scene* scene = Manager::GetScene();
	oldpos = m_Position;
	
	Camera* camera = scene->GetGameObject<Camera>();

	
	XMFLOAT3 forward = camera->GetForward();
	XMFLOAT3 right = { -forward.z, 0, forward.x }; // カメラの右方向ベクトル
	

	m_Velocity.x = 0.0f;
	m_Velocity.y = 0.0f;
	m_Velocity.z = 0.0f;
	
	if (Input::GetKeyPress('W'))
	
	{m_Rotation = camera->GetRotation();
	
		m_Velocity.x += forward.x ;
		m_Velocity.y += forward.y ;
		m_Velocity.z += forward.z ;
	}

	
	
	
	
	if (Input::GetKeyPress('S'))
	{
		m_Velocity.x -= forward.x ;
		m_Velocity.y -= forward.y ;
		m_Velocity.z -= forward.z ;
	
		XMVECTOR quat = XMQuaternionRotationRollPitchYaw(0, camera->GetRotation().y, 0);
	quat = XMQuaternionMultiply(XMLoadFloat4(&m_Quaternion), quat);
	XMStoreFloat4(&m_Quaternion, quat);
	}
	
	if (Input::GetKeyPress('A'))
	{
		m_Velocity.x = right.x; 
		m_Velocity.z = right.z;

	
	}
	if (Input::GetKeyPress('D'))
	{
		m_Velocity.x = -right.x;
		m_Velocity.z = -right.z;
		
	}


	
	if (Input::GetKeyPress(VK_SPACE))//ジャンプ
	{
		m_Velocity.y = 1;
	}
	else if (Input::GetKeyPress('V'))//ジャンプ
	{
		m_Velocity.y = -1;
	}
	else
	{
		m_Velocity.y = 0;
	}


	m_Position.y += m_Velocity.y;
	m_Position.x += m_Velocity.x;
	m_Position.z += m_Velocity.z;
	meshField* meshfield = Manager::GetScene()->GetGameObject<meshField>();
	 groundHeight =meshfield->GetHeight(m_Position);
	
	
	
	
	if (m_Position.y<groundHeight)//地面との衝突判定
	{
		m_Velocity.y = 0.0f;

		m_Position.y = groundHeight;
	}
	Scene* pscene = Manager::GetScene();
	
	
	
		if (Input::GetKeyTrigger(VK_RETURN))
		{
			Manager::SetScene<Title>();
		}

}
void Cursor::Draw()
{
	
	m_Scale = XMFLOAT3(0.1, 0.1, 0.1);
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	XMMATRIX world, scale, rot, trans;

	scale = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
	rot = XMMatrixRotationRollPitchYaw(m_Rotation.x , m_Rotation.y, m_Rotation.z);
	
	trans = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(world);

	m_Component->Draw();

	XMMATRIX childTrans,childScale,childWorld,childRot;
	childRot = XMMatrixRotationRollPitchYaw(0, 1, 0);
	childScale = XMMatrixScaling(1 / m_Scale.x, 1 / m_Scale.y, 1 / m_Scale.z);
	childTrans = XMMatrixTranslation(2, 0, 0);
	childWorld = childRot*childTrans *childScale* world;
	
	
}

std::string Cursor::GetClassModel()
{
	return m_classmodel;
}

std::string Cursor::GetClassVS()
{
	return m_classVS;
}

std::string Cursor::GetClassPS()
{
	return m_classPS;
}

void Cursor::SetTexture(std::string texture)
{
	m_classtexture = texture;
}

void Cursor::SetModel(const std::string model)
{
	m_classmodel = model;
}

void Cursor::SetVS(const std::string vs)
{
	m_classVS = vs;
}

void Cursor::SetPS(const std::string ps)
{
	m_classPS = ps;
}

const char* Cursor::GetTexture()
{
	const char* ctexture = m_classtexture.c_str();
	return ctexture;
}

const char* Cursor::GetModel()
{
	const char* cmodel = m_classmodel.c_str();
	return cmodel;
}

const char* Cursor::GetVS()
{
	const char* cvs = m_classVS.c_str();
	return cvs;
}

const char* Cursor::GetPS()
{
	const char* cps = m_classPS.c_str();
	return cps;
}

bool Cursor::GetCollision()
{
	return m_collision;
}

