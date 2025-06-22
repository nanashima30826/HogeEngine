#include "OBB.h"
#include "GameObject.h"
#include <DirectXMath.h>
#include <vector>
#include <limits>
// OBB�\���́i�v�Z�Ɏg���f�[�^�j

// GameObject����OBB�𐶐�����֐�
OBBclass::OBB OBBclass::CreateOBBFromGameObject(GameObject* obj) {
    OBB obb;
    obb.position = obj->GetPosition();
    obb.halfExtents = obj->GetScale();

    // �I�C���[�p (XMFLOAT3) ���擾
    XMFLOAT3 rotation = obj->GetRotation();

    // �I�C���[�p (���W�A���P��) �����]�s��𐶐�
    XMMATRIX rotMatrix = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);

    // ���[�J���������[���h��Ԃɕϊ�
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

// SAT�ł̕������e�X�g
bool OBBclass::SATTest(const OBB& obb1, const OBB& obb2, const XMFLOAT3& axis) {
    // ���𐳋K��
    XMVECTOR axisVector = XMVector3Normalize(XMLoadFloat3(&axis));

    // ���e�������v�Z
    float obb1Projection =
        obb1.halfExtents.x * fabsf(XMVectorGetX(XMVector3Dot(axisVector, XMLoadFloat3(&obb1.axes[0])))) +
        obb1.halfExtents.y * fabsf(XMVectorGetX(XMVector3Dot(axisVector, XMLoadFloat3(&obb1.axes[1])))) +
        obb1.halfExtents.z * fabsf(XMVectorGetX(XMVector3Dot(axisVector, XMLoadFloat3(&obb1.axes[2]))));

    float obb2Projection =
        obb2.halfExtents.x * fabsf(XMVectorGetX(XMVector3Dot(axisVector, XMLoadFloat3(&obb2.axes[0])))) +
        obb2.halfExtents.y * fabsf(XMVectorGetX(XMVector3Dot(axisVector, XMLoadFloat3(&obb2.axes[1])))) +
        obb2.halfExtents.z * fabsf(XMVectorGetX(XMVector3Dot(axisVector, XMLoadFloat3(&obb2.axes[2]))));

    // ���S�ԋ������v�Z
    XMVECTOR centerDiff = XMLoadFloat3(&obb2.position) - XMLoadFloat3(&obb1.position);
    float centerProjection = fabsf(XMVectorGetX(XMVector3Dot(centerDiff, axisVector)));

    // ���e���d�Ȃ��Ă��邩����
    return centerProjection <= (obb1Projection + obb2Projection);
}

// OBB�̓����蔻��֐�
bool OBBclass::CheckOBBCollision( GameObject* obj1,  GameObject* obj2) {
    // OBB�𐶐�
    
    OBB obb1 = CreateOBBFromGameObject(obj1);
    OBB obb2 = CreateOBBFromGameObject(obj2);

    // ���ׂĂ̕��������`�F�b�N�i3�� * 2�I�u�W�F�N�g + �������j
    std::vector<XMFLOAT3> axes;

    // �e�I�u�W�F�N�g�̃��[�J����
    axes.push_back(obb1.axes[0]);
    axes.push_back(obb1.axes[1]);
    axes.push_back(obb1.axes[2]);
    axes.push_back(obb2.axes[0]);
    axes.push_back(obb2.axes[1]);
    axes.push_back(obb2.axes[2]);

    // ������ (�O�ςŌv�Z)
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            XMVECTOR cross = XMVector3Cross(XMLoadFloat3(&obb1.axes[i]), XMLoadFloat3(&obb2.axes[j]));
            XMFLOAT3 crossAxis;
            XMStoreFloat3(&crossAxis, cross);
            axes.push_back(crossAxis);
        }
    }

    // �������e�X�g
    for (const auto& axis : axes) {
        if (!SATTest(obb1, obb2, axis)) {
            return false; // �����������������ꍇ�A�Փ˂��Ă��Ȃ�
        }
    }

    return true; // ���ׂĂ̎��ŏd�Ȃ��Ă���ꍇ�A�Փ˂��Ă���
}
using namespace DirectX;



// �X�N���[�����W���烌�C�𐶐�����֐�
OBBclass::Ray OBBclass::ScreenToRay(int mouseX, int mouseY, int screenWidth, int screenHeight, const XMMATRIX& viewMatrix, const XMMATRIX& projMatrix) {
    Ray ray;

    // �X�N���[����� (-1, 1) �ɐ��K��
    float normalizedX = (2.0f * mouseX) / screenWidth - 1.0f;
    float normalizedY = 1.0f - (2.0f * mouseY) / screenHeight;

    // �f�o�b�O�p�o��
    //std::cout << "Normalized Coordinates: (" << normalizedX << ", " << normalizedY << ")" << std::endl;

    // ���C�̃N���b�v��ԍ��W
    XMVECTOR rayClip = XMVectorSet(normalizedX, normalizedY, 1.0f, 1.0f);

    // ���e�s��̋t�s��Ńr���[��Ԃɕϊ�
    XMMATRIX invProjMatrix = XMMatrixInverse(nullptr, projMatrix);
    if (XMMatrixIsIdentity(invProjMatrix)) {
       // std::cerr << "Error: Projection matrix is not invertible!" << std::endl;
    }

    XMVECTOR rayView = XMVector4Transform(rayClip, invProjMatrix);
    rayView = XMVectorSetW(rayView, 0.0f); // ���_�����̃x�N�g�� (�N���b�v��� �� �r���[���)

    // �r���[�s��̋t�s��Ń��[���h��Ԃɕϊ�
    XMMATRIX invViewMatrix = XMMatrixInverse(nullptr, viewMatrix);
    if (XMMatrixIsIdentity(invViewMatrix)) {
      //  std::cerr << "Error: View matrix is not invertible!" << std::endl;
    }

    XMVECTOR rayOrigin = XMVector3Transform(XMVectorZero(), invViewMatrix); // ���_�i�J�����ʒu�j
    XMVECTOR rayDirection = XMVector3Normalize(XMVector3TransformNormal(rayView, invViewMatrix)); // ���C�̕���

    // XMVECTOR �� XMFLOAT3 �ɕϊ�
    XMStoreFloat3(&ray.origin, rayOrigin);
    XMStoreFloat3(&ray.direction, rayDirection);

    // �f�o�b�O�p�o��
   // std::cout << "Ray Origin: (" << ray.origin.x << ", " << ray.origin.y << ", " << ray.origin.z << ")" << std::endl;
    //std::cout << "Ray Direction: (" << ray.direction.x << ", " << ray.direction.y << ", " << ray.direction.z << ")" << std::endl;

    return ray;
}


// ���C��OBB�̌�������
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
            // �����Ƃ�OBB�̔��a���擾
            float obbRadius = (&obb.halfExtents.x)[i]; // X, Y, Z�̏��ɃA�N�Z�X
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

// �}�E�X�N���b�N���ꂽ���̂��擾
GameObject* OBBclass::GetClickedObject(int mouseX, int mouseY, int screenWidth, int screenHeight, const XMMATRIX& viewMatrix, const XMMATRIX& projMatrix, const std::vector<GameObject*>& objects) {
    // ���C�𐶐�
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

// �}�E�X�ʒu���擾����֐�
POINT OBBclass::GetMousePosition(HWND hwnd)
{
    POINT mousePos;
    // �}�E�X�̉�ʍ��W���擾
    if (GetCursorPos(&mousePos))
    {
        // �E�B���h�E�̃N���C�A���g���W�ɕϊ�
        ScreenToClient(hwnd, &mousePos);
    }
    return mousePos;
}