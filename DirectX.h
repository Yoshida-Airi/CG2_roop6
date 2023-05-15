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

	

	//フレームバッファ数
	static const int32_t FrameCount_ = 2;
	//デバイス
	ID3D12Device* Device_ = nullptr;
	//使用するアダプタ用の変数。最初にnullptrを入れておく
	IDXGIAdapter4* useAdapter_ = nullptr;
	//dxgiFactory
	IDXGIFactory7* dxgiFactory_ = nullptr;

	HRESULT hr;
	//スワップチェーン
	IDXGISwapChain4* swapChain_ = nullptr;
	//スワップチェーンリソース
	ID3D12Resource* swapChainResource_[2] = { nullptr };
	//コマンドキュー
	ID3D12CommandQueue* commandQueue_ = nullptr;
	//コマンドアロケータ
	ID3D12CommandAllocator* commandAllocator_ = nullptr;
	//コマンドリスト
	ID3D12GraphicsCommandList* commandList_ = nullptr;
	//RTVを2つ作るのでディスクリプタを2つ用意
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles_[2];
	//フェンス
	ID3D12Fence* fence_ = nullptr;
	uint64_t fenceValue_ = 0;
	HANDLE fenceEvent_ = CreateEvent(NULL, FALSE, FALSE, NULL);
	//info
	ID3D12InfoQueue* infoQueue_ = nullptr;

	//ディスクリプタヒープ
	ID3D12DescriptorHeap* rtvdescriptorHeap_ = nullptr;
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescriptorHeapDesc_{};
	//ウィンドウクラス
	WindowAPI window_;


	int32_t WindowWidth_;
	int32_t WindowHeight_;

	/*=====================================*/
	/* 　　　　   プライベートメソッド　　　      */
	/*=====================================*/

	bool Initialize();
	void MainRoop();
	void End();

};

