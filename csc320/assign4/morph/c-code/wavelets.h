#ifndef _WAVELETS_H
#define _WAVELETS_H

#include <math.h>

#include <newmat/newmat.h>
#include <newmat/newmatap.h>

using namespace NEWMAT;



bool reduce(Matrix full_res, int k, int j, int d, 
			Matrix& half_res, Matrix& detail);
bool expand(Matrix current, Matrix original, Matrix hiResOriginal, Matrix& next, int k, int j, int d);
Matrix initPQMatrix(int size);
Matrix initABMatrix(int size);
#endif
