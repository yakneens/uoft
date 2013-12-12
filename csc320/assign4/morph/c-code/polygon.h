/********************************************************************************
 * Class Poly inherits from ImagePrimitive
 *
 * This class serves as a representation of a polygon with an arbitrary number of
 * vertices in 2D space.
 *******************************************************************************/

#ifndef POLYGON_H
#define POLYGON_H

#include <FL/gl.h>
#include <iostream.h>
#include <deque>
#include "point.h"
#include "imdraw.h"

using namespace std;

class Poly : public ImagePrimitive{
	public:
		
		/* Constructors for class Poly. */
		Poly(ImDraw *imagePanel);
		Poly(int x, int y, ImDraw *imagePanel);

		/* Draws this polygon on the sceen. */
		virtual void draw() const;

		/* Returns the string representation of this polygon. */
		virtual string toString() const;

		/* Returns whether this polygon is complete (closed). */
		bool isComplete() const;

		/* Completes this polygon by creating a vertex that coincides
		   with the first vetex.
		 */
		void completePoly();

		/* Returns whether the vertex that has been most recently defined
		   is located at point (x,y)
		 */
		bool isLastVertex(int x, int y);

		/* Adds a new vertex to this polygon. */
		void addPoint(int x, int y);

		/* Set the coordinates of the most recently defined vertex to x, and y. */ 
		void setBack(int x, int y);

	private:

		/* A queue of polygon vertices. */
		deque<Point> vertices;

		/* Flag that determines whether the polygon is complete (closed). */
		bool complete;

		/* Pointer to an ImDraw instance containing this polygon. */
		ImDraw *imagePanel;
};

#endif
