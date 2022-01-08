#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb/stb_image.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include"Texture.h"
#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include"Camera.h"

const unsigned int width = 1920;
const unsigned int height = 1080;

// prva piramida
GLfloat vertices[] =
{ //  koordinate				boja			     tekst koord          normale       //
	-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,      0.0f, -1.0f, 0.0f, 
	-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 5.0f,      0.0f, -1.0f, 0.0f, 
	 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 5.0f,      0.0f, -1.0f, 0.0f, 
	 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.0f, -1.0f, 0.0f, 

	-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,     -0.8f, 0.5f,  0.0f, 
	-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,     -0.8f, 0.5f,  0.0f, 
	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,     -0.8f, 0.5f,  0.0f,

	-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.0f, 0.5f, -0.8f, 
	 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 0.0f,      0.0f, 0.5f, -0.8f, 
	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,      0.0f, 0.5f, -0.8f, 

	 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 0.0f,      0.8f, 0.5f,  0.0f, 
	 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.8f, 0.5f,  0.0f, 
	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,      0.8f, 0.5f,  0.0f, 

	 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.0f, 0.5f,  0.8f, 
	-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,      0.0f, 0.5f,  0.8f, 
	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,      0.0f, 0.5f,  0.8f  
};

// indexi
GLuint indices[] =
{
	0, 1, 2, 
	0, 2, 3, 
	4, 6, 5, 
	7, 9, 8, 
	10, 12, 11, 
	13, 15, 14 
};

GLfloat lvertices[] =
{ //    koordinate           boja               teksturne koord //
	0.7f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,		0.0f, 0.0f,
	0.7f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,		5.0f, 0.0f,
	1.7f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,		0.0f, 0.0f,
	1.7f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,		5.0f, 0.0f,
	1.2f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,		2.5f, 5.0f
};


GLuint lindices[] =
{
	0, 1, 2,
	0, 2, 3,
	0, 1, 4,
	1, 2, 4,
	2, 3, 4,
	3, 0, 4
};

GLfloat c1vertices[] =
{ 
	 -1.0f,  1.0f,   0.5f,     0.0f, 0.70f, 0.44f,  0.0f, 0.0f,
	 -1.0f,  1.0f,  -0.5f,     0.0f, 0.70f, 0.44f,  0.0f, 1.0f,
	  0.0f,  1.0f,  -0.5f,     0.0f, 0.70f, 0.44f,  1.0f, 1.0f,  
	  0.0f,  1.0f,   0.5f,     0.0f, 0.70f, 0.44f,  1.0f, 0.0f, 
	 -1.0f,  2.0f,   0.5f,     0.0f, 0.70f, 0.44f,  1.0f, 0.0f, 
	 -1.0f,  2.0f,  -0.5f,     0.0f, 0.70f, 0.44f,  1.0f, 2.0f,  
	  0.0f,  2.0f,  -0.5f,     0.0f, 0.70f, 0.44f,  2.0f, 2.0f,  
	  0.0f,  2.0f,   0.5f,     0.0f, 0.70f, 0.44f,  2.0f, 0.0f,  
};

GLuint c1indices[] =
{
  0, 2, 3, 0, 3, 1,
  2, 6, 7, 2, 7, 3,
  6, 4, 5, 6, 5, 7,
  4, 0, 1, 4, 1, 5,
  0, 4, 6, 0, 6, 2,
  1, 5, 7, 1, 7, 3,
};


GLfloat c2vertices[] =
{ 
	 0.7f,  1.0f,  0.5f,
	 0.7f,  1.0f,  -0.5f,
	 1.7f,  1.0f,  -0.5f,
	 1.7f,  1.0f,  0.5f,
	 0.7f,  2.0f,  0.5f,
	 0.7f,  2.0f,  -0.5f,
	 1.7f,  2.0f,  -0.5f,
	 1.7f,  2.0f,  0.5f
};

GLuint c2indices[] =
{
  0, 2, 3, 0, 3, 1,
  2, 6, 7, 2, 7, 3,
  6, 4, 5, 6, 5, 7,
  4, 0, 1, 4, 1, 5,
  0, 4, 6, 0, 6, 2,
  1, 5, 7, 1, 7, 3,
};

GLfloat lightVertices[] =
{ 
	-0.1f, -0.1f,  0.1f,
	-0.1f, -0.1f, -0.1f,
	 0.1f, -0.1f, -0.1f,
	 0.1f, -0.1f,  0.1f,
	-0.1f,  0.1f,  0.1f,
	-0.1f,  0.1f, -0.1f,
	 0.1f,  0.1f, -0.1f,
	 0.1f,  0.1f,  0.1f
};

GLuint lightIndices[] =
{
	0, 1, 2,
	0, 2, 3,
	0, 4, 7,
	0, 7, 3,
	3, 7, 6,
	3, 6, 2,
	2, 6, 5,
	2, 5, 1,
	1, 5, 4,
	1, 4, 0,
	4, 5, 6,
	4, 6, 7
};

int main()
{
	// inicijalizacija glfw-a
	glfwInit();

	// koju verziju koristimo
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// stvaranje prozora
	GLFWwindow* window = glfwCreateWindow(width, height, "Corgi_pyramid", NULL, NULL);
	// error u stvaranju prozora
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// ucitavanje GLAD
	gladLoadGL();
	// lokacija viewporta
	glViewport(0, 0, width, height);

	// generiranje shaderprogram
	Shader shaderProgram("default.vert", "default.frag");

	// Prva piramida
	// vertex array
	VAO VAO1;
	VAO1.Bind();

	// vertex buffer
	VBO VBO1(vertices, sizeof(vertices));
	// element buffer
	EBO EBO1(indices, sizeof(indices));

	// spajanje vbo atributa na vao
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 11 * sizeof(float), (void*)(6 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 3, 3, GL_FLOAT, 11 * sizeof(float), (void*)(8 * sizeof(float)));
	// unbindanje da ih ne mijenjamo
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	// Druga piramida
	VAO VAO2;
	VAO2.Bind();
	VBO VBO2(lvertices, sizeof(lvertices));
	EBO EBO2(lindices, sizeof(lindices));
	VAO2.LinkAttrib(VBO2, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAO2.LinkAttrib(VBO2, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO2.LinkAttrib(VBO2, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	VAO2.Unbind();
	VBO2.Unbind();
	EBO2.Unbind();

	// Prva kocka
	VAO VAO3;
	VAO3.Bind();
	VBO VBO3(c1vertices, sizeof(c1vertices));
	EBO EBO3(c1indices, sizeof(c1indices));	
	VAO3.LinkAttrib(VBO3, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);	
	VAO3.Unbind();
	VBO3.Unbind();
	EBO3.Unbind();

	//Druga kocka
	VAO VAO4;
	VAO4.Bind();
	VBO VBO4(c2vertices, sizeof(c2vertices));
	EBO EBO4(c2indices, sizeof(c2indices));
	VAO4.LinkAttrib(VBO4, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
	VAO4.Unbind();
	VBO4.Unbind();
	EBO4.Unbind();

	// Shader za svijetlo
	Shader lightShader("light.vert", "light.frag");
	// vertex array za svijetlo
	VAO lightVAO;
	lightVAO.Bind();
	// vertex buffer
	VBO lightVBO(lightVertices, sizeof(lightVertices));
	// element buffer
	EBO lightEBO(lightIndices, sizeof(lightIndices));
	// vbo attributi na vao
	lightVAO.LinkAttrib(lightVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
	// unbindanje da ih ne mijenjamo
	lightVAO.Unbind();
	lightVBO.Unbind();
	lightEBO.Unbind();

	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	glm::vec3 pyramidPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 pyramidModel = glm::mat4(1.0f);
	pyramidModel = glm::translate(pyramidModel, pyramidPos);

	lightShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
	glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(pyramidModel));
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	//teksture

	Texture brickTex("wowowo.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	brickTex.texUnit(shaderProgram, "tex0", 0);

	Texture corgoTex("corgi1.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	corgoTex.texUnit(shaderProgram, "tex0", 0);


	// Depth Buffer
	glEnable(GL_DEPTH_TEST);

	// stvaranje kamere
	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

	// glavni loop 
	while (!glfwWindowShouldClose(window))
	{

		// Boja pozadina, neka bude neka lila aj
		glClearColor(0.2f, 0.1f, 0.2f, 1.0f);
		// cisti pozadinu i depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		camera.Inputs(window);
		camera.updateMatrix(45.0f, 0.1f, 100.0f);

		// aktiviramo shadere koje zelimo
		shaderProgram.Activate();

		glUniform3f(glGetUniformLocation(shaderProgram.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);

		
		camera.Matrix(shaderProgram, "camMatrix");

		// binda texturu da se moze renderati
		corgoTex.Bind();
		
		// binda vao da opengl zna koristit
		VAO1.Bind();
		
		// crta objekt
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);

		brickTex.Bind();
	
		VAO2.Bind();
		glDrawElements(GL_TRIANGLES, sizeof(lindices) / sizeof(int), GL_UNSIGNED_INT, 0);

		VAO3.Bind();
		glDrawElements(GL_TRIANGLES, sizeof(c1indices) / sizeof(int), GL_UNSIGNED_INT, 0);

		VAO4.Bind();
		glDrawElements(GL_TRIANGLES, sizeof(c2indices) / sizeof(int), GL_UNSIGNED_INT, 0);

		// govori koji shader zelimo, ovo je za svijetlo
		lightShader.Activate();
		camera.Matrix(lightShader, "camMatrix");
		// binda vao da opengl zna koristit
		lightVAO.Bind();
		// crta objekt, ovdje svjetlosnu kocku
		glDrawElements(GL_TRIANGLES, sizeof(lightIndices) / sizeof(int), GL_UNSIGNED_INT, 0);
		// swapa front i back buffer
		glfwSwapBuffers(window);
		// idk nes za glfw evente
		glfwPollEvents();
	}

	// brise sve objekte da odrzimo kod cistim
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	VAO2.Delete();
	VBO2.Delete();
	EBO2.Delete();
	VAO3.Delete();
	VBO3.Delete();
	EBO3.Delete();
	VAO4.Delete();
	VBO4.Delete();
	EBO4.Delete();
	lightVAO.Delete();
	lightVBO.Delete();
	lightEBO.Delete();
	lightShader.Delete();
	brickTex.Delete();
	corgoTex.Delete();
	shaderProgram.Delete();
	// brisanje prozora prije kraja programa
	glfwDestroyWindow(window);
	// terminacija glfwa prije kraja programa
	glfwTerminate();
	return 0;
}