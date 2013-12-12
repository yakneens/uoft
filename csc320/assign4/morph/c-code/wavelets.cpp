#include <iostream.h>
#include "wavelets.h"

#include <newmat/newmatio.h>

// This is a simple example of a 'reduce' function
// that just subsamples the interior knots 
// of a bspline to create a bspline with half the 
// number of interior knot intervals.
//
// For example, a bspline with 
// 
//   knots    [f f f f i1 i2 i3 e e e e] (j=4 interior intervals)
//   becomes  [f f f f i2 e e e e] (j=2 interior intervals)
// 


bool reduce(Matrix full_res, int k, int j, int d,
			Matrix& half_res, Matrix& detail) {
	int n1, n2, i;
	int c1, c2, numdrows;
	Matrix chop, chop2;

	if (j == 0) {
     return false;
	}

	n1 = full_res.nrows();
	
	half_res.ReSize(pow((double)2, j - 1) + d + 4, 2);
	n2 = half_res.nrows();
	

	numdrows = pow((double)2, j) - pow((double)2, j - 1);
	
		
	/* Remove two rows off the top and two rows off the bottom of full_res
	   and store the resulting matrix in full_res. */
	chop.ReSize(n1 - 4, 2);

	for(i = 3; i <= n1 - 2; i++){
		chop.Row(i - 2) = full_res.Row(i);
	}



	chop2.ReSize(n1 - 4, 2);
	
	/* Obtain the matrix consisting of a vertical concatenation
	   of the A and B matrices from the paper. */
	Matrix ab = initABMatrix(n1 - 4);

	/* Perform the transform to a lower resolution. */
	chop2 = ab * chop;
	
	/* Pad values back to form a legal endpoint interpolating bspline
	   and store result in half_res. */
	chop2.Row(2) = chop2.Row(1);
	chop2.Row(n2 - 5) = chop2.Row(n2 - 4);

	half_res.Row(1) = chop2.Row(1);
	half_res.Row(2) = chop2.Row(1);
	half_res.Row(n2) = chop2.Row(n2 - 4);
	half_res.Row(n2 - 1) = chop2.Row(n2 - 4);
	
	for(i = 1; i <= n2 - 4; i++){
		half_res.Row(i + 2) = chop2.Row(i);

	}

	/* Store the detail image. */
	detail.ReSize(n1 - n2, 2);

	for(i = 1; i <= numdrows; i++){
		detail.Row(i) = chop2.Row(n2 - 4 + i);	
	}
	
	return true;
}

bool expand(Matrix current, Matrix original, Matrix hiResOriginal, Matrix& next, int k, int j, int d){
	int size = pow((double)2, j + 1) + d;
	int i;
	int q;
	
	/* Obtain [P | Q] matrix. */
	Matrix pq = initPQMatrix(size);
	
	/* Obtain P matrix */
	Matrix p(size, pow((double)2, j) + d);
	int nrows = p.nrows();
	int ncols = p.ncols();
	for(i = 1; i <= ncols; i++){
		p.Column(i) = pq.Column(i);
	}

	/* Compute change matrix. */
	Matrix deltaC = current - original;
	
	/* Remove two rows on each side from delta matrix. */
	Matrix chop(deltaC.nrows() - 4, 2);
	
	for(i = 3; i <= deltaC.nrows() - 2; i++){
		chop.Row(i - 2) = deltaC.Row(i);
	}
	
	/* Multiply by reconstruction matrix p */
	Matrix tempRes = p * chop;

	tempRes.Row(2) = tempRes.Row(1);
	tempRes.Row(tempRes.nrows() - 1) = tempRes.Row(tempRes.nrows());
	
	Matrix restored(tempRes.nrows() + 4, 2);
	restored = 0.0;

	/* Pad with two rows on each side. */
	restored.Row(1) = tempRes.Row(1);
	restored.Row(2) = tempRes.Row(1);
	restored.Row(restored.nrows()) = tempRes.Row(tempRes.nrows());
	restored.Row(restored.nrows() - 1) = tempRes.Row(tempRes.nrows());

	for(i = 3; i <= tempRes.nrows() - 2; i++){
		restored.Row(i) = tempRes.Row(i - 2);
	}
	
	/* Add change matrix to original. */
	next = hiResOriginal + restored;
	
	return true;

}

/* Initialize matrix [P | Q] */
Matrix initPQMatrix(int size){
	
	int ncolsp = ((size - 3) / 2) + 3;
	int ncolsq = size - ncolsp;
	int nrows = size;
	int col, row = 4;
	int ind = 0;
	
	Matrix pq(size, size);
	pq = 0.0;
		
		
		
	/* j = 1 */
	if(size == 5){
		
		pq(1, 1) = 2;
		pq(2, 1) = 1;
		
		pq(2, 2) = 1;
		pq(3, 2) = 1;
		
		pq(3, 3) = 1;
		pq(4, 3) = 1;
		
		pq(4, 4) = 1;
		pq(5, 4) = 2;
		
		for(ind = 1; ind <= 4; ind++){
			pq.Column(ind) *= 0.5;
		}

		pq(1, 5) = 1;
		pq(2, 5) = -2;
		pq(3, 5) = 3;
		pq(4, 5) = -2;
		pq(5, 5) = 1;
		pq.Column(5) *= sqrt((double)7);
	/* j = 2 */
	}else if(size == 7){
		
		pq(1, 1) = 16;
		pq(2, 1) = 8;
		
		pq(2, 2) = 8;
		pq(3, 2) = 12;
		pq(4, 2) = 3;
		
		pq(3, 3) = 4;
		pq(4, 3) = 10;
		pq(5, 3) = 4;
		
		pq(4, 4) = 3;
		pq(5, 4) = 12;
		pq(6, 4) = 8;
		
		pq(6, 5) = 8;
		pq(7, 5) = 16;

		for(ind = 1; ind <= 5; ind++){
			pq.Column(ind) *= (double)1 / 16;
		}
		
		pq(1, 6) = 1368;
		pq(2, 6) = -2064;
		pq(3, 6) = 1793;
		pq(4, 6) = -1053;
		pq(5, 6) = 691;
		pq(6, 6) = -240;

		pq(2, 7) = -240;
		pq(3, 7) = 691;
		pq(4, 7) = -1053;
		pq(5, 7) = 1793;
		pq(6, 7) = -2064;
		pq(7, 7) = 1368;

		for(ind = 6; ind <= 7; ind++){
			pq.Column(ind) *= sqrt((double)((double)315 / 31196288));
		}
	/* j = 3 */
	}else if(size == 11){

		pq(1, 1) = 16;
		pq(2, 1) = 8;

		pq(2, 2) = 8;
		pq(3, 2) = 12;
		pq(4, 2) = 3;

		pq(3, 3) = 4;
		pq(4, 3) = 11;
		pq(5, 3) = 8;
		pq(6, 3) = 2;

		pq(4, 4) = 2;
		pq(5, 4) = 8;
		pq(6, 4) = 12;
		pq(7, 4) = 8;
		pq(8, 4) = 2;

		pq(6, 5) = 2;
		pq(7, 5) = 8;
		pq(8, 5) = 11;
		pq(9, 5) = 4;

		pq(8, 6) = 3;
		pq(9, 6) = 12;
		pq(10, 6) = 8;

		pq(10, 7) = 8;
		pq(11, 7) = 16;
		
		cout << pq(1, 1);
		cout.flush();
		
		for(ind = 1; ind <= 7; ind++){
			pq.Column(ind) *= (double)1 / 16;
		}

		cout << pq(1, 1);
		cout.flush();


		pq(1, 8) = 6.311454;
		pq(2, 8) = -9.189342;
		pq(3, 8) = 7.334627;
		pq(4, 8) = -3.514553;
		pq(5, 8) = 1.271268;
		pq(6, 8) = -0.259914;
		pq(7, 8) = 0.019190;
		pq(8, 8) = -0.000155;
		
		pq(2, 9) = -1.543996;
		pq(3, 9) = 4.226722;
		pq(4, 9) = -5.585477;
		pq(5, 9) = 6.059557;
		pq(6, 9) = -4.367454;
		pq(7, 9) = 1.903267;
		pq(8, 9) = -0.473604;
		pq(9, 9) = 0.087556;
		
		pq(3, 10) = 0.087556;
		pq(4, 10) = -0.473604;
		pq(5, 10) = 1.903267;
		pq(6, 10) = -4.367454;
		pq(7, 10) = 6.059557;
		pq(8, 10) = -5.585477;
		pq(9, 10) = 4.226722;
		pq(10, 10) = -1.543996;

		pq(4, 11) = -0.000155;
		pq(5, 11) = 0.019190;
		pq(6, 11) = -0.259914;
		pq(7, 11) = 1.271268;
		pq(8, 11) = -3.514553;
		pq(9, 11) = 7.334627;
		pq(10, 11) = -9.189342;
		pq(11, 11) = 6.311454;

	/* j > 3 */	
	}else{
	
		double pScalingFactor = (double)1 / 16;
		double qScalingFactor = sqrt((double)((double)(5 * (size - 3)) / 675221664));

		pq(1, 1) = 16 * pScalingFactor;
		pq(2, 1) = 8 * pScalingFactor;

		pq(2, 2) = 8 * pScalingFactor;
		pq(3, 2) = 12 * pScalingFactor;
		pq(4, 2) = 3 * pScalingFactor;

		pq(3, 3) = 4 * pScalingFactor;
		pq(4, 3) = 11 * pScalingFactor;
		pq(5, 3) = 8 * pScalingFactor;
		pq(6, 3) = 2 * pScalingFactor;

		
		pq(nrows - 5, ncolsp - 2) = 2 * pScalingFactor;
		pq(nrows - 4, ncolsp - 2) = 8 * pScalingFactor;
		pq(nrows - 3, ncolsp - 2) = 11 * pScalingFactor;
		pq(nrows - 2, ncolsp - 2) = 4 * pScalingFactor;

		pq(nrows - 3, ncolsp - 1) = 3 * pScalingFactor;
		pq(nrows - 2, ncolsp - 1) = 12 * pScalingFactor;
		pq(nrows - 1, ncolsp - 1) = 8 * pScalingFactor;

		pq(nrows - 1, ncolsp) = 8 * pScalingFactor;
		pq(nrows, ncolsp) = 16 * pScalingFactor;
		
	
		pq(1, ncolsp + 1) = 25931.20071 * qScalingFactor;
		pq(2, ncolsp + 1) = -37755.271723 * qScalingFactor;
		pq(3, ncolsp + 1) = 30135.003012 * qScalingFactor;
		pq(4, ncolsp + 1) = -14439.869635 * qScalingFactor;
		pq(5, ncolsp + 1) = 5223.125428 * qScalingFactor;
		pq(6, ncolsp + 1) = -1067.879425 * qScalingFactor;
		pq(7, ncolsp + 1) = 78.842887 * qScalingFactor;
		pq(8, ncolsp + 1) = -0.635830 * qScalingFactor;

		
		pq(2, ncolsp + 2) = -6369.305453 * qScalingFactor;
		pq(3, ncolsp + 2) = 17429.266054 * qScalingFactor;
		pq(4, ncolsp + 2) = -23004.252368 * qScalingFactor;
		pq(5, ncolsp + 2) = 24848.487871 * qScalingFactor;
		pq(6, ncolsp + 2) = -17678.884301 * qScalingFactor;
		pq(7, ncolsp + 2) = 7394.685374 * qScalingFactor;
		pq(8, ncolsp + 2) = -1561.868558 * qScalingFactor;
		pq(9, ncolsp + 2) = 115.466347 * qScalingFactor;
		pq(10, ncolsp + 2) = -0.93118 * qScalingFactor;

		pq(3, ncolsp + 3) = 385.797044 * qScalingFactor;
		pq(4, ncolsp + 3) = -2086.545605 * qScalingFactor;
		pq(5, ncolsp + 3) = 8349.373420 * qScalingFactor;
		pq(6, ncolsp + 3) = -18743.473059 * qScalingFactor;
		pq(7, ncolsp + 3) = 24291.795239 * qScalingFactor;
		pq(8, ncolsp + 3) = -18420.997597 * qScalingFactor;
		pq(9, ncolsp + 3) = 7866.732009 * qScalingFactor;
		pq(10, ncolsp + 3) = -1668.615872 * qScalingFactor;
		pq(11, ncolsp + 3) = 123.378671 * qScalingFactor;
		pq(12, ncolsp + 3) = -0.994989 * qScalingFactor;

	
		pq(size - 11, size - 2) = -0.994989 * qScalingFactor;
		pq(size - 10, size - 2) = 123.378671 * qScalingFactor;
		pq(size - 9, size - 2) = -1668.615872 * qScalingFactor;
		pq(size - 8, size - 2) = 7866.732009 * qScalingFactor;
		pq(size - 7, size - 2) = -18420.997597 * qScalingFactor;
		pq(size - 6, size - 2) = 24291.795239 * qScalingFactor;
		pq(size - 5, size - 2) = -18743.473059 * qScalingFactor;
		pq(size - 4, size - 2) = 8349.373420 * qScalingFactor;
		pq(size - 3, size - 2) = -2086.545605 * qScalingFactor;
		pq(size - 2, size - 2) = 385.797044 * qScalingFactor;
		
		pq(size - 9, size - 1) = -0.93118 * qScalingFactor;
		pq(size - 8, size - 1) = 115.466347 * qScalingFactor;
		pq(size - 7, size - 1) = -1561.868558 * qScalingFactor;
		pq(size - 6, size - 1) = 7394.685374 * qScalingFactor;
		pq(size - 5, size - 1) = -17678.884301 * qScalingFactor;
		pq(size - 4, size - 1) = 24848.487871 * qScalingFactor;
		pq(size - 3, size - 1) = -23004.252368 * qScalingFactor;
		pq(size - 2, size - 1) = 17429.266054 * qScalingFactor;
		pq(size - 1, size - 1) = -6369.305453 * qScalingFactor;
		
		pq(size - 7, size) = -0.635830 * qScalingFactor;
		pq(size - 6, size) = 78.842887 * qScalingFactor;
		pq(size - 5, size) = -1067.879425 * qScalingFactor;
		pq(size - 4, size) = 5223.125428 * qScalingFactor;
		pq(size - 3, size) = -14439.869635 * qScalingFactor;
		pq(size - 2, size) = 30135.003012 * qScalingFactor;
		pq(size - 1, size) = -37755.271723 * qScalingFactor;
		pq(size, size) = 25931.20071 * qScalingFactor;
		
		for(ind = size - 2; ind <= size; ind++){
			pq.Column(ind) *= sqrt((double)((double)(5 * (size - 3)) / 675221664));
		}

		for(col = 4; col <= ncolsp - 3; col++){
			pq(row,col) = 2 * pScalingFactor;
			pq(row + 1, col) = 8 * pScalingFactor;
			pq(row + 2, col) = 12 * pScalingFactor;
			pq(row + 3, col) = 8 * pScalingFactor;
			pq(row + 4, col) = 2 * pScalingFactor;
			
			row += 2;
		}

			row = 4;

		for(col = ncolsp + 4; col <= ncolsp + ncolsq - 3; col++){
			pq(row,col) = -1 * qScalingFactor;
			pq(row + 1, col) = 124 * qScalingFactor;
			pq(row + 2, col) = -1677 * qScalingFactor;
			pq(row + 3, col) = 7904 * qScalingFactor;
			pq(row + 4, col) = -18482 * qScalingFactor;
			pq(row + 5,col) = 24264 * qScalingFactor;
			pq(row + 6, col) = -18482 * qScalingFactor;
			pq(row + 7, col) = 7904 * qScalingFactor;
			pq(row + 8, col) = -1677 * qScalingFactor;
			pq(row + 9, col) = 124 * qScalingFactor;
			pq(row + 10, col) = -1 * qScalingFactor;

			row += 2;
		}
	}
	
	return pq;



}

/* Initialize the matrix [A / B] */
Matrix initABMatrix(int size){
	return initPQMatrix(size).i();
}