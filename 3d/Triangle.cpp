#include "Triangle.h"



Triangle::Triangle()
{
	fenceValue_ = direct_->GetFenceValue();
}

Triangle::~Triangle()
{

}

void Triangle::Initialize()
{
	IntializeDXC();
	CreateSignature();
	InputLayout();
	BlendState();
	RasterizerState();
	ShaderCompile();
	CreatePSO();

	//頂点データ
	VertexResource();
}

void Triangle::Run()
{
	Render();
}

void Triangle::End()
{
	HandleClose();
}

void Triangle::Draw(float x1, float y1, float x2, float y2, float x3, float y3)
{
	Resource(x1, y1, x2, y2, x3, y3);
}

//CompileShader関数
IDxcBlob* Triangle::CompileShader
(
	//compilerするshaderファイルへのパス
	const std::wstring& filePath,
	//Compilerに使用するprofile
	const wchar_t* profile,
	//初期化で生成したものを3つ
	IDxcUtils* dxcUtils,
	IDxcCompiler3* dxcCompiler,
	IDxcIncludeHandler* includeHandler
)
{
	//1.hlslファイルを読む
	//これからシェーダーをコンパイルする旨をログに出す
	Log(ConvertString(std::format(L"Begin CompileShader,path:{},profile:{}\n", filePath, profile)));
	//hlslファイルを読む
	IDxcBlobEncoding* shaderSource = nullptr;
	HRESULT hr = dxcUtils->LoadFile(filePath.c_str(), nullptr, &shaderSource);
	//読めなかったら止める
	assert(SUCCEEDED(hr));
	//読み込んだファイルの内容を設定する
	DxcBuffer shaderSourceBuffer;
	shaderSourceBuffer.Ptr = shaderSource->GetBufferPointer();
	shaderSourceBuffer.Size = shaderSource->GetBufferSize();
	shaderSourceBuffer.Encoding = DXC_CP_UTF8;//UTF8の文字コードであることを通知

	//2.compileする
	LPCWSTR arguments[] =
	{
		filePath.c_str(),//コンパイル対象のhlslファイル名
		L"-E",L"main",	 //エントリーポイントの指定。基本的にmain以外にはしない
		L"-T",profile,	 //ShaderProfileの設定
		L"-Od",			 //最適化をはずしておく
		L"-Zpr",		 //メモリアウトは行優先
	};
	//実際にShaderをコンパイルする
	IDxcResult* shaderResult = nullptr;
	hr = dxcCompiler->Compile
	(
		&shaderSourceBuffer,		//読み込んだファイル
		arguments,					//コンパイルオプション
		_countof(arguments),
		includeHandler,				//コンパイルオプションの数
		IID_PPV_ARGS(&shaderResult)	//includeが含まれた諸々
	);
	//コンパイルエラーではなくdxcが起動できないなど致命的な状況
	assert(SUCCEEDED(hr));

	//3.警告・エラーが出ていないか確認する
	//警告・エラーが出てたらログに出して止める
	IDxcBlobUtf8* shaderError = nullptr;
	if (shaderResult != 0)
	{
		shaderResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&shaderError), nullptr);
		if (shaderError != nullptr && shaderError->GetStringLength() != 0)
		{
			Log(shaderError->GetStringPointer());
			//警告・エラーダメゼッタイ
			assert(false);
		}
	}
	//4.Compile結果を受け取って返す
	//コンパイル結果から実行用のバイナリ部分を取得
	IDxcBlob* shaderBlob = nullptr;
	hr = shaderResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shaderBlob), nullptr);
	assert(SUCCEEDED(hr));
	//成功したログを出す
	Log(ConvertString(std::format(L"Compile Succeeded,path:{},profile:{}\n", filePath, profile)));
	//もう使わないリソースを解放
	shaderSource->Release();
	shaderResult->Release();
	//実行用のバイナリを返却
	return shaderBlob;

}


void Triangle::IntializeDXC()
{
	hr_ = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils_));
	assert(SUCCEEDED(hr_));
	hr_ = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler_));
	assert(SUCCEEDED(hr_));

	hr_ = dxcUtils_->CreateDefaultIncludeHandler(&includeHandler_);
	assert(SUCCEEDED(hr_));
}

void Triangle::CreateSignature()
{

	descriptionRootSignature_.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	//シリアライズしてバイナリにする
	hr_ = D3D12SerializeRootSignature(&descriptionRootSignature_, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob_, &errorBlob_);
	if (FAILED(hr_))
	{
		Log(reinterpret_cast<char*>(errorBlob_->GetBufferPointer()));
		assert(false);
	}

	hr_ = direct_->GetDevice()->CreateRootSignature(0, signatureBlob_->GetBufferPointer(), signatureBlob_->GetBufferSize(), IID_PPV_ARGS(&rootSignature_));
	assert(SUCCEEDED(hr_));
}

void Triangle::InputLayout()
{
	inputElementDescs_[0].SemanticName = "POSITION";
	inputElementDescs_[0].SemanticIndex = 0;
	inputElementDescs_[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs_[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	inputLayoutDesc_.pInputElementDescs = inputElementDescs_;
	inputLayoutDesc_.NumElements = _countof(inputElementDescs_);
}

void Triangle::BlendState()
{
	//すべての色要素を書き込む
	blendDesc_.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
}


void Triangle::RasterizerState()
{
	//裏面(時計回り)を表示しない
	rasterizerDesc_.CullMode = D3D12_CULL_MODE_BACK;
	//三角形の中を塗りつぶす
	rasterizerDesc_.FillMode = D3D12_FILL_MODE_SOLID;
}

void Triangle::ShaderCompile()
{
	//Shaderをコンパイルする
	vertexShaderBlob_ = CompileShader(L"Object3d.VS.hlsl", L"vs_6_0", dxcUtils_, dxcCompiler_, includeHandler_);
	assert(vertexShaderBlob_ != nullptr);

	pixelShaderBlob_ = CompileShader(L"Object3d.PS.hlsl", L"ps_6_0", dxcUtils_, dxcCompiler_, includeHandler_);
	assert(pixelShaderBlob_ != nullptr);
}

void Triangle::CreatePSO()
{

	graphicsPipelineStateDesc_.pRootSignature = rootSignature_;	//RootSignature
	graphicsPipelineStateDesc_.InputLayout = inputLayoutDesc_;	//InputLayout
	graphicsPipelineStateDesc_.VS = { vertexShaderBlob_->GetBufferPointer(),vertexShaderBlob_->GetBufferSize() };	//VertexShader
	graphicsPipelineStateDesc_.PS = { pixelShaderBlob_->GetBufferPointer(),pixelShaderBlob_->GetBufferSize() };	//PixelShader
	graphicsPipelineStateDesc_.BlendState = blendDesc_;	//BlendDesc
	graphicsPipelineStateDesc_.RasterizerState = rasterizerDesc_;	//RasterrizerState
	//書き込むRTVの情報
	graphicsPipelineStateDesc_.NumRenderTargets = 1;
	graphicsPipelineStateDesc_.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	//利用するトポロジ(形状)のタイプ。三角形
	graphicsPipelineStateDesc_.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	//どのように画面に色を打ち込むのかの設定(気にしなくて良い)
	graphicsPipelineStateDesc_.SampleDesc.Count = 1;
	graphicsPipelineStateDesc_.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	hr_ = direct_->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc_, IID_PPV_ARGS(&graphicsPipelineState_));
	assert(SUCCEEDED(hr_));
}


void Triangle::VertexResource()
{

	//頂点データ

	//頂点リソース用のヒープの設定
	D3D12_HEAP_PROPERTIES uploadHeapProperties{};
	uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;	//UplodeHeapを使う

	//バッファリソース。テクスチャの場合は別の設定する
	vertexResourceDesc_.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	vertexResourceDesc_.Width = sizeof(Vector4) * indexVertex_;	//リソースのサイズ。今回はVector4を3頂点分
	//バッファの場合はこれらは1にする決まり
	vertexResourceDesc_.Height = 1;
	vertexResourceDesc_.DepthOrArraySize = 1;
	vertexResourceDesc_.MipLevels = 1;
	vertexResourceDesc_.SampleDesc.Count = 1;
	//バッファの場合はこれにする決まり
	vertexResourceDesc_.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	hr_ = direct_->GetDevice()->CreateCommittedResource(&uploadHeapProperties, D3D12_HEAP_FLAG_NONE, &vertexResourceDesc_, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertexResource_));

	assert(SUCCEEDED(hr_));



	//リソースの先頭のアドレスから使う
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();

	//使用するリソースのサイズは頂点3つ分のサイズ
	vertexBufferView_.SizeInBytes = sizeof(Vector4) * static_cast<UINT>(indexVertex_);
	//1頂点あたりのサイズ
	vertexBufferView_.StrideInBytes = sizeof(Vector4);

}

void Triangle::Resource(float x1, float y1, float x2, float y2, float x3, float y3)
{
	indexVertex_ = indexTriangle_ * kVertexCountTriangle;

	//頂点リソースにデータを書き込む
	Vector4* vertexData = nullptr;
	//書き込むためのアドレスを取得
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData[indexTriangle_]));

	//// 三角形の位置を計算
	//float positionX = i * 0.0f;
	//float positoinY = i * 0.2f - 1.3f;

	//// 左下
	//vertexData[i * 3] = { -size + positionX, -size + positoinY, 0.0f, 1.5f };
	//// 上
	//vertexData[i * 3 + 1] = { 0.0f + positionX, size + positoinY - 0.02f, 0.0f, 1.5f };
	//// 右下
	//vertexData[i * 3 + 2] = { size + positionX, -size + positoinY, 0.0f, 1.5f };

	//左下
	vertexData[indexTriangle_*3] = { x1,y1,0.0f,2.5f };
	//上
	vertexData[indexTriangle_*3+1] = { x2,y2,0.0f,2.5f };
	//右下
	vertexData[indexTriangle_*3+2] = { x3,y3,0.0f,2.5f };

	////コマンドを積む
	//direct_->GetCommandList()->RSSetViewports(1, &viewport_);	//Viewportを設定
	//direct_->GetCommandList()->RSSetScissorRects(1, &scissorRect_);	//Scirssorを設定
	////RootSignatureを設定。PSOに設定しているけど別途設定が必要
	//direct_->GetCommandList()->SetGraphicsRootSignature(rootSignature_);
	//direct_->GetCommandList()->SetPipelineState(graphicsPipelineState_);	//PSOを設定
	//direct_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);	//VBVを設定


	////形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけばよい
	//direct_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	////描画！(DrawCall/ドローコール)。3頂点で1つのインスタンス。
	//direct_->GetCommandList()->DrawInstanced(kVertexCountTriangle, 1, static_cast<UINT>(indexVertex_), 0);


	indexTriangle_++;


}


void Triangle::Render()
{



	//クライアント領域のサイズと一緒にして画面全体に表示
	viewport_.Width = static_cast<float>(winApp_.GetWidth());
	viewport_.Height = static_cast<float>(winApp_.GetHeight());

	viewport_.TopLeftX = 0;
	viewport_.TopLeftY = 0;
	viewport_.MinDepth = 0.0f;
	viewport_.MaxDepth = 1.0f;


	//基本的にビューポートと同じ矩形が構成されるようにする
	scissorRect_.left = 0;
	scissorRect_.right = winApp_.GetWidth();
	scissorRect_.top = 0;
	scissorRect_.bottom = winApp_.GetHeight();

	//これから書き込むバッグバッファのインデックスを取得
	backBufferIndex = direct_->GetSwapChain()->GetCurrentBackBufferIndex();


	//TransitionBarrierの設定
	D3D12_RESOURCE_BARRIER barrier{};
	//今回のバリアはTransition
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	//Noneにしておく
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	//バリアを張る対象リソース。現在のバッグバッファに対して行う
	barrier.Transition.pResource = direct_->GetSwapChainResource(backBufferIndex);
	//遷移前(現在)のResourceState
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	//遷移後のResourceState
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	//TransitionBarrierを張る
	direct_->GetCommandList()->ResourceBarrier(1, &barrier);
	//描画先のRTVを設定する
	direct_->GetCommandList()->OMSetRenderTargets(1, &direct_->GetRTVHandle(backBufferIndex), false, nullptr);
	//指定した色で画面全体をクリアする
	float clearcolor[] = { 0.1f,0.25f,0.5f,1.0f };//青っぽい色。RGBAの順
	direct_->GetCommandList()->ClearRenderTargetView(direct_->GetRTVHandle(backBufferIndex), clearcolor, 0, nullptr);


	//コマンドを積む
	direct_->GetCommandList()->RSSetViewports(1, &viewport_);	//Viewportを設定
	direct_->GetCommandList()->RSSetScissorRects(1, &scissorRect_);	//Scirssorを設定
	//RootSignatureを設定。PSOに設定しているけど別途設定が必要
	direct_->GetCommandList()->SetGraphicsRootSignature(rootSignature_);
	direct_->GetCommandList()->SetPipelineState(graphicsPipelineState_);	//PSOを設定
	direct_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);	//VBVを設定


	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけばよい
	direct_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//描画！(DrawCall/ドローコール)。3頂点で1つのインスタンス。
	direct_->GetCommandList()->DrawInstanced(kVertexCountTriangle, 1, static_cast<UINT>(indexVertex_), 0);


	//画面に描く処理はすべて終わり、画面に移すので、状態を遷移
	//今回はRenderTargetからPresentにする
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	//TransitionBarrierを張る
	direct_->GetCommandList()->ResourceBarrier(1, &barrier);
	//コマンドリストの内容を確定させる。全てのコマンドを積んでからCloseすること
	hr_ = direct_->GetCommandList()->Close();
	assert(SUCCEEDED(hr_));


	//GPUにコマンドリストの実行を行わせる
	ID3D12CommandList* commandLists[] = { direct_->GetCommandList() };
	direct_->GetCommandQueue()->ExecuteCommandLists(1, commandLists);
	//GPUとOSに画面の交換を行うよう通知する
	direct_->GetSwapChain()->Present(1, 0);

	//Fenceの値を更新
	fenceValue_++;
	//GPUがここまでたどり着いたときに、Fenceの値を指定した値に代入するようにSignalを送る
	direct_->GetCommandQueue()->Signal(direct_->GetFence(), fenceValue_);

	//Fenceの値が指定したSignal値にたどり着いているか確認する
	//GetCompletedValueの初期値はFence作成時に渡した初期値
	if (direct_->GetFence()->GetCompletedValue() < fenceValue_)
	{
		//指定したSignalにたどり着いていないので、たどり着くまで待つようにイベントを設定する
		direct_->GetFence()->SetEventOnCompletion(fenceValue_, direct_->GetFenceEvent());
		//イベント待つ
		WaitForSingleObject(direct_->GetFenceEvent(), INFINITE);
	}

	//次のフレーム用のコマンドリストを準備
	hr_ = direct_->GetCommandAllocator()->Reset();
	assert(SUCCEEDED(hr_));
	hr_ = direct_->GetCommandList()->Reset(direct_->GetCommandAllocator(), nullptr);
	assert(SUCCEEDED(hr_));

}

void Triangle::HandleClose()
{
	vertexResource_->Release();
	graphicsPipelineState_->Release();
	signatureBlob_->Release();
	if (errorBlob_)
	{
		errorBlob_->Release();
	}
	rootSignature_->Release();
	pixelShaderBlob_->Release();
	vertexShaderBlob_->Release();
}