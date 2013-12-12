// WARNING: DO NOT MODIFY THIS FILE EXCEPT WHERE EXPLICITLY NOTED


#include <math.h>
#include <iostream.h>
#include "imdraw.h"
#include "../glutils.h"

////// PUT CODE USED BY CONTRUCTOR (IF ANY) BETWEEN THESE LINES ///////
///////////////////////////////////////////////////////////////////////

// Constructor initalization routine
ImDraw::ImDraw(int x,int y,int w,int h,const char *l)
	: Fl_Gl_Window(x,y,w,h,l)
{
	// w and h are the width and height of the OpenGL canvas 
	width = w;
	height = h;
	im_width = width;
	im_height = height;
	aspect_ratio = 1;
	magnification = 1;
	// Initially, the allocated texture memory contains no data
	imTexture = NULL;

	////// PUT YOUR INITIALIZATION CODE (IF ANY) BETWEEN THESE LINES ///////
	
	//current state
	drawingState = 0;
	displayState = DISPLAY_IMAGE;
	
	/* Line being currently dragged. */
	dragLine = NULL;

	/* Starting coordinates for line dragging. */
	startX = 0;
	startY = 0;

	lastScissorX = 0;
	lastScissorY = 0;
	firstScissorClick = true;
	
	/* Flag for setting cursor shape. */
	cursorSet = false;


	bspline = new BSpline;
	
	/* Last level of detail. */
	lastLOD = 1;

	changeThreshold = 1;
	////////////////////////////////////////////////////////////////////////
};


