#pragma once
#include<Windows.h>
class Window
{
public:
	//インスタンス
	Window();
	//デストラクタ
	~Window();

private:
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	
	int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
};

