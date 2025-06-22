
#include "common.hlsl"

StructuredBuffer<float3> Position : register(t2);

void main(in VS_IN In, out PS_IN Out)
{

	//matrix wvp;
	//wvp = mul(World, View);
	//wvp = mul(wvp, Projection);
    Out.Position = mul(In.Position, World);
    //In.Position.x += In.Instanceid* 2.0;
    In.Position.xyz += Position[In.Instanceid];
	Out.Position = mul(Out.Position, View);
    Out.Position = mul(Out.Position, Projection);
	
	Out.TexCoord = In.TexCoord;
	Out.Diffuse = In.Diffuse * Material.Diffuse;

}

