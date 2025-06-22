///////////////////////////////////
//2025/02/17 
//nanashima Weve.cpp
//////////////////////////////////
#include"main.h"
#include"renderer.h"
#include"Weve.h"
//----- 定数定義
#define FRAME_TIME				20						// フレームあたり時間

#define SCREEN_WIDTH			800						// スクリーン　幅
#define SCREEN_HEIGHT			600						// スクリーン　高さ
#define AXIS_X_OFFSET			SCREEN_WIDTH/2			// Ｘ座標オフセット
#define AXIS_Y_OFFSET			SCREEN_HEIGHT/2			// Ｙ座標オフセット

#define WAVE_PITCH				10						// ウェーブポイントの間隔
#define WAVE_QTY				(40+1)					// ウェーブポイントの総数
#define WAVE_AMPLITUDE			20.0f					// ウェーブポイントの振幅
#define WAVE_LENGTH				70.0f					// ウェーブポイントの波長
#define WAVE_CYCLE				5.0f	

float g_WeveHeight[21][21] =
{
	{10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10},
	{10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10},
	{10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10},
	{10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10},
	{10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10},
	{10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10},
	{10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10},
	{10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10},
	{10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10},
	{10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10},
	{10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10},
	{10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10},
	{10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10},
	{10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10},
	{10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10},
	{10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10},
	{10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10},
	{10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10},
	{10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10},
	{10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10},
	{10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10},
	
	
	
};
void Weve::Init()
{
	//頂点バッファせいせい
	{
		for (int x = 0; x < 21; x++)
		{

			for (int z = 0; z < 21; z++)
			{
				m_vertex[x][z].Position = XMFLOAT3((x - 10) * 5, g_WeveHeight[x][z], (z - 10) * -5);
				m_vertex[x][z].Normal = XMFLOAT3(0, 1, 0);
				m_vertex[x][z].Diffuse = XMFLOAT4(1, 1, 1, 1);
				m_vertex[x][z].TexCoord = XMFLOAT2(x, z);


			}

		}

		for (int x = 1; x < 20; x++)
		{
			for (int z = 1; z < 20; z++)
			{
				XMFLOAT3 vx, vz, vn;
				vx.x = m_vertex[x + 1][z].Position.x - m_vertex[x - 1][z].Position.x;
				vx.y = m_vertex[x + 1][z].Position.y - m_vertex[x - 1][z].Position.y;
				vx.z = m_vertex[x + 1][z].Position.z - m_vertex[x - 1][z].Position.z;

				vz.x = m_vertex[x][z - 1].Position.x - m_vertex[x][z + 1].Position.x;
				vz.y = m_vertex[x][z - 1].Position.y - m_vertex[x][z + 1].Position.y;
				vz.z = m_vertex[x][z - 1].Position.z - m_vertex[x][z + 1].Position.z;

				vn.x = vz.y * vx.z - vz.z * vx.y;
				vn.y = vz.z * vx.x - vz.x * vx.z;
				vn.z = vz.x * vx.y - vz.y * vx.x;

				float len = sqrtf(vn.x * vn.x + vn.y * vn.y + vn.z * vn.z);
				vn.x /= len;
				vn.y /= len;
				vn.z /= len;

				m_vertex[x][z].Normal = vn;
			}
		}
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(VERTEX_3D) * 21 * 21;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = m_vertex;

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);
	}


	unsigned int index[((21 + 1) * 2) * 20 - 2];
	int i = 0;
	for (int x = 0; x < 20; x++)
	{
		for (int z = 0; z < 21; z++)
		{
			index[i] = x * 21 + z;
			i++;

			index[i] = (x + 1) * 21 + z;
			i++;
		}
		if (x == 19)
			break;
		index[i] = (x + 1) * 21 + 20;
		i++;
		index[i] = (x + 1) * 21;
		i++;


	}
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(unsigned int) * ((22 * 2) * 20 - 2);
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = index;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_IndexBuffer);

	TexMetadata metadata;
	ScratchImage image;
	LoadFromWICFile(L"asset\\texture\\44.JPG", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &m_Texture);
	assert(m_Texture);

	LoadFromWICFile(L"asset\\texture\\earthenvmap.png", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &m_EnvTexture);
	assert(m_EnvTexture);

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\weveVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\wevePS.cso");


}
void Weve::Uninit()
{

	m_VertexShader->Release();
	m_VertexBuffer->Release();
	m_Texture->Release();
	m_VertexLayout->Release();
	m_PixelShader->Release();
	m_EnvTexture->Release();
}

void Weve::Update()
{float wevelength = 5;
float amplitude =0.5f;
float wevecycle = 2;
	for (int x = 0; x < 21; x++)
	{
		for (int z = 0; z < 21; z++)
		{
			
			
			float dx= m_vertex[z][x].Position.x - m_vertex[0][0].Position.x;
			float dz = m_vertex[z][x].Position.z - m_vertex[0][0].Position.z;
			float length;
			
		
			length = sqrtf(dx * dx + dz * dz);

			m_vertex[z][x].Position.y = amplitude * sinf(2 * XM_PI * (length / wevelength - m_time / wevecycle));

			
		}
	}
	m_time += 0.1f;
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;
	//ここで頂点データを変更
	memcpy(vertex, m_vertex, sizeof(VERTEX_3D) * 21 * 21);

	Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);

}

void Weve::Draw()
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

	UINT stride = sizeof(VERTEX_3D);//頂点バッファ設定
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	Renderer::GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);//indexバッファ設定

	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1, 1, 1, 1);
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);
	Renderer::GetDeviceContext()->PSSetShaderResources(1, 1, &m_Texture);
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//Renderer::GetDeviceContext()->Draw(21*21, 0);
	Renderer::GetDeviceContext()->DrawIndexed((22 * 2) * 20 - 2, 0, 0);
}
float Weve::GetHeight(XMFLOAT3 Position)
{
	int x, z;

	x = Position.x / 5 + 10;
	z = Position.z / -5 + 10;

	XMFLOAT3 pos0, pos1, pos2, pos3;
	pos0 = m_vertex[x][z].Position;
	pos1 = m_vertex[x + 1][z].Position;
	pos2 = m_vertex[x][z + 1].Position;
	pos3 = m_vertex[x + 1][z + 1].Position;

	XMFLOAT3 v12, v1p;
	v12.x = pos2.x - pos1.x;
	v12.y = pos2.y - pos1.y;
	v12.z = pos2.z - pos1.z;

	v1p.x = Position.x - pos1.x;
	v1p.y = Position.y - pos1.y;
	v1p.z = Position.z - pos1.z;

	//gaiseki
	float cy = v12.z * v1p.x - v12.x * v1p.z;

	float py;

	XMFLOAT3 n;
	if (cy > 0)
	{
		XMFLOAT3 v10;
		v10.x = pos0.x - pos1.x;
		v10.y = pos0.y - pos1.y;
		v10.z = pos0.z - pos1.z;

		n.x = v10.y * v12.z - v10.z * v12.y;
		n.y = v10.z * v12.x - v10.x * v12.z;
		n.z = v10.x * v12.y - v10.y * v12.x;
	}
	else
	{
		XMFLOAT3 v13;
		v13.x = pos3.x - pos1.x;
		v13.y = pos3.y - pos1.y;
		v13.z = pos3.z - pos1.z;

		n.x = v12.y * v13.z - v12.z * v13.y;
		n.y = v12.z * v13.x - v12.x * v13.z;
		n.z = v12.x * v13.y - v12.y * v13.x;

	}
	py = -((Position.x - pos1.x) * n.x + (Position.z - pos1.z) * n.z) / n.y + pos1.y;
	return py;
}