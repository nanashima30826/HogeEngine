
///////////////////////////////////
//2025/02/17 
//nanashima game.cpp
//////////////////////////////////
#include <filesystem> 

#include"main.h"
#include"manager.h"
#include"renderer.h"
#include "game.h"
#include"input.h"
#include"result.h"
#include"particleEmitter.h"


#include"meshField.h"
#include"CreateCppAndH.h"
#include"Weve.h"
#include <memory>
#include <unordered_map>
#include"OBB.h"

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
Game* Game::m_game{};
Game* g_game;
Camera* g_camera;
std::vector<GameObject*> objects;
bool Game::initcollision = true;
std::vector<std::pair<std::string, bool>> Game::InitPairedData;
void Game::Init()
{
	SetGame();


	g_camera=AddGameObject<Camera>(e_Camera);
   
	
	// INSERT HERE
Box* Boxp0= AddGameObject<Box>(e_Object); // CLASSADD
Boxp0->SetPosition(XMFLOAT3(0, 0, 0)); 
Boxp0->SetScale(XMFLOAT3(1, 1, 1));
Boxp0->SetRotation(XMFLOAT3(0, 0, 0));












	AddGameObject<meshField>(e_Object);
	
	pcursor = AddGameObject<Cursor>(e_Object);
	

    InitPairedData = CreateCppAndH::GetPair();
    //InitCollisions
   
    InitCollision<Box>();
    InitCollision<Camera>();
    InitCollision<meshField>();
    InitCollision<Cursor>();
    CreateCppAndH::SetPair(InitPairedData);
}


void Game::Update()
{
    int Select = CreateCppAndH::GetSelectedMode();
    XMFLOAT3 addpotision = pcursor->GetPosition();
    XMFLOAT3 addscale = pcursor->GetScale();
    XMFLOAT3 addrotation = pcursor->GetRotation();

    XMMATRIX projMatrix = XMMatrixPerspectiveFovLH(FovAngleY, (float)SCREEN_WIDTH / SCREEN_HEIGHT, NearZ, FarZ);


    //クリックされたオブジェクト
    GameObject* clickedObject = OBBclass::GetClickedObject(OBBclass::GetMousePosition(GetWindow()).x, OBBclass::GetMousePosition(GetWindow()).y, SCREEN_WIDTH, SCREEN_HEIGHT, g_camera->GetViewMatrix(), projMatrix, GetAllNonCameraObjects());

    if (clickedObject && Input::GetKeyTrigger(VK_LBUTTON) && Select == 1 && !ImGui::IsAnyItemHovered()) {
        //	 //クリックされたオブジェクトに対する処理
        Camera* cameraclass = dynamic_cast<Camera*>(clickedObject);
        meshField* meshfildclass = dynamic_cast<meshField*>(clickedObject);

        if (cameraclass || meshfildclass) {
            //この二つは消さない
        }


        else {


            int i = -1; // 初期値を無効な値に設定
            for (int index = 0; auto gameobj : GetALLObject()) {
                if (clickedObject == gameobj) {
                    i = index;
                    CreateCppAndH::RemoveNthLineAfterString("CLASSADD", i - 1);
                    break;
                }
                index++;
            }

            if (i != -1) {
                //ここに来たらエラーである

            }
            clickedObject->SetDestroy();


        }
    }
    if (clickedObject && Input::GetKeyTrigger(VK_LBUTTON) && Select == 0 && !ImGui::IsAnyItemHovered()) {
        //	 //クリックされたオブジェクトに対する処理
        if (Camera* cameraclass = dynamic_cast<Camera*>(clickedObject)) {

        }

        else {



            clickedObject->SetScale(CreateCppAndH::GetScale());
            clickedObject->SetPosition(CreateCppAndH::GetPosition());
            clickedObject->SetRotation(CreateCppAndH::GetRotation());



            int i = -1; // 初期値を無効な値に設定
            int index = 0; // インデックスをループ外で管理
            for (auto gameobj : GetALLObject()) {
                if (clickedObject == gameobj) {
                    i = index;
                }
            }

                // i が -1 のままなら無効な値として処理しない
                if (i <= 0) {

                }
            }

        }
        // Updateif

        ClassCollision<Camera, Box>();
        ClassCollision<Cursor, Box>();
        ClassCollision<meshField, Box>();

        if (CreateCppAndH::GetClassNama() == "Box")
        {
            std::string modelname = CreateCppAndH::GetModel();
            std::string vsname = CreateCppAndH::GetVS();
            std::string psname = CreateCppAndH::GetPS();
            if (modelname.empty() || !std::filesystem::exists(modelname)) {
                modelname = CreateCppAndH::escapeBackslashes(Box::GetClassModel());
            }
            if (vsname.empty() || !std::filesystem::exists(vsname)) {
                vsname = CreateCppAndH::escapeBackslashes(Box::GetClassVS());
            }
            if (psname.empty() || !std::filesystem::exists(psname)) {
                psname = CreateCppAndH::escapeBackslashes(Box::GetClassPS());
            }
            if (CreateCppAndH::GetChange() == true) {
                std::vector<Box*> BoxList = GetGameObjects<Box>();
                for (Box* pBox : BoxList) {
                    pBox->DetailChange(modelname, vsname, psname);
                    CreateCppAndH::modifyStaticVariables(CreateCppAndH::GetFileName(), vsname, psname, modelname);
                }
            }
            if (Input::GetKeyTrigger(VK_LBUTTON) && !ImGui::IsAnyItemHovered() && Select == 2) {
                CreateCppAndH::WriteNewCode(addpotision, CreateCppAndH::GetScale(), CreateCppAndH::GetRotation(), e_Object);
                Box* addobj = AddGameObject<Box>(e_Object);
                addobj->SetPosition(addpotision);
                addobj->SetScale(CreateCppAndH::GetScale());
                addobj->SetRotation(CreateCppAndH::GetRotation());
            }
        }
        ClassCollision<Box, Cursor>();
        ClassCollision<Camera, Cursor>();
        ClassCollision<meshField, Cursor>();

        if (CreateCppAndH::GetClassNama() == "Cursor")
        {

            std::string modelname = CreateCppAndH::GetModel();
            std::string vsname = CreateCppAndH::GetVS();
            std::string psname = CreateCppAndH::GetPS();
            if (modelname.empty() || !std::filesystem::exists(modelname)) {
                modelname = CreateCppAndH::escapeBackslashes(Cursor::GetClassModel());
            }
            if (vsname.empty() || !std::filesystem::exists(vsname)) {
                vsname = CreateCppAndH::escapeBackslashes(Cursor::GetClassVS());
            }
            if (psname.empty() || !std::filesystem::exists(psname)) {
                psname = CreateCppAndH::escapeBackslashes(Cursor::GetClassPS());
            }
            if (CreateCppAndH::GetChange() == true) {
                std::vector<Cursor*> CursorList = GetGameObjects<Cursor>();
                for (Cursor* pCursor : CursorList) {
                    pCursor->DetailChange(modelname, vsname, psname);
                    CreateCppAndH::modifyStaticVariables(CreateCppAndH::GetFileName(), vsname, psname, modelname);
                }
            }
            if (Input::GetKeyTrigger(VK_LBUTTON) && !ImGui::IsAnyItemHovered() && Select == 2) {
                CreateCppAndH::WriteNewCode(addpotision, CreateCppAndH::GetScale(), CreateCppAndH::GetRotation(), e_Object);
                Cursor* addobj = AddGameObject<Cursor>(e_Object);
                addobj->SetPosition(addpotision);
                addobj->SetScale(CreateCppAndH::GetScale());
                addobj->SetRotation(CreateCppAndH::GetRotation());
            }
        }

        initcollision = false;

        Scene::Update();
    
    
}
void Game::SetGame()
{
	m_game = this;
	g_game = m_game;
   
}
Game* GetGame()
{
	return g_game;
}

std::vector<std::pair<std::string, bool>> Game::GetInitPaired()
{
    
    return InitPairedData;
}

template<typename T>
void Game::InitCollision()
{
    InitPairedData.resize(g_game->GetClassALL().size());
    for (size_t i = 0; i < g_game->GetClassALL().size() ;i++)
    {
        InitPairedData[i].first = g_game->GetClassALL()[i];

        // 比較する型名を明示的に確認
        std::string typeName = typeid(T).name();
        const std::string prefix = "class ";
        auto pos = typeName.find(prefix);
        if (pos != std::string::npos) {
            typeName.erase(pos, prefix.length());
        }
        if (InitPairedData[i].first == typeName) // 型名が一致する場合
        {
            InitPairedData[i].second = T::GetCollision();
        }
    }
}

template <typename T, typename T2>
void Game::ClassCollision()
{
    for (auto& pair : CreateCppAndH::GetPair())
    {

        // 比較する型名を明示的に確認
        std::string typeName = typeid(T2).name();
        const std::string prefix = "class ";
        auto pos = typeName.find(prefix);
        if (pos != std::string::npos) {
            typeName.erase(pos, prefix.length());
        }

        if (pair.first == typeName) // 型名が一致する場合
        {
           
                
            if (initcollision)//当たり判定初期設定の反映
            {
                if (T2::GetCollision() == true)
                {   
                    
                    pair.second = true;
                }
                else
                {
                    pair.second = false;
                }
                
            }

             
            if (pair.second == true)
            {
                
              
                for (T* colisionclass : GetGameObjects<T>())
                {
                    for (T2* colisionclass2 : GetGameObjects<T2>())
                    {
                        if (OBBclass::CheckOBBCollision(colisionclass, colisionclass2))
                        {
                            //ぶつかったら停止
                            colisionclass2->Stop(colisionclass->GetPosition(), colisionclass->GetScale());

                            colisionclass->Stop(colisionclass2->GetPosition(), colisionclass2->GetScale());

                        }
                    }
                }
            }
        }

    }
}




