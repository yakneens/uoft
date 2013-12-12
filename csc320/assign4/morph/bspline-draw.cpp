// WARNING: DO NOT MODIFY THIS FILE

#include <math.h>
#include <Fl/gl.h>
#include <iostream.h>
#include <newmat/newmatio.h>
#include <math.h>

#include "bspline.h"

// Drawing b-splines using OpenGL 
// 
//
// (You do not need to read or understand this file)

static Real bspline_matrix[] =
{
     -1/6.0,  1/2.0, -1/2.0, 1/6.0,
      1/2.0, -1,      1/2.0, 0,
     -1/2.0,  0,      1/2.0, 0,
      1/6.0,  2/3.0,  1/6.0, 0
};

void BSpline::draw_accuracy(int acc) {
     double u, u2;
     int i;

     if (acc > 0) {
	  _accuracy = acc; 

	  // fill the matrix containing the u-parameter values
	  // for each point on the curve
	  _uMat.ReSize(_accuracy+1, 4);
	  _uMat.Column(4) = 1;
	  for (i=1, u=0; i<=_uMat.nrows(); i++, u+=1.0/(_uMat.nrows()-1)) {
	       _uMat(i,3) = u;
	       _uMat(i,2) = u2 = u*u;
	       _uMat(i,1) = u*u2;
	  }
     }
}


// Initialize the bspline drawing function
// Needs to be called once in the program, before bsplines can be drawn
void BSpline::draw_init(void) {
     Matrix b(4,4);

     // load the B-spline matrix
     b << bspline_matrix;
     _bMat = b;

     // default knot color is yellow
     _knotCol = new double[3];
     _knotCol[0] = _knotCol[1] = 1.0; _knotCol[2] = 0;

     // default curve color is red
     _curveCol = new double[3];
     _curveCol[0] = 1.0; _curveCol[1] = _curveCol[2] = 0;

     // draw 10 line segments per knot pair
     draw_accuracy(32);

     // by default, do not draw the curve and the knots
     draw_knots(true);
     draw_curve(true);
}

// Compute a set of points on the bspline, in OpenGL coordinates
// Returns false if the bspline's endpoints have not been locked
// otherwise, returns a matrix with 2 columns and (N-5)*a+1 rows,
// where N is the number of control points and a is the drawing
// accuracy of the spline, i.e., the number of line segments to
// be drawn per pair of adjancent control points
bool BSpline::evaluate_opengl(Matrix& pts, const Matrix v2gl)
{
     int i, j, k;

     if (endpoints_locked()) {
	  // to evalute the spline, we compute points on all but the
	  // first and last segment of the spline; this is because
	  // the first and last segments spline segments are degenerate,
	  // mapping the entire segment to a single point (the bspline
	  // endpoints
	  _glPts.ReSize((_knotMat.nrows()-(2*_d)+1)*_accuracy+1, 2);

	  for (j=1, i=_d-1; i <= _knotMat.nrows()-_d-1; i++, j+=_accuracy) {
	       // compute points on the b-spline curve
	       _glPts.Rows(j, j+_accuracy) = 
		    _uMat*_bMat*(_knotMat.Rows(i,i+_d)*v2gl.t());
	  }
	  return true;
     } else 
		return false;
}

// Draw the bspline
void BSpline::draw(const Matrix v2gl) {
     int i, j;
     double glx, gly;
     Matrix Seg;     
     Matrix glKnotMat;
     
     // can draw the spline only if some knots have been specified
     if (empty())
	  return;
     
     if (endpoints_locked() && draw_curve()) {
	  // compute a set of points on the bspline
	  // and store the result in matrix _glPts
	  evaluate_opengl(_glPts, v2gl);
	  // set the drawing color
	  glColor3dv(_curveCol);
	  // draw the bspline as a sequence of line segments between
	  // points computed on the bspline
	  glBegin(GL_LINE_STRIP);
	  {	       
	       for (j=1; j <= _glPts.nrows(); j++) 
		    glVertex2d(_glPts(j,1), _glPts(j,2));
	  }
	  glEnd();
     }
     
     if (draw_knots()) {
	  // convert from vista to GL coordinates
	  glKnotMat = _knotMat*v2gl.t();

 	  // Draw the currently-defined knots, if any
 	  glColor3dv(_knotCol);
 	  glBegin(GL_LINES);
 	  {	  
 	       for (i=1; i<=_knotMat.nrows(); i++) {
 		    // convert vista coordinates to opengl coordinates
 		    // using the supplied coordinate conversion function
		    glVertex2d(glKnotMat(i,1)-3, glKnotMat(i,2));
		    glVertex2d(glKnotMat(i,1)+3, glKnotMat(i,2));
		    glVertex2d(glKnotMat(i,1), glKnotMat(i,2)-3);
		    glVertex2d(glKnotMat(i,1), glKnotMat(i,2)+3);
 	       }
 	  }
 	  glEnd();
     }
     
}

