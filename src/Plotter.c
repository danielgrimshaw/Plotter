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
double scale_x = 0.1;
double offset_x = 0.0;
char * vertex_fname = "Plotter_vertex_default.glsl";
char * fragment_fname = "Plotter_fragment_default.glsl";
char * data_fname = "data.csv";
const char * controls = "\t\t\tPlotter\n"
"Right now its just a picture, have fun :-)\n";

int main(int argc, char ** argv) {
	char * file_string;
	Point * data;

	//file_string = read_file((const char *)data_fname);

	//data = malloc(sizeof(Point) * num_points(file_string));
	//parse_csv(file_string, &data, num_points(file_string));
	data = malloc(sizeof(Point)*data_size);
	
	int i;
	for (i = 0; i < data_size; i++) {
		GLfloat x = (i - 1000.0) / 100.0;
		data[i].x = x;
		data[i].y = sin(x*10.0)/(1.0 + x * x);
		data[i].z = 0.0f;
	}

	for (i = 995; i < 1005; i++) {
		printf("(%f, %f, %f)\n", data[i].x, data[i].y, data[i].z);
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
	set_uniform1f(prog, "offset_x", (GLfloat)offset_x);
	set_uniform1f(prog, "scale_x", (GLfloat)scale_x);
	
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glViewport(0,0,800,600); // Tell GPU where to draw to and window size
	glutMainLoop(); // Enter callback loop
	glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
	return 0;
}

void draw(void) { // Redraw function
	glUseProgram(prog);
	glBindVertexArray(vao);
	glDrawArrays(GL_POINTS, 0, data_size);
	glBindVertexArray(0);
	glutSwapBuffers();
}

void idle_handler(void) {
	// Idle handler
	glutPostRedisplay();
}

void key_handler(unsigned char key, int x, int y) {
	// Keyboard handler
	// Empty for now
}

void special_handler(int key, int x, int y) {
	// Handle the special key presses
	switch(key) {
		case GLUT_KEY_LEFT:
			printf("left\n");
			offset_x -= 0.1;
			break;
		case GLUT_KEY_RIGHT:
			printf("right\n");
			offset_x += 0.1;
			break;
		case GLUT_KEY_UP:
			printf("up\n");
			scale_x *= 1.5;
			break;
		case GLUT_KEY_DOWN:
			printf("down\n");
			scale_x /= 1.5;
			break;
		case GLUT_KEY_HOME:
			printf("home\n");
			offset_x = 0.0;
			scale_x = 1.0f;
			break;
	}
	
	set_uniform1f(prog, "offset_x", (GLfloat)offset_x);
	set_uniform1f(prog, "scale_x", (GLfloat)scale_x);
	
	glutPostRedisplay();
}

void button_handler(int bn, int state, int x, int y) {
	// Button handler
}

void mouse_handler(int x, int y) {
	// Mouse motion handler
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
