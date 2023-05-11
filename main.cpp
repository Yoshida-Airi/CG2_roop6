#include<Windows.h>
#include"Window.h"

//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	Window window(1280, 720);
	window.Run();

	return 0;

}