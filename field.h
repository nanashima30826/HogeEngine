///////////////////////////////////
//2025/02/17 
//nanashima field.h
//////////////////////////////////
#pragma once
#include "main.h"
#include"GameObject.h"
class Field:public GameObject
{
private:
	//XMFLOAT3  m_Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	//XMFLOAT3  m_Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	//XMFLOAT3  m_Scale = XMFLOAT3(1.0f, 1.0f, 1.0f);

	ID3D11Buffer* m_VertexBuffer{};
	ID3D11ShaderResourceView* m_Texture{};

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
	
};