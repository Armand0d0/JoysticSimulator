#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include<sys/ioctl.h>
 
#define WR_VALUE_X _IOW('a','a',int)
#define WR_VALUE_Y _IOW('a','b',int)

int main()
{
       int fd;
        int32_t value, number = 1;
        int x=0,y=0;
        printf("\nOpening Driver\n");
        fd = open("/dev/js1", O_RDWR);          if(fd < 0) {printf("Cannot open device file...\n");return 0;}
        while( x!= -1000){
                printf("Enter pos :?");
                
                scanf("%d %d",&x,&y);
                printf("x=%d , y=%d\n",x,y);
        

        ioctl(fd, WR_VALUE_X, x);
        ioctl(fd, WR_VALUE_Y, -y); 
        }
        /*ioctl(fd, WR_VALUE_X, 0);
        ioctl(fd, WR_VALUE_Y, 0);//*/ 
        close(fd);
        printf("Closing Driver\n");
        
        return 0;
}