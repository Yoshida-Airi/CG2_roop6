#pragma once
#include<d3d12.h>
#include<dxgi1_6.h>
#include<cassert>
#include"ConvertString.h"
#include<format>

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
	void Initialize();

private:

	/*=====================================*/
	/* 　　　　   プライベート変数    　        */
	/*=====================================*/

	IDXGIFactory7* dxgiFactory = nullptr;	//DXGIファクトリーの生成
	IDXGIAdapter4* useAdapter = nullptr;	//使用するアダプタ用の変数。
	ID3D12Device* device = nullptr;			//デバイスの生成

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

