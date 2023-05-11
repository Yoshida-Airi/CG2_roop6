#include "Window.h"
#include <windows.h>
#include <tchar.h>

/*=====================================*/
/* 　　　　   パブリックメソッド　　　 　     */
/*=====================================*/

//コンストラクタ
Window::Window(int32_t width, int32_t height)
{
	hInst = nullptr;
	hwnd = nullptr;
	Width = width;
	Height=height;
}

//デストラクタ
Window::~Window()
{

}

//ループ起動
void Window::Run()
{
	if (Initialize())
	{
		MainRoop();
	}
	//アプリケーションの終了
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

	//ウィンドウサイズの決定
	//クライアント領域のサイズ
	Width = 1280;
	Height = 720;

	//ウィンドウサイズを表す構造体にクライアント領域を入れる
	RECT wrc = { 0,0,Width,Height };

	//クライアント領域をもとに実際のサイズにwrcを変更してもらう
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	//ウィンドウの生成
	hwnd = CreateWindow
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

	if (hwnd == nullptr)
	{
		return false;
	}

	//ウィンドウを表示する
	ShowWindow(hwnd, SW_SHOW);

	return true;

}

//ウィンドウの更新
void Window::MainRoop()
{
	MSG msg{};
	while (msg.message != WM_QUIT)
	{
		//Windowsにメッセージが来てたら最優先で処理させる
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);


		}
		else
		{
			//ゲームの処理
		}
	}
}

void Window::EndRoop()
{
	//ウィンドウの終了処理
	EndWindow();
}

//ウィンドウの終了
void Window::EndWindow()
{
	//ウィンドウの登録解除
	hwnd=nullptr;
}
