///////////////////////////////////
//2025/02/17 
//nanashima box.h
//////////////////////////////////
#pragma once
#include"main.h"
#include"GameObject.h"
#include"component.h"
class Box:public GameObject
{
private:

	Component* m_Component{};


	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	static std::string m_classtexture;
	static std::string m_classmodel;
	static std::string m_classVS;
	static std::string m_classPS;
	static  bool m_collision;
public:
	void Init()override;
	void DetailChange(std::string modelortexture, std::string vs, std::string ps) override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	static std::string GetClassModel();
	static std::string GetClassVS();
	static std::string GetClassPS();

	void SetTexture(std::string texture);
	void SetModel(const std::string model);
	void SetVS(const std::string vs);
	void SetPS(const std::string ps);
	const char* GetTexture();
	const char* GetModel();
	const char* GetVS();
	const char* GetPS();
	static bool GetCollision();

	
};

