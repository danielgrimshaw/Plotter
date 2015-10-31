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
#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif
#include <GL/glew.h>
#include <GL/glut.h>
#include "util.h"
#include "Plotter.h"

GLuint vbo;
char * vertex_fname = "Plotter_vertex_default.glsl";
char * fragment_fname = "Plotter_vertex_default.glsl";
char * data_fname = "data.csv"
const char * controls = "\t\t\tPlotter"
"Right now its just a picture, have fun :-)";

int main(int argc, char ** argv) {
	unsigned int prog; // Program ID
	char * file_string;
	Point * data;

	file_string = read_file(data_fname);
	data = parse_csv(file_string);


	//Initialize GLUT
	glutInit(&argc, argv);
	glutInitWindowSize(800, 600);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutCreateWindow("Plotter");

	glutDisplayFunc(draw);
	glutIdleFunc(idle_handler);
	glutKeyboardFunc(key_handler);
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
		return EXIT_FAILURE;
	}

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

	printf(controls);
	glViewport(0,0,800,600); // Tell GPU where to draw to and window size
	glutMainLoop(); // Enter callback loop
	return 0;
}

void draw(void) { // Redraw function
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glEnableVertexAttribArray(attribute_coord2d);
	glVertexAttribPointer(
		attribute_coord2d,   // attribute
		2,                   // number of elements per vertex, here (x,y)
		GL_FLOAT,            // the type of each element
		GL_FALSE,            // take our values as-is
		0,                   // no space between values
		0                    // use the vertex buffer object
	);

	glDrawArrays(GL_LINE_STRIP, 0, 2000);

	glDisableVertexAttribArray(attribute_coord2d);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void idle_handler(void) {
	// Idle handler
}

void key_handler(unsigned char key, int x, int y) {
	// Keyboard handler
}

void button_handler(int bn, int state, int x, int y) {
	// Button handler
}

void mouse_handler(int x, int y) {
	// Mouse motion handler
}
