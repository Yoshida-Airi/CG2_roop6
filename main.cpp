#include<Windows.h>
#include"WindowAPI.h"
#include"DirectX.h"


//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	WindowAPI window;
	DirectX direct;

	//アプリケーションの開始
	window.StartApp();
	
	direct.Initialize(window.GetHwnd());

	/*=====================================*/
	/* 　　　　   メインループ　　    　       */
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
			direct.Run();
		}
	}
	
	direct.End();

	//アプリケーションの終了
	window.EndApp();

	return 0;

}