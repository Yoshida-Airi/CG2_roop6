#include "DirectX.h"



/*=====================================*/
/* 　　　　   パブリックメソッド　　　 　     */
/*=====================================*/

//コンストラクタ
DirectX::DirectX()
{
	hr_ = 0;
}
//デストラクタ
DirectX::~DirectX()
{

}

//初期化
void DirectX::Initialize(HWND hwnd)
{
	hwnd_ = hwnd;
	//デバイスの生成
	InitializeDXGIDevice();
	//コマンド初期化
	InitializeCommand();
	//スワップチェーン
	CreateSwapChain();
	// レンダーターゲット生成
	CreateFinalRenderTargets();
	// SwapChainからリソースをひっぱってくる
	PullResourceSwapChain();
	//RTVの設定
	CreateRTV();
	//フェンスの作成
	CreateFence();

}

void DirectX::Run()
{
	//コマンドをキックする
	CommandKick();
}

void DirectX::End()
{
	HandleClose();
}



/*=====================================*/
/* 　　　　   プライベートメソッド　　　      */
/*=====================================*/



void DirectX::InitializeDXGIDevice()
{
	//--------------------------------
//DXGIファクトリーの生成
//-------------------------------

//HRESULTはWindows系のエラーコードであり、
//関数が成功したかどうかをSUCCEEDEDマクロで判定できる
	hr_ = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));
	//初期化の根本的な部分でエラーが出た場合はプログラムが間違ているか
	//どうにもできない場合が多いのでassertにしておく
	assert(SUCCEEDED(hr_));

	//-----------------------------------
	//アダプタの決定
	//-----------------------------------

	//良い順にアダプタを頼む
	for (UINT i = 0;
		dxgiFactory->EnumAdapterByGpuPreference(i, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&useAdapter)) != DXGI_ERROR_NOT_FOUND;
		++i)
	{
		//アダプターの情報を取得する
		DXGI_ADAPTER_DESC3 adapterDesc{};
		hr_ = useAdapter->GetDesc3(&adapterDesc);
		assert(SUCCEEDED(hr_));//取得できないのは一大事
		//ソフトウェアアダプタでなければ採用！
		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE))
		{
			//採用したアダプタの情報をログに出力。wstringの方なので注意
			Log(ConvertString(std::format(L"Use Adapter:{}\n", adapterDesc.Description)));
			break;
		}
		useAdapter = nullptr;//ソフトウェアアダプタの場合はみなかったことにする
	}
	//適切なアダプタが見付らなかったので起動できない
	assert(useAdapter != nullptr);

	//--------------------------------
	//D3D12Deviceの生成
	//----------------------------------

	//機能レベルとログ出力用の文字列
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_12_2,D3D_FEATURE_LEVEL_12_1,D3D_FEATURE_LEVEL_12_0
	};
	const char* featureLevelStrings[] =
	{
		"12.2",
		"12.1",
		"12.0"
	};
	//高い順に生成できるか試していく
	for (size_t i = 0; i < _countof(featureLevels); ++i)
	{
		//採用したアダプターでデバイスを生成
		hr_ = D3D12CreateDevice(useAdapter, featureLevels[i], IID_PPV_ARGS(&device_));
		//指定した機能レベルでデバイスが生成出来たかを確認
		if (SUCCEEDED(hr_))
		{
			//生成できたのでログ出力を行ってループを抜ける
			Log(std::format("FeatureLevel : {}\n", featureLevelStrings[i]));
			break;
		}
	}
	//デバイスの生成がうまくいかなかったので起動できない
	assert(device_ != nullptr);
	Log("Complete create D3D12Device!!!\n");//初期化完了のログをだす

#ifdef _DEBUG
	ID3D12InfoQueue* infoQueue = nullptr;
	if (SUCCEEDED(device_->QueryInterface(IID_PPV_ARGS(&infoQueue))))
	{
		//ヤバイエラー時に止まる
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
		//エラー時に止まる
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
		//警告時に止まる
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);

		//警告時に止まる
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);
		//抑制するメッセージのID
		D3D12_MESSAGE_ID denyIds[] =
		{
			//Windows11でのDXGIデバッグレイヤーとDX12デバッグレイヤーの相互作用バグによるエラーメッセージ
			D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE
		};
		//抑制するレベル
		D3D12_MESSAGE_SEVERITY severities[] = { D3D12_MESSAGE_SEVERITY_INFO };
		D3D12_INFO_QUEUE_FILTER filter{};
		filter.DenyList.NumIDs = _countof(denyIds);
		filter.DenyList.pIDList = denyIds;
		filter.DenyList.NumSeverities = _countof(severities);
		filter.DenyList.pSeverityList = severities;
		//指定したメッセージの表示を抑制する
		infoQueue->PushStorageFilter(&filter);
		//解放
		infoQueue->Release();
	}
#endif
}


// コマンド関連初期化
void DirectX::InitializeCommand()
{
	//-----------------------------
	//コマンドキューを生成する
	//------------------------------

	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};
	hr_ = device_->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&commandQueue_));
	//コマンドキューの生成がうまくいかなかったので起動できない
	assert(SUCCEEDED(hr_));

	//------------------------------------
	//コマンドアロケータを生成する
	//-------------------------------------

	hr_ = device_->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator_));
	//コマンドアロケータの生成がうまくいかなかったので起動できない
	assert(SUCCEEDED(hr_));

	//---------------------------------
	//コマンドリストを生成する
	//--------------------------------

	hr_ = device_->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator_, nullptr, IID_PPV_ARGS(&commandList_));
	//コマンドリストの生成がうまくいかなかったので起動できない
	assert(SUCCEEDED(hr_));
}

// スワップチェーンの生成
void DirectX::CreateSwapChain()
{
	//----------------------------------
	//スワップチェーンを生成する
	//---------------------------------

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	swapChainDesc.Width = winApp_.GetWidth();	//画面の幅。クライアント領域を同じものにしておく
	swapChainDesc.Height = winApp_.GetHeight();//画面の高さ。ウィンドウのクライアント領域を同じものにしておく
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;//色の形式
	swapChainDesc.SampleDesc.Count = 1;//マルチサンプルしない
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;//描画のターゲットを利用する
	swapChainDesc.BufferCount = 2;//ダブルバッファ
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;//モニタにうつしたら、中身を破棄
	//コマンドキュー、ウィンドウハンドル、設定を渡して生成する
	hr_ = dxgiFactory->CreateSwapChainForHwnd(commandQueue_, hwnd_, &swapChainDesc, nullptr, nullptr, reinterpret_cast<IDXGISwapChain1**>(&swapChain_));
	assert(SUCCEEDED(hr_));
}

void DirectX::CreateFinalRenderTargets()
{
	//---------------------------------
	//ディスクリプタヒープの生成
	//---------------------------------

	D3D12_DESCRIPTOR_HEAP_DESC rtvDescriptorHeapDesc{};
	rtvDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;	//レンダーターゲットビュー用
	rtvDescriptorHeapDesc.NumDescriptors = 2;						//ダブルバッファ用にふたつ。多くても別に構わない。
	hr_ = device_->CreateDescriptorHeap(&rtvDescriptorHeapDesc, IID_PPV_ARGS(&rtvdescriptorHeap));
	//ディスクリプタヒープが作れなかったので起動できない
	assert(SUCCEEDED(hr_));
}

// SwapChainからリソースをひっぱってくる
void DirectX::PullResourceSwapChain()
{
	hr_ = swapChain_->GetBuffer(0, IID_PPV_ARGS(&swapChainResources_[0]));
	//うまく取得できなければ起動できない
	assert(SUCCEEDED(hr_));
	hr_ = swapChain_->GetBuffer(1, IID_PPV_ARGS(&swapChainResources_[1]));
	assert(SUCCEEDED(hr_));
}

void DirectX::CreateRTV()
{

	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	//ディスクリプタの先頭を取得する
	D3D12_CPU_DESCRIPTOR_HANDLE rtvStartHandle = rtvdescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	//まず１つ目を作る。１つ目は最初のところに作る。作る場所をこちらで指定してあげる必要がある
	rtvHandles_[0] = rtvStartHandle;
	device_->CreateRenderTargetView(swapChainResources_[0], &rtvDesc, rtvHandles_[0]);
	//2つ目のディスクリプタハンドルを得る(自力で)
	rtvHandles_[1].ptr = rtvHandles_[0].ptr + device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	//二つ目を作る
	device_->CreateRenderTargetView(swapChainResources_[1], &rtvDesc, rtvHandles_[1]);
}


// フェンス生成
void DirectX::CreateFence()
{
	hr_ = device_->CreateFence(fenceValue_, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence_));
	assert(SUCCEEDED(hr_));

	//FenceのSignalを持つためのイベントを作成する
	fenceEvent_ = CreateEvent(NULL, FALSE, FALSE, NULL);
	assert(fenceEvent_ != nullptr);

}

//コマンドのキック
void DirectX::CommandKick()
{


}

//解放処理
void DirectX::HandleClose()
{
	CloseHandle(fenceEvent_);
	fence_->Release();
	rtvdescriptorHeap->Release();
	swapChainResources_[0]->Release();
	swapChainResources_[1]->Release();
	swapChain_->Release();
	commandList_->Release();
	commandAllocator_->Release();
	commandQueue_->Release();
	device_->Release();
	useAdapter->Release();
	dxgiFactory->Release();

	CloseWindow(hwnd_);



	//リソースリークチェック
	IDXGIDebug1* debug;
	if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&debug))))
	{
		debug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
		debug->ReportLiveObjects(DXGI_DEBUG_APP, DXGI_DEBUG_RLO_ALL);
		debug->ReportLiveObjects(DXGI_DEBUG_D3D12, DXGI_DEBUG_RLO_ALL);
		debug->Release();
	}
}



ID3D12Device* DirectX::device_;
ID3D12CommandQueue* DirectX::commandQueue_;
ID3D12CommandAllocator* DirectX::commandAllocator_;
ID3D12GraphicsCommandList* DirectX::commandList_;
IDXGISwapChain4* DirectX::swapChain_;	//スワップチェーン
ID3D12Resource* DirectX::swapChainResources_[2];//SwapChainからResourceを引っ張ってくる
D3D12_CPU_DESCRIPTOR_HANDLE DirectX::rtvHandles_[2];	//RTVを二つ作るのでディスクリプタを二つ用意
ID3D12Fence* DirectX::fence_;//初期値0でFenceを作る