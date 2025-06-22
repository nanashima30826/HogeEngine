
#include"main.h"
#include"manager.h"
#include"renderer.h"
#include "result.h"
#include"input.h"
#include"title.h"
///////////////////////////////////
//2025/02/17 
//nanashima Result.cpp
//////////////////////////////////
void Result::Init()
{

	AddGameObject<Polygon2D>(e_UI);//�V�̌�ɂ�3d�̃I�u�W�F�N�g�͂����Ȃ�
}

void Result::Update()
{
	if (Input::GetKeyTrigger(VK_RETURN))
	{
		Manager::SetScene<Title>();
	}
}
