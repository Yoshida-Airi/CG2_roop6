#pragma once
#include<d3d12.h>
#include<assert.h>

ID3D12Resource* CreateBufferResource(ID3D12Device* device, size_t sizeInBytes);
