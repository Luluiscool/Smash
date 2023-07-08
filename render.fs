#version 330



in vec2 fragTexCoord;


uniform sampler2D texture0;
uniform int showBeam;
uniform float Xres;
uniform float Yres;
uniform float rotation;
// uniform sampler2D ppos;

out vec4 finalColor;


void main()
{
    vec3 color = texture(texture0, fragTexCoord).xyz;

    finalColor = vec4(color, 1);
}