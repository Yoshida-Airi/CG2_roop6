#include<Windows.h>
#include"WindowAPI.h"
#include"DirectX.h"
#include"Triangle.h"


//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	WindowAPI* window = new WindowAPI;
	DirectX* direct = new DirectX;
	Triangle* triangle = new Triangle;

	float x1 = -0.5f;
	float y1 = -0.5f;
	float x2=0.0f;
	float y2=0.5f;
	float x3=0.5f;
	float y3=-0.5f;
	
	int objectCount = 10;

	const float size = 0.f;

	//アプリケーションの開始
	window->StartApp();
	direct->Initialize(window->GetHwnd());
	triangle->Initialize();

	/*=====================================*/
	/* 　　　　   メインループ　　    　       */
	/*=====================================*/

		//Windowsにメッセージが来てたら最優先で処理させる
		while (window->ProcessMessage() == 0) 
		{
			triangle->Draw(x1, y1, x2, y2, x3, y3);
			triangle->Draw(x1 + size, y1 + size, x2 + size, y2 + size, x3 + size, y3 + size);

			//ゲームの処理
			triangle->Run();


		}
	

	triangle->End();
	direct->End();


	//アプリケーションの終了
	window->EndApp();

	return 0;

}