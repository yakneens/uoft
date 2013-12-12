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
#include "cardinal.h"



extern void setupUI(int winID);

void nextFrame();
void nextFrame2();
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

double eye[3] = {-5, 5, 40.0};

/* storage for one texture  */
int texture[5];

GLUquadricObj *aardvark;                               // Storage For Our Quadratic Objects ( NEW )

/* Image type - contains height, width, and data */
struct Image {
    unsigned long sizeX;
    unsigned long sizeY;
    char *data;
};
typedef struct Image Image;

// quick and dirty bitmap loader...for 24 bit bitmaps with 1 plane only.  
// See http://www.dcs.ed.ac.uk/~mxr/gfx/2d/BMP.txt for more info.
int ImageLoad(char *filename, Image *image) {
    FILE *file;
    unsigned long size;                 // size of the image in bytes.
    unsigned long i;                    // standard counter.
    unsigned short int planes;          // number of planes in image (must be 1) 
    unsigned short int bpp;             // number of bits per pixel (must be 24)
    char temp;                          // temporary color storage for bgr-rgb conversion.

    // make sure the file is there.
    if ((file = fopen(filename, "rb"))==NULL)
    {
	printf("File Not Found : %s\n",filename);
	return 0;
    }
    
    // seek through the bmp header, up to the width/height:
    fseek(file, 18, SEEK_CUR);

    // read the width
    if ((i = fread(&image->sizeX, 4, 1, file)) != 1) {
	printf("Error reading width from %s.\n", filename);
	return 0;
    }
    printf("Width of %s: %lu\n", filename, image->sizeX);
    
    // read the height 
    if ((i = fread(&image->sizeY, 4, 1, file)) != 1) {
	printf("Error reading height from %s.\n", filename);
	return 0;
    }
    printf("Height of %s: %lu\n", filename, image->sizeY);
    
    // calculate the size (assuming 24 bits or 3 bytes per pixel).
    size = image->sizeX * image->sizeY * 3;

    // read the planes
    if ((fread(&planes, 2, 1, file)) != 1) {
	printf("Error reading planes from %s.\n", filename);
	return 0;
    }
    if (planes != 1) {
	printf("Planes from %s is not 1: %u\n", filename, planes);
	return 0;
    }

    // read the bpp
    if ((i = fread(&bpp, 2, 1, file)) != 1) {
	printf("Error reading bpp from %s.\n", filename);
	return 0;
    }
    if (bpp != 24) {
	printf("Bpp from %s is not 24: %u\n", filename, bpp);
	return 0;
    }
	
    // seek past the rest of the bitmap header.
    fseek(file, 24, SEEK_CUR);

    // read the data. 
    image->data = (char *) malloc(size);
    if (image->data == NULL) {
	printf("Error allocating memory for color-corrected image data");
	return 0;	
    }

    if ((i = fread(image->data, size, 1, file)) != 1) {
	printf("Error reading image data from %s.\n", filename);
	return 0;
    }

    for (i=0;i<size;i+=3) { // reverse all of the colors. (bgr -> rgb)
	temp = image->data[i];
	image->data[i] = image->data[i+2];
	image->data[i+2] = temp;
    }
    
    // we're done.
    return 1;
}
    
// Load Bitmaps And Convert To Textures
void LoadGLTextures() {	
    // Load Texture
    Image *image1, *image2, *image3, *image4, *image5;
    
    
    // allocate space for texture
    image1 = (Image *) malloc(sizeof(Image));
    if (image1 == NULL) {
	printf("Error allocating space for image");
	exit(0);
    }

    if (!ImageLoad("travertn.bmp", image1)) {
	exit(1);
    }        


    // allocate space for texture
    image2 = (Image *) malloc(sizeof(Image));
    if (image2 == NULL) {
	printf("Error allocating space for image");
	exit(0);
    }

    if (!ImageLoad("red.bmp", image2)) {
	exit(1);
    }        
    
     // allocate space for texture
    image3 = (Image *) malloc(sizeof(Image));
    if (image3 == NULL) {
	printf("Error allocating space for image");
	exit(0);
    }

    if (!ImageLoad("green.bmp", image3)) {
	exit(1);
    }        
    
     // allocate space for texture
    image4 = (Image *) malloc(sizeof(Image));
    if (image4 == NULL) {
	printf("Error allocating space for image");
	exit(0);
    }

    if (!ImageLoad("blue.bmp", image4)) {
	exit(1);
    }    
    
     // allocate space for texture
    image5 = (Image *) malloc(sizeof(Image));
    if (image5 == NULL) {
	printf("Error allocating space for image");
	exit(0);
    }

    if (!ImageLoad("karan.bmp", image5)) {
	exit(1);
    }       
    // Create Texture	
    glGenTextures(1, (GLuint*)&texture[0]);
    glBindTexture(GL_TEXTURE_2D, texture[0]);   // 2d texture (x and y size)

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // scale linearly when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); // scale linearly when image smalled than texture

    // 2d texture, level of detail 0 (normal), 3 components (red, green, blue), x size from image, y size from image, 
    // border 0 (normal), rgb color data, unsigned byte data, and finally the data itself.
    glTexImage2D(GL_TEXTURE_2D, 0, 3, image1->sizeX, image1->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, image1->data);
    

    // Create Texture	
    glGenTextures(1, (GLuint*)&texture[1]);
    glBindTexture(GL_TEXTURE_2D, texture[1]);   // 2d texture (x and y size)


    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // scale linearly when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); // scale linearly when image smalled than texture

    // 2d texture, level of detail 0 (normal), 3 components (red, green, blue), x size from image, y size from image, 
    // border 0 (normal), rgb color data, unsigned byte data, and finally the data itself.
    glTexImage2D(GL_TEXTURE_2D, 0, 3, image2->sizeX, image2->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, image2->data);
    
    // Create Texture	
    glGenTextures(1, (GLuint*)&texture[2]);
    glBindTexture(GL_TEXTURE_2D, texture[2]);   // 2d texture (x and y size)


    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // scale linearly when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); // scale linearly when image smalled than texture

    // 2d texture, level of detail 0 (normal), 3 components (red, green, blue), x size from image, y size from image, 
    // border 0 (normal), rgb color data, unsigned byte data, and finally the data itself.
    glTexImage2D(GL_TEXTURE_2D, 0, 3, image3->sizeX, image3->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, image3->data);

    // Create Texture	
    glGenTextures(1, (GLuint*)&texture[3]);
    glBindTexture(GL_TEXTURE_2D, texture[3]);   // 2d texture (x and y size)


    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // scale linearly when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); // scale linearly when image smalled than texture

    // 2d texture, level of detail 0 (normal), 3 components (red, green, blue), x size from image, y size from image, 
    // border 0 (normal), rgb color data, unsigned byte data, and finally the data itself.
    glTexImage2D(GL_TEXTURE_2D, 0, 3, image4->sizeX, image4->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, image4->data);

    // Create Texture	
    glGenTextures(1, (GLuint*)&texture[4]);
    glBindTexture(GL_TEXTURE_2D, texture[4]);   // 2d texture (x and y size)


    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // scale linearly when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); // scale linearly when image smalled than texture

    // 2d texture, level of detail 0 (normal), 3 components (red, green, blue), x size from image, y size from image, 
    // border 0 (normal), rgb color data, unsigned byte data, and finally the data itself.
    glTexImage2D(GL_TEXTURE_2D, 0, 3, image5->sizeX, image5->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, image5->data);


};
//////////////////////////////////////////////////////
// ***************************************************
// * You should change these lines to reflect the number
// * of degrees of freedom in your animation
// ***************************************************
//////////////////////////////////////////////////////

int nDOF = 5;
DegreeOfFreedom dofs[5];
enum {HAMMER_X,HAMMER_Y,HAMMER_Z, HAMMER_ROT_Y, HAMMER_ROT_X};

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
  nextFrame2();
}

void step_back() {
  curTime -= tStep;
  nextFrame2();
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
  
  dofs[HAMMER_X].curValue = 14;
  dofs[HAMMER_X].name = "Sphere Y Position";
  dofs[HAMMER_X].nKeyframes = 9;
  dofs[HAMMER_X].keys = new Keyframe[9];
//  dofs[HAMMER_X].bezier = true;
  dofs[HAMMER_X].keys[0].set(1, 14);
  dofs[HAMMER_X].keys[1].set(1.25, 7);
  dofs[HAMMER_X].keys[2].set(1.5, 0);
  dofs[HAMMER_X].keys[3].set(1.5, 0);
  dofs[HAMMER_X].keys[4].set(1.5, 0);
  dofs[HAMMER_X].keys[5].set(1.75, 7);
  dofs[HAMMER_X].keys[6].set(2, 14);
  dofs[HAMMER_X].keys[7].set(2.1, 14);
  dofs[HAMMER_X].keys[8].set(2.2, 14);

  dofs[HAMMER_Y].curValue = 20;
  dofs[HAMMER_Y].name = "Sphere Y Position";
  dofs[HAMMER_Y].nKeyframes = 9;
  dofs[HAMMER_Y].keys = new Keyframe[9];
//  dofs[HAMMER_Y].bezier = true;
  dofs[HAMMER_Y].keys[0].set(1, 20);
  dofs[HAMMER_Y].keys[1].set(1.25, 13);
  dofs[HAMMER_Y].keys[2].set(1.5, 6);
  dofs[HAMMER_Y].keys[3].set(1.5, 6);
  dofs[HAMMER_Y].keys[4].set(1.5, 6);
  dofs[HAMMER_Y].keys[5].set(1.75, 13);
  dofs[HAMMER_Y].keys[6].set(2, 20);
  dofs[HAMMER_Y].keys[7].set(2.1, 20);
  dofs[HAMMER_Y].keys[8].set(2.2, 20);
  
  dofs[HAMMER_Z].curValue = -20;
  dofs[HAMMER_Z].name = "Sphere Y Position";
  dofs[HAMMER_Z].nKeyframes = 9;
  dofs[HAMMER_Z].keys = new Keyframe[9];
//  dofs[HAMMER_Z].bezier = true;
  dofs[HAMMER_Z].keys[0].set(1, -20);
  dofs[HAMMER_Z].keys[1].set(1.25, -17.5);
  dofs[HAMMER_Z].keys[2].set(1.5, -15);
  dofs[HAMMER_Z].keys[3].set(1.5, -15);
  dofs[HAMMER_Z].keys[4].set(1.5, -15);
  dofs[HAMMER_Z].keys[5].set(1.75, -17.5);
  dofs[HAMMER_Z].keys[6].set(2, -20);
  dofs[HAMMER_Z].keys[7].set(2.1, -20);
  dofs[HAMMER_Z].keys[8].set(2.2, -20);
  
   dofs[HAMMER_ROT_Y].curValue = 90;
  dofs[HAMMER_ROT_Y].name = "Sphere Y Position";
  dofs[HAMMER_ROT_Y].nKeyframes = 5;
  dofs[HAMMER_ROT_Y].keys = new Keyframe[5];
//  dofs[HAMMER_ROT_Y].bezier = false;
  dofs[HAMMER_ROT_Y].keys[0].set(1, 90);
  dofs[HAMMER_ROT_Y].keys[1].set(1.5, 0);
  dofs[HAMMER_ROT_Y].keys[2].set(1.75, 45);
  dofs[HAMMER_ROT_Y].keys[3].set(2.0, 90);
  dofs[HAMMER_ROT_Y].keys[4].set(2.1, 90);
  
  dofs[HAMMER_ROT_X].curValue = -50;
  dofs[HAMMER_ROT_X].name = "Sphere Y Position";
  dofs[HAMMER_ROT_X].nKeyframes = 6;
  dofs[HAMMER_ROT_X].keys = new Keyframe[6];
//  dofs[HAMMER_ROT_X].bezier = false;
  dofs[HAMMER_ROT_X].keys[0].set(1, -50);
  dofs[HAMMER_ROT_X].keys[1].set(1.25, -25);
  dofs[HAMMER_ROT_X].keys[2].set(1.5, 0);
  dofs[HAMMER_ROT_X].keys[3].set(1.75, -25);
  dofs[HAMMER_ROT_X].keys[4].set(2.0, -50);
  dofs[HAMMER_ROT_X].keys[5].set(2.1, -50);
  
 
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
  
  LoadGLTextures();				// Load The Texture(s) 
  glEnable(GL_TEXTURE_2D);			// Enable Texture Mapping
  glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	
  GLfloat ambient[] = {0.6, 0.6, 0.6, 1.0 };
  GLfloat diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat position[] = { 0.0, 3.0, 12.0, 0.0 };
    
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
	
  aardvark=gluNewQuadric();                      // Create A Pointer To The Quadric Object ( NEW )
  gluQuadricNormals(aardvark, GLU_SMOOTH);       // Create Smooth Normals ( NEW )
  gluQuadricTexture(aardvark, GL_TRUE);          // Create Texture Coords ( NEW )

 
}

/*********************************************************
    PROC: set_colour();
    DOES: sets all material properties to the given colour -- don't change
**********************************************************/

void set_colour(float r, float g, float b, float ambient, float diffuse, float specular) {
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

void set_colour(float r, float g, float b){
	set_colour(r, g, b, 0.2, 0.7, 0.4);
}

void drawStairs(){
	

	glBindTexture(GL_TEXTURE_2D, texture[0]);   // choose the texture to use.
	set_colour(1.0f, 1.0f, 1.0f, 0.7f, 0.7f, 0.1f);
	//set_colour(0.4f, 0.4f, 0.4f, 0.7f, 0.7f, 0.1f);
	
	for(int i = 1; i <= 6; i++){
		
		glBegin(GL_QUADS);
		
		glNormal3f(0,1,0);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-10.0f, i - 1 , -2 * (i - 1));
		glNormal3f(0,1,0);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(10.0f, i - 1, -2 * (i - 1));
		glNormal3f(0,1,0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(10.0f, i, -2 * (i - 1));
		glNormal3f(0,1,0);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-10.0f, i, -2 * (i - 1));
		glEnd();
		
		
		glBegin(GL_QUADS);
		glNormal3f(0,0,1);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-10.0f, i, -2 * (i-1));
		glNormal3f(0,0,1);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(10.0f, i, -2 * (i-1));
		glNormal3f(0,0,1);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(10.0f, i, -2 * i);
		glNormal3f(0,0,1);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-10.0f, i, -2 * i);
		glEnd();
	
	}
}
void drawColumnShaft(){

	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	gluCylinder(aardvark, 1.2, 1.2, 1.0, 32, 32);
	glPopMatrix();

}


void drawColumn(float shaftHeight){
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[0]);   // choose the texture to use.
	
	glPushMatrix();
	
	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	//glRotatef(90,0,1,0);
	gluCylinder(aardvark, 2.0, 2.0, 2.0, 32, 32);
	gluDisk(aardvark, 0, 2, 32, 32);
	glTranslatef(0, 0, 2);
	gluDisk(aardvark, 0, 2, 32, 32);
	glPopMatrix();
	
	glPushMatrix();
	glScalef(1.0, shaftHeight, 1.0);
	drawColumnShaft();
	glPopMatrix();
	
	glTranslatef(0,shaftHeight, 0);
	
	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	//glRotatef(90,0,1,0);
	gluCylinder(aardvark, 2.0, 2.0, 2.0, 32, 32);
	gluDisk(aardvark, 0, 2, 32, 32);
	glTranslatef(0, 0, 2);
	gluDisk(aardvark, 0, 2, 32, 32);
	glPopMatrix();
	
	glPopMatrix();
	
	glDisable(GL_TEXTURE_2D);
}



void drawStairs2(){
	
	glBindTexture(GL_TEXTURE_2D, texture[3]);   // choose the texture to use.
	set_colour(1.0f, 1.0f, 1.0f, 0.7f, 0.7f, 0.1f);
	
	for(int i=0; i < 7; i++){
	
		glPushMatrix();
		glScalef(20, 0.1, 2.0);
		glutSolidCube(1.0);
		glPopMatrix();
	
		glTranslatef(0, 0, -1);
		
		glPushMatrix();
		glScalef(20, 1.0, 0.1);
		glutSolidCube(1.0);
		glPopMatrix();
		
		glTranslatef(0, -1, 0);
	}
}


void drawHammer(){
	glBindTexture(GL_TEXTURE_2D, texture[0]);   // choose the texture to use.
	
	glPushMatrix();
	glTranslatef(2,5,0);
	
	glTranslatef(dofs[HAMMER_X].curValue, dofs[HAMMER_Y].curValue, dofs[HAMMER_Z].curValue);
	glRotatef(-180, 0, 0, 1);
	glRotatef(dofs[HAMMER_ROT_Y].curValue, 0, 1, 0);
	glRotatef(dofs[HAMMER_ROT_X].curValue, 1, 0, 0);
	
        //Shaft
	glPushMatrix();

	glRotatef(-90, 0,1,0);
	glTranslatef(0, 0, -2);
	gluCylinder(aardvark, 0.2, 0.2, 4.0, 32, 32);
	glPushMatrix();
	glScalef(1.0, 1.0, 0.5);
	gluSphere(aardvark, 0.2, 32, 32);
	glPopMatrix();
	glTranslatef(0, 0, 4.0);
	glPushMatrix();
	glScalef(1.0, 1.0, 0.5);
	gluSphere(aardvark, 0.2, 32, 32);
	glPopMatrix();

	glPopMatrix();
	
	//Head
	glPushMatrix();

	glTranslatef(1.2, 0, -1);
	gluCylinder(aardvark, 0.8, 0.8, 2.0, 32, 32);
	glPushMatrix();
	glScalef(1.0,1.0, 0.5);
	gluSphere(aardvark, 0.8, 32, 32);
	glPopMatrix();
	glTranslatef(0, 0, 2.0);
	glPushMatrix();
	glScalef(1.0, 1.0, 0.5);
	gluSphere(aardvark, 0.8, 32, 32);
	glPopMatrix();
	
	glPopMatrix();
	
	glPopMatrix();
	
}

void drawPlatform(){
	
	glBindTexture(GL_TEXTURE_2D, texture[0]);   // choose the texture to use.
	
	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	glTranslatef(0, 0, -2.0);
	gluCylinder(aardvark, 1.0, 1.0, 4.0, 32, 32);
	
	glPushMatrix();
	glTranslatef(0,0,4);
	gluCylinder(aardvark, 2.5, 2.5, 0.4, 32, 32);
	
	glPushMatrix();
	glScalef(1.0, 1.0, 0.05);
	gluSphere(aardvark, 2.5, 32, 32);
	glPopMatrix();
	
	glTranslatef(0,0,0.4);
	
	glPushMatrix();
	glScalef(1.0, 1.0, 0.05);
	gluSphere(aardvark, 2.5, 32, 32);
	glPopMatrix();
	
	glPopMatrix();
	
	glPopMatrix();

}

void drawSphere(){
	 glBindTexture(GL_TEXTURE_2D, texture[2]);   // choose the texture to use.
	 set_colour(0.3f,0.3f,0.3f, 0.7f, 0.2f, 1.0f);
	 glPushMatrix();
	 glTranslatef(0.0f, 8.9f, -12.0f);
	 gluSphere(aardvark, 2.8, 32, 32);
	 glPopMatrix();
	 
}

void drawCube(){
	set_colour(0.3f,0.3f,0.3f, 0.7f, 0.6f, 0.6f);
	glBindTexture(GL_TEXTURE_2D, texture[1]);   // choose the texture to use.
	 
	glPushMatrix();
	glTranslatef(-6.5f, 8.3f, -12.0f);
	glRotatef(45, 0.0f, 1.0f, 0.0f);
	glutSolidCube(4.5f);
	glPopMatrix();
	
}

void drawPyramid(){
	glBindTexture(GL_TEXTURE_2D, texture[3]);   // choose the texture to use.
	 
	set_colour(0.5f,0.5f,0.5f, 0.7f, 0.6f, 0.6f);
	 
	glPushMatrix();
	glTranslatef(6.5f, 8.6f, -12.0f);
	glScalef(2.6f, 2.6f, 2.6f);
	glRotatef(45, 0, 1, 0);
	
	glBegin(GL_TRIANGLES);                                  // Start Drawing The Pyramid
        glVertex3f( 0.0f, 1.0f, 0.0f);                  // Top Of Triangle (Front)
        glVertex3f(-1.0f,-1.0f, 1.0f);                  // Left Of Triangle (Front)
        glVertex3f( 1.0f,-1.0f, 1.0f);                  // Right Of Triangle (Front)
        glVertex3f( 0.0f, 1.0f, 0.0f);                  // Top Of Triangle (Right)
        glVertex3f( 1.0f,-1.0f, 1.0f);                  // Left Of Triangle (Right)
        glVertex3f( 1.0f,-1.0f, -1.0f);                 // Right Of Triangle (Right)
        glVertex3f( 0.0f, 1.0f, 0.0f);                  // Top Of Triangle (Back)
        glVertex3f( 1.0f,-1.0f, -1.0f);                 // Left Of Triangle (Back)
        glVertex3f(-1.0f,-1.0f, -1.0f);                 // Right Of Triangle (Back)
        glVertex3f( 0.0f, 1.0f, 0.0f);                  // Top Of Triangle (Left)
        glVertex3f(-1.0f,-1.0f,-1.0f);                  // Left Of Triangle (Left)
        glVertex3f(-1.0f,-1.0f, 1.0f);                  // Right Of Triangle (Left)
        glEnd();  

	glPopMatrix();
}

void drawPalm(){
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);   // choose the texture to use.
	
	glPushMatrix();
	
	
	glPushMatrix();
	glScalef(0.7,1.0,0.4);
	gluSphere(aardvark, 1,32,32);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(-0.45,0,0);
	glRotatef(-90,1,0,0);
	glRotatef(-45,0,1,0);
	gluCylinder(aardvark, 0.3,0.08,1.2,32,32);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(-0.2,0.25,0.0);
	glRotatef(-90,1,0,0);
	glRotatef(-20, 0,1,0);
	gluCylinder(aardvark, 0.3, 0.08,1.4,32,32);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(0,0.6,0);
	glRotatef(-90,1,0,0);
	gluCylinder(aardvark, 0.25, 0.08, 1.4, 32, 32);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(0.2,0.25,0.0);
	glRotatef(-90,1,0,0);
	glRotatef(20, 0, 1, 0);
	gluCylinder(aardvark, 0.3, 0.08, 1.4, 32, 32);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(0.1, -0.25, 0.0);
	glRotatef(-90, 1, 0, 0);
	glRotatef(40, 0, 1, 0);
	gluCylinder(aardvark, 0.3, 0.08, 1.4, 32, 32);
	glPopMatrix();
	
	glPopMatrix();
	
	glEnable(GL_TEXTURE_2D);
	
}

void drawMan(){
        
	//torso
	glPushMatrix();
	glRotatef(-90.0f,1.0f,0.0f,0.0f);
	gluCylinder(aardvark, 0.4f, 0.6f, 1.2f, 32, 32);
	glPopMatrix();
	
	//right shoulder
	glPushMatrix();
	glTranslatef(-0.6f, 1.1f, 0.0f);
	gluSphere(aardvark, 0.25, 32, 32);
	glPopMatrix();
	
	//right upper arm
	glPushMatrix();
	glTranslatef(-0.6f, 1.1f, 0.0f);
	glRotatef(90,1.0,0.0,0.0);
	gluCylinder(aardvark,0.23f, 0.1f, 0.7, 32, 32); 
	glPopMatrix();
	
	//right elbow
	glPushMatrix();
	glTranslatef(-0.6, 0.4, 0.0);
	gluSphere(aardvark, 0.12, 32, 32);
	glPopMatrix();
	
	//right forearm
	glPushMatrix();
	glTranslatef(-0.6,0.4,0.0);
	glRotatef(90,1.0,0.0,0.0);
	gluCylinder(aardvark, 0.11, 0.04, 0.7, 32, 32);
	glPopMatrix();
	
	//right palm
	glPushMatrix();
	glTranslatef(-0.6,-0.4,0.0);
	glRotatef(180,0,0,1);
	glRotatef(180,0,1,0);
	glScalef(0.12,0.12,0.12);
	drawPalm();
	glPopMatrix();
	
	
	//left shoulder
	glPushMatrix();
	glTranslatef(0.6f, 1.1f, 0.0f);
	gluSphere(aardvark, 0.25, 32, 32);
	glPopMatrix();
	
	
	//left upper arm
	glPushMatrix();
	glTranslatef(0.6f, 1.1f, 0.0f);
	glRotatef(90,1.0,0.0,0.0);
	gluCylinder(aardvark,0.23f, 0.1f, 0.7, 32, 32); 
	glPopMatrix();
	
	//left elbow
	glPushMatrix();
	glTranslatef(0.6, 0.4, 0.0);
	gluSphere(aardvark, 0.12, 32, 32);
	glPopMatrix();
	
	//left forearm
	glPushMatrix();
	glTranslatef(0.6,0.4,0.0);
	glRotatef(90,1.0,0.0,0.0);
	gluCylinder(aardvark, 0.11, 0.04, 0.7, 32, 32);
	glPopMatrix();
	
	//left palm
	glPushMatrix();
	glTranslatef(0.6,-0.4,0.0);
	glRotatef(180,0,0,1);
	glScalef(0.12,0.12,0.12);
	drawPalm();
	glPopMatrix();
	
	
	//neck
        glPushMatrix();
	glTranslatef(0.0f, 1.2f, 0.0f);
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	gluCylinder(aardvark, 0.2f, 0.2f, 0.3f, 32, 32);
	glPopMatrix();
	
	
	//head
	glPushMatrix();
	glTranslatef(0.0f, 1.7f, 0.0f);
	glRotatef(-90,1,0,0);
	gluSphere(aardvark, 0.4, 32, 32);
	glPopMatrix();
	
	//ass
	glPushMatrix();
	glTranslatef(0.0f, -0.5f, 0.0f);
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	gluCylinder(aardvark, 0.53f, 0.4f, 0.5f, 32, 32);
	glPopMatrix();
	
//	glPushMatrix();
//	glTranslatef(0,0.05,0.2);
//	glRotatef(20, 1,0,0);
	
	//right thigh
	glPushMatrix();
	glTranslatef(-0.225f, -0.5f, 0.0f);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	gluCylinder(aardvark, 0.28f, 0.1f, 0.8f, 32, 32);
	glPopMatrix();
	
	//left thigh
	glPushMatrix();
	glTranslatef(0.225f, -0.5f, 0.0f);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	gluCylinder(aardvark, 0.28f, 0.1f, 0.8f, 32, 32);
	glPopMatrix();
	
	//right knee
	glPushMatrix();
	glTranslatef(-0.225f, -1.35f, 0.0f);
	gluSphere(aardvark, 0.15f, 32, 32);
	glPopMatrix();
	
	
	//glPushMatrix();
	//glRotatef(-30, 1, 0, 0);
	
	//right calf
	glPushMatrix();
	glTranslatef(-0.225f, -1.4f, 0.0f);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	gluCylinder(aardvark, 0.11f, 0.04f, 0.6f, 32, 32);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(0,-1.33,0);
	glRotatef(60, 1, 0, 0);
	glTranslatef(0, 1.33, 0);
	
	//left nee
	glPushMatrix();
	glTranslatef(0.225f, -1.35f, 0.0f);
	gluSphere(aardvark, 0.15f, 32, 32);
	glPopMatrix();
	
	//left calf
	glPushMatrix();
	glTranslatef(0.225f, -1.4f, 0.0f);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	gluCylinder(aardvark, 0.11f, 0.04f, 0.6f, 32, 32);
	glPopMatrix();
	
	//right foot
	glPushMatrix();
	glTranslatef(-0.225f, -2.0f, 0.1f);
	glScalef(0.1,0.05,0.2);
	glutSolidCube(2.0);
	glPopMatrix();
	
	glPopMatrix();
	
	
	//left foot
	glPushMatrix();
	glTranslatef(0.225f, -2.0f, 0.1f);
	glScalef(0.1,0.05,0.2);
	glutSolidCube(2.0);
	glPopMatrix();
	
	//glPopMatrix();
	
//	glPopMatrix();
	
}

/*********************************************************
    PROC: display()
    DOES: this gets called by the event handler to draw
          the scene, so this is where you need to build
	  your scene -- make your changes and additions here
	  Add other procedures if you like.
**********************************************************/

void display(void) {
  
  glLoadIdentity();
  glClearColor(0.0f,0.0f,0.0f,1.0f);   //set the background colour 
  // OK, now clear the screen with the background colour 
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
	//back view
	//gluLookAt (0, -20, -0.1,  0.0,0.0,0.0, 0.0,1.0,0.0);
	
	//front view
	//gluLookAt (0, 55, -17.5,  0.0,0, -15.5, 0.0,1.0,0.0);
	//  gluLookAt(-15.5, 0, 0, 0, 0, 0, 0, 1, 0);
	//gluLookAt(0,5,0,0,0,0,0,1,0);
	gluLookAt(0,0,0,14,20,-20,0,1,0);
	
	//left view
	//gluLookAt (-45, 7, -10,  0.0,7.0,-10.0, 0.0,1.0,0.0);
	
	//right view
	//gluLookAt (10, 0, 0,  0.0,0.0,0.0, 0.0,1.0,0.0);
	
	
	
//	set_colour(1, 0.5f, 0.5f);  // pink 
 
//	drawStairs();
//	drawCube();
//	drawSphere();
//	drawPyramid();
 	
//	glPushMatrix();
//	glTranslatef(0.0f, 3.45f, -2.5f);
	//glRotatef(20, 1,0,0);
/*	glPushMatrix();
	glRotatef(180,0,1,0);
	drawMan();
	glPopMatrix();
// 	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(-2,0,0);
	glRotatef(90,0,1,0);
	drawMan();
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(2,0,0);
	glRotatef(-90,0,1,0);
	drawMan();
	glPopMatrix();
*/
	drawHammer();
	
/*	glPushMatrix();
	glTranslatef(dofs[HAMMER_X].curValue, dofs[HAMMER_Y].curValue, dofs[HAMMER_Z].curValue);
	drawPalm();
	glPopMatrix();
	
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0,0,-20);
	drawPalm();
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(0,0,-15);
	drawPalm();
	glPopMatrix();
	
	//drawStairs();
	//drawColumn(8.0);*/
	
	//drawMan();
	
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
//  gluLookAt(eye[X],eye[Y],eye[Z],   0,0,0,   0,1,0);
gluLookAt (-5, 5, 40,  0.0,0.0, 0.0, 0.0,1.0,0.0);
	
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
      nextFrame2();
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

void nextFrame2() {

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
    float v1, v2, v3, v4, v5;
    for (int ikey=0; ikey < n-3; ikey++) {
      tStart = dofs[idof].keys[ikey]  .getTime();
      tEnd   = dofs[idof].keys[ikey+3].getTime();
      if (tStart <= curTime && curTime <= tEnd) {
	curInterval = ikey;
	v1 = dofs[idof].keys[ikey]  .getValue();
	v2   = dofs[idof].keys[ikey+1].getValue();
	v3 = dofs[idof].keys[ikey+2].getValue();
	v4 = dofs[idof].keys[ikey+3].getValue();
	//cout << "v1=" << v1 << " v2=" << v2 << " v3=" << v3 << " v4=" << v4 << endl;
	break;
      }
    }
    double t = (curTime - tStart)/(tEnd - tStart);
    dofs[idof].curValue = (1-t)*(1-t)*(1-t)*v1 + 3*t*(1-t)*(1-t)*v2 + 3 *
    t*t*(1-t)*v3+t*t*t*v4;
    cout << "nextFrame. curValue="<<dofs[idof].curValue<<endl << endl;

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
  glutInitWindowPosition (400, 400);
 // glutInitWindowSize(196,196);
  glutInitWindowSize(600,600);
  
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
/*
  glPushMatrix();
	glRotatef(-90.0f,1.0f,0.0f,0.0f);
	gluCylinder(aardvark, 0.4f, 0.6f, 1.2f, 32, 32);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(-0.6f, 1.1f, 0.0f);
	gluSphere(aardvark, 0.25, 32, 32);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(0.6f, 1.1f, 0.0f);
	gluSphere(aardvark, 0.25, 32, 32);
	glPopMatrix();
	
  glPushMatrix();
	glTranslatef(0.0f, 1.2f, 0.0f);
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	gluCylinder(aardvark, 0.2f, 0.2f, 0.3f, 32, 32);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(0.0f, 1.7f, 0.0f);
	gluSphere(aardvark, 0.4, 32, 32);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(0.0f, -0.5f, 0.0f);
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	gluCylinder(aardvark, 0.45f, 0.4f, 0.5f, 32, 32);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(-0.225f, -0.5f, 0.0f);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	gluCylinder(aardvark, 0.25f, 0.1f, 0.8f, 32, 32);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(0.225f, -0.5f, 0.0f);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	gluCylinder(aardvark, 0.25f, 0.1f, 0.8f, 32, 32);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(-0.225f, -1.35f, 0.0f);
	gluSphere(aardvark, 0.15f, 32, 32);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(0.225f, -1.35f, 0.0f);
	gluSphere(aardvark, 0.15f, 32, 32);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(-0.225f, -1.4f, 0.0f);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	gluCylinder(aardvark, 0.11f, 0.04f, 0.6f, 32, 32);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(0.225f, -1.4f, 0.0f);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	gluCylinder(aardvark, 0.11f, 0.04f, 0.6f, 32, 32);
	glPopMatrix();
*/
