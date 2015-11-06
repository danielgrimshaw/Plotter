/*
 * Plotter.h
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


#ifndef SRC_PLOTTER_H_
#define SRC_PLOTTER_H_

typedef struct {
	GLfloat x;
	GLfloat y;
	GLfloat z;
} Point;

void parse_csv(char * csv_file_string, Point ** buffer, size_t buffer_size);
size_t num_points(char * csv_file_string);

void setScalingUniforms();

void draw(void); // Redraw function
void idle_handler(void); // Idle handler
void key_handler(unsigned char key, int x, int y); // Keyboard handler
void special_handler(int key, int x, int y); // Special key handler
void button_handler(int bn, int state, int x, int y); // Button handler
void mouse_handler(int x, int y); // Mouse motion handler

#endif /* SRC_PLOTTER_H_ */
