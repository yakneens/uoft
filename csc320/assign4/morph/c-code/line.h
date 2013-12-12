/********************************************************************************
 * Class Line inherits from ImagePrimitive.
 *
 * This class serves as a representation of a line segment in 2D space 
 * consisting of the x and y coordinates of the line segment's endpoints.
 *******************************************************************************/
#ifndef LINE_H
#define LINE_H

#include <FL/gl.h>
#include "imageprimitive.h"
#include "imdraw.h"

using namespace std;

class Line : public ImagePrimitive{
	public:
		
		/* Line constructor. */
		Line(int x1, int y1, int x2, int y2, ImDraw *imagePanel);
		
		/* Display the line on the screen. */
		virtual void draw() const;
		
		/* Write the string representation of this line. */
		virtual string toString() const;
		
		/* Set the x coordinate of the first endpoint of this line to x1. */
		void setX1(int x1);

		/* Set the y coordinate of the first endpoint of this line to y1. */
		void setY1(int y1);
		
		/* Set the x coordinate of the second endpoint of this line to x2. */
		void setX2(int x2);
		
		/* Set the y coordinate of the second endpoint of this line to y2. */
		void setY2(int y2);
		
		/* Return the x coordinate of the first endpoint of this line. */
		int getX1() const;

		/* Return the x coordinate of the second endpoint of this line. */
		int getX2() const;
		
		/* Return the y coordinate of the first endpoint of this line. */
		int getY1() const;
		
		/* Return the y coordinate of the second endpoint of this line. */
		int getY2() const;

		/* Returns the magnitude of this line. */
		double getMagnitude() const;

		/* Translate this line by vector (x,y). */
		void translateLine(int x, int y);		

	private:
		
		/* x1 and y1 denote coordinates of the first endpoint of this line.
		   x2 and y2 denote coordinates of the second endpoint of this line. 
		   imagePanel is a pointer to an ImDraw instance containing this circle.
		 */
		int x1, y1, x2, y2;
		double magnitude;
		ImDraw *imagePanel;
};

#endif
