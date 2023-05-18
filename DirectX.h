#pragma once
#include<cstdint>
#include<d3d12.h>
#include<dxgi1_6.h>
#include<cassert>
#include<cstdint>
#include<string>
#include<format>
#include<dxgidebug.h>

#include "ConvertString.h"
#include"WindowAPI.h"
#include"ProtectedVar.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dxguid.lib")

class DirectX:protected protectedVar
{
public:
	//コンストラクタ
	DirectX();
	//デストラクタ
	~DirectX();



	void Start();
	void Update();
	void EndD3D();

private:
	/*=====================================*/
	/* 　　　　   プライベート変数    　        */
	/*=====================================*/

	

	static const int32_t FrameCount_ = 2;//フレームバッファ数
	ID3D12Device* Device_ = nullptr;//デバイス
	IDXGIAdapter4* useAdapter_ = nullptr;//使用するアダプタ用の変数。最初にnullptrを入れておく
	IDXGIFactory7* dxgiFactory_ = nullptr;//dxgiFactory

	HRESULT hr;

	IDXGISwapChain4* swapChain_ = nullptr;	//スワップチェーン
	ID3D12Resource* swapChainResource_[2] = { nullptr };//スワップチェーンリソース
	ID3D12CommandQueue* commandQueue_ = nullptr;	//コマンドキュー
	ID3D12CommandAllocator* commandAllocator_ = nullptr;//コマンドアロケータ
	ID3D12GraphicsCommandList* commandList_ = nullptr;//コマンドリスト
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles_[2];//RTVを2つ作るのでディスクリプタを2つ用意

	ID3D12Fence* fence_ = nullptr;//フェンス
	uint64_t fenceValue_ = 0;
	HANDLE fenceEvent_ = CreateEvent(NULL, FALSE, FALSE, NULL);
	
	ID3D12InfoQueue* infoQueue_ = nullptr;//info

	//ディスクリプタヒープ
	ID3D12DescriptorHeap* rtvdescriptorHeap_ = nullptr;
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescriptorHeapDesc_{};
	
	WindowAPI windowAPI_;	//ウィンドウクラス



	/*=====================================*/
	/* 　　　　   プライベートメソッド　　　      */
	/*=====================================*/

	bool Initialize();
	void MainRoop();
	void End();

};

