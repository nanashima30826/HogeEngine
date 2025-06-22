#pragma once
#include"main.h"
#include"GameObject.h"
#include"component.h"
#include"audio.h" 
#include<string>
///////////////////////////////////
//2025/02/17 
//nanashima Player.h
//////////////////////////////////
class Player:public GameObject
{
private:
	
	
	Component* m_Component{};
	Component* m_ChildModel{};

	float m_animationtime = 0;

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};
	XMFLOAT3		oldpos{};
	XMFLOAT3		m_Velocity{};
	float m_ratio=0;

	Audio* m_se{};

	XMFLOAT4 m_Quaternion{};
	std::string m_AnimationName{};
	std::string m_AnimationName2{};
	int m_AnimationFrame{};
public:
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
	
};

