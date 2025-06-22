///////////////////////////////////
//2025/02/17 
//nanashima CreateCppAndH.cpp
//////////////////////////////////
#include <filesystem>
#include <string>
#include <vector>
#include"ImGuiFileDialog.h"
#include "main.h"
#include "renderer.h"
#include <io.h>
#include"game.h"
#include"manager.h"
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include"CreateCppAndH.h"

#include <cstdlib>
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <directxmath.h> // XMFLOAT3用

using namespace DirectX;
std::string CreateCppAndH::m_filepass="";
std::string CreateCppAndH::m_filename="";
std::string CreateCppAndH::m_classname="";
std::string CreateCppAndH::m_model="";
std::string CreateCppAndH::m_vs="";
std::string CreateCppAndH::m_ps="";
int CreateCppAndH::selectedMode=0; // 選択中のオプションのインデックス
 bool CreateCppAndH::m_change=false;
ImGuiFileDialog fileDialogcppandh;
ImGuiFileDialog fileDialogmodel;
ImGuiFileDialog fileDialogvs;
ImGuiFileDialog fileDialogps;
XMFLOAT3 CreateCppAndH::m_scale = XMFLOAT3(1,1,1);
XMFLOAT3 CreateCppAndH::m_position=XMFLOAT3(0,0,0);
XMFLOAT3 CreateCppAndH::m_rotation=XMFLOAT3(0,0,0);
std::vector<bool> CreateCppAndH::oldcollision = {};

std::vector<std::pair<std::string, bool>> CreateCppAndH::pairedData;

void CreateCppAndH::Init()
{

}
void CreateCppAndH::Render()
{
    if (Manager::GetScene() == GetGame())
    {
        // Start ImGui frame
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        CreateCppAndH::ModeSelect();


        CreateCppAndH::ShowFileDialog();
        std::string	className = CreateCppAndH::ShowInputDialog();
      
        CreateCppAndH::DetailClass();
        ImGui::Begin("Transform");
        CreateCppAndH::GuiSetScale();
        CreateCppAndH::GuiSetPosition();
        CreateCppAndH::GuiSetRotation();
        ImGui::End();
        CreateCppAndH::ColisionListBox(GetGame()->GetClassALL());

        // ImGuiの描画データをレンダーターゲットに描画
        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    }
    


}
void CreateCppAndH::GenerateHeaderFile(const std::string& className) {
    // ヘッダーファイルのパス
    if (className != "") {
        std::string headerFileName = className + ".h";
        std::ofstream headerFile(headerFileName);

        if (headerFile.is_open()) {
            headerFile << "#pragma once\n\n";
            headerFile << "#include \"main.h\"\n";
            headerFile << "#include \"GameObject.h\"\n";
            headerFile << "#include \"component.h\"\n";

            headerFile << "class " << className << ":public GameObject {\n";
            headerFile << "private:\n";
            headerFile << "    Component* m_Component{};\n";
            headerFile << "     ID3D11VertexShader* m_VertexShader{};\n";
            headerFile << "     ID3D11PixelShader* m_PixelShader{};\n";
            headerFile << "      ID3D11InputLayout* m_VertexLayout{};\n";
            headerFile<<"static std::string m_classtexture;\n";
            headerFile << "static std::string m_classmodel;\n";
            headerFile << "static std::string m_classVS;\n";
            headerFile << "static std::string m_classPS;\n";
            headerFile << "static  bool m_collision;\n";
            headerFile << "public:\n";
           
            headerFile << "     void Init()override;\n";
            headerFile << "void DetailChange(std::string modelortexture, std::string vs, std::string ps) override;\n";
            headerFile << "    void Uninit()override;\n";
            headerFile << "      void Update()override;\n";
            headerFile << "    void Draw()override;\n";
           
            headerFile << "static std::string GetClassModel();\n";
            headerFile << " static std::string GetClassVS();\n";
            headerFile << "static std::string GetClassPS();\n";
            headerFile << " void SetTexture(std::string texture);\n";
            headerFile << " void SetModel(const std::string model);\n";
            headerFile << " void SetVS(const std::string vs);\n";
            headerFile << " void SetPS(const std::string ps);\n";
            headerFile << "const char* GetTexture();\n";
            headerFile << "const char* GetModel();\n";
            headerFile << "const char* GetVS();\n";
            headerFile << "const char* GetPS();\n";
            headerFile << "static bool GetCollision();\n";
            headerFile << "};\n";
            headerFile.close();
        }
        AddFileToProject("GM31_Test.vcxproj", headerFileName);
    }
}

    void CreateCppAndH::GenerateCppFile(const std::string & className) {
        // ソースファイルのパス
        if (className != "") {
            std::string cppFileName = className + ".cpp";
            std::ofstream cppFile(cppFileName);

            if (cppFile.is_open()) {
                cppFile << "#include \"" << className << ".h\"\n";
                cppFile << "#include \"main.h\"\n";
                cppFile << "#include \"renderer.h\"\n";
                cppFile << "#include \"modelRenderer.h\"\n";
                cppFile << "#include \"scene.h\"\n";
                cppFile << "#include \"manager.h\"\n";
                cppFile << "\n";

                cppFile << "std::string " << className << "::m_classtexture = \"\";\n";
                cppFile << "std::string " << className << "::m_classmodel = \"asset\\\\model\\\\box.obj\";\n";
                cppFile << "std::string " << className << "::m_classVS = \"shader\\\\unlitTextureVS.cso\";\n";
                cppFile << "std::string " << className << "::m_classPS = \"shader\\\\unlitTexturePS.cso\";\n";
                cppFile << "bool "<< className<<"::m_collision = false;\n";
                cppFile << "\n";

                cppFile << "void " << className << "::Init() {\n";
                cppFile << "    SetModel(m_classmodel);\n";
                cppFile << "    SetVS(m_classVS);\n";
                cppFile << "    SetPS(m_classPS);\n";
                cppFile << "    m_Component = new ModelRenderer(this);\n";
                cppFile << "    ((ModelRenderer*)m_Component)->Load(GetModel());\n";
                cppFile << "    Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, GetVS());\n";
                cppFile << "    Renderer::CreatePixelShader(&m_PixelShader, GetPS());\n";
                cppFile << "}\n";
                cppFile << "\n";

                cppFile << "void " << className << "::DetailChange(std::string modelortexture, std::string vs, std::string ps) {\n";
                cppFile << "    SetModel(modelortexture);\n";
                cppFile << "    SetVS(vs);\n";
                cppFile << "    SetPS(ps);\n";
                cppFile << "    ((ModelRenderer*)m_Component)->Load(GetModel());\n";
                cppFile << "    Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, GetVS());\n";
                cppFile << "    Renderer::CreatePixelShader(&m_PixelShader, GetPS());\n";
                cppFile << "}\n";
                cppFile << "\n";

                cppFile << "void " << className << "::Uninit() {\n";
                cppFile << "    m_VertexLayout->Release();\n";
                cppFile << "    m_VertexShader->Release();\n";
                cppFile << "    m_PixelShader->Release();\n";
                cppFile << "}\n";
                cppFile << "\n";

                cppFile << "void " << className << "::Update() {\n";
                cppFile << "oldpos = m_Position;\n";
                cppFile << "}\n";
                cppFile << "\n";

                cppFile << "void " << className << "::Draw() {\n";
               // cppFile << "    m_Scale = XMFLOAT3(m_Scale);\n";
                cppFile << "    Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);\n";
                cppFile << "    Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);\n";
                cppFile << "    Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);\n";
                cppFile << "\n";
                cppFile << "    XMMATRIX world, scale, rot, trans;\n";
                cppFile << "    scale = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);\n";
                cppFile << "    rot = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);\n";
                cppFile << "    trans = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);\n";
                cppFile << "    world = scale * rot * trans;\n";
                cppFile << "    Renderer::SetWorldMatrix(world);\n";
                cppFile << "\n";
                cppFile << "    m_Component->Draw();\n";
                cppFile << "}\n";
                // `GetClassModel` 関数
                cppFile << "std::string " << className << "::GetClassModel()\n";
                cppFile << "{\n";
                cppFile << "    return m_classmodel;\n";
                cppFile << "}\n\n";

                // `GetClassVS` 関数
                cppFile << "std::string " << className << "::GetClassVS()\n";
                cppFile << "{\n";
                cppFile << "    return m_classVS;\n";
                cppFile << "}\n\n";

                // `GetClassPS` 関数
                cppFile << "std::string " << className << "::GetClassPS()\n";
                cppFile << "{\n";
                cppFile << "    return m_classPS;\n";
                cppFile << "}\n\n";

                // `SetTexture` 関数
                cppFile << "void " << className << "::SetTexture(std::string texture)\n";
                cppFile << "{\n";
                cppFile << "    m_classtexture = texture;\n";
                cppFile << "}\n\n";

                // `SetModel` 関数
                cppFile << "void " << className << "::SetModel(const std::string model)\n";
                cppFile << "{\n";
                cppFile << "    m_classmodel = model;\n";
                cppFile << "}\n\n";

                // `SetVS` 関数
                cppFile << "void " << className << "::SetVS(const std::string vs)\n";
                cppFile << "{\n";
                cppFile << "    m_classVS = vs;\n";
                cppFile << "}\n\n";

                // `SetPS` 関数
                cppFile << "void " << className << "::SetPS(const std::string ps)\n";
                cppFile << "{\n";
                cppFile << "    m_classPS = ps;\n";
                cppFile << "}\n\n";

                // `GetTexture` 関数
                cppFile << "const char* " << className << "::GetTexture()\n";
                cppFile << "{\n";
                cppFile << "    const char* ctexture = m_classtexture.c_str();\n";
                cppFile << "    return ctexture;\n";
                cppFile << "}\n\n";

                // `GetModel` 関数
                cppFile << "const char* " << className << "::GetModel()\n";
                cppFile << "{\n";
                cppFile << "    const char* cmodel = m_classmodel.c_str();\n";
                cppFile << "    return cmodel;\n";
                cppFile << "}\n\n";

                // `GetVS` 関数
                cppFile << "const char* " << className << "::GetVS()\n";
                cppFile << "{\n";
                cppFile << "    const char* cvs = m_classVS.c_str();\n";
                cppFile << "    return cvs;\n";
                cppFile << "}\n\n";

                // `GetPS` 関数
                cppFile << "const char* " << className << "::GetPS()\n";
                cppFile << "{\n";
                cppFile << "    const char* cps = m_classPS.c_str();\n";
                cppFile << "    return cps;\n";
                cppFile << "}\n";

                //GetCollision関数
                cppFile<<"bool "<<className<<"::GetCollision()\n";
                cppFile << "{\n";
                cppFile << "return m_collision;\n";
                cppFile<<"}\n";
                cppFile.close();
            }
            AddFileToProject("GM31_Test.vcxproj", cppFileName);
        }
}



void CreateCppAndH::ShowFileDialog() {
    // ボタンでファイル選択ダイアログを開く
    ImGui::Begin("Select File");
    if (ImGui::Button("Select File")) {

        IGFD::FileDialogConfig config;
        config.path = ".";
        fileDialogcppandh.OpenDialog("ChooseFileDlgKey", "Choose File", ".cpp", config);
    }
ImGui::End();
    // ダイアログ表示中
    if (fileDialogcppandh.Display("ChooseFileDlgKey")) {
        if (fileDialogcppandh.IsOk()) {
            std::string selectedPath = fileDialogcppandh.GetFilePathName();
            std::string fileName = fileDialogcppandh.GetCurrentFileName();
            std::string classname;
            SetFileName(fileName);
            SetFilePass(selectedPath);
            // ファイルパスを使った処理をここで行う
            ImGui::Text("Selected File: %s", selectedPath.c_str());
            ImGui::Text("Selected File Name: %s", fileName.c_str());
            std::string toRemove = ".cpp";
            size_t remove = fileName.find(toRemove);
          
            if (remove != std::string::npos) {
                // 見つかった場合、取り除く
               classname= fileName.erase(remove, toRemove.length());
            }
            std::string toRemoveh = ".h";
            size_t removeh = fileName.find(toRemoveh);
            if (removeh != std::string::npos) {
                // 見つかった場合、取り除く
                classname = fileName.erase(removeh, toRemoveh.length());
            }
            SetClassName(classname);
          /* 
            if (findclass())
            { 
                
                Recompile();
            }*/
           

        }
        fileDialogcppandh.Close();
        
    }
}
bool CreateCppAndH::findclass() {
    std::ifstream file("game.cpp"); // game.cpp を開く
    if (!file.is_open()) {
        std::cerr << "Failed to open game.cpp" << std::endl;
        return false; // ファイルが開けない場合は false を返す
    }
    std::string updif =
        "if (CreateCppAndH::GetClassNama() == \"" + GetClassNama() + "\")";
    
      
    std::string line;
    bool found = false;

    // ファイルを1行ずつ読み込んでクラス名を検索
    while (std::getline(file, line)) {
        if (line.find(updif) != std::string::npos) {
            found = true; // クラス名が見つかった場合
            break; // 見つけたのでループを終了
        }
    }

    file.close(); // ファイルを閉じる

    // 見つからなかった場合に true を返す
    return !found;
}
std::string CreateCppAndH::ShowInputDialog() {
    static char inputBuffer[256] = "";  // 入力用バッファ
    static std::string enteredText = "";  // 入力されたテキストを保存する変数

    // 入力フィールド
    ImGui::Begin("MakeFile");
    ImGui::InputText("Enter Text", inputBuffer, sizeof(inputBuffer));

    // OKボタン
    if (ImGui::Button("OK")) {
        enteredText = inputBuffer;  // ボタンが押されたらバッファの内容を取得して保存
        WriteUpdetaCode(enteredText.c_str());
        GenerateCppFile(enteredText.c_str());
        GenerateHeaderFile(enteredText.c_str());
        InsertInitCollision(enteredText.c_str());
        Recompile();
      
    }

    // 入力内容を表示（確認用）
    ImGui::Text("Entered Text: %s", enteredText.c_str());
    ImGui::End();
    return enteredText.c_str();
}
void CreateCppAndH::SetFileName(std::string filename)
{
    m_filename = filename;
}

void CreateCppAndH::SetFilePass(std::string filepass)
{
    m_filepass = filepass;
}
void CreateCppAndH::SetModel(std::string model)
{
    m_model = model;
}
void CreateCppAndH::SetPS(std::string ps)
{
    m_ps = ps;
}
void CreateCppAndH::SetVS(std::string vs)
{
    m_vs = vs;
}
 const char* CreateCppAndH::GetModel()
{
    const char* cmt = m_model.c_str(); return cmt;
}
 const char* CreateCppAndH::GetVS()
{
     const char* cvs = m_vs.c_str(); return cvs;
}
  const char* CreateCppAndH::GetPS()
{
      const char* cps = m_ps.c_str(); return cps;
}


std::string CreateCppAndH::GetFileName()
{
    return m_filename;
}

std::string CreateCppAndH::GetFilePass()
{
    return m_filepass;
}
void CreateCppAndH::SetClassName(std::string classname)
{
    m_classname = classname;
}
std::string CreateCppAndH::GetClassNama()
{
    return m_classname;
}
// enumを文字列に変換する関数
std::string CreateCppAndH::LayerToString(int layer) {
    switch (layer) {
    case 0: return "e_Camera";
    case 1: return "e_Object";
    case 2: return "e_UI";
    default: return "Unknown";
    }
}
std::string CreateCppAndH::XMFLOAT3ToString(const DirectX::XMFLOAT3& vec) {
    std::ostringstream oss;
    oss << "XMFLOAT3(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
    return oss.str();
}
void CreateCppAndH::WriteUpdetaCode(const std::string& updatetarget)
{
    
    std::string updifcolision;

    for (std::string allcolision : GetGame()->GetClassALL()) {
        // 型名を取得してテンプレート文字列に埋め込む
        updifcolision += "ClassCollision<" + allcolision + ", " + updatetarget + ">();\n";
    }
    std::string updif =


        updifcolision+"\n"+
        "if (CreateCppAndH::GetClassNama() == \"" + updatetarget + "\")\n"
        "{\n"
        "    std::string modelname = CreateCppAndH::GetModel();\n"
        "    std::string vsname = CreateCppAndH::GetVS();\n"
        "    std::string psname = CreateCppAndH::GetPS();\n"
        "    if (modelname.empty() || !std::filesystem::exists(modelname)) {\n"
        "        modelname = CreateCppAndH::escapeBackslashes("+ updatetarget +"::GetClassModel());\n"
        "    }\n"
        "    if (vsname.empty() || !std::filesystem::exists(vsname)) {\n"
        "        vsname = CreateCppAndH::escapeBackslashes("+ updatetarget + "::GetClassVS());\n"
        "    }\n"
        "    if (psname.empty() || !std::filesystem::exists(psname)) {\n"
        "       psname = CreateCppAndH::escapeBackslashes("+ updatetarget +"::GetClassPS());\n"
        "    }\n"
        "    if (CreateCppAndH::GetChange() == true) {\n"
        "        std::vector<" + updatetarget + "*> " + updatetarget + "List = GetGameObjects<" + updatetarget + ">();\n"
        "        for (" + updatetarget + "* p" + updatetarget + " : " + updatetarget + "List) {\n"
        "            p" + updatetarget + "->DetailChange(modelname, vsname, psname);\n"
        "CreateCppAndH::modifyStaticVariables(CreateCppAndH::GetFileName(), vsname, psname, modelname);\n"
        "        }\n"
        "    }\n"
        "    if (Input::GetKeyTrigger(VK_LBUTTON) && !ImGui::IsAnyItemHovered()&& Select == 2) {\n"
        "        CreateCppAndH::WriteNewCode(addpotision,CreateCppAndH::GetScale (), CreateCppAndH::GetRotation(), e_Object);\n"    
        "        " + updatetarget + "* addobj = AddGameObject<" + updatetarget + ">(e_Object);\n"
        "        addobj->SetPosition(addpotision);\n"
        "        addobj->SetScale(CreateCppAndH::GetScale());\n"
        "        addobj->SetRotation(CreateCppAndH::GetRotation());\n"
        "    }\n"
        "}";

    // game.cpp の処理
    std::ifstream input_file("game.cpp");
    if (!input_file.is_open()) {
        std::cerr << "Error: game.cpp を開けませんでした。" << std::endl;
        return;
    }

    std::ostringstream buffer;
    buffer << input_file.rdbuf();
    std::string file_content = buffer.str();
    input_file.close();

    // 挿入位置を検索
    std::string insert_updateword = "// Updateif";
    size_t updpos = file_content.find(insert_updateword);
  
    
    if (updpos != std::string::npos) {
        file_content.insert(updpos + insert_updateword.length(), "\n" + updif);
    }
    else {
        std::cerr << "Error: game.cpp に指定のキーワードが見つかりません。" << std::endl;
        return;
    }

    // game.cpp に書き戻し
    std::ofstream output_file("game.cpp");
    if (!output_file.is_open()) {
        std::cerr << "Error: game.cpp を書き込み用に開けませんでした。" << std::endl;
        return;
    }
    output_file << file_content;
    output_file.close();

    // scene.h の処理
    std::ifstream header_file("scene.h");
    if (!header_file.is_open()) {
        std::cerr << "Error: scene.h を開けませんでした。" << std::endl;
        return;
    }

    buffer.str("");
    buffer.clear();
    buffer << header_file.rdbuf();
    std::string file_content_h = buffer.str();
    header_file.close();

    // 必要なら #include 文を追加
    std::string include_statement = "#include \"" + updatetarget + ".h\"\n";
    if (file_content_h.find(include_statement) == std::string::npos) {
        file_content_h = include_statement + file_content_h;
    }

    // scene.h に書き戻し
    std::ofstream header_output_file("scene.h");
    if (!header_output_file.is_open()) {
        std::cerr << "Error: scene.h を書き込み用に開けませんでした。" << std::endl;
        return;
    }
    header_output_file << file_content_h;
    header_output_file.close();

}
void CreateCppAndH::WriteNewCode(XMFLOAT3 targetpos, XMFLOAT3 targetsize, XMFLOAT3 targetrotation, int layer)
{
    if (GetClassNama().empty()) {//そもそもそのファイルが存在するか確認
        return;
    }

    // 追記するコードスニペットを準備
 
    // game.cpp の処理
    std::ifstream input_file("game.cpp");
    if (!input_file.is_open()) {
        std::cerr << "Error: game.cpp を開けませんでした。" << std::endl;
        return;
    }

    std::ostringstream buffer;
    buffer << input_file.rdbuf();
    std::string file_content = buffer.str();
    input_file.close();

    // 静的カウンタではなく既存変数をチェックしてユニーク名を生成
    int counter = 0;
    std::string unique_name;
    do {
        unique_name = GetClassNama() + "p" + std::to_string(counter);
        counter++;
    } while (file_content.find(unique_name) != std::string::npos);

   std::string setpos = unique_name+"->SetPosition(" + XMFLOAT3ToString(targetpos) + "); \n";
    std::string setsize = unique_name + "->SetScale(" + XMFLOAT3ToString(targetsize) + ");\n";
    std::string setrotation = unique_name + "->SetRotation(" + XMFLOAT3ToString(targetrotation) + ");\n";
    std::string addclass = "\n" + GetClassNama() + "* " + unique_name + "= AddGameObject<" + GetClassNama() + ">(" + LayerToString(layer) + "); // CLASSADD\n";

   

    // 挿入位置を検索
    std::string insert_keyword = "// INSERT HERE";
  
   
    size_t pos = file_content.find(insert_keyword);
    if (pos != std::string::npos) {
        file_content.insert(pos + insert_keyword.length(), addclass+setpos + setsize+setrotation);
    }
    else {
        std::cerr << "Error: game.cpp に指定のキーワードが見つかりません。" << std::endl;
        return;
    }
    

    // game.cpp に書き戻し
    std::ofstream output_file("game.cpp");
    if (!output_file.is_open()) {
        std::cerr << "Error: game.cpp を書き込み用に開けませんでした。" << std::endl;
        return;
    }
    output_file << file_content;
    output_file.close();

    // scene.h の処理
    std::ifstream header_file("scene.h");
    if (!header_file.is_open()) {
        std::cerr << "Error: scene.h を開けませんでした。" << std::endl;
        return;
    }

    buffer.str("");
    buffer.clear();
    buffer << header_file.rdbuf();
    std::string file_content_h = buffer.str();
    header_file.close();

    // 必要なら #include 文を追加
    std::string include_statement = "#include \"" + GetClassNama() + ".h\"\n";
    if (file_content_h.find(include_statement) == std::string::npos) {
        file_content_h = include_statement + file_content_h;
    }

    // scene.h に書き戻し
    std::ofstream header_output_file("scene.h");
    if (!header_output_file.is_open()) {
        std::cerr << "Error: scene.h を書き込み用に開けませんでした。" << std::endl;
        return;
    }
    header_output_file << file_content_h;
    header_output_file.close();
    
}
void CreateCppAndH::AddFileToProject(const std::string& projectFile, const std::string& fileToAdd) {
    // プロジェクトファイルを開く
    std::ifstream project(projectFile);
    if (!project.is_open()) {
        std::cerr << "プロジェクトファイルが開けませんでした: " << projectFile << std::endl;
        return;
    }

    std::string line;
    bool isFileAdded = false;

    // プロジェクトファイルを読み込み、追加する場所を探す
    std::ofstream tempFile("temp.vcxproj");
    while (std::getline(project, line)) {
        // <ItemGroup> タグの間に追加する
        if (line.find("<ItemGroup>") != std::string::npos && !isFileAdded) {
            tempFile << line << std::endl;
            tempFile << "    <ClCompile Include=\"" << fileToAdd << "\" />" << std::endl;
            isFileAdded = true;
        }
        else {
            tempFile << line << std::endl;
        }
    }

    project.close();
    tempFile.close();

    // 一時ファイルを元のプロジェクトファイルに置き換える
    std::remove(projectFile.c_str());
    std::rename("temp.vcxproj", projectFile.c_str());

    std::cout << "ファイルをプロジェクトに追加しました: " << fileToAdd << std::endl;
}
namespace fs = std::filesystem;

void CreateCppAndH::Recompile() {
  
    // 実行したいプロジェクトのコマンド
    const char* command = "recompile.exe"; // 実行ファイルの名前やパスを指定

    // system関数でプロセスを起動
    int result = std::system(command);

    // 起動結果の確認
    if (result == 0) {
        std::cout << "Project A started successfully!" << std::endl;
    }
    else {
        std::cerr << "Failed to start Project A." << std::endl;
    }
}


void CreateCppAndH::DetailClass()//クラスの詳細設定
{


    ImGui::Begin("Detali");

   

    IGFD::FileDialogConfig modelfile;
    IGFD::FileDialogConfig vsfile;
    IGFD::FileDialogConfig psfile;
    modelfile.path = ".\\asset\\model";
    vsfile.path = ".\\shader";
    psfile.path = ".\\shader";
    //シェーダー等の設定ダイアログ
    fileDialogmodel.OpenDialog("ChooseFileModelDlgKey", "ChooseModelFile", ".obj", modelfile);
    fileDialogvs.OpenDialog("ChooseFileVSDlgKey", "ChooseVSFile", ".hlsl", vsfile);
    fileDialogps.OpenDialog("ChooseFilePSDlgKey", "ChoosePSFile", ".hlsl", psfile);
    //ここまで
    if (fileDialogmodel.Display("ChooseFileModelDlgKey")) {
        if (fileDialogmodel.IsOk()) {
            std::string selectedModelPath = fileDialogmodel.GetFilePathName();
            std::string fileModelName = fileDialogmodel.GetCurrentFileName();

            SetModel("asset\\\\model\\\\" + fileModelName);
            m_change = true;


        }

    }

    ImGui::Text("SelectedModelFile: %s", GetModel());

    if (fileDialogvs.Display("ChooseFileVSDlgKey")) {
        if (fileDialogvs.IsOk()) {
            std::string selectedVSPath = fileDialogvs.GetFilePathName();
            std::string fileVSName = fileDialogvs.GetCurrentFileName();
            std::string extension = ".hlsl";

            // .hlslが含まれている場合のみ削除
            size_t pos = fileVSName.rfind(extension);
            if (pos != std::string::npos) {
                fileVSName.erase(pos, extension.length());
            }

            SetVS("shader\\\\" + fileVSName + ".cso");
            m_change = true;


        }

    } ImGui::Text("SelectedVSFile: %s", GetVS());
    if (fileDialogps.Display("ChooseFilePSDlgKey")) {
        if (fileDialogps.IsOk()) {
            std::string selectedPSPath = fileDialogps.GetFilePathName();
            std::string filePSName = fileDialogps.GetCurrentFileName();

            std::string extension = ".hlsl";

            // .hlslが含まれている場合のみ削除
            size_t pos = filePSName.rfind(extension);
            if (pos != std::string::npos) {
                filePSName.erase(pos, extension.length());
            }
            SetPS("shader\\\\" + filePSName + ".cso");
            m_change = true;


        }

    }ImGui::Text("SelectedPSFile: %s", GetPS());

  
   
    ImGui::End();

    
}
void CreateCppAndH::GuiSetScale()
{
if (ImGui::InputFloat3("Scale", &m_scale.x)) {
            // 値が変更された場合の処理
            SetScale(m_scale);
        }
}
void CreateCppAndH::GuiSetPosition()
{
    if (ImGui::InputFloat3("Position", &m_position.x)) {
        // 値が変更された場合の処理
        SetPosition(m_position);
    }
}
void CreateCppAndH::GuiSetRotation()
{
    if (ImGui::InputFloat3("Rotation", &m_rotation.x)) {
        // 値が変更された場合の処理
        SetRotation(m_rotation);
    }
}

void CreateCppAndH::ColisionListBox(const std::vector<std::string>& classlist) {
    ImGui::Begin("Colision");

    // pairedData のサイズが classlist のサイズと異なる場合に合わせる
    
        pairedData.resize(classlist.size()); // pairedData のサイズを classlist に合わせて調整
        oldcollision.resize(pairedData.size());
        // 新しく追加した要素にデフォルト値を設定
        for (size_t i = 0; i < classlist.size(); ++i) {
        std::vector<std::pair<std::string, bool>> pair = pairedData;
            if (i > pairedData.size()&&classlist[i]== Game::GetInitPaired()[i].first) {
                pairedData.push_back({ classlist[i], Game::GetInitPaired()[i].second});// サイズに合わせて挿入

            }
            else {
               
                pairedData[i].first = classlist[i]; // クラス名を設定
                if (!Game::GetInitPaired().empty())
                {
                    pairedData[i].second = pair[i].second;//チェックの値を設定
                }
                
            }
        }
    
    
    // 各クラス名に対してチェックボックスを生成
    for (size_t i = 0; i < classlist.size(); i++) {
        bool isChecked = pairedData[i].second;
        if (ImGui::Checkbox(classlist[i].c_str(), &isChecked)) {
            pairedData[i].second = isChecked; // 状態を更新
            if (oldcollision[i] != isChecked)//前回のフレームと今のチェックボックスに変化があれば真
            {
                oldcollision[i] = isChecked;
                CollisionWrite(pairedData[i].first, isChecked);//毎フレーム呼ばれると重くなるのでチェックボックス更新時のみ呼ぶ
            }
        }
    }

    ImGui::End();
}
void CreateCppAndH::SetPair(std::vector<std::pair<std::string, bool>> pairs)
{
    pairedData = pairs;
}

std::vector<std::pair<std::string, bool>> CreateCppAndH::GetPair()
{
    return pairedData;
}
bool CreateCppAndH::GetChange() {
     
                  
    return  m_change;         // 保持していた値を返す
}

void CreateCppAndH::SetScale(XMFLOAT3 scale)
{
   m_scale = scale;
}
void CreateCppAndH::SetPosition(XMFLOAT3 position)
{
    m_position = position;
}
void CreateCppAndH::SetRotation(XMFLOAT3 rotation)
{
    m_rotation = rotation;
}

XMFLOAT3 CreateCppAndH::GetPosition()
{
    return m_position;
}
XMFLOAT3 CreateCppAndH::GetScale()
{
    return m_scale;
}
XMFLOAT3 CreateCppAndH::GetRotation()
{
    return m_rotation;
}
void CreateCppAndH::SetChange(bool change)
{
    m_change = change;
}

void CreateCppAndH::modifyStaticVariables(const std::string& filePath,std::string cVS,std::string cPS,std::string cModel) {
    
  
    // ファイル全体を読み込む
    std::ifstream inputFile(filePath);
    if (!inputFile.is_open()) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return;
    }

    std::stringstream buffer;
    buffer << inputFile.rdbuf(); // ファイルの全内容をバッファに読み込み
    std::string fileContent = buffer.str();
    inputFile.close();

    // 正規表現で変更対象を検出・置換
    std::string className = GetClassNama();
    std::regex vsRegex(R"(std::string )" + className + R"(::m_classVS = ".*";)");
    std::regex psRegex(R"(std::string )" + className + R"(::m_classPS = ".*";)");
    std::regex modelRegex(R"(std::string )" + className + R"(::m_classmodel = ".*";)");
    std::string newPS = "std::string " + className + "::m_classPS = \"" + cPS + "\";";
    std::string newVS = "std::string " + className + "::m_classVS = \"" + cVS + "\";";
    std::string newModel = "std::string "+className+"::m_classmodel = \""+cModel+"\";";
   
 
    // 各部分を順に置き換える
    fileContent = std::regex_replace(fileContent, psRegex, newPS);
    fileContent = std::regex_replace(fileContent, vsRegex, newVS);
    fileContent = std::regex_replace(fileContent, modelRegex, newModel);
  

   
    // ファイルに書き戻す
    std::ofstream outputFile(filePath, std::ios::trunc); // 上書きモード
    if (!outputFile.is_open()) {
        std::cerr << "Failed to open file for writing: " << filePath << std::endl;
        return;
    }

    outputFile << fileContent; // 新しい内容を書き込み
    outputFile.close();

    std::cout << "File modified successfully: " << filePath << std::endl;

}
std::string CreateCppAndH::escapeBackslashes(const std::string& input) {
    std::string result;
    result.reserve(input.size() * 2); // 十分なメモリを確保

    for (size_t i = 0; i < input.size(); ++i) {
        if (input[i] == '\\') {
            // 次の文字が既に '\' でエスケープされている場合はそのまま追加
            if (i + 1 < input.size() && input[i + 1] == '\\') {
                result += "\\\\"; // そのままコピー
                ++i; // 次の '\' をスキップ
            }
            else {
                result += "\\\\"; // エスケープ
            }
        }
        else {
            result += input[i]; // 他の文字はそのまま追加
        }
    }

    return result;
}


void CreateCppAndH::RemoveNthLineAfterString(const std::string& target, size_t targetNumber) {
    // ファイルを開く
    std::ifstream inputFile("game.cpp");
    if (!inputFile) {
        std::cerr << "ファイルが開けませんでした: " << "game.cpp" << std::endl;
        return;
    }

    std::vector<std::string> lines;
    std::string line;

    // ファイルの内容を行ごとに読み込む
    while (std::getline(inputFile, line)) {
        lines.push_back(line);
    }

    inputFile.close();

    // target文字列の出現回数
    size_t targetCount = 0;
    size_t targetIndex = lines.size();  // 初期値は範囲外

    // target文字列がtargetNumber番目に出現した位置を探す
    for (size_t i = 0; i < lines.size(); ++i) {
        if (lines[i].find(target) != std::string::npos) {
           
            if (targetCount == targetNumber) {
                targetIndex = i;
                break;
            }
            targetCount++;
        }
    }

    // targetNumber番目が見つからなかった場合
    if (targetIndex == lines.size()) {
        std::cerr << "指定したtarget文字列がファイル内に見つかりませんでした。" << std::endl;
        return;
    }

    // target文字列が見つかった場合、その後の3行を削除
    lines.erase(lines.begin() + targetIndex, lines.begin() + std::min(targetIndex + 4, lines.size()));

    // 修正された内容を新しいファイルに書き込む
    std::ofstream outputFile("game.cpp");
    if (!outputFile) {
        std::cerr << "ファイルの書き込みに失敗しました: " << "game.cpp" << std::endl;
        return;
    }

    for (const auto& line : lines) {
        outputFile << line << std::endl;
    }

    outputFile.close();
    std::cout << "ファイルの更新が完了しました。" << std::endl;
}
void CreateCppAndH::ModeSelect() {
    
    // ボタンでファイル選択ダイアログを開く
    ImGui::Begin("SelectMode");
    if (ImGui::RadioButton("Select", selectedMode == 0)) {
        selectedMode = 0;
    }
    if (ImGui::RadioButton("Delete", selectedMode == 1)) {
        selectedMode = 1;
    }
    if (ImGui::RadioButton("Create", selectedMode == 2)) {
        selectedMode = 2;
    }
    
    ImGui::End();
   

}
int CreateCppAndH::GetSelectedMode()
{
    return selectedMode;
}







void CreateCppAndH::ChangePositionSizeRotation(const std::string& target, size_t targetNumber, XMFLOAT3 pos, XMFLOAT3 size, XMFLOAT3 rot) {

    // ファイルを開く
    std::ifstream inputFile("game.cpp");
    if (!inputFile) {
        std::cerr << "ファイルが開けませんでした: " << "game.cpp" << std::endl;
        return;
    }

    std::vector<std::string> lines;
    std::string line;

    // ファイルの内容を行ごとに読み込む
    while (std::getline(inputFile, line)) {
        lines.push_back(line);
    }
    inputFile.close();

    // target文字列の出現回数
    size_t targetCount = 0;
    size_t targetIndex = lines.size();  // 初期値は範囲外

    // target文字列がtargetNumber番目に出現した位置を探す
    for (size_t i = 0; i < lines.size(); ++i) {
        if (lines[i].find(target) != std::string::npos) {
            if (targetCount == targetNumber) {
                targetIndex = i;
                break;
            }
            targetCount++;
        }
    }

    // targetNumber番目が見つからなかった場合
    if (targetIndex == lines.size()) {
        std::cerr << "指定したtarget文字列がファイル内に見つかりませんでした。" << std::endl;
        return;
    }

    // XMFLOAT3 を文字列に変換する関数


    // target行の次の3行を対象とする
    for (size_t i = targetIndex + 1; i <= targetIndex + 3 && i < lines.size(); ++i) {
        if (lines[i].find("->SetPosition") != std::string::npos) {
            lines[i] = std::regex_replace(lines[i], std::regex(R"(->SetPosition\(.*\);)"), "->SetPosition(XMFLOAT3(" + std::to_string(pos.x) + "f, " + std::to_string(pos.y) + "f, " + std::to_string(pos.z) + "f)); ");
        }
        else if (lines[i].find("->SetScale") != std::string::npos) {
            lines[i] = std::regex_replace(lines[i], std::regex(R"(->SetScale\(.*\);)"), "->SetScale(XMFLOAT3(" + std::to_string(size.x) + "f, " + std::to_string(size.y) + "f, " + std::to_string(size.z) + "f));");
        }
        else if (lines[i].find("->SetRotation") != std::string::npos) {
            lines[i] = std::regex_replace(lines[i], std::regex(R"(->SetRotation\(.*\);)"), "->SetRotation(XMFLOAT3(" + std::to_string(rot.x) + "f, " + std::to_string(rot.y) + "f, " + std::to_string(rot.z) + "f));");
        }
    }

    // 修正された内容を新しいファイルに書き込む
    std::ofstream outputFile("game.cpp");
    if (!outputFile) {
        std::cerr << "ファイルの書き込みに失敗しました: " << "game.cpp" << std::endl;
        return;
    }

    for (const auto& line : lines) {
        outputFile << line << std::endl;
    }

    outputFile.close();
    std::cout << "ファイルの更新が完了しました。" << std::endl;
}
    

void CreateCppAndH::CollisionWrite(const std::string& filePath,bool cCollision) {

    std::string collisionStr;
    if (cCollision)
    {
        collisionStr = "true";
    }
    else
    {
        collisionStr = "false";
    }
    // ファイル全体を読み込む
    std::ifstream inputFile(filePath+".cpp");
    if (!inputFile.is_open()) {
        std::cerr << "Failed to open file: " << filePath+".cpp" << std::endl;
        return;
    }

    std::stringstream buffer;
    buffer << inputFile.rdbuf(); // ファイルの全内容をバッファに読み込み
    std::string fileContent = buffer.str();
    inputFile.close();

    // 正規表現で変更対象を検出・置換
    std::string className = filePath;
  
    std::regex collisionRegex(R"(bool )" + className + R"(::m_collision = \S*;)");
   
    std::string newCollision = "bool " + className + "::m_collision = " + collisionStr + ";";

    std::smatch match;
    if (std::regex_search(fileContent, match, collisionRegex)) {
        std::cout << "Matched: " << match.str() << std::endl;
    }
    else {
        std::cerr << "No match found!" << std::endl;
        return;
    }
    // 部分を順に置き換える
  
    fileContent = std::regex_replace(fileContent, collisionRegex, newCollision);


    // ファイルに書き戻す
    std::ofstream outputFile(filePath+".cpp", std::ios::trunc); // 上書きモード
    if (!outputFile.is_open()) {
        std::cerr << "Failed to open file for writing: " << filePath+".cpp" << std::endl;
        return;
    }

    outputFile << fileContent; // 新しい内容を書き込み
    outputFile.close();

    std::cout << "File modified successfully: " << filePath + ".cpp" << std::endl;

}
void CreateCppAndH::InsertInitCollision(const std::string& inittarget) {
    std::ifstream file_in("game.cpp"); // 読み込み用ファイルストリーム
    if (!file_in) {
        std::cerr << "ファイルを開けません: game.cpp" << std::endl;
        return;
    }

    std::vector<std::string> lines;
    std::string line;
    bool found = false;

    while (std::getline(file_in, line)) {
        lines.push_back(line);
        if (line.find("//InitCollisions") != std::string::npos) {
            lines.push_back("    InitCollision<"+inittarget + ">();"); // 次の行に追加
            found = true;
        }
    }
    file_in.close();

    if (!found) {
        std::cerr << "//InitCollisions が見つかりませんでした。" << std::endl;
        return;
    }

    // 上書きモードでファイルを開き、修正後の内容を書き戻す
    std::ofstream file_out("game.cpp");
    if (!file_out) {
        std::cerr << "ファイルを開き直せませんでした: game.cpp" << std::endl;
        return;
    }

    for (const auto& l : lines) {
        file_out << l << std::endl;
    }

    file_out.close();
}