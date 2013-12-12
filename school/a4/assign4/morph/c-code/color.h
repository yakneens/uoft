/********************************************************************************
 * Class Color inherits from ImagePrimitive.
 *
 * This class serves as a representation of a color in RGB space.
 *******************************************************************************/
#ifndef COLOR_H
#define COLOR_H


#include "imageprimitive.h"
#include <FL/gl.h>
using namespace std;


class Color : public ImagePrimitive{
	public:
		
		/* Color constructor. */
		Color(unsigned int  r, unsigned int  g, unsigned int  b);
		
		/* Return the value of the red component of this color. */
		unsigned int getR();

		/* Return the value of the green component of this color. */
		unsigned int getG();

		/* Return the value of the blue component of this color. */
		unsigned int getB();

		/* Set the value of the red component of this color to r. */
		void setR(unsigned int r);

		/* Set the value of the green component of this color to g. */
		void setG(unsigned int g);

		/* Set the value of the blue component of this color to b. */
		void setB(unsigned int b);

		/* Sets the current drawing color to this color. */
		virtual void draw() const;

		/* Returns the string representation of this color. */
		virtual string toString() const;
	private:
		
		/* 
			r, g, and b denote the red, green, and blue components of this color
			respectively.
		 */
		unsigned int r, g, b;
		
};

#endif
