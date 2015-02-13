//gcc -o quad test7.c -Wall -Wextra -Wdouble-promotion -lX11 -lGL -lGLU -lm -I includes
//http://content.gpwiki.org/index.php/OpenGL:Tutorials:Setting_up_OpenGL_on_X11
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <X11/Xlib.h>
#include <GL/glx.h>
#include <GL/glu.h>
#define N_KEYS	5
Display		*dp;
Window		win, root;
XVisualInfo	*vi;
GLint		attr[] = { GLX_RGBA, GLX_DEPTH_SIZE, 16, GLX_DOUBLEBUFFER, None };	//FIXME: testar profundidades válidas
XSetWindowAttributes	xattr_set;
GLXContext	glc;
XEvent		xev;
XWindowAttributes	xattr;
GLfloat		myx=0., myy=0., myz=10., camx=0., camy=0., camz= -10., ax=0., ay=0., aax=0., aay=0., fmove=0., lmove=0.;
GLuint texture;

int mousex, mousey, mousedx, mousedy, inv, centerx, centery;
int desired_res;
const char *errmsg = "ERROR: Can't %s.\n";

GLuint kinect[360][480];

#define pi 3.14159265359
#define INV_ALL		~0
#define INV_REDRAW	1
#define INV_MOUSE	(1 << 1)
#define INV_ME		(1 << 2)

#define x_size	700
#define speed_mouse	0.07
#define myspeed		0.5
#define DEBUG
#define FOV 0.017
void cubo(GLfloat x, GLfloat y, GLfloat z, GLfloat vert);
void redraw();	
void update();
void quit() __attribute__ ((noreturn));
void error(const char *msg) {
	printf(errmsg, msg);
	exit(1);
}
//http://tronche.com/gui/x/xlib/events/keyboard-pointer/keyboard-pointer.html
int main () {
	if ((dp = XOpenDisplay(NULL)) == NULL) error ("open display");
	
	root = DefaultRootWindow(dp);
	if ((vi = glXChooseVisual(dp, 0, attr)) == NULL) error ("set visual");
	//xattr_set.override_redirect = True;
	xattr_set.colormap = XCreateColormap(dp, root, vi->visual, AllocNone);
	xattr_set.event_mask = ExposureMask; // | KeyPressMask | KeyReleaseMask | PointerMotionMask;
	// display, parente, x, y, largura, altura, borda, profundidade, classe, visual, attr setados, attr
	// (http://tronche.com/gui/x/xlib/window/attributes/#XSetWindowAttributes)
	win = XCreateWindow(dp, root, 0, 0, 480, 360, 0, vi->depth, InputOutput, vi->visual, CWColormap | CWEventMask | CWOverrideRedirect, &xattr_set);
	XMapRaised(dp, win);
	XStoreName(dp, win, "Kinect depth sensor");
	
	glc = glXCreateContext(dp, vi, NULL, GL_TRUE);
	glXMakeCurrent(dp, win, glc);
	//glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0., 480., 0., 360., -10., 200.);
	
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	//glFrustum(FOV, -FOV, (FOV * xattr.height) / xattr.width, -(FOV * xattr.height) / xattr.width, .1, 500.);
	
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	
	update();
	return 0;
}

void redraw() {
	// desenha algo (ver cubo())
	//cubo(240, 180, 0, 180);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 480, 360, 0, GL_RGB, GL_UNSIGNED_BYTE, kinect);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	glBegin(GL_QUADS);
	glTexCoord2f(0., 0.); glVertex2f(360, 0);
	glTexCoord2f(0., 1.); glVertex2f(360, 480);
	glTexCoord2f(1., 1.); glVertex2f(0, 480);
	glTexCoord2f(1., 0.); glVertex2f(0, 0);
	glEnd();
	
	glXSwapBuffers(dp, win);
}

void update() {
	for (;;) {
		XPeekEvent(dp, &xev);	//espera evento
		if (XCheckMaskEvent(dp, ExposureMask, &xev)) {
			while (XCheckMaskEvent(dp, ExposureMask, &xev));
			XGetWindowAttributes(dp, win, &xattr);
			glViewport(0, 0, xattr.width, xattr.height);
			redraw();
		}
	}
}

void cubo(GLfloat x, GLfloat y, GLfloat z, GLfloat vert) {
	glBegin(GL_QUADS);	// quadrado
	glColor3f(1., 0., 0.); glVertex3f(x-vert, y-vert, z-vert);
	glColor3f(0., 1., 0.); glVertex3f(x-vert, y+vert, z-vert);
	glColor3f(0., 0., 1.); glVertex3f(x+vert, y+vert, z-vert);
	glColor3f(1., 1., 0.); glVertex3f(x+vert, y-vert, z-vert);
	glEnd();
	
	glBegin(GL_QUADS);	// quadrado
	glColor3f(1., 0., 0.); glVertex3f(x-vert, y-vert, z+vert);
	glColor3f(0., 1., 0.); glVertex3f(x+vert, y-vert, z+vert);
	glColor3f(0., 0., 1.); glVertex3f(x+vert, y+vert, z+vert);
	glColor3f(1., 1., 0.); glVertex3f(x-vert, y+vert, z+vert);
	glEnd();
	
	glBegin(GL_QUADS);	// quadrado
	glColor3f(1., 0., 0.); glVertex3f(x-vert, y+vert, z+vert);
	glColor3f(0., 1., 0.); glVertex3f(x-vert, y+vert, z-vert);
	glColor3f(0., 0., 1.); glVertex3f(x-vert, y-vert, z-vert);
	glColor3f(1., 1., 0.); glVertex3f(x-vert, y-vert, z+vert);
	glEnd();
	
	glBegin(GL_QUADS);	// quadrado
	glColor3f(1., 0., 0.); glVertex3f(x+vert, y+vert, z-vert);
	glColor3f(0., 1., 0.); glVertex3f(x+vert, y+vert, z+vert);
	glColor3f(0., 0., 1.); glVertex3f(x+vert, y-vert, z+vert);
	glColor3f(1., 1., 0.); glVertex3f(x+vert, y-vert, z-vert);
	glEnd();
	
	glBegin(GL_QUADS);	// quadrado
	glColor3f(1., 0., 0.); glVertex3f(x-vert, y-vert, z+vert);
	glColor3f(0., 1., 0.); glVertex3f(x-vert, y-vert, z-vert);
	glColor3f(0., 0., 1.); glVertex3f(x+vert, y-vert, z-vert);
	glColor3f(1., 1., 0.); glVertex3f(x+vert, y-vert, z+vert);
	glEnd();
	
	glBegin(GL_QUADS);	// quadrado
	glColor3f(1., 0., 0.); glVertex3f(x-vert, y+vert, z+vert);
	glColor3f(0., 1., 0.); glVertex3f(x+vert, y+vert, z+vert);
	glColor3f(0., 0., 1.); glVertex3f(x+vert, y+vert, z-vert);
	glColor3f(1., 1., 0.); glVertex3f(x-vert, y+vert, z-vert);
	glEnd();
}
/*
int check (GLfloat x, GLfloat y) {
	#define radius	.1
	int checking=maxobj-1;
	for (;;) {
		if (x + radius)
	}
	int min_check=0, max_check=maxobj-3;
	for (;;) {
		checking = (min_check + max_check) >> 1;
		if (x + radius < obj[checking]) {
			min_check = checking;
			continue;
		}
		if (x - radius > obj[checking]) {
			max_check = checking;
			continue;
		}
		if ((y + radius < obj[checking+1]) && (y - radius > obj[checking+1])) continue;
	}
}
*/
void quit() {
	XUngrabPointer(dp, CurrentTime);
	XUngrabKeyboard(dp, CurrentTime);
	glXMakeCurrent(dp, None, NULL);
	glXDestroyContext(dp, glc);
	XDestroyWindow(dp, win);
	XCloseDisplay(dp);
	exit(0);
}
