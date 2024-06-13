#include <iostream>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <assert.h>
#include <unistd.h>
#include <X11/extensions/XInput.h>
#include <X11/extensions/XI.h>
#include <X11/extensions/XInput2.h>


#define NIL (0)
using namespace std;

void wait(int t){
  clock_t t0 = clock();
  while (clock()-t0 < t){}
  return;
}

int main(int, char**)
{
  
 

    Display *d = XOpenDisplay((char *) NULL);
    assert(d);
  /*  int sc = XDefaultScreen(d);
    XImage *img;*/
    int root = XRootWindow(d,0);
    ///////////////////////////////////////////////////////////////////////
int defaultScreen = DefaultScreen(d);
  long win = 0x5a00001;

    XIM xInputMethod = XOpenIM(d, 0, 0, 0);
  if(!xInputMethod)
      printf("Input Method could not be opened\n");

  XIMStyles* styles = 0;
  if(XGetIMValues(xInputMethod, XNQueryInputStyle, &styles, NULL) || !styles) 
      printf("Input Styles could not be retrieved\n");
  XIMStyle bestMatchStyle = 0;
  for(int i=0; i<styles->count_styles; i++) {
      XIMStyle thisStyle = styles->supported_styles[i];
      if (thisStyle == (XIMPreeditNothing | XIMStatusNothing)){
      bestMatchStyle = thisStyle;
      break; }}
  XFree(styles);
if(!bestMatchStyle)
  printf("No matching input style could be determined\n");

  XIC xInputContext = XCreateIC(xInputMethod, XNInputStyle, bestMatchStyle,
                XNClientWindow, win,
                XNFocusWindow, win,
                NULL);
  if(!xInputContext)
    printf("Input Context could not be created\n");

    //printf("yy\n");
    
    XWindowAttributes wa ;
    XGetWindowAttributes(d,win,&wa);
    printf("masks : %ld\n" ,wa.all_event_masks);


      XEvent  ev;
    KeySym keysym;

    int i = 0;
    int max = 1;
  //while (i<max) {
        i++;
        XEvent ev2;
         ev2.type = KeyPress;
        ev2.xkey.state = ShiftMask;
        ev2.xkey.keycode = 60;//24 + (rand() % 33);
        ev2.xkey.same_screen = True;
        XSendEvent(d, win, True/*propagate*/, KeyPressMask, &ev2);
   // XNextEvent(d, &ev);
    switch (ev.type){
      case KeyPress:{
        printf("a");
      }break;

      default:{
              printf("b");}
              break;


    }
    ///////////////////////////////////////////////////////////////////////

    /* check XInput */
    int xi_opcode, event, error;
    if (!XQueryExtension(d, "XInputExtension", &xi_opcode, &event, &error)) {
        fprintf(stderr, "Error: XInput extension is not supported!\n");
        return 1;
    }
    
    /* Check XInput 2.0 */
    int major = 2;
    int minor = 0;
    int retval = XIQueryVersion(d, &major, &minor);
    if (retval != Success) {
        fprintf(stderr, "Error: XInput 2.0 is not supported (ancient X11?)\n");
        return 1;
    }
    unsigned char mask_bytes[(XI_LASTEVENT + 7) / 8] = {0};  /* must be zeroed! */
    XISetMask(mask_bytes, XI_RawKeyPress  );
      XIEventMask evmasks[1];
    /* You can use XIAllDevices for XWarpPointer() */
    evmasks[0].deviceid = XIAllDevices;
    evmasks[0].mask_len = sizeof(mask_bytes);
    evmasks[0].mask = mask_bytes;
    XISelectEvents(d, root, evmasks, 1);


      XEvent xevent;
    while (1) {
        XNextEvent(d, &xevent);
        /*if (xevent.xcookie.type != GenericEvent || xevent.xcookie.extension != xi_opcode) {
            // not an XInput event 
            continue;
        }*/

        XGetEventData(d, &xevent.xcookie);
       // printf(" event  : evtypr = %d",xevent.xcookie.evtype);
        if (xevent.xcookie.evtype == XI_RawKeyPress) {
            
        printf("key pressed  : keycode = %d ", (int)xevent.xkey.x_root);


        }


        XFreeEventData(d, &xevent.xcookie);

       /* Window root_return, child_return;
        int root_x_return, root_y_return;
        int win_x_return, win_y_return;
        unsigned int mask_return;
        
         // We need:
         //    child_return - the active window under the cursor
         //     win_{x,y}_return - pointer coordinate with respect to root window
        
        int retval = XQueryPointer(d, root, &root_return, &child_return,
                                   &root_x_return, &root_y_return,
                                   &win_x_return, &win_y_return,
                                   &mask_return);
        if (!retval) {
            // pointer is not in the same screen, ignore 
            continue;
        }

        // We used root window as its reference, so both should be the same 
        assert(root_x_return == win_x_return);
        assert(root_y_return == win_y_return);

        printf("root: x %d y %d\n", root_x_return, root_y_return);

        if (child_return) {
            int local_x, local_y;
            XTranslateCoordinates(d, root, child_return,
                                  root_x_return, root_y_return,
                                  &local_x, &local_y, &child_return);
            printf("local: x %d y %d\n\n", local_x, local_y);
        }*/
    }

    XCloseDisplay(d);

      //XFlush(d);
      //sleep(30);
    return 0;
}/*
g++ -o IputRecorder  -L/usr/X11R6/lib IputRecorder.cpp -lX11 && ./IputRecorder 
*/
/*  XSelectInput(d, win, ExposureMask | KeyPressMask | ButtonPressMask);
  XEvent  ev;
    KeySym keysym;

    int i = 0;
    int max = 1;
  //while (i<max) {
        i++;
    XNextEvent(d, &ev);
    switch (ev.type){
            case KeyPress:
       ///  handle key press 
        puts("KeyPress event received");
        printf("> keycode: %d\n", ev.xkey.keycode);
        //exit if q or escape are pressed 
        keysym = XLookupKeysym(&(ev.xkey), 0);
        if (keysym == XK_q ||
            keysym == XK_Escape) {
          i = max;
        } else {
          char buffer[] = "  ";
          int nchars = XLookupString(
                &(ev.xkey),
                buffer,
                2,  // buffer size 
                &keysym,
                NULL );
          if (nchars == 1)
            printf("> Key '%c' pressed\n", buffer[0]);
        }
        break;

            default:
            printf("default");
              break;
    }
  //}
*/
    



