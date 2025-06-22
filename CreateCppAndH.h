///////////////////////////////////
//2025/02/17 
//nanashima CreateCppAndH.h
//////////////////////////////////
#pragma once
#include <string>  // �ǉ��Fstd::string���g�p���邽�߂̃C���N���[�h
#include<DirectXMath.h>
#include <vector>
#include <utility>  // std::pair
#include <map>
using namespace DirectX;

class CreateCppAndH
{
	
	static bool m_change;
	 static std::string m_filepass;
	static std::string m_filename;
	static std::string m_classname;
	static std::string m_model;
	static std::string m_vs;
	static std::string m_ps;
	static XMFLOAT3 m_scale;
	static XMFLOAT3 m_position;
	static XMFLOAT3 m_rotation;
	static std::vector<bool> oldcollision;//1�t���[���O�̃R���W�����ݒ�
	static int selectedMode ; // �I�𒆂̃I�v�V�����̃C���f�b�N�X
	static std::vector<std::pair<std::string, bool>> pairedData;//�����蔻��p�̔z��
public:
	static void GenerateCppFile(const std::string& className);//�V����cpp����
	
	
	static void Init();//������
	static void Render();//�`��
	static void GenerateHeaderFile(const std::string& className);//�V����.h����
	static void ShowFileDialog();//�V�F�[�_�[���̕ύX�p�_�C�A���O
	static bool findclass();
	static std::string ShowInputDialog();//�V���ȃt�@�C�����̎擾
	static void SetFileName(std::string filename);
	static void SetFilePass(std::string filepass);
	static void SetModel(std::string model);
	static void SetPS(std::string ps);
	static void SetVS(std::string ps);
	static const char* GetModel();
	static const char* GetVS();
	static const char* GetPS();
	static std::string GetFileName();
	static std::string GetFilePass();
	static void SetClassName(std::string classname);//�N���X���ݒ�
	static std::string GetClassNama();//���ݑI�𒆂̃N���X���O�擾�@GetClass"Nama"�̓^�C�v�~�X�ł͂Ȃ�

	static std::string LayerToString(int layer);//���������C���[�̕����ɕς���

	static std::string XMFLOAT3ToString(const DirectX::XMFLOAT3& vec);

	static void WriteUpdetaCode(const std::string& updetatarget);//game��updeta�����ɑ΂��鏑������

	static void WriteNewCode(const DirectX::XMFLOAT3 targetpos, const DirectX::XMFLOAT3 targetsize, const DirectX::XMFLOAT3 targetrotation, int layer);//game��Init�����ɑ΂��鏑������

	static void AddFileToProject(const std::string& projectFile, const std::string& fileToAdd);//�\�����[�V�����t�@�C���ɒǉ�����

	static void Recompile();//�ċN���R���p�C������
	
	
	
	static void DetailClass();//�N���X�̏ڍׂ�\������
	static void SetPair(std::vector<std::pair<std::string, bool>> pairs);//�����蔻��Ǘ��p�̕ϐ��̃Z�b�g
	
	static  std::vector<std::pair<std::string, bool>>GetPair();
	static bool GetChange();
	static void SetScale(XMFLOAT3 scale);
	static void SetRotation(XMFLOAT3 rotation);
	static void SetPosition(XMFLOAT3 position);
	static XMFLOAT3 GetScale();
	static XMFLOAT3 GetPosition();
	static XMFLOAT3 GetRotation();
	static void modifyStaticVariables(const std::string& FilePath, std::string VS, std::string PS, std::string Model);//�V�F�[�_�[���̏�������
	static void SetChange(bool change);//�ݒ肪�ύX���ꂽ��ݒ�
	static std::string escapeBackslashes(const std::string& input);//�ǉ������p�X��escape����
	
	static void ColisionListBox(const std::vector<std::string> &classlist);//�����蔻��̐ݒ�p�`�F�b�N�{�b�N�X
	static void GuiSetScale();//�傫���ύX�_�C�A���O
	static void GuiSetPosition();//�ʒu�ύX�_�C�A���O
	static void GuiSetRotation();//��]�ύX�_�C�A���O
	static void RemoveNthLineAfterString(const std::string& target, size_t targetNumber);//�I�u�W�F�N�g�̍폜
	static void ModeSelect();//�I���A�폜�A�쐬�̑I��
	static int GetSelectedMode();//���I�΂�Ă��郂�[�h
	static void ChangePositionSizeRotation(const std::string& target, size_t targetNumber, XMFLOAT3 pos, XMFLOAT3 size, XMFLOAT3 rot);//�ړ��A�g��k���A��]�̐ݒ�
	static void CollisionWrite(const std::string& FilePath, bool collision);//�e�N���X�̃t�@�C���ɃR���W�����̐^�U��ݒ肷��
	static void InsertInitCollision(const std::string& inittarget);//�R���W�����̏�������game�ɏ�������
	//�����蔻��̏�������
};

