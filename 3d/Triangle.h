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

	void Draw(const Vector4& a, const Vector4& b, const Vector4& c);

	void End();


private:
	MyEngine* engine_;
	DirectX* direct_;

	Vector4* vertexData_;
	ID3D12Resource* vertexResource_;	//頂点
	ID3D12Resource* materilResource_;	//マテリアル
	ID3D12Resource* wvpResource_;		//トランスフォーメーションマトリックス

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;

	Transform transform_;
	Matrix4x4* wvpData_;

private:
	void SetVertex();
	void SetMaterial();
	void SetWvp();

};
