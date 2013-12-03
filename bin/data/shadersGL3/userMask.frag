#version 150

// these are our textures
uniform sampler2DRect tex0;
uniform sampler2DRect tex1;
uniform sampler2DRect maskTex;

// this comes from the vertex shader
in vec2 texCoordVarying;

// this is the output of the fragment shader
out vec4 outputColor;

void main()
{
    // get rgb from tex0
    vec3 fg = texture(tex0, texCoordVarying).rgb;
    vec3 bg = texture(tex1, texCoordVarying).rgb;
    // get alpha from mask
    float mask = texture(maskTex, texCoordVarying).r;
    
    //mix the rgb from tex0 with the alpha of the mask
    if ( mask == 1.0 ) outputColor = vec4(fg , 1.0);
    else outputColor = vec4(bg , 1.0);
}