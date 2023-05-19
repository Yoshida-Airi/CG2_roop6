#include<Windows.h>
#include<cstdint>
#include <string>
#include<format>
#include<d3d12.h>
#include<dxgi1_6.h>
#include<cassert>
#include<dxgidebug.h>
#include<dxcapi.h>

#include"DirectX.h"
#include"WindowAPI.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dxcompiler.lib")
class Object
{
public:
	Object();
	~Object();

	void DrawInitialize(ID3D12Device* device, ID3D12GraphicsCommandList* commandList);
	void Run
	(
		ID3D12GraphicsCommandList* GetCommandList,
		IDXGISwapChain4* GetswapChain,
		D3D12_RESOURCE_BARRIER Getbarrier,
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles[2],
		ID3D12Resource* swapChainResources[2],
		);
	void End();

	

private:

	DirectX direct_;
	WindowAPI winApp_;

	HRESULT hr;
	ID3D12Device* device_;
	ID3D12GraphicsCommandList* commandList_;
	IDXGISwapChain4* swapChain_;
	D3D12_RESOURCE_BARRIER barrier_;
	D3D12_CPU_DESCRIPTOR_HANDLE  rtvHandles[2];
	ID3D12Resource*  swapChainResources[2];

	IDxcUtils* dxcUtils = nullptr;
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};	//RootSignature作成
	ID3D12RootSignature* rootSignature = nullptr;	//バイナリを元に生成
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[1] = {};//InputLayout
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	IDxcBlob* vertexShaderBlob;
	IDxcBlob* pixelShaderBlob;
	D3D12_BLEND_DESC blendDesc{};//BlendStateの設定
	D3D12_RASTERIZER_DESC rasterizerDesc{};//RasiterzerStateの設定
	IDxcCompiler3* dxcCompiler = nullptr;
	IDxcIncludeHandler* includeHandler = nullptr;
	D3D12_RESOURCE_DESC vertexResourceDesc{};	//頂点リソースの設定
	ID3D12Resource* vertexResource = nullptr;	//実際に頂点リソースを作る
	D3D12_VIEWPORT viewport{};	//ビューポート
	D3D12_RECT scissorRect{};//シザー矩形
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};	//PSO
	ID3D12PipelineState* graphicsPipelineState = nullptr;	//実際に生成
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};//頂点バッファビューを作成する
	ID3DBlob* signatureBlob = nullptr;//シリアライズしてバイナリにする
	ID3DBlob* errorBlob = nullptr;

	struct Vector4
	{
		float x, y, z, w;
	};


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

	void IntializeDXC();
	void CreateSignature();
	void InputLayout();
	void BlendState();
	void RasterizerState();
	void ShaderCompile();
	void CreatePSO();

	void VertexResource();
	void Resource();
	
	void Render();


	void HandleClose();
};

