// WARNING: DO NOT MODIFY THIS FILE 


#include <iostream.h>
#include <newmat/newmatio.h>
#include <math.h>

#ifdef _WIN32
#define rint(x) ((((x)-floor(x))>0.5)?(ceil(x)):(floor(x)))
#endif

#include "bspline.h"

// Add a knot to the bspline
void BSpline::add_knot(int row, int col) {
     // add a new knot to the end of the knot list
     _knots.push_back(Knot(row, col));

     // store the current knot coordinates in a newmat matrix
     update_matrix();
}

// Move knot
void BSpline::move_knot(int near_row, int near_col, int to_row, int to_col)
{
     KnotList::iterator i, bi;
     KnotList::reverse_iterator ri;
     int dist, bdist, dr, dc, j, bj;

     // can only move knots if all knots of bspline have already been specified
     if (!endpoints_locked())
	  return;

     //
     // search for the knot closest to near_row, near_col
     //
     dr = _knots.begin()->r() - near_row;
     dc = _knots.begin()->c() - near_col;
     bdist = dr*dr + dc*dc;
     bj = 1;
     for (i=_knots.begin(), bi=_knots.begin(), j=1; i!=_knots.end(); i++, j++) {
	  // compute distance of knot from near_ point
	  dr = i->r() - near_row;
	  dc = i->c() - near_col;
	  dist = dr*dr + dc*dc;
	  if (bdist > dist) {
	       bi = i;
	       bdist = dist;
	       bj = j;
	  }
     }
     // if it is not an endpoint of the spline, we just update a single knot
     if (( bj > _d+1) && (bj <= _k)) {
	  bi->r() = near_row;
	  bi->c() = near_col;
     } else if (bj == 1) {
	  // closest point is the first endpoint
	  for (i=_knots.begin(), j=1; j <= _d+1; i++, j++) {
	       i->r() = near_row;
	       i->c() = near_col;
	  } 
     } else {
	  // closest point is the second endpoint
	  for (ri=_knots.rbegin(), j=1; j <= _d+1; j++, ri++) {
	       ri->r() = near_row;
	       ri->c() = near_col;
	  }
     }
     
     // update the knot matrix
     update_matrix();	       
}     

// Delete all knots from the bspline
void BSpline::delete_knots() {
     // delete all knots from the knot list
     _knots.clear();
     // deallocate the matrix holding the knot points
     _knotMat.CleanUp();
     // set the done flag to false, indicating that the complete spline
     // has not been defined
     _done = false;
	 _k = 0;
}

// Update the netmat matrix that holds the bspline knot coordinates
void BSpline::update_matrix()
{
     KnotList::iterator i;
     int el;
     
     if (_knots.size() > _knotMat.nrows()) {
	  // allocate space for the vector of 
	  _knotMat.ReSize(_knots.size(), 2);
     }

     // iterate over all knots in the knot list
     for (i=_knots.begin(), el=1; i!=_knots.end(); i++, el++) {
	  // store the knot coordinates in a Mx2 vector where M is the number
	  // of knots
	  _knotMat(el, 1) = i->r();
	  _knotMat(el, 2) = i->c();
     }

	 // update the parameter k:
	 //  * when the user has not yet specified all knots,
	 //    set k equal to the number of knots in the bspline
	 //  * when the user has specified all knots, and therefore
	 //    the bspline's endpoints are known, we use the definition
	 //    from the wavelet tutorial, i.e., the number of knots in
	 //    the bspline is k+d+1
	 if (!endpoints_locked())
		 _k = _knotMat.nrows();
	 else
		 _k = _knotMat.nrows()-_d-1;
}

// Get the matrix associated with the endpoint-interpolating b-spline
bool BSpline::get_matrix(Matrix& mat, int& k, int& j, int& d) const
{
	if (!endpoints_locked())
		return false;

	mat = _knotMat;
	k = _k;
	j = _j;
	d = _d;

	return true;
}

// Construct an endpoint-interpolating bspline from the supplied
// matrix mat. The matrix must describe an endpoint-interpolating
// bspline. Specifically, mat must be a 2xN matrix such that
//    * N=2^j + 2d + 1 (with d=3)
//    * The first d+1 columns are identical
//    * The last d+1 columns are identical
//    * mat(1,j) is the row coordinate of the j-th knot
//    * mat(2,j) is the column coordinate of the j-th knot
//    * coordinates are specified in the vista coordinate system

BSpline::BSpline(const Matrix mat)
{
	int N, j, i;
	RowVector first(2), last(2);

	// specify default parameters in case the matrix
	// passed as argument is incorrect
	_d = 3;
	_k = 0;
	_done = false;
	draw_init(); 


	// check that the matrix has two rows
	if (mat.ncols() != 2) {
		cerr << "BSpline::BSpline(): Error: Knot matrix must have two columns" << endl
		 	 << "BSpline::BSpline(): Creating empty B-spline" << endl;
		return;
	}

	// check that the number of interior knot intervals is a power of two
	N = mat.nrows();
	j = (int) ceil(log((double)N-2*_d-1)/log(2.0));
	if ((N-2*_d-1) != (1<<j)) {
		cerr << "BSpline::BSpline(): Error: Incorrect number of interior knots" << endl
		 	 << "BSpline::BSpline(): Creating empty B-spline" << endl;
		return;
	}

	// check that the first and last d+1 columns are identical
	first = mat.Row(1);
	last = mat.Row(N);
	for (i=1; i<_d; i++) {
		if ((first(1) != mat(i+1,1)) ||
			(first(2) != mat(i+1,2)) ||
			(last(1) != mat(N-i,1)) ||
			(last(2) != mat(N-i,2))) {
			cerr << "BSpline::BSpline(): Error: Matrix is not an endpoint-interpolating bspline" 
			     << endl << "BSpline::BSpline(): Creating empty B-spline" << endl;
			return;
		}
	}
    
	// matrix is in correct format, so create the knots
	for (i=1; i<=N; i++)
	     add_knot((int)rint(mat(i,1)), (int)rint(mat(i,2)));
	// update the internal bspline parameters
	_j = j;
	_done = true;
	update_matrix();
}
	

// This function is called after the last knot in the bspline is
// specified
void BSpline::lock_endpoints() {
     int i, j;
	 int new_k;

     _done = true;

     // In a bspline with N user-specified knots, there are
     // N-1 intervals between the knot points (called 'interior
     // intervals' in the wavelet tutorial). In order to
     // represent it with a wavelet basis, the number of these
     // intervals must be a power of 2. If they are not, we must
     // 'pad' the bspline with extra knots to make N-1 a power of 2
     // In this implementation, N=_k when _done=false;
     // In the following we have N=2^(_j), i.e.,  _j is the exponent
     // of 2 in the wavelets tutorial
     
     _j = (int) ceil(log((double)_k-1)/log(2.0));
     new_k = 1 << _j;
     if (new_k > _k-1) {
	  // we pad both the beginning and end of the list with half
	  // the required set of additional knots
	  for (i=1; i<=(new_k-_k+1)/2; i++)
	       _knots.push_front(Knot(_knots.begin()->r(), 
				      _knots.begin()->c()));
	  for (;i<=new_k-_k+1; i++)
	       _knots.push_back(Knot(_knots.rbegin()->r(), 
				     _knots.rbegin()->c()));
     }
     
     // convert into an endpoint-interpolating spline of degree d by
     // repeating the first & last control points another d times, so that
	 // the first and last (d+1) knots are identical
     for (i=1; i<=_d; i++) {
	  _knots.push_front(Knot(_knots.begin()->r(), _knots.begin()->c()));
	  _knots.push_back(Knot(_knots.rbegin()->r(), _knots.rbegin()->c()));
     }

     update_matrix();
     // print the contents of the matrix
     // cout << _knotMat;
}

