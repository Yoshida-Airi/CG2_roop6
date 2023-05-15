#pragma once

#include<Windows.h>

#include <string>
#include<dxcapi.h>

#include "ConvertString.h"
#include"DirectX.h"
#include"Window.h"


#pragma comment(lib,"dxcompiler.lib")



class Polygon
{
public:
	/*=====================================*/
	/* 　　　　   パブリックメソッド　　　 　    */
	/*=====================================*/

	//コンストラクタ
	Polygon();
	//デストラクタ
	~Polygon();


	void Start();
	void Draw();
	void End();

	//ベクトル
	struct Vector4
	{
		float x, y, z, w;
	}; 

private:
	/*=====================================*/
	/* 　　　　   プライベート変数    　       */
	/*=====================================*/

	//ビューポート
	D3D12_VIEWPORT viewport{};
	//シザー矩形
	D3D12_RECT scissorRect{};
	//ルートシグネチャ
	//バイナリを元に生成
	ID3D12RootSignature* rootSignature ;
	//PSOの生成
	ID3D12PipelineState* graphicsPipelineState;
	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	//頂点リソースの設定
	D3D12_RESOURCE_DESC vertexResourceDesc{};
	//実際に頂点リソースを作る
	ID3D12Resource* vertexResource;
	//シリアライズしてバイナリにする
	ID3DBlob* signatureBlob ;
	ID3DBlob* errorBlob ;
	IDxcBlob* pixelShaderBlob;
	IDxcBlob* vertexShaderBlob;

	//ダイレクトX
	DirectX direct_;
	HRESULT hr;

	Window window_;



	/*=====================================*/
	/* 　　　　   プライベートメソッド　　　     */
	/*=====================================*/

	//シェーダーコンパイル用の関数
	IDxcBlob* CompileShader
	(
		//compilerするshaderファイルへのパス
		const std::wstring& filePath,
		//compilerに使用するProfile
		const wchar_t* profile,
		//初期化で生成したものを3つ
		IDxcUtils* dxcUtils,
		IDxcCompiler3* dxcCompiler,
		IDxcIncludeHandler* includeHandler
	);

	bool Initialize();
	void PolygonDraw();
	void PolygonEnd();
};

