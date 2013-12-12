/********************************************************************************
 * Class Point inherits from ImagePrimitive.
 *
 * This class serves as a representation of a point in 2D space.
 *******************************************************************************/
#ifndef POINT_H
#define POINT_H


#include "imageprimitive.h"
#include <FL/gl.h>
#include "imdraw.h"

using namespace std;

class Point : public ImagePrimitive{
	public:
		
		/* Point constructor. */
		Point(int x, int y, ImDraw *imagePanel);
		
		/* Returns the x-coordinate of this point. */
		int getX() const;

		/* Sets the x-coordinate of this point to x. */
		void setX(int x);

		/* Returns the y-coordinate of this point. */
		int getY() const;

		/* Sets the y-coordinate of this point to y. */
		void setY(int y);

		/* Draws this point on the screen. */
		virtual void draw() const;

		/* Returns the string representation of this point. */
		virtual string toString() const;
	private:
		
		/* x and y denote the coordinates of this point in 2D space. 
		   imagePanel is a pointer to an ImDraw instance containing this circle.
		 */
		int x, y;
		ImDraw *imagePanel;
		
};

#endif
