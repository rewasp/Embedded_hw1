/* FPGA DotMatirx Test Application
File : fpga_test_dot.c
Auth : largest@huins.com */
#include <stdbool.h>
#include <time.h>

#include "dot.h"

static time_t start;
static bool chk = false;


unsigned char fpga_number[11][10] = {
	{0x3e,0x7f,0x63,0x73,0x73,0x6f,0x67,0x63,0x7f,0x3e}, // 0
	{0x0c,0x1c,0x1c,0x0c,0x0c,0x0c,0x0c,0x0c,0x0c,0x1e}, // 1
	{0x7e,0x7f,0x03,0x03,0x3f,0x7e,0x60,0x60,0x7f,0x7f}, // 2
	{0xfe,0x7f,0x03,0x03,0x7f,0x7f,0x03,0x03,0x7f,0x7e}, // 3
	{0x66,0x66,0x66,0x66,0x66,0x66,0x7f,0x7f,0x06,0x06}, // 4
	{0x7f,0x7f,0x60,0x60,0x7e,0x7f,0x03,0x03,0x7f,0x7e}, // 5
	{0x60,0x60,0x60,0x60,0x7e,0x7f,0x63,0x63,0x7f,0x3e}, // 6
	{0x7f,0x7f,0x63,0x63,0x03,0x03,0x03,0x03,0x03,0x03}, // 7
	{0x3e,0x7f,0x63,0x63,0x7f,0x7f,0x63,0x63,0x7f,0x3e}, // 8
	{0x3e,0x7f,0x63,0x63,0x7f,0x3f,0x03,0x03,0x03,0x03}, // 9
	{0x1c,0x36,0x63,0x63,0x63,0x7f,0x7f,0x63,0x63,0x63}   // for drawing A, A is considered as no.10
};

unsigned char fpga_set_full[10] = {
	// memset(array,0x7e,sizeof(array));
	0x7f,0x7f,0x7f,0x7f,0x7f,0x7f,0x7f,0x7f,0x7f,0x7f
};

unsigned char fpga_set_blank[10] = {
	// memset(array,0x00,sizeof(array));
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

unsigned char draw_board[10] = {   // to draw and overwrite!
	// memset(array,0x00,sizeof(array));
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

unsigned char draw_static[10] = {   // to draw and overwrite!
	// memset(array,0x00,sizeof(array));
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};


void dot_matrix(int x)
{
	int i;
	
	int str_size;
	int set_num;
	
	set_num = x;

	str_size=sizeof(fpga_number[set_num]);

    /*
	write(dev,fpga_set_full,sizeof(fpga_set_full));
	sleep(1);

	write(dev,fpga_set_blank,sizeof(fpga_set_blank));
	sleep(1);
    */

	write(dot_dev,fpga_number[set_num],str_size);	F
	
}

void dot_matrix_blank(){

	int i;
	
	int str_size;

	str_size=sizeof(fpga_set_blank);
	memcpy(draw_board,fpga_set_blank,sizeof(draw_board));
    /*
	write(dev,fpga_set_full,sizeof(fpga_set_full));
	sleep(1);

	write(dev,fpga_set_blank,sizeof(fpga_set_blank));
	sleep(1);
    */

	write(dot_dev,draw_board,str_size);	
}

void dot_matrix_draw(int position, int blink, int confirmed) {
	clock_t End;
	float exe_time;
	int i;
	int str_size;
	int row, col;

	row = position / 7;
	col = position % 7; // get the right position from the position parameter !

	if (col == 0) col = 64;
	else if (col == 1) col = 32;
	else if (col == 2) col = 16;
	else if (col == 3) col = 8;
	else if (col == 4) col = 4;
	else if (col == 5) col = 2;
	else if (col == 6) col = 1;

	str_size = sizeof(draw_board);

	printf("%s \n",draw_board);

	memcpy(draw_static, draw_board, str_size); // imsi jeojangso

	
	if (blink == 1) { // blinking part
			End = clock ();
			exe_time = (float)(End -start) / (float)CLOCKS_PER_SEC;

			if (chk && exe_time > 0.001f) {

				draw_board[row] ^= col | draw_board[row]; // only blink in current position!
				write(dot_dev, draw_board, str_size);
				chk = false;
				start = clock ();
			} 
			
			else if (!chk && exe_time > 0.001f)
			{	
				draw_board[row] ^= col | draw_board[row];
				write(dot_dev, draw_board, str_size);
				chk = true;
				start = clock ();
			}
	}
	else{
		start = clock();
		chk = false;
	}
	memcpy(draw_board, draw_static, str_size);
	printf("draw confirm is    %d \n", confirmed);

	if (confirmed == 1) {   // real draw part
		draw_board[row] |= col;
		write(dot_dev, draw_board, str_size);		
		start = clock();
	}
	usleep(300000);
	write(dot_dev, draw_board, str_size);

}

void dot_matrix_reverse() { // reverse the current dot matrix

	int i;

	for (i = 0; i < 10; i++) {
		draw_board[i] ^= 0xff; // xor with 0xff
	}
	
	write(dot_dev, draw_board, sizeof(draw_board));
}