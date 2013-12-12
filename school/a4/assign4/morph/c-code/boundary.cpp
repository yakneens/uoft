/********************************************************************************
 * Implementation of class Boundary. For declaration see "boundary.h" 
 *******************************************************************************/
#include "boundary.h"
#include <sstream>
#include <vista/VImage.h>
#include <math.h>

/********************************************************************************
 * Constructor for class Boundary. The boundary is initially not complete.
 *******************************************************************************/
Boundary::Boundary(ImDraw *imagePanel) : ImagePrimitive(boundary){
	this->complete = false;
	this->imagePanel = imagePanel;
	
};

/********************************************************************************
 * Constructor for class Boundary. Initializes members of this boundary to values
 * passed as parameters. The boundary is initially not complete.
 *******************************************************************************/
Boundary::Boundary(int x, int y, ImDraw *imagePanel) : ImagePrimitive(boundary){
	this->complete = false;
	this->imagePanel = imagePanel;
	Point* myPoint = new Point(x, y, imagePanel);
	
	this->vertices.push_back(*myPoint);
	this->seedPoints.push_back(*myPoint);
	
	/* Reinitialize pathfinder data structure. */
	this->imagePanel->pathFinderReInit();

	/* Compute path costs based on the given seed. */
	this->imagePanel->computePathCosts(x, y);
};

/********************************************************************************
 * function draw() - draws this boundary on the sceen.
 *******************************************************************************/
void Boundary::draw() const{
	double x,y;
	
	/* Red. */
	glColor3d(1,0,0);


	
	
	/* Draw all the seed points as larger size points followed by all the regular
	   points as smaller size points. */

	glPointSize(5);
	glBegin(GL_POINTS);
	for(int j = 0; j < (int)this->seedPoints.size(); j++){
		imagePanel->vista2gl(this->seedPoints[j].getX(), this->seedPoints[j].getY(), y, x);
		glVertex2d(x, y);
	}
	glEnd();
	glFlush();

	glPointSize(1);

	glBegin(GL_POINTS);
	
	for(int i = 0; i < (int)this->vertices.size(); i++){
		imagePanel->vista2gl(this->vertices[i].getX(), this->vertices[i].getY(), y, x);
		glVertex2d(x, y);
	}
	glEnd();
	glFlush();

}

/********************************************************************************
 * function toString() - returns a string representation of this boundary, of the
 * form <p> <r1> <c1> <r2> <c2> <r3> <c3> ..., where <p> denotes the letter p, 
 * <rN> <cN> denote the x and y coordinates of the Nth vertex of the boundary.
 *******************************************************************************/
string Boundary::toString() const{
	std::stringstream ss;
	ss << "p";
	for(int i = 0; i < (int)vertices.size(); i++){
		ss << " " << vertices[i].getX() << " " << vertices[i].getY();	
	}

	ss << "\n";
	return ss.str();
}

/********************************************************************************
 * function verticesToString() - returns a string representation of this boundary
 * of the form <P> <r1> <c1> <P> <r2> <c2> <P> <r3> <c3> ..., 
 * where <P> denotes the letter P, 
 * <rN> <cN> denote the x and y coordinates of the Nth vertex of the boundary.
 *******************************************************************************/
string Boundary::verticesToString() const{
	std::stringstream ss;
	
	for(int i = 0; i < (int)vertices.size(); i++){
		ss << "P" << " " << vertices[i].getY() << " " << vertices[i].getX();	
	}

	ss << "\n";
	return ss.str();
}

/********************************************************************************
 * function seedPointsToString() - returns a string representation of this boundary, 
 * of the form <P> <r1> <c1> <P> <r2> <c2> <P> <r3> <c3> ..., 
 * where <P> denotes the letter P, 
 * <rN> <cN> denote the x and y coordinates of the Nth seed point of the boundary.
 *******************************************************************************/
string Boundary::seedPointsToString() const{
	std::stringstream ss;
	
	for(int i = 0; i < (int)seedPoints.size(); i++){
		ss << "P" << " " << seedPoints[i].getY() << " " << seedPoints[i].getX();	
	}

	ss << "\n";
	return ss.str();
}

/********************************************************************************
 * function isComplete() - returns whether this boundary is complete. A boundary is
 * complete when it is closed.
 *******************************************************************************/
bool Boundary::isComplete() const{
	return this->complete;
}

/********************************************************************************
 * function addPoint() - adds a new vertex to this boundary with coordinates x 
 * and y.
 *******************************************************************************/
void Boundary::addPoint(int x, int y){
	Point* myPoint = new Point(x, y, imagePanel);

	this->vertices.push_back(*myPoint);
}

/********************************************************************************
 * function addSeedPoint() - adds a new seed point to this boundary with 
 * coordinates x and y. Recomputes path costs based on new seed point.
 *******************************************************************************/
void Boundary::addSeedPoint(int x, int y){
	int width = this->imagePanel->getImageWidth();
	int height = this->imagePanel->getImageHeight();
	int npixels;
	int pixelPos = y * width + x;

	VDouble *ancest, *ancestInit;
	
	if(!VSelectBand("Link Weights", this->imagePanel->nodeImage, 1, &npixels, (VPointer*)&ancest))
			VError("Could not get band %d of gradient image", 1);
	ancestInit = ancest;
	ancest += pixelPos;

	Point* myPoint = new Point(x, y, imagePanel);
	
	/* Retrieve the image node coresponding to x and y. */
	//Node *tempNode = &(this->imagePanel->getImageNodes()[width * y + x]);
	
	/* Traverse the path from this seed point to last seed point, adding vertices
	   to this boundary at each step. */
	int seedPointPos = this->seedPoints.back().getX() + 
			this->seedPoints.back().getY() * width;
	if(this->seedPoints.size() > 0){
		while(!(*ancest == seedPointPos)){
			int xpos = (int)(*ancest) % width;
			int ypos = (int)floor((double)(*ancest / width));
			this->vertices.push_back(*(new Point(xpos, ypos, imagePanel)));
			ancest = ancestInit + (int)*ancest;
		}
	}

	/* Add the new seed point itself to the boundary. */
	this->vertices.push_back(*myPoint);
	
	this->seedPoints.push_back(*myPoint);
	
	/* Recompute path costs. */
	this->imagePanel->pathFinderReInit();
	this->imagePanel->computePathCosts(x, y);
}

/********************************************************************************
 * function completeBoundary() - completes this boundary.
 *******************************************************************************/
void Boundary::completeBoundary(){
	if(this->vertices.size() > 0 && this->seedPoints.size() > 0){

		
		/* Add a seed point with coordinates of the first seed point. */
		this->addSeedPoint(this->seedPoints.front().getX(), this->seedPoints.front().getY());
		
		/* Set the complete flag. */
		this->complete = true;
	}
}

/********************************************************************************
 * function isLastVertex() - returns whether the most recently added vertex of
 * this boundary has coordinates (x,y).
 *******************************************************************************/
bool Boundary::isLastVertex(int x, int y){
	return (this->vertices.back().getX() == x && this->vertices.back().getY() == y);
}

/********************************************************************************
 * function setBack() - sets the coordinates of the most recently added vertex
 * of this boundary to x and y.
 *******************************************************************************/
void Boundary::setBack(int x, int y){
	if(this->vertices.size() > 0){
		this->vertices.back().setX(x);
		this->vertices.back().setY(y);
	}
}

/********************************************************************************
 * function getVerticesBegin() - returns an iterator positioned at the first
 * vertex of this boundary.
 *******************************************************************************/
std::deque<Point>::iterator Boundary::getVerticesBegin(){
	 std::deque<Point>::iterator it = this->vertices.begin();

	return it;
}

/********************************************************************************
 * function getVerticesEnd() - returns an iterator positioned after the last
 * vertex of this boundary.
 *******************************************************************************/
std::deque<Point>::iterator Boundary::getVerticesEnd(){
	 std::deque<Point>::iterator it = this->vertices.end();

	return it;
}
