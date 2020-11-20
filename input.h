#ifndef INPUT_H
#define INPUT_H

#include <errno.h>
#include<time.h>
#include <dirent.h>
#include <linux/input.h>
#include <sys/select.h>
#include <sys/time.h>
#include <termios.h>
#include <pthread.h>

#include "push_switch.h"
#include "led_mmap.h"
#include "fnd.h"
#include "headers.h"
#include "dot.h"
#include "text_lcd.h"

#define FPGA_READKEY_DEVICE "/dev/input/event0"
#define DIFFICULTY 900000    // 랜덤한 숫자 보여주는데 몇 초나 줄것인가? 이 값을 통해 난이도 조절이 가능하다.

int get_readkey(int fd);
void input_handler(int mode);
void mode1_input();
void mode2_input();
void mode3_input();
void mode4_input();
void mode5_input();
void time_change();
void dot_matrix_reset();
int to_decimal(int sys);

static char buf2[32];
static struct timespec time_clock; // clock_gettime을 사용하기 위해
static struct tm tm_cur; // tm 구조체의 hour, minute을 사용하기 위해
static struct tm tm_static; // for time reset 
static int hour, min; // mode1의 시간과 분
int temp; // 이것저것 쓰는 용도로.. 
static int time_save_flag = 0; // mode1 시간 변경용 flag
static int numeral_sys[4] = {10,8,4,2}; // 진법!
static int numeral_index = 0; // numeral_sys를 돌기 위한 index
static int numeral_count; // 3자리를 넘어가는지 확인해주기 위한 변수
static int numeral=0; // 진법 표시용 숫자, 초기 값은 0이다.
static int numeral_tmp = 0; // 진법 변환 계산용 임시 숫자
static int n = 1; //  진수 변환용 숫자
static int i=0; // 왜 만들었떠라..
extern int fd_readkey; // readkey용 file descriptor
static int readkey_val; // readkey val
extern unsigned char switch_buffer[9]; // switch 입력 받아오기 위한 buffer
int draw_confirm; 
int numeral_adding[3]; // used temporarily for numeral_adding...

int text_mode;
char lcd_output[MAX_BUFF]; // for text_lcd();
int lcd_position; // for lcd_output's position
static char button[9][3] = { {'.', 'Q', 'Z'}, {'A', 'B', 'C'}, {'D', 'E', 'F'}, {'G', 'H', 'I'}, {'J', 'K', 'L'}, {'M', 'N', 'O'}, {'P', 'R', 'S'}, {'T', 'U', 'V'}, {'W', 'X', 'Y'}}; // buttons
int button_changed[9];
int in_move[9]; // if pressed several times..
extern int fnd_count;
// mode3 values need to be initiaizlied..

static int dot_position=0;
static int blink_mode=1; // blinking = 1, not blinking = 0;



// mode 4


#endif
