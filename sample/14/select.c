#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    int sfd1, sfd2, sfd3, i;

    fd_set rfds;
    fd_set errorfds;
    struct timeval tv;
    int retval;
    char buff[256];
    int readcnt;


    sfd1 = open("/dev/ttyS1", O_RDWR | O_NOCTTY);
    sfd2 = open("/dev/ttyS2", O_RDWR | O_NOCTTY);
    sfd3 = open("/dev/ttyS3", O_RDWR | O_NOCTTY);



    //各种serial环境设定程序



    while(1)
    {
        //read event对象
        FD_ZERO(&rfds);
        FD_SET(sfd1, &rfds);
        FD_SET(sfd2, &rfds);
        FD_SET(sfd3, &rfds);

        //error event对象
        FD_ZERO(&errorfds);

        FD_SET(sfd1, &errorfds);
        FD_SET(sfd2, &errorfds);
        FD_SET(sfd3, &errorfds);

        tv.tv_sec = 5; //时间“5s”
        tv.tv_usec = 0;

        //等待事件
        retval = select(sfd3+1, &rfds, NULL, &errorfds, &tv);
        if(retval < 0)
        {
            perror ("select");
            exit (-1);
        }
        if(retval == 0)
        {
            printf("5s内没有任何数据.\n");
        }
        //检查有无接收的数据.
        for (i = 0; i < FD_SETSIZE; ++i)
        {
            if (FD_ISSET (i, &read_fd_set))
            {
                readcnt = read(i, buff, 256);
                write(i, buff, readcnt);
            }
        }
        //检查error
        for (i = 0; i < FD_SESIZE; ++i)
        {
            if (FD_ISSET (i, &errorfds))
            {
                printf("设备发生了error..\n");
                exit (EXIT_FAILURE);
            }
        }
    }

    close(sfd1);
    close(sfd2);
    close(sfd3);

}
