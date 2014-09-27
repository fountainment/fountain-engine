#include <stdlib.h>
#include <stdio.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <X11/X.h>
#include <X11/keysym.h>

static int snglBuf[] = {GLX_RGBA, GLX_DEPTH_SIZE, 16, None};
static int dblBuf[]  = {GLX_RGBA, GLX_DEPTH_SIZE, 16, GLX_DOUBLEBUFFER, None};

Display   *dpy;
Window     win;
GLboolean  doubleBuffer = GL_TRUE;

void fatalError(const char *s)
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
	if (vi == NULL) {
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
	swa.event_mask = KeyPressMask | KeyReleaseMask | ExposureMask
		| ButtonPressMask | StructureNotifyMask | PointerMotionMask;
	win = XCreateWindow(dpy, RootWindow(dpy, vi->screen), 0, 0,
			fountain::mainWin.w, fountain::mainWin.h, 0, vi->depth, InputOutput, vi->visual,
			CWBorderPixel | CWColormap | CWEventMask, &swa);
	XSetStandardProperties(dpy, win, fountain::mainWin.title.c_str(), "main", None, argv, argc, NULL);

	glXMakeCurrent(dpy, win, cx);

	XMapWindow(dpy, win);

	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	for (;;) {
        	while (XPending(dpy) != 0) {
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
					if ((XLookupString((XKeyEvent *)&event,buffer,1,&keysym,NULL) == 1) 
							&& (keysym == (KeySym)XK_Escape) )
						exit(0);
				}
				break;

				case KeyRelease:
				{
					unsigned short is_retriggered = 0;
					if (XEventsQueued(dpy, QueuedAfterReading)) {
						XEvent nev;
						XPeekEvent(dpy, &nev);

						if (nev.type == KeyPress && nev.xkey.time == event.xkey.time 
								  && nev.xkey.keycode == event.xkey.keycode) {
							fprintf(stdout, "re\n");
							XNextEvent(dpy, &event);
							is_retriggered = 1;
						}
					}
					if (!is_retriggered)
						printf("keyRelease %d\n", event.xkey.keycode);
				}
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
					printf("%d %d\n", event.xmotion.x, event.xmotion.y);
				break;

				case ConfigureNotify:
					glViewport(0, 0, event.xconfigure.width,
						event.xconfigure.height);
				break;

				case Expose:
				break;
			}
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glColor3f(1.0, 1.0, 1.0);
		glPushMatrix();
		glScalef(45.0, 45.0, 45.0);
		fountain::singleFrame();
		glPopMatrix();
		glXSwapBuffers(dpy, win);
	}
	return 0;
}

