#include "text_lcd.h"

void text_initialize(){

	char clean_all[32];
	memset(clean_all,' ',MAX_BUFF);

	write(lcd_dev,clean_all,MAX_BUFF);
}

void text_lcd(const char *up)
{
	int i;
	int str_size;
	int chk_size;
	printf("text_lcd entered!!!!!!!!\n");
	unsigned char string[32];

	memset(string, 0, sizeof(string));

/*
	//str_size=strlen(argv[1]);
	str_size = strlen(up);
	if(str_size>0) {
		strncat(string,up,str_size);
		memset(string+str_size,' ',LINE_BUFF-str_size);
	}

	//str_size=strlen(argv[2]);
	str_size = strlen(down);
	if(str_size>0) {
		strncat(string,down,str_size); 
		memset(string+LINE_BUFF+str_size,' ',LINE_BUFF-str_size); // put value to text_lcd
	}
*/
	write(lcd_dev,up,MAX_BUFF);	
	
}
