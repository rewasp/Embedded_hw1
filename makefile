CC = arm-none-linux-gnueabi-gcc
CFLAGS = -static
TARGET = app
OBJS = main.o input.o output.o fnd.o led_mmap.o push_switch.o dot.o text_lcd.o

$(TARGET): $(OBJS)
		$(CC) $(CFLAGS) -o app main.o input.o output.o fnd.o led_mmap.o push_switch.o dot.o text_lcd.o -lrt

main.o : main.c
		$(CC) $(CFLAGS) -c -o main.o main.c -lrt

input.o : input.c
		$(CC) $(CFLAGS) -c -o input.o input.c -lrt

output.o : output.c
		$(CC) $(CFLAGS) -c -o output.o output.c -lrt

fnd.o : fnd.c
		$(CC) $(CFLAGS) -c -o fnd.o fnd.c -lrt

led_mmap.o : led_mmap.c
		$(CC) $(CFLAGS) -c -o led_mmap.o led_mmap.c -lrt

push_switch.o : push_switch.c
		$(CC) $(CFLAGS) -c -o push_switch.o push_switch.c -lrt

dot.o : dot.c
		$(CC) $(CFLAGS) -c -o dot.o dot.c -lrt

text_lcd.o: text_lcd.c
		$(CC) $(CFLAGS) -c -o text_lcd.o text_lcd.c -lrt

clean: 
	rm -rf $(OBJS) $(TARGET)
