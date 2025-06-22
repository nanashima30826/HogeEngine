///////////////////////////////////
//2025/02/17 
//nanashima field.cpp
//////////////////////////////////
#include"main.h"
#include"renderer.h"
#include"field.h"

void Field::Init()
{
	VERTEX_3D vertex[4];
	{
		vertex[0].Position = XMFLOAT3(-50, 0, 50);
		vertex[0].Normal = XMFLOAT3(0, 1, 0);
		vertex[0].Diffuse = XMFLOAT4(1, 1, 1, 1);
		vertex[0].TexCoord = XMFLOAT2(0, 0);

		vertex[1].Position = XMFLOAT3(50, 0, 50);
		vertex[1].Normal = XMFLOAT3(0, 1, 0);
		vertex[1].Diffuse = XMFLOAT4(1, 1, 1, 1);
		vertex[1].TexCoord = XMFLOAT2(20, 0);

		vertex[2].Position = XMFLOAT3(-50, 0, -50);
		vertex[2].Normal = XMFLOAT3(0, 1, 0);
		vertex[2].Diffuse = XMFLOAT4(1, 1, 1, 1);
		vertex[2].TexCoord = XMFLOAT2(0, 20);

		vertex[3].Position = XMFLOAT3(50, 0, -50);
		vertex[3].Normal = XMFLOAT3(0, 1, 0);
		vertex[3].Diffuse = XMFLOAT4(1, 1, 1, 1);
		vertex[3].TexCoord = XMFLOAT2(20, 20);

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(VERTEX_3D) * 4;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = vertex;

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

		TexMetadata metadata;
		ScratchImage image;
		LoadFromWICFile(L"asset\\texture\\wood.jpg", WIC_FLAGS_NONE, &metadata, image);
		CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &m_Texture);
		assert(m_Texture);

		Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\unlitTextureVS.cso");
		Renderer::CreatePixelShader(&m_PixelShader, "shader\\unlitTexturePS.cso");

	}
}
void Field::Uninit()
{
	
	m_VertexShader->Release();
	m_VertexBuffer->Release();
	m_Texture->Release();
	m_VertexLayout->Release();
	m_PixelShader->Release();
}

void Field::Update()
{

}

void Field::Draw()
{
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	XMMATRIX world, scale, rot, trans;
	scale = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
	rot = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
	trans = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(world);

	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1, 1, 1, 1);
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	Renderer::GetDeviceContext()->Draw(4, 0);
}