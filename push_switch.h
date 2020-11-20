#ifndef PUSH_SWITCH
#define PUSH_SWITCH

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/stat.h> 
#include <fcntl.h> 
#include <sys/ioctl.h>
#include<sys/select.h>
#include <signal.h>
#include<time.h>
#include "headers.h"


#define MAX_BUTTON 9

void push_switch(void);
void user_signal1(int sig);
unsigned char quit;
unsigned char switch_buffer[MAX_BUTTON];
struct timeval timeout; // to delay_read the switch input
int rv;
extern int switch_dev;
fd_set set;

#endif