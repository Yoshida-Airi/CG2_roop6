#include<Windows.h>
#include"Window.h"

//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	Window window(1280, 720);
	
	//アプリケーションの開始
	window.StartApp();


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
		}
	}
	


	//アプリケーションの終了
	window.EndApp();

	return 0;

}