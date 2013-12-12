/********************************************************************************
 * Implementation of class Line. For declaration see "line.h" 
 *******************************************************************************/
#include "line.h"
#include <sstream>
#include <math.h>
#include "../glutils.h"

/********************************************************************************
 * Constructor for class Line. Initializes the members of this Line to values 
 * passed as parameters.
 *******************************************************************************/
Line::Line(int x1, int y1, int x2, int y2, ImDraw *imagePanel) : ImagePrimitive(line){

	this->x1 = x1;
	this->y1 = y1;
	this->x2 = x2;
	this->y2 = y2;	
	this->imagePanel = imagePanel;
	this->magnitude = sqrt(pow((double)(x1 - x2), 2) + pow((double)(y1 - y2), 2));

};


/********************************************************************************
 * function draw() - draws this line on the screen.
 *******************************************************************************/
void Line::draw() const{
	double x, y;
	
	/* Draw a line segment between (x1,y1) and (x2,y2). 
	   Coordinates of both endpoints are converted from
	   vista to gl coordinates.
	 */
	glBegin(GL_LINES);
	imagePanel->vista2gl(this->x1, this->y1, x, y);
	glVertex2d(x, y);
	imagePanel->vista2gl(this->x2, this->y2, x, y);
	glVertex2d(x, y);
	glEnd();
	glFlush();

}


/********************************************************************************
 * function toString() - returns a string representation of this line. The 
 * representation is of the form <l> <row1> <col1> <row2> <col2>, where <l> 
 * denotes the character l, and <row1>, <col1>, <row2>, and <col2>, denote the 
 * x and y coordinates of this line segment's first and second endpoints
 * respectively.
 *******************************************************************************/
string Line::toString() const{
	std::stringstream ss;
	ss << "l " << this->x1 << " " << this->y1 << " " << this->x2 << " " << this->y2 << "\n";
	return ss.str();
}


/********************************************************************************
 * function setX1() - sets the x-coordinate of the first endpoint of this line
 * segment to x1.
 *******************************************************************************/
void Line::setX1(int x1){
	this->x1 = x1;
	this->magnitude = sqrt(pow((double)(x1 - x2), 2) + pow((double)(y1 - y2), 2));
}


/********************************************************************************
 * function setX2() - sets the x-coordinate of the second endpoint of this line
 * segment to x2.
 *******************************************************************************/
void Line::setX2(int x2){
	this->x2 = x2;
	this->magnitude = sqrt(pow((double)(x1 - x2), 2) + pow((double)(y1 - y2), 2));
}


/********************************************************************************
 * function setY1() - sets the y-coordinate of the first endpoint of this line
 * segment to y1.
 *******************************************************************************/
void Line::setY1(int y1){
	this->y1 = y1;
	this->magnitude = sqrt(pow((double)(x1 - x2), 2) + pow((double)(y1 - y2), 2));
}


/********************************************************************************
 * function setY2() - sets the y-coordinate of the second endpoint of this line
 * segment to y2.
 *******************************************************************************/
void Line::setY2(int y2){
	this->y2 = y2;
	this->magnitude = sqrt(pow((double)(x1 - x2), 2) + pow((double)(y1 - y2), 2));
}


/********************************************************************************
 * function getX1() - returns the x-coordinate of the first endpoint of this line
 * segment.
 *******************************************************************************/
int Line::getX1() const{
	return this->x1;
}


/********************************************************************************
 * function getX2() - returns the x-coordinate of the second endpoint of this 
 * line segment.
 *******************************************************************************/
int Line::getX2() const{
	return this->x2;
}


/********************************************************************************
 * function getY1() - returns the y-coordinate of the first endpoint of this line
 * segment.
 *******************************************************************************/
int Line::getY1() const{
	return this->y1;
}


/********************************************************************************
 * function getY2() - returns the y-coordinate of the second endpoint of this
 * line segment.
 *******************************************************************************/
int Line::getY2() const{
	return this->y2;
}

/********************************************************************************
 * function translateLine() - translates this line by vector (x,y)
 *******************************************************************************/
void Line::translateLine(int x, int y){
	this->x1 += x;
	this->x2 += x;
	this->y1 += y;
	this->y2 += y;
}


/********************************************************************************
 * function getMagnitude() - returns the magnitude of this line
 *******************************************************************************/
double Line::getMagnitude() const{
	return this->magnitude;
}
