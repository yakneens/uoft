/********************************************************************************
 * Class Rect inherits from ImagePrimitive.
 *
 * This class serves as a representation of and axis-aligned rectangle in 2D 
 * space.
 *******************************************************************************/
#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <FL/gl.h>

#include "imageprimitive.h"
#include "imdraw.h"

using namespace std;
class Rect : public ImagePrimitive{
	public:
		
		/* Constructor for class Rect. */
		Rect(int x1, int y1, int x2, int y2, ImDraw *imagePanel);
		
		/* Draw this rectangle on the screen. */
		virtual void draw() const;

		/* Returns the string representation of this rectangle. */
		virtual string toString() const;

		/* Set the x-coordinate of the first vertex of this rectangle
		   to x1.
		 */
		void setX1(int x1);

		/* Set the y-coordinate of the first vertex of this rectangle
		   to y1.
		 */
		void setY1(int y1);

		/* Set the x-coordinate of the second vertex of this rectangle
		   to x2.
		 */
		void setX2(int x2);

		/* Set the y-coordinate of the second vertex of this rectangle
		   to y2.
		 */
		void setY2(int y2);

		/* Return the x-coordinate of the first vertex of this rectangle. */
		int getX1() const;

		/* Return the x-coordinate of the second vertex of this rectangle. */
		int getX2() const;

		/* Return the y-coordinate of the first vertex of this rectangle. */
		int getY1() const;

		/* Return the y-coordinate of the second vertex of this rectangle. */
		int getY2() const;
	private:
		/* x1, y1 denote the x and y coordinates of the first vertex of this
		   rectangle respectively.
		   x2, y2 denote the x and y coordinates of the second vertex of this
		   rectangle respectively.
		   imagePanel is a pointer to an ImDraw instance that contains this
		   rectangle.
		 */
		int x1, y1, x2, y2;
		ImDraw *imagePanel;
};

#endif
