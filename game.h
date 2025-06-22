///////////////////////////////////
//2025/02/17 
//nanashima game.h�@�Q�[���G���W������
//////////////////////////////////
#pragma once
#include"scene.h"
#include <memory>

#define FovAngleY (1)//����p
#define NearZ (1)//�ǂ�����`�悷�邩
#define FarZ (1000)//�ǂ��܂ŕ`�悷�邩
class Game:public Scene
{
	static bool initcollision;//�����蔻��̏������@��x�����ĂԂ��߂̃t���O
	Cursor* pcursor;
	GameObject* oldobject=nullptr;
	static Game* m_game;//�C���X�^���X
	static std::vector<std::pair<std::string, bool>> InitPairedData;//�����蔻��Ǘ��p�̏����l
public:
	
	void Init()override;

	
	void Update()override;

	void SetGame();//Game�̃C���X�^���X��o�^
	
	static std::vector<std::pair<std::string, bool>> GetInitPaired();//InitPairedData�̃Q�b�^�[
	template<typename T>
	void InitCollision();
		
	
	
	template<typename T, typename T2>
	void ClassCollision();//T��T2�̓����蔻��
	
};
	 Game* GetGame();//�O������Game�̃C���X�^���X���擾����֐�
