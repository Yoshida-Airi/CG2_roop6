#pragma once
#include <windows.h>
#include<cstdint>
#include<d3d12.h>
#include"ProtectedVar.h"

class Window:public protectedVar
{
public:
	/*=====================================*/
	/* 　　　　   パブリックメソッド　　　 　     */
	/*=====================================*/
	//コンストラクタ
	Window();
	//デストラクタ
	~Window();
	//アプリケーションの開始
	void StartApp();
	//アプリケーションの終了
	void EndApp();


private:

	/*=====================================*/
	/* 　　　　   プライベート変数    　        */	
	/*=====================================*/

	HINSTANCE hInst_;	//インスタンスハンドル
	
	
	//デバッグコントローラー
#ifdef _DEBUG
	ID3D12Debug1* debugController_;
#endif


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
	
	
	
	//ウィンドウプロシージャ
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);


};