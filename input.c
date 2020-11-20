#include "input.h"

#define BUFF_SIZE 64
#define KEY_RELEASE 0
#define KEY_PRESS 1

int fnd_count = 0;
// dk gksrmf dho dksskdhkdk

int to_decimal(int sys){ // 10진법으로 변환!
    int num=0;
    numeral_tmp = numeral;
    
    num += (numeral_tmp % 10); // 1의자리 완성
    numeral_tmp /= 10;
    num += (numeral_tmp % 10) * sys;
    numeral_tmp /= 10;
    num += (numeral_tmp % 10) * sys * sys;

    return num;
}

void dot_matrix_reset() {

	dot_position = 0;
	blink_mode = 1;
	reverse_flag = 0;

	dot_matrix_blank(); // remove everything!

}

int get_readkey(int fd){

    struct input_event ev[BUFF_SIZE];
	int rd, value, size = sizeof (struct input_event);
    int code=0; // readkey 에서 넘어오는 값을 받아주기 위한것
	// ioctl (fd, EVIOCGNAME (sizeof (name)), name);
	// printf ("Reading From : %s (%s)n", device, name);
        
        

		rd = read (fd, ev, size * BUFF_SIZE);

		if (!(rd < size)) {
			
			if (ev[0].value == 1) { // only when pressed!
				printf("successfully passing readkey\n");
				printf("code: %d\n", code);
				return ev[0].code;
			}

		}
}

void time_change(){ // mode1의 시간 변경용 함수
    int q;

    while(1){
        
        led_mmap(32);
        //memset(switch_buffer, 0, sizeof(switch_buffer)); // switch buffer 초기화
        //strncpy(switch_buffer, push_switch(), sizeof(switch_buffer)); // updated line instad of upper line
        push_switch();


        printf("\n");

        if( clock_gettime(CLOCK_REALTIME, &time_clock) == -1 ){ // 실제 시간 받아오기
                perror("time error");
                exit(0);
            }

            localtime_r((time_t *)&time_clock.tv_sec, &tm_cur); // struct tm 형태로 변환(읽어오기 편하도록)
            hour = tm_cur.tm_hour;
            min = tm_cur.tm_min;

        
        if(switch_buffer[0]==1){
            printf("changed time saved\n");
            break; // save and exit from infinite loop 
        } 

        if(switch_buffer[1]==1){
            time_save_flag = 0; // don't save the modified time!
            printf("time reset\n");
            break; // reset!!
        }


        if(switch_buffer[2] ==1){ // 시간을 1 증가 시키기
            if(clock_gettime(CLOCK_REALTIME, &time_clock) == -1){
                perror("time error");
                exit(0);
            }
            tm_cur.tm_hour += 1; 
            time_clock.tv_sec = mktime(&tm_cur); // 증가된 시간 반영  !!!!!!!!!!!!!!!!!!!!!!! time -> tm_cur

            clock_settime(CLOCK_REALTIME, &time_clock);

            hour = tm_cur.tm_hour;
            min = tm_cur.tm_min;
            mode1(hour, min);
            printf("current in change...\n");
        }

        if(switch_buffer[3] ==1){
            if(clock_gettime(CLOCK_REALTIME, &time_clock) == -1){
                perror("time error");
                exit(0);
            }
            tm_cur.tm_min += 1;
            time_clock.tv_sec = mktime(&tm_cur); // 증가된 시간 반영 !!!!!!!!!!!!!!!!!!!!!!! time -> tm_cur
            
            clock_settime(CLOCK_REALTIME, &time_clock);

            hour = tm_cur.tm_hour;
            min = tm_cur.tm_min;
            mode1(hour, min);
            printf("current in change...\n");

        }

        led_mmap(16);
        
    }
    

}

void input_handler(int mode){
    pid_t pid = getppid();
    if(pid ==1) exit(0);

	

    switch(mode){

    case 1:{
           // printf("Trying to call mode1_input func..\n");
           led_mmap(128);
           mode1_input(); // mode1 함수 call
           
    }
    break;

    case 2:{
            
            mode2_input(); // mode2 함수 call
            
    }
    break;

    case 3:{
            
            mode3_input();
            
    }
    break;

    case 4:{
		
            mode4_input();
            
    }
    break;

    case 5:{

            mode5_input();
            
    }
    break;

    default: break;
    }
}

void mode1_input(){

            printf("Mode 1 currently running\n");
            int blink = 0;

            if(clock_gettime(CLOCK_REALTIME, &time_clock) == -1){ // 실제 시간 받아오기
                perror("time error");
                exit(0);
            }

            localtime_r((time_t *)&time_clock.tv_sec, &tm_cur); // struct tm 형태로 변환(읽어오기 편하도록)
            localtime_r((time_t *)&time_clock.tv_sec, &tm_static);
            hour = tm_cur.tm_hour;
            min = tm_cur.tm_min;

            /* Debugging code */
            printf("hour: %d\n",hour);
            printf("min: %d\n",min);

            printf("\n"); // for debugging

            push_switch();

	            for(i=0;i<9;i++){
                printf("[%d] ",switch_buffer[i]); // for debugging..
                }
                if(switch_buffer[0]==1){ //time change
                    printf("switch no.1 pushed\n");
                    time_save_flag = 1;
                    //blink = 1;
                   // mq_send(blink_qd, (char *)&blink, attr.mq_msgsize, 0); // output process blink!

                    time_change();
                   // blink = 0;
                   // mq_send(blink_qd, (char *)&blink, attr.mq_msgsize, 0); // output process blink!
                    
                    led_mmap(128); // only light the 1st one
                    if(time_save_flag ==0){
                        hour = tm_static.tm_hour;
                        min = tm_static.tm_min;
                        time_clock.tv_sec = mktime(&tm_static); 
                        clock_settime(CLOCK_REALTIME, &time_clock); // return to original time_ before time change..
                    }
                }

                //memset(switch_buffer, 0, sizeof(switch_buffer)); // reset the switch buffer data

                mq_send(input_qd, (char *)&hour, attr.mq_msgsize, 0);
                mq_send(input_qd, (char *)&min, attr.mq_msgsize, 0);

}

void mode2_input(){
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 4; // message queue 기본 attribute 설정
    //memset(switch_buffer, 0, sizeof(switch_buffer)); // switch buffer 초기화
    
        push_switch();
		
        
        if(switch_buffer[0]==1){ // 진법 변환
            numeral_index++;
            n = 1;
        
            if(numeral_index>3){
                    numeral_index = 0; // 0, 1, 2, 3, 0, 1, 2, 3....
                    numeral = to_decimal(numeral_sys[3]); // 2진법 -> 10진법 변환
                    if(numeral>1000) numeral = 0;
            }

                if(numeral_index==1){ // 10 -> 8진법
                    
                     numeral_count = 0;
                    // 이제 8진법으로 변환
                    numeral_tmp = numeral;
                    numeral = 0;
                    while(1){
						 numeral_count++;
						if (numeral_count > 3) break;
                        numeral += ((numeral_tmp % 8) * n);
                        numeral_adding[3-numeral_count] = (numeral_tmp % 8);
                        printf("numeral adding!!!  %d", numeral_adding[3-numeral_count]);
                        numeral_tmp /= 8;
                        n *= 10;
						
                    }
                    //numeral += (numeral_tmp * n);
                    n = 1;
                }
            
                else if(numeral_index ==2){ // 8 -> 4진법
                    numeral = to_decimal(numeral_sys[numeral_index-1]); // 일단 10진법으로 변환
                    numeral_count = 0;
                    // 이제 4진법으로 변환
                    numeral_tmp = numeral;
                    numeral = 0;
                    while(1){
						 numeral_count++;
						if (numeral_count > 3) break;
                        numeral +=  ((numeral_tmp % 4) * n);
                        numeral_adding[3-numeral_count] = (numeral_tmp % 4);
                        printf("numeral adding!!!  %d", numeral_adding[3-numeral_count]);
                        numeral_tmp /= 4;
                        n *= 10;
						
                    }
                    //numeral += (numeral_tmp * n);
                    n = 1;
                }
                
                else if(numeral_index ==3){ // 4진법 -> 2진법
                    numeral = to_decimal(numeral_sys[numeral_index-1]); // 일단 10진법으로 변환
                    numeral_count = 0;
                    // 이제 2진법으로 변환
                    numeral_tmp = numeral;
                    numeral = 0;
                    while(1){ 
						numeral_count++;
						if (numeral_count > 3) break;
                        numeral += ((numeral_tmp % 2) * n);
                        numeral_adding[3-numeral_count] = (numeral_tmp % 2);
                        printf("numeral adding!!! %d ", numeral_adding[3-numeral_count]);
                        numeral_tmp /= 2;
                        n *= 10;
                    }
                    //numeral += (numeral_tmp * n);
                    n = 1;

                }
            }
        
        if(numeral_adding[0] ==0 && numeral_adding[1] ==0 && numeral_adding[2] ==0){
            numeral_adding[0] = 1;
            numeral_adding[1] = 1;
            numeral_adding[2] = 1;
        }
        

        else if(switch_buffer[1]==1){
            numeral += 100; // 100의 자리 숫자를 1 증가

            if(numeral_sys[numeral_index] == 8){
                if(numeral/100 == 8)
                    numeral -= 8 * 100;

            }

            else if(numeral_sys[numeral_index] == 4){

                 if(numeral/100 == 4)
                    numeral -= 4 * 100;

            }

            else if(numeral_sys[numeral_index] == 2){
                if(numeral/100 == 2)
                    numeral -= 2 * 100;

            }
            
        }

        else if(switch_buffer[2]==1){
            numeral += 10; // 10의 자리 숫자를 1 증가

            if(numeral_sys[numeral_index] == 8){
                
                if( (numeral - (numeral/100)) / 10 == 8 ) {
                    numeral -= (numeral - (numeral/100))/10 * 10;

                    if(numeral/100 == 7) numeral -= 8 * 100;
                    else numeral += 100;
                }


            }

            else if(numeral_sys[numeral_index] == 4){

                
                if( (numeral - (numeral/100))/10 == 4 ) {
                    numeral -= (numeral - (numeral/100))/10 * 10;

                    if(numeral/100 == 3) numeral -= 4 * 100;
                    else numeral += 100;
                }
                    
            }

            else if(numeral_sys[numeral_index] == 2){
                    
                    if( (numeral - (numeral/100))/10 == 2 ) {
                    numeral -= (numeral - (numeral/100))/10 * 10;

                    if(numeral/100 == 1) numeral -= 2 * 100;
                    else numeral += 100;
                }
             
        }
        }


        else if(switch_buffer[3]==1){
            numeral += 1; // 1의 자리 숫자를 1 증가

            if(numeral_sys[numeral_index] == 8){
                
                if(numeral - ((numeral - (numeral/100)) / 10) == 8){

                    numeral -= numeral - ((numeral - (numeral/100)) / 10);

                    if((numeral - (numeral/100)) / 10 == 7){
                        numeral -= (numeral - (numeral/100))/10 * 10;

                        if(numeral/100 == 7){
                            numeral -= 7 * 100;

                        }
                        else numeral +=100;

                    }

                    else numeral += 10;
                }


            }

            else if(numeral_sys[numeral_index] == 4){

                    if(numeral - ((numeral - (numeral/100)) / 10) == 4){

                    numeral -= numeral - ((numeral - (numeral/100)) / 10);

                    if((numeral - (numeral/100)) / 10 == 3){
                        numeral -= (numeral - (numeral/100))/10 * 10;

                        if(numeral/100 == 3){
                            numeral -= 3 * 100;

                        }
                        else numeral +=100;

                    }

                    else numeral += 10;
                }

            }

            else if(numeral_sys[numeral_index] == 2){

                   if(numeral - ((numeral - (numeral/100)) / 10) == 2){

                    numeral -= numeral - ((numeral - (numeral/100)) / 10);

                    if((numeral - (numeral/100)) / 10 == 1){
                        numeral -= (numeral - (numeral/100))/10 * 10;

                        if(numeral/100 == 1){
                            numeral -= 1 * 100;

                        }
                        else numeral +=100;

                    }

                    else numeral += 10;
                }
        }
        }

        if(numeral_index==0) led_mmap(64); // 10진수일 때는 2번 led에 불이 켜진다
        else if(numeral_index==1) led_mmap(32); // 8진수일 때는 3번 led에 불이 켜진다
        else if(numeral_index==2) led_mmap(16); // 4진수일 때는 4번 led에 불이 켜진다
        else if(numeral_index==3) led_mmap(128); // 2진수일 때는 1번 led에 불이 켜진다

        if(numeral>999) numeral -= 1000; // 만약에 세 자리를 넘어간다면, 천의 자리는 단순한 버림으로 처리 
        numeral_tmp = 0;
        
        mq_send(input_qd, (char *)&numeral, attr.mq_msgsize, 0); // 계산이 끝난 값을 input queue를 통해 main process로 넘겨주자!

}

void mode3_input(){
    led_mmap(0);
    char whitespace = ' ';
    char tempo;
    int z;

    if(lcd_position>=MAX_BUFF){ // did it go over?
        for(i=0;i<lcd_position-1;i++){
            lcd_output[i] = lcd_output[i+1];
        }
        lcd_position = MAX_BUFF-1;
    } // if buffer overflows... remove the first character in the string
    i = 0;


    push_switch(); // get switch input

    for(z=0;z<9;z++){
        printf("[%d] ",switch_buffer[z]);
    }
    printf("\n");
    if(switch_buffer[1]==1 && switch_buffer[2]==1){ // text LCD clear!
            memset(lcd_output,' ', MAX_BUFF);
            lcd_position = 0;
            fnd_count+=2;
    }
    else if(switch_buffer[4] == 1 && switch_buffer[5] == 1){ // numeric to alphabet, or vice versa
            text_mode ^= 1; // change text_mode
			fnd_count+=2;
    }
    else if(switch_buffer[7] == 1 && switch_buffer[8] == 1){ // give one blank
            lcd_output[lcd_position] = whitespace;
            lcd_position ++;
            for(z=0;z<9;z++) in_move[z] = 0; // since button mode has changed.. reset

            fnd_count+=2; // increment two! ; since two buttons are simultaenously pressed..
    }


    if(text_mode ==0){ // alphabet input

        if(switch_buffer[0] ==1){

           lcd_output[lcd_position] = button[0][in_move[0]];
            in_move[0]++;
            if(in_move[0]>2) in_move[0] = 0;
            lcd_position ++;
            fnd_count++;

            for(z=0;z<9;z++){ // if button is changed, reset the previous button count!
                if(in_move[z]!=0 && z!=0) in_move[z]=0;
            }
    }

    else if(switch_buffer[1] ==1 && switch_buffer[2] !=1 ){
        lcd_output[lcd_position] = button[1][in_move[1]];
            in_move[1]++;
            if(in_move[1]>2) in_move[1] = 0;
            lcd_position ++;
            fnd_count++;

            for(z=0;z<9;z++){ // if button is changed, reset the previous button count!
                if(in_move[z]!=0 && z!=1) in_move[z]=0;
            }
    }

    else if(switch_buffer[2] ==1 && switch_buffer[1] !=1){
        lcd_output[lcd_position] = button[2][in_move[2]];
            in_move[2]++;
            if(in_move[2]>2) in_move[2] = 0;
            lcd_position ++;
            fnd_count++;

            for(z=0;z<9;z++){ // if button is changed, reset the previous button count!
                if(in_move[z]!=0 && z!=2) in_move[z]=0;
            }
    }

    else if(switch_buffer[3] ==1){
        lcd_output[lcd_position] = button[3][in_move[3]];
            in_move[3]++;
            if(in_move[3]>2) in_move[3] = 0;
            lcd_position ++;
            fnd_count++;

            for(z=0;z<9;z++){ // if button is changed, reset the previous button count!
                if(in_move[z]!=0 && z!=3) in_move[z]=0;
            }
    }

    else if(switch_buffer[4] ==1 && switch_buffer[5] !=1){
       lcd_output[lcd_position] = button[4][in_move[4]];
            in_move[4]++;
            if(in_move[4]>2) in_move[4] = 0;
            lcd_position ++;
            fnd_count++;

            for(z=0;z<9;z++){ // if button is changed, reset the previous button count!
                if(in_move[z]!=0 && z!=4) in_move[z]=0;
            }
    }

    else if(switch_buffer[5] ==1 && switch_buffer[4] !=1){
       lcd_output[lcd_position] = button[5][in_move[5]];
            in_move[5]++;
            if(in_move[5]>2) in_move[5] = 0;
            lcd_position ++;
            fnd_count++;

            for(z=0;z<9;z++){ // if button is changed, reset the previous button count!
                if(in_move[z]!=0 && z!=5) in_move[z]=0;
            }
    }

    else if(switch_buffer[6] ==1){
     lcd_output[lcd_position] = button[6][in_move[6]];
            in_move[6]++;
            if(in_move[6]>2) in_move[6] = 0;
            lcd_position ++;
            fnd_count++;

            for(z=0;z<9;z++){ // if button is changed, reset the previous button count!
                if(in_move[z]!=0 && z!=6) in_move[z]=0;
            }
    }

    else if(switch_buffer[7] ==1 && switch_buffer[8] !=1){
      lcd_output[lcd_position] = button[7][in_move[7]];
            in_move[7]++;
            if(in_move[7]>2) in_move[7] = 0;
            lcd_position ++;
            fnd_count++;

            for(z=0;z<9;z++){ // if button is changed, reset the previous button count!
                if(in_move[z]!=0 && z!=7) in_move[z]=0;
            }
    }

    else if(switch_buffer[8] ==1 && switch_buffer[7] !=1){     
      lcd_output[lcd_position] = button[8][in_move[8]];
            in_move[8]++;
            if(in_move[8]>2) in_move[8] = 0;
            lcd_position ++;
            fnd_count++;

            for(z=0;z<9;z++){ // if button is changed, reset the previous button count!
                if(in_move[z]!=0 && z!=8) in_move[z]=0;
            }
        }
    }

    else{  
        // number input

            for(z=0;z<9;z++) in_move[z] = 0; // reset the alphabet button count!

            if(switch_buffer[0] ==1){
           lcd_output[lcd_position] = 0x31;
            
            lcd_position ++;
            fnd_count++;
    }

    else if(switch_buffer[1] ==1 && switch_buffer[2] !=1 ){
        lcd_output[lcd_position] = 0x32;
           
            lcd_position ++;
            fnd_count++;
    }

    else if(switch_buffer[2] ==1 && switch_buffer[1] !=1){
        lcd_output[lcd_position] = 0x33;
            
            lcd_position ++;
            fnd_count++;
    }

    else if(switch_buffer[3] ==1){
        lcd_output[lcd_position] = 0x34;
        
            lcd_position ++;
            fnd_count++;
    }

    else if(switch_buffer[4] ==1 && switch_buffer[5] !=1){
       lcd_output[lcd_position] = 0x35;
           
            lcd_position ++;
            fnd_count++;
    }

    else if(switch_buffer[5] ==1 && switch_buffer[4] !=1){
       lcd_output[lcd_position] = 0x36;
            
            lcd_position ++;
            fnd_count++;
    }

    else if(switch_buffer[6] ==1){
     lcd_output[lcd_position] = 0x37;
            
            lcd_position ++;
            fnd_count++;
    }

    else if(switch_buffer[7] ==1 && switch_buffer[8] !=1){
      lcd_output[lcd_position] = 0x38;
           
            lcd_position ++;
            fnd_count++;
    }

    else if(switch_buffer[8] ==1 && switch_buffer[7] !=1){     
      lcd_output[lcd_position] = 0x39;
            
            lcd_position ++;
            fnd_count++;
        }

    }
    
    
    // read from premade array   ..... lcd_output: output that will be shown in text_lcd 
      // lcd_position: current 'to-be-written' position
      // button: pre-defined values
      // in_move: how many times did you press button no.x?


    if(text_mode ==0){
        dot_matrix(10); // draw A;
    }

    else{
        dot_matrix(1); // draw 1;
    }
    printf("lcd_position is: %d\n",lcd_position);
    if(fnd_count>9999) fnd_count = 0;

    mq_send(string_qd, lcd_output, sizeof(lcd_output), 0); // send the input string
    mq_send(input_qd, (char *)&fnd_count, attr.mq_msgsize, 0); // send fnd_count 

}
 // dk gksrmf dho dks ehodododododoododo
void mode4_input() { // dot matrix 찍기
    draw_confirm = 0; // retake the value
    
	push_switch();

    for(i=0;i<9;i++){
        printf("[%d] ",switch_buffer[i]);
    }
	printf("\n");

	if (switch_buffer[0] == 1) { // reset the mode
		
		dot_matrix_reset();
		fnd_count++;
	}

	else if (switch_buffer[1] == 1) { // go up

		if (dot_position > 6) { // if dot position is below first line..
			dot_position -= 7; // move up 
		}
		fnd_count++;

	}

	else if (switch_buffer[2] == 1) {  // cursor mode on & off

		blink_mode ^= 1;

		fnd_count++;
	}

	else if (switch_buffer[3] == 1) { // go left 

		if ((dot_position % 7) != 0) { // if dot position is not in the first column..
			dot_position--;
		}
		fnd_count++;
	}

	else if (switch_buffer[4] == 1) {
        draw_confirm = 1;
		fnd_count++;
	}

	else if (switch_buffer[5] == 1) { // go right

		if ((dot_position + 1) % 7 != 0) {  // if dot position is not at the rightmost side.. 
			dot_position++;
		}
		fnd_count++;
	}

	else if (switch_buffer[6] == 1) { // clear the drawboard

		dot_matrix_blank(); // only clear the drawboard, other values(dot_position) are not affected.
		fnd_count++;
	}

	else if (switch_buffer[7] == 1) {  // go down

		if (dot_position < 63) {
			dot_position += 7; // go down
		}
		fnd_count++;
	}

	else if (switch_buffer[8] == 1) {

		reverse_flag = 1;
		fnd_count++;
	}
    if(fnd_count>9999) fnd_count = 0;

    printf("dot position : %d \n", dot_position);
    printf("blink mode %d \n", blink_mode);
    printf("fnd_count %d \n", fnd_count);

    mode2(fnd_count, 0);

	mq_send(input_qd, (char *)&dot_position, attr.mq_msgsize, 0);
	mq_send(input_qd, (char *)&blink_mode, attr.mq_msgsize, 0); // first send the dot position to the main process, and then the blink_mode flag.
    usleep(4000);
    mq_send(input_qd, (char *)&draw_confirm, attr.mq_msgsize, 0);

	
}

void mode5_input(){

int arr[9] = {0,1,2,3,4,5,6,7,8 };
	int j, k;
	int gotit = 0; // 몇 개 맞혔나?
    int key;
    int kflag=0; // to count
	srand((unsigned)time(NULL)); // 랜덤 시드(time) 대입

	for (k = 0; k < 9; k++) {

		arr[k] = rand() % 9; // 0 ~ 8 사이의 번호 랜덤하게 대입
        if(k){
            if(arr[k]==arr[k-1]){
                k--;
                continue;   // avoid serial duplication
            }
        }

	}

	for (k = 0; k < 9; k++) {
		dot_matrix(arr[k]); // 9개의 숫자를 순차대로 1번 보여준다... ; 난이도 조절 가능
		usleep(DIFFICULTY);
	}

	k = 0;
	while (1){   // you have to finish the game.. you cannot change mode

		push_switch();

		for (j = 0; j < 9; j++) {
            
            if(switch_buffer[j]==1){
                printf("Button pushed!!!!!!!!!1\n");
                k++;
            }

			if(switch_buffer[j] ==1  && j+1 == arr[k] ){ // 눌린 버튼이 있고, 그 버튼이  arr에 저장된 랜덤한 숫자와 같을 경우
				
				gotit++;
			}

		}
        key = get_readkey(fd_readkey); // also make exit mode for mode 5
        if(key == 158){
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
                kill(to_kill[2], SIGKILL);
                kill(to_kill[0], SIGKILL);
                exit(0);
        }


            if(k==9) break; // if pressed 9 buttons, break;
	}

	mq_send(input_qd, (char*)& gotit, attr.mq_msgsize, 0); // 얻은 점수를 input message queue로 보내준다.

}