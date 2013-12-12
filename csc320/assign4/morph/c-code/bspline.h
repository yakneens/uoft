// WARNING: DO NOT MODIFY THIS FILE

//
// Class implementing an endpoint-interpolating cubic b-spline
//
// Endpoint-interpolating cubic bsplines, appropriate 
// for wavelet decomposition as defined in the wavelets 
// tutorial-partII, are represented by a list of knots that 
// has the following structure:
//      [f f f f i1 i2 i3 ... ik e e e e]
// in this list, the first (d+1) knots and the last (d+1) knots
// are identical, where d is the degree of the bspline (in our case
// we will be using cubic bsplines, so d=3).
// 
// Each pair of adjacent knots defines an 'interval'. The intervals
//           <f i1> <i1 i2> ... <ik e> 
// are called 'interior intervals'. To be wavelet-decomposable, the
// number of interior intervals must be a power of 2. 
//
// 'Reducing' a bspline corresponds to halving the number of 
// interior intervals of the spline.
// 
// 'Expanding' the bspline doubles the number of interior intervals
//
// Each knot is a 2D location that defines the overall shape of the
// b-spline. The knots f and e correspond to the location
// the b-spline's endpoints.  
//
// Each knot has two Vista coordinates (row,col) so the b-spline
// is also represented as a 2xN matrix. 
// 
// For the purpose of the assignment, the internal details of the
// class are not important. It suffices to think of a bspline as
// a list of points, as shown above, 


#ifndef _BSPLINE_H
#define _BSPLINE_H 1

#include <list>
using namespace std;

#include <newmat/newmat.h>
using namespace NEWMAT;



// Miscellaneous class that uses STL to create and maintain knot lists
class Knot {
     int _r, _c;
public:
     Knot() {_r = _c = 0;};
     Knot(int r, int c) {_r = r; _c = c;};
     Knot(const Knot &sn) {_r = sn._r; _c = sn._c; };
     int r() const {return _r;};
     int c() const {return _c;};
     int &r() {return _r;};
     int &c() {return _c;};
};
typedef list<Knot> KnotList;

class BSpline {
     //
     // tutorial-related parameters
     //
     int _d;  // degree of the bspline
     int _k;  // parameter k from wavelets tutorial
     int _j;  // parameter j from wavelets tutorila
     
     //
     // internal data structures
     //
     
     // list of bspline knots
     KnotList _knots;
     // user has specified all knots
     bool _done;
     // matrix containing the knot coordinates
     Matrix _knotMat;
     
     //
     // parameters that control bspline drawing
     // 
     // draw the knots when drawing the bspline?
     bool _drawKnots;
     // draw the bspline curve?
     bool _drawCurve;
     // knot color
     double *_knotCol;
     // curve color
     double *_curveCol;
     // number of line segments drawn 
     // per interior interval
     int _accuracy;
     
     // matrices needed for computing points on the bspline
     Matrix _uMat;
     Matrix _bMat;
     // matrix holding the OpenGl coordinates of all points on the bspline
     Matrix _glPts;
 private:
     bool empty() const {return (_k == 0);};
     void draw_init();     
     void update_matrix();
 public:
     //
     // default constructor & destructor
     //
     BSpline() {_d = 3; _k = 0; _done = false; draw_init(); };
     // destructor
     ~BSpline() {delete _curveCol; delete _knotCol;};
     
     //
     // public methods for manipulating bspline knots
     //
     // add a new knot to the end of the bspline curve
     void add_knot(int row, int col);
     // move the knot closest to (near_row, near_col)
     // the new knot is moved to position (to_row, to_col)
     // note that all knots must have already been specified
     // before the move function can have an effect
     void move_knot(int near_row, int near_col, int to_row, int to_col);
     // delete all knots
     void delete_knots();
     // declare that the last knot has been specified and therefore
     // the bspline's endpoints will be the first and last knot
     void lock_endpoints();
     // check if the bspline endpoints have been specified
     bool endpoints_locked() const {return _done;};
     
     //
     // public methods for representing an endpoint-interpolating
     // bspline as a newmat matrix
     //
     
     // Return the matrix representation of the user-specified endpoint
     // interpolating bspline, along with the associated 
     // parameters j, k, d. These values are only returned if the 
     // bspline's endpoints have been locked/specified. The function
     // returns 'false' if the endpoints of the bspline
     // have not yet been specified and returns 'true' otherwise
     // 
     // Specifically, mat is a 2xN matrix such that
     //    * N=k + d + 1 (with d=3)
     //    * k = 2^j + d (with d=3)
     //    * The first d+1 columns are identical
     //    * The last d+1 columns are identical
     //    * mat(1,j) is the row coordinate of the j-th knot
     //    * mat(2,j) is the column coordinate of the j-th knot
     //    * coordinates are specified in the vista coordinate system
     bool get_matrix(Matrix& mat, int& k, int& j, int& d) const;
     
     // Construct an endpoint-interpolating bspline from the supplied
     // matrix mat. The matrix must describe an endpoint-interpolating
     // bspline. Specifically, mat must be a 2xN matrix such that
     //    * N=2^j + 2d + 1 (with d=3)
     //    * The first d+1 columns are identical
     //    * The last d+1 columns are identical
     //    * mat(1,j) is the row coordinate of the j-th knot
     //    * mat(2,j) is the column coordinate of the j-th knot
     //    * coordinates are specified in the vista coordinate system
     BSpline(const Matrix mat);
     
     // 
     // public methods that control bspline drawing
     //
     
     // draw the bspline; function also takes as argument a
     // matrix that describes the vista2gl coordinate conversion.
     // this is needed because (1) knot points are specified in 
     // vista coordinates and (2) the bspline class is 
     // independent of the imdraw class, and does not have access
     // to the internal imdraw data structures
     void draw(const Matrix v2gl);

     // Compute a set of points on the bspline, in OpenGL coordinates
     // Returns false if the bspline's endpoints have not been locked
     // otherwise, returns a matrix with 2 columns and (N-5)*a+1 rows,
     // where N is the number of control points and a is the drawing
     // accuracy of the spline, i.e., the number of line segments to
     // be drawn per pair of adjancent control points
     bool evaluate_opengl(Matrix& pts, const Matrix v2gl);
     
     // are knot points being drawn?
     bool draw_knots() const { return _drawKnots; };
     // draw knots whenever the draw() method is called
     void draw_knots(bool yesno) {_drawKnots = yesno;};
     // set the color for drawing the knots
     void knot_color(double r, double g, double b);
     
     // is the bspline curve being drawn?
     bool draw_curve() const { return _drawCurve; };
     // draw the bspline curve whenever the draw() method is called
     void draw_curve(bool yesno) { _drawCurve = yesno;};
     // controls the accuracy with which the
     // bspline is drawn: the bspline is drawn as a collection of line
     // segments and acc controls the number of line segments that will
     // be drawn per knot point
     // NOTE: keep the value of acc small (ie. 1 or 2) when the bspline 
     // has 100s of control points, as would happen when every pixel on
     // an object boundary is a control point. 
     void draw_accuracy(int acc);
     // set the color for drawing the bspline curve
     void curve_color(double r, double g, double b);
};


#endif
