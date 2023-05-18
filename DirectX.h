#pragma once
#include<d3d12.h>
#include<dxgi1_6.h>
#include<cassert>
#include"ConvertString.h"
#include<format>
#include<dxgidebug.h>

#include"WindowAPI.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dxguid.lib")

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
	void Run();
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
	IDXGISwapChain4* swapChain = nullptr;	//スワップチェーン
	ID3D12DescriptorHeap* rtvdescriptorHeap = nullptr;	//ディスクリプタヒープ
	ID3D12Resource* swapChainResources[2] = { nullptr };//SwapChainからResourceを引っ張ってくる
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};//RTVの設定
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles[2];	//RTVを二つ作るのでディスクリプタを二つ用意
	ID3D12Fence* fence = nullptr;//初期値0でFenceを作る
	uint64_t fenceValue = 0;	//フェンス値
	HANDLE fenceEvent;
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

	//スワップチェーンからもってくる
	void PullResourceSwapChain();

	//RTV作成
	void CreateRTV();

	// フェンス生成
	void CreateFence();

	//コマンドのキック
	void CommandKick();

	//解放
	void HandleClose();
};

