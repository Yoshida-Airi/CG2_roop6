#pragma once
#include <windows.h>
#include<cstdint>

class Window
{
public:
	//コンストラクタ
	Window(int32_t width, int32_t height);
	//デストラクタ
	~Window();
	//ループ起動
	void Run();

private:

	/*=====================================*/
	/* 　　　　   プライベート変数    　        */	
	/*=====================================*/

	HINSTANCE hInst;	//インスタンスハンドル
	HWND hwnd;			//ウィンドウハンドル
	int32_t Width;		//ウィンドウの横幅
	int32_t Height;	//ウィンドウの縦幅


	/*=====================================*/
	/* 　　　　   プライベートメソッド　　　      */
	/*=====================================*/

	//初期化
	bool Initialize();
	//アプリケーションの終了
	void EndRoop();
	//ウィンドウクラスを登録
	bool InitializeWindow();
	//ウィンドウの終了
	void EndWindow();
	//ウィンドウの更新
	void MainRoop();
	
	
	//ウィンドウプロシージャ
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);


};