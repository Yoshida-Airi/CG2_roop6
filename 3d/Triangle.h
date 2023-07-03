#pragma once
#include"DirectX.h"
#include"Vector4.h"
#include"MathUtility.h"
#include"CreateResource.h"

class MyEngine;

class Triangle
{

public:
	void Initialize(DirectX* direct);

	void Draw(const Vector4& a, const Vector4& b, const Vector4& c, const Vector4& color);

	void End();


private:
	MyEngine* engine_;
	DirectX* direct_;
	

	Vector4* vertexData_;	//頂点データ
	ID3D12Resource* vertexResource_;	//頂点
	ID3D12Resource* materilResource_;	//マテリアル
	ID3D12Resource* wvpResource_;		//トランスフォーメーションマトリックス

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;

	Vector4* materialData_;	//マテリアルにデータを書き込む
	Matrix4x4* wvpData_;	//オブジェクトの位置をいじるためのデータ
	Matrix4x4 worldMatrix_;

	Transform transform_;	


private:

	/// <summary>
	/// 頂点データの設定
	/// </summary>
	void SetVertex();

	/// <summary>
	/// マテリアル(色)の設定
	/// </summary>
	void SetMaterial();

	/// <summary>
	/// トランスフォームの設定
	/// </summary>
	void SetWvp();



};