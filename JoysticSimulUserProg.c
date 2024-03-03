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

#define WR_VALUE_X _IOW('a','a',int)
#define WR_VALUE_Y _IOW('a','b',int)
#define WR_VALUE_BTN _IOW('a','c',int)

#define moves 1


void Write(int x, int y,int btn){
        int fd;
fd = open("/dev/js1", O_WRONLY);          if(fd < 0) {printf("Cannot open device file...\n");return;}

        if(btn ==-1){
        ioctl(fd, WR_VALUE_X,  x);
        ioctl(fd, WR_VALUE_Y,  -y);
        }else{
        ioctl(fd, WR_VALUE_BTN,  btn);
        }
        close(fd);
}
 void Wait(float s){
         int t = (int)(s*1000000);
         printf("t=%d\n",t);
        clock_t t0 = clock();
        while (clock()-t0<t){}
 }

int main()
{
       
        printf("start..\n");
       int  trajectory[moves][3] = {{0,0,255}}; // {{time,xpos,ypos},{time,xpos,ypos},...}
        float r = 0.2;//0.8r 2s -> ~100km/h  1.1r 2.5s ~130km/h   1.3r 3s ~150km/h      1.8r 4.5s 200km/h
        //r = (intial speed / 100 ) - 0.2
      /*  Write(0,0,-1);
        Wait(3);                 // | wait 3 sec
        Write(0,0,1);//             | restart race   
       // Wait(0.5);               ///| 
        Write(0,0,0); //            / | relese button
        Wait(2.6);                //  | wait the 3 start sec
        
        float Time = 11*(14.37/14)/10;
        Wait(Time);
*/
        clock_t t0 = clock();/*
        for(int i=0;i< moves;i++){
                while(clock()-t0 < trajectory[i][0]){}
                Write(trajectory[i][1],trajectory[i][2], -1);
        }*/
       /* for(int i=0;i< 100;i++){
                clock_t c0 = clock();
                Write(0,0,-1);
                while(clock()-c0< 50000){}
                Write(0,255,-1);
                c0 = clock();
                while(clock()-c0< (int)(50000*r)){}
        }*/
         clock_t t1 = clock();
         Write(255,0,-1);
        printf("t0 =%ld, t1 =%ld, diff = %ld\n",t0,t1,(t1-t0));
        
        int x,y; 
        while(x != -1000){
        printf("enter x and y values :");
        scanf("%d %d",&x,&y);
        Write(x,y,-1);
        printf("set x = %d and y = %d\n",x,y);
        }
        printf("End !\n");//*/
        return 0;
}// 425km/h    14.42s 985m