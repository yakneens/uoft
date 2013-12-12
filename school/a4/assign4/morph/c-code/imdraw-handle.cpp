// WARNING: DO NOT MODIFY THIS FILE EXCEPT WHERE EXPLICITLY NOTED
#include <newmat/newmatio.h>
#include <math.h>
#include <iostream.h>
#include "imdraw.h"
#include "point.h"
#include "line.h"
#include "rectangle.h"
#include "color.h"
#include "circle.h"
#include "polygon.h"
#include "boundary.h"
#include "../glutils.h"
#include <assert.h>

////// PUT YOUR EVENT-HANDLING CODE BETWEEN THESE LINES ///////

// define an example event handler that intercepts Mouse-1 events in
// order to draw a rectangle over an image by clicking and dragging
// the Mouse-1 button
int ImDraw::handle(int event) {
	int mousex, mousey;
	//  flag that specifies whether the event was handled by the
	//  code below or whether it should be passed on to the default
	//  fltk handler
	int intercepted=0;
	
	/* Set the cursor to FL_CURSOR_CROSS if it hasn't yet been set. */
	if(!cursorSet){
		cursor(FL_CURSOR_CROSS, FL_WHITE, FL_BLACK);
	}

	switch (event) {
	case FL_ENTER:
	case FL_MOVE:
		break;

	/* Catch the mouse click event. */
	case FL_PUSH:
		
		
		
		/* Left mouse button clicked */
		if (Fl::event_button() == 1) {
			mousex = Fl::event_x();
			mousey = Fl::event_y();
			
			Point* myPoint;
			Rect* myRect;
			Line* myLine;
			Circle* myCircle;
			Poly* myPoly;
			ImagePrimitive* myTestObj;
			Boundary* myBoundary;
			
			int x,y;

			
			
			/* Convert mouse position from gl coordinates to vista coordinates. */
			/* Also checks if mouse pointer is off the canvas. */
			if(gl2vista(mousex,mousey, x, y)){
				
				/* Handle the mouse click based on the state of the program. */
				switch(this->drawingState){

				/* Drawing a point. */
				case D_POINT:
					
					/* Create a new point and add to the scene. */
					myPoint = new Point(x, y, this);
					sceneObjects.push_back(myPoint);
					break;

				/* Drawing a line. */
				case D_LINE:

					/* Create a new line with both endpoints at the mouse cursor
					   and add to the scene. */
					myLine = new Line(x, y, x, y, this);
					sceneObjects.push_back(myLine);
					break;
				
				/* Drawing a rectangle. */
				case D_RECT:

					/* Create a new rectangle with both vertices at mouse cursor
					   and add to the scene. */
					myRect = new Rect(x, y, x, y, this);
					sceneObjects.push_back(myRect);
					break;

				/* Drawing a circle. */
				case D_CIRC:

					/* Create a new circle centered at mouse pointer with radius 0
					   and add to the scene. */
					myCircle = new Circle(x, y, x, y, this);
					sceneObjects.push_back(myCircle);
					break;

				/* Drawing a polygon */
				case D_POLY:

					/* If there are objects in the scene. */
					if(!sceneIsEmpty()){

						/* Fetch the most recently added object. */
						myTestObj = sceneObjects.back();

						/* If the most recent object is a polygon. */
						if(myTestObj->getType() == ImagePrimitive::poly){
							
							/* Cast the oject pointer to Poly. */
							myPoly = static_cast<Poly*>(myTestObj);

							/* If the polygon is complete */
							if(myPoly->isComplete()){
								/* Create a new polygon with one vertex at the mouse
								   pointer and add to the scene. */
								myPoly = new Poly(x, y, this);
								sceneObjects.push_back(myPoly);
							/* If the polygon is not complete. */
							}else{

								/* If the last defined vertex has the same
								   coordinates as current mouse coordinates
								   (ie a double click occured), complete
								   the polygon. */
								if(myPoly->isLastVertex(x, y)){
									myPoly->completePoly();
								
								/* If the last defined vertex has different
								   coordinates then add a new vertex at (x,y). */
								}else{
									myPoly->addPoint(x, y);
								}
							}
						
						/* If the most recent object is not a polygon. */
						}else{
							/* Create a new polygon with one vertex at the mouse
							   pointer and add to the scene. */
							myPoly = new Poly(x, y, this);
							sceneObjects.push_back(myPoly);

						}
					/* If there are no objects in the scene. */
					}else{
						/* Create a new polygon with one vertex at the mouse
						   pointer and add to the scene. */
						myPoly = new Poly(x, y, this);
						sceneObjects.push_back(myPoly);
					}
					break;
				case D_SCISSOR:
					if(imTexture != NULL){
						/* If there are objects in the scene. */
						if(!sceneIsEmpty()){

							/* Fetch the most recently added object. */
							myTestObj = sceneObjects.back();

							/* If the most recent object is a boundary. */
							if(myTestObj->getType() == ImagePrimitive::boundary){
								
								/* Cast the oject pointer to Boundary. */
								myBoundary = static_cast<Boundary*>(myTestObj);

								/* If the boundary is complete */
								if(myBoundary->isComplete()){
									/* Create a new boundary with one vertex at the mouse
									   pointer and add to the scene. */
									myBoundary = new Boundary(y, x, this);
									sceneObjects.push_back(myBoundary);

									/* If the boundary is not complete. */
								}else{

									/* If the last defined vertex has the same
									   coordinates as current mouse coordinates
									   (ie a double click occured), complete
									   the boundary. */
									if(myBoundary->isLastVertex(y, x)){
										myBoundary->completeBoundary();
										
										/* Create an image of the boundary. */
										VImage boundary = getBoundary(myBoundary);

										/* Create an alpha matte corresponding to the
										   boundary. */
										VImage matte = getMatte(boundary);
										/* Display the alpha matte. */
										set(matte);
									/* If the last defined vertex has different
									   coordinates then add a new seed points at (x,y). */
									}else{
										myBoundary->addSeedPoint(y, x);
									}
								}
							
							/* If the most recent object is not a boundary. */
							}else{
								/* Create a new boundary with one vertex at the mouse
								   pointer and add to the scene. */
								myBoundary = new Boundary(y, x, this);
								sceneObjects.push_back(myBoundary);

							}
						/* If there are no objects in the scene. */
						}else{
							/* Create a new boundary with one vertex at the mouse
							   pointer and add to the scene. */
							myBoundary = new Boundary(y, x, this);
							sceneObjects.push_back(myBoundary);
							
						}
					}
					break;	
				
				case D_BSPLINE:
					this->splineLOD->value(this->splineLOD->minimum());
					if (bspline->endpoints_locked()){
						bspline->delete_knots();
					}
					
					// insert a new knot to the end of the bspline
					bspline->add_knot(x, y);
					
					break;

				}
			}
			
			/* Flag the fact that we handled this event. */
			intercepted = 1;

		/* Right mouse button clicked. */
		}else if(Fl::event_button() == 3){
			Matrix mat;
				
			switch(drawingState){
			case D_BSPLINE:
				int bk, bj, bd;

				if (!bspline->endpoints_locked()) {
					bspline->lock_endpoints();
					setSlider();
					this->resolutionImages.push_back(mat);
				}
				break;
			default:
				/* If there are objects in the scene
				   remove the one that was most recenlty added.
				   This implements the undo functionality. */
				if(!sceneIsEmpty()){
					sceneObjectsUndo();
				}
			
			}
			/* Flag the fact that we handled this event. */
			intercepted = 1;
		/* Scroll wheel is clicked. */
		}else if (Fl::event_button() == 2) {
			
			

			mousex = Fl::event_x();
			mousey = Fl::event_y();
			
			int x,y;
			/* Convert mouse position from gl coordinates to vista coordinates. */
			/* Also checks if mouse pointer is off the canvas. */
			if(gl2vista(mousex,mousey, x, y)){
				
				/* Detect a line that is closest to current mouse pointer
				   position and store the pointer in dragLine. 
				   Store starting coordinates for the translation in 
				   startX and startY.*/
				if((dragLine = static_cast<Line*>(nearestLine(x,y))) != NULL){
					startX = x;
					startY = y;
				}
			}
			
			/* Flag the fact that we handled this event. */
			intercepted = 1;
		}
		break;
	/* Catch the mouse drag event. */
	case FL_DRAG:

		/* If the left mouse button is pressed. */
		if (Fl::event_button() == 1) {
			Rect *myRect;
			Line *myLine;
			Circle *myCircle;
			ImagePrimitive *testObj;

			int x1,y1;
			
			mousex = Fl::event_x();
			mousey = Fl::event_y();
			
			/* Convert mouse position from gl to vista coordinates. */
			/* Also checks whether mouse pointer is off the canvas. */
			if(gl2vista(mousex,mousey, x1, y1)){
				
				/* Handle the event based on current program drawing state. */
				switch(this->drawingState){

				/* Drawing a point. */
				case D_POINT:
					/* Do nothing. */
					break;
				/* Drawing a line. */
				case D_LINE:
					/* Fetch the most recently added object. */
					testObj = sceneObjects.back();
					
					/* If the object is a line, set the second endpoint
					   of the line to the current mouse pointer position. */
					if(testObj->getType() == ImagePrimitive::line){
						myLine = static_cast<Line*>(testObj);
						myLine->setX2(x1);
						myLine->setY2(y1);
					}

					break;
				/* Drawing a rectangle. */
				case D_RECT:

					/*  Fetch the most recently added object. */
					testObj = sceneObjects.back();

					/* If the object is a rectangle, set the second
					   vertex of the rectangle to the current mouse pointer
					   position. */
					if(testObj->getType() == ImagePrimitive::rect){
						myRect = static_cast<Rect*>(sceneObjects.back());
						myRect->setX2(x1);
						myRect->setY2(y1);
					}
					
					break;
				/* Drawing a circle. */
				case D_CIRC:
					/* Fetch the most recently added object. */
					testObj = sceneObjects.back();

					/* If the object is a circle, set the coordinates
					   of the circle boundary point to the current
					   mouse pointer position. */
					if(testObj->getType() == ImagePrimitive::circle){
						myCircle = static_cast<Circle*>(sceneObjects.back());
						myCircle->setX2(x1);
						myCircle->setY2(y1);
					}
					break;
				/* Drawing a polygon. */
				case D_POLY:
					/* Do nothing. */
					break;
				}
			}
			
			/* Flag the fact that we handled this event. */
			intercepted = 1;
		/* Scroll wheel is pressed. */
		}else if(Fl::event_button() == 2){
			int x1,y1;
			mousex = Fl::event_x();
			mousey = Fl::event_y();
			
			
			/* Convert mouse position from gl to vista coordinates. */
			/* Also checks whether mouse pointer is off the canvas. */
			if(gl2vista(mousex,mousey, x1, y1)){
				
				switch(this->drawingState){

				case D_BSPLINE:
					bspline->move_knot(x1, y1, x1, y1);
					break;
				default:
					/* If a line has been established for dragging.*/
					if(dragLine != NULL){
						Line *myLine2 = static_cast<Line*>(dragLine);
						
						/* Translate the line by a vector
						   (x1 - startX, y1 - startY). */
						myLine2->translateLine(x1 - startX, y1 - startY);
						
						/* Set the starting coordinates for a new translation. */
						startX = x1;
						startY = y1;
					}
				}
			}
				
			
			
			/* Flag the fact that we handled this event. */
			intercepted = 1;
		}
		
		break;
	/* Mouse button released. */
	case FL_RELEASE:
		/* If the middle mouse button is released. */
		if (Fl::event_button() == 2) {
			
			/* Unset the cursor flag. */
			cursorSet = false;

			/* Flag the fact that we handled this event. */
			intercepted = 1;
			
		}
		break;
	default:
		;
	}

	if (!intercepted) {
		// delegate all other events to the regular Fl_Gl_Window handler
		return(Fl_Gl_Window::handle(event));
	} else {
		// otherwise, update the interface
		redraw();
		return 1;
	}
}

////////////////////////////////////////////////////////////////////////


