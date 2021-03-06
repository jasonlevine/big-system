#version 150

in vec2 texCoordVarying;

uniform sampler2DRect tex1;
uniform sampler2DRect tex2;
uniform sampler2DRect tex3;

out vec4 outputColor;
 
void main()
{
    vec4 imageColor = texture(tex1, texCoordVarying);
    vec4 projColor = texture(tex2, texCoordVarying);
    vec4 maskColor = texture(tex3, texCoordVarying);
    
    vec4 color;
    color.r = (imageColor.r * projColor.r) + projColor.r / 2;
    color.g = (imageColor.g * projColor.g)+ projColor.g / 2;
    color.b = (imageColor.b * projColor.b)+ projColor.b / 2;
    color.a = 1.0 - maskColor.a;
    

    
    
    outputColor = color;
}