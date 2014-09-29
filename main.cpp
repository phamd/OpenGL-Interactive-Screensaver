#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <windows.h> //
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <vector>
#include <math.h>
#include "main.h"
#include "containers.h"
#include "calculations.h"

// Globals
GLfloat gSize = 3;
const GLfloat gSizeStep = 1;
int clickedTimes = 0;
int gWidth = 600;
int gHeight = 600;
int gTime = 0;
float gSpeed = 0.25;
const float gSpeedStep = 0.05;
float gSpeedPause = 0.01;
bool gPaused = false;
bool gfullscreen = true;
bool gHelpMenu = true;
enum Modes {Dot, Line, Poly};
Modes mouseMode = Dot;
std::vector<Drawable> drawable;
std::vector<Drawable>::iterator i;
std::vector<Vertex> currentVertices;
std::vector<Vertex>::iterator j;
Vertex currentVertex;

void updateSizes(void)
{
	glPointSize(gSize);
	glLineWidth(gSize);
}

void drawDrawable(void)
{
	int elapsedTime = glutGet(GLUT_ELAPSED_TIME);
	int deltaTime = elapsedTime - gTime;
	gTime = elapsedTime;

	for (i = drawable.begin(); i != drawable.end(); i++) {
		glBegin(i->type);
		for (j = i->vertices.begin(); j != i->vertices.end(); j++) {
			if (j->position.x < 0) j->direction.x = abs(j->direction.x); // boundry check
			if (j->position.x > gWidth) j->direction.x = 0 - abs(j->direction.x); // simply flipping the sign wouldn't work
			if (j->position.y < 0) j->direction.y = abs(j->direction.y);
			if (j->position.y > gHeight) j->direction.y = 0 - abs(j->direction.y);
			
			j->position.x += j->direction.x * deltaTime * gSpeed;
			j->position.y += j->direction.y * deltaTime * gSpeed;
			glColor3f(j->color.x, j->color.y, j->color.z);
			glVertex2f((GLfloat)j->position.x, (GLfloat)j->position.y);
		}
		glEnd();
	}
}


void makeVertex(void) {
	currentVertex.position = Vector2d(randFloat(0, gWidth), randFloat(0, gHeight));
	currentVertex.direction = Vector2d(randFloat(-1, 1), randFloat(-1, 1));
	currentVertex.color = randomColor();
	currentVertices.push_back(currentVertex);
}

void randomizeScene(void)
{
	for (int i = 0; i < (int) randFloat(50,100); i++) { // 50 to 100 objects
		
		float decideShape = randFloat(0, 1);
			
		if (decideShape < 0.7) { // 70% point
			makeVertex();
			drawable.push_back(currentVertices);
			drawable.back().type = GL_POINTS;
			clearCurrent();
		}
		else if (decideShape > 0.95) { // 5% polygon
			for (int i = 0; i < (int)randFloat(3, 7); i++) { // 3 to 7 vertices in a polygon
				makeVertex();
			}
			drawable.push_back(currentVertices);
			drawable.back().type = GL_POLYGON;
			clearCurrent();
		}
		else { // 25% line
			makeVertex();
			makeVertex();
			drawable.push_back(currentVertices);
			drawable.back().type = GL_LINES;
			clearCurrent();
		}
	}
}

void clearCurrent(void)
{
	currentVertices = std::vector<Vertex>(); // empty the in progress vertices
	currentVertex = Vertex();
	clickedTimes = 0;
}

void drawHelpWindow(void)
{
	glColor3f(1.0f, 1.0f, 1.0f);

	std::string line;
	std::ifstream helpfile("help.txt");

	float startX = gWidth / 4; // position of the first line
	float startY = gHeight / 4;

	if (helpfile.is_open())	{
		while (getline(helpfile, line))
		{
			glRasterPos2f(startX, startY);
			const char *tmp = line.c_str();
			for (size_t i = 0; i < strlen(tmp); ++i) {
				glutBitmapCharacter(GLUT_BITMAP_9_BY_15, tmp[i]);
			}
			startY += 20;
		}
	}
	else {
		std::cout << "help.txt missing!";
	}
	helpfile.close();
	/*
	glRasterPos2f(startX, startY);
	std::string tmp2 = "Current Speed: " + std::to_string(gSpeed);
	const char *tmp = tmp2.c_str();
	for (size_t i = 0; i < strlen(tmp); ++i) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, tmp[i]);
	} */
}


void mouse(int btn, int state, int x, int y)
{
	switch (btn) {
	case(GLUT_LEFT_BUTTON) :
		printf("%d %d \n", x, y);
		Vector2d clickedXY = Vector2d((float)x, (float)y);
		if ((clickedTimes % 2 == 0) && state == GLUT_UP) { // first click
			currentVertex.position = clickedXY;
			clickedTimes++;
		}
		else if ((clickedTimes % 2 == 1) && state == GLUT_UP) { // second click
			currentVertex.direction = pointSlope(currentVertex.position.x, currentVertex.position.y, x, y);
			currentVertex.color = randomColor(); //color
			currentVertices.push_back(currentVertex);
			clickedTimes++;
		}
		if (((clickedTimes == 2 && mouseMode == Dot) || (clickedTimes == 4 && mouseMode == Line)) && state == GLUT_UP) {
			drawable.push_back(currentVertices);
			switch (mouseMode){
			case(Dot) :
				drawable.back().type = GL_POINTS;
				break;
			case(Line) :
				drawable.back().type = GL_LINES;
				break;
			}
			clearCurrent();
		} 
		break;
	}
}

void keyboard(unsigned char btn, int x, int y)
{
	switch (btn) {
	case('1') : // Dot Mode
		mouseMode = Dot;
		clearCurrent();
		break;
	case('2') : // Line Mode
		mouseMode = Line;
		clearCurrent(); 
		break;
	case('3') : // Polygon Mode
		mouseMode = Poly;
		clearCurrent(); 
		break;
	case('-') : // Speed Down
		gSpeed = (gSpeed - gSpeedStep < 0) ? 0 : gSpeed - gSpeedStep; // no negative speeds
		printf("- pressed\n"); //debug
		break;
	case('+') : // Speed Up
		gSpeed += gSpeedStep;
		printf("+ pressed\n"); //debug
		break;
	case('I') : // DELETE THIS
		gSpeed = (gSpeed - gSpeedStep < 0) ? 0 : gSpeed - 2; // no negative speeds
		break;
	case('O') : // DELETE THIS
		gSpeed = (gSpeed < 0) ? 0 : gSpeed + 2;
		break;
	case('a') :
		randomizeScene();
		break;
	case('r') : // Reset
		drawable.clear();
		break;
	case('p') : // Pause
		gSpeedPause = gPaused ? gSpeedPause : gSpeed;
		gSpeed = gPaused ? gSpeedPause : 0;
		gPaused = !gPaused;
		break;
	case('g') : // Size Down
		gSize = (gSize - gSizeStep < 1) ? 1 : gSize - gSizeStep;
		updateSizes();
		break;
	case('t') : // Size Up
		gSize += gSizeStep;
		updateSizes();
		break;
	case(' ') : // Finish Polygon
		drawable.push_back(currentVertices);
		drawable.back().type = GL_POLYGON;
		currentVertices = std::vector<Vertex>(); // empty it
		clickedTimes = 0;
		break;
	case('h') : // Show Help
		gHelpMenu = !gHelpMenu;
		break;
	case('q') : // Exit
		exit(0);
		break;
	case('f') :
		if (gfullscreen) {
			glutPositionWindow(0, 0);
			glutReshapeWindow(600, 600);
			gfullscreen = false;
		}
		else {
			glutFullScreen();
			gfullscreen = true;
		}
		break;
	}
}

void reshape(int w, int h)
{
	gWidth = w;
	gHeight = h;

	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, glutGet(GLUT_WINDOW_WIDTH) - 1, glutGet(GLUT_WINDOW_HEIGHT) - 1, 0);
	glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
}

void timer(int value)
{
	glutPostRedisplay();
	glutTimerFunc(16, timer, 0);
}

void init(void)
{
	updateSizes();
	glColor3f(1, 1, 1);
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	drawDrawable();
	if (gHelpMenu) drawHelpWindow();
	//glFlush();
	glutSwapBuffers(); // double buffer
}

int main(int argc, char** argv)
{
	std::string line;
	std::ifstream helpfile("help.txt");
	if (helpfile.is_open())	{
		while (getline(helpfile, line))
		{
			std::cout << line << "\n";
		}
	}
	else {
		std::cout << "help.txt missing!";
	}
	helpfile.close();


	/*
	printf("List of commands:\n");
	printf("%8s %25s\n", "Key", "Function");
	printf("-------------------------------------\n");
	printf("%8s %25s\n", "1", "Dot Mode");
	printf("%8s %25s\n", "2", "Line Mode");
	printf("%8s %25s\n", "3", "Polygon Mode");
	printf("%8s %25s\n", "<space>", "Finish Polygon");
	printf("%8s %25s\n", "+", "Increase speed");
	printf("%8s %25s\n", "-", "Decrease speed");
	printf("%8s %25s\n", "y", "Increase vertex size");
	printf("%8s %25s\n", "h", "Decrease vertex size");
	printf("%8s %25s\n", "r", "Reset scene");
	printf("%8s %25s\n", "a", "Randomize scene");
	printf("%8s %25s\n", "f", "Toggle fullscreen");
	*/
	system("PAUSE");

	glutInit(&argc, argv);		  //starts up GLUT
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	//glutInitWindowSize(600, 600);
	glutCreateWindow("Screensaver");	 //creates the window

	glutFullScreen();
	//gluOrtho2D(0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), 0);
	
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutTimerFunc(16, timer, 0);

	glutMainLoop();
	return(0);
}




