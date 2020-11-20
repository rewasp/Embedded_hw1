#ifndef FND_H
#define FND_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define MAX_DIGIT 4
#define FND_DEVICE "/dev/fpga_fnd"


extern int fnd_dev;
void fnd_initializer(void);
void mode1(int x1, int x2); // fnd output 기능을 위해서는 값 2개를 받는다(mode1의 경우에는 2개가 다 필요하고, 나머지 mode는 하나만 있으면 된다).
void mode2(int x1, int x2);
void mode3(int x1, int x2);
void mode4(int x1, int x2);
void mode5(int x1, int x2);

#endif