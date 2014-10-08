#ifdef _WIN32
#   include <windows.h> // not needed explicitly
#endif
#ifdef __APPLE__
#   include <GLUT/glut.h>
#   include <OpenGL/gl.h>
#   include <OpenGL/glu.h>
#else
#   include <GL/glut.h>
#   include <GL/gl.h> // not needed explicitly
//#   include <GL/freeglut.h>
#endif
#include <vector>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include "containers.h"
#include "calculations.h"

// Constants
const float gSizeStep = 1;
const float gSpeedStep = 0.05f;
enum Modes {Dot, Line, Poly};
enum Colours { RED, GREEN, BLUE, YELLOW, MAGENTA, TEAL, ORANGE, PINK, PURPLE, WHITE, RANDOMCOLOUR };
// States
Modes mouseMode = Dot;
bool gPaused = false;
bool gfullscreen = false;
bool gHelpMenu = true;
bool gDragMode = false; // Drag to set direction instead of clicking
float gSize = 3;
float gSpeed = 0.205f;
float gSpeedPause; // Speed to return to after pausing
int clickedTimes = 0;
int gWidth = 600;
int gHeight = 600;
int gTime = 0;
Vector2d gMousePos;
Vector3d gColour = Vector3d(-1, -1, -1); 
// Containers
std::vector<Shapes> finishedShapes; // Shapes
std::vector<Vertex> fireworks; // Explosions
std::vector<Vertex> currentVertices;
Vertex currentVertex;
std::vector<std::string> helpText;

void drawShapes(void)
{
	int elapsedTime = glutGet(GLUT_ELAPSED_TIME);
	int deltaTime = elapsedTime - gTime;
	gTime = elapsedTime;
	
	// Draw finished shapes
	for (std::vector<Shapes>::iterator i = finishedShapes.begin(); i != finishedShapes.end(); i++) {
		glBegin(i->type);
			for (std::vector<Vertex>::iterator j = i->vertices.begin(); j != i->vertices.end(); j++) {
				if (j->position.x < 0) j->direction.x = abs(j->direction.x); // boundry check
				if (j->position.x > gWidth) j->direction.x = 0 - abs(j->direction.x);
				if (j->position.y < 0) j->direction.y = abs(j->direction.y);
				if (j->position.y > gHeight) j->direction.y = 0 - abs(j->direction.y);
			
				j->position.x += j->direction.x * deltaTime * gSpeed;
				j->position.y += j->direction.y * deltaTime * gSpeed;
				glColor3f(j->colour.x, j->colour.y, j->colour.z);
				glVertex2f((GLfloat)j->position.x, (GLfloat)j->position.y);
			}
		glEnd();
	}

	// Draw explosions
	for (std::vector<Vertex>::iterator j = fireworks.begin(); j != fireworks.end(); ) {
		if (j->position.x < 0 || j->position.x > gWidth || j->position.y < 0 || j-> position.y > gHeight) {
			j = fireworks.erase(j); // remove point from vector if it leaves the screen
		}
		else {
			j->position.x += j->direction.x * deltaTime * 0.205 * 2;
			j->position.y += j->direction.y * deltaTime * 0.205 * 2;
			glColor3f(j->colour.x, j->colour.y, j->colour.z);
			glBegin(GL_POINTS);
				glVertex2f((GLfloat)j->position.x, (GLfloat)j->position.y);
			glEnd();
			j++;
		}
	}

	// Draw unfinished shapes
	if (currentVertices.size() > 0) {
		for (std::vector<Vertex>::iterator j = currentVertices.begin(); j != currentVertices.end(); j++) {
			glBegin(GL_POINTS);
			glColor3f(j->colour.x, j->colour.y, j->colour.z);
			glVertex2f((GLfloat)j->position.x, (GLfloat)j->position.y);
			glEnd();
		}
	}
}

void clearCurrentVertex(void) // Empty the in progress vertices
{
	currentVertices.clear();
	currentVertex = Vertex();
	clickedTimes = 0;
}

void stickyVertexSwitch(void) // Switch between modes without erasing vertices in progress
{
	if (currentVertices.size() > 0) {
		if (mouseMode == Dot) {
			for (int i = 0; i < currentVertices.size(); i++) {
				finishedShapes.push_back(Shapes(currentVertices[i]));
				finishedShapes.back().type = GL_POINTS;
			}
			clearCurrentVertex();
		}
		else if (mouseMode == Line) {
			for (int i = 0, j = 1; j < currentVertices.size(); i+=2, j+=2) {
				std::vector<Vertex> newLine;
				newLine.push_back(currentVertices[i]);
				newLine.push_back(currentVertices[j]);
				finishedShapes.push_back(Shapes(newLine));
				finishedShapes.back().type = GL_LINES;
			}
			clearCurrentVertex();
		}
	}
}

void makeVertex(void)
{
	currentVertex.position = Vector2d(randFloat(0, gWidth), randFloat(0, gHeight));
	currentVertex.direction = Vector2d(randFloat(-1, 1), randFloat(-1, 1));
	currentVertex.colour = getColour(gColour);
	currentVertices.push_back(currentVertex);
}

void randomizeScene(void)
{
	for (int i = 0; i < (int) randFloat(50,100); i++) { // 50 to 100 objects	
		float decideShape = randFloat(0, 1);
		if (decideShape < 0.7) { // 70% point
			makeVertex();
			finishedShapes.push_back(currentVertices);
			finishedShapes.back().type = GL_POINTS;
			clearCurrentVertex();
		}
		else if (decideShape > 0.9) { // 10% polygon
			for (int i = 0; i < (int)randFloat(3, 7); i++) { // 3 to 7 vertices in a polygon
				makeVertex();
			}
			finishedShapes.push_back(currentVertices);
			finishedShapes.back().type = GL_POLYGON;
			clearCurrentVertex();
		}
		else { // 20% line
			makeVertex();
			makeVertex();
			finishedShapes.push_back(currentVertices);
			finishedShapes.back().type = GL_LINES;
			clearCurrentVertex();
		}
	}
}

void makeExplosion(Vertex vertex)
{
	Vertex fireworkPoint = Vertex();
	fireworkPoint.position = Vector2d(vertex.position.x, vertex.position.y);
	fireworkPoint.colour = vertex.colour;

	float x, y;
	for (x = -1; x <= 1; x += 0.25) {
		y = sqrt(1 - x*x);
		fireworkPoint.direction = Vector2d(x, y);
		fireworks.push_back(fireworkPoint);
		fireworkPoint.direction = Vector2d(x, -y);
		fireworks.push_back(fireworkPoint);
		fireworkPoint.direction = Vector2d(y, x);
		fireworks.push_back(fireworkPoint);
		fireworkPoint.direction = Vector2d(-y, x);
		fireworks.push_back(fireworkPoint);
	}
}

void blowUpRandomShape(void)
{
	if (finishedShapes.size() > 0) {
		int index = randFloat(0, finishedShapes.size() - 1);
		for (std::vector<Vertex>::iterator j = finishedShapes[index].vertices.begin(); j != finishedShapes[index].vertices.end(); j++) {
			makeExplosion(*j);
		}
		finishedShapes.erase(finishedShapes.begin() + index);
	}
}

void drawSentence(const char* line, float startX, float startY)
{
	glRasterPos2f(startX, startY);
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
	debugText[2] = "Number of Shapes: " + std::to_string(finishedShapes.size());
	debugText[3] = "Speed: " + std::to_string(gSpeed);
	debugText[4] = "Size: " + std::to_string(gSize);

	for (int i = 0, Y = 20; i < 5; i++, Y += 20) {
		drawSentence(debugText[i].c_str(), 10, Y);
	}
}

void drawHelpText(float startX, float startY)
{
	for (std::vector<std::string>::iterator li = helpText.begin(); li != helpText.end(); li++, startY+=20) {
		const char *tmp = li->c_str();
		drawSentence(tmp, startX, startY);
	}
	drawDebugText();
}

void drawDirectionIndicator(void)
{
	if (clickedTimes % 2 == 1) {
		float X = gMousePos.x;
		float Y = gMousePos.y;
		Vector2d arrowheadPos = pointSlope(gMousePos, currentVertex.position);
		Vector2d arrowSidePos = Vector2d(arrowheadPos.x - 1 / (X - arrowheadPos.x), arrowheadPos.x + 1 / (X - arrowheadPos.x));
		glColor3f(currentVertex.colour.x, currentVertex.colour.y, currentVertex.colour.z);
		glLineWidth(0.5f);
		glBegin(GL_LINES); // Arrow line
			glVertex2f((GLfloat)currentVertex.position.x, (GLfloat)currentVertex.position.y);
			glVertex2f(X, Y);
		glEnd();
		glBegin(GL_POLYGON); // Arrow head
			//glVertex2f(X, Y);
			glVertex2f(X - 20 * arrowSidePos.x, Y - 20 * arrowSidePos.y);
			glVertex2f(X - 40 * arrowheadPos.x, Y - 40 * arrowheadPos.y);
			glVertex2f(X + 20 * arrowSidePos.x, Y + 20 * arrowSidePos.y);
		glEnd();
		glLineWidth(gSize);
		glFlush();
	}
}

void updateSizes(void)
{
	glPointSize(gSize);
	glLineWidth(gSize);
}

void mousePassive(int x, int y)
{
	gMousePos.x = x;
	gMousePos.y = y;
}

void mouse(int btn, int state, int x, int y)
{
	switch (btn) {
	case(GLUT_LEFT_BUTTON) :
		Vector2d clickedXY = Vector2d((float)x, (float)y);
		if ((clickedTimes == 0 || clickedTimes % 2 == 0) && state == (gDragMode ? GLUT_DOWN : GLUT_UP)) { // first click
			currentVertex.position = clickedXY;
			currentVertex.colour = getColour(gColour);
			clickedTimes++;
		}
		else if ((clickedTimes % 2 == 1) && state == GLUT_UP) { // second click
			currentVertex.direction = pointSlope(currentVertex.position.x, currentVertex.position.y, x, y);
			currentVertices.push_back(currentVertex);
			clickedTimes++;
		}
		if (((clickedTimes == 2 && mouseMode == Dot) || (clickedTimes == 4 && mouseMode == Line)) && state == GLUT_UP) {
			finishedShapes.push_back(currentVertices);
			switch (mouseMode){
			case(Dot) :
				finishedShapes.back().type = GL_POINTS;
				break;
			case(Line) :
				finishedShapes.back().type = GL_LINES;
				break;
			case(Poly) :
				break;
			}
			clearCurrentVertex();
		} 
		break;
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
		stickyVertexSwitch();
		break;
	case('2') : // Line Mode
		mouseMode = Line;
		stickyVertexSwitch();
		break;
	case('3') : // Polygon Mode
		mouseMode = Poly;
		stickyVertexSwitch();
		break;
	case(' ') : // Finish Polygon
		if (currentVertices.size() >= 3) {
			finishedShapes.push_back(currentVertices);
			finishedShapes.back().type = GL_POLYGON;
			clearCurrentVertex();
		}
		break;
	case('=') : // Speed Up alternate button
	case('+') : // Speed Up
		gSpeed += !gPaused ? gSpeedStep : 0;
		break;
	case('-') : // Speed Down
		gSpeed = !gPaused ? (gSpeed - gSpeedStep < 0) ? 0.005f : gSpeed - gSpeedStep : gSpeed; // no negative speeds
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
		finishedShapes.clear();
		fireworks.clear();
		break;
	case('a') : // Randomize scene
		randomizeScene();
		break;
	case('f') :
		if (gfullscreen) {
			glutReshapeWindow(600, 720);
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
	case('b') : // Blow up a random shape
		blowUpRandomShape();
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
	glLoadIdentity();
	gluOrtho2D(0, w - 1, h - 1, 0);
	glMatrixMode(GL_MODELVIEW);
}

void menu(int label) {

	switch (label) {
	case 'q':
		exit(0);
	}
}

void colourMenu(int colour)
{
	switch (colour) {
	case (RED) :
		gColour = Vector3d(1, 0, 0);
		break;
	case (GREEN) :
		gColour = Vector3d(0, 0.5f, 0);
		break;
	case (BLUE) :
		gColour = Vector3d(0, 0, 1);
		break;
	case (YELLOW) :
		gColour = Vector3d(1, 1, 0);
		break;
	case (MAGENTA) :
		gColour = Vector3d(1, 0, 1);
		break;
	case (TEAL) :
		gColour = Vector3d(0, 0.5f, 0.5f);
		break;
	case (ORANGE) :
		gColour = Vector3d(1, 0.2706f, 0);
		break;
	case (PINK) :
		gColour = Vector3d(1, 0.4118f, 0.7059f);
		break;
	case (PURPLE) :
		gColour = Vector3d(0.5f, 0, 0.5f);
		break;
	case (WHITE) :
		gColour = Vector3d(1, 1, 1);
		break;
	case (RANDOMCOLOUR) :
	default :
		gColour = Vector3d(-1, -1, -1);
		break;
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
	drawShapes();
	if (gHelpMenu) drawHelpText(gWidth / 9, gHeight / 5);
	drawDirectionIndicator();
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

	#ifdef _WIN32
		system("PAUSE"); // "Press any key to continue"
	#endif

	srand(time(NULL));

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(600, 720); // Start Windowed
	glutCreateWindow("Interactive Screensaver");

	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutPassiveMotionFunc(mousePassive); // for direction indicators in click mode
	glutMotionFunc(mousePassive); // for direction indicators in drag mode
	glutTimerFunc(16, timer, 0);

	int colourSubMenu = glutCreateMenu(colourMenu);
	glutAddMenuEntry("Random", RANDOMCOLOUR);
	glutAddMenuEntry("Red", RED);
	glutAddMenuEntry("Green", GREEN);
	glutAddMenuEntry("Blue", BLUE);
	glutAddMenuEntry("Yellow", YELLOW);
	glutAddMenuEntry("Magenta", MAGENTA);
	glutAddMenuEntry("Teal", TEAL);
	glutAddMenuEntry("Orange", ORANGE);
	glutAddMenuEntry("Pink", PINK);
	glutAddMenuEntry("Purple", PURPLE);
	glutAddMenuEntry("White", WHITE);

	glutCreateMenu(menu);
	glutAddSubMenu("Set Colour", colourSubMenu);
	glutAddMenuEntry("Quit Program", 'q');
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutMainLoop();
	return(0);
}
