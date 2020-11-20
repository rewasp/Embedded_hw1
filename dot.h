/* This file is FPGA-DOT MATRIX font file.
	FILE : fpga_dot_font.h
	AUTH : Hong, Sung-Hyun
		Huins, Inc */
		
#ifndef __FPGA_NUMBER__
#define __FPGA_NUMBER__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "headers.h"
#define FPGA_DOT_DEVICE "/dev/fpga_dot"

void dot_matrix_blank();
void dot_matrix_draw(int, int, int);
void dot_matrix_reverse();
void dot_matrix(int x);
int dot_dev;



#endif