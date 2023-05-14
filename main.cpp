#include<Windows.h>
#include"Window.h"
#include "DirectX.h"
#include "Polygon.h"



//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	Window window;
	DirectX direct;
	Polygon polygon;

	//アプリケーションの開始
	window.StartApp();

	//数値の取得
	direct.hwnd_ = window.GetHwnd();
	
	//DirectXの初期化
	direct.Start();

	//ポリゴンの初期化
	

	/*=====================================*/
	/* 　　　　   メインループ　　    　        */
	/*=====================================*/

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
			direct.Update();
		}
	}
	


	//アプリケーションの終了
	window.EndApp();

	direct.EndD3D();

	return 0;

}