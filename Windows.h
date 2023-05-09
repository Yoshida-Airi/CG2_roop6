#pragma once

#include <Windows.h>
#include <cstdint>

class Window
{
public:
	bool Initialize(LPCWSTR pTitle, int width, int height);

private:
	LPCWSTR pTitle;
};

