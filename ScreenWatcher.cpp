#include <iostream>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <assert.h>
#include <unistd.h>
#define Width 1920
#define Height 1080
#define ITER 50
#define Interval 500000
#define NIL (0)
using namespace std;

bool IsWhite(Display *d,XImage *img, int x, int y){
XColor ret;
ret.pixel = XGetPixel(img,x,y);
XQueryColor (d, XDefaultColormap(d, XDefaultScreen(d)), &ret);
return ((ret.red/256) >=250 && (ret.green/256)>=250 && (ret.blue/256)>=250 );
}
void wait(int t){
  clock_t t0 = clock();
  while (clock()-t0 < t){}
  return;
}
int Read(bool segment[]){
    int ret;
                        if(segment[7]){ret = 1; 
                         }else if(!(segment[0] || segment[1] || segment[2] || segment[3] || segment[4] || segment[5] || segment[6])){
                             ret =0;
                         }else{
if(segment[5]){//045689
    if(segment[4]){//068
        if(segment[6]){//68
            if(segment[1]){
                ret = 8;
            }else{
                ret = 6;
            }
        }else{
            ret = 0;
        }
    }else{//459
        if(segment[0]){//59
            if(segment[1]){
                ret = 9;
            }else{
                ret = 5;
            }
        }else{
            ret = 4;
        }
    }
}else{//1237
    if(segment[6]){//23
        if(segment[4]){
            ret = 2;
        }else{
            ret = 3;
        }
    }else{//17
        if(segment[0]){
            ret = 7;
        }else{
            ret = 1;
        }
    }
}
                            }
return  ret;
}
int main(int, char**)
{
    bool units[8];
    bool tens[8];
    bool hundreds[8];
    int speed[ITER];
    double acc[ITER];
    double t[ITER];
    speed[0]= 0;

    Display *d = XOpenDisplay((char *) NULL);
    assert(d);
    int sc = XDefaultScreen(d);
    XImage *img;

    t[0]=(double)clock();
    for (size_t i = 0; i < ITER; i++){
    img = XGetImage (d, XRootWindow (d, sc), 1760, 1000, 160, 80, AllPlanes, XYPixmap);

    units[0]= IsWhite(d,img,120,30);
    units[1]= IsWhite(d,img,135,45);
    units[2]= IsWhite(d,img,135,64);//|
    units[3]= IsWhite(d,img,120,75);//|unused
    units[4]= IsWhite(d,img,105,64);
    units[5]= IsWhite(d,img,105,45);
    units[6]= IsWhite(d,img,120,52);
    units[7]= IsWhite(d,img,125,42);
    int delta = 42;
    tens[0]= IsWhite(d,img,120-delta,30);
    tens[1]= IsWhite(d,img,135-delta,45);
    tens[2]= IsWhite(d,img,135-delta,64);//|
    tens[3]= IsWhite(d,img,120-delta,75);//|unused
    tens[4]= IsWhite(d,img,105-delta,64);
    tens[5]= IsWhite(d,img,105-delta,45);
    tens[6]= IsWhite(d,img,120-delta,52);
    tens[7]= IsWhite(d,img,125-delta,42);

    hundreds[0]= IsWhite(d,img,120-(2*delta),30);
    hundreds[1]= IsWhite(d,img,135-(2*delta),45);
    hundreds[2]= IsWhite(d,img,135-(2*delta),64);//|
    hundreds[3]= IsWhite(d,img,120-(2*delta),75);//|unused
    hundreds[4]= IsWhite(d,img,105-(2*delta),64);
    hundreds[5]= IsWhite(d,img,105-(2*delta),45);
    hundreds[6]= IsWhite(d,img,120-(2*delta),52);
    hundreds[7]= IsWhite(d,img,125-(2*delta),42);
    XFree (img);
    //int r = c.red/256, g = c.green/256, b = c.blue/256;
    //cout << "(RGB) = "<< r << " " << g << " " << b << "\n";//65535
    int hundred = Read(hundreds);
    int ten = Read(tens);
    int unit = Read(units);

    t[i+1] = (double)clock()/1000000.0;
    speed[i+1] =(hundred *100) + (ten * 10) + unit;
    acc[i] = (double)(speed[i+1]-speed[i]) / (double)(t[i+1]-t[i]);
    double a = acc[i];
     if(i>10){
        // a =( acc[i-9]+acc[i-8]+acc[i-7]+acc[i-6]+acc[i-5]+acc[i-4]+acc[i-3]+acc[i-2]+acc[i-1]+acc[i] )/10.0;
         
     }
     cout << "speed = " << speed[i+1] << "   time = " << t[i+1] <<"   acc = " << a <<"\n";
     wait(Interval);
    }//*/
    double time = (((double)clock()) - t[0])/ 1000000.0;
    double R = (double)Width/time; 
    int blackColor = BlackPixel(d, DefaultScreen(d));
      int whiteColor = WhitePixel(d, DefaultScreen(d));
      // Create the window
      Window w = XCreateSimpleWindow(d, DefaultRootWindow(d), 0, 0, Width, Height, 0, blackColor, blackColor);
      XSelectInput(d, w, StructureNotifyMask);
      XMapWindow(d, w);
      GC gc = XCreateGC(d, w, 0, NIL);
      XSetForeground(d, gc, whiteColor);
      for(;;) {
	    XEvent e;
	    XNextEvent(d, &e);
	    if (e.type == MapNotify)
		  break;
      }
      for(int i = 0;i<10;i++){
            XDrawLine(d,w,gc,0,i*100,Width,i*100);
      }
      
        for(int i =0;i<ITER;i++){
            XDrawLine(d, w, gc, t[i]*R, acc[i]+500, t[i+1]*R, acc[i+1]+500);
        }

      XFlush(d);
      sleep(30);
    return 0;
}//g++ -o ScreenWatcher ScreenWatcher.cpp -lX11