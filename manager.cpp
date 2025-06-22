///////////////////////////////////
//2025/02/17 
//nanashima manager.cpp
//////////////////////////////////
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include"input.h"
#include"polygon2D.h"
#include"camera.h"
#include"field.h"

#include"component.h"
#include"player.h"
#include"scene.h"
#include"game.h"
#include"title.h"
#include"result.h"
#include"modelRenderer.h"
#include"audio.h"
#include"CreateCppAndH.h"
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

Scene* Manager::m_Scene{};//メンバをスタティックにしたらcpp側でもう一度宣言する
Scene* Manager::m_NextScene{};
void Manager::Init()
{
	
	Renderer::Init();

	Input::Init();
	Audio::InitMaster();
	
	m_Scene = new Title();
	m_Scene->Init();
	

	
	
}


void Manager::Uninit()
{
	
	m_Scene->Uninit();
	delete m_Scene;
	ModelRenderer::UnloadAll();
	Renderer::Uninit();
	Input::Uninit();
}

void Manager::Update()
{
	Input::Update();
	m_Scene->Update();
	
	
	
	
}

void Manager::Draw()
{
	Renderer::Begin();

	m_Scene->Draw();
	CreateCppAndH::Render();
	
	Renderer::End();

	if (m_NextScene!=nullptr)
	{
	m_Scene->Uninit();
	delete m_Scene;

	ModelRenderer::UnloadAll();
	m_Scene = m_NextScene;
	m_Scene->Init();

	m_NextScene = nullptr;
	}

}
