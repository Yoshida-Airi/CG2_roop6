#include "DirectX.h"

/*=====================================*/
/* 　　　　   パブリックメソッド　　　 　     */
/*=====================================*/

//コンストラクタ
DirectX::DirectX()
{

}
//デストラクタ
DirectX::~DirectX()
{

}

//初期化
void DirectX::Initialize()
{
	//デバイスの生成
	InitializeDXGIDevice();
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
	HRESULT hr = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));
	//初期化の根本的な部分でエラーが出た場合はプログラムが間違ているか
	//どうにもできない場合が多いのでassertにしておく
	assert(SUCCEEDED(hr));

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
		hr = useAdapter->GetDesc3(&adapterDesc);
		assert(SUCCEEDED(hr));//取得できないのは一大事
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
		hr = D3D12CreateDevice(useAdapter, featureLevels[i], IID_PPV_ARGS(&device));
		//指定した機能レベルでデバイスが生成出来たかを確認
		if (SUCCEEDED(hr))
		{
			//生成できたのでログ出力を行ってループを抜ける
			Log(std::format("FeatureLevel : {}\n", featureLevelStrings[i]));
			break;
		}
	}
	//デバイスの生成がうまくいかなかったので起動できない
	assert(device != nullptr);
	Log("Complete create D3D12Device!!!\n");//初期化完了のログをだす

}


// コマンド関連初期化
void DirectX::InitializeCommand()
{

}

// スワップチェーンの生成
void DirectX::CreateSwapChain()
{

}

// レンダーターゲット生成
void DirectX::CreateFinalRenderTargets()
{

}

// 深度バッファ生成
void DirectX::CreateDepthBuffer()
{

}

// フェンス生成
void DirectX::CreateFence()
{

}