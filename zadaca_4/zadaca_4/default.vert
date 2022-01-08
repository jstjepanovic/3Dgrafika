#version 330 core

// pozicije
layout (location = 0) in vec3 aPos;
// boje
layout (location = 1) in vec3 aColor;
// teksturne koord
layout (location = 2) in vec2 aTex;
// normale
layout (location = 3) in vec3 aNormal;


// izbacuje boju za frag shader
out vec3 color;
// izbacuje tekst koord za frag shader
out vec2 texCoord;
// izbacuje normalu za frag shader
out vec3 Normal;
// izbacuje poziciju za frag shader
out vec3 crntPos;

// uzima matricu kamere
uniform mat4 camMatrix;
// uzima matricu modela 
uniform mat4 model;


void main()
{
	// racuna trenutnu poziciju
	crntPos = vec3(model * vec4(aPos, 1.0f));
	// izbacuje pozicije svih vrhova
	gl_Position = camMatrix * vec4(crntPos, 1.0);

	// postavlja boju
	color = aColor;
	// postavlja tekst koord
	texCoord = aTex;
	// postavlja normale
	Normal = aNormal;
}