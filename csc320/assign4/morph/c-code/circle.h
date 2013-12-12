/********************************************************************************
 * Class Circle inherits from ImagePrimitive. 
 *
 * This class serves as a representation of a circle in 2D space, consisting of
 * x and y coordinates for the circle centre, and a radius.
 *******************************************************************************/
#ifndef CIRCLE_H
#define CIRCLE_H

#include <FL/gl.h>
#include "imageprimitive.h"
#include <string>
#include "imdraw.h"

using namespace std;

class Circle : public ImagePrimitive{
	public:
		
		/* Circle constructor. */
		Circle(int x1, int y1, int x2, int y2, ImDraw *imagePanel);
		
		/* Display this circle on the screen. */
		virtual void draw() const;

		/* Write the string representation of this circle. */
		virtual string toString() const;

		/* Return the radius of this circle. */
		int getRadius() const;

		/* Return the x-coordinate of this circle's center. */
		int getX1() const;

		/* Return the y-coordinate of this circle's center. */
		int getY1() const;


		/* Set the x-coordinate of this circle's center to x. */
		void setX1(int x1);

		/* Set the y-coordinate of this circle's center to y. */
		void setY1(int y1);


		/* Return the x-coordinate of a point on this circle's boundary. */
		int getX2() const;
		
		/* Return the y-coordinate of a point on this circle's boundary. */
		int getY2() const;
		
		/* Set the x-coordinate of a point on this circle's boundary to x2. */
		void setX2(int x2);
		
		/* Set the y-coordinate of a point on this circle's boundary to y2. */
		void setY2(int y2);
		
	private:

		/*
			x1 and y1 denote the coordinates of this circle's center.
			x2 and y1 denot the coordinates of a point on this circle's boundary.
			imagePanel is a pointer to an ImDraw instance containing this circle.
		*/
		int x1, y1;
		int x2, y2;
		ImDraw *imagePanel;

};

#endif
