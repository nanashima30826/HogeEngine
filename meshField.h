///////////////////////////////////
//2025/02/17 
//nanashima meshField.h
//////////////////////////////////
#pragma once
#include "main.h"
#include"GameObject.h"
class meshField :public GameObject
{
private:
	//XMFLOAT3  m_Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	//XMFLOAT3  m_Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	//XMFLOAT3  m_Scale = XMFLOAT3(1.0f, 1.0f, 1.0f);

	ID3D11Buffer* m_VertexBuffer{};
	ID3D11Buffer* m_IndexBuffer{};
	ID3D11ShaderResourceView* m_Texture{};

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	VERTEX_3D m_vertex[21][21];
	static  bool m_collision;

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
	float GetHeight(XMFLOAT3 Position);
	static bool GetCollision();
	
}; 
