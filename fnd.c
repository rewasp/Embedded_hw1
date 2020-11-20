#include "fnd.h"


void mode1(int x1, int x2){   // Only for time fnd outputs! 

    int data[4];
    char real_data[4];
	unsigned char retval;
	int i;
 
    real_data[0] = x1/10;
    real_data[1] = x1%10;
    real_data[2] = x2/10;
    real_data[3] = x2%10; // 시간 계산용


    printf("%d %d\n",data[2],data[3]);


    retval=write(fnd_dev,&real_data,4);	
    usleep(500);

	memset(real_data,0,4);

}
void mode2(int x1, int x2){   // for other FND outputs!
    
    char data[4];
	unsigned char retval;

    data[0] = x1 / 1000;
    x1 -= data[0] * 1000;

    data[1] = x1 / 100;
    x1 -= data[1] * 100;

    data[2] = x1 / 10;
    x1 -= data[2] * 10;

    data[3] = x1;


    retval=write(fnd_dev,&data,4);	
    usleep(500);

	memset(data,0,sizeof(data));

    
}

void fnd_initializer(void){ // 초기 0000상태

    int data[4];
	unsigned char retval;
	int i;

    memset(data,0,sizeof(data)); // 0000으로 초기화! 이상한 애들 좀 쳐내야 하긴 함

    retval=write(fnd_dev,&data,4);	

}