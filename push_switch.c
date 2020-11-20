/* FPGA Push Switch Test Application
File : fpga_test_push.c
Auth : largest@huins.com */

#include "push_switch.h"

void user_signal1(int sig) 
{
	quit = 1;
}

void push_switch(void) // switch 입력 감지하는 함수
{
	int i;
	
	int buff_size;
	

	memset(switch_buffer,0,sizeof(switch_buffer));
	

	(void)signal(SIGINT, user_signal1); // device에 signal 보내기

	buff_size=sizeof(switch_buffer);


	read(switch_dev, &switch_buffer, buff_size);

	
	usleep(80000);
}
