#ifndef TEXT_LCD
#define TEXT_LCD
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#define MAX_BUFF 32 // maximum output range!
#define LINE_BUFF 16
#define FPGA_TEXT_LCD_DEVICE "/dev/fpga_text_lcd"


void text_initialize();
void text_lcd(const char *);
extern int lcd_dev;


#endif