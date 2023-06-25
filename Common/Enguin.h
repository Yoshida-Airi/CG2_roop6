#pragma once
#include<Windows.h>
#include<cstdint>
#include <string>
#include<format>
#include<d3d12.h>
#include<dxgi1_6.h>
#include<cassert>
#include<dxgidebug.h>
#include<dxcapi.h>
#include<Vector>

#include"DirectX.h"
#include"WindowAPI.h"
#include"Triangle.h"
#include "ConvertString.h"
#include"Vector4.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dxcompiler.lib")


class Engine
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Engine();
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Engine();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Run();

	/// <summary>
	/// 更新処理の終了
	/// </summary>
	void RunEnd();

	/// <summary>
	/// 三角形描画
	/// </summary>
	void DrawTriangle(const Vector4& a, const Vector4& b, const Vector4& c);

	/// <summary>
	/// 終了(解放処理)
	/// </summary>
	void End();



private:
	DirectX* direct_ = new DirectX;
	WindowAPI winApp_;

	Triangle* triangle_[11];//描画出来る最大個数

	int triangleCount_;	//三角形が描画されている数

	

	HRESULT hr_;				//結果確認用
	IDxcUtils* dxcUtils_ = nullptr;
	IDxcCompiler3* dxcCompiler_ = nullptr;
	IDxcIncludeHandler* includeHandler_ = nullptr;
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature_{};	//RootSignature作成
	ID3D12RootSignature* rootSignature_ = nullptr;	//バイナリを元に生成
	D3D12_INPUT_ELEMENT_DESC inputElementDescs_[1] = {};//InputLayout
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc_{};
	D3D12_BLEND_DESC blendDesc_{};//BlendStateの設定
	D3D12_RASTERIZER_DESC rasterizerDesc_{};//RasiterzerStateの設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc_{};	//PSO
	ID3D12PipelineState* graphicsPipelineState_ = nullptr;	//実際に生成
	D3D12_RESOURCE_DESC vertexResourceDesc_{};	//頂点リソースの設定
	ID3D12Resource* vertexResource_ ;	//実際に頂点リソースを作る
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};//頂点バッファビューを作成する
	D3D12_VIEWPORT viewport_{};	//ビューポート
	D3D12_RECT scissorRect_{};//シザー矩形
	UINT backBufferIndex;
	D3D12_RESOURCE_BARRIER barrier{};	//TransitionBarrierの設定




	ID3DBlob* signatureBlob_ = nullptr;//シリアライズしてバイナリにする
	ID3DBlob* errorBlob_ = nullptr;

	IDxcBlob* vertexShaderBlob_;
	IDxcBlob* pixelShaderBlob_;

	D3D12_CPU_DESCRIPTOR_HANDLE RTVHandle_[2];
	uint64_t fenceValue_;

	//頂点リソースにデータを書き込む
	Vector4* vertexData_;


	//コンパイルシェーダー関数
	IDxcBlob* CompileShader
	(//compilerするshaderファイルへのパス
		const std::wstring& filePath,
		//Compilerに使用するprofile
		const wchar_t* profile,
		//初期化で生成したものを3つ
		IDxcUtils* dxcUtils,
		IDxcCompiler3* dxcCompiler,
		IDxcIncludeHandler* includeHandler
	);

	/// <summary>
	/// DXCの初期化
	/// </summary>
	void IntializeDXC();

	/// <summary>
	/// RootSignatureの生成
	/// </summary>
	void CreateSignature();

	/// <summary>
	/// InputLayoutの生成
	/// </summary>
	void InputLayout();

	/// <summary>
	/// BlendState
	/// </summary>
	void BlendState();

	/// <summary>
	/// ラスタライザステートの設定
	/// </summary>
	void RasterizerState();

	/// <summary>
	/// シェーダーを取り込む
	/// </summary>
	void ShaderCompile();

	/// <summary>
	/// PSOの生成
	/// </summary>
	void CreatePSO();

	/// <summary>
	/// 頂点データ用のリソース
	/// </summary>
	void VertexResource();

	/// <summary>
	/// リソースに書き込む
	/// </summary>
	void Resource(float x1, float y1, float x2, float y2, float x3, float y3);

	/// <summary>
	/// 描画処理
	/// </summary>
	void Render();

	/// <summary>
	/// 画面描画の終わり
	/// </summary>
	void StateChange();

	/// <summary>
	/// 解放処理
	/// </summary>
	void HandleClose();

};
