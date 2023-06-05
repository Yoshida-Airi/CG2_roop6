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
#include "ConvertString.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dxcompiler.lib")

struct Vector4
{
	float x, y, z, w;
};

struct Vector3
{
	float x;
	float y;
	float z;
};


class Triangle
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Triangle();
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Triangle();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="device"></param>
	/// <param name="commandList"></param>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="direct"></param>
	void Run();

	/// <summary>
	/// 終了(解放処理)
	/// </summary>
	void End();



private:
	DirectX* direct_ = new DirectX;
	WindowAPI winApp_;
	

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
	ID3D12Resource* vertexResource_ = nullptr;	//実際に頂点リソースを作る
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};//頂点バッファビューを作成する
	D3D12_VIEWPORT viewport_{};	//ビューポート
	D3D12_RECT scissorRect_{};//シザー矩形
	UINT backBufferIndex;


	ID3DBlob* signatureBlob_ = nullptr;//シリアライズしてバイナリにする
	ID3DBlob* errorBlob_ = nullptr;
	
	IDxcBlob* vertexShaderBlob_;
	IDxcBlob* pixelShaderBlob_;

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
	void Resource();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Render();

	/// <summary>
	/// 解放処理
	/// </summary>
	void HandleClose();

};

