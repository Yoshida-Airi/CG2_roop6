#include "Window.h"
#include <windows.h>
#include <tchar.h>
#include<d3d12.h>


/*=====================================*/
/* 　　　　   パブリックメソッド　　　 　     */
/*=====================================*/

//コンストラクタ
Window::Window()
{
	hInst_ = nullptr;
	hwnd_ = nullptr;
	
	Height_ = 0;
	Width_ = 0;
#ifdef _DEBUG
	debugController_ = nullptr;
#endif
}

//デストラクタ
Window::~Window()
{
	EndRoop();
}


//開始
void Window::StartApp()
{
	Initialize();
}
//終了
void Window::EndApp()
{
	EndRoop();
}



/*=====================================*/
/* 　　　　   プライベートメソッド　　　      */
/*=====================================*/

//ウィンドウプロシージャ
LRESULT CALLBACK Window::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
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
bool Window::Initialize()
{
	if (!InitializeWindow())
	{
		return false;
	}
	return true;
}


//ウィンドウクラスの登録(初期化)
bool Window::InitializeWindow()
{
	//ウィンドウクラスの登録(設定をWindowsに伝える)
	WNDCLASS wc{};
	//ウィンドウプロシージャ
	wc.lpfnWndProc = WindowProc;
	//ウィンドウクラス名(なんでも良い)
	wc.lpszClassName = L"CG2WindowClass";
	//インスタンスハンドル
	wc.hInstance = GetModuleHandle(nullptr);
	//カーソル
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);

	//ウィンドウの登録
	RegisterClass(&wc);

	Width_ = 1280;
	Height_ = 720;

	//ウィンドウサイズを表す構造体にクライアント領域を入れる
	RECT wrc = { 0,0,Width_,Height_ };

	//クライアント領域をもとに実際のサイズにwrcを変更してもらう
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	//ウィンドウの生成
	hwnd_ = CreateWindow
	(
		wc.lpszClassName,		//利用するクラス名
		L"CG2",					//タイトルバーの文字(何でも良い)
		WS_OVERLAPPEDWINDOW,	//よく見るウィンドウスタイル
		CW_USEDEFAULT,			//表示X座標(Windowsに任せる)
		CW_USEDEFAULT,			//表示Y座標(WindowsOSに任せる)
		wrc.right - wrc.left,	//ウィンドウ横幅
		wrc.bottom - wrc.top,	//ウィンドウ縦幅
		nullptr,				//親ウィンドウハンドル
		nullptr,				//メニューハンドル
		wc.hInstance,			//インスタンスハンドル
		nullptr					//オプション
	);

	//デバッグ
#ifdef _DEBUG
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController_))))
	{
		//デバッグレイヤーを有効化する
		debugController_->EnableDebugLayer();
		//さらにGPU側でもチェックを行うようにする
		debugController_->SetEnableGPUBasedValidation(TRUE);
	}
#endif


	if (hwnd_ == nullptr)
	{
		return false;
	}

	//ウィンドウを表示する
	ShowWindow(hwnd_, SW_SHOW);

	return true;

}



void Window::EndRoop()
{
	//ウィンドウの終了処理
	EndWindow();
}

//ウィンドウの終了
void Window::EndWindow()
{
#ifdef _DEBUG
	if (debugController_ != nullptr)
	{
		debugController_->Release();
	}
#endif
}
