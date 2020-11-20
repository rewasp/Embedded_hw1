#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/types.h>
#include "input.h"
#include "output.h"
#include "headers.h"

#define MSG_SIZE 4096

void handler(int sig_num){
    printf("Received sig %d\n", sig_num);
}
///////////// when changing the mode, reset everything! ////////////


void overall_initializer() {
    int t;
    fnd_count = 0;
	fnd_initializer();
    text_initialize();
    dot_matrix_reset();
	led_mmap(0);
	reverse_flag = 0;
	lcd_position = 0;
	text_mode = 0;   // mode 3 start with an alphabet input
	memset(in_move, 0, 9);
}   


int main(void){ 

    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 4; // message queue 기본 attribute 설정
    attr2.mq_maxmsg = 10;
    attr2.mq_msgsize = MAX_BUFF;
    int readkey_ret; // readkey에서 읽어온 값 ret 해주기 용
    int mode_input=1, mode_output=1, mode_main=1;
    int hour, min; // mode 1용 
    int blink_flag;
    int t;
    int readkey;
    int num_val; // mode 2용(진법 계산 끝나고 메인 프로세스로 넘어온 녀석)
	int dot_po_main, dot_blink_main;  // dot poistion and blink mode flag for main
    int extra; // for 추가구현

    inmode_qd = mq_open("/mq_inmode", O_RDWR | O_CREAT   , 0666, &attr); // message queue, only for mode change
    if(input_qd == -1){
        perror("open error");
        exit(0);
    }

    outmode_qd = mq_open("/mq_outmode", O_RDWR | O_CREAT   , 0666, &attr);
     // message queue, only for mode changes
    if(input_qd == -1){
        perror("open error");
        exit(0);
    }

    string_qd = mq_open("/mq_string", O_RDWR | O_CREAT   , 0666, &attr2); // for mode3.. string message queue!
    if(string_qd == -1){
        perror("open error");
        exit(0);
    }

    input_qd = mq_open("/mq_input", O_RDWR | O_CREAT   , 0666, &attr); // input용 message queue
    if(input_qd == -1){
        perror("open error");
        exit(0);
    }

    output_qd = mq_open("/mq_output", O_RDWR | O_CREAT  , 0666, &attr); // output용 message queue
    if(output_qd == -1){
        perror("open error");
        exit(0);
    }
    readkey_qd = mq_open("/mq_readkey", O_RDWR | O_CREAT   , 0666, &attr); // readkey 용 message queue
    if(readkey_qd == -1){
        perror("open error");
        exit(0);
    }


    if((fd_readkey = open (FPGA_READKEY_DEVICE, O_RDONLY)) == -1) {
		printf ("%s is not a vaild device.n", FPGA_READKEY_DEVICE);
	}
	fcntl(fd_readkey, F_SETFL, O_NONBLOCK);

    switch_dev = open("/dev/fpga_push_switch", O_RDWR); // switch device open miri
	
	if(switch_dev<0){
		perror("switch device open error\n");
		close(switch_dev);
	}

    lcd_dev = open(FPGA_TEXT_LCD_DEVICE, O_WRONLY); // open text_lcd device
	if (lcd_dev<0) {
		printf("Device open error : %s\n",FPGA_TEXT_LCD_DEVICE);
		return -1;
	}

    dot_dev = open(FPGA_DOT_DEVICE, O_WRONLY);
	if (dot_dev<0) {
		printf("Device open error : %s\n",FPGA_DOT_DEVICE);
		exit(1);
	}

    fnd_dev = open(FND_DEVICE, O_RDWR);
    if (fnd_dev<0) {
        printf("Device open error : %s\n",FND_DEVICE);
        exit(1);
    }



    pid = fork(); // input process를 위한 fork

    if(pid == 0){ // input process 수행을 위한 함수 호출

        while(1){

        to_kill[0] = getpid(); // later.. to use sigkill in parent process..

        readkey_ret = mode_input;
        readkey_ret = get_readkey(fd_readkey); // readkey용 file descriptor를 넘겨준다. 


        mq_send(readkey_qd, (char*)&readkey_ret, attr.mq_msgsize, 0); // readkey용 message queue에 현재 받아온 값 넘겨주기
        

        mq_receive(inmode_qd, (char *)&mode_input, attr.mq_msgsize, 0);

            printf("intput mode : %d\n", mode_input);

            input_handler(mode_input); // input handler로 변경된 mode 값을 넘겨준다.
            
        }
    }

    pid = fork(); // output process를 위한 fork

    if(pid == 0){ // output process를 위한 함수 호출
       
        while(1){


        to_kill[1] = getpid(); // later.. to use sigkill in parent process..

        
        //t = mode;
        mq_receive(outmode_qd, (char *)&mode_output, attr.mq_msgsize, 0);

        printf("output mode : %d\n",mode_output);

        output_handler(mode_output); // mode 1; output process의 default는 clock mode(1)이다.


        }
    }
                
    while(1){ // back 을 받기 전까지는 계속 프로세스들을 유지!(여기서 in, out mq 관리하며.. )
        to_kill[2] = getpid(); // get main's pid

        mq_receive(readkey_qd, (char *)&readkey, attr.mq_msgsize,0); // readkey queue에서 값을 읽어온다. 

            if(readkey == 158){ // process 종료
                printf("exiting main process...\n");
                mq_close(input_qd);
                mq_close(output_qd);
                mq_close(readkey_qd);
                mq_close(inmode_qd);
                mq_close(outmode_qd);
                mq_unlink("/mq_input");
                mq_unlink("/mq_output"); // message queue 닫기
                mq_unlink("/mq_readkey");
                mq_unlink("/mq_inmode");
                mq_unlink("/mq_outmode");
                led_mmap(0);
                fnd_initializer();
                dot_matrix_blank();
                text_initialize();
                kill(to_kill[0], SIGKILL);
                kill(to_kill[1], SIGKILL);
                exit(0);
            }
            else if(readkey == 114){ // mode down
                printf("mode_main down\n"); // for debugging..
                mode_main--;
                if(mode_main<1){
                     mode_main = 5; // mode 순환을 위해서

                }
                overall_initializer();

                
            }

            else if(readkey == 115){ // mode up
                printf("mode_main up\n"); // for debugging..

                mode_main++;
                
                if(mode_main>5){
                     mode_main = 1; //mode 순환을 위해서
                }
                overall_initializer();

            }
            readkey_ret = -1; // reset the readkey!

            
		
        mq_send(inmode_qd, (char *)&mode_main, attr.mq_msgsize, 0); // input process용 message queue에 readkey에서 읽어온 현재 mode 담기

        mq_send(outmode_qd, (char *)&mode_main, attr.mq_msgsize, 0); // output process에게 현재 mode 알려주기
        

        printf("main proc mode is %d\n",mode_main);

        switch(mode_main){ // mode 별로 message queue에서 받고 보내는게 달라져야 한다. 
            
            case 1:{
                
                mq_receive(input_qd, (char *)&hour, attr.mq_msgsize, 0); // 시간 받아오기 
                mq_receive(input_qd, (char *)&min, attr.mq_msgsize, 0); // 분 받아오기

                mq_send(output_qd, (char *)&hour, attr.mq_msgsize, 0); //output process로 현재 시간 보내주기
                mq_send(output_qd, (char *)&min, attr.mq_msgsize, 0); //output process로 현재 분 보내주기


                break;
            }

            case 2:{
                
                mq_receive(input_qd, (char *)&num_val, attr.mq_msgsize, 0); // numeral value 받아오기

                printf("main process received num: %d\n",num_val);

                mq_send(output_qd, (char *)&num_val, attr.mq_msgsize, 0); // numeral value output 프로세스로 넘겨주기


                break;
            }

            case 3:{
                
                mq_receive(string_qd, lcd_output, sizeof(lcd_output), 0); // read only 32 byte for this case.. since it is passing the  whole string
                mq_receive(input_qd, (char *)&extra, attr.mq_msgsize, 0); // get fnd_count from input process!
                printf("main process got %s from mode3 input\n",lcd_output);
                mq_send(string_qd, lcd_output, sizeof(lcd_output), 0); // read only one byte for this case.. since it is passing the char
                mq_send(output_qd, (char *)&extra, attr.mq_msgsize, 0); // send fnd_count to output process!
                break;
            }

            case 4:{
                

				mq_receive(input_qd, (char *)&dot_po_main, attr.mq_msgsize, 0);
				mq_receive(input_qd, (char *)&dot_blink_main, attr.mq_msgsize, 0); // receiving order is same as the order sent from the input process
                usleep(4000);
                mq_receive(input_qd, (char *)&extra, attr.mq_msgsize, 0);
                

                usleep(4000);
				mq_send(output_qd, (char*)&dot_po_main, attr.mq_msgsize, 0);
				mq_send(output_qd, (char*)&dot_blink_main, attr.mq_msgsize, 0);
                usleep(4000);
                mq_send(output_qd, (char *)&extra, attr.mq_msgsize, 0);
                
                break;
            }

            case 5:{
                ///// 추가 구현;  9자리 숫자 기억하기 ///////
				mq_receive(input_qd, (char *)&extra, attr.mq_msgsize, 0);

				mq_send(output_qd, (char*)& extra, attr.mq_msgsize, 0); // input process에서 받은 점수 output process로 넘겨주기!

                break;
            }

            default:{
                break;
            }
        }

       
            usleep(10000);
    }


                mq_close(input_qd);
                mq_close(output_qd);
                mq_close(readkey_qd);
                mq_close(inmode_qd);
                mq_close(outmode_qd);
                mq_unlink("/mq_input");
                mq_unlink("/mq_output"); // message queue 닫기
                mq_unlink("/mq_readkey");
                mq_unlink("/mq_inmode");
                mq_unlink("/mq_outmode");
    return 0;
}