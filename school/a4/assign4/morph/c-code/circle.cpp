/********************************************************************************
 * Implementation of class Circle. For declaration see "circle.h" 
 *******************************************************************************/
#include "circle.h"
#include <math.h>
#include <sstream>

/********************************************************************************
 * Constructor for class Circle. Initializes the members of this Circle to the 
 * values passed as parameters.
 *******************************************************************************/
Circle::Circle(int x1, int y1, int x2, int y2, ImDraw *imagePanel) : ImagePrimitive(circle){
	this->x1 = x1;
	this->y1 = y1;
	this->x2 = x2;
	this->y2 = y2;
	this->imagePanel = imagePanel;

};

/********************************************************************************
 * function draw() - draws this circle on the screen. The approximation to a
 * circle uses 365 line segments. 
 *******************************************************************************/
void Circle::draw() const{

	/* Angle increment */
	double angleInc = 2 * 3.14159265 / 365;
	
	double x,y;
	
	/* convert coordinates of the centre from vista to gl coordinates. */
	if(imagePanel->vista2gl(this->x1, this->y1, x, y)){
	
		glBegin(GL_LINE_LOOP);
		
		/* Draw 365 line segments approximating a circle. Endpoints of all line segments
		   are getRadius() units away from the center of the circle. */
		for(int i = 0; i < 365; i++){

			glVertex2d(x + getRadius() * (cos(angleInc * i)), y + getRadius() * sin(angleInc * i));
		}
		glEnd();
		glFlush();
	}
}

/********************************************************************************
 * function toString() - returns a string representation of this circle. The
 * string representation is of the form <c> <row> <col> <radius> where <c> 
 * denotes the letter c and <row>, <col>, and <radius> denote the x and y 
 * coordinates of the center, and the radius of this circle respectively.
 *******************************************************************************/
string Circle::toString() const{
	std::stringstream ss;
	ss << "c " << this->x1 << " " << this->y1 << " " << getRadius() << "\n";
	return ss.str();
}


/********************************************************************************
 * function getRadius() - returns the radius of this circle 
 *******************************************************************************/
int Circle::getRadius() const{
	double xv1, xv2, yv1, yv2;
	imagePanel->vista2gl(this->x1, this->y1, xv1, yv1);
	imagePanel->vista2gl(this->x2, this->y2, xv2, yv2);
	return (int)floor(sqrt(pow(xv1 - xv2, 2) + pow(yv1 - yv2, 2)));
}

/********************************************************************************
 * function getX1() - returns the x-coordinate of the centre of this circle.
 *******************************************************************************/
int Circle::getX1() const{
	return this->x1;
}

/********************************************************************************
 * function getY1() - returns the y-coordinate of the centre of this circle.
 *******************************************************************************/
int Circle::getY1() const{
	return this->y1;
}


/********************************************************************************
 * function setX1() - sets the x-coordinate of the centre of this circle to x1.
 *******************************************************************************/
void Circle::setX1(int x1){
	this->x1 = x1;
}

/********************************************************************************
 * function setY1() - sets the y-coordinate of the centre of this circle to y1.
 *******************************************************************************/
void Circle::setY1(int y1){
	this->y1 = y1;
}

/********************************************************************************
 * function getX2() - returns the x-coordinate of a point on the circle boundary.
 *******************************************************************************/
int Circle::getX2() const{
	return this->x2;
}

/********************************************************************************
 * function getY2() - returns the y-coordinate of a point on the circle boundary.
 *******************************************************************************/
int Circle::getY2() const{
	return this->y2;
}


/********************************************************************************
 * function setX2() - sets the x-coordinate of a point on the circle boundary to
 * x2.
 *******************************************************************************/
void Circle::setX2(int x2){
	this->x2 = x2;
}

/********************************************************************************
 * function setY2() - sets the y-coordinate of a point on the circle boundary to
 * y2.
 *******************************************************************************/
void Circle::setY2(int y2){
	this->y2 = y2;
}
