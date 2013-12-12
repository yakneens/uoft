// DO NOT MODIFY THIS FILE!!!!!!
//
// (c) Kyros Kutulakos 06/01/04
//
// This file gives an example of how an image can be displaed in an
// OpenGL panel which is part of an fltk window
//
// The idea behind the image display code below is to display an image
// by drawing a rectangle on the screen whose dimensions are equal to
// the initial size of the OpenGL panel. Rather than giving the
// rectangle a single color, however, we texture-map the Vista image
// to be displayed in that rectangle.
//
// Doing the image display in this way will be useful for a number of
// assignments later on, because it allows us to shrink, distort,
// enlarge, or warp an image by simply changing the size and position
// of the rectancle's 4 vertices.
//
// To better understand the code in this file, you may want to refer
// to the Texture Mapping section of the OpenGL programming guide

#include "stdafx.h"
#include <math.h>
#include <iostream.h>
#include "c-code/imdraw.h"
#include "glutils.h"
#include <float.h>
#include <queue>
#include <vector>
#include "c-code/pqelement.h"
#include <assert.h>
#include "c-code/point.h"
#include "c-code/boundary.h"
#include <ctime>
#include <limits>
#include <sstream>
#include "c-code/line.h"

#include <FL/fl_ask.H>
#define NORTH_WEST 0
#define NORTH 1
#define NORTH_EAST 2
#define WEST 7
#define EAST 3
#define SOUTH_WEST 6
#define SOUTH 5
#define SOUTH_EAST 4

using namespace std;

#define nbrPos(src_ind, dst_ind, dir) \
	switch(dir){ \
		/* North west */ \
		case 0: \
		{ \
			if(floor((double)src_ind / ncols) == 0 || (src_ind % ncols) == 0){ \
				dst_ind = INT_MAX; \
			}else{ \
				dst_ind = src_ind - ncols - 1; \
			}\
			break; \
		} \
		/* North */ \
		case 1: \
		{ \
			if(floor((double)src_ind / ncols) == 0){ \
				dst_ind = INT_MAX; \
			}else{ \
				dst_ind = src_ind - ncols; \
			} \
			break; \
		} \
		/* North east */ \
		case 2: \
		{ \
			if(floor((double)src_ind / ncols) == 0 || (src_ind % ncols) == ncols - 1){ \
				dst_ind = INT_MAX; \
			}else{ \
				dst_ind = src_ind - ncols + 1; \
			} \
			break; \
		} \
		/* east */ \
		case 3: \
		{ \
			if((src_ind % ncols) == ncols - 1){ \
				dst_ind = INT_MAX; \
			}else{ \
				dst_ind = src_ind + 1; \
			} \
			break; \
		} \
		/* south east */ \
		case 4: \
		{ \
			if(floor((double)src_ind / ncols) == nrows - 1 || (src_ind % ncols) == ncols - 1){ \
				dst_ind = INT_MAX; \
			}else{ \
				dst_ind = src_ind + ncols + 1; \
			} \
			break; \
		} \
		/* south */ \
		case 5: \
		{ \
			if(floor((double)src_ind / ncols) == nrows - 1){ \
				dst_ind = INT_MAX; \
			}else{ \
				dst_ind = src_ind + ncols; \
			} \
			break; \
		} \
		/* south west */ \
		case 6: \
		{ \
			if(floor((double)src_ind / ncols) == nrows - 1 || (src_ind % ncols) == 0){ \
				dst_ind = INT_MAX; \
			}else{ \
				dst_ind = src_ind + ncols - 1; \
			} \
			break; \
		} \
		/* west */ \
		case 7: \
		{ \
			if((src_ind % ncols) == 0){ \
				dst_ind = INT_MAX; \
			}else{ \
				dst_ind = src_ind - 1; \
			} \
			break; \
		} \
	}\

#define pVal(ptr) *(ptr + ppos)
#define qVal(ptr) *(ptr + qpos)


// Sets the image being displayed in the window
void ImDraw::set(VImage new_im)
{
     if ((VImageNRows(new_im) < 1) || (VImageNColumns(new_im) < 1)) {
	  return;
     }
     
     // Direct all further OpenGL commands to this OpenGL panel
     // This is NECESSARY if there are more than one OpenGL panels on 
     // the screen!
     make_current();
     if (imTexture == NULL) {
	  // These commands are executed only once, when the variable
	  // containing the image to be displayed points to a valid
	  // Vista image.  The command initializes the OpenGL
	  // texturing operations
	  
	  // Allocate space in texture memory for a single texture
	  // that has a maximum size of 1024x1024
	  
	  // Note that texture dimensions have to be powers of 2 and
	  // that the dimensions of any Vista image to be used as
	  // texture must have dimensions smaller than the texture
	  // dimensions.
	  
	  // You will need to call this routine with different
	  // parameters if you are to display Vista images larger than
	  // 1024x1024 (this is probably large enough and you might
	  // want to use smaller images to conserve memory & disk
	  // space
	  texture_set_globals(1024, 1024, 1);
          imTexture = new Texture(new_im);
	} else
	  imTexture->refresh(new_im);
     
     aspect_ratio = VImageNColumns(new_im)*1.0/VImageNRows(new_im);
     im_height = VImageNRows(new_im);
     im_width = VImageNColumns(new_im);

     // compute the magnification factor of the Vista image

     // define the width and height of the canvas area that will be
     // occupied by the displayed image, in order to preserve the
     // image's aspect ratio (which may be different from the aspect
     // ratio of the canvas window)
     double vh, vw;

     vh = (height < width/aspect_ratio)?height:(width/aspect_ratio);
     vw = vh*aspect_ratio;
     
     // Return the magnification factor of the image being
     // displayed. This is important if we want to get the coordinates
     // of the drawn rectangle to match pixel coordinates of the Vista
     // image being drawn
     magnification = vw/im_width;
     
     // Redraw the entire window to show the new image
     Fl_Window::redraw();
}
	
// Converts GL canvas coordinates to Vista coordinates. Vista
// coordinates are integers so that they can be used as indices into a
// Vista image array.  The function returns 1 if the GL coordinates
// corresponds to a point within the image boundary
int ImDraw::gl2vista(double glx, double gly, int& r, int& c)
{
     r = (int) rint(gly/magnification);
     c = (int) rint(glx/magnification);

     if ((r >= 0) && (r < im_height) && (c >= 0) && (c < im_width))
	  return 1;
     else 
	  return 0;
}

// Converts Vista coordinates to GL canvas coordinates
int ImDraw::vista2gl(int r, int c, double& glx, double& gly)
{
     gly = r*magnification;
     glx = c*magnification;

     if ((gly >= 0) && (gly < height) || (glx >= 0) && (glx < width))
	  return 1;
     else 
	  return 0;
}

// Displays a Vista image in a GL panel.  The image is displayed by
// texture-mapping it onto a polygon whose size is equal to the size
// of the GL panel
void ImDraw::draw_image()
{
     double tex_width, tex_height;
     double w, h;
     
     // texture-map polygons with the current texture
     if (imTexture != NULL) {
	  // Record the texture coordinates of the top-right corner of
	  // the image
	  imTexture->get_bounds(tex_width, tex_height);
	  // Enable GL texture mapping
	  imTexture->activate();
     }
  
     // define the width and height of the canvas area that will be
     // occupied by the displayed image, in order to preserve the
     // image's aspect ratio (which may be different from the aspect
     // ratio of the canvas window)
     h = (height < width/aspect_ratio)?height:(width/aspect_ratio);
     w = h*aspect_ratio;
  
     // The following statements are what actually display the image
     // in the panel.  The statements draw a rectangle defined by
     // (0,0) and (w, h); this rectangle is texture-mapped with a
     // rectangular image whose texture coordinates are defined by
     // (0,0) and (tex_width, tex_height)
 
     glColor3d(0.0, 0.0, 0.0);
     glBegin(GL_POLYGON);
     {
	  glTexCoord2d(0,0);
	  glVertex2d(0,h);
	  
	  glTexCoord2d(0,tex_height);
	  glVertex2d(0,0);
	  
	  glTexCoord2d(tex_width, tex_height);
	  glVertex2d(w, 0);
	  
	  glTexCoord2d(tex_width,0);
	  glVertex2d(w, h);
     }
     glEnd();
     glFlush();
     
     // Disable texture mapping for the OpenGL commands that follow
     if (imTexture != NULL) {
	  imTexture->deactivate();
     }
}


// Routine that intercepts window resize operations performed by the user
void ImDraw::resize(int x, int y, int w, int h)
{
     
     // the window has been resized so we need to update the width & height 
     // information 
     width = w;
     height = h;

     // Need to call this method ensure that the GL panel "knows" that
     // it has been resized as well...
     Fl_Gl_Window::resize(x, y, w, h);

     // determine the new magnification of the Vista image

     // define the width and height of the canvas area that will be
     // occupied by the displayed image, in order to preserve the
     // image's aspect ratio (which may be different from the aspect
     // ratio of the canvas window)
     double vh, vw;
     vh = (height < width/aspect_ratio)?height:(width/aspect_ratio);
     vw = vh*aspect_ratio;
     
     // Return the magnification factor of the image being
     // displayed. This is important if we want to get the coordinates
     // of the drawn rectangle to match pixel coordinates of the Vista
     // image being drawn
     magnification = vw/im_width;
}
VImage ImDraw::rgbToGrayscale(VImage img){
	/* Convert to grayscale. */	
	if(VImageNBands(img) == 3){
		VImage myImage = VCreateImage(1, VImageNRows(img), VImageNColumns(img),VUByteRepn);

		for(int i = 0; i < VImageNRows(img); i++){
			for(int j = 0; j < VImageNColumns(img); j++){
				
				VPixel(myImage, 0, i, j, VUByte) = 
					floor((double)((VPixel(img, 0, i, j, VUByte) + 
					 VPixel(img, 1, i, j, VUByte)  + 
					 VPixel(img, 2, i, j, VUByte)) / 3));

			}
		}
	return myImage;
	}else{
		return img;
	}
}

VImage ImDraw::zeroCrossings(VImage img, int sigma){

	double sigma2 = sigma / sqrt((double)(2)), sigma3 = sqrt((double)(2)) * sigma;
	double maxRange = -128, minRange = 127;
			
	if(sigma2 < 1){ sigma2 = 1;}
	if(sigma3 < 1){ sigma3 = 1;}

	
	/* Convert image to signed byte representation. */
	//VImage imgSigned = VConvertImageRange(img, NULL, VAllBands, VSByteRepn);
	VImage imgSigned = VConvertImageLinear(img, NULL, VAllBands, VSByteRepn, 1, -128);
	
	
	/* Compute two gaussian smoothed versions of the image */
	VImage gaussSmoothImg = 
		VGaussianConvolveImage(imgSigned, NULL, VAllBands, sigma2, ceil(6 * sigma2), VFilterGaussian);
	VImage gaussSmoothImg2 = 
		VGaussianConvolveImage(imgSigned, NULL, VAllBands, sigma3, ceil(6 * sigma3), VFilterGaussian);
	
	VImage laplImg = VImageOpI(gaussSmoothImg, NULL, VAllBands, VImageOpSub, gaussSmoothImg2, VAllBands, &maxRange, &minRange);

	/* Find the zero crossings in resulting image. */
	VImage zeroCrossImg = VZeroCrossings(laplImg, NULL, VAllBands);


	/* Convert image back to unsigned byte representation. */
	//VImage zeroCrossUnsigned = VConvertImageRange(zeroCrossImg, NULL, VAllBands, VUByteRepn);
	VImage zeroCrossUnsigned = VConvertImageLinear(zeroCrossImg, NULL, VAllBands, VUByteRepn, 1, 128);
	
	VDestroyImage(imgSigned);
	VDestroyImage(gaussSmoothImg);
	VDestroyImage(gaussSmoothImg2);
	VDestroyImage(laplImg);
	VDestroyImage(zeroCrossImg);

	return zeroCrossUnsigned;

}

VImage ImDraw::gradient(VImage img){
	
	/* Compute image gradient. */
	VImage gradImage = VImageGradient(img, NULL, VAllBands);
	
	
	return gradImage;
}

VImage ImDraw::gradientMagnitude(VImage img){
	
	
	/* Compute magnitudes of the image gradient. */
	VImage gradMagImage = VImageMagnitude(img, NULL, VAllBands);
	return gradMagImage;
}

VDouble ImDraw::gradMagMax(VImage img){
	
	VDouble magMax = 0;

	/* Compute maximum gradient magnitude in the image. */
	if(!VImageStats(img, VAllBands, NULL, &magMax, NULL, NULL)){
		exit(1);
	}

	return magMax;
}

void ImDraw::setDisplayState(int state){
	this->displayState = state;


}

int ImDraw::getDisplayState(){
	return this->displayState;
}


void ImDraw::pathFinderReInit(){
	VDouble *totalCost, *ancest;
	VBit *valid, *active, *processed;
	int npixels;
	if(!VSelectBand("Link Weights", nodeImage, 0, &npixels, (VPointer*)&totalCost))
			VError("Could not get band %d of gradient image", 0);
	if(!VSelectBand("Link Weights", nodeImage, 1, &npixels, (VPointer*)&ancest))
			VError("Could not get band %d of gradient image", 1);
	if(!VSelectBand("Link Weights", nodeInfoImage, 0, &npixels, (VPointer*)&valid))
			VError("Could not get band %d of gradient image", 0);
	if(!VSelectBand("Link Weights", nodeInfoImage, 1, &npixels, (VPointer*)&active))
			VError("Could not get band %d of gradient image", 1);
	if(!VSelectBand("Link Weights", nodeInfoImage, 2, &npixels, (VPointer*)&processed))
			VError("Could not get band %d of gradient image", 2);

	/* Reinitialize image nodes. */
	for(int i = 0; i < npixels; i++){
		*(totalCost++) = DBL_MAX;
		*(ancest++) = 0;
		*(valid++) = 1;
		*(active++) = 0;
		*(processed++) = 0;
	}
}

/* Computes link costs in im. */
void ImDraw::computeLinkCosts(VImage im){
	clock_t start = clock();

	/* Convert image to grayscale. */	
	VImage grayImg = rgbToGrayscale(im);
	
	//set(grayImg);
	this->grayImage = grayImg;
	/* Compute zero-crossings of the second derivative. */
	VImage zeroCross = zeroCrossings(grayImg, 1);
	this->zeroImage = zeroCross;
	
	VImage gradientImage = gradient(grayImg);
	

	/* Compute the image gradient magnitude. */
	VImage gradMagImage = gradientMagnitude(gradientImage);
	this->gradImage = gradMagImage;
	
	/* Compute the maximum gradient magnitude in the image. */
	VDouble maxMag = gradMagMax(gradMagImage);

	/* Number of rows in current image. */
	int nrows = VImageNRows(im);

	/* Number of columns in current image. */
	int ncols = VImageNColumns(im);

	int npixels, nzpixels;
	VPointer p;
	VSByte *gx, *gy, *gm;
	VUByte *z;
	VBit *active, *processed, *valid;
	VDouble  *totalCost, *ancest, *weightPtr[8];
	int qpos, xpos, ypos;
	
	/* Create image to store link weights. */
	VImage linkWeights = VCreateImage(8, nrows, ncols, VDoubleRepn);

	/* Create image to store link weights. */
	VImage nodes = VCreateImage(2, nrows, ncols, VDoubleRepn);

	VImage nodeInfo = VCreateImage(3, nrows, ncols, VBitRepn);

	/* Get pointers to image pixels for link weight image. */
	for(int w = 0; w < 8; w++){
		if(!VSelectBand("Link Weights", linkWeights, w, &npixels, (VPointer*)&weightPtr[w]))
			VError("Could not get band %d of link cost image", w);
	}
	if(!VSelectBand("Link Weights", nodes, 0, &npixels, (VPointer*)&totalCost))
		VError("Could not get band %d of node image", 0);

	if(!VSelectBand("Link Weights", nodes, 1, &npixels, (VPointer*)&ancest))
		VError("Could not get band %d of node image", 1);

	if(!VSelectBand("Link Weights", nodeInfo, 0, &npixels, (VPointer*)&valid))
		VError("Could not get band %d of nodeInfo image", 0);

	if(!VSelectBand("Link Weights", nodeInfo, 1, &npixels, (VPointer*)&active))
		VError("Could not get band %d of nodeInfo image", 1);

	if(!VSelectBand("Link Weights", nodeInfo, 2, &npixels, (VPointer*)&processed))
		VError("Could not get band %d of nodeInfo image", 2);

	double dpNorm[2], dqNorm[2], lpq[2], qToP[2], dotPr, dSubP, dSubQ;
	double gradDirTerm, zeroCrossTerm = 1, gradMagTerm, scalingFactor = 1;
	
	/* Get pointer to the pixels in frist band of gradient image. */
	if(!VSelectBand("Gradient X", gradientImage, 0, &npixels, (VPointer*)&gx))
		VError("Could not get band 0 of gradient image");
	/* Get pointer to the pixels in second band of gradient image. */
	if(!VSelectBand("Gradient Y", gradientImage, 1, &npixels, (VPointer*)&gy))
		VError("Could not get band 1 of gradient image");
	/* Get pointer to pixels in the zero crossing image. */
	if(!VSelectBand("Zero Crossing", zeroCross, 0, &nzpixels, (VPointer*)&z))
		VError("Could not get band 0 of zero crossing image");
	/* Get pointer to pixels in the gradient magnitude image. */
	if(!VSelectBand("Gradient Magnitude", gradMagImage, 0, &npixels, (VPointer*)&gm))
		VError("Could not get band 0 of gradient magnitude image");
	
	/* For each pixel in the image. */
	for(int ppos = 0; ppos < npixels; ppos++){
		
		/* Determine x and y coordinates of the pixel. */
		xpos = ppos % ncols;
		ypos = (int)floor((double)(ppos / ncols));
		
		
		/* Normalize the gradient and obtain a vector orthogonal to it. */
		dpNorm[0] = pVal(gm) > 0 ? (pVal(gx) / pVal(gm)) : 0;
		dpNorm[1] = pVal(gm) > 0 ? -1 * (pVal(gy) / pVal(gm)) : 0;
		
		/* For each neighbour of current pixel. */
		for(int j = 0; j < 8; j++){
			/* Set pointer to first pixel in gradient magnitude image. */
			
			/* Initialize scaling factor for gradient magnitude. */
			scalingFactor = 1;

			/* Obtain pointers to gradient pixels, and gradient magnitude
			   of current neighbour. Obtain pixel position of current neighbour. */
			nbrPos(ppos, qpos, j);
			
			/* If neighbour is within image boundary. */
			if(qpos != INT_MAX){
				
				int xposq = (qpos % ncols);
				int yposq = floor((double)(qpos / ncols));
				
				/* Calculate vector between this pixel and current neighbour. */
				qToP[0] = xposq - xpos;
				qToP[1] = yposq - ypos;

				/* Normalize the gradient for current neighbour
				   and obtain a vector orthogonal to it. */
				dqNorm[0] = qVal(gm) > 0 ? (qVal(gx) / qVal(gm)) : 0;
				dqNorm[1] = qVal(gm) > 0 ? -1 * (qVal(gy) / qVal(gm)) : 0;
				
				/* <D(p) | q - p> */
				dotPr = dpNorm[0] * qToP[0] + dpNorm[1] * qToP[1];
				
				/* Compute biderectional link between current pixel and
				   current neighbour. */
				if(dotPr >= 0){
					lpq[0] = qToP[0];
					lpq[1] = qToP[1];
					dSubP = dotPr;
				}else{
					lpq[0] = -1 * qToP[0];
					lpq[1] = -1 * qToP[1];

					dSubP = dpNorm[0] * lpq[0] + dpNorm[1] * lpq[1];
				}
				
				/* <D(q) | L(p,q)> */
				dSubQ = abs(dqNorm[0] * lpq[0] + dqNorm[1] * lpq[1]);
			
				/* Calculate the gradient direction term of the link weight. */
				gradDirTerm = (double)((2 * (acos(dSubP) + acos(dSubQ))) / (3 * PI));
#ifdef WIN32
				if(_isnan(gradDirTerm)){
#else
				if(isnan(gradDirTerm)){
#endif
					gradDirTerm = 0.5;
				}

				assert(gradDirTerm >= 0 && gradDirTerm <= 1);

				if(xposq > 0 && yposq > 0){
					int zeroCorssIndex = (yposq - 1) * im_width + xposq - 1;
					
					/* Calculate the zero crossing term of the link weight. */
					if(*(z + zeroCorssIndex) > 0){
						zeroCrossTerm = 0;
					}
				}

			//	assert(zeroCrossTerm >= 0 && zeroCrossTerm <= 1);

				/* Update scaling factor depending on whether the neighbour is diagonal. */
				if((j % 2) != 0){
					scalingFactor /= sqrt((double)2);
				}

				/* Compute the gradient magnitude term of the link weight. */
				gradMagTerm = scalingFactor * (1 - (qVal(gm) / maxMag));
				
			//	assert(gradMagTerm >= 0 && gradMagTerm <= 1);
				
				double linkWeight = 0.43 * zeroCrossTerm + 0.00 * gradDirTerm + 0.43 * gradMagTerm;
			//	assert(linkWeight >= 0 && linkWeight <= 1);
					
					/* Set the link weight to the computed value and advance one pixel. */
				pVal(weightPtr[j]) = linkWeight;
					
					
			}
		}
		
		/* Advance pointers by one pixel. */
		*(totalCost++) = DBL_MAX;
		*(ancest++) = 0;
		*(valid++) = 1;
		*(active++) = 0;
		*(processed++) = 0;
	}

	this->weightImage = linkWeights;
	this->nodeImage = nodes;
	this->nodeInfoImage = nodeInfo;
	
	cout << "Link Weight time: " << clock() - start  << " clock ticks \n";
	cout.flush();

	
}

void ImDraw::computePathCosts(int seedX, int seedY){
	clock_t start = clock();
	int npixels, pixelPos = this->im_width * seedY + seedX;
	VDouble *weightPtr[8], *totalCost, *ancest;
	VBit *active, *processed, *valid;
	VDouble *totalCostInit, *ancestInit, *activeInit, *processedInit, *validInit;
	int nrows = this->im_height, ncols = this->im_width, nbrPixelPos;

	/* Get pointers to image pixels for link weight image. */
	for(int w = 0; w < 8; w++){
		if(!VSelectBand("Link Weights", weightImage, w, &npixels, (VPointer*)&weightPtr[w]))
			VError("Could not get band %d of gradient image", w);
		
	}
	
	if(!VSelectBand("Link Weights", nodeImage, 0, &npixels, (VPointer*)&totalCost))
			VError("Could not get band %d of gradient image", 0);
	/*totalCostInit = totalCost;
	totalCost += pixelPos;*/

	if(!VSelectBand("Link Weights", nodeImage, 1, &npixels, (VPointer*)&ancest))
			VError("Could not get band %d of gradient image", 1);
	/*ancestInit = ancest;
	ancest += pixelPos;*/

	if(!VSelectBand("Link Weights", nodeInfoImage, 0, &npixels, (VPointer*)&valid))
			VError("Could not get band %d of gradient image", 0);
	/*validInit = valid;
	valid += pixelPos;*/

	if(!VSelectBand("Link Weights", nodeInfoImage, 1, &npixels, (VPointer*)&active))
			VError("Could not get band %d of gradient image", 1);
	/*activeInit = active;
	active += pixelPos;*/

	if(!VSelectBand("Link Weights", nodeInfoImage, 2, &npixels, (VPointer*)&processed))
			VError("Could not get band %d of gradient image", 2);
	
	/* New active list. */
	priority_queue<PQElement> activeList;
	
	/* Set the active flag on this pixel's node. */
	*(active + pixelPos) = 1;
	*(totalCost + pixelPos) = 0;

	/* Create a new queue element containing a pointer to this pixel
	   with priority set to this pixel's cost. */
	PQElement topElem(pixelPos, 0);
	
	/* Insert new element into the queue. */
	activeList.push(topElem);

	
	/* Process all elements in the active list. */
	while(!activeList.empty()){
		
		/* Retreive the node pointer contained in top element. */
		pixelPos = activeList.top().getPixelIndex();
			
		/* If top element is valid. */
		if(*(valid + pixelPos)){

			
			*(valid + pixelPos) = 0; 


			/* Pop the top element off the queue. */
			activeList.pop();

			/* Set this node as processed. */			
			*(processed + pixelPos) = 1;
			*(active + pixelPos) = 0;
			
			/* Go through all the neighbours. */
			for(int i = 0; i < MAX_NEIGHBOURS; i++){

				/* Current neighbour. */
				nbrPos(pixelPos, nbrPixelPos, i);
				
				/* If neighbour represents a valid pixel. */
				if(nbrPixelPos != INT_MAX){
					/* If neighbour is not yet processed. */
					if(!*(processed + nbrPixelPos)){
						double tempCost;

						if(*(totalCost + pixelPos) == DBL_MAX){
							tempCost = *(weightPtr[i] + pixelPos);
						}else{
							/* Compute current estimated minimal cost from seed to neighbour. */
							tempCost = *(totalCost + pixelPos) + *(weightPtr[i] + pixelPos);
						}
						
						/* If neighbour already on active list and the new cost
						   is lower than the previously computed cost. */
						if(*(active + nbrPixelPos) && tempCost < *(totalCost + nbrPixelPos)){
							
							*(active + nbrPixelPos) = 0;
						}
						
						/* If neighbour is not on the active list. */
						if(!*(active + nbrPixelPos)){

							/* Flag neighbour as being on the active list. */
							*(active + nbrPixelPos) = 1;

							/* Set the cost to the new estimated cost. */
							*(totalCost + nbrPixelPos) = tempCost;
							
							/* Set the node's ancestor. */
							*(ancest + nbrPixelPos) = pixelPos;
							
							/* Create a new queue element containing a pointer to 
							   neighbour with cost tempCost. */
							PQElement newElem(nbrPixelPos, tempCost);

							/* Insert the new element into the queue. */
							activeList.push(newElem);
						}
					}
				}
			}
		/* The element is not valid so simply remove it from the queue. */
		}else{
			activeList.pop();
		}
	}
	
	cout << "Path cost time: " << clock() - start  << " clock ticks \n";
	cout.flush();
}


int ImDraw::getImageWidth(){
	return this->im_width;
}

int ImDraw::getImageHeight(){
	return this->im_height;
}

/* Create an image containing boundary. */
VImage ImDraw::getBoundary(ImagePrimitive *boundary){
	
	Boundary *myBoundary = (Boundary*)boundary;
	
	/* Band 1 is used to store pixel values. Band 2 is used to store information about
	   pixels to be used in computing the alpha matte. */
	VImage boundaryImg = VCreateImage(2, this->im_height, this->im_width, VUByteRepn);
	
	std::deque<Point>::iterator it;
	
	Point* pt;
	
	bool flag = true;

	/* Zero in the image. */
	VFillImage(boundaryImg, VAllBands, 0);
	
	Point prev = *(myBoundary->getVerticesEnd()), next = *(myBoundary->getVerticesBegin() + 1);
	
	/* Go through all the vertices on the boundary. */
	for(it = myBoundary->getVerticesBegin(); it < myBoundary->getVerticesEnd(); it++){
		Point thisPixel = *it;
		
		/* Set the corresponding pixel in the image to intensity 255. */
		VPixel(boundaryImg, 0, thisPixel.getY(), thisPixel.getX(), VUByte) = 255;
		
		/* Look at the previous and the next vertex. */
		if((it + 1) != myBoundary->getVerticesEnd()){
			next = (*it);
			//it--;
		}else{
			next = *(myBoundary->getVerticesBegin());
		}
		
		/* In this case we should not flip the parity bit when scan converting the alpha
		   matte. */
		if((prev.getY() < thisPixel.getY() && 
		    prev.getX() < thisPixel.getX() && 
		    next.getY() < thisPixel.getY() &&
		    next.getX() > thisPixel.getX())){
			VPixel(boundaryImg, 1, thisPixel.getY(), thisPixel.getX(), VUByte) = 1;
		}else if(prev.getY() > thisPixel.getY() &&
		    prev.getX() < thisPixel.getX() &&
			next.getY() > thisPixel.getY() &&
			next.getX() > thisPixel.getX()){
			VPixel(boundaryImg, 1, thisPixel.getY(), thisPixel.getX(), VUByte) = 2;
		}else if((prev.getX() < thisPixel.getX() &&
			  prev.getY() == thisPixel.getY() &&
			  next.getX() > thisPixel.getX() &&
			  next.getY() == thisPixel.getY())){
			VPixel(boundaryImg, 1, thisPixel.getY(), thisPixel.getX(), VUByte) = 3;
		}else{
			VPixel(boundaryImg, 1, thisPixel.getY(), thisPixel.getX(), VUByte) = 4;
		}
	}
	return boundaryImg;
}

/* Computes the alpha matte based on the boundary. */
VImage ImDraw::getMatte(VImage boundary){
	
	int parity = 0;
	VImage matte = VCreateImage(1, this->im_height, this->im_width, VUByteRepn);
	VUByte flag;

	for(int i = 0; i < this->im_height; i++){
		parity = 0;
		for(int j = 0; j < this->im_width; j++){
			int val = VPixel(boundary, 0, i, j, VUByte);
			flag = VPixel(boundary, 1, i, j, VUByte);
			/* flag = 1 - max
			   flag = 2 - min
			   flag = 3 - horizontal edge
			   flag = 4 - other */
			if(val == 255 && parity == 0){
				if(flag == 4){
					parity = 1;
				}else if(flag == 3){
					cout << "3\n";
				}else if(flag == 2){
					cout << "2\n";
				}else if(flag == 1){
					cout << "1\n";
				}
				cout.flush();
				VPixel(matte, 0, i, j, VUByte) = 0;

			}else if(val == 255 && parity == 1){
				if(flag == 4){
					parity = 0;
				}if(flag == 3){
					cout << "3\n";
				}else if(flag == 2){
					cout << "2\n";
				}else if(flag == 1){
					cout << "1\n";
				}
				cout.flush();

				VPixel(matte, 0, i, j, VUByte) = 0;

			}else if(val == 0 && parity == 0){

			}else if(val == 0 && parity == 1){
				VPixel(matte, 0, i, j, VUByte) = 255;
				if(flag == 3){
					cout << "3\n";
				}else if(flag == 2){
					cout << "2\n";
				}else if(flag == 1){
					cout << "1\n";
				}
				cout.flush();
			}
		}

	}
	

	return matte;
}

/* Return string representation of the boundary vertices */
string ImDraw::writeBoundaryVertices(){
	ImagePrimitive *tempObj;
	
	for(int i = 0; i < this->sceneObjects.size(); i++){
		tempObj = sceneObjects[i];
		if(tempObj->getType() == ImagePrimitive::boundary){
			Boundary *myBoundary = (Boundary*)tempObj;
			return myBoundary->verticesToString();
		}
	}

	return "";
}

/* Return string representation of the boundary seed points */
string ImDraw::writeBoundarySeedPoints(){
	ImagePrimitive *tempObj;
	
	for(int i = 0; i < this->sceneObjects.size(); i++){
		tempObj = sceneObjects[i];
		if(tempObj->getType() == ImagePrimitive::boundary){
			Boundary *myBoundary = (Boundary*)tempObj;
			return myBoundary->seedPointsToString();
		}
	}

	return "";
}

/* Write alpha matte to file */
void ImDraw::writeMatteToFile(char* filename){
	Boundary *myBoundary;
	ImagePrimitive *tempObj;
	bool found = false;

	for(int i = 0; i < this->sceneObjects.size(); i++){
		tempObj = sceneObjects[i];
		if(tempObj->getType() == ImagePrimitive::boundary){
			myBoundary = (Boundary*)tempObj;
			found = true;
			break;
		}
	}

	if(found){
		VImage boundaryImage = this->getBoundary(myBoundary);
		VImage matte = this->getMatte(boundaryImage);

		FILE *fp;
	
		/* If a file has been successfully chosen, write the matte
		to the file. */
#ifdef WIN32
	fp = fopen(filename, "wb");
#else
	fp = fopen(filename, "w");
#endif
		VWriteImages(fp, NULL, 1, &matte);
		VDestroyImage(matte);
		fclose(fp);
	}
}

/* Write gradient image to file */
void ImDraw::writeGradToFile(char *filename){
		FILE *fp;
	
		/* If a file has been successfully chosen, write the matte
		to the file. */

#ifdef WIN32
	fp = fopen(filename, "wb");
#else
	fp = fopen(filename, "w");
#endif
		
		if(!VWriteImages(fp, NULL, 1, &this->gradImage)){
			exit(1);
		}
		fclose(fp);
	
}

/* Write zero crossing image to file */
void ImDraw::writeZeroCrossToFile(char *filename){
		FILE *fp;
	
		/* If a file has been successfully chosen, write the matte
		to the file. */

#ifdef WIN32
	fp = fopen(filename, "wb");
#else
	fp = fopen(filename, "w");
#endif
		VWriteImages(fp, NULL, 1, &this->zeroImage);
		fclose(fp);
}

/* Write link cost image to file */
void ImDraw::writeLinkCostsToFile(char *filename){
		FILE *fp;
	
		/* If a file has been successfully chosen, write the matte
		to the file. */

#ifdef WIN32
	fp = fopen(filename, "wb");
#else
	fp = fopen(filename, "w");
#endif		
		
		VWriteImages(fp, NULL, 1, &this->weightImage);
		fclose(fp);
}

void ImDraw::gl2v_matrices(Matrix& gl2v, Matrix& v2gl){
	// matrix mapping 2D OpenGL coordinates to 2D Vista coordianates
	gl2v.ReSize(2,2);
	gl2v(1,1) = 0;
	gl2v(1,2) = 1/magnification;
	gl2v(2,1) = 1/magnification;
	gl2v(2,2) = 0;

	// matrix mapping 2D OpenGL coordinates to 2D Vista coordianates
	v2gl.ReSize(2,2);
	v2gl(1,1) = 0;
	v2gl(1,2) = magnification;
	v2gl(2,1) = magnification;
	v2gl(2,2) = 0;
}


void ImDraw::setSlider(Fl_Value_Slider *slider){
	this->splineLOD = slider;
}

void ImDraw::deleteSpline(){
	this->bspline->delete_knots();
}
void ImDraw::addSplineKnot(int x, int y){
	this->bspline->add_knot(x, y);
}
void ImDraw::completeSpline(){
	Matrix m1;
	int k, j, d;
	this->bspline->lock_endpoints();
	bspline->get_matrix(m1, k, j, d);
	this->resolutionImages.push_back(m1);
	setSlider();

}

void ImDraw::convertPointsToSpline(){
	ImagePrimitive* obj;
	bspline->delete_knots();
	Matrix m1;
	int k, j, d;

	while(sceneObjects.size() > 0){
		obj = sceneObjects.front();
		sceneObjects.pop_front();

		/* If the most recent object is a polygon. */
		if(obj->getType() == ImagePrimitive::point){
			Point* myPoint = (Point*)obj;
			bspline->add_knot(myPoint->getX(), myPoint->getY());
		}else{
			fl_alert("Scene can only contain points to convert! Creating empty spline");
			bspline->delete_knots();
			return;
		}
	}

	bspline->lock_endpoints();
	bspline->get_matrix(m1, k, j, d);
	this->resolutionImages.push_back(m1);
	setSlider();
}

/* Set slider attributes. */
void ImDraw::setSlider(){
	Matrix mat;
	int bk, bj, bd;

	bspline->get_matrix(mat, bk, bj, bd);
	//this->splineLOD->activate();
	
	/* Map slider minimum to current resolution level. */
	int sliderMin = bj;
	this->splineLOD->minimum(sliderMin);
	this->splineLOD->value(sliderMin);
	//this->splineLOD->step((double)sliderMin / 100);
	this->lastLOD = this->splineLOD->minimum();
}

/* Reduce spline resolution to given level. */
void ImDraw::reduceSplineTo(int level){
	Matrix mat;
	int bk, bj, bd;

	bspline->get_matrix(mat, bk, bj, bd);

	/* Reduce level by 1 until desired level is reached. */
	if(bj >= level){
		while(bj != level){
			setLevelOfDetail(bj - 1);
			bspline->get_matrix(mat, bk, bj, bd);
		}
	}
}

/* Create a file named filename containing the spline. */
void ImDraw::writeSplineToFile(char *filename){
	FILE *fp;

	/* If a file has been successfully chosen, write the matte
	to the file. */

#ifdef WIN32
	fp = fopen(filename, "wb");
#else
	fp = fopen(filename, "w");
#endif		
	
	Matrix mat;
	int bk, bj, bd;
	
	bspline->get_matrix(mat, bk, bj, bd);
	
	std::stringstream ss;
	ss << "p";
	for(int i = 1; i <= mat.nrows(); i++){
		ss << "P " << mat(i,1) << " " << mat(i, 2) << "\n";
	}

	fputs(ss.str().c_str(), fp);
	fflush(fp);
	fclose(fp);
}

/********************************************************************************
 * function prepMorph() - reads in source and destination images for morph, as 
 * well as source and destination line sets. Generates frames number of morph
 * frames by calling morph() with different values of warpfraction.
 *******************************************************************************/
void ImDraw::prepMorph(char* sourceImageName, char* destImageName, char* sourceCommandsName,
					   char* destCommandsName, char* morphBaseName, int frames){
	FILE *cmdfile;
	char cmd;
	
	/* Deques to hold source and destination control lines. */
	deque<ImagePrimitive*> srcLines;
	deque<ImagePrimitive*> dstLines;
		
	/* Control line counter. */
	int lineCounter = 0;
	
	/* Read in the source line file. */
	if((cmdfile = fopen(sourceCommandsName, "r")) == NULL){
		cerr << ": Error opening file " 
			<< sourceCommandsName << "\n";
	}else{
		while ((cmd = fgetc(cmdfile)) != EOF) {
			switch (cmd) {
				
				/* Reading a line. */
				case 'l':
				{
					int x1,y1, x2, y2;
					
					/* Read in coordinates of the line and insert into source queue. */
					fscanf(cmdfile, "%d %d %d %d", &x1, &y1, &x2, &y2);
					srcLines.push_back(new Line(x1, y1, x2, y2, this));
					lineCounter++;
					break;
					
				}
	
			}
		}
	
	}

	/* Read in the destination line file. */
	if((cmdfile = fopen(destCommandsName, "r")) == NULL){
		cerr << ": Error opening file " 
			<< destCommandsName << "\n";
	}else{
		while ((cmd = fgetc(cmdfile)) != EOF) {
			switch (cmd) {
				
				/* Reading a line. */
				case 'l':
				{
					int x1,y1, x2, y2;
					
					/* Read in coordinates of the line and insert into destination queue. */
					fscanf(cmdfile, "%d %d %d %d", &x1, &y1, &x2, &y2);
					dstLines.push_back(new Line(x1, y1, x2, y2, this));
					lineCounter--;
					break;
					
				}
	
			}
		}
	
	}

	/* Make sure the number of source and destination lines balances. */
	if(lineCounter != 0){
		cout << "Source and destination must have the same number of lines.";
		cout.flush();
		exit(1);
	}

	VAttrList srcAtrr, dstAttr;
	VImage *srcIm, *dstIm;
	FILE *fd;
	VUByte *src, *dst;
	
	/* Read source image into memory. */
	fd = fopen(sourceImageName, "rb");
	if(fd){
		VReadImages(fd, &srcAtrr, &srcIm);
	}

	/* Red destination image into memory. */
	fd = fopen(destImageName, "rb");
	
	if(fd){
		VReadImages(fd, &dstAttr, &dstIm);
	}

	double warpfraction;
	int i = 0;

	/* Allocate memory for filenames. */
	char *frameFileName = (char*)malloc((strlen(morphBaseName) + 6) * sizeof(char));
	
	/* Loop throught the frames. */
	for(i = 0; i < frames; i++){
		strcpy(frameFileName, morphBaseName);
		
		/* Generate filename. */
		/* If frame number less than 10 prepend frame number with a 0*/
		if(i == 0){
			strcat(frameFileName, ".00.v");
		}	else if(i < 10){
			strcat(frameFileName, ".0");
			char frame;
			sprintf(&frame, "%d", i);
			strcat(frameFileName, &frame);
			strcat(frameFileName, ".v");
		}else{
			strcat(frameFileName, ".");
			char frame[2];
			sprintf(frame, "%d", i);
			strcat(frameFileName, frame);
			strcat(frameFileName, ".v");
		}

		cout << "Creating " << frameFileName << "\n";
		cout.flush();
		
		/* Calculate warpfraction.*/
		warpfraction = (double)i / (frames - 1);
		
		/* Morph source to destination. */
		VImage im1 = morph(*srcIm, *dstIm, srcLines, dstLines, warpfraction);
		
		/* Morph destination to source. */
		VImage im2 = morph(*dstIm, *srcIm, dstLines, srcLines, 1 - warpfraction);
		
		/* Blend the two morphs. */
		blend(im1, im2, warpfraction, frameFileName);
	}
	
}

/* Linear interpolation between a and b according to parameter t */
#define interp(a, b, t) floor(((a * (1 - t)) + (b * t)))

/* Number of elements in lineData array */
#define NUM_ELS 14

/* Legend of indices into lineData array */
#define P_PRIME_X 0
#define P_PRIME_Y 1
#define Q_PRIME_X 2
#define Q_PRIME_Y 3
#define P_X 4
#define P_Y 5
#define Q_X 6
#define Q_Y 7
#define PQ_X 8
#define PQ_Y 9
#define PQ_MAG 10
#define PQ_PRIME_X 11
#define PQ_PRIME_Y 12
#define PQ_PRIME_MAG 13

/* Macro for convenient access into lineData array */
#define dataVal(where) *(lineData + ((NUM_ELS * ind) + where))

/********************************************************************************
 * function morph() - morphs between images srcIm and dstIm according to 
 * warpfraction and a set of control lines srcLines and dstLines. Returns
 * result of morph.
 *******************************************************************************/
VImage ImDraw::morph(VImage srcIm, VImage dstIm, 
deque<ImagePrimitive*> srcLines, deque<ImagePrimitive*> dstLines, 
double warpfraction){
	clock_t start = clock();
	
	/* Perform gaussian smoothing on source image. */
	VImage imgSigned = VConvertImageLinear(srcIm, NULL, VAllBands, VSByteRepn, 1, -128);
	VImage smoothed = VGaussianConvolveImage(imgSigned, NULL, VAllBands, 1, 6, VFilterGaussian);
	VImage filtered = VConvertImageLinear(smoothed, NULL, VAllBands, VUByteRepn, 1, 128);
	
	/* Result of morph. */
	VImage current = VCopyImage(dstIm, NULL, VAllBands);

	
	VUByte *src_b1, *src_b2, *src_b3;
	VUByte *dst_b1, *dst_b2, *dst_b3;
	VUByte *cur_b1, *cur_b2, *cur_b3;
	
	/* Select the bands of the source image. */
	if(!VSelectBand("Image", filtered, 0, NULL, (VPointer*)&src_b1))
		VError("Could not get band %d of source image", 0);

	if(!VSelectBand("Image", filtered, 1, NULL, (VPointer*)&src_b2))
		VError("Could not get band %d of source image", 0);

	if(!VSelectBand("Image", filtered, 2, NULL, (VPointer*)&src_b3))
		VError("Could not get band %d of source image", 0);

	/* Select the bands of the destination image. */
	if(!VSelectBand("Image", dstIm, 0, NULL, (VPointer*)&dst_b1))
		VError("Could not get band %d of destination image", 0);

	if(!VSelectBand("Image", dstIm, 1, NULL, (VPointer*)&dst_b2))
		VError("Could not get band %d of destination image", 0);

	if(!VSelectBand("Image", dstIm, 2, NULL, (VPointer*)&dst_b3))
		VError("Could not get band %d of destination image", 0);
	
	
	/* Select the bands of the result image. */
	if(!VSelectBand("Image", current, 0, NULL, (VPointer*)&cur_b1))
		VError("Could not get band %d of result image", 0);

	if(!VSelectBand("Image", current, 1, NULL, (VPointer*)&cur_b2))
		VError("Could not get band %d of result image", 0);

	if(!VSelectBand("Image", current, 2, NULL, (VPointer*)&cur_b3))
		VError("Could not get band %d of result image", 0);

	
	/* Utility variables. */
	int row, col;
	double qpMag, qpPrimeMag;
	int xp[2], xPrime[2], xPrimeFinal[2];
	int down[2], right[2], diag[2];
	double dsum[2];
	Line *srcLine, *dstLine;
	int nrows = VImageNRows(dstIm), ncols = VImageNColumns(dstIm);
	int im_size = nrows * ncols;
	int pos, npos;

	/* Allocate memory for an array that will hold precomputed data for each control
	line to speed up computation. */
	double* lineData = (double *)malloc(NUM_ELS * srcLines.size() * sizeof(double));

	/* Initialize iterators into the line queues. */
	std::deque<ImagePrimitive*>::iterator srcIt = srcLines.begin();
	std::deque<ImagePrimitive*>::iterator dstIt = dstLines.begin();
	
	int ind;
	
	int numLines = srcLines.size();

	/* Go through all the lines. */
	for(ind = 0; ind < numLines; ind++){
	
		/* Retreive current source and destination lines. */
		srcLine = static_cast<Line*>(*srcIt++);
		dstLine = static_cast<Line*>(*dstIt++);
		
		/* Retreive coordinates of first line endpoint in source image. */
		dataVal(P_PRIME_X) = srcLine->getX1();
		dataVal(P_PRIME_Y) = srcLine->getY1();
		
		/* Retreive coordinates of second line endpoint in source image. */
		dataVal(Q_PRIME_X) = srcLine->getX2();
		dataVal(Q_PRIME_Y) = srcLine->getY2();
		
		/* Retreive coordinates of first line endpoint in destination image
		interpolated by warpfraction. */
		dataVal(P_X) = interp(dataVal(P_PRIME_X), dstLine->getX1(), warpfraction);
		dataVal(P_Y) = interp(dataVal(P_PRIME_Y), dstLine->getY1(), warpfraction);
		
		/* Retreive coordinates of second line endpoint in destination image
		interpolated by warpfraction. */
		dataVal(Q_X) = interp(dataVal(Q_PRIME_X), dstLine->getX2(), warpfraction);
		dataVal(Q_Y) = interp(dataVal(Q_PRIME_Y), dstLine->getY2(), warpfraction);
		
		/* Compute the vector PQ in destination image. */
		dataVal(PQ_X) = dataVal(Q_X) - dataVal(P_X);
		dataVal(PQ_Y) = dataVal(Q_Y) - dataVal(P_Y);

		/* Compute the magnitude of the line segment PQ in destination image. */
		dataVal(PQ_MAG) = sqrt(pow(dataVal(PQ_Y), 2) + pow(dataVal(PQ_X), 2));
		
		/* Compute the vectore PQ prime in source image. */
		dataVal(PQ_PRIME_X) = dataVal(Q_PRIME_X) - dataVal(P_PRIME_X);
		dataVal(PQ_PRIME_Y) = dataVal(Q_PRIME_Y) - dataVal(P_PRIME_Y);
		
		/* Compute the magnitude of the line segment PQ prime in destination image. */
		dataVal(PQ_PRIME_MAG) = sqrt(pow(dataVal(PQ_PRIME_X), 2) + pow(dataVal(PQ_PRIME_Y), 2));
	}

	/* For each pixel in the destination image. */
	for(pos = 0; pos < im_size; pos++){
		
		/* Compute row and column positions for that pixel. */
 		row = (int)floor((double)(pos / ncols));
		col = pos % ncols;
		
		int srcPos;
		
		/* Compute the coordinates of the pixel in the source image that
		maps to this pixel and store them in xPrimeFinal. */
		computeSourcePixel(numLines, row, col, lineData, xPrimeFinal);
		
		/* Calculate coordinates as an offset into the source image band. */
		int spos = xPrimeFinal[0] * ncols + xPrimeFinal[1];
				
		/* If coordinates lie within the boundaries of the image. */
		if(xPrimeFinal[0] >= 0 && xPrimeFinal[0] < VImageNRows(srcIm) && 
		   xPrimeFinal[1] >= 0 && xPrimeFinal[1] < VImageNColumns(srcIm)){
			
			/* Set pixel intensity at position pos in the destination image 
			to pixel intensity at position spos in source image. */
			*(cur_b1 + pos) = *(src_b1 + spos);
			*(cur_b2 + pos) = *(src_b2 + spos);
			*(cur_b3 + pos) = *(src_b3 + spos);
		/* If source pixel outside of the image boundary set intensity to 0. */
		}else{
			*(cur_b1 + pos) = 0;
			*(cur_b2 + pos) = 0;
			*(cur_b3 + pos) = 0;
		}

	}
	
	cout << "Morph time: " << clock() - start  << " clock ticks \n";
	cout.flush();

	/* Return the resulting morph. */
	return current;
	
}

VImage ImDraw::mipMorph(VImage srcIm, VImage dstIm, 
deque<ImagePrimitive*> srcLines, deque<ImagePrimitive*> dstLines, 
double warpfraction){
	clock_t start = clock();
	
	VImage imgSigned = VConvertImageLinear(srcIm, NULL, VAllBands, VSByteRepn, 1, -128);
	

	/* Create 6 versions of source image with various degrees of gaussian smoothing. */
	int index;
	VImage filtered[6];
	
	for(index = 0; index < 6; index++){
		int sigma = pow((double)2, index);
		VImage smoothed = VGaussianConvolveImage(imgSigned, NULL, VAllBands, sigma, sigma * 3 + 1, VFilterGaussian);
		filtered[index] = VConvertImageLinear(smoothed, NULL, VAllBands, VUByteRepn, 1, 128);
	}
	
	/* Result of morph. */
	VImage current = VCopyImage(dstIm, NULL, VAllBands);

	/* Create an initialize image that stores indices into the source image that have already been
	determined. */
	VImage precomputed = VCreateImage(1, VImageNRows(dstIm), VImageNColumns(dstIm), VDoubleRepn);
	VFillImage (precomputed, VAllBands, DBL_MAX);
	
	VUByte *src_b1, *src_b2, *src_b3;
	VUByte *dst_b1, *dst_b2, *dst_b3;
	VUByte *cur_b1, *cur_b2, *cur_b3;
	VDouble *precomp_b;
	
	/* Select the bands of the source image. */
	if(!VSelectBand("Image", srcIm, 0, NULL, (VPointer*)&src_b1))
		VError("Could not get band %d of source image", 0);

	if(!VSelectBand("Image", srcIm, 1, NULL, (VPointer*)&src_b2))
		VError("Could not get band %d of source image", 0);

	if(!VSelectBand("Image", srcIm, 2, NULL, (VPointer*)&src_b3))
		VError("Could not get band %d of source image", 0);

	/* Select the bands of the destination image. */
	if(!VSelectBand("Image", dstIm, 0, NULL, (VPointer*)&dst_b1))
		VError("Could not get band %d of destination image", 0);

	if(!VSelectBand("Image", dstIm, 1, NULL, (VPointer*)&dst_b2))
		VError("Could not get band %d of destination image", 0);

	if(!VSelectBand("Image", dstIm, 2, NULL, (VPointer*)&dst_b3))
		VError("Could not get band %d of destination image", 0);
	
	
	/* Select the bands of the result image. */
	if(!VSelectBand("Image", current, 0, NULL, (VPointer*)&cur_b1))
		VError("Could not get band %d of result image", 0);

	if(!VSelectBand("Image", current, 1, NULL, (VPointer*)&cur_b2))
		VError("Could not get band %d of result image", 0);

	if(!VSelectBand("Image", current, 2, NULL, (VPointer*)&cur_b3))
		VError("Could not get band %d of result image", 0);

	if(!VSelectBand("Image", precomputed, 0, NULL, (VPointer*)&precomp_b))
		VError("Could not get band %d of precomputed value image", 0);

	/* Utility variables. */
	int row, col;
	double qpMag, qpPrimeMag;
	int xp[2], xPrime[2], xPrimeFinal[2];
	int down[2], right[2], diag[2];
	double dsum[2];
	Line *srcLine, *dstLine;
	int nrows = VImageNRows(dstIm), ncols = VImageNColumns(dstIm);
	int im_size = nrows * ncols;
	int pos, npos;

	/* Allocate memory for an array that will hold precomputed data for each control
	line to speed up computation. */
	double* lineData = (double *)malloc(NUM_ELS * srcLines.size() * sizeof(double));

	/* Initialize iterators into the line queues. */
	std::deque<ImagePrimitive*>::iterator srcIt = srcLines.begin();
	std::deque<ImagePrimitive*>::iterator dstIt = dstLines.begin();
	
	int ind;
	
	int numLines = srcLines.size();

	/* Go through all the lines. */
	for(ind = 0; ind < numLines; ind++){
	
		/* Retreive current source and destination lines. */
		srcLine = static_cast<Line*>(*srcIt++);
		dstLine = static_cast<Line*>(*dstIt++);
		
		/* Retreive coordinates of first line endpoint in source image. */
		dataVal(P_PRIME_X) = srcLine->getX1();
		dataVal(P_PRIME_Y) = srcLine->getY1();
		
		/* Retreive coordinates of second line endpoint in source image. */
		dataVal(Q_PRIME_X) = srcLine->getX2();
		dataVal(Q_PRIME_Y) = srcLine->getY2();
		
		/* Retreive coordinates of first line endpoint in destination image
		interpolated by warpfraction. */
		dataVal(P_X) = interp(dataVal(P_PRIME_X), dstLine->getX1(), warpfraction);
		dataVal(P_Y) = interp(dataVal(P_PRIME_Y), dstLine->getY1(), warpfraction);
		
		/* Retreive coordinates of second line endpoint in destination image
		interpolated by warpfraction. */
		dataVal(Q_X) = interp(dataVal(Q_PRIME_X), dstLine->getX2(), warpfraction);
		dataVal(Q_Y) = interp(dataVal(Q_PRIME_Y), dstLine->getY2(), warpfraction);
		
		/* Compute the vector PQ in destination image. */
		dataVal(PQ_X) = dataVal(Q_X) - dataVal(P_X);
		dataVal(PQ_Y) = dataVal(Q_Y) - dataVal(P_Y);

		/* Compute the magnitude of the line segment PQ in destination image. */
		dataVal(PQ_MAG) = sqrt(pow(dataVal(PQ_Y), 2) + pow(dataVal(PQ_X), 2));
		
		/* Compute the vectore PQ prime in source image. */
		dataVal(PQ_PRIME_X) = dataVal(Q_PRIME_X) - dataVal(P_PRIME_X);
		dataVal(PQ_PRIME_Y) = dataVal(Q_PRIME_Y) - dataVal(P_PRIME_Y);
		
		/* Compute the magnitude of the line segment PQ prime in destination image. */
		dataVal(PQ_PRIME_MAG) = sqrt(pow(dataVal(PQ_PRIME_X), 2) + pow(dataVal(PQ_PRIME_Y), 2));
	}

	/* For each pixel in the destination image. */
	for(pos = 0; pos < im_size; pos++){
		
		/* Compute row and column positions for that pixel. */
 		row = (int)floor((double)(pos / ncols));
		col = pos % ncols;
		
		int srcPos;
		
		/* If the coordinates of the pixel in source image that maps to this pixel have 
		already been determined simply store them in xPrimeFinal.*/
		if(*(precomp_b + pos) < DBL_MAX && (srcPos = (int)(*(precomp_b + pos))) < im_size){
			xPrimeFinal[0] = (int)floor((double)(srcPos / ncols));
			xPrimeFinal[1] = srcPos % ncols;
		/* Otherwise determine these coordinates. Store result in xPrimeFinal as 
		well as add it to the precomputed image. */
		}else{
			computeSourcePixel(numLines, row, col, lineData, xPrimeFinal);

			*(precomp_b + pos) = xPrimeFinal[0] * ncols + xPrimeFinal[1];
		}

		bool bdryFlag = true;
		int nrow, ncol;

		/* Get coordinates of pixel below this pixel. */
		nbrPos(pos, npos, SOUTH);
		
		/* Retrieve coordinates of pixel in source image that map to that pixel. */
		if(npos != INT_MAX){
			nrow = (int)floor((double)(npos / ncols));
			ncol = pos % ncols;
			if(*(precomp_b + npos) < DBL_MAX && (srcPos = (int)(*(precomp_b + npos))) < im_size){
				down[0] = (int)floor((double)(srcPos / ncols));
				down[1] = srcPos % ncols;
			
			}else{
				computeSourcePixel(numLines, nrow, ncol, lineData, down);
			
				*(precomp_b + npos) = down[0] * ncols + down[1];
			}
			
		}else{
			bdryFlag = false;
		}

		/* Retreive coordinates of pixel to the right of this pixel. */
		nbrPos(pos, npos, EAST);
		
		/* Retrieve coordinates of pixel in source image that map to that pixel. */
		if(npos != INT_MAX){
			nrow = (int)floor((double)(npos / ncols));
			ncol = pos % ncols;
			
			if(*(precomp_b + npos) < DBL_MAX && (srcPos = (int)(*(precomp_b + npos))) < im_size){
				right[0] = (int)floor((double)(srcPos / ncols));
				right[1] = srcPos % ncols;
			
			}else{
				computeSourcePixel(numLines, nrow, ncol, lineData, right);
			
				*(precomp_b + npos) = right[0] * ncols + right[1];
			}
		}else{
			bdryFlag = false;
		}

		/* Retreive coordinates of pixel below and to the right of this pixel. */
		nbrPos(pos, npos, SOUTH_EAST);
		
		/* Retrieve coordinates of pixel in source image that map to that pixel. */
		if(npos != INT_MAX){
			nrow = (int)floor((double)(npos / ncols));
			ncol = pos % ncols;
			
			if(*(precomp_b + npos) < DBL_MAX && (srcPos = (int)(*(precomp_b + npos))) < im_size){
				diag[0] = (int)floor((double)(srcPos / ncols));
				diag[1] = srcPos % ncols;
			
			}else{
				computeSourcePixel(numLines, nrow, ncol, lineData, diag);
			
				*(precomp_b + npos) = diag[0] * ncols + diag[1];
			}
		}else{
			bdryFlag = false;
		}

		double depthLevel;

		/* If we didn't fall outside of the image boundary. */
		if(bdryFlag){
			/* Determine the bounding box of the area that maps
			to a square of area 1 in the neighbourhood of current pixel.*/
			int minX, maxX, minY, maxY;
			
			minX = xPrimeFinal[0];
			maxX = xPrimeFinal[0];
			minY = xPrimeFinal[1];
			maxY = xPrimeFinal[1];

			if(down[0] < minX){
				minX = down[0];
			}else if(down[0] > maxX){
				maxX = down[0];
			}

			if(down[1] < minY){
				minY = down[1];
			}else if(down[1] > maxY){
				maxY = down[1];
			}

			if(right[0] < minX){
				minX = right[0];
			}else if(right[0] > maxX){
				maxX = right[0];
			}

			if(right[1] < minY){
				minY = right[1];
			}else if(right[1] > maxY){
				maxY = right[1];
			}

			if(diag[0] < minX){
				minX = diag[0];
			}else if(diag[0] > maxX){
				maxX = diag[0];
			}

			if(diag[1] < minY){
				minY = diag[1];
			}else if(diag[1] > maxY){
				maxY = diag[1];
			}


			double boundingArea = ((double)(maxX - minX)) * (maxY - minY);
			
			/* Determine depth level of the MIP map that we need to sampe from based
			on the obtained bounding box area. */
			if(boundingArea == 0){
				depthLevel = 0;
			}else{
				depthLevel = log10(sqrt((double)boundingArea)) / log10((double)2);
			}
		/* If we fell outside of the image boundary sample from the non-smoothed source image. */	
		}else{
			depthLevel = 0;
		}

		/* Calculate coordinates as an offset into the source image band. */
		int spos = xPrimeFinal[0] * ncols + xPrimeFinal[1];
				
		/* If coordinates lie within the boundaries of the image. */
		if(xPrimeFinal[0] >= 0 && xPrimeFinal[0] < VImageNRows(srcIm) && 
		   xPrimeFinal[1] >= 0 && xPrimeFinal[1] < VImageNColumns(srcIm)){
			
			/* If the area in the source is 1 we sample from depth level 0,
			or the unsmoothed image. */
			if(floor(depthLevel) == 0 || ceil(depthLevel) == 0){
				
				*(cur_b1 + pos) = *(src_b1 + spos);
				*(cur_b2 + pos) = *(src_b2 + spos);
				*(cur_b3 + pos) = *(src_b3 + spos);
			/* If depth level > 0 the area in the source image bounding box has a
			larger area than the destination image bounding box. */
			}else if(depthLevel > 0){
				/* Clip the depth level at 7 (maximum computed level of smoothing). */
				if(depthLevel > 7){
					depthLevel = 7;
				}

				int dCeil = ceil(depthLevel);
				int dFloor = floor(depthLevel);
				
				/* Calculate destination pixel intensity as a weighted sum of source pixel intensities
				at levels floor(depthLevel) and ceiling(depthLevel) as described in minification slide
				in lecture 21. */
				*(cur_b1 + pos) = 
					(int)((depthLevel - dFloor) * VPixel(filtered[dCeil - 1], 0, xPrimeFinal[0], xPrimeFinal[1], VUByte) + 
					(dCeil - depthLevel) * VPixel(filtered[dFloor - 1], 0, xPrimeFinal[0], xPrimeFinal[1],VUByte));
				*(cur_b2 + pos) = 
					(int)((depthLevel - dFloor) * VPixel(filtered[dCeil - 1], 1, xPrimeFinal[0], xPrimeFinal[1], VUByte) + 
					(dCeil - depthLevel) * VPixel(filtered[dFloor - 1], 1, xPrimeFinal[0], xPrimeFinal[1],VUByte));
				*(cur_b3 + pos) = 
					(int)((depthLevel - dFloor) * VPixel(filtered[dCeil - 1], 2, xPrimeFinal[0], xPrimeFinal[1], VUByte) + 
					(dCeil - depthLevel) * VPixel(filtered[dFloor - 1], 2, xPrimeFinal[0], xPrimeFinal[1],VUByte));
			/* If depth level < 0 the area in the source image bounding box has a smaller area than the
			destination image bounding box. */
			}else{
				/* Calculate destination pixel intensity as a weighted sum of source pixel intensities
				at levels 0 and 1 as described in magnification slide in lecture 21. */
				*(cur_b1 + pos) = (int)(*(src_b1 + spos) + -1 * depthLevel*(*(src_b1 + spos) - VPixel(filtered[0], 0, xPrimeFinal[0], xPrimeFinal[1], VUByte)));
				*(cur_b2 + pos) = (int)(*(src_b2 + spos) + -1 * depthLevel*(*(src_b2 + spos) - VPixel(filtered[0], 1, xPrimeFinal[0], xPrimeFinal[1], VUByte)));
				*(cur_b3 + pos) = (int)(*(src_b3 + spos) + -1 * depthLevel*(*(src_b3 + spos) - VPixel(filtered[0], 2, xPrimeFinal[0], xPrimeFinal[1], VUByte)));
			}

		/* If outside the boundary set pixel intensity to 0. */	
		}else{
			*(cur_b1 + pos) = 0;
			*(cur_b2 + pos) = 0;
			*(cur_b3 + pos) = 0;
		}

	}
	
	cout << "Morph time: " << clock() - start  << " clock ticks \n";
	cout.flush();
	return current;
	
}



/********************************************************************************
 * function computeSourcePixel() - calculates coordinates of the pixel in the  
 * source image that get mapped to coordinates (row,col) in the destination 
 * image given control lines specified in lineData. Stores result in srcPixel.
 *******************************************************************************/
void ImDraw::computeSourcePixel(int numLines, int row, int col, double *lineData, int *srcPixel){
	
	/* Mapping paramters. */
	double a = 0.01, b = 1.8, p = 0;
	
	/* Utility variables. */
	double u, v, dist, weight, weightsum; 
	int xp[2], xPrime[2];
	double dsum[2];
	
	dsum[0] = 0;
	dsum[1] = 0;
	weightsum = 0;
	int ind;

	/* For each line */
	for(ind = 0; ind < numLines; ind++){
		
		/* Vector XP */
		xp[0] = row - dataVal(P_X);
		xp[1] = col - dataVal(P_Y);
		
		/* Calculate parameter u as described in Beier & Neely */
		u = (xp[0] * dataVal(PQ_X) + xp[1] * dataVal(PQ_Y)) / (dataVal(PQ_MAG) * dataVal(PQ_MAG));
			
		/* Calculate parameter v as described in Beier & Neely */
		v = (xp[0] * dataVal(PQ_Y) - xp[1] * dataVal(PQ_X)) / dataVal(PQ_MAG);

		/* Calculate coordinates of pixel in the source image according to current line.*/
		xPrime[0] = dataVal(P_PRIME_X) + u * dataVal(PQ_PRIME_X) + ((v * dataVal(PQ_PRIME_Y)) / dataVal(PQ_PRIME_MAG));
		xPrime[1] = dataVal(P_PRIME_Y) + u * dataVal(PQ_PRIME_Y) + ((-1 * v * dataVal(PQ_PRIME_X)) / dataVal(PQ_PRIME_MAG));

		/* Calculate distance from pixel to current line. */
		if(u >= 0 && u <= 1){
			dist = abs(v);
		}else if(u < 0){
			dist = sqrt(pow((double)xp[0], 2) + pow((double)xp[1], 2));
		}else{
			int xq[2];
			xq[0] = row - dataVal(Q_X);
			xq[1] = col - dataVal(Q_Y);

			dist = sqrt(pow((double)xq[0], 2) + pow((double)xq[1], 2));
		}

		/* Calculate the weight assigned to current line. */
		weight = pow((pow(dataVal(PQ_MAG), p) / (a + dist)), b);

		/* Update the current estimate of where the pixel is located in the source. */
		dsum[0] += (xPrime[0] - row) * weight;
		dsum[1] += (xPrime[1] - col) * weight;

		/* Update running total weight. */
		weightsum += weight;

	
	
	}

	/* Calculate final coordinates of the pixel in source image. */
	srcPixel[0] = row + dsum[0] / weightsum;
	srcPixel[1] = col + dsum[1] / weightsum;
}

/********************************************************************************
 * function blend() - blends pixels in im1 and im2 according to blendingFactor
 * and writes resulting image to file named filename. 
 *******************************************************************************/
void ImDraw::blend(VImage im1, VImage im2, double blendingFactor, char* filename){
	VUByte *im1_band1, *im1_band2, *im1_band3;
	VUByte *im2_band1, *im2_band2, *im2_band3;
	VUByte *result_band1, *result_band2, *result_band3;
	int i = 0;

	/* Create result image. */
	VImage result = VCreateImage(3, VImageNRows(im1), VImageNColumns(im1), VUByteRepn);

	/* Initialize pointers to bands of the two source images and the result image. */
	if(!VSelectBand("Image", im1, 0, NULL, (VPointer*)&im1_band1))
		VError("Could not get band %d of gradient image", 0);

	if(!VSelectBand("Image", im1, 1, NULL, (VPointer*)&im1_band2))
		VError("Could not get band %d of gradient image", 0);

	if(!VSelectBand("Image", im1, 2, NULL, (VPointer*)&im1_band3))
		VError("Could not get band %d of gradient image", 0);

	if(!VSelectBand("Image", im2, 0, NULL, (VPointer*)&im2_band1))
		VError("Could not get band %d of gradient image", 0);

	if(!VSelectBand("Image", im2, 1, NULL, (VPointer*)&im2_band2))
		VError("Could not get band %d of gradient image", 0);

	if(!VSelectBand("Image", im2, 2, NULL, (VPointer*)&im2_band3))
		VError("Could not get band %d of gradient image", 0);

	if(!VSelectBand("Image", result, 0, NULL, (VPointer*)&result_band1))
		VError("Could not get band %d of gradient image", 0);

	if(!VSelectBand("Image", result, 1, NULL, (VPointer*)&result_band2))
		VError("Could not get band %d of gradient image", 0);

	if(!VSelectBand("Image", result, 2, NULL, (VPointer*)&result_band3))
		VError("Could not get band %d of gradient image", 0);


	/* For each pixel in result store a weighted sum of pixel intensities
	in im1 and im2. */
	for(i = 0; i < VImageNRows(im1) * VImageNColumns(im1); i++){
		*(result_band1 + i) = (int)floor(*(im1_band1 + i) * (1 - blendingFactor)) +
							  (int)floor(*(im2_band1 + i)  * blendingFactor);
		*(result_band2 + i) = (int)floor(*(im1_band2 + i) * (1 - blendingFactor)) +
							  (int)floor(*(im2_band2 + i)  * blendingFactor);
		*(result_band3 + i) = (int)floor(*(im1_band3 + i) * (1 - blendingFactor)) +
							  (int)floor(*(im2_band3 + i)  * blendingFactor);
	}

	FILE *fp;
	
	/* Open destination file. */	

#ifdef WIN32
	fp = fopen(filename, "wb");
#else
	fp = fopen(filename, "w");
#endif
	
	/* Write result image to destination file. */
	if(!VWriteImages(fp, NULL, 1, &result)){
		exit(1);
	}
	fclose(fp);
}
