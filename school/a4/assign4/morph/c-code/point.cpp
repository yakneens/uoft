/********************************************************************************
 * Implementation of class Point. For declaration see "point.h" 
 *******************************************************************************/
#include "point.h"
#include <sstream>
#include "imdraw.h"

/********************************************************************************
 * Constructor for class Point. Initializes members of this point to values
 * passed as parameters.
 *******************************************************************************/
Point::Point(int x, int y, ImDraw *imagePanel) : ImagePrimitive(point){
	this->x = x;
	this->y = y;
	this->imagePanel = imagePanel;
}

/********************************************************************************
 * function getX() - returns the x-coordinate of this point.
 *******************************************************************************/
int Point::getX() const{
	return this->x;
}

/********************************************************************************
 * function setX() - sets the x-coordinate of this point to x.
 *******************************************************************************/
void Point::setX(int x){
	this->x = x;
}

/********************************************************************************
 * function getY() - returns the y-coordinate of this point.
 *******************************************************************************/
int Point::getY() const{
	return this->y;
}

/********************************************************************************
 * function setY() - sets the y-coordinate of this point to y.
 *******************************************************************************/
void Point::setY(int y){
	this->y = y;
}

/********************************************************************************
 * function draw() - draws this point on the sreen.
 *******************************************************************************/
void Point::draw() const{
	double x,y;
	
	/* Convert the coordinates of the point from vista to gl coordinates. */
	imagePanel->vista2gl(this->x, this->y, x, y);

	/* Draw the point on the sceen. */
	glBegin(GL_POINTS);
	glVertex2d(x,y);
	glEnd();
	glFlush();
}

/********************************************************************************
 * function toString() - returns a string representation of this point. The
 * representation consists of <P> <row> <col>, where <P> denotes the letter P and
 * <row> and <col> denote the x and y coordinates of this point respectively.
 *******************************************************************************/
string Point::toString() const{
	std::stringstream ss;
	ss << "P " << this->x << " " << this->y << "\n";
	return ss.str();
}
