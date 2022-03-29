//
// Created by mva on 3/21/22.
//

#include <iostream>

#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>

//sudo mknod /dev/hello c 115 0

int main(){

    char buf[100];
    int rez;

    int fd = open("/dev/hello",  O_RDWR, 0);
    printf("fd = %d \n", fd);

//    rez = read(fd, &buf, sizeof(buf));
//
//    printf("read buf : %s \n size = %d \n", buf, rez);
//
//    strcpy(buf, "Hello driver ");
//    rez = write(fd, &buf, sizeof(buf));
//    printf("write rez : %d buf =%s\n", rez, buf);


    strcpy(buf, "Hello driver 2");
    rez = ioctl(fd, 0x01, &buf); //TO driver

    printf("ioctl 1 rez : %d buf =%s\n", rez, buf);

    strcpy(buf, "Hello driver 3");
    rez = ioctl(fd, 0x03, &buf); //TO driver

    printf("ioctl 2 rez : %d buf =%s\n", rez, buf);
    sleep(20);
    close(fd);

    return 0;
}