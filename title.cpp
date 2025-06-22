




#include <filesystem> 

#include"main.h"
#include"manager.h"
#include"renderer.h"
#include "title.h"
#include"input.h"

#include"particleEmitter.h"


#include"meshField.h"
#include"TitleObject.h"
#include"game.h"


#include"Weve.h"




///////////////////////////////////
//2025/02/17 
//nanashima Title.cpp
//////////////////////////////////
void Title::Init()
{
	


	AddGameObject<Camera>(e_Camera);
	AddGameObject<Player>(e_Object)->SetPosition(XMFLOAT3(-1, 28, -104.5));
	AddGameObject<Sky>(e_Object);
	AddGameObject<Field>(e_Object)->SetScale(XMFLOAT3(3,3,3));
	
	AddGameObject<TitleObject>(e_Object)->SetPosition(XMFLOAT3(0, 40, 20));
	Weve* weve = AddGameObject<Weve>(e_Object); 
	weve->SetPosition(XMFLOAT3(0, 2, 0));
	weve->SetScale(XMFLOAT3(3, 3, 3));
	AddGameObject<ParticleEmitter>(e_Object)->SetPosition(XMFLOAT3(70, 65, 20));
	


	AddGameObject<meshField>(e_Object);
	


}

void Title::Update()
{




	Scene::Update();
	if (Input::GetKeyTrigger(VK_RETURN))
	{
		Manager::SetScene<Game>();
	}


	
}