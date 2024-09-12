/************************************************************************
**
** NAME:        imageloader.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**              Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-15
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "imageloader.h"

//Opens a .ppm P3 image file, and constructs an Image object.
//You may find the function fscanf useful.
//Make sure that you close the file with fclose before returning.
Image *readData(char *filename)
{
	if (filename == NULL) {
		return NULL;
	}
	//YOUR CODE HERE
	FILE *fp = fopen(filename, "r");

	if (fp == NULL) {
		return NULL;
	}

	char buf[20];
	if (fscanf(fp, "%s", buf) != EOF) {
		//printf("read title: %s \r\n", buf);
	} else {
		return NULL;
	}

	int row, col;
	if (fscanf(fp, "%d %d", &col, &row) != EOF) {
		//printf("read row: %d, col: %d\r\n", row, col);
	} else {
		return NULL;
	}

	int pixel_size;
	if (fscanf(fp, "%d", &pixel_size) != EOF) {
		//printf("read pixel_size: %d\r\n", pixel_size);
	} else {
		return NULL;
	}

	Color **image = malloc(sizeof(Color *) * row);

	for (int i = 0; i < row; i++) {
		image[i] = malloc(sizeof(Color) * col);
	}

	int r, g, b;
	int i = 0;
	while (fscanf(fp, "%d %d %d", &r, &g, &b) != EOF) {
		int tmp_row = i / col;
		int tmp_col = i % col;
		//printf("i: %d tmp_row: %d, tmp_col: %d\r\n", i, tmp_row, tmp_col);
		image[tmp_row][tmp_col].R = r;
		image[tmp_row][tmp_col].G = g;
		image[tmp_row][tmp_col].B = b;
		i++;
	}

	fclose(fp);

	Image *img = malloc(sizeof(Image));
	img->image = image;
	img->cols = col;
	img->rows = row;

	return img;
}

//Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the image's data.
void writeData(Image *image)
{
	//YOUR CODE HERE
	if (image == NULL) {
		return;
	}

	printf("P3\n");
	printf("%d %d\n", image->cols, image->rows);
	printf("255\n");

	for (int i = 0; i < image->rows; i++) {
		for (int j = 0; j < image->cols; j++) {
			Color *tmp = &image->image[i][j];
			if (j == 0)
				printf("%3d %3d %3d", tmp->R, tmp->G, tmp->B);
			else
				printf("   %3d %3d %3d", tmp->R, tmp->G, tmp->B);

		}
		printf("\n");
	}
}

//Frees an image
void freeImage(Image *image)
{
	//YOUR CODE HERE
	for (int i = 0; i < image->rows; i++) {
		free(image->image[i]);
	}

	free(image->image);

	free(image);
}