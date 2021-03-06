//
// Structure definitions
//

struct VS_OUTPUT {
    vec4 HPosition;
    vec2 HalfAngleTerms;
    vec2 NormalAngleTerms;
    vec2 UV;
    vec4 diffCol;
};


//
// Global variable definitions
//

uniform vec4 CamPos;
uniform vec4 LightPos;
uniform mat4 World;
uniform mat4 mWorldViewProj;

//
// Function declarations
//

VS_OUTPUT vertexMain( in vec3 Position, in vec4 UV, in vec4 Normal );

//
// Function definitions
//

VS_OUTPUT vertexMain( in vec3 Position, in vec4 UV, in vec4 Normal ) {
    vec3 Nn;
    vec4 Po;
    vec3 Pw;
    vec3 Ln;
    float ldn;
    float diffComp;
    VS_OUTPUT OUT;
    vec3 EyePos;
    vec3 Vn;
    vec3 Hn;
    vec2 halfIndices;
    vec2 normIndices;

    Nn = ( -Normal.xyz  );
    Po = vec4( Position.xyz , 1.00000);
    Pw = ( World * Po  ).xyz ;
    Ln = normalize( (vec3( LightPos) - Pw) );
    ldn = dot( Ln, Nn);
    diffComp = max( 0.000000, ldn);
    OUT.diffCol = vec4( 0.0500000, 0.0200000, 0.0100000, 1.00000);
    OUT.UV = UV.xy ;
    EyePos = vec3( CamPos.w );
    Vn = normalize( (EyePos - Pw) );
    Hn = normalize( (Vn + Ln) );
    halfIndices = vec2( (0.500000 + (dot( Ln, Hn) / 2.00000)), (1.00000 - (0.500000 + (dot( Nn, Hn) / 2.00000))));
    normIndices = vec2( (0.500000 + (dot( Ln, Nn) / 2.00000)), (1.00000 - (0.500000 + (dot( Nn, Vn) / 2.00000))));
    OUT.HalfAngleTerms = halfIndices;
    OUT.NormalAngleTerms = normIndices;
    OUT.HPosition = ( mWorldViewProj * Po  );
    return OUT;
}


//
// User varying
//
varying vec4 xlat_varying_TEXCOORD0;
varying vec4 xlat_varying_TEXCOORD1;
varying vec4 xlat_varying_TEXCOORD2;
varying vec4 xlat_varying_COLOR0;

//
// Translator's entry point
//
void main() {
    VS_OUTPUT xlat_retVal;

    xlat_retVal = vertexMain( vec3(gl_Vertex), vec4(gl_MultiTexCoord0), vec4(gl_Normal, 0.0));

    gl_Position = vec4( xlat_retVal.HPosition);
    xlat_varying_TEXCOORD0 = vec4( xlat_retVal.HalfAngleTerms, 0.0, 0.0);
    xlat_varying_TEXCOORD1 = vec4( xlat_retVal.NormalAngleTerms, 0.0, 0.0);
    xlat_varying_TEXCOORD2 = vec4( xlat_retVal.UV, 0.0, 0.0);
    xlat_varying_COLOR0 = vec4( xlat_retVal.diffCol);
}


 