
// Vertex Shader(VS) 입력
struct VS_INPUT
{
    float4 mPosition : POSITION;
    float2 mUV : TEXCOORD;
    float3 mNormal : NORMAL0;
};

// Vertex Shader(VS) 출력
// SV_POSITION의 SV는 SystemValue의 약자이다.
// HLSL상에서 이미 예약되어 있는 이름이고 렌더링 파이프라인 상에서의 의미가 정해져있다.
struct VS_OUTPUT
{
    float4 mPositionProj : SV_POSITION;
    float4 mPositionWorld : POSITION;
    float2 mUV : TEXCOORD1;
    float3 mViewDir : TEXCOORD2;
    float4 mDiffuse : COLOR;
    float3 mNormal : NORMAL0;
};

cbuffer CB_ModelTransform : register(b0)
{
    Matrix World;
};

cbuffer CB_Camera : register(b1)
{
    Matrix View;
    Matrix Projection;
    float3 CameraPos;
};

cbuffer CB_DirectionLight : register(b2)
{
    float3 Direction;
    float3 DirectionColor;
};

cbuffer CB_Material : register(b3)
{
    float3 baseColor;
    float3 emissiveColor;
    float OpacityValue;
    float EmissivePower;
    int bIsTexture;
};

cbuffer CB_MatrixPalette : register(b4)
{
    Matrix MatrixPalleteArray[128];
};

cbuffer CB_MatrixPalette : register(b5)
{
    int bIsValidDiffuseMap;
    int bIsValidNormalMap;
    int bIsValidSpecularMap;
    int bIsValidOpcityMap;
    int bIsValidEmissiveMap;
    int bIsValidMetalnessMap;
    int bIsValidRoughnessMap;
};

// Normal Distribution Function : 거칠기에 따라 반사율을 작게한다.
float ndfGGX(float cosLh, float roughness)
{
    float alpha = roughness * roughness;
    float alphasq = alpha * alpha;
    
    float denominator = 3.141592f * pow((cosLh * cosLh) * (alpha - 1.f) + 1.f, 2);
    return alpha / denominator;
}

float3 FresnelReflection(float costheta, float3 F0)
{
    return F0 + (1.f - F0) * pow((1.f - costheta), 5);
}

// 
float gaSchlickG1(float cosTheta, float k)
{
    float denominator = cosTheta * (1.f - k) + k;
    
    return cosTheta / denominator;
}

// 
float gaSchlickGGX(float cosLi, float cosLo, float roughness)
{
    float r = roughness + 1.0;
    float k = (r * r) / 8.0; // Epic suggests using this roughness remapping for analytic lights.
    return gaSchlickG1(cosLi, k) * gaSchlickG1(cosLo, k);
}

// IA - VS - RS - PS - OM
// Vertex Shader(VS) 프로그래밍
VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;

    // 오브젝트 월드 변환
    float4 pos = input.mPosition;
    
    //pos = mul(pos, World);
    output.mPositionWorld = pos;
    
    // ndc 공간 내의 위치
    pos = mul(pos, View);
    pos = mul(pos, Projection);
    output.mPositionProj = pos;
    
    // 빛 방향, 오브젝트에서 카메라 방향 계산 ( 월드 )
    float3 lightDir = normalize(Direction);
    float3 viewDir = normalize(output.mPositionWorld.xyz - CameraPos.xyz);
    output.mViewDir = viewDir;
    
    // 오브젝트 월드에서 노말 벡터 계산 (오브젝트의 정면에 90도를 이루는 벡터)
    output.mNormal = normalize(mul(input.mNormal, (float3x3) World));
    
    // 난반사(Diffuse) 내적으로 구하기
    output.mDiffuse = saturate(dot(-lightDir, output.mNormal));
    
    // 텍스처를 입히기 위한 UV값 전달
    output.mUV = input.mUV;
    
    return output;
}

// Texture, Sampler 
Texture2D texture0 : register(t0);
Texture2D normal0 : register(t1);
Texture2D specular0 : register(t2);
Texture2D opcity0 : register(t3);
Texture2D emissive0 : register(t4);
Texture2D metalness0 : register(t5);
Texture2D roughness0 : register(t6);

SamplerState sampler0 : register(s0);
SamplerState BRDFsampler0 : register(s1);

// Pixel Shader(PS) 프로그래밍
float4 PS(VS_OUTPUT input) : SV_Target
{
    float3 albedo = baseColor.rgb;
    float roughness = 0.5f;
    float metalness = 0.5f;
    
    //텍스처
    //if (bIsValidDiffuseMap)
    albedo = texture0.Sample(sampler0, input.mUV).rgb;
    // 러프니스
    //if (bIsValidRoughnessMap)
    roughness = roughness0.Sample(sampler0, input.mUV).r;
    // 메탈릭
    //if (bIsValidMetalnessMap)
    metalness = metalness0.Sample(sampler0, input.mUV).r;
    
    if (roughness <= 0)
        roughness = 0.1f;
    
    // 감마 콜렉션
    albedo.rgb = albedo.rgb * albedo.rgb;
    
    //(난반사광 + 직접광 + 주변광)
    return float4(albedo * (float3)input.mDiffuse, 1.f);
}