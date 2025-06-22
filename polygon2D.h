///////////////////////////////////
//2025/02/17 
//nanashima polygon2D.h
//////////////////////////////////
#pragma once
#include"GameObject.h"
class Polygon2D :public GameObject
{
public:
	void Init();
	void Init(XMFLOAT3 pos1, XMFLOAT3 pos2, XMFLOAT3 pos3, XMFLOAT3 pos4, XMFLOAT4 color);
	void Uninit();
	void Update();
	void Draw();

private:
	ID3D11Buffer* m_VertexBuffer = NULL;
	ID3D11ShaderResourceView* m_Texture = NULL;

	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_VertexLayout;

	class Audio* m_BGM;

};

