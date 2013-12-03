#version 120

uniform sampler2DRect tex0;
uniform sampler2DRect tex1;
uniform sampler2DRect maskTex;

varying vec2 texCoordVarying;

void main()
{
    // Get color value from
    vec3 fg = texture2DRect(tex0, texCoordVarying).rgb;
    vec3 bg = texture2DRect(tex1, texCoordVarying).rgb;
    // Get alpha value
    float mask = texture2DRect(maskTex, texCoordVarying).r;

    // Set
    if ( mask == 1.0 ) gl_FragColor = vec4(fg , 1.0);
    else gl_FragColor = vec4(bg , 1.0);
}