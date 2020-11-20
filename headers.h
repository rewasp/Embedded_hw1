#ifndef HEADERS_H
#define HEADERS_H

#include <mqueue.h>


mqd_t input_qd, output_qd, readkey_qd, inmode_qd, outmode_qd, string_qd;
struct mq_attr attr, attr2;
int switch_dev; // device fd for switch 
int fnd_dev;
int fd_readkey;
int lcd_dev;
int reverse_flag;
extern int draw_confirm;
pid_t to_kill[3];
pid_t pid;

#endif