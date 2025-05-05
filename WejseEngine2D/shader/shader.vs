#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;
out vec3 ourColor;
out vec3 FragPos;


uniform vec3 uColor;
uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main()
{
	vec4 result = model * vec4(aPos, 1.0);
	FragPos = vec3(result.x,result.y,result.z);  
	gl_Position = projection * view * model * vec4(aPos, 1.0);;
	ourColor = uColor;
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}