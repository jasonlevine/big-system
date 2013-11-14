#version 120

// this is how we receive the texture
uniform sampler2DRect tex0;
uniform sampler2DRect tex1;

varying vec2 texCoordVarying;

void main()
{
    vec4 texColor = texture2DRect(tex0, texCoordVarying);
    vec4 maskColor = texture2DRect(tex1, texCoordVarying);
    vec4 color;
    
    color.r = texColor.r;
    color.g = texColor.g;
    color.b = texColor.b;
    color.a = maskColor.r;
    
    gl_FragColor = color;
//    gl_FragColor.a = texture2DRect(tex1., texCoordVarying);
}