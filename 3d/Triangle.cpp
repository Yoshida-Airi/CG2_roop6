#include "Triangle.h"

//******************************************//
//			　パブリックメソッド		　		//
//******************************************//

void Triangle::Initialize(DirectX* direct)
{
	direct_ = direct;
	SetVertex();
	SetMaterial();
	SetWvp();
}

void Triangle::Draw(const Vector4& a, const Vector4& b, const Vector4& c, const Vector4& color, const Matrix4x4& matrix)
{
	//左下
	vertexData_[0] = a;
	//上
	vertexData_[1] = b;
	//右下
	vertexData_[2] = c;

	//マテリアル(色)代入
	materialData_[0] = color;

	*wvpData_ = matrix;

	//VBVを設定
	direct_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);
	//形状を設定。PS0にせっていしているものとはまた別。同じものを設定する
	direct_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//マテリアルCBufferの場所を指定
	direct_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materilResource_->GetGPUVirtualAddress());
	//wvp用のCBufferの場所を設定
	direct_->GetCommandList()->SetGraphicsRootConstantBufferView(1, wvpResource_->GetGPUVirtualAddress());
	//描画
	direct_->GetCommandList()->DrawInstanced(3, 1, 0, 0);

}

//******************************************//
//			プライベートメソッド				//
//******************************************//

void Triangle::End()
{
	vertexResource_->Release();
	materilResource_->Release();
	wvpResource_->Release();
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
	//書き込むためのアドレスを取得
	materilResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
}

void Triangle::SetWvp()
{
	//WVPようのリソースを作る。Matrix4x4 1つ分のサイズを用意する
	wvpResource_ = CreateBufferResource(direct_->GetDevice(), sizeof(Matrix4x4));
	//書き込むためのアドレスを取得
	wvpResource_->Map(0, nullptr, reinterpret_cast<void**>(&wvpData_));
	//単位行列を書き込んでいく
	*wvpData_ = MakeIdentity4x4();


}