#include<Windows.h>
#include"WindowAPI.h"
#include"DirectX.h"
#include"Object.h"


//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	WindowAPI* window=new WindowAPI;
	DirectX* direct = new DirectX;
	Object* object = new Object;

	//アプリケーションの開始
	window->StartApp();
	
	direct->Initialize(window->GetHwnd());

	object->Initialize(direct->GetDevice(), direct->GetCommandQueue(), );



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
			
			direct->Run();
			
		}
	}
	

	
	//アプリケーションの終了
	window->EndApp();

	return 0;

}