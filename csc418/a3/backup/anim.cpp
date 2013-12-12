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
  bool bezier;
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
bool bezier = false;
int imageWindowID;

double eye[3] = {-5, 5, 40.0};

/* storage for one texture  */
int texture[4];

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
    Image *image1, *image2, *image3, *image4;
    
    
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

    if (!ImageLoad("wood.bmp", image2)) {
	exit(1);
    }        
    
     // allocate space for texture
    image3 = (Image *) malloc(sizeof(Image));
    if (image3 == NULL) {
	printf("Error allocating space for image");
	exit(0);
    }

    if (!ImageLoad("lava.bmp", image3)) {
	exit(1);
    }        
    
     // allocate space for texture
    image4 = (Image *) malloc(sizeof(Image));
    if (image4 == NULL) {
	printf("Error allocating space for image");
	exit(0);
    }

    if (!ImageLoad("tetra.bmp", image4)) {
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


};
//////////////////////////////////////////////////////
// ***************************************************
// * You should change these lines to reflect the number
// * of degrees of freedom in your animation
// ***************************************************
//////////////////////////////////////////////////////

int nDOF = 16;
DegreeOfFreedom dofs[16];
enum {LEFT_SHOULDERX, LEFT_ELBOW, RIGHT_SHOULDERX, RIGHT_ELBOW, NECK_ANGLE, PLATFORM, SPHERE_Y, HAMMER_ROT_Y, HAMMER_ROT_X, HAMMER_X, HAMMER_Y, HAMMER_Z, SPHERE_Z, MAN_Z, MAN_Y, MAN_ROT_X};

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
  
  dofs[RIGHT_SHOULDERX].curValue = 0;
  dofs[RIGHT_SHOULDERX].name = "Left Shoulder X Angle";
  dofs[RIGHT_SHOULDERX].nKeyframes = 21;
  dofs[RIGHT_SHOULDERX].keys = new Keyframe[21];
  dofs[RIGHT_SHOULDERX].bezier = false;
  dofs[RIGHT_SHOULDERX].keys[0].set(0, 0);
  dofs[RIGHT_SHOULDERX].keys[1].set(0.1, 0);
  dofs[RIGHT_SHOULDERX].keys[2].set(1,165);
  dofs[RIGHT_SHOULDERX].keys[3].set(2.0,165);
  dofs[RIGHT_SHOULDERX].keys[4].set(2.2,80);
  dofs[RIGHT_SHOULDERX].keys[5].set(2.5,0);
  dofs[RIGHT_SHOULDERX].keys[6].set(3.0,0);
  dofs[RIGHT_SHOULDERX].keys[7].set(3.1, 0);
  dofs[RIGHT_SHOULDERX].keys[8].set(4,165);
  dofs[RIGHT_SHOULDERX].keys[9].set(5.0,165);
  dofs[RIGHT_SHOULDERX].keys[10].set(5.2,80);
  dofs[RIGHT_SHOULDERX].keys[11].set(5.5,0);
  dofs[RIGHT_SHOULDERX].keys[12].set(6.0,0);
  dofs[RIGHT_SHOULDERX].keys[13].set(6.1, 0);
  dofs[RIGHT_SHOULDERX].keys[14].set(7,165);
  dofs[RIGHT_SHOULDERX].keys[15].set(8.0,165);
  dofs[RIGHT_SHOULDERX].keys[16].set(8.2,80);
  dofs[RIGHT_SHOULDERX].keys[17].set(8.5,0);
  dofs[RIGHT_SHOULDERX].keys[18].set(10.85,0);
  dofs[RIGHT_SHOULDERX].keys[19].set(10.9,180);
  dofs[RIGHT_SHOULDERX].keys[20].set(11.0,180);

   
  dofs[RIGHT_ELBOW].curValue = 0;
  dofs[RIGHT_ELBOW].name = "Left Elbow Angle";
  dofs[RIGHT_ELBOW].nKeyframes = 22;
  dofs[RIGHT_ELBOW].keys = new Keyframe[22];
  dofs[RIGHT_ELBOW].bezier = false;
  dofs[RIGHT_ELBOW].keys[0].set(0, 0);
  dofs[RIGHT_ELBOW].keys[1].set(1,50);
  dofs[RIGHT_ELBOW].keys[2].set(1.2, 10);
  dofs[RIGHT_ELBOW].keys[3].set(1.5, 60); 
  dofs[RIGHT_ELBOW].keys[4].set(1.6, 10); 
  dofs[RIGHT_ELBOW].keys[5].set(2.3, 10);
  dofs[RIGHT_ELBOW].keys[6].set(2.5, 0); 
  dofs[RIGHT_ELBOW].keys[7].set(3.0, 0);
  dofs[RIGHT_ELBOW].keys[8].set(4,50);
  dofs[RIGHT_ELBOW].keys[9].set(4.2, 10);
  dofs[RIGHT_ELBOW].keys[10].set(4.5, 60); 
  dofs[RIGHT_ELBOW].keys[11].set(4.6, 10); 
  dofs[RIGHT_ELBOW].keys[12].set(5.3, 10);
  dofs[RIGHT_ELBOW].keys[13].set(5.5, 0); 
  dofs[RIGHT_ELBOW].keys[14].set(6.0, 0);
  dofs[RIGHT_ELBOW].keys[15].set(7,50);
  dofs[RIGHT_ELBOW].keys[16].set(7.2, 10);
  dofs[RIGHT_ELBOW].keys[17].set(7.5, 60); 
  dofs[RIGHT_ELBOW].keys[18].set(7.6, 10); 
  dofs[RIGHT_ELBOW].keys[19].set(8.3, 10);
  dofs[RIGHT_ELBOW].keys[20].set(8.5, 0); 
  dofs[RIGHT_ELBOW].keys[21].set(9.0, 0);

  
  dofs[LEFT_SHOULDERX].curValue = 0;
  dofs[LEFT_SHOULDERX].name = "Right Shoulder X Angle";
  dofs[LEFT_SHOULDERX].nKeyframes = 21;
  dofs[LEFT_SHOULDERX].keys = new Keyframe[21];
  dofs[LEFT_SHOULDERX].bezier = false;
  dofs[LEFT_SHOULDERX].keys[0].set(0, 0);
  dofs[LEFT_SHOULDERX].keys[1].set(0.1, 0);
  dofs[LEFT_SHOULDERX].keys[2].set(1,145);
  dofs[LEFT_SHOULDERX].keys[3].set(2.0,145);
  dofs[LEFT_SHOULDERX].keys[4].set(2.2,70);
  dofs[LEFT_SHOULDERX].keys[5].set(2.5,0);
  dofs[LEFT_SHOULDERX].keys[6].set(3.0,0);
  dofs[LEFT_SHOULDERX].keys[7].set(3.1, 0);
  dofs[LEFT_SHOULDERX].keys[8].set(4,145);
  dofs[LEFT_SHOULDERX].keys[9].set(5.0,145);
  dofs[LEFT_SHOULDERX].keys[10].set(5.2,70);
  dofs[LEFT_SHOULDERX].keys[11].set(5.5,0);
  dofs[LEFT_SHOULDERX].keys[12].set(6.0,0);
  dofs[LEFT_SHOULDERX].keys[13].set(6.1, 0);
  dofs[LEFT_SHOULDERX].keys[14].set(7,145);
  dofs[LEFT_SHOULDERX].keys[15].set(8.0,145);
  dofs[LEFT_SHOULDERX].keys[16].set(8.2,70);
  dofs[LEFT_SHOULDERX].keys[17].set(8.5,0);
  dofs[LEFT_SHOULDERX].keys[18].set(10.85,0);
  dofs[LEFT_SHOULDERX].keys[19].set(10.9,180);
  dofs[LEFT_SHOULDERX].keys[20].set(11.0,180);
  
 
  dofs[LEFT_ELBOW].curValue = 0;
  dofs[LEFT_ELBOW].name = "Right Elbow Angle";
  dofs[LEFT_ELBOW].nKeyframes = 22;
  dofs[LEFT_ELBOW].keys = new Keyframe[22];
  dofs[LEFT_ELBOW].bezier = false;
  dofs[LEFT_ELBOW].keys[0].set(0, 0);
  dofs[LEFT_ELBOW].keys[1].set(1,60);
  dofs[LEFT_ELBOW].keys[2].set(1.2, 10);
  dofs[LEFT_ELBOW].keys[3].set(1.5, 50);
  dofs[LEFT_ELBOW].keys[4].set(1.6, 10);
  dofs[LEFT_ELBOW].keys[5].set(2.3, 10);
  dofs[LEFT_ELBOW].keys[6].set(2.5, 0);
  dofs[LEFT_ELBOW].keys[7].set(3.0, 0);
  dofs[LEFT_ELBOW].keys[8].set(4,60);
  dofs[LEFT_ELBOW].keys[9].set(4.2, 10);
  dofs[LEFT_ELBOW].keys[10].set(4.5, 50);
  dofs[LEFT_ELBOW].keys[11].set(4.6, 10);
  dofs[LEFT_ELBOW].keys[12].set(5.3, 10);
  dofs[LEFT_ELBOW].keys[13].set(5.5, 0);
  dofs[LEFT_ELBOW].keys[14].set(6.0, 0);
  dofs[LEFT_ELBOW].keys[15].set(7,60);
  dofs[LEFT_ELBOW].keys[16].set(7.2, 10);
  dofs[LEFT_ELBOW].keys[17].set(7.5, 50);
  dofs[LEFT_ELBOW].keys[18].set(7.6, 10);
  dofs[LEFT_ELBOW].keys[19].set(8.3, 10);
  dofs[LEFT_ELBOW].keys[20].set(8.5, 0);
  dofs[LEFT_ELBOW].keys[21].set(9.0, 0);
  
  
  dofs[NECK_ANGLE].curValue = 0;
  dofs[NECK_ANGLE].name = "Neck and Head Andle";
  dofs[NECK_ANGLE].nKeyframes = 22;
  dofs[NECK_ANGLE].keys = new Keyframe[22];
  dofs[NECK_ANGLE].bezier = false;
  dofs[NECK_ANGLE].keys[0].set(0, 0);
  dofs[NECK_ANGLE].keys[1].set(0.2, 25);
  dofs[NECK_ANGLE].keys[2].set(1.0, 25);
  dofs[NECK_ANGLE].keys[3].set(1.05, -15);
  dofs[NECK_ANGLE].keys[4].set(1.1, -25);
  dofs[NECK_ANGLE].keys[5].set(1.7, -25);
  dofs[NECK_ANGLE].keys[6].set(1.9, 0);
  dofs[NECK_ANGLE].keys[7].set(3.0, 0);
  dofs[NECK_ANGLE].keys[8].set(3.2, 25);
  dofs[NECK_ANGLE].keys[9].set(4.0, 25);
  dofs[NECK_ANGLE].keys[10].set(4.05, -15);
  dofs[NECK_ANGLE].keys[11].set(4.1, -25);
  dofs[NECK_ANGLE].keys[12].set(4.7, -25);
  dofs[NECK_ANGLE].keys[13].set(4.9, 0);
  dofs[NECK_ANGLE].keys[14].set(6.0, 0);
  dofs[NECK_ANGLE].keys[15].set(6.2, 25);
  dofs[NECK_ANGLE].keys[16].set(7.0, 25);
  dofs[NECK_ANGLE].keys[17].set(7.05, -15);
  dofs[NECK_ANGLE].keys[18].set(7.1, -25);
  dofs[NECK_ANGLE].keys[19].set(7.7, -25);
  dofs[NECK_ANGLE].keys[20].set(7.9, 0);
  dofs[NECK_ANGLE].keys[21].set(7.0, 0);

  dofs[PLATFORM].curValue = 3.6;
  dofs[PLATFORM].name = "Platform Position";
  dofs[PLATFORM].nKeyframes = 6;
  dofs[PLATFORM].keys = new Keyframe[6];
  dofs[PLATFORM].bezier = false;
  dofs[PLATFORM].keys[0].set(0, 3.6);
  dofs[PLATFORM].keys[1].set(9, 3.6);
  dofs[PLATFORM].keys[2].set(10, 5.6);
  dofs[PLATFORM].keys[3].set(10.4, 5.6);
  dofs[PLATFORM].keys[4].set(10.5, 3.6);
  dofs[PLATFORM].keys[5].set(10.6, 3.6);
  
 
  dofs[SPHERE_Y].curValue = 8.9;
  dofs[SPHERE_Y].name = "Sphere Y Position";
  dofs[SPHERE_Y].nKeyframes = 6;
  dofs[SPHERE_Y].keys = new Keyframe[6];
  dofs[SPHERE_Y].bezier = false;
  dofs[SPHERE_Y].keys[0].set(0, 8.9);
  dofs[SPHERE_Y].keys[1].set(9, 8.9);
  dofs[SPHERE_Y].keys[2].set(10, 10.9);
  dofs[SPHERE_Y].keys[3].set(10.5, 10.9);
  dofs[SPHERE_Y].keys[4].set(11.0, 3.9);
  dofs[SPHERE_Y].keys[5].set(11.1, 3.9);
  
  dofs[HAMMER_ROT_Y].curValue = 90;
  dofs[HAMMER_ROT_Y].name = "Sphere Y Position";
  dofs[HAMMER_ROT_Y].nKeyframes = 6;
  dofs[HAMMER_ROT_Y].keys = new Keyframe[6];
  dofs[HAMMER_ROT_Y].bezier = false;
  dofs[HAMMER_ROT_Y].keys[0].set(0, 90);
  dofs[HAMMER_ROT_Y].keys[1].set(10, 90);
  dofs[HAMMER_ROT_Y].keys[2].set(10.5, 0);
  dofs[HAMMER_ROT_Y].keys[3].set(10.75, 45);
  dofs[HAMMER_ROT_Y].keys[4].set(11.0, 90);
  dofs[HAMMER_ROT_Y].keys[5].set(11.1, 90);
  
  dofs[HAMMER_ROT_X].curValue = -50;
  dofs[HAMMER_ROT_X].name = "Sphere Y Position";
  dofs[HAMMER_ROT_X].nKeyframes = 7;
  dofs[HAMMER_ROT_X].keys = new Keyframe[7];
  dofs[HAMMER_ROT_X].bezier = false;
  dofs[HAMMER_ROT_X].keys[0].set(0, -50);
  dofs[HAMMER_ROT_X].keys[1].set(10, -50);
  dofs[HAMMER_ROT_X].keys[2].set(10.25, -25);
  dofs[HAMMER_ROT_X].keys[3].set(10.5, 0);
  dofs[HAMMER_ROT_X].keys[4].set(10.75, -25);
  dofs[HAMMER_ROT_X].keys[5].set(11.0, -50);
  dofs[HAMMER_ROT_X].keys[6].set(11.1, -50);
  
  
  dofs[HAMMER_X].curValue = 14;
  dofs[HAMMER_X].name = "Sphere Y Position";
  dofs[HAMMER_X].nKeyframes = 10;
  dofs[HAMMER_X].keys = new Keyframe[10];
  dofs[HAMMER_X].bezier = true;
  dofs[HAMMER_X].keys[0].set(0, 14);
  dofs[HAMMER_X].keys[1].set(10, 14);
  dofs[HAMMER_X].keys[2].set(10.25, 7);
  dofs[HAMMER_X].keys[3].set(10.5, 0);
  dofs[HAMMER_X].keys[4].set(10.5, 0);
  dofs[HAMMER_X].keys[5].set(10.5, 0);
  dofs[HAMMER_X].keys[6].set(10.75, 7);
  dofs[HAMMER_X].keys[7].set(11, 14);
  dofs[HAMMER_X].keys[8].set(11.1, 14);
  dofs[HAMMER_X].keys[9].set(11.2, 14);
 
  dofs[HAMMER_Y].curValue = 20;
  dofs[HAMMER_Y].name = "Sphere Y Position";
  dofs[HAMMER_Y].nKeyframes = 10;
  dofs[HAMMER_Y].keys = new Keyframe[10];
  dofs[HAMMER_Y].bezier = true;
  dofs[HAMMER_Y].keys[0].set(0, 20);
  dofs[HAMMER_Y].keys[1].set(10, 20);
  dofs[HAMMER_Y].keys[2].set(10.25, 13);
  dofs[HAMMER_Y].keys[3].set(10.5, 6);
  dofs[HAMMER_Y].keys[4].set(10.5, 6);
  dofs[HAMMER_Y].keys[5].set(10.5, 6);
  dofs[HAMMER_Y].keys[6].set(10.75, 13);
  dofs[HAMMER_Y].keys[7].set(11, 20);
  dofs[HAMMER_Y].keys[8].set(11.1, 20);
  dofs[HAMMER_Y].keys[9].set(11.2, 20);
  
  dofs[HAMMER_Z].curValue = -20;
  dofs[HAMMER_Z].name = "Sphere Y Position";
  dofs[HAMMER_Z].nKeyframes = 9;
  dofs[HAMMER_Z].keys = new Keyframe[9];
  dofs[HAMMER_Z].bezier = true;
  dofs[HAMMER_Z].keys[0].set(0, -20);
  dofs[HAMMER_Z].keys[0].set(10, -20);
  dofs[HAMMER_Z].keys[1].set(10.25, -17.5);
  dofs[HAMMER_Z].keys[2].set(10.5, -15);
  dofs[HAMMER_Z].keys[3].set(10.5, -15);
  dofs[HAMMER_Z].keys[4].set(10.5, -15);
  dofs[HAMMER_Z].keys[5].set(10.75, -17.5);
  dofs[HAMMER_Z].keys[6].set(11, -20);
  dofs[HAMMER_Z].keys[7].set(11.1, -20);
  dofs[HAMMER_Z].keys[8].set(11.2, -20);
  
  dofs[SPHERE_Z].curValue = -12;
  dofs[SPHERE_Z].name = "Sphere Y Position";
  dofs[SPHERE_Z].nKeyframes = 4;
  dofs[SPHERE_Z].keys = new Keyframe[4];
  dofs[SPHERE_Z].bezier = false;
  dofs[SPHERE_Z].keys[0].set(0.0, -12);
  dofs[SPHERE_Z].keys[1].set(10.5, -12);
  dofs[SPHERE_Z].keys[2].set(11.0, -2);
  dofs[SPHERE_Z].keys[3].set(11.5, -2);
  
  dofs[MAN_Z].curValue = -1.8;
  dofs[MAN_Z].name = "Sphere Y Position";
  dofs[MAN_Z].nKeyframes = 4;
  dofs[MAN_Z].keys = new Keyframe[4];
  dofs[MAN_Z].bezier = false;
  dofs[MAN_Z].keys[0].set(0.0, -1.8);
  dofs[MAN_Z].keys[1].set(10.85, -1.8);
  dofs[MAN_Z].keys[2].set(10.9, -1);
  dofs[MAN_Z].keys[3].set(11.0, -1);
 
  dofs[MAN_Y].curValue = 3.45;
  dofs[MAN_Y].name = "Sphere Y Position";
  dofs[MAN_Y].nKeyframes = 4;
  dofs[MAN_Y].keys = new Keyframe[4];
  dofs[MAN_Y].bezier = false;
  dofs[MAN_Y].keys[0].set(0.0, 3.45);
  dofs[MAN_Y].keys[1].set(10.85, 3.45);
  dofs[MAN_Y].keys[2].set(10.9, 1);
  dofs[MAN_Y].keys[3].set(11.0, 1);
  
  dofs[MAN_ROT_X].curValue = 0;
  dofs[MAN_ROT_X].name = "Sphere Y Position";
  dofs[MAN_ROT_X].nKeyframes = 4;
  dofs[MAN_ROT_X].keys = new Keyframe[4];
  dofs[MAN_ROT_X].bezier = false;
  dofs[MAN_ROT_X].keys[0].set(0.0, 0);
  dofs[MAN_ROT_X].keys[1].set(10.85, 0);
  dofs[MAN_ROT_X].keys[2].set(10.9, 90);
  dofs[MAN_ROT_X].keys[3].set(11.0, 90);
  
  tMax = 0;
  for (int idof = 0; idof<nDOF; idof++) {
    for (int ikey = 0; ikey<dofs[idof].nKeyframes; ikey++) {
      double t = dofs[idof].keys[ikey].getTime();
      if (t > tMax)
	    tMax = t;
    }
  }
  
//  tMax = 2;
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
  glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	
  GLfloat ambient[] = {.4, 0.4, 0.4, 1.0 };
  GLfloat diffuse[] = { 1, 1, 1, 1.0 };
  GLfloat specular[] = { 1, 1.0, 1.0, 1.0 };
  GLfloat position[] = { -0.0, 15.0, -5.0, 1.0 };
  
   GLfloat ambient2[] = {.4, 0.4, 0.4, 1.0 };
  GLfloat diffuse2[] = { 1, 1, 1, 1.0 };
  GLfloat specular2[] = { 1, 1.0, 1.0, 1.0 };
  GLfloat position2[] = { -12.0, 15.0, 2.0, 1.0 };
  
 GLfloat ambient3[] = {0.2, 0.2, 0.2, 1.0 };
  GLfloat diffuse3[] = { .1, .1, .1, 1.0 };
  GLfloat specular3[] = { .1, .1, .1, 1.0 };
  GLfloat position3[] = { 5.0, 25.0, -5.0, 1.0 };
 
    
  GLfloat lmodel_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
  GLfloat local_view[] = { 0.0 };

  
/*  GLfloat ambient2[] = {0.8, 0.8, 0.8, 1.0 };
  GLfloat diffuse2[] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat specular2[] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat position2[] = { 0.0, 15.0, -5.0, 1.0 };
  
  GLfloat spot_direction[] = { -4.0f, 3.45f, -1.8f };*/
  
  /**** set lighting parameters ****/
  glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
  glLightfv(GL_LIGHT0, GL_POSITION, position);
  
  glLightfv(GL_LIGHT1, GL_AMBIENT, ambient2);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse2);
  glLightfv(GL_LIGHT1, GL_POSITION, position2);
  
  glLightfv(GL_LIGHT2, GL_AMBIENT, ambient3);
  glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuse3);
  glLightfv(GL_LIGHT2, GL_POSITION, position3);
  
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
  glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, local_view);

/*spotlight
glLightfv(GL_LIGHT1, GL_AMBIENT, ambient2);
glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse2);
glLightfv(GL_LIGHT1, GL_POSITION, position2);
  
glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION,1.5);
glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.5);
glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.2);

glLightf(GL_LIGHT1,GL_SPOT_CUTOFF,20.0);
glLightfv(GL_LIGHT1,GL_SPOT_DIRECTION, spot_direction);
glLightf(GL_LIGHT1,GL_SPOT_EXPONENT,120.0);*/

  /*    glFrontFace (GL_CW); */
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
//  glEnable(GL_LIGHT1);
//  glEnable(GL_LIGHT2);
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

void drawWalls(){
	glEnable(GL_TEXTURE_2D);			// Enable Texture Mapping
 	glBindTexture(GL_TEXTURE_2D, texture[0]);   // choose the texture to use.
	set_colour(0.4f, 0.4f, 0.4f, 0.4f, 0.7f, 0.1f);
	
	glPushMatrix();
	glTranslatef(-12,0,0);
	
/*	//Left wall
	glBegin(GL_QUADS);
	
	glNormal3f(1,0,0);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0,0, 30);
	
	glNormal3f(1,0,0);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0,30,30);
	
	glNormal3f(1,0,0);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0,30, -30);
	
	glNormal3f(1,0,0);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0,0, -30); 
	
	glEnd();
	
	
	//Back wall
	glBegin(GL_QUADS);
	glNormal3f(0,0,-1);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0,0, 30);
	
	glNormal3f(0,0,-1);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0,30,30);
	
	glNormal3f(0,0,-1);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(24,30, 30);
	
	glNormal3f(0,0,-1);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(24,0, 30); 
	glEnd();
	*/
		glTranslatef(24,0,0);

/*	//Right wall
	glBegin(GL_QUADS);
	
	glNormal3f(-1,0,0);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0,0, 30);
	
	glNormal3f(-1,0,0);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0,30,30);
	
	glNormal3f(-1,0,0);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0,30, -30);
	
	glNormal3f(-1,0,0);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0,0, -30); 
	
	glEnd();
	
	//Front wall
	glBegin(GL_QUADS);
	glNormal3f(0,0,1);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0,0, -30);
	
	glNormal3f(0,0,1);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0,30,-30);
	
	glNormal3f(0,0,1);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-24,30, -30);
	
	glNormal3f(0,0,1);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-24,0, -30); 
	glEnd();*/
	
	//Floor
	glBegin(GL_QUADS);
	
	glNormal3f(0,1,0);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0,0, 30);
	
	glNormal3f(0,1,0);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0,0,0);
	
	glNormal3f(0,1,0);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(24,0, 0);
	
	glNormal3f(0,1,0);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(24,0,30); 
	
	
	glEnd();
	
/*	//Ceiling
	glBegin(GL_QUADS);
	
	glNormal3f(0,-1,0);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0,30, 30);
	
	glNormal3f(0,-1,0);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0,30,-30);
	
	glNormal3f(0,-1,0);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(24,30, -30);
	
	glNormal3f(0,-1,0);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(24,30,30); 
	
	
	glEnd();*/
	
	glPopMatrix();
	
	glDisable(GL_TEXTURE_2D);
}

void drawColumnShaft(){

	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	gluCylinder(aardvark, 0.6, 0.6, 1.0, 32, 32);
	glPopMatrix();

}


void drawColumn(float shaftHeight){
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[0]);   // choose the texture to use.
	set_colour(0.5f, 0.5f, 0.5f, 0.7f, 0.7f, 0.1f);
	
	glPushMatrix();
	
	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	//glRotatef(90,0,1,0);
	gluCylinder(aardvark, 1.0, 1.0, 1.0, 32, 32);
	gluDisk(aardvark, 0, 1, 32, 32);
	glTranslatef(0, 0, 1);
	gluDisk(aardvark, 0, 1, 32, 32);
	glPopMatrix();
	
	glPushMatrix();
	glScalef(1.0, shaftHeight, 1.0);
	drawColumnShaft();
	glPopMatrix();
	
	glTranslatef(0,shaftHeight, 0);
	
	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	//glRotatef(90,0,1,0);
	gluCylinder(aardvark, 1.0, 1.0, 1.0, 32, 32);
	gluDisk(aardvark, 0, 1, 32, 32);
	glTranslatef(0, 0, 1);
	gluDisk(aardvark, 0, 1, 32, 32);
	glPopMatrix();
	
	glPopMatrix();
	
	glDisable(GL_TEXTURE_2D);
}

void drawColumnSet(){
	
	glPushMatrix();
	glTranslatef(-11, 0, 0);
	
	glPushMatrix();
	for(int i = 1; i < 7; i++){
	
		glTranslatef(0, 1, -1);
		drawColumn(28 - i);
	
	}
	glPopMatrix();
	
	glTranslatef(22, 0, 0);
	
	glPushMatrix();
	for(int i = 1; i < 7; i++){
	
		glTranslatef(0, 1, -1);
		drawColumn(28 - i);
	
	}
	glPopMatrix();
	
	glPopMatrix();
}


void drawStairs(){
	
	glEnable(GL_TEXTURE_2D);			// Enable Texture Mapping
 	glBindTexture(GL_TEXTURE_2D, texture[0]);   // choose the texture to use.
	//set_colour(1.0f, 1.0f, 1.0f, 0.2f, 0.7f, 0.1f);
	set_colour(0.7f, 0.7f, 0.7f, 0.7f, 0.7f, 0.1f);
	
	for(int i = 1; i <= 6; i++){
		
		glBegin(GL_QUADS);
		glNormal3f(0,1,0);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-12.0f, i - 1 , -2 * (i - 1));
		glNormal3f(0,1,0);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(12.0f, i - 1, -2 * (i - 1));
		glNormal3f(0,1,0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(12.0f, i, -2 * (i - 1));
		glNormal3f(0,1,0);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-12.0f, i, -2 * (i - 1));
		glEnd();
		
		
		glBegin(GL_QUADS);
		glNormal3f(0,0,1);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-12.0f, i, -2 * (i-1));
		glNormal3f(0,0,1);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(12.0f, i, -2 * (i-1));
		glNormal3f(0,0,1);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(12.0f, i, -2 * i);
		glNormal3f(0,0,1);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-12.0f, i, -2 * i);
		
		glEnd();
	
	}
	
	glDisable(GL_TEXTURE_2D);
}

void drawSphere(){
	 glEnable(GL_TEXTURE_2D);
	 glBindTexture(GL_TEXTURE_2D, texture[2]);   // choose the texture to use.
	 set_colour(0.6f,0.6f,0.3f, 0.7f, 0.2f, 1.0f);
	 glPushMatrix();
	 glTranslatef(0.0f, dofs[SPHERE_Y].curValue, dofs[SPHERE_Z].curValue);
	 gluSphere(aardvark, 2.8, 32, 32);
	 glPopMatrix();
	 glDisable(GL_TEXTURE_2D);
}

void drawCube(){
	double timeOffset = 1.6;
	
	set_colour(0.28f,0.36f,0.8f, 0.3f, 0.6f, 0.6f);
	//glBindTexture(GL_TEXTURE_2D, texture[1]);   // choose the texture to use.
	 
	glPushMatrix();
	glTranslatef(-6.5f, 8.3f, -12.0f);
	glRotatef(45, 0.0f, 1.0f, 0.0f);
	
	if(curTime > timeOffset){
	 glRotatef((int)((curTime - timeOffset) * 180) % 360, 0, 1, 0);
	}
	
	glutSolidCube(4.5f);
	glPopMatrix();
	
}

void drawPyramid(){
	double timeOffset = 4.6;
	
	set_colour(0.28f,0.36f,0.8f, 0.7f, 0.6f, 0.6f);
	 
	glPushMatrix();
	glTranslatef(6.5f, 8.6f, -12.0f);
	glScalef(3.f, 3.f, 3.f);
	glRotatef(-45, 0, 1, 0);
	
	if(curTime > timeOffset){
	 glRotatef((int)((curTime - timeOffset) * 180) % 360, 1, 0, 0);
	}
	
	glutSolidTetrahedron();
	
/*	glBegin(GL_TRIANGLES);                                  // Start Drawing The Pyramid
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
        glEnd();  */

	glPopMatrix();
	
	glDisable(GL_TEXTURE_2D);
}

void drawHammer(){
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[1]);   // choose the texture to use.
	set_colour(1.0f,1.0f,1.0f, 1.0f, 0.6f, 0.6f);
	
        glPushMatrix();
	glTranslatef(2,5,0);
	
	glTranslatef(dofs[HAMMER_X].curValue, dofs[HAMMER_Y].curValue, dofs[HAMMER_Z].curValue);
	glRotatef(-180, 0, 0, 1);
	glRotatef(dofs[HAMMER_ROT_Y].curValue, 0, 1, 0);
	glRotatef(dofs[HAMMER_ROT_X].curValue, 1, 0, 0);
	
	glPushMatrix();
	glScalef(1.6, 1.6, 1.6);
	
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
	
	glPopMatrix();
	
	glDisable(GL_TEXTURE_2D);
}

void drawPlatform(){
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);   // choose the texture to use.
	 
	
	glPushMatrix();
	glTranslatef(0.0f, dofs[PLATFORM].curValue, -13.0f);
	 
	
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
	
	glPopMatrix();
	
	glDisable(GL_TEXTURE_2D);

}


void drawPalm(){
	
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

}

void drawMan(){
        //glBindTexture(GL_TEXTURE_2D, texture[3]);   // choose the texture to use.
	 
	set_colour(0.28f,0.36f,0.8f, 0.7f, 0.6f, 0.6f);
	
	glPushMatrix();
	
	glTranslatef(0.0f, dofs[MAN_Y].curValue, dofs[MAN_Z].curValue);
	glRotatef(dofs[MAN_ROT_X].curValue, 1,0,0);
	
	//torso
	glPushMatrix();
	glRotatef(-90.0f,1.0f,0.0f,0.0f);
	gluCylinder(aardvark, 0.4f, 0.6f, 1.2f, 32, 32);
	glPopMatrix();
	
	
	//glTranslatef(0,0,-0.6);
	//glTranslatef(0,0,0.6);
	
	//Whole left arm rotations
	glPushMatrix();
	glTranslatef(-0.48,1.1,0);
	glRotatef(dofs[LEFT_SHOULDERX].curValue, 1,0,0);
//        glRotatef(dofs[LEFT_SHOULDERZ].curValue, 0,0,1);
	glTranslatef(0.48,-1.1,0);
	
	//left shoulder
	glPushMatrix();
	glTranslatef(-0.6f, 1.1f, 0.0f);
	gluSphere(aardvark, 0.25, 32, 32);
	glPopMatrix();
	
	
	//left upper arm
	glPushMatrix();
	glTranslatef(-0.6f, 1.1f, 0.0f);
	glRotatef(90,1.0,0.0,0.0);
	gluCylinder(aardvark,0.23f, 0.1f, 0.7, 32, 32); 
	glPopMatrix();
	
	
	//left elbow
	glPushMatrix();
	glTranslatef(-0.6, 0.4, 0.0);
	gluSphere(aardvark, 0.12, 32, 32);
	glPopMatrix();
	
	//left elbow rotations
	glPushMatrix();
	glTranslatef(0,0.4,0);
	glRotatef(dofs[LEFT_ELBOW].curValue, 1,0,0);
	glTranslatef(0,-0.4,0);
	
	//left forearm
	glPushMatrix();
	glTranslatef(-0.6,0.4,0.0);
	glRotatef(90,1.0,0.0,0.0);
	gluCylinder(aardvark, 0.11, 0.04, 0.7, 32, 32);
	glPopMatrix();
	
	//left palm
	glPushMatrix();
	glTranslatef(-0.6,-0.4,0.0);
	glRotatef(180,0,0,1);
	glRotatef(180,0,1,0);
	glScalef(0.12,0.12,0.12);
	drawPalm();
	glPopMatrix();
	
	//end left elbow rotation
	glPopMatrix();
	
	//end whole left arm rotations
	glPopMatrix();
	
	
	//Whole right arm rotations
	glPushMatrix();
	glTranslatef(0.48,1.1,0);
	glRotatef(dofs[RIGHT_SHOULDERX].curValue, 1, 0, 0);
//	glRotatef(dofs[RIGHT_SHOULDERZ].curValue, 0, 1, 0);
	glTranslatef(-0.48,-1.1,0);
	
	//right shoulder
	glPushMatrix();
	glTranslatef(0.6f, 1.1f, 0.0f);
	gluSphere(aardvark, 0.25, 32, 32);
	glPopMatrix();
	
	
	//right upper arm
	glPushMatrix();
	glTranslatef(0.6f, 1.1f, 0.0f);
	glRotatef(90,1.0,0.0,0.0);
	gluCylinder(aardvark,0.23f, 0.1f, 0.7, 32, 32); 
	glPopMatrix();
	
	
	//right elbow
	glPushMatrix();
	glTranslatef(0.6, 0.4, 0.0);
	gluSphere(aardvark, 0.12, 32, 32);
	glPopMatrix();
	
	//right elbow rotation
	glPushMatrix();
	glTranslatef(0,0.4,0);
	glRotatef(dofs[RIGHT_ELBOW].curValue, 1,0,0);
	glTranslatef(0,-0.4,0);
	
	//right forearm
	glPushMatrix();
	glTranslatef(0.6,0.4,0.0);
	glRotatef(90,1.0,0.0,0.0);
	gluCylinder(aardvark, 0.11, 0.04, 0.7, 32, 32);
	glPopMatrix();
	
	//right palm
	glPushMatrix();
	glTranslatef(0.6,-0.4,0.0);
	glRotatef(180,0,0,1);
	glScalef(0.12,0.12,0.12);
	drawPalm();
	glPopMatrix();
	
	//end right elbow rotation
	glPopMatrix();
	
	//end whole right arm rotations
	glPopMatrix();
	
	//Neck and Head rotations
	glPushMatrix();
	glTranslatef(0,1.08,0);
	glRotatef(dofs[NECK_ANGLE].curValue, 1,0,0);
	glTranslatef(0,-1.08,0);
	
	//neck
        glPushMatrix();
	glTranslatef(0.0f, 1.0f, 0.0f);
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	gluCylinder(aardvark, 0.2f, 0.2f, 0.4f, 32, 32);
	glPopMatrix();
	
	
	//head
	glPushMatrix();
	glTranslatef(0.0f, 1.7f, 0.0f);
	glRotatef(-90,1,0,0);
	gluSphere(aardvark, 0.4, 32, 32);
	glPopMatrix();
	
	//end Neck and Head rotations
	glPopMatrix();
	
	//ass
	glPushMatrix();
	glTranslatef(0.0f, -0.5f, 0.0f);
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	gluCylinder(aardvark, 0.53f, 0.4f, 0.5f, 32, 32);
	glPopMatrix();
	
	//rotate legs into kneeling position
	glPushMatrix();
	glTranslatef(0,0.05,0.2);
	glRotatef(20, 1,0,0);
	
	//rotate left leg to point feet and calves outwards
	glPushMatrix();
	glRotatef(-10, 0,1,0);
	
	//left thigh
	glPushMatrix();
	glTranslatef(-0.225f, -0.5f, 0.0f);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	gluCylinder(aardvark, 0.28f, 0.1f, 0.8f, 32, 32);
	glPopMatrix();
	
	//left knee
	glPushMatrix();
	glTranslatef(-0.225f, -1.35f, 0.0f);
	gluSphere(aardvark, 0.15f, 32, 32);
	glPopMatrix();
	
	//left calf and foot
	glPushMatrix();
	glTranslatef(-0.225f, -1.4f, 0.0f);
	gluCylinder(aardvark, 0.11f, 0.04f, 0.6f, 32, 32);
	glTranslatef(0,-0.1,0.6);
	glRotatef(-90,1,0,0);
	glScalef(0.1,0.05,0.2);
	glutSolidCube(2.0);
	glPopMatrix();
	
	//end rotate left leg to point feet and calves outwards
	glPopMatrix();
	
	//rotate right leg to point feet and calves outwards

	glPushMatrix();
	glRotatef(10,0,1,0);

	//right thigh
	glPushMatrix();
	glTranslatef(0.225f, -0.5f, 0.0f);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	gluCylinder(aardvark, 0.28f, 0.1f, 0.8f, 32, 32);
	glPopMatrix();
	
	//right nee
	glPushMatrix();
	glTranslatef(0.225f, -1.35f, 0.0f);
	gluSphere(aardvark, 0.15f, 32, 32);
	glPopMatrix();
	
	
	//right calf and foot
	glPushMatrix();
	glTranslatef(0.225f, -1.4f, 0.0f);
	//glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	gluCylinder(aardvark, 0.11f, 0.04f, 0.6f, 32, 32);
	glTranslatef(0,-0.1,0.6);
	glRotatef(-90,1,0,0);
	glScalef(0.1,0.05,0.2);
	glutSolidCube(2.0);
	glPopMatrix();
	
	//end rotate right leg to point feet and calves outwards
	glPopMatrix();

	//end rotate legs into kneeling position
	glPopMatrix();
	
	
	glPopMatrix();
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
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  
	//back view
	//gluLookAt (0, 0, -10,  0.0,0.0,0.0, 0.0,1.0,0.0);
	
	//front view
	//gluLookAt (-15, 2, -0.5,  0.0,2.0, -2.0, 0.0,1.0,0.0);
	//gluLookAt(-40, 5, 13, 0,2,-5,0,1,0);
	//gluLookAt (-5, 10, -2.0,  0.0,2.0, -2.0, 0.0,1.0,0.0);
	gluLookAt(0,5,30,0,5,0,0,1,0);
	
	//gluLookAt(7,0,-20,7,0,0,0,1,0);
	
	//left view
	//gluLookAt (-45, 7, -10,  0.0,7.0,-10.0, 0.0,1.0,0.0);
	
	//right view
	//gluLookAt (10, 0, 0,  0.0,0.0,0.0, 0.0,1.0,0.0);
	
//	drawWalls();
	drawColumnSet();
	drawStairs();
	drawCube();
	drawSphere();
	drawPlatform();
	drawPyramid();
 	
	glPushMatrix();
	//glRotatef(20, 1,0,0);
	drawMan();
 	glPopMatrix();
	
	glPushMatrix();
	//glTranslatef(-2.0f, 3.45f, dofs[MAN_Z].curValue);
	//glRotatef(dofs[MAN_ROT_X].curValue, 1,0,0);
	glTranslatef(-4, 0, 0);
	glRotatef(-90, 0,1,0);
	drawMan();
 	glPopMatrix();
	
	drawHammer();
	
	/*glPointSize(10);
	glBegin(GL_POINTS);
	set_colour(1.0, 0.0, 0.0);
	glVertex3f(dofs[HAMMER_X].curValue, 0, 0);
	glEnd();*/
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
    
    if(!dofs[idof].bezier){
    
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
    } else {
    
    
    	float v1, v2, v3, v4;
	for (int ikey=0; ikey < n-3; ikey++) {
      		tStart = dofs[idof].keys[ikey]  .getTime();
      		tEnd   = dofs[idof].keys[ikey+3].getTime();
      		if (tStart <= curTime && curTime <= tEnd) {
			curInterval = ikey;
			v1 = dofs[idof].keys[ikey].getValue();
			v2   = dofs[idof].keys[ikey+1].getValue();
			v3 = dofs[idof].keys[ikey+2].getValue();
			v4 = dofs[idof].keys[ikey+3].getValue();
			break;
      		}
    	}
    	double t = (curTime - tStart)/(tEnd - tStart);
    	dofs[idof].curValue = pow(1 - t, 3) * v1 + 3 * t * pow(1 - t, 2) * v2 + 3 * pow(t, 3) * (1 - t) * v3 + pow(t, 3)*v4;
    	cout << dofs[idof].curValue << endl;
    
    }
  
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
	break;
      }
    }
    double t = (curTime - tStart)/(tEnd - tStart);
    dofs[idof].curValue = (1-t)*(1-t)*(1-t)*v1 + 3*t*(1-t)*(1-t)*v2 + 3 *
    t*t*(1-t)*v3+t*t*t*v4;
   // cout << "nextFrame. curValue="<<dofs[idof].curValue<<endl;

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
