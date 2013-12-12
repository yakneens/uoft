/********************************************************************************
 * Implementation of class ImagePrimitive. For declaration see "imageprimitive.h" 
 *******************************************************************************/
#include "imageprimitive.h"



/********************************************************************************
 * Default constructor for ImagePrimitive.
 *******************************************************************************/
ImagePrimitive::ImagePrimitive(){return;}

/********************************************************************************
 * Constructor for ImagePrimitive. Initializes the object type of this 
 * ImagePrimitive to type.
 *******************************************************************************/
ImagePrimitive::ImagePrimitive(ObjectType type){this->type = type; return;}

/* Destructor for ImagePrimitive. */
ImagePrimitive::~ImagePrimitive(){;}

/********************************************************************************
 * function getType() - returns the type of this ImagePrimitive.
 *******************************************************************************/
ImagePrimitive::ObjectType ImagePrimitive::getType() const{
	return this->type;
}
