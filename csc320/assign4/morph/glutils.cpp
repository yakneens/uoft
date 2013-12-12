// (c) Kyros Kutulakos 10/1/99
// Adapted for use in CSC320 course

#include "stdafx.h"
#include <stdlib.h>
#include <FL/gl.h>
#include <vista/VImage.h>
#include "glutils.h"

GLubyte* glutils_Vista_to_RGBA(VImage im)
{
  GLubyte *data, *ptr;
  int r, c, b;

  data = (GLubyte *) 
    calloc(VImageNRows(im)*VImageNColumns(im)*4, sizeof(GLubyte));

  if (!data) {
    VError("glutils_draw_VImage: calloc failed\n");
    return 0;
  }

  switch (VImageNBands(im)) {
  case 1:
    ptr = data;
    for (r=0; r<VImageNRows(im); r++) 
      for (c=0; c<VImageNColumns(im); c++, ptr+=4) {
	*(ptr) = *(ptr+1) = *(ptr+2) 
	  = (GLubyte) VPixel(im, 0, r, c, VUByte);
	*(ptr+3) = (GLubyte) 255;
      }
    break;
  case 3:
    ptr = data;
    for (r=0; r<VImageNRows(im); r++) 
      for (c=0; c<VImageNColumns(im); c++) {
	for (b=0; b<3; b++, ptr++)
	  *(ptr) = (GLubyte) VPixel(im, b, r, c, VUByte);
	*(ptr++) = (GLubyte) 255;
      }
    break;
  default:
    VError("glutils_draw_VImage: Image must have 1 or 3 bands\n");
    return 0;
  } 

  return data;
}

VImage glutils_RGBA_to_Vista(GLubyte* data, int width, int height)
{
  GLubyte *ptr;
  VImage im;
  int r, c, b;

  if (!(im = VCreateImage(3, height, width, VUByteRepn))) {
    VError("glutils_RGBA_to_Vista: VCreateImage failed");
    return 0;
  }
  VSetBandInterp(im, VBandInterpNone, 1, VBandInterpNone, 1,
		 VBandInterpRGB, 3, VBandInterpNone, 1);

  ptr = data;
  for (r=0; r<VImageNRows(im); r++) 
    for (c=0; c<VImageNColumns(im); c++, ptr++) 
      for (b=0; b<3; b++, ptr++)
	VPixel(im, b, r, c, VUByte) = *(ptr);

  return im;
}

VImage glutils_read_VImage(int width, int height) 
{
  GLubyte *data;
  VImage im, flipped;

  data = (GLubyte *) calloc(width*height*4, sizeof(GLubyte));
  if (!data) {
    VSystemError("glutils_read_VImage: calloc failed\n");
    exit(1);
  }
  glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
  flipped = glutils_RGBA_to_Vista(data, width, height);
  // image needs to be flipped because Vista images have their (0,0) point 
  // at the top-left corner of the image, while OpenGL panels have their (0,0)
  // point at their bottom-left corner
  im = VFlipImage(flipped, NULL, VAllBands, 1);
  free(flipped);
  free(data);
  return im;
}
