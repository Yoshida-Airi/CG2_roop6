#include<Windows.h>
#include"WindowAPI.h"
#include"DirectX.h"
#include"Engine.h"

#include"Triangle.h"

//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	WindowAPI* window = new WindowAPI;
	DirectX* direct = new DirectX;
	Engine* engine = new Engine;



	//アプリケーションの開始
	window->StartApp();
	direct->Initialize(window->GetHwnd());
	engine->Initialize();

	Vector4 data1[3];
	Vector4 data2[3];
	Vector4 data3[3];
	Vector4 colorA = { 1.0f,0.0f,0.0f,1.0f };

	for (int i = 0; i < 3; i++)
	{

		data1[i] = { -0.2f,-0.2f + (i * -0.5f),0.0f,2.0f };
		data2[i] = { 0.0f,0.2f + (i * -0.5f),0.0f,2.0f };
		data3[i] = { 0.2f,-0.2f + (i * -0.5f),0.0f,2.0f };
	}

	Vector4 data4[3];
	Vector4 data5[3];
	Vector4 data6[3];
	Vector4 colorB = { 1.0f,1.0f,0.0f,1.0f };

	for (int i = 0; i < 3; i++)
	{

		data4[i] = { 0.2f,-0.2f + (i * -0.5f),0.0f,2.0f };
		data5[i] = { 0.4f,0.2f + (i * -0.5f),0.0f,2.0f };
		data6[i] = { 0.6f,-0.2f + (i * -0.5f),0.0f,2.0f };
	}

	Vector4 data7[4];
	Vector4 data8[4];
	Vector4 data9[4];
	Vector4 colorC = { 1.0f,0.0f,1.0f,1.0f };

	for (int i = 0; i < 4; i++)
	{

		data7[i] = { -0.6f,-0.2f + (i * -0.5f),0.0f,2.0f };
		data8[i] = { -0.4f,0.2f + (i * -0.5f),0.0f,2.0f };
		data9[i] = { -0.2f,-0.2f + (i * -0.5f),0.0f,2.0f };
	}

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
			engine->Run();

			for (int i = 0; i < 3; i++)
			{
				//三角形描画
				engine->DrawTriangle(data1[i], data2[i], data3[i],colorA);
			}

			for (int i = 0; i < 3; i++)
			{
				//三角形描画
				engine->DrawTriangle(data4[i], data5[i], data6[i], colorB);
			}

			for (int i = 0; i < 4; i++)
			{
				//三角形描画
				engine->DrawTriangle(data7[i], data8[i], data9[i], colorC);
			}

			engine->RunEnd();

		}
	}

	window->EndApp();
	engine->End();
	direct->End();




	return 0;

}