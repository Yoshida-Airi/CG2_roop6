#include "Triangle.h"

void Triangle::Initialize(DirectX* direct)
{
	direct_ = direct;
	SetVertex();
	SetMaterial();
}

void Triangle::Draw(const Vector4& a, const Vector4& b, const Vector4& c) {
	//左下
	vertexData_[0] = a;
	//上
	vertexData_[1] = b;
	//右下
	vertexData_[2] = c;

	//VBVを設定
	direct_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);
	//形状を設定。PS0にせっていしているものとはまた別。同じものを設定する
	direct_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//マテリアルCBufferの場所を指定
	direct_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materilResource_->GetGPUVirtualAddress());
	//描画
	direct_->GetCommandList()->DrawInstanced(3, 1, 0, 0);

}

void Triangle::End()
{
	vertexResource_->Release();
	materilResource_->Release();
}

void Triangle::SetVertex()
{
	//頂点リソースの設定
	vertexResource_ = CreateBufferResource(direct_->GetDevice(), sizeof(Vector4) * 3);

	//リソースの先頭のアドレスから使う
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点3つ分のサイズ
	vertexBufferView_.SizeInBytes = sizeof(Vector4) * 3;
	//1頂点当たりのサイズ
	vertexBufferView_.StrideInBytes = sizeof(Vector4);
	//書き込むためのアドレスを取得
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));

}

void Triangle::SetMaterial()
{
	//マテリアル用のリソースを作る。今回はcolor1つ分のサイズを用意する
	materilResource_ = CreateBufferResource(direct_->GetDevice(), sizeof(Vector4));
	//マテリアルにデータを書き込む
	Vector4* materialData = nullptr;
	//書き込むためのアドレスを取得
	materilResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	//今回は赤を書き込んでみる
	*materialData = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
}