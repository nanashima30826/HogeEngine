///////////////////////////////////
//2025/02/17 
//nanashima manager.h
//////////////////////////////////
#pragma once
#include"GameObject.h"
#include"scene.h";
class Manager 
{
private:
	static Scene* m_Scene;//メンバをスタティックにしたらcpp側でもう一度宣言する
	static Scene* m_NextScene;

public:
	static void Init();
	static void Uninit();
	static void Update();
	static void Draw();
	static Scene* GetScene() { return m_Scene; }
	template<typename T>
	static void SetScene()
	{
		m_NextScene = new T;
	}


};