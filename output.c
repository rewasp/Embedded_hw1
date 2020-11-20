#include "output.h"


void output_handler(int mode){ // message queue 받아오기 필요
    int misc;

    pid_t pid = getppid();
    if(pid ==1) exit(0);

    switch(mode){

        case 1:{ // 여기서 device open을 해야하나?? 어차피 그건 공통적으로 다 하니까..
            
            mq_receive(output_qd, (char *)&hour, attr.mq_msgsize, 0); // 시간 받아오기 
            mq_receive(output_qd, (char *)&min, attr.mq_msgsize, 0); // 분 받아오기

            mode1(hour,min);
            
            
        }
        
        break;

        case 2:{
            mq_receive(output_qd, (char *)&num_2, attr.mq_msgsize, 0);
            printf("output process received num: %d\n",num_2);
            mode2(num_2,0); // print fnd
        }
        break;

        case 3:{
            
            mq_receive(string_qd, buf , sizeof(buf), 0);   /////////// get string from the main
            mq_receive(output_qd, (char *)&misc, attr.mq_msgsize, 0); // get fnd_count from main 
            printf("mode 3 buffer is ....%s\n",buf);
            text_lcd(buf);
            printf("successfully written to text lcd\n");

            mode2(misc, 0); // print fnd

        }
        break;

        case 4:{

			mq_receive(output_qd, (char *)&dot_po, attr.mq_msgsize, 0);
			mq_receive(output_qd, (char *)&dot_blink, attr.mq_msgsize, 0);
            usleep(4000);
            mq_receive(output_qd, (char *)&confirm, attr.mq_msgsize, 0);

            

			dot_matrix_draw(dot_po, dot_blink, confirm); // draw dot matrix on drawboard

			if (reverse_flag == 1) {
                printf("reverse flag!!!!!!\n");
				dot_matrix_reverse();
				reverse_flag = 0;
				
			}

        }
        break;

        case 5:{

            mq_receive(output_qd, (char*)&score, attr.mq_msgsize, 0);

			dot_matrix(score); // 점수 dot matrix로 표시(깜빡이)
			sleep(2);
			dot_matrix_blank();
			sleep(2);
			dot_matrix(score);
			sleep(2);
			dot_matrix_blank();
			sleep(2);
			dot_matrix(score);


			sleep(3); // 3초 후에 새로운 게임 시작

            
        }
        break;

        default: break;
    }


}