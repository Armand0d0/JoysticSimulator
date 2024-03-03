#include <stdio.h>



#define len 32
int main(){
    float times[] = {-0.20, 2.05, 3.04, 3.80, 4.48, 5.06, 5.61, 6.12, 6.61, 7.08, 7.53, 7.96, 8.38, 8.78, 9.17, 9.54, 9.90, 10.26, 10.60, 10.94, 11.27,
    11.60, 11.92, 12.23, 12.53, 12.83, 13.12, 13.41, 13.69, 13.97, 14.25, 14.52};
    float speeds[] = {0, 102, 135, 162, 184, 204, 218, 231, 238, 249, 261, 272, 282, 292, 302, 311, 320, 329, 337, 342, 350, 358, 366, 373, 381, 
    389, 396, 403, 408, 414, 419, 425};

    for(int i =0;i<(len-1);i++){
        float a = ( speeds[i+1]-speeds[i] ) / ( times[i+1]-times[i] );
        printf("a= %f\n",a);
    }

return 0;
}