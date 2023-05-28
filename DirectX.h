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
	void End();

	ID3D12Device* GetDevice() { return device_; };
	ID3D12GraphicsCommandList* GetCommandQueue() { return commandList_; };

private:

	/*=====================================*/
	/* 　　　　   プライベート変数    　        */
	/*=====================================*/

	//ウィンドウ管理
	WindowAPI winApp_;
	HWND hwnd_;

	IDXGIFactory7* dxgiFactory_ = nullptr;	//DXGIファクトリーの生成
	HRESULT hr_;								//結果確認用
	IDXGIAdapter4* useAdapter_ = nullptr;	//使用するアダプタ用の変数。
	ID3D12Device* device_ = nullptr;			//デバイスの生成
	ID3D12CommandQueue* commandQueue_ = nullptr;	//コマンドキュー
	ID3D12CommandAllocator* commandAllocator_ = nullptr;	//コマンドアロケータ
	ID3D12GraphicsCommandList* commandList_ = nullptr;	//コマンドリスト
	IDXGISwapChain4* swapChain_ = nullptr;	//スワップチェーン
	ID3D12DescriptorHeap* rtvdescriptorHeap_ = nullptr;	//ディスクリプタヒープ
	ID3D12Resource* swapChainResources_[2] = { nullptr };//SwapChainからResourceを引っ張ってくる
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc_{};//RTVの設定
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles_[2];	//RTVを二つ作るのでディスクリプタを二つ用意
	D3D12_RESOURCE_BARRIER barrier_{};	//TransitionBarrierの設定
	ID3D12Fence* fence_ = nullptr;//初期値0でFenceを作る
	uint64_t fenceValue_ = 0;	//フェンス値
	HANDLE fenceEvent_;
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

	//コマンドの実行
	void CommandPlay();

	//解放
	void HandleClose();
};

