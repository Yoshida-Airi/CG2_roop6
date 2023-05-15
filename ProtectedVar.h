#pragma once
#include<cstdint>
#include<d3d12.h>

class protectedVar
{
protected:
	int32_t Width_;		//ウィンドウの横幅
	int32_t Height_;	//ウィンドウの縦幅
	HWND  hwnd_;		//ウィンドウハンドル
};