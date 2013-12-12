////////////////////////////////////////////////////
// anim.cpp
// Template code for drawing an interesting animation.
////////////////////////////////////////////////////

#ifdef WIN32
#include <windows.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#ifdef WIN32
#include <fstream.h>
#else
#include <fstream.h>
#include <stream.h>
#endif


extern void setupUI(int winID);

void nextFrame();
void save_image(int frame);


class Keyframe {
public:
  void set(double time, double value) {
    t = time;
    v = value;
  }
  inline double getValue() {return v;}
  inline double getTime() {return t;}

  double t; // time
  double v; // value;
};

class DegreeOfFreedom {
public:
  double curValue;
  char *name;
  int nKeyframes;
  Keyframe *keys;
};

int winWidth,winHeight;

#define PI 3.1415926535897
#define X 0
#define Y 1
#define Z 2
#define EPSILON 0.00001

double curTime = 0.0;
double tStep   = 0.05;
double dt      = tStep;
double tMax    = 1.0;
bool animating = false;
bool recording = false;
int imageWindowID;

double eye[3] = {1.5, 2.0, 10.0};

//////////////////////////////////////////////////////
// ***************************************************
// * You should change these lines to reflect the number
// * of degrees of freedom in your animation
// ***************************************************
//////////////////////////////////////////////////////

int nDOF = 3;
DegreeOfFreedom dofs[3];
enum {BASE_ANGLE, ARM_ANGLE, EYE_Y};

void set_animation_direction(int dir) {
  if (dir == 1) {
	dt = tStep;
  }
  else {
	dt = -tStep;
  }
}

void step_forward() {
  curTime += tStep;
  nextFrame();
}

void step_back() {
  curTime -= tStep;
  nextFrame();
}

void set_recording(bool newState) {
  recording = newState;
}

void set_animating(bool newState) {
  animating = newState;
}

//////////////////////////////////////////////////////
//    PROC: glut_key_action()
//    DOES: this function gets called for any keypresses
//////////////////////////////////////////////////////

void glut_key_action(unsigned char key, int x, int y)
{
  switch (key) {
  case 'q':
  case 27:
    exit(0); 
  case 's':
    save_image(-1);
    break;
  case 'r':
    recording ^= 1;
    if (recording)
      printf("Recording ON\n");
    else
      printf("Recording OFF\n");
    break;
  case ',':
  case '+':
    animating = 0;
    step_forward();
    break;
  case '.':
  case '-':
    animating = 0;
    step_back();
    break;
  case ' ':
    animating = !animating;
    break;
  case '<':
    animating = 1;
    dt = -tStep;
    break;
  case '>':
    animating = 1;
    dt = tStep;
    break;
  }
}

//////////////////////////////////////////////////////
// ***************************************************
// * You should change this function to use the
// * curve coefficients for your animation.
// ***************************************************
//////////////////////////////////////////////////////

void initDofs() {
  dofs[BASE_ANGLE].curValue = 0;
  dofs[BASE_ANGLE].name = "Base Angle";
  dofs[BASE_ANGLE].nKeyframes = 3;
  dofs[BASE_ANGLE].keys = new Keyframe[3];
  dofs[BASE_ANGLE].keys[0].set(0,0);
  dofs[BASE_ANGLE].keys[1].set(1,0.5);
  dofs[BASE_ANGLE].keys[2].set(2,1);

  dofs[ARM_ANGLE].curValue = 0;
  dofs[ARM_ANGLE].name = "Arm Angle";
  dofs[ARM_ANGLE].nKeyframes = 3;
  dofs[ARM_ANGLE].keys = new Keyframe[3];
  dofs[ARM_ANGLE].keys[0].set(0,0);
  dofs[ARM_ANGLE].keys[1].set(1,0);
  dofs[ARM_ANGLE].keys[2].set(2,1);

  dofs[EYE_Y].curValue = 0;
  dofs[EYE_Y].name = "Eye Y coord";
  dofs[EYE_Y].nKeyframes = 3;
  dofs[EYE_Y].keys = new Keyframe[3];
  dofs[EYE_Y].keys[0].set(0,0);
  dofs[EYE_Y].keys[1].set(1,1);
  dofs[EYE_Y].keys[2].set(2,1);

  tMax = 0;
  for (int idof = 0; idof<nDOF; idof++) {
    for (int ikey = 0; ikey<dofs[idof].nKeyframes; ikey++) {
      double t = dofs[idof].keys[ikey].getTime();
      if (t > tMax)
	    tMax = t;
    }
  }
}

/////////////////////////////////////////
//	PROC:	save_image
//	DOES:	saves the current image to a ppm file
/////////////////////////////////////////

void save_image(int frame) {
  char fname[100];
  const int maxVal=255;
  register int y;
  char *pixels;

  if (frame < 0) {
    strcpy(fname,"scene.ppm");
  }
  else {
    sprintf(fname,"anim%03d.ppm",frame);
  }
  cout << "Saving image " << fname 
       << " " << winWidth << " x " << winHeight << endl;

  ofstream outImg;

  outImg.open(fname, ios::binary);
  outImg << "P6" << endl;
  outImg << winWidth << " " << winHeight << endl;
  outImg << maxVal << endl;

  pixels = new char [3*winWidth];

  glutSetWindow(imageWindowID);
  for ( y = winHeight-1; y>=0; y-- ) {
    glReadPixels(0,y,winWidth,1,GL_RGB,GL_UNSIGNED_BYTE, (GLvoid *) pixels);
    outImg.write(pixels, winWidth*3);
  }
  delete[] pixels;
  outImg.close();
}

/*********************************************************
    PROC: myinit()
    DOES: performs most of the OpenGL intialization
     -- change these with care, if you must.
**********************************************************/

void myinit(void) {
  GLfloat ambient[] = { 0.0, 0.0, 0.0, 1.0 };
  GLfloat diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat position[] = { 0.0, 3.0, 3.0, 0.0 };
    
  GLfloat lmodel_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
  GLfloat local_view[] = { 0.0 };

  /**** set lighting parameters ****/
  glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
  glLightfv(GL_LIGHT0, GL_POSITION, position);
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
  glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, local_view);

  /*    glFrontFace (GL_CW); */
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_AUTO_NORMAL);
  glEnable(GL_NORMALIZE);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glPixelStorei(GL_PACK_ALIGNMENT, 1);
}

/*********************************************************
    PROC: set_colour();
    DOES: sets all material properties to the given colour -- don't change
**********************************************************/

void set_colour(float r, float g, float b) {
  float ambient = 0.2f;
  float diffuse = 0.7f;
  float specular = 0.4f;
  GLfloat mat[4];
      /**** set ambient lighting parameters ****/
  mat[0] = ambient*r;
  mat[1] = ambient*g;
  mat[2] = ambient*b;
  mat[3] = 1.0;
  glMaterialfv (GL_FRONT, GL_AMBIENT, mat);

  /**** set diffuse lighting parameters ******/
  mat[0] = diffuse*r;
  mat[1] = diffuse*g;
  mat[2] = diffuse*b;
  mat[3] = 1.0;
  glMaterialfv (GL_FRONT, GL_DIFFUSE, mat);

  /**** set specular lighting parameters *****/
  mat[0] = specular*r;
  mat[1] = specular*g;
  mat[2] = specular*b;
  mat[3] = 1.0;
  glMaterialfv (GL_FRONT, GL_SPECULAR, mat);
  glMaterialf (GL_FRONT, GL_SHININESS, 0.5);
}

/*********************************************************
    PROC: display()
    DOES: this gets called by the event handler to draw
          the scene, so this is where you need to build
	  your scene -- make your changes and additions here
	  Add other procedures if you like.
**********************************************************/

void display(void) {
  for (int idof=0; idof<nDOF; idof++) {
    cout <<	dofs[idof].name << ": " << dofs[idof].curValue << endl;
  }

  eye[Y] = dofs[EYE_Y].curValue * 10;
  glLoadIdentity();

  gluLookAt (eye[X], eye[Y], eye[Z],  0.0,0.0,0.0, 0.0,1.0,0.0);

  /* glClearColor (red, green, blue, alpha           */
  /* Ignore the meaning of the 'alpha' value for now */
  glClearColor(0.7f,0.7f,0.9f,1.0f);   /* set the background colour */
  /* OK, now clear the screen with the background colour */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      /* draw the base */
  glRotated(360.0*dofs[BASE_ANGLE].curValue, 0.0, 1.0, 0.0);
  set_colour(1, 1, 0.6f);  /* yellow */
  glPushMatrix();
    glScaled(1.0, 2.0, 1.0);
    glutSolidCube(1.0);
  glPopMatrix();

    /* draw the arm */
  set_colour(1, 0.5f, 0.5f);  /* pink */
  glTranslated(0.5, 1.0, 0.0);
  glRotated(180.0*dofs[ARM_ANGLE].curValue, 0.0, 0.0, 1.0);
  glTranslated(0.5, -1.0, 0.0);
  glPushMatrix();
    glScalef(1.0f, 2.0f, 1.0f);
    glutSolidCube(1.0);
  glPopMatrix();

  glFlush();

  glutSwapBuffers();

  if (recording) {
    int frame = (int)(curTime / tStep + 0.5);
    save_image(frame);
  }      
}

/*********************************************************
    PROC: myReshape()
    DOES: handles the window being resized -- don't change
**********************************************************/

void myReshape(int w, int h)
{
  winWidth  = w;
  winHeight = h;

  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
     /*** this defines the field of view of the camera   ***/
     /*** Making the first 4 parameters larger will give ***/
     /*** a larger field of view, therefore making the   ***/
     /*** objects in the scene appear smaller            ***/
  glFrustum(-1,1,-1,1,4,100);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
     /*** this sets the virtual camera          ***/
     /*** gluLookAt( x,y,z,   x,y,z   x,y,z );  ***/
     /***            camera  look-at camera-up  ***/
     /***            pos'n    point   vector    ***/
    
     /*** place camera at (x=4, y=6, z=20), looking at origin,
          y-axis being up         ***/
  gluLookAt(eye[X],eye[Y],eye[Z],   0,0,0,   0,1,0);
}

void handle_timer(int dummy) {
  if (animating) {
    curTime += dt;
    if (curTime > tMax+EPSILON) {
      curTime = tMax;
    }
	else if (curTime < 0.0) {
      curTime = 0.0;
    }
	else {
      nextFrame();
	}
  }

  //
  // Re-schedule myself.
  //
  glutTimerFunc(33,handle_timer,0);
}

//////////////////////////////////////////////////////
// This is called when it's time to draw the next frame.
//////////////////////////////////////////////////////

void nextFrame() {

  cout << "nextFrame. curTime="<<curTime<<endl;

  //
  // Make sure time is in bounds
  //
  if (curTime > tMax+EPSILON) curTime = tMax;
  if (curTime < 0.0)  curTime = 0.0;

  for (int idof=0; idof < nDOF; idof++) {
    //
    // Find the interval that contains the current time point.
    //
    int curInterval = 0;
	int n = dofs[idof].nKeyframes;
    double tStart = dofs[idof].keys[n-2].getTime();
    double tEnd   = dofs[idof].keys[n-1].getTime();
    double vStart = dofs[idof].keys[n-2].getValue();
    double vEnd   = dofs[idof].keys[n-1].getValue();
    for (int ikey=0; ikey < n-1; ikey++) {
      tStart = dofs[idof].keys[ikey]  .getTime();
      tEnd   = dofs[idof].keys[ikey+1].getTime();
      if (tStart <= curTime && curTime <= tEnd) {
	curInterval = ikey;
	vStart = dofs[idof].keys[ikey]  .getValue();
	vEnd   = dofs[idof].keys[ikey+1].getValue();
	break;
      }
    }
    double t = (curTime - tStart)/(tEnd - tStart);
    dofs[idof].curValue = (1-t)*vStart + t*vEnd;
  }

  glutPostRedisplay();
}

void instructions() {
  printf("An OpenGL Animation\n");
  printf("Animation controls:\n");
  printf("Key      Result\n");
  printf(" >        play forwards\n");
  printf(" <        play backwards\n");
  printf("space     stop/start animation\n");
  printf(" r        stop/start recording frames\n");
  printf(" . or +   step forwards one frame\n");
  printf(" , or -   step backwards one frame\n");
}

void handleMouseMotion(int x, int y) {
}

void handleMouse(int a, int b, int x, int y) {

}

/*********************************************************
     PROC: main()
     DOES: calls initialization, then hands over control
	   to the event handle, which calls 
	   display() whenever the screen needs to be redrawn
**********************************************************/

int main(int argc, char** argv) {
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowPosition (0, 0);
  glutInitWindowSize(196,196);
  imageWindowID = glutCreateWindow(argv[0]);

  myinit();
  initDofs();

  glutReshapeFunc (myReshape);
  glutKeyboardFunc( glut_key_action );
  glutMouseFunc (handleMouse);
  glutMotionFunc (handleMouseMotion);

  instructions();
	
  glutIdleFunc(NULL);
  glutTimerFunc(33,handle_timer,0);

  glutDisplayFunc(display);
  setupUI(imageWindowID);
//    glutMainLoop();
  return 0;         // never reached
}
