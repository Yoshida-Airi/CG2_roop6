#pragma once
#include<d3d12.h>
#include<assert.h>

ID3D12Resource* CreateBufferResource(ID3D12Device* device, size_t sizeInBytes)
{
	//リソース用のヒープの設定
	D3D12_HEAP_PROPERTIES uploadHeapProperties{};
	uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;	//UplodeHeapを使う

	// 頂点リソースの設定
	D3D12_RESOURCE_DESC vertexResourceDesc{};

	//バッファリソース。テクスチャの場合は別の設定する
	vertexResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	vertexResourceDesc.Width = sizeInBytes;	//リソースのサイズ。今回はVector4を3頂点分
	//バッファの場合はこれらは1にする決まり
	vertexResourceDesc.Height = 1;
	vertexResourceDesc.DepthOrArraySize = 1;
	vertexResourceDesc.MipLevels = 1;
	vertexResourceDesc.SampleDesc.Count = 1;
	//バッファの場合はこれにする決まり
	vertexResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	// 実際に頂点リソースを作る
	ID3D12Resource* vertexResource = nullptr;
	HRESULT hr_ = device->CreateCommittedResource(&uploadHeapProperties, D3D12_HEAP_FLAG_NONE, &vertexResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertexResource));
	assert(SUCCEEDED(hr_));
	return vertexResource;
}
