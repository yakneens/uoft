// (c) Kyros Kutulakos 10/15/99

#ifndef _glutils_h
#define _glutils_h

#include <vista/VImage.h>

#define glutils_FONT_DISPLAY_LIST_BASE 100

// Convert a Vista image into an RGBA format appropriate for
// glCopyPixels, glDrawPixels, glTexImage2D, and general texture-mapping 
// operations and return the pointer 
GLubyte* glutils_Vista_to_RGBA(VImage im);

// Draw a Vista image using OpenGL 
void glutils_draw_VImage(VImage im);

// Create a Vista image with the contents of the current GL buffer
// The routine requires the dimensions of the GL buffer
VImage glutils_read_VImage(int width, int height);

// Convert the current RGBA OpenGL buffer into a 3-component color Vista image
VImage glutils_RGBA_to_Vista(GLubyte* data, int width, int height);

#endif
