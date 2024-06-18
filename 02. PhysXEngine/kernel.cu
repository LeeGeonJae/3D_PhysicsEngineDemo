//#include "type.h"
//
//#include <stdio.h>
//#include <cuda_runtime.h>
//#include <device_launch_parameters.h>
//#include <directxtk\SimpleMath.h>
//#include <xlocale>
//
//__device__ float2 Sub(const float2& lhs, const float2& rhs)
//{
//    float2 result;
//    result.x = lhs.x - rhs.x;
//    result.y = lhs.y - rhs.y;
//    return result;
//}
//
//__device__ float3 Sub(const float3& lhs, const float3& rhs)
//{
//    float3 result;
//    result.x = lhs.x - rhs.x;
//    result.y = lhs.y - rhs.y;
//    result.z = lhs.z - rhs.z;
//    return result;
//}
//
//__device__ float3 cross(const float3& a, const float3& b) {
//    return {
//        a.y * b.z - a.z * b.y,
//        a.z * b.x - a.x * b.z,
//        a.x * b.y - a.y * b.x
//    };
//}
//
//__device__ float DotProduct(const float3& a, const float3& b) {
//    return a.x * b.x + a.y * b.y + a.z * b.z;
//}
//
//__device__ void NormalizeVector(float3& vectors)
//{
//    float length = vectors.x + vectors.y + vectors.z;
//    if (length > 0) {
//        vectors.x /= length;
//        vectors.y /= length;
//        vectors.z /= length;
//    }
//}
//
//// CUDA 커널 함수 정의
//__global__ void CalculateNormals(
//    float3* vertices,
//    float2* uvs,
//    unsigned int vertexSize,
//    unsigned int* indices,
//    unsigned int indexSize,
//    PhysicsEngine::PhysicsVertex* buffer)
//{
//    int idx = blockIdx.x * blockDim.x + threadIdx.x;
//    if (idx >= indexSize / 3) return;
//
//    unsigned int i0 = indices[idx * 3];
//    unsigned int i1 = indices[idx * 3 + 1];
//    unsigned int i2 = indices[idx * 3 + 2];
//
//    float3 v0 = vertices[i0];
//    float3 v1 = vertices[i1];
//    float3 v2 = vertices[i2];
//
//    float2 uv0 = uvs[i0];
//    float2 uv1 = uvs[i1];
//    float2 uv2 = uvs[i2];
//
//    float3 edge1 = Sub(v1, v0);
//    float3 edge2 = Sub(v2, v0);
//
//    float2 deltaUV1 = Sub(uv1, uv0);
//    float2 deltaUV2 = Sub(uv2, uv0);
//
//    float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
//
//    float3 tangent;
//    tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
//    tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
//    tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
//    NormalizeVector(tangent);
//
//    float3 bitangent;
//    bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
//    bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
//    bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
//    NormalizeVector(bitangent);
//
//    float3 normal = cross(edge1, edge2);
//    NormalizeVector(normal);
//
//    buffer[i0].normal.x += normal.x;
//    buffer[i0].normal.y += normal.y;
//    buffer[i0].normal.z += normal.z;
//
//    buffer[i1].normal.x += normal.x;
//    buffer[i1].normal.y += normal.y;
//    buffer[i1].normal.z += normal.z;
//
//    buffer[i2].normal.x += normal.x;
//    buffer[i2].normal.y += normal.y;
//    buffer[i2].normal.z += normal.z;
//
//    buffer[i0].tangent.x += tangent.x;
//    buffer[i0].tangent.y += tangent.y;
//    buffer[i0].tangent.z += tangent.z;
//
//    buffer[i1].tangent.x += tangent.x;
//    buffer[i1].tangent.y += tangent.y;
//    buffer[i1].tangent.z += tangent.z;
//
//    buffer[i2].tangent.x += tangent.x;
//    buffer[i2].tangent.y += tangent.y;
//    buffer[i2].tangent.z += tangent.z;
//
//    buffer[i0].biTangent.x += bitangent.x;
//    buffer[i0].biTangent.y += bitangent.y;
//    buffer[i0].biTangent.z += bitangent.z;
//
//    buffer[i1].biTangent.x += bitangent.x;
//    buffer[i1].biTangent.y += bitangent.y;
//    buffer[i1].biTangent.z += bitangent.z;
//
//    buffer[i2].biTangent.x += bitangent.x;
//    buffer[i2].biTangent.y += bitangent.y;
//    buffer[i2].biTangent.z += bitangent.z;
//}
//
//using namespace PhysicsEngine;
//
//// 커널 호출을 위한 C++ 함수 정의
//extern "C" void launchCalculateNormals(
//    float3 * vertices,
//    float2 * uvs,
//    unsigned int vertexSize,
//    unsigned int* indices,
//    unsigned int indexSize,
//    PhysicsEngine::PhysicsVertex * buffer)
//{
//    int threadsPerBlock = 256;
//    int blocksPerGrid = (indexSize / 3 + threadsPerBlock - 1) / threadsPerBlock;
//
//    CalculateNormals<<<blocksPerGrid, threadsPerBlock >>>(
//        vertices, uvs, vertexSize, indices, indexSize, buffer);
//
//    cudaDeviceSynchronize();
//}