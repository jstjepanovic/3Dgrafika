#define GLFW_DLL
#define GLEW32_DLL


#include <vector>
#include <iostream>
#include <algorithm>
#include <string>
#include <limits>
#include "GL/glew.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "CircleRenderer.h"
#include "LineRenderer.h"

using namespace std;

int Factorial(int x)
{
	if (x > 1)
		return x * Factorial(x - 1);
	else
		return 1;
}

int BinomialCoefficiant(int n, int k)
{
	return Factorial(n) / (Factorial(k) * Factorial(n - k));
}

class BezierCurve
{
public:
	void RegisterPoint(float x, float y)
	{
		points.push_back({ x,y });
	}

	void ClearPoints()
	{
		points.clear();
	}

	std::vector<glm::vec2> GetCurve()
	{
		vector<glm::vec2> CPoints;

		float curveStart = points[0].x;

		float curveLength = 0.0f;

		for (int i = 0; i < points.size() - 1; i++)
		{
			curveLength += abs(sqrt(pow(points[i + 1].x - points[i].x, 2) + pow(points[i + 1].y - points[i].y, 2)));
		}

		float preciznost = 1.0f;
		float curPoint = 0.0f;
		float curCurveLength = 0.0f;

		while (curPoint < curveLength)
		{
			float t = curPoint / curveLength;

			int n = points.size() - 1;
			glm::vec2 point(0.0f, 0.0f);

			for (int i = 0; i <= n; i++)
			{
				point += (BinomialCoefficiant(n, i) * pow(1 - t, n - i) * pow(t, i)) * points[i];
			}

			CPoints.push_back(point);
			curPoint += preciznost;
		}

		return CPoints;
	}

public:
	std::vector<glm::vec2> points;
};


int main()
{
	GLFWwindow* window;

	if (!glfwInit())
		throw std::runtime_error("glfwInit failed");

	// Tell GLFW what version of OpenGL we are using 
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(1000, 1000 * 9/16, "test window", NULL, NULL);
	if (!window)
		throw std::runtime_error("glfwOpenWindow failed.");

	// GLFW settings
	glfwMakeContextCurrent(window);

	glewInit();

	int nx, ny;
	glfwGetFramebufferSize(window, &nx, &ny);
	glViewport(0, 0, nx, ny);
	//glewExperimental = GL_TRUE;


	// ispisi neke informacije
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);
	printf("renderer: %s\n", renderer);
	printf("OpenGL version supported: %s\n", version);

	// definiraj z-spremnik i test dubine
	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS);  // manja z-vrijednost jest blizi predmet;

	InitCircleRendering(32);
	InitLineRendering();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	vector<glm::vec2> pointovi;
	pointovi.push_back(glm::vec2(50.0f, 50.0f));
	pointovi.push_back(glm::vec2(850.0f, 320.0f));
	pointovi.push_back(glm::vec2(600.0f, 400.0f));
	pointovi.push_back(glm::vec2(250.0f, 250.0f));
	

	BezierCurve curve;
	for (auto o : pointovi) {
		curve.RegisterPoint(o.x, o.y);
	}
	
	

	while (!glfwWindowShouldClose(window))
	{

		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);

		glClear(GL_COLOR_BUFFER_BIT);

		

		vector<glm::vec2> vec = curve.GetCurve();
		
		for (auto p : curve.points) {
			RenderCircle(p, 5);
		}

		RenderLine(vec);

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
			
			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);

			auto vec = glm::vec2(xpos, ypos);

			float min = 1e9;
			int pos = 1e9;

			for (int i = 0; i < curve.points.size(); i++)
			{
				float cur = glm::distance(curve.points[i], vec);
				if (cur < min) {
					min = cur;
					pos = i;
				}
			}

			curve.points[pos].x = xpos;
			curve.points[pos].y = ypos;
		}

		


		glfwSwapBuffers(window);

		glfwPollEvents();

	}

	glfwTerminate();

	return 0;
}

