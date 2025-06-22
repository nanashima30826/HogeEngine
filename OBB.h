///////////////////////////////////
//2025/02/17 
//nanashima OBB.h
//////////////////////////////////
#pragma once
#include <DirectXMath.h>
#include <vector>
#include <algorithm>
#include"scene.h"
using namespace DirectX;
class OBBclass
{
   
public: struct OBB {
        XMFLOAT3 position;   // ���S���W
        XMFLOAT3 axes[3];    // �e���̕����x�N�g��
        XMFLOAT3 halfExtents;
       
        // �X�P�[���i�e�������̔����j
    };
      // ���C�\����
      struct Ray {
          XMFLOAT3 origin;
          XMFLOAT3 direction;
      }; 
      static POINT GetMousePosition(HWND hwnd);
      static OBBclass::OBB CreateOBBFromGameObject(GameObject* obj);// GameObject����OBB�𐶐�����֐�
      static bool SATTest(const OBBclass::OBB& obb1, const OBBclass::OBB& obb2, const XMFLOAT3& axis);// SAT�ł̕������e�X�g
      static bool CheckOBBCollision( GameObject* obj1,  GameObject* obj2);// OBB�̓����蔻��֐�
      static Ray ScreenToRay(int mouseX, int mouseY, int screenWidth, int screenHeight, const XMMATRIX& viewMatrix, const XMMATRIX& projMatrix);// �X�N���[�����W���烌�C�𐶐�����֐�

        static  bool RayIntersectsOBB(const Ray& ray, const OBBclass::OBB& obb, float& distance);// ���C��OBB�̌�������


      static GameObject* GetClickedObject(int mouseX, int mouseY, int screenWidth, int screenHeight, const XMMATRIX& viewMatrix, const XMMATRIX& projMatrix, const std::vector<GameObject*>& objects);

};


