///////////////////////////////////
//2025/02/17 
//nanashima gameobject.h
//////////////////////////////////
#pragma once
#include<string>

#define MAXLAYER (3)
enum Layers
	{
	e_Camera=0,
	e_Object,
	e_UI
	};
	
class GameObject
{

protected:

XMFLOAT3  m_Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
XMFLOAT3  m_Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
XMFLOAT3  m_Scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
XMFLOAT3		oldpos = XMFLOAT3(0.0f, 0.0f, 0.0f);
XMFLOAT3		m_Velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
float groundHeight;


//Component* m_Component{};

bool m_Destoroy = false;
public:

	virtual ~GameObject(){}//リストを消すのにデストラクタがないとだめ
	virtual void Init()=0;
	virtual void Uninit() = 0;
	virtual void Draw() = 0;
	virtual void Update() = 0;
	virtual void DetailChange(std::string modelortexture, std::string vs, std::string ps) {};
	
	
	XMFLOAT3 GetPosition() { return m_Position; }
	XMFLOAT3 GetRotation() { return m_Rotation; }
	XMFLOAT3 GetScale() { return m_Scale; }
	void SetPosition(XMFLOAT3 position) { m_Position=position; }
	void SetRotation(XMFLOAT3 rotation) { m_Rotation = rotation; }
	void SetScale(XMFLOAT3 scale) { m_Scale = scale; }
	
	
	void SetDestroy() { m_Destoroy = true; }
	bool Destroy()
	{
		if (m_Destoroy)
		{
			Uninit();
			delete this;
			return true;
		}
		else
		{
			return false;
		}
	}
	XMFLOAT3 GetForward()
	{
		XMMATRIX rotationMatrix;
		rotationMatrix = XMMatrixRotationRollPitchYaw
		(m_Rotation.x, m_Rotation.y, m_Rotation.z);

		XMFLOAT3 foward;
		XMStoreFloat3(&foward, rotationMatrix.r[2]);
		return foward;
	}
	void Stop(XMFLOAT3 Position, XMFLOAT3 Scale)
	{
		XMFLOAT3 direction;
		direction.x = Position.x - m_Position.x;
		direction.y = Position.y - m_Position.y;
		direction.z = Position.z - m_Position.z;
		if (-direction.y > Scale.y - 1)
		{
			groundHeight = Position.y + Scale.y * 2;
		}
		else {
			m_Position.x = oldpos.x;
			m_Position.z = oldpos.z;
			m_Velocity.x = 0;
			m_Velocity.z = 0;
		}
		if (m_Position.y < groundHeight)//地面との衝突判定
		{
			m_Velocity.y = 0.0f;

			m_Position.y = groundHeight;
		}
	}
	
};

