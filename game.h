///////////////////////////////////
//2025/02/17 
//nanashima game.h　ゲームエンジン部分
//////////////////////////////////
#pragma once
#include"scene.h"
#include <memory>

#define FovAngleY (1)//視野角
#define NearZ (1)//どこから描画するか
#define FarZ (1000)//どこまで描画するか
class Game:public Scene
{
	static bool initcollision;//当たり判定の初期化　一度だけ呼ぶためのフラグ
	Cursor* pcursor;
	GameObject* oldobject=nullptr;
	static Game* m_game;//インスタンス
	static std::vector<std::pair<std::string, bool>> InitPairedData;//当たり判定管理用の初期値
public:
	
	void Init()override;

	
	void Update()override;

	void SetGame();//Gameのインスタンスを登録
	
	static std::vector<std::pair<std::string, bool>> GetInitPaired();//InitPairedDataのゲッター
	template<typename T>
	void InitCollision();
		
	
	
	template<typename T, typename T2>
	void ClassCollision();//TとT2の当たり判定
	
};
	 Game* GetGame();//外部からGameのインスタンスを取得する関数
