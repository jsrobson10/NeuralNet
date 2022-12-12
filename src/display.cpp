
#include <GLFW/glfw3.h>

#include <memory>
#include <iostream>
#include <thread>
#include <atomic>

#include "time.hpp"
#include "display.hpp"

static GLFWwindow* window;
static const int GRAPH_LEN = 160;
static int window_w = 1;
static int window_h = 1;

static void render(Brain* brain)
{
	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &window_w, &window_h);
	glViewport(0, 0, window_w, window_h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	
	brain->render();

	glfwSwapBuffers(window);
	glfwPollEvents();
}

bool Display::shouldClose()
{
	return glfwWindowShouldClose(window);
}

void Display::update(Brain* brain)
{
	static long last = 0;
	long now = Time::get_us();

	if(now - last > 10000)
	{
		last = now;
		render(brain);
	}
}

void Display::init()
{
	// init glfw
	if(!glfwInit())
	{
		throw "Failed to initialize GLFW";
	}

	// init window
	if(!(window = glfwCreateWindow(640, 480, "Neural Network Sim", nullptr, nullptr)))
	{
		throw "Failed to create window";
	}
}

void Display::cleanup()
{
	glfwTerminate();
}

void Display::Draw::colour(double r, double g, double b)
{
	glColor3d(r, g, b);
}

void Display::Draw::colour(double r, double g, double b, double a)
{
	glColor4d(r, g, b, a);
}

void Display::Draw::quad(Vector p1, Vector p2)
{
	p1 = Vector(p1.x / window_w * 2, p1.y / window_h * 2);
	p2 = Vector(p2.x / window_w * 2, p2.y / window_h * 2);

	glBegin(GL_QUADS);
	glVertex2d(p1.x, p1.y);
	glVertex2d(p2.x, p1.y);
	glVertex2d(p2.x, p2.y);
	glVertex2d(p1.x, p2.y);
	glEnd();
}

void Display::Draw::line(Vector p1, Vector p2)
{
	p1 = Vector(p1.x / window_w * 2, p1.y / window_h * 2);
	p2 = Vector(p2.x / window_w * 2, p2.y / window_h * 2);
	
	glBegin(GL_LINES);
	glVertex2d(p1.x, p1.y);
	glVertex2d(p2.x, p2.y);
	glEnd();
}

void Display::Draw::dot(Vector p, double r)
{
	quad(p - r, p + r);
}

