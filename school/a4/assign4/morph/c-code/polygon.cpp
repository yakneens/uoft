/********************************************************************************
 * Implementation of class Poly. For declaration see "poly.h" 
 *******************************************************************************/
#include "polygon.h"
#include <sstream>

/********************************************************************************
 * Constructor for class Poly. The polygon is initially not complete.
 *******************************************************************************/
Poly::Poly(ImDraw *imagePanel) : ImagePrimitive(poly){
	this->complete = false;
	this->imagePanel = imagePanel;
	
};

/********************************************************************************
 * Constructor for class Poly. Initializes members of this polygon to values
 * passed as parameters. The polygon is initially not complete.
*******************************************************************************/
Poly::Poly(int x, int y, ImDraw *imagePanel) : ImagePrimitive(poly){
	this->complete = false;
	this->imagePanel = imagePanel;
	Point* myPoint = new Point(x, y, imagePanel);
	
	this->vertices.push_back(*myPoint);
};

/********************************************************************************
 * function draw() - draws this polygon on the sceen.
 *******************************************************************************/
void Poly::draw() const{
	double x,y;
	glBegin(GL_LINE_STRIP);
	
	/* For each vertex in the polygon, convert the coordinates of that vertex
	   from vista to gl format and draw a line segment between this vertex and
	   the previous vertex. 
	 */
	for(int i = 0; i < (int)this->vertices.size(); i++){
		imagePanel->vista2gl(this->vertices[i].getX(), this->vertices[i].getY(), x, y);
		glVertex2d(x, y);
	}
	glEnd();
	glFlush();

}

/********************************************************************************
 * function toString() - returns a string representation of this polygon, of the
 * form <p> <r1> <c1> <r2> <c2> <r3> <c3> ..., where <p> denotes the letter p, 
 * <rN> <cN> denote the x and y coordinates of the Nth vertex of the polygon
 *******************************************************************************/
string Poly::toString() const{
	std::stringstream ss;
	ss << "p";
	for(int i = 0; i < (int)vertices.size(); i++){
		ss << " " << vertices[i].getX() << " " << vertices[i].getY();	
	}

	ss << "\n";
	return ss.str();
}

/********************************************************************************
 * function isComplete() - returns whether this polygon is complete. A polygon is
 * complete when it is closed.
 *******************************************************************************/
bool Poly::isComplete() const{
	return this->complete;
}

/********************************************************************************
 * function addPoint() - adds a new vertex to this polygon with coordinates x 
 * and y.
 *******************************************************************************/
void Poly::addPoint(int x, int y){
	Point* myPoint = new Point(x, y, imagePanel);

	this->vertices.push_back(*myPoint);
}

/********************************************************************************
 * function completePoly() - completes this polygon.
 *******************************************************************************/
void Poly::completePoly(){
	if(this->vertices.size() > 0){

		/* Set the complete flag. */
		this->complete = true;

		/* Add a new vertex with the same coordinates as the first vertex. */
		addPoint(this->vertices.front().getX(), this->vertices.front().getY());
	}
}

/********************************************************************************
 * function isLastVertex() - returns whether the most recently added vertex of
 * this polygon has coordinates (x,y).
 *******************************************************************************/
bool Poly::isLastVertex(int x, int y){
	return (this->vertices.back().getX() == x && this->vertices.back().getY() == y);
}

/********************************************************************************
 * function setBack() - sets the coordinates of the most recently added vertex
 * of this polygon to x and y.
 *******************************************************************************/
void Poly::setBack(int x, int y){
	if(this->vertices.size() > 0){
		this->vertices.back().setX(x);
		this->vertices.back().setY(y);
	}
}
