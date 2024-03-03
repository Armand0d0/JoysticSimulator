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
#include <assert.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#define WR_VALUE_X _IOW('a','a',int)
#define WR_VALUE_Y _IOW('a','b',int)
#define WR_VALUE_BTN _IOW('a','c',int)
#define NIL (0)
#define WIDTH 1920
#define HEIGHT 1080
#define DELTA  100
#define ROW_NB  HEIGHT/DELTA
#define COLUMN_NB WIDTH/DELTA
#define ARRAY_LENGTH ROW_NB*COLUMN_NB
using namespace std;


 int Write(int x, int y){

      int fd;
        int32_t value, number = 1;
        fd = open("/dev/js1", O_RDWR);          if(fd < 0) {printf("Cannot open device file...\n");return 1;}        
        ioctl(fd, WR_VALUE_X, x);
        ioctl(fd, WR_VALUE_Y, -y); 

        close(fd);
        return 0;
 }
 float col( int col){
return (float)col/65535;
 }
 float arrCol(XColor xcol){//to black and white
     return ( col(xcol.red)+col(xcol.green)+col(xcol.blue) )/3;
 }
XColor getColor(Display *d,XImage *img, int x, int y){
XColor ret;
ret.pixel = XGetPixel(img,x,y);
XQueryColor (d, XDefaultColormap(d, XDefaultScreen(d)), &ret);
return ret;//((ret.red/256) >=250 && (ret.green/256)>=250 && (ret.blue/256)>=250 );
}

int main()
{
     Display *d = XOpenDisplay((char *) NULL);
    assert(d);
    int sc = XDefaultScreen(d);
    XImage *img;
    img = XGetImage (d, XRootWindow (d, sc), 0, 0, WIDTH, HEIGHT, AllPlanes, XYPixmap);

       /* int x,y,x1,y1,x2,y2;
        unsigned int mask =0;
        Window child_win, root_win;
        XQueryPointer(d,XRootWindow(d,sc),&child_win,&root_win,&x1,&y1,&x2,&y2,&mask);
            x = x1;//(int)(((float)x1-(1920.0/2.0)) /(1920.0/255.0/2.0));
            y = y1;//(int)(((float)y1-(1080.0/2.0)) /(1080.0/255.0/2.0));

        cout << col(getColor(d,img,x,y).red)  << " " << col(getColor(d,img,x,y).green)<< " " << col(getColor(d,img,x,y).blue) << endl;*/
      //Write(127,200);

        float array[ROW_NB][COLUMN_NB];
        for(int i=0;i<ROW_NB;i++){
            for(int j=0;j<COLUMN_NB;j++){
                array[i][j] = arrCol(getColor(d,img,j*DELTA,i*DELTA));
                cout<< array[i][j]<<" ";
            }
            cout<<endl;
        }

         XFree (img);





        return 0;
}