///////////////////////////////////
//2025/02/17 
//nanashima Weve.h
//////////////////////////////////
#pragma once
#include "main.h"
#include"GameObject.h"
class Weve :public GameObject
{
private:
	
	ID3D11Buffer* m_VertexBuffer{};
	ID3D11Buffer* m_IndexBuffer{};
	ID3D11ShaderResourceView* m_Texture{};

	ID3D11ShaderResourceView* m_EnvTexture{};

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	VERTEX_3D m_vertex[21][21];
	float m_time{};

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
	float GetHeight(XMFLOAT3 Position);
	
};
