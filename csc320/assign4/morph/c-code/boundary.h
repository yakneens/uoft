/********************************************************************************
 * Class Boundary inherits from ImagePrimitive
 *
 * This class serves as a representation of a alpha matte boundary with an 
 * arbitrary number of vertices in 2D space.
 *******************************************************************************/

#ifndef BOUNDARY_H
#define BOUNDARY_H

#include <FL/gl.h>
#include <iostream.h>
#include <deque>
#include "point.h"
#include "circle.h"
#include "imdraw.h"
#include <iterator>

using namespace std;

class Boundary : public ImagePrimitive{
	
	public:
		
		/* Constructors for class Boundary. */
		Boundary(ImDraw *imagePanel);
		Boundary(int x, int y, ImDraw *imagePanel);

		/* Draws this boundary on the sceen. */
		virtual void draw() const;

		/* Returns the string representation of this boundary. */
		virtual string toString() const;

		/* Returns whether this boundary is complete (closed). */
		bool isComplete() const;

		/* Completes this boundary by creating a vertex that coincides
		   with the first vetex.
		 */
		void completeBoundary();

		/* Returns whether the vertex that has been most recently defined
		   is located at point (x,y)
		 */
		bool isLastVertex(int x, int y);

		/* Adds a new vertex to this boundary. */
		void addPoint(int x, int y);
		
		/* Adds a new seed point to this boundary. */
		void addSeedPoint(int x, int y);

		/* Set the coordinates of the most recently defined vertex to x, and y. */ 
		void setBack(int x, int y);

		/* Returns an iterator positioned at the first vertex of this boundary. */
		std::deque<Point>::iterator getVerticesBegin();

		/* Returns an iterator positioned at the last vertex of this boundary. */
		std::deque<Point>::iterator getVerticesEnd();

		/* Returns a string representation of the vertices of this boundary. */
		string verticesToString() const;

		/* Returns a string representation of the seed points of this boundary. */
		string seedPointsToString() const;
	private:

		/* A queue of boundary vertices. */
		deque<Point> vertices;

		/* A queue of boundary seed points. */
		deque<Point> seedPoints;

		/* Flag that determines whether the boundary is complete (closed). */
		bool complete;

		/* Pointer to an ImDraw instance containing this boundary. */
		ImDraw *imagePanel;
};

#endif
