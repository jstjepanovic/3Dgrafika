#version 330 core

// boje u RGBA
out vec4 FragColor;


// importa boju
in vec3 color;
// importa teks koord
in vec2 texCoord;
// importa normale
in vec3 Normal;
// importa trenutnu poziciju
in vec3 crntPos;

// uzima teksturu
uniform sampler2D tex0;
// uzima boju svijetla
uniform vec4 lightColor;
// uzima poziciju svijetla
uniform vec3 lightPos;
// uzima poziciju kamere
uniform vec3 camPos;

void main()
{
	// ambientalno svijetlo
	float ambient = 0.30f;

	// difuzno svijetlo
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightPos - crntPos);
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	// spekularno svijetlo
	float specularLight = 0.50f;
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 8);
	float specular = specAmount * specularLight;

	// izbaciju konacnu boju
	FragColor = texture(tex0, texCoord) * lightColor * (diffuse + ambient + specular);
}