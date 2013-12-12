// WARNING: DO NOT MODIFY THIS FILE EXCEPT WHERE EXPLICITLY NOTED
//
#ifndef IMDRAW_H
#define IMDRAW_H

#include <FL/Fl.h>
#include <FL/Fl_Window.h>

#include <FL/gl.h>
#include <FL/Fl_Gl_Window.h>
#include <FL/Fl_Value_Slider.h>

#include <deque>

#include "../Texture.h"

#include "imageprimitive.h"
#include "../bspline.h"
#include <deque>

using namespace std;
////// PUT YOUR CODE (IF ANY) BETWEEN THESE LINES ///////

//Define some constants to represent program state.
#ifndef D_POINT
#define D_POINT 1
#endif

#ifndef D_LINE
#define D_LINE 2
#endif

#ifndef D_RECT
#define D_RECT 3
#endif

#ifndef D_CIRC
#define D_CIRC 4
#endif

#ifndef D_POLY
#define D_POLY 5
#endif

#ifndef D_SCISSOR
#define D_SCISSOR 6
#endif

#ifndef D_BSPLINE
#define D_BSPLINE 7
#endif


#ifndef DISPLAY_IMAGE
#define DISPLAY_IMAGE 1
#endif

#ifndef DISPLAY_GRAD
#define DISPLAY_GRAD 2
#endif

#ifndef PI
#define PI 3.14159265
#endif

#ifndef MAX_NEIGHBOURS
#define MAX_NEIGHBOURS 8
#endif
/////////////////////////////////////////////////////////
class Boundary;

class ImDraw : public Fl_Gl_Window {  
	
	friend class Boundary;
	
	public:
		// The class constructor 
		ImDraw(int x,int y,int w,int h,const char *l=0);

		// The widget class draw() override.  The draw() function
		// initializes GL for another round of drawing and then calls
		// specialized functions for drawing each of the entities to be
		// displayed.
		void draw();

		// This is the event handling routine. All UI events go through
		// this routine, which can choose to pass them on to the default
		// handler or handle them itself.
		int handle(int event);

		// Set the image to be displayed on the GL canvas. The image will
		// be displayed the next time the method draw_image() is called.
		// The image must have a resolution of at most 1024x1024. If
		// larger images are needed, the file Texture.cpp must be
		// modified accordingly.
		void set(VImage im);

		// Draw the image on the GL canvas. This method is called by the
		// draw() method.
		void draw_image();

		// convert an (x,y) position given in GL canvas coordinates
		// (eg. generated through a mouse event or used to draw a vertex
		// with OpenGL) to an integer (row,column) coordinate
		// corresponding to a pixel in a Vista image
		int gl2vista(double glx, double gly, int& r, int& c);

		// convert the (row,column) coordinate of a pixel in a Vista
		// image to an (x,y) position on the GL canvas.
		int vista2gl(int r, int c, double& glx, double& gly);
		void resize(int x, int y, int w, int h);

		/* Returns the value of a symbolic constant representing the
		   current program state. */
		int getDrawingState();

		/* Sets the current program state to the value passed as a
		   parameter. */
		void setDrawingState(int drawingState);

		/* Check whether the last scene object in the queue is a
		   polygon. If it is, and the polygon is not complete, 
		   complete the polygon. */
		void checkForPoly();

		/* Insert a scene object at the back of the queue. */
		void sceneObjectsPushBack(ImagePrimitive *myObj);
		
		/* Returns the string representation of all objectsin the scene. */
		string sceneObjectsToString();

		/* Clear all the objects in the scene. */
		void clearScene();
		
		/* If there is a line in the scene that is closer
		   to (x,y) than any other line and closer than 
		   some threshold, returns the pointer to that line,
		   otherwise returns null. */
		ImagePrimitive* nearestLine(int x, int y);
		
		
		/* Remove the most recently added object from the scene. */
		void sceneObjectsUndo();
		
		/* Returns true if there are no objects in the scene. */
		bool sceneIsEmpty();

		/* Converts an rgb image to grayscale. */
		VImage rgbToGrayscale(VImage img);

		/* Returns an image of zero crossings of the second
		   derivative of img, using gaussian filter of
		   standard deviation = sigma. */
		VImage zeroCrossings(VImage img, int sigma);

		/* Returns an image of gradients of pixels
		   in img. */
		VImage gradient(VImage img);

		/* Returns an image of gradient magnitudes of pixels
		   in img. */
		VImage gradientMagnitude(VImage img);

		/* Returns the maximum gradient magnitued in img. */
		VDouble gradMagMax(VImage img);

		/* Returns the current display state. */
		int getDisplayState();

		/* Sets the current display state. */
		void setDisplayState(int state);

		/* Reinitialize path finder data structures. */
		void pathFinderReInit();

		/* Destroy all resources used by the pathfinder. */
		void pathFinderDestroy();

		/* Compute the link costs for image im. */
		void computeLinkCosts(VImage im);

		/* Compute minimal path costs given a seed point. */
		void computePathCosts(int seedX, int seedY);
		
		/* Returns this width of the currently displayed image. */
		int getImageWidth();
		
		/* Returns the height of the currently displayed image. */
		int getImageHeight();

		/* Returns an image containing the boundary described by
		   boundary. */
		VImage getBoundary(ImagePrimitive *boundary);
		
		/* Returns an image containing an alpha matte based on the
		   boundary contained in boundary. */
		VImage getMatte(VImage boundary);

		/* Returns the string representation of the boundary
		   vertices contained in the scene. */
		string writeBoundaryVertices();

		/* Returns the string representation of the boundary
		   seed points contained in the scene. */
		string writeBoundarySeedPoints();

		/* Writes the current image matte to file. */
		void writeMatteToFile(char *filename);

		/* Writes the current image gradient image to file. */
		void writeGradToFile(char *filename);

		/* Writes the current zero crosssings image to file. */
		void writeZeroCrossToFile(char *filename);

		/* Writes the current link costs image to file. */
		void writeLinkCostsToFile(char *filename);
		/////////////////////////////////////////////////////////

		void gl2v_matrices(Matrix& gl2v, Matrix& v2gl);
		// this is the top-level function that 
		// controls the level of detail of a bspline
		void setLevelOfDetail(double l);
		
		void setSlider(Fl_Value_Slider *slider);
		
		/* Delete the spline */
		void deleteSpline();

		/* Add knot to spline. */
		void addSplineKnot(int x, int y);

		/* Lock spline endpoints. */
		void completeSpline();

		/* Convert a set of control points to a b-spline. */
		void convertPointsToSpline();

		/* Map slider range to current resolution range. */
		void setSlider();

		/* Reduce spline to specified resolution level . */
		void reduceSplineTo(int level);

		/* Write the spline control points out to file. */
		void writeSplineToFile(char *filename);

		/* Morph between srcIm and dstIm according to parameters specified in 
		srcLine and dstLine and warpfraction. Uses gaussian smoothing. */
		VImage morph(VImage srcIm, VImage dstIm, deque<ImagePrimitive*> srcLine, 
			deque<ImagePrimitive*> dstLine, double warpfraction);
		
		/* Morph between srcIm and dstIm according to parameters specified in 
		srcLine and dstLine and warpfraction. Uses MIP map antialiasing. */
		VImage mipMorph(VImage srcIm, VImage dstIm, deque<ImagePrimitive*> srcLine, 
			deque<ImagePrimitive*> dstLine, double warpfraction);
		
		/* Read in source and destination images for morph as well as control lines.
		   Perform morph between source and destination in frames number of frames
		   Store resuling images in files of the form morphBaseName.frame#.v*/
		void prepMorph(char* sourceImageName, char* destImageName, 
			char* sourceCommandsName, char* destCommandsName, char* morphBaseName, int frames);
		
		/* Blend images im1 and im2 according to blending factor and store result in file named
		filename. */
		void ImDraw::blend(VImage im1, VImage im2, double blendingFactor, char* filename);
		
		/* Compute coordinates of pixel in the source image that maps to pixel with coordinates
		(row, col) in the destination image. Store results in srcPixel. */
		void computeSourcePixel(int numLines, int row, int col, double *lineData, int *srcPixel);
		
	private:	
		int width, height;
		int im_height, im_width;
		double aspect_ratio;
		double magnification;
		Texture *imTexture;
	 
	 	
		/* This pointer is used to specify what line is being dragged. */
		ImagePrimitive *dragLine;
		
		/* Starting coordinates for line dragging. */
		int startX , startY;
		
		/* Flag specifying whether cursor shape has been set. */
		bool cursorSet;
		
		/* Current state. Can be one of D_POINT, D_LINE, D_RECT, D_CIRC, D_POLY
		signifying the drawing of a point, a line, a rectangle, a circle, or
		a polygon respectively. */
		int drawingState;
		int displayState;

		/* A deque of objects in the scene. */
		deque<ImagePrimitive*> sceneObjects;
		
		VImage grayImage, gradImage, zeroImage, weightImage, nodeImage, nodeInfoImage;

		int lastScissorX, lastScissorY;
		bool firstScissorClick;
	
		Matrix v2gl;
		Matrix gl2v;
		Matrix p1, q1, p2, q2, p3, q3, pn, qn;
		
		/* Queue of detail matrices and images at different resolutions. */
		deque<Matrix> detailImages;
		deque<Matrix> resolutionImages;

		double changeThreshold;
		double lastLOD;
		Fl_Value_Slider *splineLOD;
		
		BSpline *bspline;
};
#endif
