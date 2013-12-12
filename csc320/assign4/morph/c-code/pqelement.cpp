#include "pqelement.h"


PQElement::PQElement(int pixelIndex, double priority){ 
	this->pixelIndex = pixelIndex;
	this->priority = priority;
}

double PQElement::getPriority() const{
	return this->priority;
}

void PQElement::setPriority(double priority) {
	this->priority = priority;
}

int PQElement::getPixelIndex() const{
	return this->pixelIndex;
}

void PQElement::setPixelIndex(int pixelIndex) {
	this->pixelIndex = pixelIndex;
}