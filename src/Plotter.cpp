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

// Standard Libraries
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

// OpenGL
#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glut.h>

// OpenGL Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other includes
#include "util.h"
#include "Plotter.h"

GLuint vao;
GLuint vbo;

unsigned int prog; // Program ID

glm::vec3 camera_pos = glm::vec3(0.0f, 0.0f, 3.0f); // Where the camera is
glm::vec3 camera_target = glm::vec3(0.0f, 0.0f, 0.0f); // Where the camera is pointing
glm::vec3 camera_dir = glm::normalize(camera_pos - camera_target); // Direction from the origin to the camera
glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 camera_right = glm::normalize(glm::cross(up, camera_dir));
glm::vec3 camera_up = glm::cross(camera_dir, camera_right);
glm::vec3 camera_front = glm::vec3(0.0f, 0.0f, -1.0f);


glm::mat4 model_transform; // Transformation for each model on the scene
glm::mat4 view; // Transformation of the scene for the camera
glm::mat4 projection; // Projection matrix (simulates perspective)
mouse_button button;
GLfloat yaw = -90.0f;
GLfloat pitch = 0.0f;
bool scroll_pressed = false;
int prev_x = 0;
int prev_y = 0;
int window_x = 800;
int window_y = 600;
unsigned long last_frame = get_msec();
int data_size = 2000;
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
	data = new Point[data_size];
	vbo_data_size = sizeof(Point)*data_size;
	
	int i;
	for (i = 0; i < data_size; i++) {
		GLfloat x = (i - 1000.0) / 100.0;
		data[i].x = x;
		data[i].y = sin(x*10.0)/(1.0 + x * x);
		data[i].z = 1.0f;
	}

	printf(controls);
	unsigned long start = get_msec();
	while (get_msec() - start < 5000) {
		// Instructions are being read
	}

	//Initialize GLUT
	glutInit(&argc, argv);
	glutInitWindowSize(window_x, window_y);

	window_x = glutGet(GLUT_SCREEN_WIDTH);
	window_y = glutGet(GLUT_SCREEN_HEIGHT);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutCreateWindow("Plotter");

	glutDisplayFunc(draw);
	glutIdleFunc(idle_handler);
	glutReshapeFunc(resize_handler);
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

	projection = glm::perspective(45.0f, (float)window_x/window_y, 0.1f, 100.0f);
	view = glm::lookAt(camera_pos, camera_target, up);
	
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
	glutFullScreen();
	glutMainLoop(); // Enter callback loop
	glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
	return 0;
}

void draw(void) { // Redraw function
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(prog);

	GLint uniform_loc = glGetUniformLocation(prog, "model_transform");
	glUniformMatrix4fv(uniform_loc, 1, GL_FALSE, glm::value_ptr(model_transform));

	uniform_loc = glGetUniformLocation(prog, "view");
	glUniformMatrix4fv(uniform_loc, 1, GL_FALSE, glm::value_ptr(view));

	uniform_loc = glGetUniformLocation(prog, "projection");
	glUniformMatrix4fv(uniform_loc, 1, GL_FALSE, glm::value_ptr(projection));

	glBindVertexArray(vao);
	glDrawArrays(GL_LINE_STRIP, 0, data_size);
	glBindVertexArray(0);

	glutSwapBuffers();
	last_frame = get_msec();
}

void idle_handler(void) {
	// Idle handler
	glutPostRedisplay();
}

void resize_handler(int width, int height) {
	window_x = width;
	window_y = height;

	projection = glm::perspective(45.0f, (float)window_x/window_y, 0.0001f, 100.0f);
}
void key_handler(unsigned char key, int x, int y) {
	// Keyboard handler
	GLfloat fast = (get_msec() - last_frame != 0 ? (get_msec() - last_frame) : 1)*1.0f;
	GLfloat slow = (get_msec() - last_frame != 0 ? (get_msec() - last_frame) : 1)*0.1f;

	switch(key) {
		case 'q':
		case 'Q':
		case 27: // ESC key
			exit(0);
			break;
		case 'w':
			camera_pos += slow * camera_front;
			break;
		case 'W':
			camera_pos += fast * camera_front;
			break;
		case 's':
			camera_pos -= slow * camera_front;
			break;
		case 'S':
			camera_pos -= fast * camera_front;
			break;
		case 'a':
			camera_pos -= glm::normalize(glm::cross(camera_front, camera_up)) * slow;
			break;
		case 'A':
			camera_pos -= glm::normalize(glm::cross(camera_front, camera_up)) * fast;
			break;
		case 'd':
			camera_pos += glm::normalize(glm::cross(camera_front, camera_up)) * slow;
			break;
		case 'D':
			camera_pos += glm::normalize(glm::cross(camera_front, camera_up)) * fast;
			break;
	}

	view = glm::lookAt(camera_pos, camera_pos + camera_front, camera_up);

	glutPostRedisplay();
}

void special_handler(int key, int x, int y) {
	// Handle the special key presses

	GLfloat fast = (get_msec() - last_frame != 0 ? (get_msec() - last_frame) : 1)*1.0f;
	GLfloat slow = (get_msec() - last_frame != 0 ? (get_msec() - last_frame) : 1)*0.1f;

	switch(key) {
		case GLUT_KEY_LEFT:
			camera_pos -= glm::normalize(glm::cross(camera_front, camera_up)) * slow;
			break;
		case GLUT_KEY_RIGHT:
			camera_pos += glm::normalize(glm::cross(camera_front, camera_up)) * slow;
			break;
		case GLUT_KEY_UP:
			camera_pos += slow * camera_front;
			break;
		case GLUT_KEY_DOWN:
			camera_pos -= slow * camera_front;
			break;
	}
	
	view = glm::lookAt(camera_pos, camera_pos + camera_front, camera_up);

	glutPostRedisplay();
}

void button_handler(int bn, int state, int x, int y) {
	// Button handler
	switch(bn) {
		case GLUT_MIDDLE_BUTTON:
			if (state == GLUT_DOWN) {
				scroll_pressed = true;
				button = MOUSE_MIDDLE;
			} else {
				scroll_pressed = false;
				button = MOUSE_NONE;
			}
			break;
		case GLUT_LEFT_BUTTON:
			if (state == GLUT_DOWN) {
				button = MOUSE_LEFT;
			} else {
				button = MOUSE_NONE;
			}
			break;
		case GLUT_RIGHT_BUTTON:
			if (state == GLUT_DOWN) {
				button = MOUSE_RIGHT;
			} else {
				button = MOUSE_NONE;
			}
			break;/*
		case 3: // Scroll up
			if (scroll_pressed) {
				model_transform = glm::scale(model_transform, glm::vec3(1.01f, 1.01f, 1.01f));
			} else {
				model_transform = glm::scale(model_transform, glm::vec3(1.1f, 1.1f, 1.1f));
			}
			break;
		case 4: // Scroll down
			if (scroll_pressed) {
				model_transform = glm::scale(model_transform, glm::vec3(0.99f, 0.99f, 0.99f));
			} else {
				model_transform = glm::scale(model_transform, glm::vec3(0.9f, 0.9f, 0.9f));
			}
			break;*/
	}

	glutPostRedisplay();
}

void mouse_handler(int x, int y) {
	// Mouse active motion handler
	// This will be called when the mouse is moving with a button pressed
	// otherwise known as dragged
	GLfloat sensitivity = 0.2f;
	GLfloat delta_x = x - prev_x;
	GLfloat delta_y = y - prev_y;
	delta_y *= -1;
	delta_x *= sensitivity;
	delta_y *= sensitivity;

	if (button == MOUSE_NONE) {
		yaw += delta_x;
		pitch += delta_y;
	}

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	camera_front = glm::normalize(front);

	prev_x = x;
	prev_y = y;

	view = glm::lookAt(camera_pos, camera_pos + camera_front, camera_up);

	glutPostRedisplay();
}

void mouse_idle_handler(int x, int y) {
	// Mouse idle motion handler
	mouse_handler(x, y);
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
