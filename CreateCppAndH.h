///////////////////////////////////
//2025/02/17 
//nanashima CreateCppAndH.h
//////////////////////////////////
#pragma once
#include <string>  // 追加：std::stringを使用するためのインクルード
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
	static std::vector<bool> oldcollision;//1フレーム前のコリジョン設定
	static int selectedMode ; // 選択中のオプションのインデックス
	static std::vector<std::pair<std::string, bool>> pairedData;//当たり判定用の配列
public:
	static void GenerateCppFile(const std::string& className);//新たなcpp生成
	
	
	static void Init();//初期化
	static void Render();//描画
	static void GenerateHeaderFile(const std::string& className);//新たな.h生成
	static void ShowFileDialog();//シェーダー等の変更用ダイアログ
	static bool findclass();
	static std::string ShowInputDialog();//新たなファイル名の取得
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
	static void SetClassName(std::string classname);//クラス名設定
	static std::string GetClassNama();//現在選択中のクラス名前取得　GetClass"Nama"はタイプミスではない

	static std::string LayerToString(int layer);//数字をレイヤーの文字に変える

	static std::string XMFLOAT3ToString(const DirectX::XMFLOAT3& vec);

	static void WriteUpdetaCode(const std::string& updetatarget);//gameのupdeta部分に対する書き込み

	static void WriteNewCode(const DirectX::XMFLOAT3 targetpos, const DirectX::XMFLOAT3 targetsize, const DirectX::XMFLOAT3 targetrotation, int layer);//gameのInit部分に対する書き込み

	static void AddFileToProject(const std::string& projectFile, const std::string& fileToAdd);//ソリューションファイルに追加する

	static void Recompile();//再起動コンパイル処理
	
	
	
	static void DetailClass();//クラスの詳細を表示する
	static void SetPair(std::vector<std::pair<std::string, bool>> pairs);//当たり判定管理用の変数のセット
	
	static  std::vector<std::pair<std::string, bool>>GetPair();
	static bool GetChange();
	static void SetScale(XMFLOAT3 scale);
	static void SetRotation(XMFLOAT3 rotation);
	static void SetPosition(XMFLOAT3 position);
	static XMFLOAT3 GetScale();
	static XMFLOAT3 GetPosition();
	static XMFLOAT3 GetRotation();
	static void modifyStaticVariables(const std::string& FilePath, std::string VS, std::string PS, std::string Model);//シェーダー等の書き換え
	static void SetChange(bool change);//設定が変更されたら設定
	static std::string escapeBackslashes(const std::string& input);//追加したパスのescape処理
	
	static void ColisionListBox(const std::vector<std::string> &classlist);//当たり判定の設定用チェックボックス
	static void GuiSetScale();//大きさ変更ダイアログ
	static void GuiSetPosition();//位置変更ダイアログ
	static void GuiSetRotation();//回転変更ダイアログ
	static void RemoveNthLineAfterString(const std::string& target, size_t targetNumber);//オブジェクトの削除
	static void ModeSelect();//選択、削除、作成の選択
	static int GetSelectedMode();//今選ばれているモード
	static void ChangePositionSizeRotation(const std::string& target, size_t targetNumber, XMFLOAT3 pos, XMFLOAT3 size, XMFLOAT3 rot);//移動、拡大縮小、回転の設定
	static void CollisionWrite(const std::string& FilePath, bool collision);//各クラスのファイルにコリジョンの真偽を設定する
	static void InsertInitCollision(const std::string& inittarget);//コリジョンの初期化をgameに書き込む
	//当たり判定の書き込み
};

