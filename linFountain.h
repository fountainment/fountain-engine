#include <stdlib.h>
#include <stdio.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <X11/X.h>
#include <X11/keysym.h>

float x=0;

int tmpint;

static int snglBuf[] = {GLX_RGBA, GLX_DEPTH_SIZE, 16, None};
static int dblBuf[]  = {GLX_RGBA, GLX_DEPTH_SIZE, 16, GLX_DOUBLEBUFFER, None};

Display   *dpy;
Window     win;
GLboolean  doubleBuffer = GL_TRUE;

void fatalError(char *s)
{
    printf("%s", s);
}

int main(int argc, char **argv)
{
    fountain::gameInit();
    XVisualInfo         *vi;
    Colormap             cmap;
    XSetWindowAttributes swa;
    GLXContext           cx;
    XEvent               event;
    int                  dummy;


    dpy = XOpenDisplay(NULL);
    if (dpy == NULL)
        fatalError("could not open display");

    if(!glXQueryExtension(dpy, &dummy, &dummy))
        fatalError("X server has no OpenGL GLX extension");

    vi = glXChooseVisual(dpy, DefaultScreen(dpy), dblBuf);
    if (vi == NULL)
    {
        vi = glXChooseVisual(dpy, DefaultScreen(dpy), snglBuf);
        if (vi == NULL) fatalError("no RGB visual with depth buffer");
        doubleBuffer = GL_FALSE;
    }
    //if(vi->class != TrueColor)
    //  fatalError("TrueColor visual required for this program");

    cx = glXCreateContext(dpy, vi, /* no shared dlists */ None,
                          /* direct rendering if possible */ GL_TRUE);
    if (cx == NULL)
        fatalError("could not create rendering context");

    cmap = XCreateColormap(dpy, RootWindow(dpy, vi->screen), vi->visual, AllocNone);
    swa.colormap = cmap;
    swa.border_pixel = 0;
    swa.event_mask = KeyPressMask    | ExposureMask
                     | ButtonPressMask | StructureNotifyMask | PointerMotionMask | KeymapStateMask;
    win = XCreateWindow(dpy, RootWindow(dpy, vi->screen), 0, 0,
                        fountain::mainWin.w, fountain::mainWin.h, 0, vi->depth, InputOutput, vi->visual,
                        CWBorderPixel | CWColormap | CWEventMask, &swa);
    XSetStandardProperties(dpy, win, fountain::mainWin.title.c_str(), "main", None,
                           argv, argc, NULL);

    glXMakeCurrent(dpy, win, cx);

    XMapWindow(dpy, win);


    while (1)
    {
        while (XPending(dpy) != 0)
        {
            XNextEvent(dpy, &event);

            switch (event.type)
            {

            case KeymapNotify:
                printf("keyMap\n");
                break;
            case KeyPress:
            {

                KeySym     keysym;
                XKeyEvent *kevent;
                printf("keyPress %d\n", kevent->keycode);
                char       buffer[1];
                kevent = (XKeyEvent *) &event;
                if (   (XLookupString((XKeyEvent *)&event,buffer,1,&keysym,NULL) == 1)
                        && (keysym == (KeySym)XK_Escape) )
                    exit(0);
                /*
                          if (keysym >= (KeySym)'A' && keysym <= (KeySym)'Z') keyboard[keysym] = 1;
                          else if (keysym == '`') switchConsole();
                          inputConsole(keysym);
                */
                break;
            }
            case KeyRelease:
                printf("keyRelease\n");
                break;

            case ButtonPress:
                switch (event.xbutton.button)
                {
                case 1:
                    break;
                case 2:
                    break;
                case 3:
                    break;
                }
                break;
            case MotionNotify:
                //printf("%d %d\n", event.xmotion.x, event.xmotion.y);
                break;
            case ConfigureNotify:
                glViewport(0, 0, event.xconfigure.width,
                           event.xconfigure.height);
            case Expose:
                break;
            }
        }
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glFrustum(-200, 200, -150, 150, 200, 600);
        //glOrtho(0, w, 0, h, -99999, 99999);
        glTranslatef(0, 0, -400);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glColor3f(1.0, 1.0, 1.0);
        glPushMatrix();
        glRotatef(x, 0.0, 1.0, 0.0);
        glScalef(45.0, 45.0, 45.0);
        fountain::singleFrame();
        glPopMatrix();
        glXSwapBuffers(dpy, win);
        x += 1;
    }

    return 0;
}

