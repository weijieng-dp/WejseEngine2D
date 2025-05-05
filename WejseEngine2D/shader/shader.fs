#version 330 core
layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec4 PickingColor;

#define NUM_OF_LIGHT 8
  
in vec3 ourColor;
in vec2 TexCoord;
in vec3 FragPos;

uniform vec2 lightPosition[NUM_OF_LIGHT]; // Light position in screen space
uniform float lightIntensity[NUM_OF_LIGHT];
uniform float ambientStrength;

uniform vec3 encodedID ;

uniform bool hasTexture;
uniform sampler2D ourTexture;


// Light attenuation coefficients

void main()
{
    vec4 ambient = vec4(ambientStrength);
    vec4 result = vec4(0,0,0,0);
    for(int i =0; i < NUM_OF_LIGHT; i++)
    {
        
        float distance = length(lightPosition[i] - vec2(FragPos.x, FragPos.y)); 
        vec3 lAtt = vec3(0.0001, 0.0001, 0.001);
        // Improved attenuation formula
        float attenuation = 1/(lAtt.x + lAtt.y * distance * lAtt.z*distance *distance);
        result += lightIntensity[i] * vec4(attenuation);
    }
    if (hasTexture) {
        vec4 color = texture(ourTexture, TexCoord);
        if(color.a < .1)
            discard;
        FragColor = color * vec4(ourColor, 1.0) * (result+ambient);
    } else {
    float value = 0;
    for(int i =0; i < NUM_OF_LIGHT; i++)
    {
        value += lightIntensity[i];
    }
    if(value <= 0)
    {
        ambient = vec4(.7,.7,.7,1.0);
    }
       FragColor = vec4(ourColor,1.0 ) * (result+ambient) *vec4(.7,.7,.7,1.0);
    }
    PickingColor = vec4(encodedID,1.0);
}