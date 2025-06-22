///////////////////////////////////
//2025/02/17 
//nanashima explosion.h
//////////////////////////////////
#pragma once
#include "main.h"
#include"GameObject.h"
#define MAXCOUNT (16)
class Explosion :public GameObject
{
private:
	
	ID3D11Buffer* m_VertexBuffer{};
	ID3D11ShaderResourceView* m_Texture{};

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	int m_Count{};

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
	
};