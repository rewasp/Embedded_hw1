#include "led_mmap.h"

void led_mmap(unsigned char data)
{
	int fd,i;

	unsigned long *fpga_addr = 0;
	unsigned char *led_addr =0;

	//argument ���� �´��� check
	/*
	if(argc!=2) {
		printf("please input the parameter! \n");
		printf("ex)./test_led [0-255]\n");
		return -1;
	}
	data=atoi(argv[1]);  //ex : digit 5 -> 0000 0101 (2������ �ٲ��� �� 1�� ǥ���� ���� LED�� Ŵ)
	
	printf("%d\n", data);

	if( (data<0) || (data>255) ){ //�Է��� argument�� 0~255, �� 8bit�� ǥ�������� �������� check
		printf("Invalid range!\n");
		exit(1);
	}
*/
	fd = open("/dev/mem", O_RDWR | O_SYNC); //memory device open
	if (fd < 0) {  //open fail check
		perror("/dev/mem open error");
		exit(1);
	}
	//�޸� 0x08000000 ������ mmap (4KB mapping)
	fpga_addr = (unsigned long *)mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, FPGA_BASE_ADDRESS);
	if (fpga_addr == MAP_FAILED) //mapping fail check
	{
		printf("mmap error!\n");
		close(fd);
		exit(1);
	}
	//led data ���������� �ּҰ� 0x08000016(0x08000000+16)
	led_addr=(unsigned char*)((void*)fpga_addr+LED_ADDR);
	
	*led_addr=data; //led �������Ϳ� data�� write(data���� ���� led device�� ������ led �����)  
	
	sleep(1);
	//for read
	//�ش� �׽�Ʈ ���α׷������� �ٷ� ���� write�ߴ� ���� �ٽ� read�ϴ� ���̶� �ǹ̰� ������
	//���� write �κ��� �ּ�ģ�ٸ�, led ���� ���¸� read�ؿ�
	//data=0;
	//data=*led_addr; //read led
	//printf("Current LED VALUE : %d\n",data);

	munmap(led_addr, 4096); //mapping ����
	close(fd);

}
