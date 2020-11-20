#ifndef OUTPUT_H
#define OUTPUT_H


#include<time.h>
#include<errno.h>

#include "push_switch.h"
#include "led_mmap.h"
#include "fnd.h"
#include "headers.h"
#include "dot.h"
#include "text_lcd.h"


void output_handler(int mode);

static char buf[32];
static int hour, min; // mode1의 시간과 분
static int dot_po, dot_blink;
static int num_2;
static int score; // 추가구현한  mode5를 위한 score
static int confirm; 

#endif