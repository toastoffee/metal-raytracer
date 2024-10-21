// #include <metal_stdlib>
// using namespace metal;

// struct v2f
// {
//     float4 position [[position]];
//     float2 texcoord;
// };

// struct VertexData
// {
//     float3 position;
//     float2 texcoord;
// };


// v2f vertex vertexMain( device const VertexData* vertexData [[buffer(0)]],
//                        uint vertexId [[vertex_id]] )
// {
//     v2f o;

//     const device VertexData& vd = vertexData[ vertexId ];

//     float4 pos = float4( vd.position, 1.0 );
    
//     o.position = pos;
//     o.texcoord = vd.texcoord.xy;

//     return o;
// }

// half4 fragment fragmentMain( v2f in [[stage_in]], texture2d< half, access::sample > tex [[texture(0)]] )
// {
//     constexpr sampler s( address::repeat, filter::linear );
//     half3 texel = tex.sample( s, in.texcoord ).rgb;

//     // return half4( texel, 1.0 );
//     return half4(1.0, 0.0, 0.0, 1.0);
// }


#include <metal_stdlib>
using namespace metal;

struct v2f
{
    float4 position [[position]];
    half3 color;
};

struct VertexData
{
    float3 position;
    float2 texcoord;
};

v2f vertex vertexMain( device const VertexData* vertexData [[buffer(0)]],
                        uint vertexId [[vertex_id]])
{
    const device VertexData& vd = vertexData[ vertexId ];

    v2f o;
    o.position = float4( vd.position, 1.0 );
    o.color = half3 (1.0 ,0.0, 0.0);
    return o;
}

half4 fragment fragmentMain( v2f in [[stage_in]] )
{
    return half4( in.color, 1.0 );
}