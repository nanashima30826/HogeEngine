
#include "common.hlsl"

Texture2D g_Texture : register(t0);
Texture2D g_TextureEnv : register(t1);
SamplerState g_SamplerState : register(s0);
static const float PI = 3.141592;

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    float3 eyev = normalize(In.WorldPosition.xyz - CameraPosition.xyz);
    float3 normal = normalize(In.Normal.xyz);

    // 反射ベクトル計算
    float3 refColor;
    {
        float3 refv = normalize(reflect(eyev, normal));
        float2 envTexCoord;
        envTexCoord.x = (atan2(refv.x, refv.z) / (PI * 2)) + 0.5;
        envTexCoord.y = acos(refv.y) / PI;
        refColor = g_TextureEnv.Sample(g_SamplerState, envTexCoord);
    }

    //// 屈折ベクトル計算
    //float3 refrColor;
    //{
    //    float3 refrv = normalize(refract(eyev, normal, 1.0f / 1.5f));
    //    float2 envTexCoord;
    //    envTexCoord.x = (atan2(refrv.x, refrv.z) / (PI * 2)) + 0.5;
    //    envTexCoord.y = acos(refrv.y) / PI;
    //    refrColor = g_TextureEnv.Sample(g_SamplerState, envTexCoord);
    //}

    // フレネル効果計算
    float f0 = 0.04;
    float d = saturate(dot(-eyev, normal));
    float fresnel = f0 + (1.0 - f0) * pow(1.0 - d, 2.0);

    // 出力色
  //  outDiffuse.rgb = lerp(refrColor, refColor, fresnel);
    outDiffuse.rgb = refColor;
    //outDiffuse.a = 1.0f;
    outDiffuse.a = fresnel;
}
/*
#include "common.hlsl"


Texture2D		g_Texture : register(t0);
Texture2D g_TextureEnv : register(t1);
SamplerState	g_SamplerState : register(s0);
static  float PI = 3.141592;

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;
    eyev = normalize(eyev);
    
    float3 normal = normalize(In.Normal.xyz);
    
    
    //スフィアマップ
    ////float2 envTexCoord;
    ////envTexCoord.x = refv.x * 0.3 + 0.5;
    ////envTexCoord.y = -refv.y * 0.3 + 0.5;
    float3 refColor;
    {
        //反射
    float3 refv = reflect(eyev.xyz, normal);
    refv = normalize(refv);
    //ぱのらま
        float2 envTexCoord;
        envTexCoord.x = atan2(refv.x, refv.z) / (PI * 2);
        envTexCoord.y = acos(refv.y) / PI;
    
       // outDiffuse.rgb = g_TextureEnv.Sample(g_SamplerState, envTexCoord) * 0.1;
    
    
       // outDiffuse.a = 1.0;
        refColor = g_TextureEnv.Sample(g_SamplerState, envTexCoord);
    }
    float3 refrColor;
    {
    
    //屈折
        float3 refrv = refract(eyev.xyz, normal, 1.0f / 1.5f);
        refrv = normalize(refrv);

    //ぱのらま
        float2 envTexCoord;
        envTexCoord.x = atan2(refrv.x, refrv.z) / (PI * 2);
        envTexCoord.y = acos(refrv.y) / PI;
    
       // outDiffuse.rgb = g_TextureEnv.Sample(g_SamplerState, envTexCoord) * 0.9;
       // outDiffuse.a = 1.0;
        refrColor = g_TextureEnv.Sample(g_SamplerState, envTexCoord);
    }
    float f0 = 0.04;
    float d = saturate(dot(-eyev, normal));
    float fresnel = f0 + (1.0 - f0) * ((1.0 - d), 2.0);
    
    outDiffuse.rgb = lerp(refrColor, refColor, fresnel);
    outDiffuse.a = 1.0f;
}
*/