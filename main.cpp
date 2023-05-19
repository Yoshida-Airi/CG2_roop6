#include<Windows.h>
#include"WindowAPI.h"
#include"DirectX.h"
#include"Object.h"


//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	WindowAPI window;
	DirectX direct;
	Object object;

	//アプリケーションの開始
	window.StartApp();
	
	direct.Initialize(window.GetHwnd());

	object.DrawInitialize(direct.GetDevice(), direct.GetCommandList());

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
			object.Run(direct.GetCommandList(), direct.GetswapChain(),direct.Getbarrier() );
			direct.Run();
			
		}
	}
	

	object.End();
	//アプリケーションの終了
	window.EndApp();

	return 0;

}