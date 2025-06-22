///////////////////////////////////
//2025/02/17 
//nanashima particleEmitter.cpp
//////////////////////////////////
#include"main.h"
#include"renderer.h"
#include"particleEmitter.h"
#include"scene.h"
#include"camera.h"
#include"manager.h"

void ParticleEmitter::Init()
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
		LoadFromWICFile(L"asset\\texture\\particle.png", WIC_FLAGS_NONE, &metadata, image);
		CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &m_Texture);
		assert(m_Texture);

		Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\unlitTextureVS.cso");
		Renderer::CreatePixelShader(&m_PixelShader, "shader\\unlitTexturePS.cso");

	}
}
void ParticleEmitter::Uninit()
{

	m_VertexShader->Release();
	m_VertexBuffer->Release();
	m_Texture->Release();
	m_VertexLayout->Release();
	m_PixelShader->Release();
}

void ParticleEmitter::Update()
{
	//パーティクル発射
	for (int i = 0; i < MAXPARTICLE; i++)
	{
		if (m_Particle[i].Enable == false)
		{
			m_Particle[i].Enable = true;
			m_Particle[i].Life = 60;
			m_Particle[i].Position = m_Position;

			m_Particle[i].Velocity.x = (rand()%100 - 50) / 500.0f;
			m_Particle[i].Velocity.y = (rand()%100 + 50) / 500.0f;
			m_Particle[i].Velocity.z = (rand()%100 - 50) / 500.0f;
			break;
		}
	}
	for (int i = 0; i < MAXPARTICLE; i++)
	{
		if (m_Particle[i].Enable == true)
		{
			m_Particle[i].Velocity.y += -0.01f;//重力

			m_Particle[i].Position.x += m_Particle[i].Velocity.x;
			m_Particle[i].Position.y += m_Particle[i].Velocity.y;
			m_Particle[i].Position.z += m_Particle[i].Velocity.z;
			m_Particle[i].Life--;
			if (m_Particle[i].Life==0)
			{
				m_Particle[i].Enable = false;
			}
		}
	}
}

void ParticleEmitter::Draw()
{

	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);
	Scene* scene = Manager::GetScene();
	Camera* camera = scene->GetGameObject<Camera>();
	XMMATRIX view = camera->GetViewMatrix();
	XMMATRIX invView;
	invView = XMMatrixInverse(nullptr, view);//逆行列
	invView.r[3].m128_f32[0] = 0;
	invView.r[3].m128_f32[1] = 0;
	invView.r[3].m128_f32[2] = 0;

	

	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(0.5, 0.1, 0.1, 1);
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	Renderer::SetDepthEnable(false);//zバッファ無効

	Renderer::SetBlendAddEnable(true);

	for (int i = 0; i < MAXPARTICLE; i++)
	{
		if (m_Particle[i].Enable==true)
		{
			XMMATRIX world, scale, rot, trans;
			scale = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
			//rot = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
			trans = XMMatrixTranslation(m_Particle[i].Position.x, m_Particle[i].Position.y, m_Particle[i].Position.z);
			world = scale * invView * trans;
			Renderer::SetWorldMatrix(world);

			Renderer::GetDeviceContext()->Draw(4, 0);
		
		}
	
	}
	Renderer::SetDepthEnable(true);//zバッファ有効
	Renderer::SetBlendAddEnable(false);//加算合成無効
	
}