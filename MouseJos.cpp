#include <iostream>
#include <cstdlib>
#include <pthread.h>
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

using namespace std;

static volatile int x=0, y=0, End=0;
void wait(int t){
  clock_t t0 = clock();
  while (clock()-t0 < t){}
  return;
}
void *Write(void *arg) {
  int fd;
fd = open("/dev/js1", O_WRONLY);          if(fd < 0) {printf("Cannot open device file...\n");return NULL;}  
        while(!End){
        ioctl(fd, WR_VALUE_X,  x);  cout<<" X="<< x<<endl;
        //ioctl(fd, WR_VALUE_Y,  y);
        wait(50000);
        }
        close(fd);
   pthread_exit(NULL);
}
int main () {
   pthread_t writer;
   int w;
      w = pthread_create(&writer, NULL, Write, NULL);  if (w) { printf("Error:unable to create thread, %d\n", w); exit(-1);}

    Display *dpy = XOpenDisplay(NIL);
        int sc = DefaultScreen(dpy);
        int x,y,x1,y1,x2,y2;
        unsigned int mask =0;
        Window child_win, root_win;
                clock_t t0 = clock();

 while (!End)
        {   if(clock()-t0>=10000000){End =1;}
            XQueryPointer(dpy,XRootWindow(dpy,sc),&child_win,&root_win,&x1,&y1,&x2,&y2,&mask);
            
            x = (int)(((float)x1-(1920.0/2.0)) /(1920.0/255.0/2.0));
            y = (int)(((float)y1-(1080.0/2.0)) /(1080.0/255.0/2.0));
            printf("x=%d,y=%d\n",x,y);/////////////////////////////////////////
            wait(50000);
        }
        printf("End..\n");//*/
   pthread_exit(NULL);

    return 0;
}// g++ -o MouseJos MouseJos.cpp -lpthread -lX11