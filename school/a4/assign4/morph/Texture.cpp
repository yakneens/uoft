// (c) Kyros Kutulakos 10/15/99 
//    modified on 10/15/99 to include a disable() method in the
//    Texture class Adapted for use in CSC290C course

// Note to CSC320 students: You are not required to read and/or
// understand the code provided in this file. You may want to browse
// through the code at a later date, when we begin covering the topic
// of Texture Mapping in the course lectures

#include "stdafx.h"
#include <FL/gl.h>
#include "Texture.h"
#include "glutils.h"
#include <stdlib.h>
#include <iostream.h>


static int MaxTextureWidth = 1024;
static int MaxTextureHeight = 1024;
static int MaxTextureObjects = 3;

static int initialized=0;
static GLuint *tex_names;
static int top;

int texture_set_globals(int max_texture_width, int max_texture_height,
			int max_texture_objects)
{
  max_texture_objects += 1;
  if (!initialized) {
    MaxTextureWidth = max_texture_width;
    MaxTextureHeight = max_texture_height;
    MaxTextureObjects = max_texture_objects;
    return 1;
  } else if ((MaxTextureWidth != max_texture_width) || 
	     (MaxTextureHeight != max_texture_height) || 
	     (MaxTextureObjects != max_texture_objects))
    return 0;
  return 1;
}

int texture_width()
{
  return MaxTextureWidth;
}

int texture_height()
{
  return MaxTextureHeight;
}

static int get_new_texobject()
{
  if (initialized) {
    if (top < MaxTextureObjects)
      return tex_names[top++];
    else
      return -1;
  }
  else {
    initialized = 1;
    if ((tex_names = (GLuint *)
	 calloc(MaxTextureObjects, sizeof(GLuint))) == NULL) {
      VSystemError("get_new_texobject:");
      exit(1);
    }
    glGenTextures(MaxTextureObjects, tex_names);
    top = 1;
    return tex_names[top++];
  }
}

Texture::Texture(VImage im)
{
  int to;
  VImage flipped;

  // Get a texture object name 
  if ((to = get_new_texobject()) < 0) {
    VError("Texture::Texture: No more texture objects to allocate\n");
    exit(1);
  }
  tex_object = to;

  max_x = VImageNColumns(im)*1.0/MaxTextureWidth;
  max_y = VImageNRows(im)*1.0/MaxTextureHeight;

  // Convert Vista image to a packed GL RGBA format
  flipped = VFlipImage(im, NULL, VAllBands, 1);
  tex_data = glutils_Vista_to_RGBA(flipped);
  VDestroyImage(flipped);

  // Bind texture name to the 2D texture target
  glBindTexture(GL_TEXTURE_2D, tex_object);
  // Create the texture image
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 
	       MaxTextureWidth, MaxTextureHeight,
	       0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  // Copy image data to texture memory
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 
		  VImageNColumns(im), VImageNRows(im),
		  GL_RGBA, GL_UNSIGNED_BYTE, tex_data);

  // Set texturing parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
  glEnable(GL_TEXTURE_2D);

  // free allocated memory
  free(tex_data);
}

void Texture::refresh(VImage im)
{
  VImage flipped;

  activate();
  // Convert Vista image to a packed GL RGBA format
  flipped = VFlipImage(im, NULL, VAllBands, 1);
  tex_data = glutils_Vista_to_RGBA(flipped);
  VDestroyImage(flipped);

  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 
		  VImageNColumns(im), VImageNRows(im),
		  GL_RGBA, GL_UNSIGNED_BYTE, tex_data);

  max_x = VImageNColumns(im)*1.0/MaxTextureWidth;
  max_y = VImageNRows(im)*1.0/MaxTextureHeight;

  free(tex_data);
}

void Texture::activate()
{
  glBindTexture(GL_TEXTURE_2D, tex_object);
}

void Texture::deactivate()
{
  //glDisable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);
}


void Texture::get_bounds(GLdouble& mx, GLdouble& my)
{
  mx = max_x;
  my = max_y;
}

void Texture::get_dimensions(int& w, int& h)
{
  w = MaxTextureWidth;
  h = MaxTextureHeight;
}

void Texture::set_bounds(int width, int height)
{
  max_x = width*1.0/MaxTextureWidth;
  max_y = height*1.0/MaxTextureHeight;
}
  
