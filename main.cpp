#include<Windows.h>
#include"WindowAPI.h"
#include "DirectX.h"

//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	WindowAPI windowAPI;
	DirectX direct;

	
	//アプリケーションの開始
	windowAPI.StartApp();

	//DirectXの初期化
	direct.Start(windowAPI.GetHwnd());

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
		/*	direct.Update();*/
		}
	}
	


	//アプリケーションの終了
	windowAPI.EndApp();

	//direct.EndD3D();

	return 0;

}