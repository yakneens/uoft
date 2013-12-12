/********************************************************************************
 * Implementation of class Rect. For declaration see "rectangle.h" 
 *******************************************************************************/

#include "rectangle.h"
#include <sstream>

/********************************************************************************
 * Constructor for class Rect. Initializes the members of this rectangle to 
 * values passed as parameters.
 *******************************************************************************/
Rect::Rect(int x1, int y1, int x2, int y2, ImDraw *imagePanel) : ImagePrimitive(rect){
	this->x1 = x1;
	this->y1 = y1;
	this->x2 = x2;
	this->y2 = y2;
	this->imagePanel = imagePanel;
};

/********************************************************************************
 * function draw() - draws this rectangle on the screen
 *******************************************************************************/
void Rect::draw() const{
	
	/* Draw the rectangle based on two opposite vertices defined by (x1,y1) and
	   (x2, y2). All coordinates are converted from vista to gl. 
	 */
	double x, y;
	glBegin(GL_LINE_LOOP);

	imagePanel->vista2gl(this->x1, this->y1, x, y);
	glVertex2d(x, y);
	imagePanel->vista2gl(this->x2, this->y1, x, y);
	glVertex2d(x, y);
	imagePanel->vista2gl(this->x2, this->y2, x, y);
	glVertex2d(x, y);
	imagePanel->vista2gl(this->x1, this->y2, x, y);
	glVertex2d(x, y);
	glEnd();
}

/********************************************************************************
 * function toString() - returns the string representation of this rectangle, of
 * the form <r> <x1> <y1> <x2> <y2>, where <r> denotes the letter r and <x1>, 
 * <y1>, <x2>, and <y2> denote the x and y coordinates of the first and second
 * vertices of the rectangle repsectively.
 *******************************************************************************/
string Rect::toString() const{
	std::stringstream ss;
	ss << "r " << this->x1 << " " << this->y1 << " " << this->x2 << " " << this->y2 << "\n";
	return ss.str();
}

/********************************************************************************
 * function setX1() - sets the x-coordinate of the first vertex of the rectangle
 * to x1.
 *******************************************************************************/
void Rect::setX1(int x1){
	this->x1 = x1;
}

/********************************************************************************
 * function setX2() - sets the x-coordinate of the second vertex of the rectangle
 * to x2.
 *******************************************************************************/
void Rect::setX2(int x2){
	this->x2 = x2;
}

/********************************************************************************
 * function setY1() - sets the y-coordinate of the first vertex of the rectangle
 * to y1.
 *******************************************************************************/
void Rect::setY1(int y1){
	this->y1 = y1;
}

/********************************************************************************
 * function setY2() - sets the y-coordinate of the second vertex of the rectangle
 * to y2.
 *******************************************************************************/
void Rect::setY2(int y2){
	this->y2 = y2;
}

/********************************************************************************
 * function getX1() - returns the x-coordinate of the first vertex of this
 * rectangle.
 *******************************************************************************/
int Rect::getX1() const{
	return this->x1;
}

/********************************************************************************
 * function getX2() - returns the x-coordinate of the second vertex of this
 * rectangle.
 *******************************************************************************/
int Rect::getX2() const{
	return this->x2;
}

/********************************************************************************
 * function getY1() - returns the y-coordinate of the first vertex of this 
 * rectangle.
 *******************************************************************************/
int Rect::getY1() const{
	return this->y1;
}

/********************************************************************************
 * function getY2() - returns the y-coordinate of the second vertex of this
 * rectangle.
 *******************************************************************************/
int Rect::getY2() const{
	return this->y2;
}
