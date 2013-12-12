/********************************************************************************
 * Abstract class ImagePrimitive. 
 *
 * Serves as a base class for all shapes and colors. Contains ObjectType 
 * enumeration for determination of type of shape at run-time. The type is set at 
 * object creation and can subsequently only be retreived.
 *******************************************************************************/
#ifndef IMAGEPRIMITIVE_H
#define IMAGEPRIMITIVE_H

#include <string>
using namespace std;



class ImagePrimitive{
	public:
		
		/* Defines an enumeration of legal object types. */
		enum ObjectType{point, line, rect, circle, poly, color, boundary};
		
		/* Default constructor */
		ImagePrimitive();

		/* Constructor that sets an ImagePrimitive's object type. */
		ImagePrimitive(ObjectType type);
		
		/* Destructor for ImagePrimitive */
		virtual ~ImagePrimitive();

		/* Displays this ImagePrimitive on the screen. */
		virtual void draw() const = 0;

		/* Returns the string representation of this ImagePrimitive. */
		virtual string toString() const = 0;

		/* Returns the object type of this ImagePrimitive. */
		ObjectType getType() const;
	protected:
		
		/* Not currently used. */
		double id;
		
		/* Defines the object type of this ImagePrimitive. */
		ObjectType type;
};
	
	
#endif
