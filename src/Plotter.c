/*
 * Plotter.c
 *
 * Copyright 2015  <Daniel Grimshaw>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include "util.h"
#include "Plotter.h"

GLuint vao;
GLuint vbo;

unsigned int prog; // Program ID

int data_size = 2000;
double scale_x = 1.0;
double offset_x = 0.0;
double scale_y = 1.0;
double offset_y = 0.0;
double scale_z = 1.0;
double offset_z = 0.0;
bool scroll_pressed = false;
int prev_x = 0;
int prev_y = 0;
const int window_x = 800;
const int window_y = 600;
int vbo_data_size;
char * vertex_fname = "Plotter_vertex_default.glsl";
char * fragment_fname = "Plotter_fragment_default.glsl";
char * data_fname = "data.csv";
const char * controls = "\t\tPlotter\n"
"Press the left and right arrow keys to scroll\n"
"Press the up and down arrow keys to zoom\n";

int main(int argc, char ** argv) {
	char * file_string;
	Point * data;

	//file_string = read_file((const char *)data_fname);

	//data = malloc(sizeof(Point) * num_points(file_string));
	//parse_csv(file_string, &data, num_points(file_string));
	data = malloc(sizeof(Point)*data_size);
	vbo_data_size = sizeof(Point)*data_size;
	
	int i;
	for (i = 0; i < data_size; i++) {
		GLfloat x = (i - 1000.0) / 100.0;
		data[i].x = x;
		data[i].y = sin(x*10.0)/(1.0 + x * x);
		data[i].z = 0.0f;
	}

	//Initialize GLUT
	glutInit(&argc, argv);
	glutInitWindowSize(800, 600);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutCreateWindow("Plotter");

	glutDisplayFunc(draw);
	glutIdleFunc(idle_handler);
	glutKeyboardFunc(key_handler);
	glutSpecialFunc(special_handler);
	glutMouseFunc(button_handler);
	glutMotionFunc(mouse_handler);
	glutPassiveMotionFunc(mouse_idle_handler);

	// Load all library function pointers
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) { // Doesn't run without GLEW
        fprintf(stderr, "Failed to initialize GLEW! Aborting!\n");
        return EXIT_FAILURE;
    }

	// load and set the shader
	if (!(prog = setup_shader((const char *)vertex_fname, (const char *)fragment_fname))) {
		fprintf(stderr, "Failed to setup shaders!\n");
		return EXIT_FAILURE;
	}
	
	printf(controls);
	setScalingUniforms();
	
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vbo_data_size, data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (GLvoid *)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glViewport(0, 0, window_x, window_y); // Tell GPU where to draw to and window size
	glutMainLoop(); // Enter callback loop
	glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
	return 0;
}

void setScalingUniforms() {
	set_uniform1f(prog, "offset_x", (GLfloat)offset_x);
	set_uniform1f(prog, "scale_x", (GLfloat)scale_x);
	set_uniform1f(prog, "offset_y", (GLfloat)offset_y);
	set_uniform1f(prog, "scale_y", (GLfloat)scale_y);
	set_uniform1f(prog, "offset_z", (GLfloat)offset_z);
	set_uniform1f(prog, "scale_z", (GLfloat)scale_z);
}

void draw(void) { // Redraw function
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(prog);
	glBindVertexArray(vao);
	glDrawArrays(GL_LINE_STRIP, 0, data_size);
	glBindVertexArray(0);
	glutSwapBuffers();
}

void idle_handler(void) {
	// Idle handler
	glutPostRedisplay();
}

void key_handler(unsigned char key, int x, int y) {
	// Keyboard handler
	switch(key) {
		case 'q':
		case 'Q':
		case 27: // ESC key
			exit(0);
			break;
		case 'w':
			offset_x += 0.1;
			break;
		case 'W':
			offset_x += 0.01;
			break;
		case 's':
			offset_x -= 0.1;
			break;
		case 'S':
			offset_x -= 0.01;
			break;
		case 'a':
			offset_z -= 0.1;
			break;
		case 'A':
			offset_z -= 0.01;
			break;
		case 'd':
			offset_z += 0.1;
			break;
		case 'D':
			offset_z += 0.01;
			break;
	}

	setScalingUniforms();

	glutPostRedisplay();
}

void special_handler(int key, int x, int y) {
	// Handle the special key presses
	switch(key) {
		case GLUT_KEY_LEFT:
			offset_x -= 0.01;
			break;
		case GLUT_KEY_RIGHT:
			offset_x += 0.01;
			break;
		case GLUT_KEY_UP:
			offset_y += 0.01;
			break;
		case GLUT_KEY_DOWN:
			offset_y -= 0.01;
			break;
		case GLUT_KEY_HOME:
			offset_x = 0.0;
			scale_x = 1.0;
			offset_y = 0.0;
			scale_y = 1.0;
			offset_z = 0.0;
			scale_z = 1.0;
			break;
	}
	
	setScalingUniforms();
	
	glutPostRedisplay();
}

void button_handler(int bn, int state, int x, int y) {
	// Button handler
	switch(bn) {
		case GLUT_MIDDLE_BUTTON:
			if (state == GLUT_DOWN)
				scroll_pressed = true;
			else
				scroll_pressed = false;
			break;
		case 3: // Scroll up
			if (scroll_pressed) {
				scale_x *= 1.01;
				scale_y *= 1.01;
				scale_z *= 1.01;
			} else {
				scale_x *= 1.1;
				scale_y *= 1.1;
				scale_z *= 1.1;
			}
			break;
		case 4: // Scroll down
			if (scroll_pressed) {
				scale_x /= 1.01;
				scale_y /= 1.01;
				scale_z /= 1.01;
			} else {
				scale_x /= 1.1;
				scale_y /= 1.1;
				scale_z /= 1.1;
			}
			break;
	}

	setScalingUniforms();

	glutPostRedisplay();
}

void mouse_handler(int x, int y) {
	// Mouse active motion handler
	// This will be called when the mouse is moving with a button pressed
	// otherwise known as dragged
	offset_x += ((x - prev_x)/200.0)*(1/scale_x);
	offset_y -= ((y - prev_y)/150.0)*(1/scale_y);

	prev_x = x;
	prev_y = y;

	setScalingUniforms();

	glutPostRedisplay();
}

void mouse_idle_handler(int x, int y) {
	// Mouse idle motion handler
	prev_x = x;
	prev_y = y;
}

void parse_csv(char * csv_file_string, Point ** buffer, size_t buffer_size) {
	// parse_csv
	int i;
	for (i = 0; i < (int)buffer_size; i++) {
		buffer[i]->x = (GLfloat)0.0;
		buffer[i]->y = (GLfloat)0.0;
	}
}

size_t num_points(char * csv_file_string) {
	// num_points
	return 1;
}
