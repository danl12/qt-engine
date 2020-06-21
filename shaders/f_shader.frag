#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform bool hasTexture;
uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{
    vec4 text = texture2D(material.diffuse, TexCoords).rgba;


    // ambient
    vec3 ambient;

    if (hasTexture) {
        if(text.a < 0.5)
        discard;

        ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
    } else {
        ambient = light.ambient * vec3(15, 15, 51);
    }

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);

    vec4 diffuse = vec4(light.diffuse, 1.0) * diff * text;

    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;

    // spotlight (soft edges)
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = (light.cutOff - light.outerCutOff);
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    diffuse  *= intensity;
    specular *= intensity;

    // attenuation
    float distance    = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    ambient  *= attenuation;
//    diffuse   *= 2;
    specular *= attenuation;

    vec4 result = vec4(ambient, 1.0) + diffuse + vec4(specular, 1.0);
    FragColor = result;
}
