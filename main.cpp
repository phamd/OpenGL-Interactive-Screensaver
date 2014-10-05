#ifdef WIN32
#include <windows.h>
#include <GL/freeglut.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <vector>
#include <math.h>
#include "containers.h"
#include "calculations.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h> // for random seed
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>

// Constants
const float gSizeStep = 1;
const float gSpeedStep = 0.05;
enum Modes {Dot, Line, Poly};
// States
Modes mouseMode = Dot;
bool gPaused = false;
bool gfullscreen = true;
bool gHelpMenu = true;
bool gDragMode = false; // Drag to set direction instead of clicking
float gSize = 3;
float gSpeed = 0.205;
float gSpeedPause = 0.01; // Speed to return to after pausing
int clickedTimes = 0;
int gWidth = 600;
int gHeight = 600;
int gTime = 0;
Vector2d gMousePos;
// Containers
std::vector<Drawable> drawable;
std::vector<Drawable>::iterator i;
std::vector<Vertex> currentVertices;
std::vector<Vertex>::iterator j;
Vertex currentVertex;
std::vector<std::string> helpText;

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

void clearCurrent(void)
{
	currentVertices = std::vector<Vertex>(); // empty the in progress vertices
	currentVertex = Vertex();
	clickedTimes = 0;
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

void drawSentence(const char* line, float startX, float startY)
{
	glColor3f(1.0f, 1.0f, 1.0f);
	for (size_t i = 0; i < std::strlen(line); ++i) {
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, line[i]);
	}
}

void drawDebugText(void)
{
	std::string debugText[5];
	debugText[0] = "Mouse Mode: " + std::to_string(mouseMode);
	debugText[1] = "Click count: " + std::to_string(clickedTimes);
	debugText[2] = "Number of Shapes: " + std::to_string(drawable.size());
	debugText[3] = "Speed: " + std::to_string(gSpeed);
	debugText[4] = "Size: " + std::to_string(gSize);

	for (int i = 0, Y = 20; i < 5; i++, Y += 20) {
		glRasterPos2f(10, Y);
		drawSentence(debugText[i].c_str(), 10, Y);
	}
}

void drawHelpText(float startX, float startY)
{
	for (std::vector<std::string>::iterator li = helpText.begin(); li != helpText.end(); li++) {
		glRasterPos2f(startX, startY);
		const char *tmp = li->c_str();
		drawSentence(tmp, startX, startY);
		startY += 20;
	}
	drawDebugText();
}


void mouse(int btn, int state, int x, int y)
{
	printf("Pressed %d, %d\n", btn, state);
	switch (btn) {
	case(GLUT_LEFT_BUTTON) :
		printf("%d %d \n", x, y);
		Vector2d clickedXY = Vector2d((float)x, (float)y);
		if ((clickedTimes == 0 || clickedTimes % 2 == 0) && state == (gDragMode ? GLUT_DOWN : GLUT_UP)) { // first click
			currentVertex.position = clickedXY;
			currentVertex.color = randomColor(); //color
			clickedTimes++;
		}
		else if ((clickedTimes % 2 == 1) && state == GLUT_UP) { // second click
			currentVertex.direction = pointSlope(currentVertex.position.x, currentVertex.position.y, x, y);
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
			case(Poly) :
				break;
			}
			clearCurrent();
		} 
		break;
	}
}

void mousePassive(int x, int y)
{
	gMousePos.x = x;
	gMousePos.y = y;
}

void drawDirectionArrow()
{
	int X = gMousePos.x;
	int Y = gMousePos.y;
	Vector2d arrowheadPos = pointSlope(gMousePos, currentVertex.position);
	float k = (-(X - arrowheadPos.x)) / ((X - arrowheadPos.x)*(X - arrowheadPos.x)); // What am I even doing here?
	Vector2d thirdPoint = Vector2d(arrowheadPos.x - k, arrowheadPos.x + k);

	if (clickedTimes % 2 == 1) {
		glBegin(GL_LINES); // Arrow line
			glColor3f(currentVertex.color.x, currentVertex.color.y, currentVertex.color.z);
			glVertex2f((GLfloat)currentVertex.position.x, (GLfloat)currentVertex.position.y);
			glVertex2f(X, Y);
		glEnd();
		glBegin(GL_POLYGON); // Arrow head
			glVertex2f(X, Y);
			glVertex2f(X - 20 * thirdPoint.x, Y - 20 * thirdPoint.y);
			glVertex2f(X-40*arrowheadPos.x, Y-40*arrowheadPos.y);
			glVertex2f(X+20*thirdPoint.x, Y+20*thirdPoint.y);
		glEnd();

	}
}

void keyboard(unsigned char btn, int x, int y)
{
	switch (btn) {
	case('h') : // Show Help
		gHelpMenu = !gHelpMenu;
		break;
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
	case(' ') : // Finish Polygon
		if (currentVertices.size() >= 3) {
			drawable.push_back(currentVertices);
			drawable.back().type = GL_POLYGON;
			currentVertices = std::vector<Vertex>(); // empty it
			clickedTimes = 0;
		}
		break;
	case('+') : // Speed Up
		gSpeed += gSpeedStep;
		break;
	case('-') : // Speed Down
		gSpeed = (gSpeed - gSpeedStep < 0) ? 0.005 : gSpeed - gSpeedStep; // no negative speeds
		break;
	case('t') : // Size Up
		gSize += gSizeStep;
		updateSizes();
		break;
	case('g') : // Size Down
		gSize = (gSize - gSizeStep < 1) ? 1 : gSize - gSizeStep;
		updateSizes();
		break;
	case('p') : // Pause scene
		gSpeedPause = gPaused ? gSpeedPause : gSpeed;
		gSpeed = gPaused ? gSpeedPause : 0;
		gPaused = !gPaused;
		break;
	case('r') : // Reset scene
		drawable.clear();
		break;
	case('a') : // Randomize scene
		randomizeScene();
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
	case('m') : // Drag direction mode
		gDragMode = !gDragMode;
		break;
	case('q') : // Exit
		exit(0);
		break;
	}

}

void reshape(int w, int h)
{
	gWidth = w;
	gHeight = h;

	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	//glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, glutGet(GLUT_WINDOW_WIDTH) - 1, glutGet(GLUT_WINDOW_HEIGHT) - 1, 0);
	//glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
}

void menu(int label) {

	switch (label) {
	case 'q':
		exit(0);
	}
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
	if (gHelpMenu) drawHelpText(gWidth / 4, gHeight / 4);
	//glFlush();
	drawDirectionArrow();
	glutSwapBuffers(); // double buffer
}

int main(int argc, char** argv)
{
	std::string line;
	std::ifstream helpfile("help.txt");

	if (helpfile.is_open())	{
		while (getline(helpfile, line)) {
			std::cout << line << "\n"; // Write to console
			helpText.push_back(line); // Save text for later as well
		}
	}
	else {
		std::cout << "help.txt missing!";
		helpText.push_back("help.txt missing!");
	}
	helpfile.close();

	#ifdef WIN32
		system("PAUSE"); // "Press any key to continue"
	#endif // WIN32

	srand(time(NULL));

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutCreateWindow("Interactive Screensaver");
	glutFullScreen();

	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutPassiveMotionFunc(mousePassive); // for direction arrows
	glutTimerFunc(16, timer, 0);

	glutCreateMenu(menu);
	glutAddMenuEntry("Quit", 'q');
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutMainLoop();
	return(0);
}




