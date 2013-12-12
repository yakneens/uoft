// WARNING: DO NOT MODIFY THIS FILE EXCEPT WHERE EXPLICITLY NOTED

#include <math.h>
#include <iostream.h>
#include "imdraw.h"
#include "../glutils.h"
#include "point.h"
#include "line.h"
#include "rectangle.h"
#include <limits>
#include "circle.h"
#include "polygon.h"
#include "imageprimitive.h"
#include "wavelets.h"


/* Define a constant for the maximum distance a line
   can be away from the mouse pointer in order to be
   grabbed in a drag procedure. */
#ifndef LINE_GRAB_THRESHOLD 
#define LINE_GRAB_THRESHOLD 15
#endif


/********************************************************************************
 * function getDrawingState() - returns the current state of this program.
 *******************************************************************************/
int ImDraw::getDrawingState(){
	return drawingState;
}

/********************************************************************************
 * function setDrawingState() - sets the current state of the program to 
 * drawingState.
 *******************************************************************************/
void ImDraw::setDrawingState(int drawingState){
	this->drawingState = drawingState;
}

/********************************************************************************
 * function sceneObjectsPushBac() - adds an object myObj to the queue of objects
 * in this scene.
 *******************************************************************************/
void ImDraw::sceneObjectsPushBack(ImagePrimitive *myObj){

	this->sceneObjects.push_back(myObj);
}

/********************************************************************************
 * function checkForPoly() - checks whether the last object added to the scene
 * was a polygon. If so, and the polygon is not complete, complete it.
 *******************************************************************************/
void ImDraw::checkForPoly(){
	
	if(sceneObjects.size() > 0){
		ImagePrimitive* tempObj;
		Poly* myPoly;
		
		/* tempObj is the last object added to the scene. */
		tempObj = sceneObjects.back();

		/* If tempObj points to a Polygon that is not complete, complete the 
		   polygon and redraw the scene. */
		if(tempObj->getType() == ImagePrimitive::poly && 
			!(myPoly = static_cast<Poly*>(tempObj))->isComplete()){
			myPoly->completePoly();	
			this->redraw();
		}
	}

}

/********************************************************************************
 * function sceneObjectsToString() - returns a string representation of all the
 * objects in the current scene.
 *******************************************************************************/
string ImDraw::sceneObjectsToString(){
	string tempString("");

	/* Generate a string consisting of string representations of all objects
	   in the current scene. */
	for(int i = 0; i < (int)sceneObjects.size(); i++){
		tempString += sceneObjects[i]->toString();
	}

	return tempString;
}

/********************************************************************************
 * function clearScene() - clears the current scene, removing all objects that
 * are currently in the scene.
 *******************************************************************************/
void ImDraw::clearScene(){
	
	while(sceneObjects.size() > 0){
		sceneObjects.pop_front();
	}

	bspline->delete_knots();
	this->splineLOD->value(this->splineLOD->minimum());
}

/********************************************************************************
 * function nearestLine() - returns a pointer to a line in the scene that is 
 * less than LINE_GRAB_THRESHOLD units away from (x,y) , otherwise
 * returns null.
 *******************************************************************************/
ImagePrimitive* ImDraw::nearestLine(int x, int y){
	
	Line *myLine, *closestLine;
	
	/* Set variables to maximum possible integer values. */
	double dist = INT_MAX, closestDist = INT_MAX;
	
	
	int x1, x2, y1, y2;
	int minX, maxX, minY, maxY;

	/* Go through all objects in the scene. */
	for(int i = 0; i < (int)sceneObjects.size(); i++){

		/* If the current object is a line. */
		if(sceneObjects[i]->getType() == ImagePrimitive::line){
			myLine = static_cast<Line*>(sceneObjects[i]);
			x1 = myLine->getX1();
			minX = x1;
			x2 = myLine->getX2();

			if(x2 < minX){
				minX = x2;
				maxX = x1;
			}else{
				maxX = x2;
			}
			
			y1 = myLine->getY1();
			minY = y1;
			y2 = myLine->getY2();
			if(y2 < minY){
				maxY = y1;
				minY = y2;
			}else{
				maxY = y2;
			}

			/* Calculate the shortest distance from (x,y) to the line 
			   pointed to by myLine. */
			dist = abs((x2 - x1) * (y1 - y) - (x1 - x) * (y2 - y1)) / 
				   sqrt(pow((double)x2 - x1, 2) + pow((double)y2 - y1, 2));
			
			if(x >= minX - LINE_GRAB_THRESHOLD && 
			   x <= maxX + LINE_GRAB_THRESHOLD && 
			   y >= minY - LINE_GRAB_THRESHOLD && 
			   y <= maxY + LINE_GRAB_THRESHOLD){
			
				/* If the distance is shorter than the previously found
				   shortestDistance, set distance to be the new closest
				   distance and set myLine to be the new closest line. */
				if(dist < closestDist){
					closestLine = myLine;
					closestDist = dist;
				}
			}	
		}
	}
	
	/* If the closest line lies within LINE_GRAB_THRESHOLD units
	   from (x,y) return a pointer to the line, otherwise return null. */
	if(closestDist < LINE_GRAB_THRESHOLD){
		
		/* Change the cursor to FL_CURSOR_HAND. */
		cursor(FL_CURSOR_HAND, FL_WHITE, FL_BLACK);
		cursorSet = true;
		
		return closestLine;

	}else{
		return NULL;
	}
}

/********************************************************************************
 * function sceneObjectsUndo() - removes the most recently added object from the
 * scene.
 *******************************************************************************/
void ImDraw::sceneObjectsUndo(){
	sceneObjects.pop_back();
}

/********************************************************************************
 * function sceneIsEmpty() - returns true if there are no objects in the scene.
 *******************************************************************************/
bool ImDraw::sceneIsEmpty(){
	return !(sceneObjects.size() > 0);
}

//////////////////////////////////////////////////////////////////////////



// This is the main GL drawing routine. 
void ImDraw::draw() {
	if (!valid()) {
		// These commands just tell GL what the size of the canvas is
		glLoadIdentity(); 
		glViewport(0,0,width,height);
		// This command ensures that both GL and Vista coordinates 
		// draw the (0,0) point at the top-left of the window
		glOrtho(0, width, height, 0, -1.0, 1.0);
	}
	
	// Use black to clear the GL canvas
	glClearColor(0.0, 0.0, 0.0, 1.0);
	// Command to clear the GL canvas 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// the routine below displays a Vista image in a GL canvas. For
	// now, you can skip the details of how this is done (ie. you
	// don't need to read that code or understand it)
	draw_image();

	////// PUT YOUR DRAWING CODE BETWEEN THESE LINES ///////
	
	// Set the default color to white
	glColor3d(1, 1, 1);
//	glPointSize(5);
	/* Draw all the objects in the scene. */
	for(int i = 0; i < (int)sceneObjects.size(); i++){
		sceneObjects[i]->draw();
	}
	
	gl2v_matrices(gl2v, v2gl);

	// draw the bspline
	bspline->draw(v2gl);
}


/********************************************************************************
 * function setLevelOfDetail() - increases or reduces the resolution of
 * the current b-spline curve as necessary.
 *******************************************************************************/
void ImDraw::setLevelOfDetail(double l)
{
	Matrix m1, m2, d1;

	int k, j, d;



	// get the matrix representation of the currently-drawn bspline
	bspline->get_matrix(m1, k, j, d);
	
	// convert to a bspline at level (j - 1) by computing a 
	// matrix that gives us a new, smaller set of bspline knots
	if(l <= lastLOD - 1){
		if (reduce(m1, k, j, d, m2, d1)) {
			/* Save the detail image. */
			this->detailImages.push_back(d1);
			
			
			// deallocate the original bspline data
			delete bspline;
				
			// create a new bspline at level (j-1) using
			// the supplied matrix of 
			bspline = new BSpline(m2);
			
			bspline->get_matrix(m2, k, j, d);
			/* Store originals of b-spline at level j and j - 1 */
			this->resolutionImages.pop_back();
			this->resolutionImages.push_back(m1);
			this->resolutionImages.push_back(m2);
			
			lastLOD = l;
			

		}
	}else if(l >= lastLOD + 1){
		// convert to a bspline at level (j + 1) by computing a 
		// matrix that gives us a new, larger set of bspline knots
		Matrix orignial = this->resolutionImages.back();
		this->resolutionImages.pop_back();
		if (expand(m1, orignial, this->resolutionImages.back(), m2, k, j, d)) {
			/* Save the detail image. */
			this->detailImages.pop_back();
			
			// deallocate the original bspline data
			delete bspline;

			// create a new bspline at level (j+1) using
			// the supplied matrix of 
			bspline = new BSpline(m2);
			
			lastLOD = l;
		}

	}
	
}