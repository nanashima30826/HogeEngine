///////////////////////////////////
//2025/02/17 
//nanashima camera.h
//////////////////////////////////
#pragma once
#include"GameObject.h"
class Camera:public GameObject
{

private:
	BYTE m_key[256];
	XMFLOAT3 m_Target{};
	XMFLOAT4X4 m_ViewMatrix{};//
	static  bool m_collision;
public:
	XMMATRIX GetViewMatrix()
	{
		return XMLoadFloat4x4(&m_ViewMatrix);
	}
	void Init();
	void Uninit();
	void Update();
	void Draw();
	static bool GetCollision();
	

};


