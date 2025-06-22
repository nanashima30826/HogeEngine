


#pragma once
#include "Cursor.h"
#include "Bullet.h"
#include "Box.h"
#include"GameObject.h"
#include"camera.h"
#include"polygon2D.h"
#include"field.h"
#include"player.h"
#include<list>
#include"explosion.h"
#include"sky.h"
///////////////////////////////////
//2025/02/17 
//nanashima scene.h
//////////////////////////////////


class Scene
{

protected:
	std::list<GameObject*> m_GameObject[3];
	std::list<std::string> m_ClassALL;


public:
	virtual ~Scene() {};//���X�g�������̂Ƀf�X�g���N�^���Ȃ��Ƃ���

	virtual void Init()
	{
		
	}
	template<typename T>//template�֐�
	T* AddGameObject(int Layer)//�I�u�W�F�N�g�̒ǉ�
	{
		T* gameObject = new T();
		gameObject->Init();
		m_GameObject[Layer].push_back(gameObject);

	
		std::string classname = typeid(T).name();
		const std::string prefix = "class ";
		auto pos = classname.find(prefix);
		if (pos != std::string::npos) {
			classname.erase(pos, prefix.length());
		}
		m_ClassALL.push_back(classname);
		return gameObject;
	}
	std::vector<std::string>GetClassALL()//�S�ăQ�[���I�u�W�F�N�g�N���X�̖��O�擾
	{
		std::vector<std::string>classList;
		// �\�[�g����i�K�v�ɉ����āj
		m_ClassALL.sort();

		// �אڂ���d���v�f���폜
		m_ClassALL.unique();
			for (std::string object : m_ClassALL)
			{
					classList.push_back(object);
			}
		return classList;
	}
	template<typename T>//template�֐�
	T* GetGameObject()//�I�u�W�F�N�g�擾
	{
		for (int i = 0; i < MAXLAYER; i++)
		{
			for (GameObject* object : m_GameObject[i])
			{
				T* ret = dynamic_cast<T*>(object);
				if (ret != nullptr)
					return ret;
			}
		}
		return nullptr;
	}
	template<typename T>//template�֐�
	std::vector<T*> GetGameObjects()//�I�u�W�F�N�g�̔z��擾
	{
		std::vector<T*>objectList;
		for (int i = 0; i < MAXLAYER; i++)
		{
			for (GameObject* object : m_GameObject[i])
			{
				T* ret = dynamic_cast<T*>(object);

				if (ret != nullptr)
					objectList.push_back(ret);
					
			}
		}
		return objectList;
	}
	std::vector<GameObject*>GetALLObject()//�S�I�u�W�F�N�g�̔z��擾
	{
		std::vector<GameObject*>objectList;
		for (int i = 0; i < MAXLAYER; i++)
		{
			for (GameObject* object : m_GameObject[i])
			{
				
				if (object != nullptr)
					objectList.push_back(object);

			}
		}
		return objectList;
	}
	std::vector<GameObject*> GetAllNonCameraObjects()//�J�����ȊO���擾
	{
		std::vector<GameObject*> objectList;

		for (int i = 0; i < MAXLAYER; i++)
		{
			for (GameObject* object : m_GameObject[i])
			{
				if (object != nullptr && dynamic_cast<Camera*>(object) == nullptr)
				{
					objectList.push_back(object);
				}
			}
		}
		return objectList;
	}
	virtual void Uninit()
	{
		for (int i = 0; i < MAXLAYER; i++)
		{
			for (GameObject* object : m_GameObject[i])
			{
				object->Uninit();
				delete object;

			}
		}
		
	}
	virtual void Draw()
	{
		for (int i = 0; i < MAXLAYER; i++)
		{
			for (GameObject* object : m_GameObject[i])
			{
				object->Draw();

			}
		}
	}
	virtual void Update()
	{
		for (int i = 0; i < MAXLAYER; i++)
		{
			for (GameObject* object : m_GameObject[i])
			{
				object->Update();

			}

			m_GameObject[i].remove_if([](GameObject* object) {return object->Destroy(); });
		}
	}
	
};