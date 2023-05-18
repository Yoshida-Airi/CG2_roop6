#pragma once
#include<d3d12.h>
#include<dxgi1_6.h>
#include<cassert>
#include"ConvertString.h"
#include<format>

#include"WindowAPI.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

class DirectX
{
public:


	/*=====================================*/
	/* 　　　　   パブリックメソッド　　　 　     */
	/*=====================================*/

	//コンストラクタ
	DirectX();
	//デストラクタ
	~DirectX();

	//初期化
	void Initialize(HWND hwnd);

private:

	/*=====================================*/
	/* 　　　　   プライベート変数    　        */
	/*=====================================*/

	//ウィンドウ管理
	WindowAPI winApp_;

	HWND hwnd_;

	IDXGIFactory7* dxgiFactory = nullptr;	//DXGIファクトリーの生成
	HRESULT hr;								//結果確認用
	IDXGIAdapter4* useAdapter = nullptr;	//使用するアダプタ用の変数。
	ID3D12Device* device = nullptr;			//デバイスの生成
	ID3D12CommandQueue* commandQueue = nullptr;	//コマンドキュー
	ID3D12CommandAllocator* commandAllocator = nullptr;	//コマンドアロケータ
	ID3D12GraphicsCommandList* commandList = nullptr;	//コマンドリスト

	/*=====================================*/
	/* 　　　　   プライベートメソッド　　　      */
	/*=====================================*/

	//DXGIファクトリーの生成
	void InitializeDXGIDevice();
	// コマンド関連初期化
	void InitializeCommand();

	// スワップチェーンの生成
	void CreateSwapChain();

	// レンダーターゲット生成
	void CreateFinalRenderTargets();

	// 深度バッファ生成
	void CreateDepthBuffer();

	// フェンス生成
	void CreateFence();
};

