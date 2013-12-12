/********************************************************************************
 * Implementation of class Color. For declaration see "color.h" 
 *******************************************************************************/
#include "color.h"

#include <sstream>
#include "imdraw.h"
 
/********************************************************************************
 * Constructor for class Color. Initializes members of this color to values
 * passed as parameters.
 *******************************************************************************/
Color::Color(unsigned int r, unsigned int g, unsigned int b) : ImagePrimitive(color){
	this->r = r;
	this->g = g;
	this->b = b;
}

/********************************************************************************
 * function getR() - returns the value of the red component of this color.
 *******************************************************************************/
unsigned int  Color::getR(){
	return this->r;
}

/********************************************************************************
 * function getG() - returns the value of the green component of this color.
 *******************************************************************************/
unsigned int  Color::getG(){
	return this->g;
}

/********************************************************************************
 * function getB() - returns the value of the blue component of this color.
 *******************************************************************************/
unsigned int  Color::getB(){
	return this->b;
}

/********************************************************************************
 * function setR() - sets the value of the red component of this color to r.
 *******************************************************************************/
void Color::setR(unsigned int  r){
	this->r = r;
}

/********************************************************************************
 * function setG() - sets the value of the green component of this color to g.
 *******************************************************************************/
void Color::setG(unsigned int  g){
	this->g = g;
}

/********************************************************************************
 * function setB() - sets the value of the blue component of this color to b.
 *******************************************************************************/
void Color::setB(unsigned int  b){
	this->b = b;
}
/********************************************************************************
 * function draw() - sets the current drawing color to this color.
 *******************************************************************************/
void Color::draw() const{
	
	
	/* Colors from range 0-255 are mapped onto range 0-1 by dividing each
	   component by 255. */
	double tempR, tempG, tempB;

	tempR = this->r;
	tempG = this->g;
	tempB = this->b;

	tempR /= 255;
	tempG /= 255;
	tempB /= 255;

	glColor3d(tempR, tempG, tempB);
}

/********************************************************************************
 * function toString() - returns a string representation of this color. The
 * representation consists of <C> <r> <g> <b>, where <C> denotes the letter C and
 * <r>, <g> and <b> denote the red. green and blue components of this color
 * respectively.
 *******************************************************************************/
string Color::toString() const{
	std::stringstream ss;
	ss << "C " << this->r << " " << this->g << " " << this->b << "\n";
	return ss.str();
}
