///////////////////////////////////
//2025/02/17 
//nanashima Bullet.h
//////////////////////////////////
#pragma once
#include"main.h"
#include"GameObject.h"
#include"component.h"

class Bullet :public GameObject
{
private:


	Component* m_Component{};


	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	int m_score = 0;
		int keta = 1;
		int m_timer = 0;

	XMFLOAT3		m_Velocity{};
	
public:
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
	
};