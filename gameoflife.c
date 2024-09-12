/************************************************************************
**
** NAME:        gameoflife.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

int isalive(Image *image, int row, int col)
{
	return image->image[row][col].R == 255 ? 1: 0;
}

int checkrow(Image *image, int row)
{
	if (row >= 0 && row < image->rows)
		return 1;
	return 0;
}
int checkcol(Image *image, int col)
{
	if (col >= 0 && col < image->cols)
		return 1;
	return 0;
}

int getalivenum(Image *image, int row, int col)
{
	int num = 0;
	for (int i = row - 1; i <= row + 1; i++) {
		if (!checkrow(image, i))
			continue;
		for (int j = col - 1; j <= col + 1; j++) {
			if (!checkcol(image, j))
				continue;
			if (i == row && j == col)
				continue;
			//printf("i: %d, j: %d, isalive: %d\n", i, j, isalive(image, i, j));
			if (isalive(image, i, j)) {
				num ++;
			}
		}
	}

	return num;
}

//Determines what color the cell at the given row/col should be. This function allocates space for a new Color.
//Note that you will need to read the eight neighbors of the cell in question. The grid "wraps", so we treat the top row as adjacent to the bottom row
//and the left column as adjacent to the right column.
Color *evaluateOneCell(Image *image, int row, int col, uint32_t rule)
{
	//YOUR CODE HERE
	Color *color = malloc(sizeof(Color));

	int alive = isalive(image, row, col);

	int neighb_alive_num = getalivenum(image, row, col);

	// alive = rule & (0x1 << (alive == 1 ? 9 : 0) + neighb_alive_num);

	int base = 0;
	if (alive == 1) {
		base = 9;
	}

	if (rule & (0x1 << base + neighb_alive_num)) {
		alive = 1;
	} else {
		alive = 0;
	}

	// printf("rule: %#x, row: %d, col: %d, neihhb: %d, alive: %d\n",rule, row, col, neighb_alive_num, alive);

	color->R = alive == 1 ? 255 : 0;
	color->G = alive == 1 ? 255 : 0;
	color->B = alive == 1 ? 255 : 0;

	return color;
}

//The main body of Life; given an image and a rule, computes one iteration of the Game of Life.
//You should be able to copy most of this from steganography.c
Image *life(Image *image, uint32_t rule)
{
	if (image == NULL) {
		printf("image is NULL\n");
		return NULL;
	}

	Image *new_image = malloc(sizeof(Image));
	new_image->rows = image->rows;
	new_image->cols = image->cols;

	Color **new_colors = malloc(sizeof(Color *) * new_image->rows);

	for (int i = 0; i < new_image->rows; i++) {
		new_colors[i] = malloc(sizeof(Color) * new_image->cols);
	}

	for (int i = 0; i < image->rows; i++) {
		for (int j = 0; j < image->cols; j++) {
			Color *color = evaluateOneCell(image, i, j, rule);
			// printf("R:%d G:%d\n", color->R, color->G);
			new_colors[i][j].R = color->R;
			new_colors[i][j].G = color->G;
			new_colors[i][j].B = color->B;
			free(color);
		}
	}

	new_image->image = new_colors;
	return new_image;
}

/*
Loads a .ppm from a file, computes the next iteration of the game of life, then prints to stdout the new image.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a .ppm.
argv[2] should contain a hexadecimal number (such as 0x1808). Note that this will be a string.
You may find the function strtol useful for this conversion.
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!

You may find it useful to copy the code from steganography.c, to start.
*/

void processCLI(int argc, char **argv, char **filename, uint32_t *rule)
{
	if (argc != 3) {
		printf("usage: %s filename\n",argv[0]);
		printf("filename is an ASCII PPM file (type P3) with maximum value 255.\n");
		exit(-1);
	}
	*filename = argv[1];
	*rule = strtol(argv[2], NULL, 16);
}

int main(int argc, char **argv)
{
	//YOUR CODE HERE
	Image *image;
	char *filename;
	uint32_t rule = 0;
	processCLI(argc,argv,&filename, &rule);
	image = readData(filename);
	writeData(image);
	printf("======== after switch ========\n");

	Image *new_image;
	new_image = life(image, rule);

	writeData(new_image);

	free(new_image);
	freeImage(image);
}
