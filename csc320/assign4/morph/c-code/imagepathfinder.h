#ifndef IMAGEPATHFINDER_H
#define IMAGEPATHFINDER_H


#include <vista/VImage.h>

class ImagePathFinder{
	public:
		ImagePathFinder();
		ImagePathFinder(VImage im);
		~ImagePathFinder();
		VImage getZeroCrossingsImage();
		VImage getGradientMagnitudeImage();
	private:
		VImage rgbToGrayscale(VImage img);
		void zeroCrossings(VImage img, int sigma);
		void gradientMagnitude(VImage img);
		VDouble gradMagMax(VImage img);
		VImage zeroCrossIm, gradMagIm; 
};

#endif