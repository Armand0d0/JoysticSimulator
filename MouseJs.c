#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <time.h>
#include <math.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#define WR_VALUE_X _IOW('a','a',int)
#define WR_VALUE_Y _IOW('a','b',int)
#define WR_VALUE_BTN _IOW('a','c',int)

#define NIL (0)
void Write(int x){
        int fd;
fd = open("/dev/js1", O_WRONLY);          if(fd < 0) {printf("Cannot open device file...\n");return;}  

        ioctl(fd, WR_VALUE_X,  x);
        close(fd);
}
 

int main()
{
       
        printf("start..\n");
        clock_t t0 = clock();
        Display *dpy = XOpenDisplay(NIL);
        int sc = DefaultScreen(dpy);
        int x,y,x1,y1,x2,y2;
        unsigned int mask =0;
        Window child_win, root_win;
        

        while (clock() < 50000000)
        {
                clock_t t1 = clock();
            XQueryPointer(dpy,XRootWindow(dpy,sc),&child_win,&root_win,&x1,&y1,&x2,&y2,&mask);
            
            x = (int)(((float)x1-(1920.0/2.0)) /(1920.0/255.0/2.0));
           Write(x);
            printf("x=%d\n",x);/////////////////////////////////////////
            //while (clock()-t1<100000){}
            
        }
        
        
        //printf("t0 =%ld\n",t0);
        printf("End..\n");//*/
        return 0;
}//gcc -o MouseJs MouseJs.c -lX11 