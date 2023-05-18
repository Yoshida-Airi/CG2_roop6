#include "WindowAPI.h"
#include <windows.h>
#include <tchar.h>
#include<d3d12.h>


/*=====================================*/
/* 　　　　   パブリックメソッド　　　 　     */
/*=====================================*/

//コンストラクタ
WindowAPI::WindowAPI()
{
	Title_ = L"CG2";

	hInst_ = nullptr;
	hwnd_ = nullptr;
	
	Height_ = 720;
	Width_ = 1280;

	wrc_ = {};
	wc_ = {};

}

//デストラクタ
WindowAPI::~WindowAPI()
{
	EndRoop();
}


//開始
void WindowAPI::StartApp()
{
	Initialize();
}

//終了
void WindowAPI::EndApp()
{
	EndRoop();
}



/*=====================================*/
/* 　　　　   プライベートメソッド　　　      */
/*=====================================*/

//ウィンドウプロシージャ
LRESULT CALLBACK WindowAPI::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	//メッセージに応じてゲーム固有の処理を行う
	switch (msg)
	{
		//ウィンドウが破壊された
	case WM_DESTROY:
		//OSに対して、アプリの終了を伝える
		PostQuitMessage(0);
		return 0;
	}
	//標準のメッセージ処理を行う
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

//ウィンドウの初期化
bool WindowAPI::Initialize()
{
	if (!InitializeWindow())
	{
		return false;
	}
	return true;
}



//ウィンドウクラスの登録(初期化)
bool WindowAPI::InitializeWindow()
{
	//ウィンドウクラスの登録(設定をWindowsに伝える)
	
	//ウィンドウプロシージャ
	wc_.lpfnWndProc = WindowProc;
	//ウィンドウクラス名(なんでも良い)
	wc_.lpszClassName = L"CG2WindowClass";
	//インスタンスハンドル
	wc_.hInstance = GetModuleHandle(nullptr);
	//カーソル
	wc_.hCursor = LoadCursor(nullptr, IDC_ARROW);

	//ウィンドウの登録
	RegisterClass(&wc_);

	//ウィンドウサイズを表す構造体にクライアント領域を入れる
	wrc_ = { 0,0,Width_,Height_ };

	//クライアント領域をもとに実際のサイズにwrcを変更してもらう
	AdjustWindowRect(&wrc_, WS_OVERLAPPEDWINDOW, false);

	//ウィンドウの生成
	hwnd_ = CreateWindow
	(
		wc_.lpszClassName,		//利用するクラス名
		Title_,					//タイトルバーの文字(何でも良い)
		WS_OVERLAPPEDWINDOW,	//よく見るウィンドウスタイル
		CW_USEDEFAULT,			//表示X座標(Windowsに任せる)
		CW_USEDEFAULT,			//表示Y座標(WindowsOSに任せる)
		wrc_.right - wrc_.left,	//ウィンドウ横幅
		wrc_.bottom - wrc_.top,	//ウィンドウ縦幅
		nullptr,				//親ウィンドウハンドル
		nullptr,				//メニューハンドル
		wc_.hInstance,			//インスタンスハンドル
		nullptr					//オプション
	);

	

	if (hwnd_ == nullptr)
	{
		return false;
	}

	//ウィンドウを表示する
	ShowWindow(hwnd_, SW_SHOW);

	return true;

}



void WindowAPI::EndRoop()
{
	//ウィンドウの終了処理
	EndWindow();
}

//ウィンドウの終了
void WindowAPI::EndWindow()
{

}

