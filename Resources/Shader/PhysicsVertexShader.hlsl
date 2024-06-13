
// Vertex Shader(VS) �Է�
struct VS_INPUT
{
    float4 mPosition : POSITION;
    float2 mUV : TEXCOORD;
    float3 mNormal : NORMAL0;
};

// Vertex Shader(VS) ���
// SV_POSITION�� SV�� SystemValue�� �����̴�.
// HLSL�󿡼� �̹� ����Ǿ� �ִ� �̸��̰� ������ ���������� �󿡼��� �ǹ̰� �������ִ�.
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

// Normal Distribution Function : ��ĥ�⿡ ���� �ݻ����� �۰��Ѵ�.
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
// Vertex Shader(VS) ���α׷���
VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;

    // ������Ʈ ���� ��ȯ
    float4 pos = input.mPosition;
    
    //pos = mul(pos, World);
    output.mPositionWorld = pos;
    
    // ndc ���� ���� ��ġ
    pos = mul(pos, View);
    pos = mul(pos, Projection);
    output.mPositionProj = pos;
    
    // �� ����, ������Ʈ���� ī�޶� ���� ��� ( ���� )
    float3 lightDir = normalize(Direction);
    float3 viewDir = normalize(output.mPositionWorld.xyz - CameraPos.xyz);
    output.mViewDir = viewDir;
    
    // ������Ʈ ���忡�� �븻 ���� ��� (������Ʈ�� ���鿡 90���� �̷�� ����)
    output.mNormal = normalize(mul(input.mNormal, (float3x3) World));
    
    // ���ݻ�(Diffuse) �������� ���ϱ�
    output.mDiffuse = saturate(dot(-lightDir, output.mNormal));
    
    // �ؽ�ó�� ������ ���� UV�� ����
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

// Pixel Shader(PS) ���α׷���
float4 PS(VS_OUTPUT input) : SV_Target
{
    float3 albedo = baseColor.rgb;
    float roughness = 0.5f;
    float metalness = 0.5f;
    
    //�ؽ�ó
    //if (bIsValidDiffuseMap)
    albedo = texture0.Sample(sampler0, input.mUV).rgb;
    // �����Ͻ�
    //if (bIsValidRoughnessMap)
    roughness = roughness0.Sample(sampler0, input.mUV).r;
    // ��Ż��
    //if (bIsValidMetalnessMap)
    metalness = metalness0.Sample(sampler0, input.mUV).r;
    
    if (roughness <= 0)
        roughness = 0.1f;
    
    // ���� �ݷ���
    albedo.rgb = albedo.rgb * albedo.rgb;
    
    //(���ݻ籤 + ������ + �ֺ���)
    return float4(albedo * (float3)input.mDiffuse, 1.f);
}