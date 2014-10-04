#include <stdio.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <X11/X.h>
#include <X11/keysym.h>

static int snglBuf[] = {GLX_RGBA, GLX_DEPTH_SIZE, 16, None};
static int dblBuf[]  = {GLX_RGBA, GLX_DEPTH_SIZE, 16, GLX_DOUBLEBUFFER, None};

static int keymap[FT_KEYBOARDSTATE_SIZE] = {0};

Display   *dpy;
Window     win;
GLboolean  doubleBuffer = GL_TRUE;

void fatalError(const char *s)
{
	printf("%s", s);
}

int main(int argc, char **argv)
{
	fountain::basicSetting();

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
		| ButtonPressMask | ButtonReleaseMask | StructureNotifyMask | PointerMotionMask;
	win = XCreateWindow(dpy, RootWindow(dpy, vi->screen), 0, 0,
			fountain::mainWin.w, fountain::mainWin.h, 0, vi->depth, InputOutput, vi->visual,
			CWBorderPixel | CWColormap | CWEventMask, &swa);
	XSetStandardProperties(dpy, win, fountain::mainWin.title.c_str(), "main", None, argv, argc, NULL);

	glXMakeCurrent(dpy, win, cx);

	XMapWindow(dpy, win);

	fountain::initAllSystem();
	fountain::gameInit();

	//TODO: move the outside OpenGL initialization to ft_render
	//glEnable(GL_LINE_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	for (;;) {
        	while (XPending(dpy) != 0) {
			XNextEvent(dpy, &event);
			//FIXME: "XIO:  fatal IO error 11" when closing with x button or alt+f4
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
					//TODO: set the sysKeyboard
					//fountain::sysKeyboard.setState(kevent->keycode, 1);
					char       buffer[1];
					kevent = (XKeyEvent *) &event;
					if ((XLookupString((XKeyEvent *)&event,buffer,1,&keysym,NULL) == 1)
							&& (keysym == (KeySym)XK_Escape) )
						return 0;
                    printf("keysym = %lu\n", keysym & 0x000001ff);
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
							printf("keyRetriggered\n");
							//TODO: set the sysKeyboard
							XNextEvent(dpy, &event);
							is_retriggered = 1;
						}
					}
					if (!is_retriggered) {
						printf("keyRelease %d\n", event.xkey.keycode);
						//TODO: set the sysKeyboard
						//fountain::sysKeyboard.setState(event.xkey.keycode, 0);
					}
				}
				break;

				case ButtonPress:
					fountain::sysMouse.setState(event.xbutton.button, 1);
				break;

				case ButtonRelease:
					fountain::sysMouse.setState(event.xbutton.button, 0);
				break;

				case MotionNotify:
					fountain::sysMouse.update(event.xmotion.x, event.xmotion.y);
				break;

				case ConfigureNotify:
					glViewport(0, 0, event.xconfigure.width,
						event.xconfigure.height);
				break;

				case Expose:
				break;
			}
		}
		//TODO: move the outside OpenGL word to ft_render
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glColor3f(1.0, 1.0, 1.0);
		glPushMatrix();
		fountain::singleFrame();
		glPopMatrix();
		glXSwapBuffers(dpy, win);
	}
	return 0;
}

