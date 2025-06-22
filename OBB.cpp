#include "OBB.h"
#include "GameObject.h"
#include <DirectXMath.h>
#include <vector>
#include <limits>
// OBB構造体（計算に使うデータ）

// GameObjectからOBBを生成する関数
OBBclass::OBB OBBclass::CreateOBBFromGameObject(GameObject* obj) {
    OBB obb;
    obb.position = obj->GetPosition();
    obb.halfExtents = obj->GetScale();

    // オイラー角 (XMFLOAT3) を取得
    XMFLOAT3 rotation = obj->GetRotation();

    // オイラー角 (ラジアン単位) から回転行列を生成
    XMMATRIX rotMatrix = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);

    // ローカル軸をワールド空間に変換
    XMVECTOR localAxes[3] = {
        XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f),
        XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f),
        XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f)
    };

    for (int i = 0; i < 3; ++i) {
        XMVECTOR worldAxis = XMVector3TransformNormal(localAxes[i], rotMatrix);
        XMStoreFloat3(&obb.axes[i], XMVector3Normalize(worldAxis));
    }

    return obb;
}

// SATでの分離軸テスト
bool OBBclass::SATTest(const OBB& obb1, const OBB& obb2, const XMFLOAT3& axis) {
    // 軸を正規化
    XMVECTOR axisVector = XMVector3Normalize(XMLoadFloat3(&axis));

    // 投影距離を計算
    float obb1Projection =
        obb1.halfExtents.x * fabsf(XMVectorGetX(XMVector3Dot(axisVector, XMLoadFloat3(&obb1.axes[0])))) +
        obb1.halfExtents.y * fabsf(XMVectorGetX(XMVector3Dot(axisVector, XMLoadFloat3(&obb1.axes[1])))) +
        obb1.halfExtents.z * fabsf(XMVectorGetX(XMVector3Dot(axisVector, XMLoadFloat3(&obb1.axes[2]))));

    float obb2Projection =
        obb2.halfExtents.x * fabsf(XMVectorGetX(XMVector3Dot(axisVector, XMLoadFloat3(&obb2.axes[0])))) +
        obb2.halfExtents.y * fabsf(XMVectorGetX(XMVector3Dot(axisVector, XMLoadFloat3(&obb2.axes[1])))) +
        obb2.halfExtents.z * fabsf(XMVectorGetX(XMVector3Dot(axisVector, XMLoadFloat3(&obb2.axes[2]))));

    // 中心間距離を計算
    XMVECTOR centerDiff = XMLoadFloat3(&obb2.position) - XMLoadFloat3(&obb1.position);
    float centerProjection = fabsf(XMVectorGetX(XMVector3Dot(centerDiff, axisVector)));

    // 投影が重なっているか判定
    return centerProjection <= (obb1Projection + obb2Projection);
}

// OBBの当たり判定関数
bool OBBclass::CheckOBBCollision( GameObject* obj1,  GameObject* obj2) {
    // OBBを生成
    
    OBB obb1 = CreateOBBFromGameObject(obj1);
    OBB obb2 = CreateOBBFromGameObject(obj2);

    // すべての分離軸をチェック（3軸 * 2オブジェクト + 交差軸）
    std::vector<XMFLOAT3> axes;

    // 各オブジェクトのローカル軸
    axes.push_back(obb1.axes[0]);
    axes.push_back(obb1.axes[1]);
    axes.push_back(obb1.axes[2]);
    axes.push_back(obb2.axes[0]);
    axes.push_back(obb2.axes[1]);
    axes.push_back(obb2.axes[2]);

    // 交差軸 (外積で計算)
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            XMVECTOR cross = XMVector3Cross(XMLoadFloat3(&obb1.axes[i]), XMLoadFloat3(&obb2.axes[j]));
            XMFLOAT3 crossAxis;
            XMStoreFloat3(&crossAxis, cross);
            axes.push_back(crossAxis);
        }
    }

    // 分離軸テスト
    for (const auto& axis : axes) {
        if (!SATTest(obb1, obb2, axis)) {
            return false; // 分離軸が見つかった場合、衝突していない
        }
    }

    return true; // すべての軸で重なっている場合、衝突している
}
using namespace DirectX;



// スクリーン座標からレイを生成する関数
OBBclass::Ray OBBclass::ScreenToRay(int mouseX, int mouseY, int screenWidth, int screenHeight, const XMMATRIX& viewMatrix, const XMMATRIX& projMatrix) {
    Ray ray;

    // スクリーン空間 (-1, 1) に正規化
    float normalizedX = (2.0f * mouseX) / screenWidth - 1.0f;
    float normalizedY = 1.0f - (2.0f * mouseY) / screenHeight;

    // デバッグ用出力
    //std::cout << "Normalized Coordinates: (" << normalizedX << ", " << normalizedY << ")" << std::endl;

    // レイのクリップ空間座標
    XMVECTOR rayClip = XMVectorSet(normalizedX, normalizedY, 1.0f, 1.0f);

    // 投影行列の逆行列でビュー空間に変換
    XMMATRIX invProjMatrix = XMMatrixInverse(nullptr, projMatrix);
    if (XMMatrixIsIdentity(invProjMatrix)) {
       // std::cerr << "Error: Projection matrix is not invertible!" << std::endl;
    }

    XMVECTOR rayView = XMVector4Transform(rayClip, invProjMatrix);
    rayView = XMVectorSetW(rayView, 0.0f); // 視点方向のベクトル (クリップ空間 → ビュー空間)

    // ビュー行列の逆行列でワールド空間に変換
    XMMATRIX invViewMatrix = XMMatrixInverse(nullptr, viewMatrix);
    if (XMMatrixIsIdentity(invViewMatrix)) {
      //  std::cerr << "Error: View matrix is not invertible!" << std::endl;
    }

    XMVECTOR rayOrigin = XMVector3Transform(XMVectorZero(), invViewMatrix); // 視点（カメラ位置）
    XMVECTOR rayDirection = XMVector3Normalize(XMVector3TransformNormal(rayView, invViewMatrix)); // レイの方向

    // XMVECTOR → XMFLOAT3 に変換
    XMStoreFloat3(&ray.origin, rayOrigin);
    XMStoreFloat3(&ray.direction, rayDirection);

    // デバッグ用出力
   // std::cout << "Ray Origin: (" << ray.origin.x << ", " << ray.origin.y << ", " << ray.origin.z << ")" << std::endl;
    //std::cout << "Ray Direction: (" << ray.direction.x << ", " << ray.direction.y << ", " << ray.direction.z << ")" << std::endl;

    return ray;
}


// レイとOBBの交差判定
bool OBBclass::RayIntersectsOBB(const Ray& ray, const OBBclass::OBB& obb, float& distance) {
    XMVECTOR rayOrigin = XMLoadFloat3(&ray.origin);
    XMVECTOR rayDir = XMVector3Normalize(XMLoadFloat3(&ray.direction));
    XMVECTOR obbCenter = XMLoadFloat3(&obb.position);

    float tMin = 0.0f;
    float tMax = std::numeric_limits<float>::max();

    for (int i = 0; i < 3; ++i) {
        XMVECTOR axis = XMLoadFloat3(&obb.axes[i]);
        float e = XMVectorGetX(XMVector3Dot(axis, obbCenter - rayOrigin));
        float f = XMVectorGetX(XMVector3Dot(axis, rayDir));

        if (fabsf(f) > 1e-6f) {
            // 軸ごとにOBBの半径を取得
            float obbRadius = (&obb.halfExtents.x)[i]; // X, Y, Zの順にアクセス
            float t1 = (e - obbRadius) / f;
            float t2 = (e + obbRadius) / f;

            if (t1 > t2) std::swap(t1, t2);
            tMin = std::max(tMin, t1);
            tMax = std::min(tMax, t2);

            if (tMin > tMax) return false;
        }
        else if (-e - (&obb.halfExtents.x)[i] > 0.0f || -e + (&obb.halfExtents.x)[i] < 0.0f) {
            return false;
        }
    }

    distance = tMin;
    return true;
}

// マウスクリックされた物体を取得
GameObject* OBBclass::GetClickedObject(int mouseX, int mouseY, int screenWidth, int screenHeight, const XMMATRIX& viewMatrix, const XMMATRIX& projMatrix, const std::vector<GameObject*>& objects) {
    // レイを生成
    Ray ray = ScreenToRay(mouseX, mouseY, screenWidth, screenHeight, viewMatrix, projMatrix);

    GameObject* closestObject = nullptr;
    float closestDistance = std::numeric_limits<float>::max();

    for (GameObject* obj : objects) {
        OBBclass::OBB obb = OBBclass::CreateOBBFromGameObject(obj);
        float distance = 0.0f;

        if (RayIntersectsOBB(ray, obb, distance)) {
            if (distance < closestDistance) {
                closestDistance = distance;
                closestObject = obj;
            }
        }
    }

    return closestObject;
}
#include <windows.h>

// マウス位置を取得する関数
POINT OBBclass::GetMousePosition(HWND hwnd)
{
    POINT mousePos;
    // マウスの画面座標を取得
    if (GetCursorPos(&mousePos))
    {
        // ウィンドウのクライアント座標に変換
        ScreenToClient(hwnd, &mousePos);
    }
    return mousePos;
}