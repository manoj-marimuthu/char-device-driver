#include<stdio.h>
#include<sys/ioctl.h>
#include<unistd.h>
#include<fcntl.h>
#include "char_driver_ioctl.h"

int main(void){
	__u32 length = 0;
	int fd;
	fd = open("/dev/char_device",O_RDONLY);
	if(fd < 0){
		perror("open");
		return 1;
	}
	if(ioctl(fd,CHAR_DEVICE_GET_LENGTH,&length) == -1){
		perror("ioctl get buffer");
		close(fd);
		return 1;
	}
	printf("Recevied length using IOCTL : %u \n", length);

	if(ioctl(fd,CHAR_DEVICE_CLEAR_BUFFER) == -1){
		perror("ioctl clear buffer");
		close(fd);
		return 1;
	}
	if(ioctl(fd,CHAR_DEVICE_GET_LENGTH,&length) == -1){
		perror("ioctl get buffer");
		close(fd);
		return 1;
	}
	printf("Recevied length using IOCTL : %u \n", length);

	close(fd);
	return 0;
}
