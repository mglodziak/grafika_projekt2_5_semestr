#version 330 

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 incol;
layout (location = 2) in vec3 offset;
layout (location = 3) in vec2 vertexUV;


uniform mat4 MVP;

//out vec3 outcol;
out vec3 pos;
out vec2 UV;



void main()
{
	pos=aPos;
    //outcol=incol;
    gl_Position = MVP*vec4(aPos.x, aPos.y, aPos.z, 1.0);
	UV = vertexUV;
}
