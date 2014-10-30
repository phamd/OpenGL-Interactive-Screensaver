OpenGL Interactive Screensaver
===============================
 - Draw dots, lines, and polygons that bounce around the screen with other additional features.
 - It is not truly a screensaver by definition, but a regular program with an interactive 2D sandbox.
 - This project is Written with OpenGL and C++ for a class assignment. 
 - Additional information and help is described in the program.

![Screenshot](screenshot.jpg?raw=true)

Required libraries
------------------
 - GLUT / Freeglut
 
Compiling
-------------------
 - A makefile is included and is customized to work on either of Windows/Linux/Mac.
 - Alternatively, a Visual Studios solution file is also present. 
 - Bug: Compiling using MinGW will give errors relating to `std::to_string` which is being used to draw debug info.
