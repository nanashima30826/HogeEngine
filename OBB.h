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
        XMFLOAT3 position;   // 中心座標
        XMFLOAT3 axes[3];    // 各軸の方向ベクトル
        XMFLOAT3 halfExtents;
       
        // スケール（各軸方向の半幅）
    };
      // レイ構造体
      struct Ray {
          XMFLOAT3 origin;
          XMFLOAT3 direction;
      }; 
      static POINT GetMousePosition(HWND hwnd);
      static OBBclass::OBB CreateOBBFromGameObject(GameObject* obj);// GameObjectからOBBを生成する関数
      static bool SATTest(const OBBclass::OBB& obb1, const OBBclass::OBB& obb2, const XMFLOAT3& axis);// SATでの分離軸テスト
      static bool CheckOBBCollision( GameObject* obj1,  GameObject* obj2);// OBBの当たり判定関数
      static Ray ScreenToRay(int mouseX, int mouseY, int screenWidth, int screenHeight, const XMMATRIX& viewMatrix, const XMMATRIX& projMatrix);// スクリーン座標からレイを生成する関数

        static  bool RayIntersectsOBB(const Ray& ray, const OBBclass::OBB& obb, float& distance);// レイとOBBの交差判定


      static GameObject* GetClickedObject(int mouseX, int mouseY, int screenWidth, int screenHeight, const XMMATRIX& viewMatrix, const XMMATRIX& projMatrix, const std::vector<GameObject*>& objects);

};


