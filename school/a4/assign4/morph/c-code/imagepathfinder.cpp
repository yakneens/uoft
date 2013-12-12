#include "imagepathfinder.h"
#include <math.h>
#include <stdlib.h>

ImagePathFinder::ImagePathFinder(){

}

ImagePathFinder::ImagePathFinder(VImage im){
	VImage grayscaleIm = rgbToGrayscale(im);
	zeroCrossings(grayscaleIm, 1);
	gradientMagnitude(grayscaleIm);
}

ImagePathFinder::~ImagePathFinder(){

}

VImage ImagePathFinder::rgbToGrayscale(VImage img){
	/* Convert to grayscale. */	
	VImage myImage = VCreateImage(1, VImageNRows(img), VImageNColumns(img),VUByteRepn);
	if(VImageNBands(img) == 3){
		for(int i = 0; i < VImageNRows(img); i++){
			for(int j = 0; j < VImageNColumns(img); j++){
				
				VPixel(myImage, 0, i, j, VUByte) = 
					(VPixel(img, 0, i,j, VUByte) + 
					 VPixel(img, 1, i,j, VUByte)  + 
					 VPixel(img, 2, i,j, VUByte)) / 3;

			}
		}

		return myImage;
	}else{
		return img;
	}
}

void ImagePathFinder::zeroCrossings(VImage img, int sigma){

	int sigma2 = ceil(sigma / sqrt(2)), sigma3 = ceil(sqrt(2) * sigma);
			
	if(sigma2 < 1){ sigma2 = 1;}
	if(sigma3 < 1){ sigma3 = 1;}

	/* Compute two gaussian smoothed versions of the image */
	VImage gaussSmoothImg = 
		VGaussianConvolveImage(img, NULL, VAllBands, sigma2, 3 * sigma2, VFilterGaussian);
	VImage gaussSmoothImg2 = 
		VGaussianConvolveImage(img, NULL, VAllBands, sigma3, 3 * sigma3, VFilterGaussian);
	VImage laplImg = VCreateImage(1, VImageNRows(gaussSmoothImg), VImageNColumns(gaussSmoothImg), VSByteRepn);


	/* Compute difference of gaussians. */
	for(int k = 0; k < VImageNRows(gaussSmoothImg); k++){
		for(int l = 0; l < VImageNColumns(gaussSmoothImg); l++){
			
			VPixel(laplImg, 0, k, l, VUByte) = 
				abs(VPixel(gaussSmoothImg, 0, k, l, VUByte) - 
				VPixel(gaussSmoothImg2, 0, k, l, VUByte));

		}
	}

	/* Find the zero crossings in resulting image. */
	VImage zeroCrossImg = VZeroCrossings(laplImg, NULL, VAllBands);

	VDestroyImage(gaussSmoothImg);
	VDestroyImage(gaussSmoothImg2);
	VDestroyImage(laplImg);

	zeroCrossIm = zeroCrossImg;

}

void ImagePathFinder::gradientMagnitude(VImage img){
	
	/* Compute image gradient. */
	VImage gradImage = VImageGradient(img, NULL, VAllBands);
	
	/* Compute magnitudes of the image gradient. */
	VImage gradMagImage = VImageMagnitude(gradImage, NULL, VAllBands);

	VDestroyImage(gradImage);

	gradMagIm = gradMagImage;
}

VDouble ImagePathFinder::gradMagMax(VImage img){
	
	VDouble magMax = 0;

	/* Compute maximum gradient magnitude in the image. */
	if(!VImageStats(img, VAllBands, NULL, &magMax, NULL, NULL)){
		exit(1);
	}

	return magMax;
}


VImage ImagePathFinder::getGradientMagnitudeImage(){
	return gradMagIm;
}

VImage ImagePathFinder::getZeroCrossingsImage(){
	return zeroCrossIm;
}