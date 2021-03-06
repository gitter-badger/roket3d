//
// Structure definitions
//

struct VS_OUTPUT {
    vec4 Position;
    vec2 UV;
    vec4 Color;
    vec4 specCol;
};


//
// Global variable definitions
//

uniform vec4 LightPos;
uniform float Timer;
uniform mat4 ViewI;
uniform mat4 World;
uniform mat4 mInvWorld;
uniform mat4 mWorldViewProj;

//
// Function declarations
//

VS_OUTPUT vertexMain( in vec3 Position, in vec4 UV, in vec4 Tangent, in vec4 Binormal, in vec4 Normal );

//
// Function definitions
//

VS_OUTPUT vertexMain( in vec3 Position, in vec4 UV, in vec4 Tangent, in vec4 Binormal, in vec4 Normal ) {
    vec3 Nn;
    float timeNow;
    vec4 Po;
    float iny;
    float wiggleX;
    float wiggleY;
    VS_OUTPUT OUT;
    vec3 Pw;
    vec3 Ln;
    float ldn;
    float diffComp;
    vec3 diffContrib;
    vec3 Vn;
    vec3 Hn;
    float hdn;

    Nn = normalize( ( mInvWorld * Normal  ).xyz  );
    timeNow = (Timer);
    Po = vec4( Position.xyz , 1.00000);
    iny = ((Po.y  * 0.500000) + timeNow);
    wiggleX = (sin( iny ) * 1.500000);
    wiggleY = (cos( iny ) * 1.500000);
    Nn.y  = (Nn.y  + wiggleY);
    Nn = normalize( Nn );
    Po.x  = (Po.x  + wiggleX);
    OUT.Position = ( mWorldViewProj * Po  );
    Pw = ( World * Po  ).xyz ;
    Ln = normalize( (vec3( LightPos) - Pw) );
    ldn = dot( Ln, Nn);
    diffComp = max( 0.000000, ldn);
    diffContrib = (vec3( 0.900000, 0.680000, 0.543000) * ((diffComp * vec3( 1.00000, 1.00000, 1.00000)) + vec3( 0.200000, 0.200000, 0.200000)));
    OUT.Color = vec4( diffContrib, 1.00000);
    OUT.UV = vec2( UV);
    Vn = normalize( (ViewI[ 3 ].xyz  - Pw) );
    Hn = normalize( (Vn + Ln) );
    hdn = pow( max( 0.000000, dot( Hn, Nn)), 5.00000);
    OUT.specCol = vec4( (hdn * vec3( 1.00000, 1.00000, 1.00000)), 1.00000);
    return OUT;
}


//
// Attributes
//
attribute vec4 xlat_attrib_tangent;
attribute vec4 xlat_attrib_binorm;

//
// User varying
//
varying vec4 xlat_varying_TEXCOORD0;
varying vec4 xlat_varying_COLOR0;
varying vec4 xlat_varying_COLOR1;

//
// Translator's entry point
//
void main() {
    VS_OUTPUT xlat_retVal;

    xlat_retVal = vertexMain( vec3(gl_Vertex), vec4(gl_MultiTexCoord0), vec4(xlat_attrib_tangent), vec4(xlat_attrib_binorm), vec4(gl_Normal, 0.0));

    gl_Position = vec4( xlat_retVal.Position);
    xlat_varying_TEXCOORD0 = vec4( xlat_retVal.UV, 0.0, 0.0);
    xlat_varying_COLOR0 = vec4( xlat_retVal.Color);
    xlat_varying_COLOR1 = vec4( xlat_retVal.specCol);
}


 