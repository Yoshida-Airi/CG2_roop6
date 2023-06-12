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

	//ゲッター
	ID3D12Device* GetDevice() const { return device_; }
	ID3D12GraphicsCommandList* GetCommandList()const { return  commandList_; };
	IDXGISwapChain4* GetSwapChain()const { return swapChain_; };	//スワップチェーン
	ID3D12Resource* GetSwapChainResource(int index)const { return swapChainResources_[index]; };
	D3D12_CPU_DESCRIPTOR_HANDLE& GetRTVHandle(int index)const
	{
		assert(index >= 0 && index < 2);
		return rtvHandles_[index];
	}
	ID3D12CommandQueue* GetCommandQueue()const { return commandQueue_; };	//コマンドキュー
	ID3D12CommandAllocator* GetCommandAllocator()const { return commandAllocator_; };	//コマンドアロケータ
	ID3D12Fence* GetFence()const { return fence_; };//初期値0でFenceを作る
	uint64_t GetFenceValue()const { return fenceValue_; };	//フェンス値
	HANDLE GetFenceEvent() { return fenceEvent_; };

	//セッター




private:

	/*=====================================*/
	/* 　　　　   プライベート変数    　        */
	/*=====================================*/

	//ウィンドウ管理
	WindowAPI winApp_;
	HWND hwnd_;
	IDXGIFactory7* dxgiFactory = nullptr;	//DXGIファクトリーの生成
	IDXGIAdapter4* useAdapter = nullptr;	//使用するアダプタ用の変数。
	ID3D12DescriptorHeap* rtvdescriptorHeap = nullptr;	//ディスクリプタヒープ
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};//RTVの設定


	HRESULT hr_;				//結果確認用
	static ID3D12Device* device_;	//デバイスの生成
	static ID3D12GraphicsCommandList* commandList_;	//コマンドリスト
	static IDXGISwapChain4* swapChain_;	//スワップチェーン
	static ID3D12Resource* swapChainResources_[2];//SwapChainからResourceを引っ張ってくる
	static D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles_[2];	//RTVを二つ作るのでディスクリプタを二つ用意
	static ID3D12CommandQueue* commandQueue_;	//コマンドキュー
	static ID3D12CommandAllocator* commandAllocator_;	//コマンドアロケータ
	static ID3D12Fence* fence_;//初期値0でFenceを作る
	uint64_t fenceValue_;	//フェンス値
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

	//解放
	void HandleClose();

};

