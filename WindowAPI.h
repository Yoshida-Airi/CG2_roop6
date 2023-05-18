#pragma once
#include <windows.h>
#include<cstdint>
#include<d3d12.h>


class WindowAPI
{
public:

	
	/*=====================================*/
	/* 　　　　   パブリックメソッド　　　 　    */
	/*=====================================*/
	//コンストラクタ
	WindowAPI();
	//デストラクタ
	~WindowAPI();
	//アプリケーションの開始
	void StartApp();
	//アプリケーションの終了
	void EndApp();


	HWND GetHwnd() const { return hwnd_; };
	int32_t GetWidth()const { return Width_; };
	int32_t GetHeight()const { return Height_; };

private:

	/*=====================================*/
	/* 　　　　   プライベート変数    　        */
	/*=====================================*/


	//タイトル
	const wchar_t* Title_;

	HINSTANCE hInst_;	//インスタンスハンドル
	HWND hwnd_;			//ウィンドウハンドル
	int32_t Width_;		//ウィンドウの横幅
	int32_t Height_;	//ウィンドウの縦幅

	//デバッグコントローラー
#ifdef _DEBUG
	ID3D12Debug1* debugController_;
#endif

	

	RECT wrc_;

	//ウィンドウクラスの登録(設定をWindowsに伝える)
	WNDCLASS wc_;

	/*=====================================*/
	/* 　　　　   プライベートメソッド　　　      */
	/*=====================================*/

	//初期化
	bool Initialize();
	//アプリケーションの終了
	void EndRoop();
	//ウィンドウクラスを登録
	bool InitializeWindow();
	//ウィンドウの終了
	void EndWindow();





	//ウィンドウプロシージャ
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);


};