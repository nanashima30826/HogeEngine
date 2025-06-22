///////////////////////////////////
//2025/02/17 
//nanashima explosion.cpp
//////////////////////////////////
#include"main.h"
#include"renderer.h"
#include"explosion.h"
#include"scene.h"
#include"camera.h"
#include"manager.h"

void Explosion::Init()
{
	VERTEX_3D vertex[4];
	{
		vertex[0].Position = XMFLOAT3(-5, 5, 0);
		vertex[0].Normal = XMFLOAT3(0, 1, 0);
		vertex[0].Diffuse = XMFLOAT4(1, 1, 1, 1);
		vertex[0].TexCoord = XMFLOAT2(0, 0);

		vertex[1].Position = XMFLOAT3(5, 5, 0);
		vertex[1].Normal = XMFLOAT3(0, 1, 0);
		vertex[1].Diffuse = XMFLOAT4(1, 1, 1, 1);
		vertex[1].TexCoord = XMFLOAT2(2, 0);

		vertex[2].Position = XMFLOAT3(-5, 0, 0);
		vertex[2].Normal = XMFLOAT3(0, 1, 0);
		vertex[2].Diffuse = XMFLOAT4(1, 1, 1, 1);
		vertex[2].TexCoord = XMFLOAT2(0, 2);

		vertex[3].Position = XMFLOAT3(5, 0, 0);
		vertex[3].Normal = XMFLOAT3(0, 1, 0);
		vertex[3].Diffuse = XMFLOAT4(1, 1, 1, 1);
		vertex[3].TexCoord = XMFLOAT2(2, 2);

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(VERTEX_3D) * 4;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = vertex;

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

		TexMetadata metadata;
		ScratchImage image;
		LoadFromWICFile(L"asset\\texture\\explosion.png", WIC_FLAGS_NONE, &metadata, image);
		CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &m_Texture);
		assert(m_Texture);

		Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\unlitTextureVS.cso");
		Renderer::CreatePixelShader(&m_PixelShader, "shader\\unlitTexturePS.cso");

	}
}
void Explosion::Uninit()
{

	m_VertexShader->Release();
	m_VertexBuffer->Release();
	m_Texture->Release();
	m_VertexLayout->Release();
	m_PixelShader->Release();
}

void Explosion::Update()
{
	m_Count++;
		if (m_Count>=MAXCOUNT)
		{
			SetDestroy();
			return;
		}
}

void Explosion::Draw()
{

	float x = m_Count % 4 * (1.0f / 4);
	float y = m_Count / 4 * (1.0f / 4);

	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;
	vertex[0].Position = XMFLOAT3(-1.0, 1.0, 0.0);
	vertex[0].Normal = XMFLOAT3(0.0, 1.0, 0.0);
	vertex[0].Diffuse = XMFLOAT4(1.0, 1.0, 1.0,1.0);
	vertex[0].TexCoord = XMFLOAT2(x,y);

	vertex[1].Position = XMFLOAT3(1.0, 1.0, 0.0);
	vertex[1].Normal = XMFLOAT3(0.0, 1.0, 0.0);
	vertex[1].Diffuse = XMFLOAT4(1.0, 1.0, 1.0, 1.0);
	vertex[1].TexCoord = XMFLOAT2(x+0.25, y);

	vertex[2].Position = XMFLOAT3(-1.0, -1.0, 0.0);
	vertex[2].Normal = XMFLOAT3(0.0, 1.0, 0.0);
	vertex[2].Diffuse = XMFLOAT4(1.0, 1.0, 1.0, 1.0);
	vertex[2].TexCoord = XMFLOAT2(x, y+0.25);

	vertex[3].Position = XMFLOAT3(1.0, -1.0, 0.0);
	vertex[3].Normal = XMFLOAT3(0.0, 1.0, 0.0);
	vertex[3].Diffuse = XMFLOAT4(1.0, 1.0, 1.0, 1.0);
	vertex[3].TexCoord = XMFLOAT2(x+0.25, y+0.25);

	Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);

	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);
	Scene* scene = Manager::GetScene();
	Camera* camera = scene->GetGameObject<Camera>();
	XMMATRIX view = camera->GetViewMatrix();
	XMMATRIX invView;
	invView = XMMatrixInverse(nullptr, view);//‹ts—ñ
	invView.r[3].m128_f32[0] = 0;
	invView.r[3].m128_f32[1] = 0;
	invView.r[3].m128_f32[2] = 0;

	XMMATRIX world, scale, rot, trans;
	scale = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
	trans = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	world = scale * invView * trans;
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