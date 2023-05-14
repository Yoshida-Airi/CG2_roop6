#pragma once
#include<cstdint>
#include<d3d12.h>
#include<dxgi1_6.h>
#include<cassert>
#include<cstdint>
#include<string>
#include<format>
#include<dxgidebug.h>
#include<dxgidebug.h>
#include<dxcapi.h>
#include "ConvertString.h"
#include"Window.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dxcompiler.lib")

class DirectX
{
public:
	//コンストラクタ
	DirectX();
	//デストラクタ
	~DirectX();

	HWND hwnd_;

	void Start();
	void Update();
	void EndD3D();


	ID3D12Device* GetDevice() const { return Device; }
	//コマンドリスト
	ID3D12GraphicsCommandList* GetCommandList()const { return commandList; }

	void SetHr(HRESULT hr_);
	void SetCommandList(ID3D12GraphicsCommandList* commandList_);

private:
	/*=====================================*/
	/* 　　　　   プライベート変数    　        */
	/*=====================================*/

	//フレームバッファ数
	static const int32_t FrameCount = 2;
	//デバイス
	ID3D12Device* Device;
	//使用するアダプタ用の変数。最初にnullptrを入れておく
	IDXGIAdapter4* useAdapter = nullptr;
	//dxgiFactory
	IDXGIFactory7* dxgiFactory = nullptr;

	HRESULT hr;
	//スワップチェーン
	IDXGISwapChain4* swapChain = nullptr;
	//スワップチェーンリソース
	ID3D12Resource* swapChainResource[2] = { nullptr };
	//コマンドキュー
	ID3D12CommandQueue* commandQueue = nullptr;
	//コマンドアロケータ
	ID3D12CommandAllocator* commandAllocator = nullptr;
	//コマンドリスト
	ID3D12GraphicsCommandList* commandList = nullptr;
	//RTVを2つ作るのでディスクリプタを2つ用意
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles[2];
	//フェンス
	ID3D12Fence* fence = nullptr;
	uint64_t fenceValue = 0;
	HANDLE fenceEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	//info
	ID3D12InfoQueue* infoQueue = nullptr;

	//ディスクリプタヒープ
	ID3D12DescriptorHeap* rtvdescriptorHeap = nullptr;
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescriptorHeapDesc{};
	//ウィンドウクラス
	Window window_;


	int32_t WindowWidth;
	int32_t WindowHeight;

	/*=====================================*/
	/* 　　　　   プライベートメソッド　　　      */
	/*=====================================*/

	bool Initialize();
	void MainRoop();
	void End();

};

